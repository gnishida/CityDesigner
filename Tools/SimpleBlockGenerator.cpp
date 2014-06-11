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

#include "SimpleBlockGenerator.h"
#include "../Core/Block.h"
#include "../Core/RoadGraph.h"
#include "SimpleRoadGraph.h"
#include "../Core/UrbanGeometry.h"
#include "../Core/Util.h"
#ifndef Q_MOC_RUN
#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#endif
namespace utool {

SimpleRoadGraph* roadGraphPtr;
std::vector<ucore::Block*>* blocksPtr;
ucore::Loop3D blockContourTmp;
std::vector<float> blockContourWidths;
bool isFirstVertexVisited;

int curRandSeed;

//Vertex visitor
struct vertex_output_visitor : public boost::planar_face_traversal_visitor {
	void begin_face() {
		//std::cout << "New face: ";
		blockContourTmp.clear();
		blockContourWidths.clear();
		isFirstVertexVisited = true;
		curRandSeed = 0;
	}

	void end_face() {
		if(blockContourTmp.size() > 2){
			ucore::Block* newBlock = new ucore::Block();
			newBlock->setContour(blockContourTmp);		
			newBlock->setRoadWidths(blockContourWidths);
			newBlock->setRandSeed(curRandSeed);
			
			blocksPtr->push_back(newBlock);
		}
	}

	template <typename Vertex> 
	void next_vertex(Vertex v) {
		blockContourTmp.push_back((roadGraphPtr->getRoadGraph())[v].getPt());
			
		//initialize block random seed from first street node random seed
		if (isFirstVertexVisited) {
			isFirstVertexVisited = false;
			//curRandSeed = (roadGraphPtr->getRoadGraph())[v].randSeed;
			curRandSeed = ((roadGraphPtr->getRoadGraph())[v].getRandSeed() * 4096 + 150889) % 714025;
		}
	}

	template <typename Edge> 
	void next_edge(Edge e) {
		blockContourWidths.push_back(0.5f * (roadGraphPtr->getRoadGraph())[e].getWidth());
	}
};

SimpleBlockGenerator::SimpleBlockGenerator(ucore::UrbanGeometry* urbanGeometry) : BlockGenerator(urbanGeometry) {
}

SimpleBlockGenerator::~SimpleBlockGenerator() {
}

/**
* Given a road network, this function extracts the blocks
**/
bool SimpleBlockGenerator::generateBlocks(ucore::RoadGraph* rg, std::vector<ucore::Block*>& blocks) {
	SimpleRoadGraph* roadGraph = dynamic_cast<SimpleRoadGraph*>(rg);

	// clean up memory allocated for blocks
	for (int i = 0; i < blocks.size(); ++i) {
		delete blocks[i];
	}
	blocks.clear();

	roadGraphPtr = roadGraph;
	blocksPtr = &blocks;
	blocksPtr->clear();

	bool isPlanar = false;
	bool converges = true;

	//maxPlaceTypeIdx = 1;

	//Make sure graph is planar
	typedef std::vector<roadGraphEdgeDesc > tEdgeDescriptorVector;
	std::vector<tEdgeDescriptorVector> embedding(boost::num_vertices(roadGraph->getRoadGraph()));

	int cont=0;
	while (!isPlanar && converges) {	
		if (cont>2) {
			return false;
		}
		// Test for planarity		
		if (boost::boyer_myrvold_planarity_test(boost::boyer_myrvold_params::graph = roadGraph->getRoadGraph(), boost::boyer_myrvold_params::embedding = &embedding[0])) {
				isPlanar = true;
		} else {			
			//Remove intersecting edges
			if (!removeIntersectingEdges(roadGraph)) {
				converges = false;
			}
		}
		cont++;
	}

	if (!isPlanar) {
		std::cout << "ERROR: Graph could not be planarized (generateBlocks)\n";
		return false;
	}

	//Create edge index property map?	
	typedef std::map<roadGraphEdgeDesc, size_t> EdgeIndexMap;
	EdgeIndexMap mapEdgeIdx;
	boost::associative_property_map<EdgeIndexMap> pmEdgeIndex(mapEdgeIdx);		
	roadGraphEdgeIter ei, ei_end;	
	int edge_count = 0;
	for (boost::tie(ei, ei_end) = boost::edges(roadGraph->getRoadGraph()); ei != ei_end; ++ei) {
		mapEdgeIdx.insert(std::make_pair(*ei, edge_count++));	
	}

	//Extract blocks from road graph using boost graph planar_face_traversal
	vertex_output_visitor v_vis;	
	boost::planar_face_traversal(roadGraph->getRoadGraph(), &embedding[0], v_vis, pmEdgeIndex);

	int maxVtxCount = 0;
	int maxVtxCountIdx = -1; 
	float avgInsetArea = 0.0f;

	int numBadBlocks = 0;

	std::vector<float> blockAreas;

	ucore::Loop3D blockContourInset;
	for (int i = 0; i < blocks.size(); ++i) {
		//Reorient faces
		blocks[i]->getContour().reorientFace();
		//Polygon3D::reorientFace(blocks[i].getContour().getContour());

		//fix block geometry before calling function...
		ucore::Loop3D cleanPgon;
		ucore::Polygon3D::cleanLoop(blocks[i]->getContour().getContour(), cleanPgon, 5.0f);		

		//update widths			
		if (blocks[i]->getContour().getContour().size() != cleanPgon.size()) {
			size_t cleanPgonSz = cleanPgon.size();
			std::vector<float> cleanWidths(cleanPgonSz);

			for (int j = 0; j < cleanPgonSz; ++j) {
				//find element j in from clean polygon in original polygon
				//if element IS there, add to clean width array
				for (int k = 0; k < blocks[i]->getContour().getContour().size(); ++k) {
					if (cleanPgon[j] == blocks[i]->getContour().getContour()[k]) {
						cleanWidths[(j-1+cleanPgonSz)%cleanPgonSz] = blocks[i]->getRoadWidths()[k];
						break;
					}			
				}
			}

			blocks[i]->setContour(cleanPgon);
			blocks[i]->setRoadWidths(cleanWidths);
		}

		if (blocks[i]->getContour().getContour().size() != blocks[i]->getRoadWidths().size()) {
			std::cout << "Error: " << blocks[i]->getContour().getContour().size() << " " << blocks[i]->getRoadWidths().size() << "\n";
			blocks[i]->getContour().getContour().clear();
			blockAreas.push_back(0.0f);
			numBadBlocks++;
			continue;
		}

		if(blocks[i]->getContour().getContour().size() < 3){
			blockAreas.push_back(0.0f);
			numBadBlocks++;
			continue;
		}

		float insetArea = blocks[i]->getContour().computeInset(blocks[i]->getRoadWidths(), blockContourInset);
		
		blocks[i]->setContour(blockContourInset);

		avgInsetArea += insetArea;
		blockAreas.push_back(insetArea);
	}
	avgInsetArea = avgInsetArea/ ( (float)(blockAreas.size() - numBadBlocks));

	//Remove the largest block
	float maxArea = -FLT_MAX;
	int maxAreaIdx = -1;
	for (int i = 0; i < blocks.size(); ++i) {
		if (blocks[i]->getContour().getContour().size() < 3) {
			continue;
		}
		//std::cout << "area: " << blockAreas[i] << "\n";
		if (blockAreas[i] > maxArea) {
			maxArea = blockAreas[i];
			maxAreaIdx = i;
		}
	}

	if (maxAreaIdx != -1) {
		blocks.erase(blocks.begin() + maxAreaIdx);
		blockAreas.erase(blockAreas.begin() + maxAreaIdx);
	}

	return true;
}

/**
* Remove intersecting edges of a graph
**/
bool SimpleBlockGenerator::removeIntersectingEdges(SimpleRoadGraph* roadGraph) {
	std::vector<roadGraphEdgeIter> edgesToRemove;

	QVector2D a0, a1, b0, b1;
	QVector2D intPt;
	roadGraphEdgeIter a_ei, a_ei_end;
	roadGraphEdgeIter b_ei, b_ei_end;
	float ta0a1, tb0b1;

	for(boost::tie(a_ei, a_ei_end) = boost::edges(roadGraph->getRoadGraph()); a_ei != a_ei_end; ++a_ei){
		a0 = QVector2D(roadGraph->getRoadGraph()[boost::source(*a_ei, roadGraph->getRoadGraph())].getPt());
		a1 = QVector2D(roadGraph->getRoadGraph()[boost::target(*a_ei, roadGraph->getRoadGraph())].getPt());

		//for(tie(b_ei, b_ei_end) = boost::edges(roadGraph.getRoadGraph()); b_ei != b_ei_end; ++b_ei){
		for(b_ei = a_ei; b_ei != a_ei_end; ++b_ei){			

			if(b_ei != a_ei){
				b0 = QVector2D(roadGraph->getRoadGraph()[boost::source(*b_ei, roadGraph->getRoadGraph())].getPt());
				b1 = QVector2D(roadGraph->getRoadGraph()[boost::target(*b_ei, roadGraph->getRoadGraph())].getPt());

				if (ucore::Util::segmentSegmentIntersectXY(a0, a1, b0, b1, &ta0a1, &tb0b1, true, intPt) ){
					bool addEd=true;
					for(int eN=0;eN<edgesToRemove.size();eN++){
						if(edgesToRemove[eN]==b_ei){
							addEd=false;
							break;
						}
					}
					if(addEd)
						edgesToRemove.push_back(b_ei);
				}
			}
		}		
	}
	
	for(int i=0; i<edgesToRemove.size(); ++i) {	
		boost::remove_edge(*(edgesToRemove[i]), roadGraph->getRoadGraph());
	}

	if(edgesToRemove.size()>0){
		return true;
	} else {
		return false;
	}
}

} // namespace utool