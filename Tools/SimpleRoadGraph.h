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

#include "../Core/RoadGraph.h"
#include "../Core/Terrain.h"
#include <vector>
#include <QGLWidget>
#include <QSettings>
//#include <QtOpenGL>
#ifndef Q_MOC_RUN
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/copy.hpp>
#endif

#include "SimpleRoadGraphEdge.h"
#include "SimpleRoadGraphVertex.h"

#include <utility>

namespace utool {
		
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, SimpleRoadGraphVertex, SimpleRoadGraphEdge> roadBGLGraph;

typedef boost::graph_traits<roadBGLGraph>::vertex_descriptor roadGraphVertexDesc;
typedef boost::graph_traits<roadBGLGraph>::edge_descriptor roadGraphEdgeDesc;

typedef boost::graph_traits<roadBGLGraph>::vertex_iterator roadGraphVertexIter;
typedef boost::graph_traits<roadBGLGraph>::edge_iterator roadGraphEdgeIter;
typedef boost::graph_traits<roadBGLGraph>::adjacency_iterator roadGraphAdjIter;

/**
* RoadGraph.
**/
class SimpleRoadGraph : public ucore::RoadGraph {
protected:
	/** Adjacency list for road graph */
	roadBGLGraph myRoadGraph;

public:
	SimpleRoadGraph(float heightAboveGround);
	~SimpleRoadGraph();

	void clear();

	/** Copy constructor */
	/*SimpleRoadGraph(const SimpleRoadGraph& ref) {	
		boost::copy_graph(ref.myRoadGraph, myRoadGraph);
	}*/
			
	/** Assignment operator */
	/*inline SimpleRoadGraph& operator=(const SimpleRoadGraph& ref) {	
		boost::copy_graph(ref.myRoadGraph, myRoadGraph);
		return (*this);
	}*/

	/** getter/setter for myRoadGraph */
	roadBGLGraph& getRoadGraph() { return myRoadGraph; }

	inline void resetRoadGraph() {
		myRoadGraph.clear();
	}

	void adaptToTerrain(ucore::Terrain* terrain);

private:
	void _generateMeshVertices(ucore::TextureManager* textureManager);
};						

} // namespace utool
