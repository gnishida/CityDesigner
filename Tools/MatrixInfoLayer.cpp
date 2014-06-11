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

#include "MatrixInfoLayer.h"
#include "../Core/Util.h"

namespace utool {

MatrixInfoLayer::MatrixInfoLayer(QString name, int width, int depth) : ucore::InfoLayer(name, width, depth, NULL) {
	layerData = cv::Mat::zeros(depth, width, CV_32FC1);
}

MatrixInfoLayer::~MatrixInfoLayer() {
}

void MatrixInfoLayer::projectiveTextureMapping() {
	/*
	if (cameraTex == 0) {
		return;
	}

	//projective texture mapping
	GLfloat eyePlaneS[] = {1.0, 0.0, 0.0, 0.0};
	GLfloat eyePlaneT[] = {0.0, 1.0, 0.0, 0.0};
	GLfloat eyePlaneR[] = {0.0, 0.0, 1.0, 0.0};
	GLfloat eyePlaneQ[] = {0.0, 0.0, 0.0, 1.0};

	glEnable(GL_TEXTURE_2D);

	// projective texture coordinates generation
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, eyePlaneS);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, eyePlaneT);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_R, GL_EYE_PLANE, eyePlaneR);
	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, eyePlaneQ);


	glBindTexture(GL_TEXTURE_2D, cameraTex);//TODO

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	//=======================================================================

	// setup texture projection
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glTranslatef(0.5f,0.5f,0.0f);
	glScalef(0.5f,0.5f,1.0f);

	gluPerspective(90.0f, 1.0f,1.0f, 10.0f*1000000.0f);//model with field of view 37.7 degrees horizontal and 25.7 degrees vertical

	glMultMatrixd(invCameraMatrix);
	glMatrixMode(GL_MODELVIEW);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//printf("projectiveTextureMapping invCameraMatrix %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",invCameraMatrix[0],invCameraMatrix[1],invCameraMatrix[2],invCameraMatrix[3],invCameraMatrix[4],invCameraMatrix[5],invCameraMatrix[6],invCameraMatrix[7],invCameraMatrix[8],invCameraMatrix[9],invCameraMatrix[10],invCameraMatrix[11],invCameraMatrix[12],invCameraMatrix[13],invCameraMatrix[14],invCameraMatrix[15]);
	*/
}

void MatrixInfoLayer::restoreTexMatrix(){
	if(cameraTex==0)
		return;
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);
	glMatrixMode(GL_MODELVIEW);
}

void MatrixInfoLayer::updateTexFromData() {
	/*
	cv::Mat layerDataConv=cv::Mat::zeros(layerData.rows,layerData.cols,CV_8UC3);

	for(int c=0;c<layerData.cols;c++){
		for(int r=0;r<layerData.rows;r++){
			float oldV=layerData.at<float>(r,c);
			float o1=1.0f-(oldV/100.0f);
			float rC,gC,bC;
			ucore::Util::HSVtoRGB(&rC,&gC,&bC,120.0f*o1,0.9f,0.8f);//0 red 120` green
			layerDataConv.at<cv::Vec3b>(r,c)=cv::Vec3b((uchar)(255*bC),(uchar)(255*gC),(uchar)(255*rC));
		}
	}

	if(cameraTex == 0) {
		glGenTextures(1, &cameraTex);                  // Create The Texture
	}
		
	//printf("put in texture1\n");
	glBindTexture(GL_TEXTURE_2D, cameraTex);               
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//GL_REPLACE);
	//printf("put in texture3\n");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);//GL_LINEAR

	//printf("put in texture4 layerDataConv %d %d\n",layerDataConv.cols,layerDataConv.rows);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, layerDataConv.cols, layerDataConv.rows, GL_BGR_EXT, GL_UNSIGNED_BYTE,  layerDataConv.ptr());
	*/
}

float MatrixInfoLayer::getValue(int idxX, int idxY) {
	return layerData.at<float>(idxY, idxX);
}

void MatrixInfoLayer::addValue(int idxX, int idxY, float change) {
	layerData.at<float>(idxY, idxX) += change;
}

void MatrixInfoLayer::generateVertices() {
}

} // namespace utool