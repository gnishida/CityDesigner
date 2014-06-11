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

#include "SimpleRoadGenerator.h"
#include "qmath.h"
#include "qmatrix4x4.h"
#ifndef Q_MOC_RUN
#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#endif
#include "../Core/UrbanGeometry.h"
#include "../Core/UrbanGeometryAttributes.h"
#include "../Core/RoadGraph.h"
#include "../Core/Util.h"

namespace utool {

RoadGraphLoop graphLoopGlobal;

SimpleRoadGenerator::SimpleRoadGenerator(float arterial_edges_width) : RoadGenerator() {
	this->arterial_edges_width = arterial_edges_width;
}

SimpleRoadGenerator::~SimpleRoadGenerator() {
}

//Generate Roads
bool SimpleRoadGenerator::generateRoadNetwork(ucore::UrbanGeometry* urbanGeometry, ucore::RoadGraph* rg) {
	SimpleRoadGraph* inRoadGraph = dynamic_cast<SimpleRoadGraph*>(rg);

	inRoadGraph->clear();

	//======== create a list of initial seeds
	std::list<roadGraphVertexDesc> initSeeds;
	std::list<roadGraphVertexDesc> newSeeds;

	generateInitialSeeds(urbanGeometry, initSeeds, inRoadGraph);

	//======== create initial edges
	ucore::Loop3D boundingPolygon = ucore::Loop3D::createRectangle(urbanGeometry->getWidth(), urbanGeometry->getDepth());
	generateInitialEdges(boundingPolygon, inRoadGraph);

	//======== start growth
	int iteCount = 0;
	int tmpPtIdx;
	bool newSeedsCreated;
	roadGraphVertexDesc tmpSeedDesc;

	//======= grow arterials
	while (!initSeeds.empty() && iteCount < 1000) {
		//std::cout << "ga ";
		tmpSeedDesc = initSeeds.front();

		tmpPtIdx = 0;

		float distToPTCenter = (urbanGeometry->getPt() - inRoadGraph->getRoadGraph()[tmpSeedDesc].getPt()).length();
		//try to expand from seed
		float refAng = urbanGeometry->getAttributes()->getFloat("orientation");
		attemptExpansion(tmpSeedDesc, inRoadGraph, newSeeds, refAng);
		newSeedsCreated = !(newSeeds.empty());

		if(newSeedsCreated){
			//append seeds in newSeeds to initSeeds
			initSeeds.splice(initSeeds.end(), newSeeds);
		}

		//remove seed from initSeeds
		initSeeds.pop_front();

		iteCount++;
	}

	//Remove dead ends
	removeDeadEnds(inRoadGraph);

	//==============================================
	//======= grow streets
	//==============================================
	std::list<roadGraphVertexDesc> initStreetSeeds;

	if(!generateInitialStreetSeeds(urbanGeometry, initStreetSeeds, inRoadGraph)){
		return false;
	}

	iteCount = 0;
	while (!initStreetSeeds.empty() && iteCount < 1000) {
		tmpSeedDesc = initStreetSeeds.front();

		bool isPointWithinLimits = false;

		if(urbanGeometry->containsPoint(inRoadGraph->getRoadGraph()[tmpSeedDesc].getPt())){
			isPointWithinLimits = true;
		}

		if(!isPointWithinLimits){
			newSeedsCreated = false;
		} else {							
			float refAng = 0.0f;
			attemptExpansion(tmpSeedDesc, inRoadGraph, newSeeds, refAng);
			newSeedsCreated = !(newSeeds.empty());	
		}

		if(newSeedsCreated){
			initStreetSeeds.splice(initStreetSeeds.end(), newSeeds);
		}

		//remove seed from initSeeds
		initStreetSeeds.pop_front();

		iteCount++;
	}

	//Remove dead ends
	removeDeadEnds(inRoadGraph);

	return true;
}

/**
 * generate initial edges.
 * this is mainly used to add edges in the bounding polygon.
 */
void SimpleRoadGenerator::generateInitialEdges(std::vector<QVector3D>& polygon, SimpleRoadGraph* inRoadGraph) {
	roadGraphVertexDesc vDescFirst;
	roadGraphVertexDesc vDesc0;
	roadGraphVertexDesc vDesc1;

	int pgonSz = polygon.size();

	if(pgonSz < 3) return;

	vDesc0 = boost::add_vertex(inRoadGraph->getRoadGraph());
	inRoadGraph->getRoadGraph()[vDesc0].setPt(polygon[0]);
	inRoadGraph->getRoadGraph()[vDesc0].setSeed(false);
	inRoadGraph->getRoadGraph()[vDesc0].setBoundingPgonVertex(true);
	inRoadGraph->getRoadGraph()[vDesc0].setWidth(arterial_edges_width);

	vDescFirst = vDesc0;

	for (int i=1; i<pgonSz; ++i) {
		vDesc1 = boost::add_vertex(inRoadGraph->getRoadGraph());
		inRoadGraph->getRoadGraph()[vDesc1].setPt(polygon[i]);
		inRoadGraph->getRoadGraph()[vDesc1].setSeed(false);
		inRoadGraph->getRoadGraph()[vDesc1].setBoundingPgonVertex(true);
		inRoadGraph->getRoadGraph()[vDesc1].setWidth(arterial_edges_width);

		//add new edge
		std::pair<roadGraphEdgeDesc, bool> edge_pair = boost::add_edge( vDesc0, vDesc1, inRoadGraph->getRoadGraph());
		inRoadGraph->getRoadGraph()[edge_pair.first].setWidth(inRoadGraph->getRoadGraph()[vDesc0].getWidth());

		vDesc0 = vDesc1;
	}
	std::pair<roadGraphEdgeDesc, bool> edge_pair = boost::add_edge( vDesc0, vDescFirst, inRoadGraph->getRoadGraph());
	inRoadGraph->getRoadGraph()[edge_pair.first].setWidth(inRoadGraph->getRoadGraph()[vDesc0].getWidth());
}

/**
 * Make a center as an initial seed, and add it as a vertex of the road graph.
 *
 * @param urbanGeometry
 * @param seeds
 * @param inRoadGraph
 */
void SimpleRoadGenerator::generateInitialSeeds(ucore::UrbanGeometry* urbanGeometry, std::list<roadGraphVertexDesc>& seeds, SimpleRoadGraph* inRoadGraph) {
	seeds.clear();

	unsigned long ranSeed = 1;
	srand(ranSeed);

	std::vector<QVector3D> seedsPositions;
	seedsPositions.push_back(urbanGeometry->getPt());

	roadGraphVertexDesc tmpSeedDesc = boost::add_vertex(inRoadGraph->getRoadGraph());

	int newRandSeed = (rand() + 188179)%714025;

	float deltaTheta = 0.0f;

	float compensatedCurvature = urbanGeometry->getAttributes()->getFloat("edgesCurvature");

	SimpleRoadGraphVertex tmpSeed(seedsPositions[0],
		urbanGeometry->getAttributes()->getFloat("edgesLengthU"),
		urbanGeometry->getAttributes()->getFloat("edgesLengthV"),
		0,
		urbanGeometry->getAttributes()->getInt("numDeparting"),
		newRandSeed,
		deltaTheta,
		true,
		false,
		urbanGeometry->getAttributes()->getFloat("edgesIrregularity"),
		compensatedCurvature,
		arterial_edges_width);

	inRoadGraph->getRoadGraph()[tmpSeedDesc].setPt(tmpSeed.getPt());
	inRoadGraph->getRoadGraph()[tmpSeedDesc].setDepartingDirections(tmpSeed.getDepartingDirections());
	inRoadGraph->getRoadGraph()[tmpSeedDesc].setDistU(tmpSeed.getDistU());
	inRoadGraph->getRoadGraph()[tmpSeedDesc].setDistV(tmpSeed.getDistV());
	inRoadGraph->getRoadGraph()[tmpSeedDesc].setRandSeed(tmpSeed.getRandSeed());
	inRoadGraph->getRoadGraph()[tmpSeedDesc].setDeltaTheta(tmpSeed.getDeltaTheta());
	inRoadGraph->getRoadGraph()[tmpSeedDesc].setSeed(tmpSeed.isSeed());
	inRoadGraph->getRoadGraph()[tmpSeedDesc].setBoundingPgonVertex(tmpSeed.isBoundingPgonVertex());
	inRoadGraph->getRoadGraph()[tmpSeedDesc].setIrregularity(tmpSeed.getIrregularity());
	inRoadGraph->getRoadGraph()[tmpSeedDesc].setCurvature(tmpSeed.getCurvature());
	inRoadGraph->getRoadGraph()[tmpSeedDesc].setWidth(tmpSeed.getWidth());

	seeds.push_back(tmpSeedDesc);
}

//===================================================
// These functions are for the routine that extracts loops from the planar graph

bool roads_isFirstVertexVisited;
std::map<int, int> roads_placeTypeFrequencyMap;
std::vector<RoadGraphLoop> *loopsPtr;
SimpleRoadGraph* roads_roadGraphPtr;
int roads_curRandSeed;

struct road_output_visitor : public boost::planar_face_traversal_visitor {
	void begin_face() {	
		graphLoopGlobal.myLoop3D.clear();
		graphLoopGlobal.myRandSeed = 0;
		roads_isFirstVertexVisited = true;		
		roads_placeTypeFrequencyMap.clear();
	}

	void end_face() {
		if (graphLoopGlobal.myLoop3D.size() > 2) {

			graphLoopGlobal.myRandSeed = roads_curRandSeed;

			//assign place type to graph loop ------------			
			std::map<int,int>::iterator it;
			int tmpMaxKey = -1;
			int tmpMaxVal = 0;
			for (it = roads_placeTypeFrequencyMap.begin() ; it != roads_placeTypeFrequencyMap.end(); it++) {
				if ((*it).second > tmpMaxVal) {
					tmpMaxKey = (*it).first;
					tmpMaxVal = (*it).second;					
				}
			}

			loopsPtr->push_back(graphLoopGlobal);			
		}			
	}
};

//Vertex visitor
struct road_vertex_output_visitor : public road_output_visitor {
	template <typename Vertex> 
	void next_vertex(Vertex v) { 	
		if (v >= 0 && v < boost::num_vertices(roads_roadGraphPtr->getRoadGraph())) {
			graphLoopGlobal.myLoop3D.push_back((roads_roadGraphPtr->getRoadGraph())[v].getPt());

			//initialize block random seed from first street node random seed
			if (roads_isFirstVertexVisited) {
				roads_isFirstVertexVisited = false;				
				roads_curRandSeed = ((roads_roadGraphPtr->getRoadGraph())[v].getRandSeed() * 4096 + 150889) % 714025;
			}
		}
	}

	template <typename Edge> 
	void next_edge(Edge e) { 
		int sIdx, tIdx;
		sIdx = boost::source(e, roads_roadGraphPtr->getRoadGraph());
		tIdx = boost::target(e, roads_roadGraphPtr->getRoadGraph());
	}
};

bool SimpleRoadGenerator::findLoopsInGraph(SimpleRoadGraph* inRoadGraph, std::vector<RoadGraphLoop>& outLoops) {	
	roads_roadGraphPtr = inRoadGraph;
	loopsPtr = &outLoops;
	loopsPtr->clear();

	bool isPlanar = false;	

	//Make sure graph is planar
	typedef std::vector<roadGraphEdgeDesc> tEdgeDescriptorVector;
	std::vector<tEdgeDescriptorVector> embedding(boost::num_vertices(inRoadGraph->getRoadGraph()));

	// Test for planarity
	if (boost::boyer_myrvold_planarity_test(boost::boyer_myrvold_params::graph = inRoadGraph->getRoadGraph(), boost::boyer_myrvold_params::embedding = &embedding[0])) {
		isPlanar = true;
	} else {
		std::cout << "ERROR: Graph could not be planarized\n";
		return false;
	}

	//Create edge index property map?	
	typedef std::map<roadGraphEdgeDesc, size_t> EdgeIndexMap;
	EdgeIndexMap mapEdgeIdx;
	boost::associative_property_map<EdgeIndexMap> pmEdgeIndex(mapEdgeIdx);		
	roadGraphEdgeIter ei, ei_end;	
	int edge_count = 0;
	for (boost::tie(ei, ei_end) = boost::edges(inRoadGraph->getRoadGraph()); ei != ei_end; ++ei) {
		mapEdgeIdx.insert(std::make_pair(*ei, edge_count++));	
	}

	//Extract blocks from road graph using boost graph planar_face_traversal	
	road_vertex_output_visitor v_vis;
	boost::planar_face_traversal(inRoadGraph->getRoadGraph(), &embedding[0], v_vis, pmEdgeIndex);

	//Clean loops, remove largest, reorient
	float curArea;
	float maxArea = -FLT_MAX;
	int maxAreaIdx = -1;
	for (int i = 0; i < outLoops.size(); ++i) {
		ucore::Loop3D cleanOutLoop;
		ucore::Polygon3D::reorientFace(outLoops.at(i).myLoop3D);
		ucore::Polygon3D::cleanLoop(outLoops.at(i).myLoop3D, cleanOutLoop, 5.0f);		
		outLoops.at(i).myLoop3D = cleanOutLoop;

		curArea = ucore::Polygon3D::computeLoopArea(outLoops.at(i).myLoop3D, true);
		if(curArea > maxArea){
			maxArea = curArea;
			maxAreaIdx = i;
		}
	}

	if (maxAreaIdx != -1) {
		outLoops.erase(outLoops.begin() + maxAreaIdx);
	}

	return true;
	//std::cout << loopsPtr->size() << " road loops extracted\n";
}

//===================================================

QVector3D SimpleRoadGenerator::getMeanLoopPoint(ucore::Loop3D& loopIn) {
	int loopInSz = loopIn.size();
	QVector3D meanPt;
	for(int i=0; i<loopInSz; ++i){
		meanPt = meanPt + loopIn.at(i);
	}
	meanPt = meanPt / (float)loopInSz;
	return meanPt;
}

//Create initial street seeds
bool SimpleRoadGenerator::generateInitialStreetSeeds(ucore::UrbanGeometry* urbanGeometry, std::list<roadGraphVertexDesc>& seeds, SimpleRoadGraph* inRoadGraph) {
	std::vector<RoadGraphLoop> outLoops;

	//extract loops from current graph
	if(!findLoopsInGraph(inRoadGraph, outLoops)){
		return false;
	}

	seeds.clear();

	unsigned long ranSeed = 1;
	srand(ranSeed);

	std::vector<QVector3D> seedsPositions;

	//======== populate initial seeds
	QVector2D newPtA;
	QVector2D newPtB;

	float locEdgesU, locEdgesV;
	float locEdgesIrregularity, locEdgesCurvature;
	float locEdgesWidth;

	for (int i = 0; i < outLoops.size(); ++i) {
		//get obb of loop
		QVector3D obbSize;
		QMatrix4x4 obbMat;
		ucore::Polygon3D::getLoopOBB(outLoops.at(i).myLoop3D, obbSize, obbMat);

		QVector3D dirVectorInit, dirVector;
		QVector3D midPt(0.0f, 0.0f, 0.0f);
		QVector3D auxPt(1.0f, 0.0f, 0.0f);

		midPt = midPt*obbMat;

		dirVectorInit = (auxPt*obbMat - midPt);
		dirVectorInit.normalize();
		if (obbSize.y() > obbSize.x()) {
			dirVector.setX( -dirVectorInit.y() );
			dirVector.setY(  dirVectorInit.x() );
		} else {
			dirVector = dirVectorInit;
		}

		newPtA = QVector2D(midPt);

		//make orientation be equal to main direction of OBB of loop
		float tmpOrient;
		float tmpDeltaTheta = 0.0f;
		tmpOrient = atan2(dirVector.y(), dirVector.x());

		newPtA = newPtA + QVector2D(ucore::Util::genRand(-2.0f, 2.0f), ucore::Util::genRand(-2.0f, 2.0f));

		roadGraphVertexDesc vDescA = boost::add_vertex(inRoadGraph->getRoadGraph());

		int newRandSeed = (rand() + 188179)%714025;

		//---------compute U and V
		locEdgesU = urbanGeometry->getAttributes()->getFloat("locEdgesLengthU") * urbanGeometry->getAttributes()->getFloat("edgesLengthU");
		locEdgesV = urbanGeometry->getAttributes()->getFloat("locEdgesLengthV") * urbanGeometry->getAttributes()->getFloat("edgesLengthV");

		float ang = tmpOrient;
		float inRef = 0.0f;
		bool isU;
		if (( fabs(fabs(ang - inRef)            ) < 0.25f*M_PI ) ||
			( fabs(fabs(ang - inRef) -      M_PI) < 0.25f*M_PI ) ||
			( fabs(fabs(ang - inRef) - 2.0f*M_PI) < 0.25f*M_PI ) ) {
			isU = true;
		} else {
			isU = false;
		}		

		if (isU) {
			float cpy = locEdgesU;
			locEdgesU = locEdgesV;
			locEdgesV = cpy;
		}

		locEdgesIrregularity = urbanGeometry->getAttributes()->getFloat("locEdgesIrregularity");
		locEdgesCurvature = urbanGeometry->getAttributes()->getFloat("locEdgesCurvature");
		locEdgesWidth = 0.5f * arterial_edges_width;

		SimpleRoadGraphVertex tmpSeed(newPtA,
			locEdgesU,
			locEdgesV,
			tmpOrient,
			4,
			newRandSeed,
			tmpDeltaTheta,
			false,
			false,
			locEdgesIrregularity,
			locEdgesCurvature,
			locEdgesWidth);

		inRoadGraph->getRoadGraph()[vDescA].setPt(tmpSeed.getPt());
		inRoadGraph->getRoadGraph()[vDescA].setDepartingDirections(tmpSeed.getDepartingDirections());
		inRoadGraph->getRoadGraph()[vDescA].setDistU(locEdgesU);
		inRoadGraph->getRoadGraph()[vDescA].setDistV(locEdgesV);
		inRoadGraph->getRoadGraph()[vDescA].setRandSeed(newRandSeed);
		inRoadGraph->getRoadGraph()[vDescA].setDeltaTheta(tmpDeltaTheta);
		inRoadGraph->getRoadGraph()[vDescA].setSeed(tmpSeed.isSeed());
		inRoadGraph->getRoadGraph()[vDescA].setBoundingPgonVertex(tmpSeed.isBoundingPgonVertex());
		inRoadGraph->getRoadGraph()[vDescA].setIrregularity(locEdgesIrregularity);
		inRoadGraph->getRoadGraph()[vDescA].setCurvature(locEdgesCurvature);
		inRoadGraph->getRoadGraph()[vDescA].setWidth(locEdgesWidth);

		seeds.push_back(vDescA);
	}

	return true;
	//std::cout << seeds.size() << " street seeds created\n";
}

/**
* Check if segment AB is redundant, i.e., if the angle between segment AB and all the edges incoming to the vertex
* with center in B is too small
* This function DOES NOT modify the graph
**/
bool SimpleRoadGenerator::isSegmentRedundant(QVector3D& a, QVector3D &b, roadGraphVertexDesc& vtxDesc, SimpleRoadGraph* inRoadGraph, float angleThreshold) {
	roadGraphAdjIter ai, ai_end;
	float ang;

	//iterate all incident edges to vtxDesc			
	for (boost::tie(ai, ai_end) = boost::adjacent_vertices(vtxDesc, inRoadGraph->getRoadGraph()); ai != ai_end; ++ai) {
		//compute angle between new potential segment and all other segments incident to vi
		ang = ucore::Util::angleThreePoints(a, b, inRoadGraph->getRoadGraph()[*ai].getPt());

		//if angle is too small, segment is redundant
		if( ang < angleThreshold )
		{
			return true;
		}
	}
	return false;
}

/**
* Attempts snapping targetVtx to the graphs 
* This function DOES NOT modify the graph
**/
bool SimpleRoadGenerator::snapsToGraphVertices(QVector3D& sourceVtxPt, QVector3D& targetVtxPt, SimpleRoadGraph* inRoadGraph, roadGraphVertexDesc& tgtVertexDesc, float threshold) {
	float distToClosest = FLT_MAX;
	float curDist;

	roadGraphVertexIter vi, viEnd;

	for (boost::tie(vi, viEnd) = boost::vertices(inRoadGraph->getRoadGraph()); vi != viEnd; ++vi) {
		//if distance between points is small
		curDist = (inRoadGraph->getRoadGraph()[*vi].getPt() - targetVtxPt).lengthSquared();
		if (curDist < distToClosest) {
			distToClosest = curDist;
			tgtVertexDesc = boost::vertex(*vi, inRoadGraph->getRoadGraph());
		}
	}

	if (distToClosest < threshold*threshold) {
		targetVtxPt = inRoadGraph->getRoadGraph()[tgtVertexDesc].getPt();
		return true;		
	}

	return false;
}

/**
* Checks if new edge will intersect an existing edge
* This function DOES NOT modify the graph
**/
bool SimpleRoadGenerator::intersectsOtherGraphEdge(QVector3D& p0, QVector3D& p1, SimpleRoadGraph* inRoadGraph, roadGraphVertexDesc& closest_ei_source_desc, roadGraphVertexDesc& closest_ei_target_desc, roadGraphEdgeIter& eiClosest, QVector3D& closestIntPt) {
	QVector2D p0_2D(p0.toVector2D());
 	QVector2D p1_2D(p1.toVector2D());
	QVector2D intPt;
	float distToInt;
	float distToClosestInt = FLT_MAX;
	float tab, tcd;
	roadGraphVertexDesc ei_source_desc, ei_target_desc;	
	bool intersect = false;

	roadGraphEdgeIter ei, eiEnd;

	//Adapt edges ===============
	for (boost::tie(ei, eiEnd) = boost::edges(inRoadGraph->getRoadGraph()); ei != eiEnd; ++ei) {
		ei_source_desc = boost::source(*ei, inRoadGraph->getRoadGraph()); 
		ei_target_desc = boost::target(*ei, inRoadGraph->getRoadGraph());

		//if new segment intersects other segment
		if (ucore::Util::segmentSegmentIntersectXY(p0_2D, p1_2D, inRoadGraph->getRoadGraph()[ei_source_desc].getPt().toVector2D(), inRoadGraph->getRoadGraph()[ei_target_desc].getPt().toVector2D(),&tab, &tcd, true, intPt)) {
			distToInt = (p0 - intPt).lengthSquared();

			//make sure we get only closest segment
			if( distToInt < distToClosestInt ){
				distToClosestInt = distToInt;
				eiClosest = ei;
				closest_ei_source_desc = ei_source_desc;
				closest_ei_target_desc = ei_target_desc;
				closestIntPt = intPt;
				intersect = true;
			}
		}
	}	

	return intersect;
}

/**
* Attempts expansion of a segment in all possible directions and adds new edges to roadGraph.
* If new seeds are found, they are added to newSeeds.
* This function DOES modify the graph
**/
void SimpleRoadGenerator::attemptExpansion(roadGraphVertexDesc& srcVertexDesc, SimpleRoadGraph* inRoadGraph, std::list<roadGraphVertexDesc>& newSeeds, float inRefAngle) {
	newSeeds.clear();

	QVector3D dirVec;
	QVector3D targetVtxPt;
	QVector3D closestIntPt;
	float theta;
	float deltaDist, deltaDistNoNoise;
	bool snap;
	bool intersects;	

	roadGraphVertexDesc tgtVertexDesc;
	roadGraphVertexDesc closest_ei_source_desc, closest_ei_target_desc;
	roadGraphEdgeIter eiClosest;

	srand(inRoadGraph->getRoadGraph()[srcVertexDesc].getRandSeed());

	int numDepDir = inRoadGraph->getRoadGraph()[srcVertexDesc].getDepartingDirections().size();

	//attempt expansion along each direction
	for (int i = 0; i < numDepDir; ++i) {
		float curIrregularity =  inRoadGraph->getRoadGraph()[srcVertexDesc].getIrregularity();

		theta = inRoadGraph->getRoadGraph()[srcVertexDesc].getDepartingDirections()[i];
		dirVec.setX(cos(theta));
		dirVec.setY(sin(theta));
		dirVec.setZ(0.0f);

		snap = false;
		intersects = false;

		bool isU;

		deltaDistNoNoise = getDistFromDirAngle(&inRoadGraph->getRoadGraph()[srcVertexDesc], theta, inRefAngle, isU);
		deltaDist = deltaDistNoNoise + ucore::Util::genRand(-0.3f*deltaDistNoNoise*curIrregularity, 0.3f*deltaDistNoNoise*curIrregularity);

		targetVtxPt = inRoadGraph->getRoadGraph()[srcVertexDesc].getPt() + dirVec*deltaDist;		

		// INTERSECTS -- If edge intersects other edge
		intersects = intersectsOtherGraphEdge(inRoadGraph->getRoadGraph()[srcVertexDesc].getPt(), targetVtxPt, inRoadGraph,
			closest_ei_source_desc, closest_ei_target_desc, eiClosest, closestIntPt);
		if (intersects) {
			targetVtxPt = closestIntPt;
		}		

		//the threshold should be the max between e.g. 1/4 of the length and e.g. 10m
		float threshold = (std::max)(0.25f*deltaDistNoNoise, 20.0f);
		//float threshold = 10.0f;

		if (!intersects) {
			// SNAPS -- attempt snapping to any other vertex
			snap = snapsToGraphVertices(inRoadGraph->getRoadGraph()[srcVertexDesc].getPt(), targetVtxPt, inRoadGraph, tgtVertexDesc, threshold);
		} else {
			// SNAPS -- attemp snapping to the eiClosest vertices
			float distToEdgeSource = (inRoadGraph->getRoadGraph()[boost::source(*eiClosest, inRoadGraph->getRoadGraph())].getPt() - targetVtxPt).length();
			float distToEdgeTarget = (inRoadGraph->getRoadGraph()[boost::target(*eiClosest, inRoadGraph->getRoadGraph())].getPt() - targetVtxPt).length();

			if (distToEdgeSource < threshold || distToEdgeTarget < threshold) {
				if(distToEdgeSource < distToEdgeTarget){
					targetVtxPt = inRoadGraph->getRoadGraph()[closest_ei_source_desc].getPt();
					tgtVertexDesc = closest_ei_source_desc;					
				} else {
					targetVtxPt = inRoadGraph->getRoadGraph()[closest_ei_target_desc].getPt();
					tgtVertexDesc = closest_ei_target_desc;
				}
				snap = true;
			}			
		}

		// ANGLE REDUNDANCY -- if departing segment is redundant
		if (isSegmentRedundant(targetVtxPt, inRoadGraph->getRoadGraph()[srcVertexDesc].getPt(), srcVertexDesc, inRoadGraph, 0.2f*M_PI)) {
			continue;
		}
		if (snap) {
			if (isSegmentRedundant(inRoadGraph->getRoadGraph()[srcVertexDesc].getPt(), targetVtxPt, tgtVertexDesc, inRoadGraph, 0.2f * M_PI)) {
				continue;
			}
		}

		if (intersects && !snap) {
			tgtVertexDesc = boost::add_vertex(inRoadGraph->getRoadGraph());

			inRoadGraph->getRoadGraph()[tgtVertexDesc].setPt(targetVtxPt);
			inRoadGraph->getRoadGraph()[tgtVertexDesc].setSeed(false);
			inRoadGraph->getRoadGraph()[tgtVertexDesc].setBoundingPgonVertex(inRoadGraph->getRoadGraph()[closest_ei_source_desc].isBoundingPgonVertex());
			inRoadGraph->getRoadGraph()[tgtVertexDesc].setWidth(inRoadGraph->getRoadGraph()[closest_ei_source_desc].getWidth());

			//add two new edges from old edge endpoints to new vertex (split edge)
			std::pair<roadGraphEdgeDesc, bool> e0_pair = boost::add_edge(  closest_ei_source_desc, tgtVertexDesc, inRoadGraph->getRoadGraph() );
			inRoadGraph->getRoadGraph()[e0_pair.first].setWidth(inRoadGraph->getRoadGraph()[closest_ei_source_desc].getWidth());

			std::pair<roadGraphEdgeDesc, bool> e1_pair = boost::add_edge(  closest_ei_target_desc, tgtVertexDesc, inRoadGraph->getRoadGraph() );				
			inRoadGraph->getRoadGraph()[e1_pair.first].setWidth(inRoadGraph->getRoadGraph()[closest_ei_source_desc].getWidth());

			//remove edge
			boost::remove_edge(*eiClosest, inRoadGraph->getRoadGraph());
		}

		int newRandSeed = (rand() + 188179)%714025;

		float curCurvature =  0.5f * inRoadGraph->getRoadGraph()[srcVertexDesc].getCurvature();
		float newDeltaTheta = inRoadGraph->getRoadGraph()[srcVertexDesc].getDeltaTheta() + curCurvature * ucore::Util::genRand(-1.0f, 1.0f);
		float refAngle = theta + newDeltaTheta;

		//Determine new deltaU and deltaV
		float newDistU;
		float newDistV;
		bool newIsSeed = false;
		bool newIsBoundingPgonVertex = false;

		newDistU = isU? deltaDist : inRoadGraph->getRoadGraph()[srcVertexDesc].getDistU();
		newDistV = isU? inRoadGraph->getRoadGraph()[srcVertexDesc].getDistV() : deltaDist;

		SimpleRoadGraphVertex targetVtx(targetVtxPt,
			//isU? deltaDist : inRoadGraph->getRoadGraph()[srcVertexDesc].distU,
			newDistU,
			//isU? inRoadGraph->getRoadGraph()[srcVertexDesc].distV : deltaDist,			
			newDistV,
			refAngle,
			4,
			newRandSeed,
			newDeltaTheta,
			newIsSeed,
			newIsBoundingPgonVertex,
			inRoadGraph->getRoadGraph()[srcVertexDesc].getIrregularity(),
			inRoadGraph->getRoadGraph()[srcVertexDesc].getCurvature(),
			inRoadGraph->getRoadGraph()[srcVertexDesc].getWidth());

		if(!snap && !intersects){
			tgtVertexDesc = boost::add_vertex(inRoadGraph->getRoadGraph());
		}

		//add edge to graph
		std::pair<roadGraphEdgeDesc, bool> tmpRGED_pair = boost::add_edge(srcVertexDesc, tgtVertexDesc, inRoadGraph->getRoadGraph());

		if(!snap && !intersects){ //if not snap, vertex must be initialized
			inRoadGraph->getRoadGraph()[tgtVertexDesc].setPt(targetVtx.getPt());
			inRoadGraph->getRoadGraph()[tgtVertexDesc].setDepartingDirections(targetVtx.getDepartingDirections());
			inRoadGraph->getRoadGraph()[tgtVertexDesc].setDistU(targetVtx.getDistU());
			inRoadGraph->getRoadGraph()[tgtVertexDesc].setDistV(targetVtx.getDistV());
			inRoadGraph->getRoadGraph()[tgtVertexDesc].setRandSeed(newRandSeed);
			inRoadGraph->getRoadGraph()[tgtVertexDesc].setDeltaTheta(newDeltaTheta);
			inRoadGraph->getRoadGraph()[tgtVertexDesc].setSeed(newIsSeed);
			inRoadGraph->getRoadGraph()[tgtVertexDesc].setBoundingPgonVertex(newIsBoundingPgonVertex);
			inRoadGraph->getRoadGraph()[tgtVertexDesc].setIrregularity(targetVtx.getIrregularity());
			inRoadGraph->getRoadGraph()[tgtVertexDesc].setCurvature(targetVtx.getCurvature());
			inRoadGraph->getRoadGraph()[tgtVertexDesc].setWidth(targetVtx.getWidth());

			//add target vertex to list of seeds
			newSeeds.push_back(tgtVertexDesc);
		}		

		inRoadGraph->getRoadGraph()[tmpRGED_pair.first].setWidth(inRoadGraph->getRoadGraph()[srcVertexDesc].getWidth());
	}
}

/**
* Remove from graph vertices of degree one and the edge they are connected to
**/
void SimpleRoadGenerator::removeDeadEnds(SimpleRoadGraph* inRoadGraph) {
	roadGraphVertexIter vi, viEnd;			
	roadGraphEdgeIter ei, eiEnd;
	roadGraphVertexDesc vDesc;

	bool deadEndsLeft = true;

	for (int i = 0; i < 5 && deadEndsLeft; ++i) {
		deadEndsLeft = false;

		//remove edges connecting to those vertices
		for (boost::tie(vi, viEnd) = boost::vertices(inRoadGraph->getRoadGraph()); vi != viEnd; ++vi) {
			vDesc = boost::vertex(*vi, inRoadGraph->getRoadGraph());

			int outDeg = boost::out_degree( vDesc, inRoadGraph->getRoadGraph());
			if (outDeg < 2) {
				boost::clear_vertex(vDesc, inRoadGraph->getRoadGraph());
				deadEndsLeft = true;
			} else if ( outDeg == 2 && !inRoadGraph->getRoadGraph()[vDesc].isBoundingPgonVertex()){
				roadGraphVertexDesc vDesc0, vDesc1;
				roadGraphAdjIter ai, ai_end;				
				int count = 0;

				for (boost::tie(ai, ai_end) = boost::adjacent_vertices(vDesc, inRoadGraph->getRoadGraph()); ai != ai_end; ++ai) {	
					if (count == 0) {
						vDesc0 = *ai;
					} else if(count == 1){
						vDesc1 = *ai;
					}
					count++;
				}

				std::pair<roadGraphEdgeDesc, bool> edge_pair = boost::add_edge( vDesc0, vDesc1, inRoadGraph->getRoadGraph() );

				inRoadGraph->getRoadGraph()[edge_pair.first].setWidth(inRoadGraph->getRoadGraph()[vDesc0].getWidth());
			}
		}
	}
}

float SimpleRoadGenerator::getDistFromDirAngle(SimpleRoadGraphVertex* roadGraphVertex, float ang, float inRef, bool &isU) {
	if (roadGraphVertex->getDepartingDirections().size() < 1) {
		return 0.0f;
	}

	if( ( fabs(fabs(ang - inRef)            ) < 0.25f*M_PI ) ||
		( fabs(fabs(ang - inRef) -      M_PI) < 0.25f*M_PI ) ||
		( fabs(fabs(ang - inRef) - 2.0f*M_PI) < 0.25f*M_PI ) )			
	{
		isU = true;
		return roadGraphVertex->getDistU();
	} else {
		isU = false;
		return roadGraphVertex->getDistV();
	}			
}

} // namespace utool