#include "MainWindow.h"
#include <QFileDialog>
#include <QShortcut>
#include "TerrainSizeInputDialog.h"
#include "GraphUtil.h"
#include "Util.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags) {
	ui.setupUi(this);

	// setup the docking widgets
	controlWidget = new ControlWidget(this);
	propertyWidget = new PropertyWidget(this);

	// setup the toolbar
	ui.fileToolBar->addAction(ui.actionNewTerrain);
	ui.fileToolBar->addAction(ui.actionOpenTerrain);
	ui.areaToolBar->addAction(ui.actionAreaSelect);
	ui.areaToolBar->addAction(ui.actionAreaCreate);
	ui.areaToolBar->addAction(ui.actionHintLine);
	ui.areaToolBar->addAction(ui.actionAvenueSketch);

	ui.actionAreaSelect->setChecked(true);

	// register the menu's action handlers
	connect(ui.actionNewTerrain, SIGNAL(triggered()), this, SLOT(onNewTerrain()));
	connect(ui.actionOpenTerrain, SIGNAL(triggered()), this, SLOT(onLoadTerrain()));
	connect(ui.actionSaveTerrain, SIGNAL(triggered()), this, SLOT(onSaveTerrain()));
	connect(ui.actionLoadRoads, SIGNAL(triggered()), this, SLOT(onLoadRoads()));
	connect(ui.actionAddRoads, SIGNAL(triggered()), this, SLOT(onAddRoads()));
	connect(ui.actionSaveRoads, SIGNAL(triggered()), this, SLOT(onSaveRoads()));
	connect(ui.actionClearRoads, SIGNAL(triggered()), this, SLOT(onClearRoads()));
	connect(ui.actionSaveImage, SIGNAL(triggered()), this, SLOT(onSaveImage()));
	connect(ui.actionSaveImageHD, SIGNAL(triggered()), this, SLOT(onSaveImageHD()));
	connect(ui.actionLoadCamera, SIGNAL(triggered()), this, SLOT(onLoadCamera()));
	connect(ui.actionSaveCamera, SIGNAL(triggered()), this, SLOT(onSaveCamera()));
	connect(ui.actionResetCamera, SIGNAL(triggered()), this, SLOT(onResetCamera()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));

	connect(ui.actionGenerate2D, SIGNAL(triggered()), this, SLOT(onGenerate2D()));
	connect(ui.actionGenerate3D, SIGNAL(triggered()), this, SLOT(onGenerate3D()));
	connect(ui.actionGenerate3DRoads, SIGNAL(triggered()), this, SLOT(onGenerate3DRoads()));
	connect(ui.actionDisplayHighway, SIGNAL(triggered()), this, SLOT(onDisplayRoads()));
	connect(ui.actionDisplayBoulevard, SIGNAL(triggered()), this, SLOT(onDisplayRoads()));
	connect(ui.actionRenderingDefault, SIGNAL(triggered()), this, SLOT(onRenderingDefault()));
	connect(ui.actionRenderingTexture, SIGNAL(triggered()), this, SLOT(onRenderingTexture()));
	connect(ui.actionRenderingGroupBy, SIGNAL(triggered()), this, SLOT(onRenderingGroupBy()));
	connect(ui.actionRenderingGenerationType, SIGNAL(triggered()), this, SLOT(onRenderingGenerationType()));
	connect(ui.actionControlWidget, SIGNAL(triggered()), this, SLOT(onShowControlWidget()));
	connect(ui.actionPropertyWidget, SIGNAL(triggered()), this, SLOT(onShowPropertyWidget()));

	// setup the GL widget
	glWidget = new GLWidget3D(this);
	setCentralWidget(glWidget);

	controlWidget->show();
	addDockWidget(Qt::LeftDockWidgetArea, controlWidget);

	urbanGeometry = new UrbanGeometry(this);

	mode = MODE_AREA_SELECT;
	//imgCount=0;
}

MainWindow::~MainWindow() {
}

void MainWindow::keyPressEvent(QKeyEvent* e) {
	glWidget->keyPressEvent(e);
}

void MainWindow::keyReleaseEvent(QKeyEvent* e) {
	glWidget->keyReleaseEvent(e);
}

void MainWindow::onNewTerrain() {
	TerrainSizeInputDialog dlg(this);
	if (dlg.exec() == QDialog::Accepted) {
		//urbanGeometry->newTerrain(dlg.width, dlg.depth, dlg.cellLength);
		glWidget->vboRenderManager.changeTerrainDimensions(dlg.side,dlg.cellResolution);
		glWidget->updateGL();
	}
}

void MainWindow::onLoadTerrain() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Load Terrain file..."), "", tr("Terrain Files (*.png *.jpg)"));
	if (filename.isEmpty()) return;
	glWidget->vboRenderManager.vboTerrain.loadTerrain(filename);
	//urbanGeometry->loadTerrain(filename);

	glWidget->updateGL();
}

void MainWindow::onSaveTerrain() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Terrain file..."), "", tr("Terrain Files (*.png)"));
	if (filename.isEmpty()) return;
	glWidget->vboRenderManager.vboTerrain.saveTerrain(filename);
	//urbanGeometry->saveTerrain(filename);
}

void MainWindow::onLoadRoads() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Street Map file..."), "", tr("StreetMap Files (*.gsm)"));
	if (filename.isEmpty()) return;

	urbanGeometry->loadRoads(filename);

	glWidget->updateGL();
}

void MainWindow::onAddRoads() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Street Map file..."), "", tr("StreetMap Files (*.gsm)"));
	if (filename.isEmpty()) return;

	urbanGeometry->addRoads(filename);

	glWidget->updateGL();
}

void MainWindow::onSaveRoads() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Street Map file..."), "", tr("StreetMap Files (*.gsm)"));
	if (filename.isEmpty()) return;

	QApplication::setOverrideCursor(Qt::WaitCursor);

	urbanGeometry->saveRoads(filename);

	QApplication::restoreOverrideCursor();
}

void MainWindow::onClearRoads() {
	urbanGeometry->clearRoads();
	glWidget->updateGL();
}

void MainWindow::onSaveImage() {
	if(QDir("screenshots").exists()==false) QDir().mkdir("screenshots");
	QString fileName="screenshots/"+QDate::currentDate().toString("yyMMdd")+"_"+QTime::currentTime().toString("HHmmss")+".png";
	glWidget->grabFrameBuffer().save(fileName);
	printf("Save %s\n",fileName.toAscii().constData());
}//

void MainWindow::onSaveImageHD() {
	if(QDir("screenshots").exists()==false) QDir().mkdir("screenshots");
	QString fileName="screenshots/"+QDate::currentDate().toString("yyMMdd")+"_"+QTime::currentTime().toString("HHmmss")+".png";
	glWidget->grabFrameBuffer().save(fileName);

	QString fileNameHD="screenshots/"+QDate::currentDate().toString("yyMMdd")+"_"+QTime::currentTime().toString("HHmmss")+"_HD.png";
	int cH=glWidget->height();
	int cW=glWidget->width();
	glWidget->resize(cW*3,cH*3);
	glWidget->updateGL();
	glWidget->grabFrameBuffer().save(fileNameHD);
	glWidget->resize(cW,cH);
	glWidget->updateGL();

	printf("Save HD %s\n",fileName.toAscii().constData());
}//

void MainWindow::onLoadCamera() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Camera file..."), "", tr("Area Files (*.cam)"));
	if (filename.isEmpty()) return;

	glWidget->camera->loadCameraPose(filename);
	glWidget->updateCamera();

	glWidget->updateGL();
}

void MainWindow::onSaveCamera() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Camera file..."), "", tr("Area Files (*.cam)"));
	if (filename.isEmpty()) return;
	
	glWidget->camera->saveCameraPose(filename);
}

void MainWindow::onResetCamera() {
	glWidget->camera->resetCamera();
	glWidget->updateCamera();
	glWidget->updateGL();
}

void MainWindow::onGenerate3D() {
	glWidget->generate3DGeometry();
	glWidget->updateGL();
}

void MainWindow::onGenerate3DRoads() {
	glWidget->generate3DGeometry(true);//true just roads
	glWidget->updateGL();
}//

void MainWindow::onGenerate2D() {
	glWidget->generate2DGeometry();
	glWidget->updateGL();
}

void MainWindow::onDisplayRoads() {
	urbanGeometry->roads.showHighways = ui.actionDisplayHighway->isChecked();
	urbanGeometry->roads.showBoulevards = ui.actionDisplayBoulevard->isChecked();
	urbanGeometry->roads.showAvenues = ui.actionDisplayAvenue->isChecked();
	urbanGeometry->roads.showLocalStreets = ui.actionDisplayLocalStreet->isChecked();
	urbanGeometry->roads.setModified();

	glWidget->updateGL();
}

void MainWindow::onRenderingDefault() {
	ui.actionRenderingTexture->setChecked(false);
	ui.actionRenderingGroupBy->setChecked(false);
	ui.actionRenderingGenerationType->setChecked(false);

	urbanGeometry->roads.renderMode = RoadGraph::RENDER_DEFAULT;
	urbanGeometry->roads.setModified();

	glWidget->updateGL();
}

void MainWindow::onRenderingTexture() {
	ui.actionRenderingDefault->setChecked(false);
	ui.actionRenderingGroupBy->setChecked(false);
	ui.actionRenderingGenerationType->setChecked(false);

	urbanGeometry->roads.renderMode = RoadGraph::RENDER_TEXTURE;
	urbanGeometry->roads.setModified();

	glWidget->updateGL();
}

void MainWindow::onRenderingGroupBy() {
	ui.actionRenderingDefault->setChecked(false);
	ui.actionRenderingTexture->setChecked(false);
	ui.actionRenderingGenerationType->setChecked(false);

	urbanGeometry->roads.renderMode = RoadGraph::RENDER_GROUPBY;
	urbanGeometry->roads.setModified();

	glWidget->updateGL();
}

void MainWindow::onRenderingGenerationType() {
	ui.actionRenderingDefault->setChecked(false);
	ui.actionRenderingTexture->setChecked(false);
	ui.actionRenderingGroupBy->setChecked(false);

	urbanGeometry->roads.renderMode = RoadGraph::RENDER_GENERATION_TYPE;
	urbanGeometry->roads.setModified();

	glWidget->updateGL();
}

void MainWindow::onShowControlWidget() {
	controlWidget->show();
	addDockWidget(Qt::LeftDockWidgetArea, controlWidget);
}

void MainWindow::onShowPropertyWidget() {
	propertyWidget->show();
	addDockWidget(Qt::RightDockWidgetArea, propertyWidget);
}
