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

#include <QVector3D>
#include <vector>

namespace utool {

class ElevationGrid;

/**
* RoadGraphVertex.
**/
class SimpleRoadGraphVertex {
protected:
	/** Location of vertex */
	QVector3D pt;

	//angles
	std::vector<float> departingDirections;

	//road segmemt distances
	float distU;
	float distV;

	//random seed
	int randSeed;

	//deltaTheta
	float deltaTheta;

	//is seed
	bool _isSeed;
	bool _isBoundingPgonVertex;

	//irregularity and curvature
	float irregularity;
	float curvature;

	//width
	float width;

public:
	/** Constructor */
	SimpleRoadGraphVertex();

	SimpleRoadGraphVertex(QVector3D pt, float dU, float dV, float ref, int dirCount,
		int randSeed, float deltaTheta, bool isSeed, bool isBoundingPgonVertex,
		float inIrregularity, float inCurvature, float width);

	/**
	* Destructor.
	**/
	~SimpleRoadGraphVertex();

	/**
	* Copy constructor.
	**/
	SimpleRoadGraphVertex(const SimpleRoadGraphVertex &ref) {
		pt = ref.pt;				
		departingDirections = ref.departingDirections;
		distU = ref.distU;
		distV = ref.distV;
		randSeed = ref.randSeed;
		deltaTheta = ref.deltaTheta;
		_isSeed = ref._isSeed;
		_isBoundingPgonVertex = ref._isBoundingPgonVertex;
		irregularity = ref.irregularity;
		curvature = ref.curvature;
		width = ref.width;
	}

	/**
	* Assignment operator.
	**/

	inline SimpleRoadGraphVertex &operator=(const SimpleRoadGraphVertex &ref) {
		pt = ref.pt;				
		departingDirections = ref.departingDirections;
		distU = ref.distU;
		distV = ref.distV;
		randSeed = ref.randSeed;
		deltaTheta = ref.deltaTheta;
		_isSeed = ref._isSeed;
		_isBoundingPgonVertex = ref._isBoundingPgonVertex;
		irregularity = ref.irregularity;
		curvature = ref.curvature;
		width = ref.width;
		return (*this);
	}

	/** getter/setter for pt */
	QVector3D& getPt() { return pt; }
	void setPt(const QVector3D& pt) { this->pt = pt; }

	/** getter/setter for departingDirections */
	std::vector<float>& getDepartingDirections() { return departingDirections; }
	void setDepartingDirections(const std::vector<float>& departingDirections) { this->departingDirections = departingDirections; }

	/** getter/setter for distU */
	float getDistU() { return distU; }
	void setDistU(float distU) { this->distU = distU; }

	/** getter/setter for distV */
	float getDistV() { return distV; }
	void setDistV(float distV) { this->distV = distV; }

	/** getter/setter for randSeed */
	int getRandSeed() { return randSeed; }
	void setRandSeed(int randSeed) { this->randSeed = randSeed; }

	/** getter/setter for deltaTheta */
	float getDeltaTheta() { return deltaTheta; }
	void setDeltaTheta(float deltaTheta) { this->deltaTheta = deltaTheta; }

	/** getter/setter for isSeed */
	bool isSeed() { return _isSeed; }
	void setSeed(bool isSeed) { this->_isSeed = isSeed; }

	/** getter/setter for isBoundingPgonVertex */
	bool isBoundingPgonVertex() { return _isBoundingPgonVertex; }
	void setBoundingPgonVertex(bool isBoundingPgonVertex) { this->_isBoundingPgonVertex = isBoundingPgonVertex; }

	/** getter/setter for irregularity */
	float getIrregularity() { return irregularity; }
	void setIrregularity(float irregularity) { this->irregularity = irregularity; }

	/** getter/setter for curvature */
	float getCurvature() { return curvature; }
	void setCurvature(float curvature) { this->curvature = curvature; }

	/** getter/setter for width */
	float getWidth() { return width; }
	void setWidth(float width) { this->width = width; }

	/**
	* Initialize directions based on reference direction and direction count
	**/
	void initMyDirections(float ref, int dirCount);
};						

} // namespace utool

