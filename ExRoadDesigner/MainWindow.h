#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "glew.h"
#include <QtGui/QMainWindow>
#include "ui_MainWindow.h"
#include "ControlWidget.h"
#include "PropertyWidget.h"
#include "GLWidget3D.h"
#include "UrbanGeometry.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	static enum { MODE_AREA_SELECT = 0 };

public:
	Ui::MainWindow ui;
	ControlWidget* controlWidget;
	PropertyWidget* propertyWidget;
	GLWidget3D* glWidget;
	UrbanGeometry* urbanGeometry;

	cv::Mat origTerrain;

	int mode;
	//int imgCount;
public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();

protected:
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);

public slots:
	void onNewTerrain();
	void onLoadTerrain();
	void onSaveTerrain();
	void onLoadRoads();
	void onAddRoads();
	void onSaveRoads();
	void onClearRoads();
	void onSaveImage();
	void onSaveImageHD();
	void onLoadCamera();
	void onSaveCamera();
	void onResetCamera();
	void onGenerate2D();
	void onGenerate3D();
	void onGenerate3DRoads();
	void onDisplayRoads();
	void onRenderingDefault();
	void onRenderingTexture();
	void onRenderingGroupBy();
	void onRenderingGenerationType();
	void onShowControlWidget();
	void onShowPropertyWidget();
};

#endif // MAINWINDOW_H
