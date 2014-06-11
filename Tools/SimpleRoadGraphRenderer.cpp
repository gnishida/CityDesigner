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

#include "SimpleRoadGraphRenderer.h"
#include "../Core/TextureManager.h"
#include "../Core/RoadGraph.h"
#include "../Core/Renderable.h"
#include "../Core/Texture.h"
#include "SimpleRoadGraph.h"

namespace utool {

SimpleRoadGraphRenderer::SimpleRoadGraphRenderer() {
}

SimpleRoadGraphRenderer::~SimpleRoadGraphRenderer() {
}

void SimpleRoadGraphRenderer::_render(ucore::GeometryObject* object, ucore::TextureManager* textureManager) {
	for (int i = 0; i < object->getNumRenderables(); ++i) {
		ucore::Renderable* renderable = object->getRenderable(i);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderable->getTexture()->getId());

		glBegin(renderable->getGlBeginMode());
		for (int j = 0; j < renderable->getNumMeshVertices(); ++j) {
			glNormal3f(renderable->getMeshVertex(j).normal[0], renderable->getMeshVertex(j).normal[1], renderable->getMeshVertex(j).normal[2]);
			glTexCoord2f(renderable->getMeshVertex(j).tex[0], renderable->getMeshVertex(j).tex[1]);
			glVertex3f(renderable->getMeshVertex(j).location[0], renderable->getMeshVertex(j).location[1], renderable->getMeshVertex(j).location[2]);
		}
		glEnd();

		glDisable(GL_TEXTURE_2D);
	}
}

/*void RoadGraphRenderer::renderRoadGraphVertices(RoadGraph* roadGraph) {
	roadGraphVertexIter vi, viEnd;

	//Render vertices ============			
	glEnable(GL_POINT_SMOOTH);
			
	glColor3i(0x00, 0xFF, 0x7F);
			
	glPointSize(10.0f);

	float rVal, gVal, bVal;
	rVal = 1.0;
	gVal = 0.0;
	bVal = 0.0;
	glColor3f(rVal, gVal, bVal);

	glBegin(GL_POINTS);
	int countVertex=0;
	float deltaZ=0.25f;
	for (boost::tie(vi, viEnd) = boost::vertices(roadGraph->getRoadGraph()); vi != viEnd; ++vi) {
		glVertex3f(roadGraph->getRoadGraph()[*vi].getPt().x(), roadGraph->getRoadGraph()[*vi].getPt().y(), roadGraph->getRoadGraph()[*vi].getPt().z()+deltaZ);
		countVertex++;
	}
	glEnd();	

	glPointSize(1.0f);
}*/

} // namespace utool