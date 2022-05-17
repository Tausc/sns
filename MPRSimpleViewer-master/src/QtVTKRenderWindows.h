#ifndef QtVTKRenderWindows_H
#define QtVTKRenderWindows_H

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
#include "newWindow.h"

using namespace cv;

// Forward Qt class declarations
class Ui_QtVTKRenderWindows;

class QtVTKRenderWindows : public QMainWindow
{
  Q_OBJECT
public:
  friend class PointPickerInteractorStyle;
  friend class newWindow;

  vtkSmartPointer<vtkPoints> roadPoints = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkPolyLine> scopeLine = vtkSmartPointer<vtkPolyLine>::New();
  vtkSmartPointer<vtkLight> myLight = vtkSmartPointer<vtkLight>::New();
  vtkSmartPointer<vtkActor> sourceActor;  //将vtkProp类型的对象添加到场景中
  vtkSmartPointer<vtkActor> soluActor;

  newWindow newW;

  int pointID = 0;
  int VBcnt = 0;
  double speed = 100;
  double xErrSum = 0, yErrSum = 0, xErrlast = 0, yErrlast = 0;
  const double Kp = 1, Ki = 0.1, Kd = 0.01;
  bool RisShow = false;

  //int ac;
  //char** av;

  // Constructor/Destructor
  QtVTKRenderWindows(int argc, char* argv[]);
  ~QtVTKRenderWindows() {}

public slots:

  //virtual void InputCursor();//vtkSmartPointer<vtkRenderer>);

  //virtual void drawCone();
  //virtual void drawSphere();
  //virtual void assembleNewStl(double *, double*);
  //virtual void setVBpose(double*, double*, bool);
  //virtual void setVBSphere(double*);

  //virtual void videoStop();
  //virtual void videoPlay();
  //virtual void importFrame();
  virtual void highlightMid();
  virtual int calcMinDel();
  virtual void traceback(int);

  //virtual void VBfollow();
  //virtual void VBstop();
  //virtual void VBpause();
  //virtual void follow1step();

  virtual void slotExit();
  virtual void resliceMode(int);
  virtual void thickMode(int);
  virtual void SetBlendModeToMaxIP();
  virtual void SetBlendModeToMinIP();
  virtual void SetBlendModeToMeanIP();
  virtual void SetBlendMode(int);
  virtual void ResetViews();
  virtual void Render();
  virtual void AddDistanceMeasurementToView1();
  virtual void AddDistanceMeasurementToView( int );

  virtual void ShowMid();
  virtual void getLine(int, bool);
  virtual void getFiles(std::string, std::vector<std::string>&);
  virtual void Control();
  virtual void newWin();
  virtual void stlTrans();
  virtual void registrateMRI();
  virtual void showRegister();

protected:
  vtkSmartPointer< vtkResliceImageViewer > riw[3];
  vtkSmartPointer< vtkImagePlaneWidget > planeWidget[3];
  vtkSmartPointer< vtkDistanceWidget > DistanceWidget[3];
  vtkSmartPointer< vtkResliceImageViewerMeasurements > ResliceMeasurements;
  //vtkSmartPointer<vtkRenderer> rendstl;
  vtkSmartPointer<vtkRenderer> rendvtl;
  vtkSmartPointer<vtkActor> actorCone;
  vtkSmartPointer<vtkActor> actorSTL;
  //vtkSmartPointer<vtkActor> actorSphere;
  vtkSmartPointer<vtkActor>actorSphereVB;
  //int lineId = 0;

protected slots:

private:
  // Designer form
  Ui_QtVTKRenderWindows *ui;
  //cv::VideoCapture capture;
  //QTimer* timer;
  //QTimer* timerVB;
  //cv::Mat frame;
  int id_line=0;
};

#endif // QtVTKRenderWindows_H
