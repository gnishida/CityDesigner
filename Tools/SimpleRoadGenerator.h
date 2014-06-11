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

#pragma once

#include "../Core/RoadGenerator.h"
#include <process.h>
#include "SimpleRoadGraph.h"
#include "../Core/Polygon3D.h"
#include <qvector3d.h>

namespace utool {

class UrbanGeometry;

struct RoadGraphLoop {
	ucore::Loop3D myLoop3D;
	int myRandSeed;
};

class SimpleRoadGenerator : public ucore::RoadGenerator {
protected:
	float arterial_edges_width;

public:
	SimpleRoadGenerator(float arterial_edges_width);
	~SimpleRoadGenerator();

	bool generateRoadNetwork(ucore::UrbanGeometry* urbanGeometry, ucore::RoadGraph* roadGraph);

private:
	void generateInitialEdges(std::vector<QVector3D>& polygon, SimpleRoadGraph* inRoadGraph);
	void generateInitialSeeds(ucore::UrbanGeometry* urbanGeometry, std::list<roadGraphVertexDesc>& seeds, SimpleRoadGraph* inRoadGraph);
	bool findLoopsInGraph(SimpleRoadGraph* inRoadGraph, std::vector<RoadGraphLoop>& outLoops);
	QVector3D getMeanLoopPoint(ucore::Loop3D& loopIn);
	bool generateInitialStreetSeeds(ucore::UrbanGeometry* urbanGeometry, std::list<roadGraphVertexDesc>& seeds, SimpleRoadGraph* inRoadGraph);
	bool isSegmentRedundant(QVector3D& a, QVector3D& b, roadGraphVertexDesc& vtxDesc, SimpleRoadGraph* inRoadGraph, float angleThreshold);
	bool snapsToGraphVertices(QVector3D& sourceVtxPt, QVector3D& targetVtxPt, SimpleRoadGraph* inRoadGraph, roadGraphVertexDesc& tgtVertexDesc, float threshold = 1.0f);
	bool intersectsOtherGraphEdge(QVector3D& p0, QVector3D& p1, SimpleRoadGraph* inRoadGraph, roadGraphVertexDesc& closest_ei_source_desc, roadGraphVertexDesc& closest_ei_target_desc, roadGraphEdgeIter& eiClosest, QVector3D& closestIntPt);
	void attemptExpansion(roadGraphVertexDesc& srcVertexDesc, SimpleRoadGraph* inRoadGraph, std::list<roadGraphVertexDesc>& newSeeds, float inRefAngle);
	void removeDeadEnds(SimpleRoadGraph* inRoadGraph);
	float getDistFromDirAngle(SimpleRoadGraphVertex* roadGraphVertex, float ang, float inRef, bool &isU);
};

} // namespace utool
