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

#include "SimpleRoadGraphOSMExporter.h"

namespace utool {

SimpleRoadGraphSOMExporter::SimpleRoadGraphSOMExporter(SimpleRoadGraph* roadGraph) : SimpleRoadGraphExporter(roadGraph) {
}

SimpleRoadGraphSOMExporter::~SimpleRoadGraphSOMExporter() {
}

void SimpleRoadGraphSOMExporter::export(QIODevice* device) {
	/*
	QTextStream osmStream(device);

	roadGraphVertexIter vi, viEnd;
	roadGraphEdgeIter ei, eiEnd;
			
	int currentWayID = 1;  //index for current id

	// write file header
	osmStream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	osmStream << "<osm version=\"0.6\" generator=\"CGImap 0.0.2\">\n";
			
	// Temp file to store ways to be printed at end of osm file
	QTemporaryFile wayText("temp.txt");
	if (!wayText.open()){
		std::cout << "ERROR: Cannot open temporary file";
		return;
	}

	QTextStream wayStream(&wayText);

	osmStream.setRealNumberPrecision(7);
	wayStream.setRealNumberPrecision(7);
	osmStream.setRealNumberNotation(QTextStream::FixedNotation);
	wayStream.setRealNumberNotation(QTextStream::FixedNotation);
			
	for (boost::tie(ei, eiEnd) = boost::edges(roadGraph->getRoadGraph()); ei != eiEnd; ++ei) {
		// store way to print below
		wayStream << "<way id=\"" << currentWayID << "\">\n";

		// manually put start and end nodes into roadSegmentGeometry (until roadSegmentGeometry is fully implemented)
		roadGraph->getRoadGraph()[*ei].roadSegmentGeometry.push_back(roadGraph->getRoadGraph()[boost::source(*ei, roadGraph->getRoadGraph())].pt);
		roadGraph->getRoadGraph()[*ei].roadSegmentGeometry.push_back(roadGraph->getRoadGraph()[boost::target(*ei, roadGraph->getRoadGraph())].pt);

		// Go through geometry nodes and print them
		for (int i = 0; i < roadGraph->getRoadGraph()[*ei].roadSegmentGeometry.size(); i++) {
			//print nodes to top of file
			osmStream << "<node id=\"" << currentWayID << i 
				<< "\" lat=\"" << roadGraph->getRoadGraph()[*ei].roadSegmentGeometry.at(i).y()
				<< "\" lon=\"" << roadGraph->getRoadGraph()[*ei].roadSegmentGeometry.at(i).x()  << "\">\n"; //must flip y-value for CE
			osmStream << "<tag k=\"ele\" v=\"" << roadGraph->getRoadGraph()[*ei].roadSegmentGeometry.at(i).z() << "\"/>\n";
			osmStream << "</node>\n";

			// store nd to print in way below
			wayStream << "<nd ref=\"" << currentWayID << i << "\"/>\n";
		}

		double width = roadGraph->getRoadGraph()[*ei].roadSegmentWidth;

		// Map road width to highway type (can be changed, based on CE OSM import)
		if (width >= 11) {
			wayStream << "<tag k=\"highway\" v=\"primary\"/>\n";
			width = width - 4; //Add space for sidewalks of width 2 (default on CE OSM Import)
		} else if (width >= 9) {
			wayStream << "<tag k=\"highway\" v=\"secondary\"/>\n";
			width = width - 4; //Add space for sidewalks of width 2 (default on CE OSM Import)
		} else if (width >= 8) {
			wayStream << "<tag k=\"highway\" v=\"tertiary\"/>\n";
			width = width - 4; //Add space for sidewalks of width 2 (default on CE OSM Import)
			wayStream << "<tag k=\"type\" v=\"MINOR\"/>\n"; //type defaults to MAJOR
		} else {
			wayStream << "<tag k=\"highway\" v=\"unknown\"/>";
			wayStream << "<tag k=\"type\" v=\"MINOR\"/>\n";
		}
		
		// write tag for road segment width
		wayStream << "<tag k=\"width\" v=\"" << width << "\"/>\n";

		wayStream << "</way>\n";	

		currentWayID++;
	}

	wayText.close();
						
	//print ways
	wayText.open();
	osmStream << wayStream.readAll();
	wayText.close();

	//write file footer
	osmStream << "</osm>";
	*/
}

} // namespace utool