/************************************************************************************************
*		Procedural City Generation: Buildings geometry
*		@author igarciad
************************************************************************************************/

#include "VBORenderManager.h"
#include "VBOGeoBuilding.h"

#include "qdir.h"
const float storyHeight=4.2f;

using namespace boost::polygon::operators;
bool VBOGeoBuilding::bldgInitialized=false;

static std::vector<QString> facadeTex;
static std::vector<QVector3D> facadeScale;
static std::vector<QString> windowTex;
static std::vector<QString> roofTex;

void addTexConvexPoly(VBORenderManager& rendManager,QString geoName,QString textureName,GLenum geometryType,int shaderMode,
	std::vector<QVector3D> pos,QVector3D col,QVector3D norm,float zShift,bool inverseLoop,bool texZeroToOne,QVector3D texScale){

		if(pos.size()<3){
			return;
		}

		VBORenderManager::PolygonSetP polySet;
		VBORenderManager::polygonP tempPolyP;

		std::vector<VBORenderManager::pointP> vP;
		vP.resize(pos.size());
		float minX=FLT_MAX,minY=FLT_MAX;
		float maxX=-FLT_MAX,maxY=-FLT_MAX;

		for(int pN=0;pN<pos.size();pN++){
			vP[pN]=boost::polygon::construct<VBORenderManager::pointP>(pos[pN].x(),pos[pN].y());
			minX=std::min<float>(minX,pos[pN].x());
			minY=std::min<float>(minY,pos[pN].y());
			maxX=std::max<float>(maxX,pos[pN].x());
			maxY=std::max<float>(maxY,pos[pN].y());
		}

		boost::polygon::set_points(tempPolyP,vP.begin(),vP.end());
		polySet+=tempPolyP;
		std::vector<VBORenderManager::polygonP> allP;
		boost::polygon::get_trapezoids(allP,polySet);
		std::vector<Vertex> vert;
		for(int pN=0;pN<allP.size();pN++){
			//glColor3ub(qrand()%255,qrand()%255,qrand()%255);
			boost::polygon::polygon_with_holes_data<double>::iterator_type itPoly=allP[pN].begin();
			std::vector<QVector3D> points;
			std::vector<QVector3D> texP;
			while(itPoly!=allP[pN].end()){
				VBORenderManager::pointP cP=*itPoly;
				if(inverseLoop==false)
					points.push_back(QVector3D(cP.x(),cP.y(),pos[0].z()+zShift));
				else
					points.insert(points.begin(),QVector3D(cP.x(),cP.y(),pos[0].z()+zShift));

				if(texZeroToOne==true){
					texP.push_back(QVector3D((cP.x()-minX)/(maxX-minX),(cP.y()-minY)/(maxY-minY),0.0f));
				}else{
					texP.push_back(QVector3D((cP.x()-minX)*texScale.x(),(cP.y()-minY)*texScale.y(),0.0f));
				}
				itPoly++;
			}
			if(points.size()>=3){//last vertex repited
				for(int i=0;i<3;i++)
					vert.push_back(Vertex(points[i],col,norm,texP[i]));
				if(points.size()==3){
					if(geometryType==GL_QUADS)
						vert.push_back(Vertex(points[2],col,norm,texP[2]));//repeat last
				}else{
					if(geometryType==GL_QUADS)
						vert.push_back(Vertex(points[3],col,norm,texP[3]));//fourth
					else{
						for(int i=0;i<2;i++)//note just first 2
							vert.push_back(Vertex(points[i],col,norm,texP[i]));
						vert.push_back(Vertex(points[3],col,norm,texP[3]));//fourth
					}
				}
			}
		}

		rendManager.addStaticGeometry(geoName,vert,textureName,geometryType,shaderMode);
}//


void addFirstFloor(VBORenderManager& rendManager,std::vector<QVector3D>& footprint,QVector3D floorColor,float initHeight,float floorHeight){

	for(int sN=0;sN<footprint.size();sN++){
		int ind1=sN;
		int ind2=(sN+1)%footprint.size();
		std::vector<Vertex> sideVert;
		int nextN;
		QVector3D normal;
		for(int curN=0;curN<footprint.size();curN++){
			nextN=(curN+1)%footprint.size();
			normal=QVector3D::crossProduct(footprint[nextN]-footprint[curN],QVector3D(0,0,1)).normalized();
			sideVert.push_back(Vertex(QVector3D(footprint[curN].x(),footprint[curN].y(),initHeight),floorColor,normal,QVector3D()));
			sideVert.push_back(Vertex(QVector3D(footprint[nextN].x(),footprint[nextN].y(),initHeight),floorColor,normal,QVector3D()));
			sideVert.push_back(Vertex(QVector3D(footprint[nextN].x(),footprint[nextN].y(),initHeight+floorHeight),floorColor,normal,QVector3D()));
			sideVert.push_back(Vertex(QVector3D(footprint[curN].x(),footprint[curN].y(),initHeight+floorHeight),floorColor,normal,QVector3D()));			
		}
		rendManager.addStaticGeometry("3d_building",sideVert,"",GL_QUADS,1|mode_Lighting|mode_AdaptTerrain);//|LC::mode_Lighting);
	}
}//

void addBox(VBORenderManager& rendManager,std::vector<QVector3D>& roofOffCont,QVector3D boxColor,float initHeight,float boxSize){
	addTexConvexPoly(rendManager,"3d_building","",GL_QUADS,1|mode_Lighting,//|LC::mode_AdaptTerrain|LC::mode_Lighting,
		roofOffCont,boxColor,QVector3D(0,0,1.0f),initHeight+boxSize,false,true,QVector3D(1,1,1));
	addTexConvexPoly(rendManager,"3d_building","",GL_QUADS,1|mode_Lighting,//|LC::mode_AdaptTerrain|LC::mode_Lighting,
		roofOffCont,boxColor,QVector3D(0,0,-1.0f),initHeight,true,true,QVector3D(1,1,1));
	/*addTexConvexPoly(-1,
	roofOffCont,
	QVector3D(boxColor.redF(), boxColor.greenF(), boxColor.blueF()),
	QVector3D(0,0,1.0f),
	height+boxSize,false,true);//no inv yes toZero
	addTexConvexPoly(-1,
	roofOffCont,
	QVector3D(boxColor.redF(), boxColor.greenF(), boxColor.blueF()),
	QVector3D(0,0,-1.0f),
	height,true);//inverse loop*/
	addFirstFloor(rendManager,roofOffCont,boxColor,initHeight,boxSize);
}//

void addRoof(VBORenderManager& rendManager,std::vector<QVector3D>& roofOffCont,QVector3D boxColor,float initHeight,float boxSize){
	addTexConvexPoly(rendManager,"3d_building",roofTex[qrand()%roofTex.size()],GL_QUADS,2|mode_Lighting|mode_AdaptTerrain,//|LC::mode_AdaptTerrain|LC::mode_Lighting, "../data/textures/LC/roof/roof0.jpg"
		roofOffCont,boxColor,QVector3D(0,0,1.0f),initHeight+boxSize,false,true,QVector3D(1,1,1));
	addTexConvexPoly(rendManager,"3d_building","",GL_QUADS,1|mode_Lighting|mode_AdaptTerrain,//|LC::mode_AdaptTerrain|LC::mode_Lighting,
		roofOffCont,boxColor,QVector3D(0,0,-1.0f),initHeight,true,true,QVector3D(1,1,1));
	addFirstFloor(rendManager,roofOffCont,boxColor,initHeight,boxSize);
}//

void calculateColumnContour(std::vector<QVector3D>& currentContour,std::vector<QVector3D>& columnContour){
	QVector3D pos1,pos2;
	for(int sN=0;sN<currentContour.size();sN++){
		int ind1=sN;
		int ind2=(sN+1)%currentContour.size();
		pos1=currentContour[ind1];
		pos2=currentContour[ind2];
		QVector3D dirV=(pos2-pos1);
		float leng=(dirV).length();
		dirV/=leng;
		if(leng>7.0f){
			QVector3D perDir=QVector3D::crossProduct(dirV,QVector3D(0,0,1.0f));

			float remindingL=leng-1.0f-1.5f;
			int numWindows=remindingL/(3.0f+1.5f);
			float windowWidth=(leng-1.0f-1.5f*(1+numWindows))/numWindows;

			columnContour.push_back(pos1);
			//first col
			columnContour.push_back(pos1+0.5f*dirV);// first col
			columnContour.push_back(pos1+0.5f*dirV+0.8f*perDir);
			columnContour.push_back(pos1+(0.5f+1.5f)*dirV+0.8f*perDir);
			columnContour.push_back(pos1+(0.5f+1.5f)*dirV);
			QVector3D cPos=pos1+(0.5f+1.5f)*dirV;
			for(int nW=0;nW<numWindows;nW++){
				//window
				columnContour.push_back(cPos+(windowWidth)*dirV);
				//column
				columnContour.push_back(cPos+(windowWidth)*dirV+0.8f*perDir);
				columnContour.push_back(cPos+(windowWidth+1.5f)*dirV+0.8f*perDir);
				columnContour.push_back(cPos+(windowWidth+1.5f)*dirV);
				cPos+=dirV*(windowWidth+1.5f);
			}

		}else{
			columnContour.push_back(pos1);
		}
	}
}//

void addWindow(VBORenderManager& rendManager,
	int type,QVector3D randN,bool frameBoder, QVector3D initPoint,QVector3D dirR,QVector3D dirUp,float width,float height){
		//type=((int)randN.x())%2;
		/*if((randN.x()/RAND_MAX)<0.1f)
		type=3;
		else
		if((randN.x()/RAND_MAX)<0.55f)
		type=1;
		else
		type=0;*/

		frameBoder=((int)randN.y())%2;
		QVector3D color;
		int randCol=((int)randN.z())%5;
		switch(randCol){
		case 0:
			color=QVector3D(0.3f,0.3f,0.3f);
			break;
		case 1:
			color=QVector3D(0.345, 0.171, 0.075);//brown
			break;
		case 2:
			color=QVector3D(0.412, 0.412, 0.412);//grey
			break;
		case 3:
			color=QVector3D(0.02, 0.02, 0.13);//blue
			break;
		case 4:
			color=QVector3D(0.961, 0.961, 0.863);//beige
			break;
		}
		// NO WINDOW, JUST DEPTH
		if(type==0){
			std::vector<Vertex> vertWind;

			float depth=2.0f;
			// IN: TOP
			QVector3D perI=QVector3D::crossProduct(dirUp,dirR);//note direction: to inside
			std::vector<QVector3D> tex;
			QVector3D vert[8];
			vert[0]=initPoint;
			vert[1]=initPoint+perI*depth;
			vert[2]=initPoint+perI*depth+dirUp*height;
			vert[3]=initPoint+dirUp*height;

			vert[4]=initPoint+perI*depth+dirR*width;
			vert[5]=initPoint+dirR*width;
			vert[6]=initPoint+dirUp*height+dirR*width;
			vert[7]=initPoint+perI*depth+dirUp*height+dirR*width;
			int texN=-1;
			QVector3D color(0.5f,0.5f,0.5f);
			// LEFT
			//addTexQuad(texN,vert[0],vert[1],vert[2],vert[3],tex,color,QVector3D(),true);
			QVector3D norm;
			norm=QVector3D::crossProduct(vert[1]-vert[0],vert[3]-vert[0]);
			vertWind.push_back(Vertex(vert[0],color,norm,QVector3D()));
			vertWind.push_back(Vertex(vert[1],color,norm,QVector3D()));
			vertWind.push_back(Vertex(vert[2],color,norm,QVector3D()));
			vertWind.push_back(Vertex(vert[3],color,norm,QVector3D()));

			// RIGHT
			//addTexQuad(texN,vert[4],vert[5],vert[6],vert[7],tex,color,QVector3D(),true);
			norm=QVector3D::crossProduct(vert[5]-vert[4],vert[7]-vert[4]);
			vertWind.push_back(Vertex(vert[4],color,norm,QVector3D()));
			vertWind.push_back(Vertex(vert[5],color,norm,QVector3D()));
			vertWind.push_back(Vertex(vert[6],color,norm,QVector3D()));
			vertWind.push_back(Vertex(vert[7],color,norm,QVector3D()));
			//// BACK
			//addTexQuad(texN,vert[1],vert[4],vert[7],vert[2],tex,QVector3D(0.0f,0.0f,0.5f),QVector3D(),true);
			// TOP
			//addTexQuad(texN,vert[2],vert[7],vert[6],vert[3],tex,color,QVector3D(),true);
			norm=QVector3D::crossProduct(vert[7]-vert[2],vert[3]-vert[2]);
			vertWind.push_back(Vertex(vert[2],color,norm,QVector3D()));
			vertWind.push_back(Vertex(vert[7],color,norm,QVector3D()));
			vertWind.push_back(Vertex(vert[6],color,norm,QVector3D()));
			vertWind.push_back(Vertex(vert[3],color,norm,QVector3D()));
			// BOT
			//addTexQuad(texN,vert[0],vert[5],vert[4],vert[1],tex,color,QVector3D(),true);
			norm=QVector3D::crossProduct(vert[5]-vert[0],vert[1]-vert[0]);
			vertWind.push_back(Vertex(vert[0],color,norm,QVector3D()));
			vertWind.push_back(Vertex(vert[5],color,norm,QVector3D()));
			vertWind.push_back(Vertex(vert[4],color,norm,QVector3D()));
			vertWind.push_back(Vertex(vert[1],color,norm,QVector3D()));
			rendManager.addStaticGeometry("3d_building",vertWind,"",GL_QUADS,1|mode_Lighting|mode_AdaptTerrain);//|LC::mode_AdaptTerrain|LC::mode_Lighting);
			//////////////////////////////////////////////////////
			// BACK
			vertWind.clear();
			norm=QVector3D::crossProduct(vert[4]-vert[1],vert[2]-vert[1]);
			vertWind.push_back(Vertex(vert[1],color,norm,QVector3D(0,0,0)));
			vertWind.push_back(Vertex(vert[4],color,norm,QVector3D(1,0,0)));
			vertWind.push_back(Vertex(vert[7],color,norm,QVector3D(1,1,0)));
			vertWind.push_back(Vertex(vert[2],color,norm,QVector3D(0,1,0)));

			//rendManager.addStaticGeometry("buildingsTop",vertWind,"../data/textures/LC/wind/c_window1.jpg",GL_QUADS,2|LC::mode_AdaptTerrain|LC::mode_Lighting);
			rendManager.addStaticGeometry("3d_building",vertWind,windowTex[((int)randN.x())%windowTex.size()],GL_QUADS,2|mode_Lighting|mode_AdaptTerrain);//|LC::mode_AdaptTerrain|LC::mode_Lighting);

			/*tex.push_back(QVector3D(0,0,0));tex.push_back(QVector3D(1.0f,0,0));tex.push_back(QVector3D(1.0f,1.0f,0));tex.push_back(QVector3D(0,1.0f,0));
			texN=window_frameTex[((int)randN.x())%window_frameTex.size()];
			addTexQuad(texN,vert[1],vert[4],vert[7],vert[2],tex,QVector3D(0.0f,0.0f,0.5f),QVector3D(),true);*/
			return;
		}

		/*if(frameBoder==true&&type!=3){//frame (not for ralings)
		float frBotDepth=0.5f;
		float frBotHeig=0.25f;
		//bot
		addHalfBox(-1,QVector3D(0.5,0.5,0.5),initPoint,dirR,dirUp,width,frBotHeig,frBotDepth,true,true);
		//top
		addHalfBox(-1,QVector3D(0.5,0.5,0.5),initPoint+dirUp*(height-frBotHeig/2.0f),dirR,dirUp,width,frBotHeig/2.0f,frBotDepth/2,true,true);
		// left
		addHalfBox(-1,QVector3D(0.5,0.5 ,0.5),initPoint+dirUp*(frBotHeig),dirR,dirUp,frBotHeig,height-1.5f*frBotHeig,frBotDepth/2,true,false);
		// right
		addHalfBox(-1,QVector3D(0.5,0.5,0.5),initPoint+dirUp*(frBotHeig)+dirR*(width-frBotDepth/2),dirR,dirUp,frBotHeig,height-1.5f*frBotHeig,frBotDepth/2,true,false);


		initPoint+=dirUp*(frBotHeig)+dirR*frBotHeig;
		width-=2*frBotHeig;
		height-=(1.5f*frBotHeig);
		}

		// BACK
		std::vector<QVector3D> tex;
		// BACK
		tex.push_back(QVector3D(0,0,0));tex.push_back(QVector3D(1.0f,0,0));tex.push_back(QVector3D(1.0f,1.0f,0));tex.push_back(QVector3D(0,1.0f,0));
		int texN=window_insideTex[((int)randN.x())%window_insideTex.size()];
		addTexQuad(texN,initPoint,initPoint+dirR*width,initPoint+dirR*width+dirUp*height,initPoint+dirUp*height,tex,QVector3D(0.0f,0.0f,0.5f),QVector3D(),true);
		tex.clear();



		if(type==2){// square frame
		float frameWidth=0.12f;

		//bot
		addHalfBox(-1,color,initPoint,dirR,dirUp,width,frameWidth,frameWidth,true,true);
		//top
		addHalfBox(-1,color,initPoint+dirUp*(height-frameWidth),dirR,dirUp,width,frameWidth,frameWidth,true,true);
		// left
		addHalfBox(-1,color,initPoint+dirUp*(frameWidth),dirR,dirUp,frameWidth,height-2.0f*frameWidth,frameWidth,true,false);
		// right
		addHalfBox(-1,color,initPoint+dirUp*(frameWidth)+dirR*(width-frameWidth),dirR,dirUp,frameWidth,height-2.0f*frameWidth,frameWidth,true,false);
		}*/

}//

void addColumnGeometry(VBORenderManager& rendManager,
	std::vector<QVector3D>& columnContour,int randomFacade,QVector3D randN,float uS,float vS,float height,int numFloors,bool buildingWithWindows,QVector3D windowRandomSize){

		std::vector<Vertex> vert;

		float verticalHoleSize=windowRandomSize.x();//0.1f+(1.2f*qrand())/RAND_MAX;
		float horHoleSize=windowRandomSize.y();
		float accPerimeter=0;
		QVector3D norm;
		for(int sN=0;sN<columnContour.size();sN++){
			int ind1=sN;
			int ind2=(sN+1)%columnContour.size();
			std::vector<QVector3D> em;
			bool window=(columnContour[ind1]-columnContour[ind2]).length()>3.0f;
			float sideLenght=(columnContour[ind1]-columnContour[ind2]).length();
			if(window==false){//just column
				float heightB=height;
				float heightT=numFloors*storyHeight+height;
				/*em.clear();//calculate coordenates (respect the perimeter)
				em.push_back();
				em.push_back(QVector3D();
				em.push_back();
				em.push_back();
				addTexQuad(facades[randomFacade],
				columnContour[ind1]+QVector3D(0,0,heightB),
				columnContour[ind2]+QVector3D(0,0,heightB),
				columnContour[ind2]+QVector3D(0,0,heightT),
				columnContour[ind1]+QVector3D(0,0,heightT),
				em,
				QVector3D(),
				QVector3D(),
				true);*/
				QVector3D norm=QVector3D::crossProduct(columnContour[ind2]-columnContour[ind1],QVector3D(0,0,1.0f));
				vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,heightB),QVector3D(),norm,QVector3D(accPerimeter*uS,heightB*vS,0.0f)));
				vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,heightB),QVector3D(),norm,QVector3D((accPerimeter+sideLenght)*uS,heightB*vS,0.0f)));
				vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,heightT),QVector3D(),norm,QVector3D((accPerimeter+sideLenght)*uS,heightT*vS,0.0f)));
				vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,heightT),QVector3D(),norm,QVector3D((accPerimeter)*uS,heightT*vS,0.0f)));


			}else{

				for(int numF=0;numF<numFloors;numF++){
					float h0=numF*storyHeight+height;
					float h3=(numF+1)*storyHeight+height;
					float h1=h0+verticalHoleSize;
					float h2=h3-verticalHoleSize;
					// BOT
					/*em.clear();//calculate coordenates (respect the perimeter)
					em.push_back(QVector3D(accPerimeter*uS,h0*vS,0.0f));
					em.push_back(QVector3D((accPerimeter+sideLenght)*uS,h0*vS,0.0f));
					em.push_back(QVector3D((accPerimeter+sideLenght)*uS,h1*vS,0.0f));
					em.push_back(QVector3D((accPerimeter)*uS,h1*vS,0.0f));
					addTexQuad(facades[randomFacade],
					columnContour[ind1]+QVector3D(0,0,h0),
					columnContour[ind2]+QVector3D(0,0,h0),
					columnContour[ind2]+QVector3D(0,0,h1),
					columnContour[ind1]+QVector3D(0,0,h1),
					em,
					QVector3D(),
					QVector3D(),
					true);*/
					norm=QVector3D::crossProduct(columnContour[ind2]-columnContour[ind1],QVector3D(0,0,1.0f));
					vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h0),QVector3D(),norm,QVector3D(accPerimeter*uS,h0*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h0),QVector3D(),norm,QVector3D((accPerimeter+sideLenght)*uS,h0*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h1),QVector3D(),norm,QVector3D((accPerimeter+sideLenght)*uS,h1*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h1),QVector3D(),norm,QVector3D((accPerimeter)*uS,h1*vS,0.0f)));
					// TOP
					/*em.clear();//calculate coordenates (respect the perimeter)
					em.push_back(QVector3D(accPerimeter*uS,h2*vS,0.0f));
					em.push_back(QVector3D((accPerimeter+sideLenght)*uS,h2*vS,0.0f));
					em.push_back(QVector3D((accPerimeter+sideLenght)*uS,h3*vS,0.0f));
					em.push_back(QVector3D((accPerimeter)*uS,h3*vS,0.0f));
					addTexQuad(facades[randomFacade],
					columnContour[ind1]+QVector3D(0,0,h2),
					columnContour[ind2]+QVector3D(0,0,h2),
					columnContour[ind2]+QVector3D(0,0,h3),
					columnContour[ind1]+QVector3D(0,0,h3),
					em,
					QVector3D(),
					QVector3D(),
					true);*/
					norm=QVector3D::crossProduct(columnContour[ind2]-columnContour[ind1],QVector3D(0,0,1.0f));//h3-h2
					vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h2),QVector3D(),norm,QVector3D(accPerimeter*uS,h2*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h2),QVector3D(),norm,QVector3D((accPerimeter+sideLenght)*uS,h2*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h3),QVector3D(),norm,QVector3D((accPerimeter+sideLenght)*uS,h3*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h3),QVector3D(),norm,QVector3D((accPerimeter)*uS,h3*vS,0.0f)));
					// LEFT
					QVector3D dirW=(columnContour[ind2]-columnContour[ind1]);
					float windWidth=dirW.length();
					dirW/=windWidth;
					/*em.clear();//calculate coordenates (respect the perimeter)
					em.push_back(QVector3D(accPerimeter*uS,h1*vS,0.0f));
					em.push_back(QVector3D((accPerimeter+horHoleSize)*uS,h1*vS,0.0f));
					em.push_back(QVector3D((accPerimeter+horHoleSize)*uS,h2*vS,0.0f));
					em.push_back(QVector3D((accPerimeter)*uS,h2*vS,0.0f));
					addTexQuad(facades[randomFacade],
					columnContour[ind1]+QVector3D(0,0,h1),
					columnContour[ind1]+QVector3D(0,0,h1)+dirW*horHoleSize,
					columnContour[ind1]+QVector3D(0,0,h2)+dirW*horHoleSize,
					columnContour[ind1]+QVector3D(0,0,h2),
					em,
					QVector3D(),
					QVector3D(),
					true);*/
					norm=QVector3D::crossProduct(dirW*horHoleSize,QVector3D(0,0,1.0f)).normalized();//h2-h1
					vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h1),QVector3D(),norm,QVector3D(accPerimeter*uS,h1*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h1)+dirW*horHoleSize,QVector3D(),norm,QVector3D((accPerimeter+horHoleSize)*uS,h1*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h2)+dirW*horHoleSize,QVector3D(),norm,QVector3D((accPerimeter+horHoleSize)*uS,h2*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind1]+QVector3D(0,0,h2),QVector3D(),norm,QVector3D((accPerimeter)*uS,h2*vS,0.0f)));
					// RIGHT
					/*em.clear();//calculate coordenates (respect the perimeter)
					em.push_back(QVector3D((accPerimeter+sideLenght-horHoleSize)*uS,h1*vS,0.0f));
					em.push_back(QVector3D((accPerimeter+sideLenght)*uS,h1*vS,0.0f));
					em.push_back(QVector3D((accPerimeter+sideLenght)*uS,h2*vS,0.0f));
					em.push_back(QVector3D((accPerimeter+sideLenght-horHoleSize)*uS,h2*vS,0.0f));
					addTexQuad(facades[randomFacade],
					columnContour[ind2]+QVector3D(0,0,h1)-dirW*horHoleSize,
					columnContour[ind2]+QVector3D(0,0,h1),
					columnContour[ind2]+QVector3D(0,0,h2),
					columnContour[ind2]+QVector3D(0,0,h2)-dirW*horHoleSize,
					em,
					QVector3D(),
					QVector3D(),
					true);*/
					norm=QVector3D::crossProduct(dirW*horHoleSize,QVector3D(0,0,1.0f)).normalized();//h2-h1
					vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h1)-dirW*horHoleSize,QVector3D(),norm,QVector3D((accPerimeter+sideLenght-horHoleSize)*uS,h1*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h1),QVector3D(),norm,QVector3D((accPerimeter+sideLenght)*uS,h1*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h2),QVector3D(),norm,QVector3D((accPerimeter+sideLenght)*uS,h2*vS,0.0f)));
					vert.push_back(Vertex(columnContour[ind2]+QVector3D(0,0,h2)-dirW*horHoleSize,QVector3D(),norm,QVector3D((accPerimeter+sideLenght-horHoleSize)*uS,h2*vS,0.0f)));

					////////// INSIDE
					addWindow(rendManager,0,randN,false,columnContour[ind1]+QVector3D(0,0,h1)+dirW*horHoleSize,dirW,QVector3D(0,0,1.0f),windWidth-2*horHoleSize,h2-h1);
					//printf("wind wid %f %f\n",windWidth,h2-h1);
				}
			}
			accPerimeter+=sideLenght;
		}
		//rendManager.addStaticGeometry("buildingsTop",vert,"../data/textures/LC/facade/SeamlessBrick05_1.25_1.3_8.JPG",GL_QUADS,2|LC::mode_AdaptTerrain|LC::mode_Lighting);
		rendManager.addStaticGeometry("3d_building",vert,facadeTex[randomFacade],GL_QUADS,2|mode_Lighting|mode_AdaptTerrain);//|LC::mode_AdaptTerrain|LC::mode_Lighting);
}//

void VBOGeoBuilding::initBuildingsTex(){
	QString pathName="../data/textures/LC/";
	QStringList nameFilter;
	nameFilter << "*.png" << "*.jpg" << "*.gif";
	// 1. facade
	QDir directory(pathName+"facade/");
	QStringList list = directory.entryList( nameFilter, QDir::Files );
	for(int lE=0;lE<list.size();lE++){
		facadeTex.push_back(pathName+"/facade/"+list[lE]);
		QStringList scaleS=list[lE].split("_");
		//printf("*********** scaleS %d\n",scaleS.size());
		if(scaleS.size()!=4)
			facadeScale.push_back(QVector3D(1.0f,1.0f,0));
		else{
			facadeScale.push_back(QVector3D(scaleS[1].toFloat(),scaleS[2].toFloat(),0));
			//printf("Scale %s -->%f %f\n",list[lE].toAscii().constData(),scaleS[1].toFloat(),scaleS[2].toFloat());
		}
	}

	// 2. windows
	QDir directoryW(pathName+"wind/");
	list = directoryW.entryList( nameFilter, QDir::Files );
	for(int lE=0;lE<list.size();lE++){
		windowTex.push_back(pathName+"wind/"+list[lE]);
	}
	// 3. roof

		QDir directoryR(pathName+"roof/");
		list = directoryR.entryList( nameFilter, QDir::Files );
		for(int lE=0;lE<list.size();lE++){
			roofTex.push_back(pathName+"roof/"+list[lE]);
		}
		printf("initBuildingsTex f %d w %d r %d\n",facadeTex.size(),windowTex.size(),roofTex.size());
	bldgInitialized=true;
}

void VBOGeoBuilding::generateBuilding(VBORenderManager& rendManager,Building& building,int type){

	Polygon3D& footprint=building.buildingFootprint;
	int numStories=building.numStories;
	//printf("numSt %d numSides %d\n",numStories,footprint.contour.size());
	///////////////////////////
	// BOX
	if(type==0){
		// SIDES
		float height=numStories*storyHeight;
		std::vector<Vertex> sideVert;
		int nextN;
		QVector3D normal;
		float rndColod=((0.7f*(float)qrand())/RAND_MAX)+0.3f;
		QVector3D color(rndColod,rndColod,rndColod);
		for(int curN=0;curN<footprint.contour.size();curN++){
			nextN=(curN+1)%footprint.contour.size();
			normal=QVector3D::crossProduct(footprint[nextN]-footprint[curN],QVector3D(0,0,1)).normalized();
			sideVert.push_back(Vertex(QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f),color,normal,QVector3D()));
			sideVert.push_back(Vertex(QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f),color,normal,QVector3D()));
			sideVert.push_back(Vertex(QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f+height),color,normal,QVector3D()));
			sideVert.push_back(Vertex(QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f+height),color,normal,QVector3D()));			
		}
		rendManager.addStaticGeometry("3d_building",sideVert,"",GL_QUADS,1|mode_Lighting|mode_AdaptTerrain);//|LC::mode_AdaptTerrain|LC::mode_Lighting);

		// ROOF
		if(footprint.contour.size()==3||footprint.contour.size()==4){
			normal=QVector3D(0,0,1.0f);
			std::vector<Vertex> topVert;
			topVert.push_back(Vertex(QVector3D(footprint[0].x(),footprint[0].y(),0.0f+height),color,normal,QVector3D()));
			topVert.push_back(Vertex(QVector3D(footprint[1].x(),footprint[1].y(),0.0f+height),color,normal,QVector3D()));
			topVert.push_back(Vertex(QVector3D(footprint[2].x(),footprint[2].y(),0.0f+height),color,normal,QVector3D()));
			if(footprint.contour.size()==4){

				topVert.push_back(Vertex(QVector3D(footprint[3].x(),footprint[3].y(),0.0f+height),color,normal,QVector3D()));
			}else{
				topVert.push_back(Vertex(QVector3D(footprint[2].x(),footprint[2].y(),0.0f+height),color,normal,QVector3D()));
			}
			rendManager.addStaticGeometry("3d_building",topVert,"",GL_QUADS,1|mode_Lighting|mode_AdaptTerrain);//|LC::mode_AdaptTerrain|LC::mode_Lighting);
		}
	}
	/*if(footprint.size()==3||footprint.size()==4){
	rendManager.addStaticConvexPoly("buildingsTop",footprint,height,false,"../data/textures/LC/sidewalk/pavement.jpg",12,QVector3D(),false,&color);
	}*/

	///////////////////////////
	// MORE COMPLEX
	if(type==1){
		if(bldgInitialized==false){
			initBuildingsTex();
		}
		float boxSize=1.0f;
		float firstFloorHeigh=4.8f;
		float buildingHeight=(numStories-1)*storyHeight+firstFloorHeigh+boxSize;//just one box size (1st-2nd)

		Loop3D roofOffCont;

		footprint.computeInset(-boxSize, roofOffCont, false); 

		////////////////////////////
		// FLOORS
		float randC=(0.8*qrand())/RAND_MAX;
		QVector3D bldgColor = QVector3D(randC,randC,randC);
		///////////////////////////
		// First floor
		addFirstFloor(rendManager,footprint.contour,bldgColor,0,firstFloorHeigh);
		//first/second floor

		addBox(rendManager,roofOffCont,bldgColor,firstFloorHeigh,boxSize);
		firstFloorHeigh+=boxSize;
		/// Add columns
		std::vector<QVector3D> columnContour;
		calculateColumnContour(footprint.contour,columnContour);

		// add geometry
		int randomFacade=qrand()%facadeTex.size();
		float uS=facadeScale[randomFacade].x();
		float vS=facadeScale[randomFacade].y();
		/*bool buildingWithWindows=((float)qrand()/RAND_MAX)<0.03f;*/

		//addColumnGeometry(columnContour,randomFacade,randN,uS,vS,firstFloorHeigh,numFloors-1,buildingWithWindows,QVector3D(),wSpec);
		QVector3D windowRandSize((float)qrand()/RAND_MAX,(float)qrand()/RAND_MAX,(float)qrand()/RAND_MAX);
		QVector3D randN(qrand(),qrand(),qrand());
		addColumnGeometry(rendManager,columnContour,randomFacade,randN,uS,vS,firstFloorHeigh,numStories-1,false,windowRandSize);

		////////////////////////////
		// ROOF	
		//int randomRoof=(int)(roofColor.redF()*13248431)%roofs.size();
		//QVector3D roofCol(roofColor.redF(),roofColor.greenF(),roofColor.blueF());
		//ddRoof(roofOffCont,roofCol,randomRoof,buildingHeight,boxSize);
		addRoof(rendManager,roofOffCont,bldgColor,buildingHeight,boxSize);
	}

	///////////////////////////
	// BOX WITH EDGES 
	if(type==2){
		float blackWidth=0.5f;
		// SIDES
		float height=numStories*storyHeight;
		std::vector<Vertex> sideVert;
		int nextN;
		QVector3D normal;
		QVector3D colorW(1.0f,1.0f,1.0f);
		QVector3D colorB(0.0f,0.0f,0.0f);
		QVector3D a,b,c,d;
		float sideWalkEl=1.5f;

		for(int curN=0;curN<footprint.contour.size();curN++){
			nextN=(curN+1)%footprint.contour.size();
			QVector3D dir=footprint[nextN]-footprint[curN];
			float leng=dir.length();
			dir/=leng;
			normal=QVector3D::crossProduct(dir,QVector3D(0,0,1)).normalized();
			if(leng<blackWidth*2){
				sideVert.push_back(Vertex(QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f+sideWalkEl),colorW,normal,QVector3D()));
				sideVert.push_back(Vertex(QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f+sideWalkEl),colorW,normal,QVector3D()));
				sideVert.push_back(Vertex(QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f+height+sideWalkEl),colorW,normal,QVector3D()));
				sideVert.push_back(Vertex(QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f+height+sideWalkEl),colorW,normal,QVector3D()));
			}else{
				
				//black top
				sideVert.push_back(Vertex(QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f+height-blackWidth+sideWalkEl),colorB,normal,QVector3D()));
				sideVert.push_back(Vertex(QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f+height-blackWidth+sideWalkEl),colorB,normal,QVector3D()));
				sideVert.push_back(Vertex(QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f+height+sideWalkEl),colorB,normal,QVector3D()));
				sideVert.push_back(Vertex(QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f+height+sideWalkEl),colorB,normal,QVector3D()));
				//black bottom
				sideVert.push_back(Vertex(QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f+sideWalkEl),colorB,normal,QVector3D()));
				sideVert.push_back(Vertex(QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f+sideWalkEl),colorB,normal,QVector3D()));
				sideVert.push_back(Vertex(QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f+blackWidth+sideWalkEl),colorB,normal,QVector3D()));
				sideVert.push_back(Vertex(QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f+blackWidth+sideWalkEl),colorB,normal,QVector3D()));

				a=QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f+blackWidth+sideWalkEl);
				b=QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f+blackWidth+sideWalkEl);
				c=QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f+height-blackWidth+sideWalkEl);
				d=QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f+height-blackWidth+sideWalkEl);
				//black
				sideVert.push_back(Vertex(a,colorB,normal,QVector3D()));
				sideVert.push_back(Vertex(a+dir*blackWidth,colorB,normal,QVector3D()));
				sideVert.push_back(Vertex(d+dir*blackWidth,colorB,normal,QVector3D()));
				sideVert.push_back(Vertex(d,colorB,normal,QVector3D()));
				//white
				sideVert.push_back(Vertex(a+dir*blackWidth,colorW,normal,QVector3D()));
				sideVert.push_back(Vertex(b-dir*blackWidth,colorW,normal,QVector3D()));
				sideVert.push_back(Vertex(c-dir*blackWidth,colorW,normal,QVector3D()));
				sideVert.push_back(Vertex(d+dir*blackWidth,colorW,normal,QVector3D()));
				//black
				sideVert.push_back(Vertex(b-dir*blackWidth,colorB,normal,QVector3D()));
				sideVert.push_back(Vertex(b,colorB,normal,QVector3D()));
				sideVert.push_back(Vertex(c,colorB,normal,QVector3D()));
				sideVert.push_back(Vertex(c-dir*blackWidth,colorB,normal,QVector3D()));
			}
		}
		rendManager.addStaticGeometry("3d_building",sideVert,"",GL_QUADS,1|mode_AdaptTerrain);//|LC::mode_AdaptTerrain|LC::mode_Lighting);

		// ROOF
		Polygon3D roofRed;
		footprint.computeInset(1.0f,roofRed.contour,false);
		//rendManager.addStaticGeometry2("3d_building",footprint.contour,height,false,"",GL_QUADS,1,QVector3D(1,1,1));
		rendManager.addStaticGeometry2("3d_building",roofRed.contour,height+sideWalkEl,false,"",GL_QUADS,1|mode_AdaptTerrain,QVector3D(1,1,1),colorW);
		rendManager.addStaticGeometry2("3d_building",footprint.contour,height-blackWidth/2.0f+sideWalkEl,false,"",GL_QUADS,1|mode_AdaptTerrain,QVector3D(1,1,1),colorB);
	}

	///////////////////////////
	// BOX WITH EDGES 
	if(type==3){
		//printf("TYPE 3\n");
		// SIDES
		float height=numStories*storyHeight;
		std::vector<Vertex> sideVert;
		int nextN;
		QVector3D normal;
		QVector3D colorW(1.0f,1.0f,1.0f);
		QVector3D colorB(0.0f,0.0f,0.0f);
		QVector3D a,b,c,d;
		float sideWalkEl=1.5f;
		int windNumber=qrand()%3;//=int(footprint.contour[0].x()*100)%3;
		for(int curN=0;curN<footprint.contour.size();curN++){
			nextN=(curN+1)%footprint.contour.size();
			QVector3D dir=footprint[nextN]-footprint[curN];
			float leng=dir.length();
			dir/=leng;
			normal=QVector3D::crossProduct(dir,QVector3D(0,0,1)).normalized();

				sideVert.push_back(Vertex(
					QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f+sideWalkEl),
					QVector3D(0,0,windNumber),
					normal,
					QVector3D(leng,height,0)));//max
				sideVert.push_back(Vertex(
					QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f+sideWalkEl),
					QVector3D(leng,0,windNumber),
					normal,
					QVector3D(leng,height,0)));//max
				sideVert.push_back(Vertex(
					QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f+height+sideWalkEl),
					QVector3D(leng,height,windNumber),
					normal,
					QVector3D(leng,height,0)));//max
				sideVert.push_back(Vertex(
					QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f+height+sideWalkEl),
					QVector3D(0,height,windNumber),
					normal,
					QVector3D(leng,height,0)));//max
			/*sideVert.push_back(Vertex(QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f+sideWalkEl),colorW,normal,QVector3D()));
				sideVert.push_back(Vertex(QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f+sideWalkEl),colorW,normal,QVector3D()));
				sideVert.push_back(Vertex(QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f+height+sideWalkEl),colorW,normal,QVector3D()));
				sideVert.push_back(Vertex(QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f+height+sideWalkEl),colorW,normal,QVector3D()));*/
		}
		/*std::vector<QString> hatchFiles;
		for(int i=0;i<=5;i++){//5 hatch + perlin
			//hatchFiles.push_back("../data/textures/LC/hatch/h"+QString::number(i)+".png");
			hatchFiles.push_back("../data/textures/LC/hatch/h"+QString::number(i)+"b.png");
		}
		rendManager.loadArrayTexture("hatching_array",hatchFiles);*/
		rendManager.addStaticGeometry("3d_building_fac",sideVert,"hatching_array",GL_QUADS,9|0x0200|mode_TexArray|mode_AdaptTerrain);//|LC::mode_AdaptTerrain|LC::mode_Lighting);
		//rendManager.addStaticGeometry("3d_building_fac",sideVert,"../data/textures/LC/hatch/h1.png",GL_QUADS,8);//|LC::mode_AdaptTerrain|LC::mode_Lighting);

		// ROOF
		float blackWidth=1.0f;
		Polygon3D roofRed;
		footprint.computeInset(blackWidth,roofRed.contour,false);
		//rendManager.addStaticGeometry2("3d_building",roofRed.contour,height+sideWalkEl,false,"",GL_QUADS,1,QVector3D(1,1,1),colorW);
		rendManager.addStaticGeometry2("3d_building",footprint.contour,height-0.2+sideWalkEl,false,"hatching_array",GL_QUADS,10|mode_TexArray|mode_AdaptTerrain,QVector3D(1,1,1),QVector3D(0.1f,0.1f,0.1f));
	}

	///////////////////////////
	// BOX HATCHING
	if(type==4){
		float blackWidth=1.0f;
		// SIDES
		float height=numStories*storyHeight;
		std::vector<Vertex> sideVertT;
		std::vector<Vertex> sideVert;
		int nextN;
		QVector3D normal;
		QVector3D colorW(1.0f,1.0f,1.0f);
		QVector3D colorB(0.0f,0.0f,0.0f);
		QVector3D a,b,c,d;
		float sideWalkEl=1.5f;

		for(int curN=0;curN<footprint.contour.size();curN++){
			nextN=(curN+1)%footprint.contour.size();
			QVector3D dir=footprint[nextN]-footprint[curN];
			float leng=dir.length();
			dir/=leng;
			normal=QVector3D::crossProduct(dir,QVector3D(0,0,1)).normalized();
			if(leng<blackWidth*2){
				sideVert.push_back(Vertex(QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f+sideWalkEl),colorW,normal,QVector3D()));
				sideVert.push_back(Vertex(QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f+sideWalkEl),colorW,normal,QVector3D()));
				sideVert.push_back(Vertex(QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f+height+sideWalkEl),colorW,normal,QVector3D()));
				sideVert.push_back(Vertex(QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f+height+sideWalkEl),colorW,normal,QVector3D()));
			}else{
				
				//black top
				sideVert.push_back(Vertex(QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f+height-blackWidth+sideWalkEl),colorB,normal,QVector3D()));
				sideVert.push_back(Vertex(QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f+height-blackWidth+sideWalkEl),colorB,normal,QVector3D()));
				sideVert.push_back(Vertex(QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f+height+sideWalkEl),colorB,normal,QVector3D()));
				sideVert.push_back(Vertex(QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f+height+sideWalkEl),colorB,normal,QVector3D()));
				//black bottom
				sideVert.push_back(Vertex(QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f+sideWalkEl),colorB,normal,QVector3D()));
				sideVert.push_back(Vertex(QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f+sideWalkEl),colorB,normal,QVector3D()));
				sideVert.push_back(Vertex(QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f+blackWidth+sideWalkEl),colorB,normal,QVector3D()));
				sideVert.push_back(Vertex(QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f+blackWidth+sideWalkEl),colorB,normal,QVector3D()));

				a=QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f+blackWidth+sideWalkEl);
				b=QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f+blackWidth+sideWalkEl);
				c=QVector3D(footprint[nextN].x(),footprint[nextN].y(),0.0f+height-blackWidth+sideWalkEl);
				d=QVector3D(footprint[curN].x(),footprint[curN].y(),0.0f+height-blackWidth+sideWalkEl);
				//black
				sideVertT.push_back(Vertex(a,colorB,normal,QVector3D(0,0,0)));
				sideVertT.push_back(Vertex(a+dir*blackWidth,colorB,normal,QVector3D(blackWidth,0,0)));
				sideVertT.push_back(Vertex(d+dir*blackWidth,colorB,normal,QVector3D(blackWidth,height,0)));
				sideVertT.push_back(Vertex(d,colorB,normal,QVector3D(0,height,0)));
				//white
				sideVert.push_back(Vertex(a+dir*blackWidth,colorW,normal,QVector3D()));
				sideVert.push_back(Vertex(b-dir*blackWidth,colorW,normal,QVector3D()));
				sideVert.push_back(Vertex(c-dir*blackWidth,colorW,normal,QVector3D()));
				sideVert.push_back(Vertex(d+dir*blackWidth,colorW,normal,QVector3D()));
				//black
				sideVertT.push_back(Vertex(b-dir*blackWidth,colorB,normal,QVector3D(leng-blackWidth,0,0)));
				sideVertT.push_back(Vertex(b,colorB,normal,QVector3D(leng,0,0)));
				sideVertT.push_back(Vertex(c,colorB,normal,QVector3D(leng,height,0)));
				sideVertT.push_back(Vertex(c-dir*blackWidth,colorB,normal,QVector3D(leng-blackWidth,height,0)));
			}
		}
		rendManager.addStaticGeometry("3d_building",sideVert,"",GL_QUADS,1|mode_AdaptTerrain);//|LC::mode_AdaptTerrain|LC::mode_Lighting);
		rendManager.addStaticGeometry("3d_building",sideVertT,"../data/textures/LC/hatch/h2.png",GL_QUADS,2|mode_AdaptTerrain);//|LC::mode_AdaptTerrain|LC::mode_Lighting);

		// ROOF
		Polygon3D roofRed;
		footprint.computeInset(1.0f,roofRed.contour,false);
		//rendManager.addStaticGeometry2("3d_building",footprint.contour,height,false,"",GL_QUADS,1,QVector3D(1,1,1));
		rendManager.addStaticGeometry2("3d_building",roofRed.contour,height+sideWalkEl,false,"",GL_QUADS,1|mode_AdaptTerrain,QVector3D(1,1,1),colorW);
		rendManager.addStaticGeometry2("3d_building",footprint.contour,height-blackWidth/2.0f+sideWalkEl,false,"",GL_QUADS,1|mode_AdaptTerrain,QVector3D(1,1,1),colorB);
	}

}//
