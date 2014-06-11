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

#include "SimpleRoadGraphVertex.h"
#include <qmath.h>

namespace utool {

SimpleRoadGraphVertex::SimpleRoadGraphVertex() {
}

SimpleRoadGraphVertex::SimpleRoadGraphVertex(QVector3D inPt, float dU, float dV,
	float ref, int dirCount, int inRandSeed, float inDeltaTheta, bool inIsSeed,
	bool inIsBoundingPgonVertex, float inIrregularity, float inCurvature, float inWidth) {
	pt = inPt;
	distU = dU;
	distV = dV;
	initMyDirections(ref, dirCount);
	randSeed = inRandSeed;
	deltaTheta = inDeltaTheta;
	_isSeed = inIsSeed;
	_isBoundingPgonVertex = inIsBoundingPgonVertex;
	irregularity = inIrregularity;
	curvature = inCurvature;
	width = inWidth;
}

SimpleRoadGraphVertex::~SimpleRoadGraphVertex() {
}

/**
* Initialize directions based on reference direction and direction count
**/
void SimpleRoadGraphVertex::initMyDirections(float ref, int dirCount) {
	if (dirCount < 1) {
		return;
	}
			
	float delta = (2.0f*M_PI)/((float)(dirCount));
	float newDir;
			
	for(int i=0; i<dirCount; ++i){			
		newDir = ref + ((float)i)*delta;
		if(newDir > 2.0f*M_PI){
			newDir = newDir - 2.0f*M_PI;
		}
		departingDirections.push_back(newDir);
	}			
}

} // namespace utool