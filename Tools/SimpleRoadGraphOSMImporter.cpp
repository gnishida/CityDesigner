/*********************************************************************
This file is part of QtUrban.

    QtUrban is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3 of the License.

    QtUrban is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QtUrban.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

#include "SimpleRoadGraphOSMImporter.h"
#include "SimpleRoadGraph.h"
#include <QtXml/qdom.h>
#include <qfile.h>
#include <qset.h>
#include <qmap.h>
#include "../Core/Util.h"

namespace utool {

SimpleRoadGraphOSMImporter::SimpleRoadGraphOSMImporter(SimpleRoadGraph* roadGraph) : SimpleRoadGraphImporter(roadGraph) {
}


SimpleRoadGraphOSMImporter::~SimpleRoadGraphOSMImporter() {
}

void SimpleRoadGraphOSMImporter::import(QIODevice* device) {
	QFile* file = new QFile(device);//"data/map.osm");

	// 1. Check if file exists
	if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
		printf("ERROR: file does not exit.\n");
		return;
	}

	// 2. Create dom document
	QDomDocument doc("mydocument"); 
	if (!doc.setContent(file)) {         
		printf("Error: Reading document\n");
		return;      
	} 

	//Get the root element
	QDomElement docElem = doc.documentElement(); 

	// you could check the root tag name here if it matters
	QString rootTag = docElem.tagName(); // == persons
	printf("rootTag %s\n",rootTag.toAscii().constData());

	// get the node's interested in, this time only caring about person's
	QDomNodeList nodeList = docElem.elementsByTagName("way");

	/////////////////////////////
	// 3. Process each road
	int numStreets=0;
	std::vector<roadEdge> roadsOSM;
	QSet<uint> nodesOSM;
	QSet<uint> nodesMiddleOSM;
	for(int ii = 0;ii < nodeList.count(); ++ii){
		// get the current one as QDomElement
		QDomElement el = nodeList.at(ii).toElement();
		bool isStreet = false;
		bool oneWay = false;
		uint numLanes = 1;
		uint typeRoad = 1;//0 pedestrian 1 ground 2 secondary 3 primary 4 motor
		QDomNodeList nodeListTag = el.elementsByTagName("tag");
		for (int iT = 0;iT < nodeListTag.count(); iT++) {
			QDomElement elTag = nodeListTag.at(iT).toElement();
			//QString tagNam="k";
			QString attTag=elTag.attribute("k");
			// road
			if (attTag=="highway") {
				isStreet=true;
				QString attV=elTag.attribute("v");
				if (attV=="motorway"||attV=="motorway_link"||attV=="trunk"||attV=="trunk") {
					typeRoad=4;
				}
				if(attV=="primary"||attV=="primary_link"){
					typeRoad=3;
				}
				if(attV=="secondary"||attV=="secondary_link"){
					typeRoad=2;
				}
				if(attV=="residential"||attV=="tertiary"||attV=="tertiary_link"||attV=="living_street"||attV=="unclassified"||attV=="living_street"){
					typeRoad=1;
				}
				if(attV=="pedestrian"||attV=="track"||attV=="footway"){
					typeRoad=0;
				}
			}
			// one way
			if(attTag=="oneway"){
				QString attV=elTag.attribute("v");
				if(attV=="yes"){
					oneWay=true;
					//printf("One way\n");
				}
			}
			// num lanes
			if(attTag=="lanes"){
				QString attV=elTag.attribute("v");
				numLanes=attV.toInt();
			}
		}
		// it is a street
		if (isStreet == true && typeRoad != 0) {
			numStreets++;
			roadEdge tempRoad;
			tempRoad.oneWay=oneWay;
			tempRoad.numLanes=numLanes;
			tempRoad.roadType=typeRoad;
				
			// take the first and last node
			QDomNodeList nodeListNode = el.elementsByTagName("nd");
			tempRoad.source=nodeListNode.at(0).toElement().attribute("ref").toUInt();
			tempRoad.destination=nodeListNode.at(nodeListNode.size()-1).toElement().attribute("ref").toUInt();
			////
			for(int iN = 0;iN < nodeListNode.count(); iN++){//all
				uint midId=nodeListNode.at(iN).toElement().attribute("ref").toUInt();
				tempRoad.nodes.push_back(midId);
				nodesMiddleOSM.insert(midId);
			}
			//
			nodesOSM.insert(tempRoad.source);
			nodesOSM.insert(tempRoad.destination);
			roadsOSM.push_back(tempRoad);
		}
	}
	printf("numStreetsOSM %d\n",numStreets);

	/////////////////////////////
	// 4. Process each node
	std::vector<QVector3D> nodesPos;
	QMap<uint,uint> nodeIdToVertexId;//from OSM Id to Vertex Id
	QHash<uint,QVector3D> nodeMidIdToPos3D;//from OSM Id to Position 3D
	uint vertexId=0;
	QVector3D accPoint;

	QDomNodeList nodeListNodes = docElem.elementsByTagName("node");
	for (int iN = 0;iN < nodeListNodes.count(); iN++) {
		// actual nodes
		uint id=nodeListNodes.at(iN).toElement().attribute("id").toUInt();
		if (nodesOSM.find(id) != nodesOSM.end()) {//node in roadOSM
			float lat=nodeListNodes.at(iN).toElement().attribute("lat").toFloat();
			float lon=nodeListNodes.at(iN).toElement().attribute("lon").toFloat();
			QVector3D nodePos=projLatLonToWorldMercator(lat,lon,true);
			nodePos.setZ(0);
			// check if very close to other nodes
			uint nN;
			bool veryClose=false;
			for(nN=0;nN<nodesPos.size();nN++){
				if((nodesPos[nN]-nodePos).lengthSquared()<4.0f){
					veryClose=true;
					break;
				}
			}
			if (veryClose){
				nodeIdToVertexId.insert(id,nN);
			} else {
				nodeIdToVertexId.insert(id,vertexId);
				nodeMidIdToPos3D[id]=nodePos;
				nodesPos.push_back(nodePos);
				vertexId++;
				//updateMinMax(nodePos,minBox,maxBox);
				accPoint+=nodePos;
			}
		}
		// mid nodes
		if(nodesMiddleOSM.find(id)!=nodesMiddleOSM.end()){//node in nodesMiddleOSM
			float lat=nodeListNodes.at(iN).toElement().attribute("lat").toFloat();
			float lon=nodeListNodes.at(iN).toElement().attribute("lon").toFloat();
			QVector3D nodePos=projLatLonToWorldMercator(lat,lon,true);
			// check if very close to other nodes
			nodeMidIdToPos3D[id] = nodePos;
		}
	}
	accPoint/=nodesPos.size();
	printf("Num of Nodes Before %d After %u %d\n",nodesOSM.size(),vertexId,nodesPos.size());
	printf("Avg point %f %f\n",accPoint.x(),accPoint.y());
		
	/////////////////////////////
	// 5. Convert to new Graph
	roadGraph->getRoadGraph().clear();
	
	// 5.1 Add vertex
	for (int nN = 0; nN < nodesPos.size(); nN++) {
		nodesPos[nN] -= accPoint;//less accPoint
		//roadGraphVertexDesc vDesc0 = boost::add_vertex(roadGraph->myRoadGraph_BI);
		//roadGraph->myRoadGraph_BI[vDesc0].pt=nodesPos[nN];//QVector3D(0,0,15.0f);
	}
	numStreets=0;
	float totalRoadLength=0;
	for (int nR = 0; nR < roadsOSM.size(); nR++) {
		// start and end
		uint sourceId=nodeIdToVertexId[roadsOSM[nR].source];
			
		float totalEdgeLength=0;
		std::vector<QVector3D> tempRoadGeometry;

		for (int iN = 0; iN < roadsOSM[nR].nodes.size(); iN++) {
			QVector3D pos=nodeMidIdToPos3D[roadsOSM[nR].nodes[iN]];
			pos-=accPoint;
			tempRoadGeometry.push_back(pos);
				
			if (tempRoadGeometry.size() > 1) {
				//// check if pos is a vertex
				bool pointIsNode=false;
				int nN;
				for (nN = 0; nN < nodesPos.size(); nN++) {
					//if((nodesPos[nN].x()-pos.x()).lengthSquared()<2.0f){
					if(((nodesPos[nN].x()-pos.x())*(nodesPos[nN].x()-pos.x())+(nodesPos[nN].y()-pos.y())*(nodesPos[nN].y()-pos.y()))<2.0f){
						pointIsNode=true;
						break;
					}
				}
				totalEdgeLength+=(tempRoadGeometry[tempRoadGeometry.size()-1]-tempRoadGeometry[tempRoadGeometry.size()-2]).length();
				if (pointIsNode) {
					if (sourceId == nN) {//just same nodes
						totalEdgeLength=0;
						continue;
					}

					//printf("[%d] numLanes %d type %u\n",nR,roadsOSM[nR].numLanes,roadsOSM[nR].roadType);
					//segment geometry
					totalRoadLength += (totalEdgeLength*roadsOSM[nR].numLanes);

					totalEdgeLength = 0;
					sourceId = nN;//new sourceId is the last one
					numStreets++;
				}
			}
		}
	}
	printf("numStreets %d Nodes %d Length %f\n",numStreets,nodeIdToVertexId.size()+nodeMidIdToPos3D.size(),totalRoadLength);

	/*
	////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////
	// 7. Process parks
	int numParks = 0;
	std::vector<buildingInfoStruct> parksOSM;
	QSet<uint> nodesParksOSM;
	QDomNodeList nodeListParks = docElem.elementsByTagName("way");
	for (int ii = 0;ii < nodeListParks.count(); ii++) {
		// get the current one as QDomElement
		QDomElement el = nodeListParks.at(ii).toElement();
		bool isPark=false;
		QDomNodeList nodeListTag = el.elementsByTagName("tag");
		for(int iT = 0;iT < nodeListTag.count(); iT++){
			QDomElement elTag = nodeListTag.at(iT).toElement();
			//QString tagNam="k";
			QString attTag=elTag.attribute("k");
			if(attTag=="leisure"){
				QString attV=elTag.attribute("v");
				if(attV=="park"){
					isPark=true;
					break;
				}
			}
		}
		// it is a building
		if (isPark) {
			numParks++;
			buildingInfoStruct tempBldg;
			QDomNodeList nodeListNode = el.elementsByTagName("nd");
			for(int iN = 0;iN < nodeListNode.count(); iN++){//all
				uint midId=nodeListNode.at(iN).toElement().attribute("ref").toUInt();
				tempBldg.nodes.push_back(midId);
				nodesParksOSM.insert(midId);
			}
			parksOSM.push_back(tempBldg);
		}
	}
	printf("numBuildings %d\n",numParks);
	////////////////////////////////////
	// 6.2 Convert ids to positions
	{
		QHash<uint,QVector3D> nodeBldgIdToPos3D;
		QDomNodeList nodeListNodesBuilding = docElem.elementsByTagName("node");
		for(int iN = 0;iN < nodeListNodesBuilding.count(); iN++){
			// actual nodes
			uint id=nodeListNodesBuilding.at(iN).toElement().attribute("id").toUInt();
			if(nodesParksOSM.find(id)!=nodesOSM.end()){//nodes in nodesBuildingOSM
				float lat=nodeListNodes.at(iN).toElement().attribute("lat").toFloat();
				float lon=nodeListNodes.at(iN).toElement().attribute("lon").toFloat();
				QVector3D nodePos=projLatLonToWorldMercator(lat,lon,true);
				nodePos.setZ(0);
				nodePos-=accPoint;
				nodeBldgIdToPos3D.insert(id,nodePos);
			}
		}
		////////////////////////////////////
		// 6.3 Create parks
		createParksFromOSM(parksOSM,nodeBldgIdToPos3D);
		///////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////
		// 7. Terrain

		polygon tPoly;
		for(int nN=0;nN<nodesPos.size();nN++){
			boost::geometry::append(tPoly,mPoint (nodesPos[nN].x(),nodesPos[nN].y()) );
		}
			
		polygon hull;
		polygonP hullP;
		boost::geometry::convex_hull(tPoly, hull);
		std::vector<pointP> hullPPoints;
		float zVal=LC::misctools::Global::global()->boundingPolygon[0].z();
		LC::misctools::Global::global()->boundingPolygon.clear();
		for(int hN=0;hN<hull.outer().size();hN++){
			QVector3D pHu=QVector3D(hull.outer()[hN].x(),hull.outer()[hN].y(),zVal);
			LC::misctools::Global::global()->boundingPolygon.push_back(pHu);
			pointP hP=boost::polygon::construct<pointP>(hull.outer()[hN].x(),hull.outer()[hN].y());
			hullPPoints.push_back(hP);
		}
		boost::polygon::set_points(hullP,hullPPoints.begin(),hullPPoints.end());
		using namespace boost::polygon::operators;
		blockSet+=hullP;
		///////////////////////////////////////////////////////////////////////////////////////
		//////////////////
		// 7. Blocks
		bool blockUsingBoost=false;
		bool blockUsinRender=true;
		if(blockUsingBoost==true){
			generateBlocksUsingBoost(inRoadGraph);

		}
		if(blockUsinRender==true){
			generateBlocksUsingRender();
		}
		printf("numBlocks %d\n",blockVec.size());
	}

	printf("<<readRoadNetworkFromOSM\n");
	*/
}


QVector3D SimpleRoadGraphOSMImporter::projLatLonToWorldMercator(float lat,float lon,bool isDeg) {
	// Ellipsoid model constants (actual values here are for WGS84) 
	const float sm_a = 6378137.0f; 
	const float sm_b = 6356752.314f;

	QVector3D result;
	float lon0 = 0;
	if (isDeg==true){
		lat = lat/180.0f * M_PI;
		lon = lon/180.0f * M_PI;
	}
	result.setX(sm_a*(lon-lon0));
	result.setY(sm_a*log((sin(lat)+1)/cos(lat)));

	return  result; 
}//


} // namespace utool