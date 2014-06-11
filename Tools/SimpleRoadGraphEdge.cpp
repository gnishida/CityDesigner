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

#include "SimpleRoadGraphEdge.h"

namespace utool {
		
SimpleRoadGraphEdge::SimpleRoadGraphEdge() {
}

SimpleRoadGraphEdge::~SimpleRoadGraphEdge() {
	roadSegmentGeometry.clear();
}

/**
* Initialize.
**/
void SimpleRoadGraphEdge::init() {
	normDistFirstVertex.clear();

	unsigned int nVertices = roadSegmentGeometry.size();
	printf("init edge\n");
	edgeLength = 0;
	std::vector<float> segmentCumulativeTempLengths;
	for (unsigned int i = 0; i < nVertices - 1; ++i) {
		QVector3D v(
			roadSegmentGeometry[i].x() - roadSegmentGeometry[i + 1].x(),
			roadSegmentGeometry[i].y() - roadSegmentGeometry[i + 1].y(),
			0);	
		edgeLength += v.length();
		segmentCumulativeTempLengths.push_back(edgeLength);
	}
	//Distance from first vertex to first vertex is 0
	normDistFirstVertex.push_back(0);
	for (unsigned int i = 1; i < nVertices; ++i) {
		normDistFirstVertex.push_back(segmentCumulativeTempLengths[i - 1] / edgeLength);
	}			
}

} // namespace utool