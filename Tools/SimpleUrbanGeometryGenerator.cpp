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

#include "SimpleUrbanGeometryGenerator.h"
#include "../Core/UrbanGeometry.h"
#include "SimpleRoadGenerator.h"
#include "SimpleBlockGenerator.h"
#include "../Core/ParcelGenerator.h"
#include "../Core/BuildingGenerator.h"
#include "../Core/VegetationGenerator.h"

namespace utool {

SimpleUrbanGeometryGenerator::SimpleUrbanGeometryGenerator(ucore::UrbanMain* urbanMain, ucore::UrbanGeometry* urbanGeometry) : ucore::UrbanGeometryGenerator(urbanMain, urbanGeometry) {
}

SimpleUrbanGeometryGenerator::~SimpleUrbanGeometryGenerator() {
}

void SimpleUrbanGeometryGenerator::generate() {
	SimpleRoadGenerator rg(18.5f);
	rg.generateRoadNetwork(urbanGeometry, urbanGeometry->getRoadGraph());

	SimpleBlockGenerator bg(urbanGeometry);
	bg.generateBlocks(urbanGeometry->getRoadGraph(), urbanGeometry->getBlocks());

	ucore::ParcelGenerator pg(urbanGeometry);
	pg.generateParcels(urbanGeometry->getBlocks());

	ucore::BuildingGenerator dg(3.0f);
	dg.generateBuildings(urbanGeometry, urbanGeometry->getBlocks());

	ucore::VegetationGenerator vg(urbanGeometry);
	vg.generateVegetation(urbanGeometry->getBlocks(), urbanGeometry->getVegetations());
}

} // namespace utool