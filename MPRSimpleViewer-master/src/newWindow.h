#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_newWindow.h"

#include "vtkSmartPointer.h"
#include "vtkResliceImageViewer.h"
#include "vtkImagePlaneWidget.h"
#include "vtkDistanceWidget.h"
#include "vtkResliceImageViewerMeasurements.h"
#include <QMainWindow>

#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include <QImage>
#include <QtMultimedia/QMediaPlayer>
#include <vtkPolyLine.h>
#include "vtkLight.h"

class newWindow : public QMainWindow
{
	Q_OBJECT

public:
	newWindow();
	~newWindow() {};
	vtkSmartPointer<vtkPoints> roadPoints = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkPoints> scpPoints = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkLight> myLight = vtkSmartPointer<vtkLight>::New();
	//int pointID = 0;
	int VBcnt = 0;
	int STLcnt = 0;
	int STLsum = 0;
	double speed = 50;
	bool init = true;

public slots:

	//virtual void InputCursor() {};//vtkSmartPointer<vtkRenderer>);
	virtual void slotExit();
	//virtual void InputCursor();//vtkSmartPointer<vtkRenderer>);

	virtual void assembleNewStl(double*, double*);
	virtual void setVBpose(double*, double*, bool);
	virtual void setVBSphere(double*);
	virtual void stlTrans();

	virtual void videoStop();
	virtual void videoPlay();
	virtual void importFrame();
	virtual void Sim();
	virtual void sim1step();

	virtual void VBfollow();
	virtual void VBstop();
	virtual void VBpause();
	virtual void follow1step();
	virtual void ZoomIn();
	virtual void showMidline();
	virtual void getFiles(std::string, std::vector<std::string>&);

protected:
	vtkSmartPointer< vtkResliceImageViewer > riw[3];
	vtkSmartPointer< vtkImagePlaneWidget > planeWidget[3];
	vtkSmartPointer< vtkDistanceWidget > DistanceWidget[3];
	vtkSmartPointer< vtkResliceImageViewerMeasurements > ResliceMeasurements;
	vtkSmartPointer<vtkRenderer> rendstl;
	vtkSmartPointer<vtkRenderer> rendcls;
	vtkSmartPointer<vtkRenderer> rendvtl;
	vtkSmartPointer<vtkActor> actorSTL;
	vtkSmartPointer<vtkActor> actorCone;
	vtkSmartPointer<vtkActor> actorSphere;
	vtkSmartPointer<vtkActor>actorSphereVB;
	vtkActor* actorDir;
	vtkSmartPointer<vtkActor> actorShape;

private:
	Ui_newWindow* ui;
	cv::VideoCapture capture;
	QTimer* timer;
	QTimer* timerVB;
	QTimer* timerSTL;
	cv::Mat frame;
};