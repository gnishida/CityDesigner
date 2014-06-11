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

#include "../Core/InfoLayer.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "qgl.h"

namespace utool {

class MatrixInfoLayer : public ucore::InfoLayer {
protected:
	cv::Mat layerData;
	GLint viewport[4];
	GLdouble cameraMatrix[16];
	GLdouble invCameraMatrix[16];
	GLdouble projection[16];
	GLuint cameraTex;

public:
	MatrixInfoLayer(QString name, int width, int depth);
	~MatrixInfoLayer();

	void projectiveTextureMapping();
	void restoreTexMatrix();
	void updateTexFromData();

	float getValue(int idxX, int idxY);
	void addValue(int idxX, int idxY, float change);
	void generateVertices();
};

} // namespace utool