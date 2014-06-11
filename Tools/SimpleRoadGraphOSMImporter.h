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

#include "SimpleRoadGraphImporter.h"
#include <QVector3D>
#include <qiodevice.h>
#include <boost/geometry/algorithms/convex_hull.hpp>
#include <boost/polygon/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

namespace utool {

typedef boost::geometry::model::d2::point_xy<double> mPoint;
typedef boost::geometry::model::polygon<mPoint> polygon;

typedef boost::polygon::point_data<float> pointP;
typedef boost::polygon::polygon_set_data<int> polygon_setP;
typedef boost::polygon::polygon_with_holes_data<int> polygonP;
typedef std::pair<pointP, pointP> edgeP;
typedef std::vector<boost::polygon::polygon_data<int> > PolygonSetP;

struct roadEdge {
	uint source;
	uint destination;
	std::vector<uint> nodes;

	uint numLanes;
	uint roadType;
	bool oneWay;
};

struct buildingInfoStruct{
	std::vector<uint> nodes;
};

class SimpleRoadGraph;

class SimpleRoadGraphOSMImporter : public SimpleRoadGraphImporter {
public:
	SimpleRoadGraphOSMImporter(SimpleRoadGraph* roadGraph);
	~SimpleRoadGraphOSMImporter();

	void import(QIODevice* device);

private:
	QVector3D projLatLonToWorldMercator(float lat,float lon,bool isDeg = false);
};

} // namespace utool