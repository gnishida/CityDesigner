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

#include "SimpleRoadGraph.h"
#include "../Core/RoadGraphEdge.h"
#include "../Core/RenderableQuadList.h"
#include "../Core/TextureManager.h"
#include "../Core/Util.h"

namespace utool {

/**
* Constructor.
**/
SimpleRoadGraph::SimpleRoadGraph(float heightAboveGround) : RoadGraph(heightAboveGround) {
}

/**
* Destructor.
**/
SimpleRoadGraph::~SimpleRoadGraph() {		
}

/**
* Clear
**/
void SimpleRoadGraph::clear() {
	this->myRoadGraph.clear();
}

void SimpleRoadGraph::adaptToTerrain(ucore::Terrain* terrain) {
	// to be implemented
	setModified();
}

void SimpleRoadGraph::_generateMeshVertices(ucore::TextureManager* textureManager) {
	ucore::RenderableQuadList* renderable = new ucore::RenderableQuadList(textureManager->get("data/textures/street.jpg"));

	roadGraphEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(myRoadGraph); ei != eend; ++ei) {
		SimpleRoadGraphEdge* edge = &myRoadGraph[*ei];

		QVector3D pt1 = myRoadGraph[boost::source(*ei, myRoadGraph)].getPt();
		QVector3D pt2 = myRoadGraph[boost::target(*ei, myRoadGraph)].getPt();

		QVector3D vec = pt2 - pt1;
		vec = QVector3D(-vec.y(), vec.x(), 0.0f);
		vec.normalize();

		QVector3D p0 = pt1 + vec * edge->getWidth() / 2.0f + QVector3D(0, 0, heightAboveGround);
		QVector3D p1 = pt1 - vec * edge->getWidth() / 2.0f + QVector3D(0, 0, heightAboveGround);
		QVector3D p2 = pt2 - vec * edge->getWidth() / 2.0f + QVector3D(0, 0, heightAboveGround);
		QVector3D p3 = pt2 + vec * edge->getWidth() / 2.0f + QVector3D(0, 0, heightAboveGround);
		QVector3D normal = ucore::Util::calculateNormal(p0, p1, p2);

		//renderable2->addQuad(p0, p1, p2, p3, normal, color);
		renderable->addQuad(p0, p1, p2, p3, normal, 0, 1, 0, 1);
	}

	renderables.push_back(renderable);
}

} // namespace utool
