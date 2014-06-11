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

#include <vector>
#include <QVector3D>

namespace utool {				

/**
* SimpleRoadGraph.
**/
class SimpleRoadGraphEdge {
protected:
	float width;

	/** Geometry of the road along this graph edge */
	std::vector<QVector3D> roadSegmentGeometry;

	/** Normalized distance from the first geometrical vertex */
	std::vector<float> normDistFirstVertex;

	/** length is computed as the sum of lengths of roadSegentGeometry */
	float edgeLength;

public:
	/** Constructor */
	SimpleRoadGraphEdge();

	/** Destructor */
	~SimpleRoadGraphEdge();

	/** Initialize */
	void init();

	/** Copy constructor */
	SimpleRoadGraphEdge(const SimpleRoadGraphEdge &ref) {
		width = ref.width;
		roadSegmentGeometry = ref.roadSegmentGeometry;
	}

	/** Assignment operator */
	inline SimpleRoadGraphEdge &operator=(const SimpleRoadGraphEdge &ref) {
		width = ref.width;
		roadSegmentGeometry = ref.roadSegmentGeometry;
		return (*this);
	}

	/** getter/setter for width */
	float getWidth() { return width; }
	void setWidth(float width) { this->width = width; }

	/** getter for roadSegmentGeometry */
	std::vector<QVector3D>& getRoadSegmentGeometry() { return roadSegmentGeometry; }

	/** getter for normDistFirstVertex */
	std::vector<float>& getNormDistFirstVertex() { return normDistFirstVertex; }

	/** getter/setter for edgeLength */
	float getEdgeLength() { return edgeLength; }
	void setEdgeLength(float edgeLength) { this->edgeLength = edgeLength; }
};						
	
} // namespace utool

