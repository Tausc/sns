#include "ui_QtVTKRenderWindows.h"
#include "QtVTKRenderWindows.h"

#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkAutoInit.h>
#include <vtkSmartPointer.h>
#include <vtkRendererCollection.h>
#include <vtkPointPicker.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>
#include <vtkProperty.h>

#include "vtkResliceImageViewer.h"
#include "vtkResliceCursorLineRepresentation.h"
#include "vtkResliceCursorThickLineRepresentation.h"
#include "vtkResliceCursorWidget.h"
#include "vtkResliceCursorActor.h"
#include "vtkResliceCursorPolyDataAlgorithm.h"
#include "vtkResliceCursor.h"
#include "vtkDICOMImageReader.h"
#include "vtkMetaImageReader.h"
#include "vtkCellPicker.h"
#include "vtkPlane.h"
#include "vtkImageData.h"
#include "vtkCommand.h"
#include "vtkPlaneSource.h"
#include "vtkLookupTable.h"
#include "vtkImageMapToWindowLevelColors.h"
#include "vtkInteractorStyleImage.h"
#include "vtkImageSlabReslice.h"
#include "vtkBoundedPlanePointPlacer.h"
#include "vtkDistanceWidget.h"
#include "vtkDistanceRepresentation.h"
#include "vtkHandleRepresentation.h"
#include "vtkResliceImageViewerMeasurements.h"
#include "vtkDistanceRepresentation2D.h"
#include "vtkPointHandleRepresentation3D.h"
#include "vtkPointHandleRepresentation2D.h"

#include "vtkSTLReader.h"
#include "vtkSTLWriter.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include <vtkConeSource.h>
#include <vtkAppendPolyData.h>
#include <vtkPolyLine.h>
#include "vtkDataSetMapper.h"
#include "vtkUnstructuredGrid.h"
#include "vtkCamera.h"
#include "vtkLight.h"

#include <QtWidgets/QApplication>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include <QImage>

#include <typeinfo> 
#include "nlohmann\json.hpp"

#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include<fstream>  //ifstream
#include<string>     //包含getline()
#include<cmath>
#include "math.h"
#include <Eigen/Core>
#include <Eigen/Dense>

#include <QSlider>
#include <vtkTransform.h>
#include <vtkSphereSource.h>
#include <vtkCallbackCommand.h>
#include <vtkCellPicker.h>
#include <vtkRendererCollection.h>
#include <vtkPointPicker.h>
#include <vtkAxesActor.h>
#include <QtMultimedia/QMediaPlayer>
//#include <QVideoWidget>
#include "newWindow.h"

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);
#include <vtkSimplePointsReader.h>
#include <vtkIterativeClosestPointTransform.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkLandmarkTransform.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkOrientationMarkerWidget.h> //坐标系交互

#include <vtkImageDataGeometryFilter.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkStructuredPointsReader.h>
#include <vtkPolyDataReader.h>

#include <vtkPointPicker.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <vtkVertexGlyphFilter.h>
#include "InputDataExtract.h"


using json = nlohmann::json;

using namespace cv;

double leisonX = 0, leisonY = 0, leisonZ = 0;
//draw sphere
vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
vtkSmartPointer<vtkActor> actorSphere;
vtkSmartPointer<vtkRenderer> rendstl;
vtkSmartPointer<vtkRenderer> rendcls;

//----------------------------------------------------------------------------
class vtkResliceCursorCallback : public vtkCommand
{
public:

  static vtkResliceCursorCallback *New()
  { return new vtkResliceCursorCallback; }

  void Execute( vtkObject *caller, unsigned long ev,
                void *callData )
    {
    if (ev == vtkResliceCursorWidget::WindowLevelEvent ||
        ev == vtkCommand::WindowLevelEvent ||
        ev == vtkResliceCursorWidget::ResliceThicknessChangedEvent)
      {
      // Render everything
      for (int i = 0; i < 3; i++)
        {
        this->RCW[i]->Render();
        }
      this->IPW[0]->GetInteractor()->GetRenderWindow()->Render();
      return;
      }

    vtkImagePlaneWidget* ipw =
      dynamic_cast< vtkImagePlaneWidget* >( caller );
    if (ipw)
      {
      double* wl = static_cast<double*>( callData );

      if ( ipw == this->IPW[0] )
        {
        this->IPW[1]->SetWindowLevel(wl[0],wl[1],1);
        this->IPW[2]->SetWindowLevel(wl[0],wl[1],1);
        }
      else if( ipw == this->IPW[1] )
        {
        this->IPW[0]->SetWindowLevel(wl[0],wl[1],1);
        this->IPW[2]->SetWindowLevel(wl[0],wl[1],1);
        }
      else if (ipw == this->IPW[2])
        {
        this->IPW[0]->SetWindowLevel(wl[0],wl[1],1);
        this->IPW[1]->SetWindowLevel(wl[0],wl[1],1);
        }
      }

    vtkResliceCursorWidget *rcw = dynamic_cast<
      vtkResliceCursorWidget * >(caller);
    if (rcw)
      {
      vtkResliceCursorLineRepresentation *rep = dynamic_cast<
        vtkResliceCursorLineRepresentation * >(rcw->GetRepresentation());
      // Although the return value is not used, we keep the get calls
      // in case they had side-effects
      rep->GetResliceCursorActor()->GetCursorAlgorithm()->GetResliceCursor();
      for (int i = 0; i < 3; i++)
        {
        vtkPlaneSource *ps = static_cast< vtkPlaneSource * >(
            this->IPW[i]->GetPolyDataAlgorithm());
        ps->SetOrigin(this->RCW[i]->GetResliceCursorRepresentation()->
                                          GetPlaneSource()->GetOrigin());
        ps->SetPoint1(this->RCW[i]->GetResliceCursorRepresentation()->
                                          GetPlaneSource()->GetPoint1());
        ps->SetPoint2(this->RCW[i]->GetResliceCursorRepresentation()->
                                          GetPlaneSource()->GetPoint2());

        // If the reslice plane has modified, update it on the 3D widget
        this->IPW[i]->UpdatePlacement();
        }
      }

    // Render everything
    for (int i = 0; i < 3; i++)
      {
      this->RCW[i]->Render();
      }
    this->IPW[0]->GetInteractor()->GetRenderWindow()->Render();
    }

  vtkResliceCursorCallback() {}
  vtkImagePlaneWidget* IPW[3];
  vtkResliceCursorWidget *RCW[3];
};

// PointPickerInteractorStyle
class PointPickerInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static PointPickerInteractorStyle* New();
    vtkTypeMacro(PointPickerInteractorStyle, vtkInteractorStyleTrackballCamera);

    virtual void OnLeftButtonDown()
    {
        std::cout << "Picking pixel: " << this->Interactor->GetEventPosition()[0] << " " << this->Interactor->GetEventPosition()[1] << std::endl;
        this->Interactor->GetPicker()->Pick(this->Interactor->GetEventPosition()[0],
            this->Interactor->GetEventPosition()[1],
            0,  // always zero.
            this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
        double picked[3];
        this->Interactor->GetPicker()->GetPickPosition(picked);
        leisonX = picked[0];
        leisonY = picked[1];
        leisonZ = picked[2];
        std::cout << "Picked value: " << picked[0] << " " << picked[1] << " " << picked[2] << std::endl;
        actorSphere->SetPosition(leisonX, leisonY, leisonZ);
        rendstl->AddActor(actorSphere);
        rendcls->AddActor(actorSphere);

        //set view8
        vtkCamera* camera8 = vtkCamera::New();
        camera8->SetViewUp(0, 0, 1);//设视角位置 
        //camera8->SetPosition(leisonX, leisonY+90, leisonZ);//设观察对象位
        camera8->SetFocalPoint(leisonX, leisonY, leisonZ);//设焦点 
        camera8->ComputeViewPlaneNormal();//自动
        rendcls->SetActiveCamera(camera8);

        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }
};

vtkStandardNewMacro(PointPickerInteractorStyle);


QtVTKRenderWindows::QtVTKRenderWindows( int vtkNotUsed(argc), char *argv[])
{
  this->ui = new Ui_QtVTKRenderWindows;
  this->ui->setupUi(this);
  cout << "test if save" << endl;
  //stl
  vtkSmartPointer<vtkSTLReader> readstl = 
      vtkSmartPointer<vtkSTLReader>::New();
  rendstl = vtkSmartPointer<vtkRenderer>::New();

  readstl->SetFileName("D:/F/201/STL/aim.stl");
  readstl->Update();

  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(readstl->GetOutputPort());

  actorSTL = vtkSmartPointer<vtkActor>::New();
  actorSTL->SetMapper(mapper);
  actorSTL->GetProperty()->SetOpacity(0.3);//透明度
  rendstl->AddActor(actorSTL);

  //camera
  vtkCamera* aCamera1 = vtkCamera::New();
  aCamera1->SetPosition(0, 400, -80);//设观察对象位
  aCamera1->SetFocalPoint(0, -500, 50);//设焦点 
  aCamera1->ComputeViewPlaneNormal();//自动
  rendstl->SetActiveCamera(aCamera1);

  //pickup
  vtkSmartPointer<vtkPointPicker> pointPicker =
      vtkSmartPointer<vtkPointPicker>::New();

  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetPicker(pointPicker);
  renderWindowInteractor->SetRenderWindow(this->ui->view5->GetRenderWindow());

  vtkSmartPointer<PointPickerInteractorStyle> style =
      vtkSmartPointer<PointPickerInteractorStyle>::New();
  renderWindowInteractor->SetInteractorStyle(style);

  this->ui->view5->GetRenderWindow()->SetInteractor(renderWindowInteractor);

  this->ui->view5->GetRenderWindow()->Render();
  this->ui->view5->GetRenderWindow()->AddRenderer(rendstl);
      
  //vtkRenderWindowInteractor* iren = this->ui->view4->GetInteractor();
  this->ui->view5->GetRenderWindow()->Render();


  //close stl
  rendcls = vtkSmartPointer<vtkRenderer>::New();
  rendcls->AddActor(actorSTL);

  //camera
  vtkCamera* aCamera3 = vtkCamera::New();
  aCamera3->SetPosition(0, 100, -20);//设观察对象位
  aCamera3->SetFocalPoint(0, -100, 50);//设焦点 
  aCamera3->ComputeViewPlaneNormal();//自动
  rendcls->SetActiveCamera(aCamera3);




  //vitual stl
  vtkSmartPointer<vtkSTLReader> readvtl =
      vtkSmartPointer<vtkSTLReader>::New();
  rendvtl = vtkSmartPointer<vtkRenderer>::New();

  readvtl->SetFileName("D:/F/201/STL/aim.stl");
  readvtl->Update();

  vtkSmartPointer<vtkPolyDataMapper> mapperV = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapperV->SetInputConnection(readvtl->GetOutputPort());

  vtkSmartPointer<vtkActor> actorV = vtkSmartPointer<vtkActor>::New();
  actorV->SetMapper(mapperV);
  actorV->GetProperty()->SetColor(1.7, 0.98, 0.87);
  actorV->GetProperty()->SetAmbient(0.4);
  actorV->GetProperty()->SetDiffuse(0.3);
  actorV->GetProperty()->SetSpecular(0.5);
  rendvtl->AddActor(actorV);

  //camera
  vtkCamera* aCamera = vtkCamera::New();
  //aCamera->SetViewUp(0, 0, -1);//设视角位置 
  aCamera->SetPosition(20, -140, 100);//设观察对象位
  aCamera->SetFocalPoint(20, -140, 10);//设焦点 
  aCamera->ComputeViewPlaneNormal();//自动
  rendvtl->SetActiveCamera(aCamera);

  //light
  myLight->SetColor(1, 1, 1);
  myLight->SetLightTypeToHeadlight();
  myLight->SetIntensity(0.4);
  //myLight->SetPosition(15, -140, 100);
  myLight->SetFocalPoint(rendvtl->GetActiveCamera()->GetFocalPoint());

  rendvtl->AddLight(myLight);

  //vtkSmartPointer< vtkDICOMImageReader > reader =
  //  vtkSmartPointer< vtkDICOMImageReader >::New();
  //reader->SetDirectoryName("F:/LIDC_DatasetRefinement_20210322/1.3.6.1.4.1.14519.5.2.1.6279.6001.205852555362702089950453265567/Input");//(argv[1]);
  //reader->Update();
  //int imageDims[3];
  //reader->GetOutput()->GetDimensions(imageDims);

  
  //mha
   vtkSmartPointer< vtkMetaImageReader > reader =
     vtkSmartPointer< vtkMetaImageReader >::New();
   reader->SetFileName("D:/F/JIAO DA_TEST/2.mha");//(argv[1]);
   reader->Update();
   int imageDims[3];
   reader->GetOutput()->GetDimensions(imageDims);
    
  for (int i = 0; i < 3; i++)
    {
    riw[i] = vtkSmartPointer< vtkResliceImageViewer >::New();
    }

  this->ui->view1->SetRenderWindow(riw[0]->GetRenderWindow());
  riw[0]->SetupInteractor(
      this->ui->view1->GetRenderWindow()->GetInteractor());

  this->ui->view2->SetRenderWindow(riw[1]->GetRenderWindow());
  riw[1]->SetupInteractor(
      this->ui->view2->GetRenderWindow()->GetInteractor());

  this->ui->view3->SetRenderWindow(riw[2]->GetRenderWindow());
  riw[2]->SetupInteractor(
      this->ui->view3->GetRenderWindow()->GetInteractor());

  for (int i = 0; i < 3; i++)
    {
    // make them all share the same reslice cursor object.
    vtkResliceCursorLineRepresentation *rep =
      vtkResliceCursorLineRepresentation::SafeDownCast(
          riw[i]->GetResliceCursorWidget()->GetRepresentation());
    riw[i]->SetResliceCursor(riw[0]->GetResliceCursor());

    rep->GetResliceCursorActor()->
      GetCursorAlgorithm()->SetReslicePlaneNormal(i);

    riw[i]->SetInputData(reader->GetOutput());
    riw[i]->SetSliceOrientation(i);
    riw[i]->SetResliceModeToAxisAligned();
    }

  vtkSmartPointer<vtkCellPicker> picker =
    vtkSmartPointer<vtkCellPicker>::New();
  picker->SetTolerance(0.005);

  vtkSmartPointer<vtkProperty> ipwProp =
    vtkSmartPointer<vtkProperty>::New();

  vtkSmartPointer< vtkRenderer > ren =
    vtkSmartPointer< vtkRenderer >::New();

  //this->ui->view7->GetRenderWindow()->AddRenderer(rendvtl);
  this->ui->view8->GetRenderWindow()->AddRenderer(rendcls);
  this->ui->view4->GetRenderWindow()->AddRenderer(ren);
  vtkRenderWindowInteractor *iren = this->ui->view4->GetInteractor();

  //this->ui->view5->GetRenderWindow()->AddRenderer(rendstl);
  //vtkRenderWindowInteractor* iren5 = this->ui->view5->GetInteractor();

  for (int i = 0; i < 3; i++)
    {
    planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
    planeWidget[i]->SetInteractor( iren );
    planeWidget[i]->SetPicker(picker);
    planeWidget[i]->RestrictPlaneToVolumeOn();
    double color[3] = {0, 0, 0};
    color[i] = 1;
    planeWidget[i]->GetPlaneProperty()->SetColor(color);

    color[0] /= 4.0;
    color[1] /= 4.0;
    color[2] /= 4.0;
    riw[i]->GetRenderer()->SetBackground( color );

    planeWidget[i]->SetTexturePlaneProperty(ipwProp);
    planeWidget[i]->TextureInterpolateOff();
    planeWidget[i]->SetResliceInterpolateToLinear();
    planeWidget[i]->SetInputConnection(reader->GetOutputPort());
    planeWidget[i]->SetPlaneOrientation(i);
    planeWidget[i]->SetSliceIndex(imageDims[i]/2);
    planeWidget[i]->DisplayTextOn();
    planeWidget[i]->SetDefaultRenderer(ren);
    planeWidget[i]->SetWindowLevel(1358, -27);
    planeWidget[i]->On();
    planeWidget[i]->InteractionOn();
    }

  vtkSmartPointer<vtkResliceCursorCallback> cbk =
    vtkSmartPointer<vtkResliceCursorCallback>::New();

  for (int i = 0; i < 3; i++)
    {
    cbk->IPW[i] = planeWidget[i];
    cbk->RCW[i] = riw[i]->GetResliceCursorWidget();
    riw[i]->GetResliceCursorWidget()->AddObserver(
        vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk );
    riw[i]->GetResliceCursorWidget()->AddObserver(
        vtkResliceCursorWidget::WindowLevelEvent, cbk );
    riw[i]->GetResliceCursorWidget()->AddObserver(
        vtkResliceCursorWidget::ResliceThicknessChangedEvent, cbk );
    riw[i]->GetResliceCursorWidget()->AddObserver(
        vtkResliceCursorWidget::ResetCursorEvent, cbk );
    riw[i]->GetInteractorStyle()->AddObserver(
        vtkCommand::WindowLevelEvent, cbk );

    // Make them all share the same color map.
    riw[i]->SetLookupTable(riw[0]->GetLookupTable());
    planeWidget[i]->GetColorMap()->SetLookupTable(riw[0]->GetLookupTable());
    //planeWidget[i]->GetColorMap()->SetInput(riw[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->GetColorMap()->GetInput());
    planeWidget[i]->SetColorMap(riw[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->GetColorMap());

    }

  this->ui->view1->show();
  this->ui->view2->show();
  this->ui->view3->show();

  // Set up action signals and slots
  connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
  connect(this->ui->resliceModeCheckBox, SIGNAL(stateChanged(int)), this, SLOT(resliceMode(int)));
  connect(this->ui->thickModeCheckBox, SIGNAL(stateChanged(int)), this, SLOT(thickMode(int)));
  //this->ui->thickModeCheckBox->setEnabled(0);
  this->ui->thickModeCheckBox->hide();

  connect(this->ui->radioButton_Max, SIGNAL(pressed()), this, SLOT(SetBlendModeToMaxIP()));
  connect(this->ui->radioButton_Min, SIGNAL(pressed()), this, SLOT(SetBlendModeToMinIP()));
  connect(this->ui->radioButton_Mean, SIGNAL(pressed()), this, SLOT(SetBlendModeToMeanIP()));
  //this->ui->blendModeGroupBox->setEnabled(0);
  this->ui->blendModeGroupBox->hide();

  connect(this->ui->resetButton, SIGNAL(pressed()), this, SLOT(ResetViews()));
  connect(this->ui->AddDistance1Button, SIGNAL(pressed()), this, SLOT(AddDistanceMeasurementToView1()));
  //connect(this->ui->SetCursorButton, SIGNAL(pressed()), this, SLOT(InputCursor()));
  connect(this->ui->leisonSet, SIGNAL(pressed()), this, SLOT(highlightMid()));

  //connect(this->ui->videoButton, SIGNAL(pressed()), this, SLOT(videoPlay()));
  //connect(this->ui->videoButton2, SIGNAL(pressed()), this, SLOT(videoStop()));

  connect(this->ui->midButton, SIGNAL(pressed()), this, SLOT(ShowMid()));
  connect(this->ui->leisonSet, SIGNAL(pressed()), this, SLOT(highlightMid()));
  connect(this->ui->register_2, SIGNAL(pressed()), this, SLOT(showRegister()));
  connect(this->ui->slider, SIGNAL(valueChanged(int)), this, SLOT(stlTrans()));

  connect(this->ui->newWin, SIGNAL(pressed()), this, SLOT(newWin()));

  //connect(this->ui->VBfollow, SIGNAL(pressed()), this, SLOT(VBfollow()));
  //connect(this->ui->VBstop, SIGNAL(pressed()), this, SLOT(VBstop()));
  //connect(this->ui->VBpause, SIGNAL(pressed()), this, SLOT(VBpause()));

  //timer = new QTimer(this);
  //connect(timer, SIGNAL(timeout()), this, SLOT(importFrame()));//import frame when timeout

  //timerVB = new QTimer(this);
  //connect(timerVB, SIGNAL(timeout()), this, SLOT(follow1step()));//import frame when timeout

  //draw sphere
  sphere->SetRadius(5.0);             // 设置半径
  sphere->Update();

  vtkSmartPointer<vtkPolyDataMapper> mapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper2->SetInputConnection(sphere->GetOutputPort());

  actorSphere = vtkSmartPointer<vtkActor>::New();
  actorSphere->SetMapper(mapper2);
  actorSphere->GetProperty()->SetColor(0.9, 0.7, 0);

  //draw cone
  //vtkSmartPointer<vtkConeSource> cone =
  //    vtkSmartPointer<vtkConeSource>::New();
  //cone->SetHeight(20.0);
  //cone->SetRadius(10.0);
  //cone->SetResolution(20);
  //cone->Update();
  //
  //vtkSmartPointer<vtkPolyDataMapper> mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
  //mapper1->SetInputConnection(cone->GetOutputPort());
  //
  //actorCone = vtkSmartPointer<vtkActor>::New();
  //actorCone->SetMapper(mapper1);
  //actorCone->GetProperty()->SetColor(0, 1, 0);
  //
  //
  //double point[3] = { 20,-140,100 };
  //double pose[3] = { 0,90,0 };
  //
  ////drawCone();
  //assembleNewStl(point, pose);
  //
  //draw VBsphere
  //vtkSmartPointer<vtkSphereSource> VBsphere =
  //    vtkSmartPointer<vtkSphereSource>::New();
  //VBsphere->SetRadius(0.2);
  //VBsphere->Update();
  //
  //vtkSmartPointer<vtkPolyDataMapper> mapper3 = vtkSmartPointer<vtkPolyDataMapper>::New();
  //mapper3->SetInputConnection(VBsphere->GetOutputPort());
  //
  //actorSphereVB = vtkSmartPointer<vtkActor>::New();
  ////actorSphereVB->GetProperty()->SetOpacity(0.8);//透明度
  //actorSphereVB->SetMapper(mapper3);
  //actorSphereVB->GetProperty()->SetColor(1, 0, 1);
  //
  //this->ui->newWin->hide();

  registrateMRI();
 };

//void QtVTKRenderWindows::VBfollow()
//{
//    int interval = round(speed);
//    timerVB->start(speed);
//}
//
//void QtVTKRenderWindows::VBstop()
//{
//    VBcnt = roadPoints->GetNumberOfPoints() - 1;
//    timerVB->stop();
//    follow1step();
//    timerVB->stop();
//    //VBpause();
//}
//
//void QtVTKRenderWindows::VBpause()
//{
//    //int remaining = timerVB->remainingTime();
//    timerVB->stop();
//    //timerVB->setInterval(remaining);
//}
//
//void QtVTKRenderWindows::follow1step()
//{
//    if (VBcnt < 20)
//    {
//        timerVB->stop();
//        return;
//    }
//    cout << "VBcnt:  " << VBcnt<<endl;
//    double x = roadPoints->GetPoint(VBcnt)[0];
//    double y = roadPoints->GetPoint(VBcnt)[1];
//    double z = roadPoints->GetPoint(VBcnt)[2];
//    double point[3] = { x,y,z };
//    int j = VBcnt - 10;
//    double a = roadPoints->GetPoint(j)[0] - x;
//    double b = roadPoints->GetPoint(j)[1] - y;
//    double c = roadPoints->GetPoint(j)[2] - z;
//    double pose[3] = { a,b,c };
//    int k = VBcnt - 20;
//    double pointCone[3] = {0};
//    pointCone[0] = roadPoints->GetPoint(k)[0];
//    pointCone[1] = roadPoints->GetPoint(k)[1];
//    pointCone[2] = roadPoints->GetPoint(k)[2];
//
//    setVBpose(point, pose, true);
//
//    Eigen::Matrix3d rotMatrix;
//    Eigen::Vector3d vectorBefore(0, 0, -1);
//    Eigen::Vector3d vectorAfter(a, b, c);
//    vectorAfter.normalize();
//    cout << a << b << c;
//    cout << "vector:  " << vectorAfter.transpose() << endl;
//
//    rotMatrix = Eigen::Quaterniond::FromTwoVectors(vectorBefore, vectorAfter).toRotationMatrix();
//    cout << "rotMatrix =\n" << rotMatrix << endl;
//
//    Eigen::Vector3d ea = rotMatrix.eulerAngles(2,1,0);
//
//    cout << "euler_angle.transpose() = " << ea.transpose() << endl << endl;
//    //yaw pitch roll  Z Y X
//    a = ea(0) / 3.14 * 180;
//    b = 90 + ea(1) / 3.14 * 180;
//    c = ea(2) / 3.14 * 180;
//    double euler[3] = { a,b,c };
//    //!!!!!!!!透明度拉条
//    assembleNewStl(point, euler);
//
//    setVBSphere(pointCone);
//    VBcnt--;
//}
 //
 //
//void QtVTKRenderWindows::InputCursor()//vtkSmartPointer<vtkRenderer> rend1)
//{
//    double x = 20 + this->ui->doubleSpinBox_x->value();
//    double y = this->ui->doubleSpinBox_y->value() - 140;
//    double z = this->ui->doubleSpinBox_z->value() + 100;
//    double point[3] = { x,y,z };
//
//    double a = this->ui->doubleSpinBox_a->value();
//    double b = 90 + this->ui->doubleSpinBox_b->value();
//    double c = this->ui->doubleSpinBox_c->value();
//    double pose[3] = { a,b,c };
//
//    //drawCone();
//    assembleNewStl(point, pose);
//    //setVBpose(point, pose, false);
//
//    char bronch[100];
//    sprintf(bronch, "bronchoscope pose:\n    %f, %f, %f", x,y,z);
//
//    //this->ui->textBrowser->append(bronch);
//
//};
 //
//void QtVTKRenderWindows::setVBpose(double *point, double *pose,bool isFollow=false)
//{
//    Eigen::Vector3d cameraV;
//    double x = point[0];
//    double y = point[1];
//    double z = point[2];
//
//    double a = pose[0];
//    double b = pose[1];
//    double c = pose[2];
//
//    if (isFollow) 
//    { 
//        cameraV << a, b, c; 
//        cameraV.normalize();
//    }
//    else
//    {
//        double total = a + b + c;
//        a = fmod(a, 360);
//        b = fmod(b, 360);
//        c = fmod(c, 360);
//        
//        cameraV << 0, 0, -1;
//
//        Eigen::Vector3d ea(a * 0.01745, (b - 90) * 0.01745, c * 0.01745); 
//
//        //欧拉角转换为旋转矩阵 0 1 2 对应 z y x
//        Eigen::Matrix3d rotation_matrix_4;
//        rotation_matrix_4 = Eigen::AngleAxisd(ea[0], Eigen::Vector3d::UnitZ()) *
//            Eigen::AngleAxisd(ea[1], Eigen::Vector3d::UnitY()) *
//            Eigen::AngleAxisd(ea[2], Eigen::Vector3d::UnitX());
//        cout << "rotation matrix_4 =\n" << rotation_matrix_4 << endl;
//
//        cameraV = rotation_matrix_4 * cameraV;
//    }
//
//    cameraV = cameraV * 100;
//    //set camera
//    vtkCamera* aCamera = vtkCamera::New();
//    //aCamera->SetViewUp(a, b, c);//设视角位置 
//    aCamera->SetPosition(x, y, z);//设观察对象位
//    //aCamera->SetFocalPoint(x, y, z-1);
//    aCamera->SetFocalPoint(x + cameraV[0], y + cameraV[1], z + cameraV[2]);//设焦点 
//    aCamera->ComputeViewPlaneNormal();//自动
//    rendvtl->SetActiveCamera(aCamera);
//
//    this->ui->view7->update();
//}
//
//void QtVTKRenderWindows::setVBSphere(double* point)
//{
//    double x = point[0];
//    double y = point[1];
//    double z = point[2];
//
//    actorSphereVB->SetPosition(x, y, z);
//    rendvtl->AddActor(actorSphereVB);
//
//    this->ui->view7->update();
//}
//
//void QtVTKRenderWindows::assembleNewStl(double *point, double *pose)
//{
//    double x = point[0];
//    double y = point[1];
//    double z = point[2];
//
//    double a = pose[0];
//    double b = pose[1];
//    double c = pose[2];
//
//    actorCone->SetPosition(x, y, z);
//    actorCone->SetOrientation(a,b,c);
//    rendstl->AddActor(actorCone);
//    rendcls->AddActor(actorCone);
//
//    this->ui->view5->update();
//    this->ui->view8->update();
//}

void QtVTKRenderWindows::stlTrans()
{
    double pos = this->ui->slider->value();
    double p = pos / 100;
    //cout << "pos" << pos << "p" << p << endl;
    actorSTL->GetProperty()->SetOpacity(p);
    rendstl->AddActor(actorSTL);
    rendcls->AddActor(actorSTL);
    this->ui->view5->update();
    this->ui->view8->update();
}

//no use unless first time to run(no cone stl)
/*
void QtVTKRenderWindows::drawCone()
{
    std::string filename = "D:/F/201/STL/cone.stl";

    vtkSmartPointer<vtkConeSource> cone =
        vtkSmartPointer<vtkConeSource>::New();
    cone->SetHeight(20.0);
    cone->SetRadius(10.0);
    cone->Update();

    vtkSmartPointer<vtkSTLWriter> stlWriter =
        vtkSmartPointer<vtkSTLWriter>::New();
    stlWriter->SetFileName(filename.c_str());
    stlWriter->SetInputConnection(cone->GetOutputPort());
    stlWriter->Write();
}

void QtVTKRenderWindows::drawSphere()
{
    std::string filename = "D:/F/201/STL/sphere.stl";

    vtkSmartPointer<vtkSphereSource> sphere =
        vtkSmartPointer<vtkSphereSource>::New();
    sphere->SetRadius(5.0);             // 设置半径
    sphere->Update();

    vtkSmartPointer<vtkSTLWriter> stlWriter =
        vtkSmartPointer<vtkSTLWriter>::New();
    stlWriter->SetFileName(filename.c_str());
    stlWriter->SetInputConnection(sphere->GetOutputPort());
    stlWriter->Write();
}
*/

//void QtVTKRenderWindows::importFrame()
//{
//    capture >> frame;
//    if (!frame.empty()) { cvtColor(frame, frame, CV_BGR2RGB); }//only RGB of Qt
//    else { videoStop(); }
//    QImage srcQImage = QImage((uchar*)(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);
//    this->ui->view6->setPixmap(QPixmap::fromImage(srcQImage));
//    this->ui->view6->resize(srcQImage.size());
//    this->ui->view6->show();
//}
//
//void QtVTKRenderWindows::videoPlay()
//{
//    capture.open("D:\\F\\201\\line\\3.avi");
//    timer->start(33);// Start timing, Signal out when timeout 33
//
//    //QMediaPlayer* player;
//    //player->setMedia(QMediaContent(QUrl::fromLocalFile("D:\\F\\201\\line\\3.avi")));
//    //player->setVideoOutput(this->ui->view6);
//}
//
//void QtVTKRenderWindows::videoStop()
//{
//    timer->stop();
//    capture.release();
//}

void QtVTKRenderWindows::highlightMid()
{
    pointID = 0;

    //roadPoints->InsertPoint(pointID, 0, 0, 0);		//_加入点信息
    roadPoints->Reset();

    int id = calcMinDel();
    //std::cout << std::endl<<point[0] << point[1] << point[2] << "id"<<point[3];
    std::cout << "find closest point!" << std::endl;
    traceback(id);

    VBcnt = roadPoints->GetNumberOfPoints() - 1;

    ofstream ofs;
    ofs.open("D:\\mid.txt", ios::out);
    for (int i = 0; i < (VBcnt+1); i++) {
        double x = roadPoints->GetPoint(i)[0];
        double y = roadPoints->GetPoint(i)[1];
        double z = roadPoints->GetPoint(i)[2];
        ofs << x << "\t" << y << "\t" << z << endl;  //endl用于换行
    }
    ofs.close();

    ofstream lei;
    lei.open("D:\\leison.txt", ios::out);
    lei << leisonX << "\t" << leisonY << "\t" << leisonZ << endl;  //endl用于换行
    lei.close();

    actorSphere->SetPosition(leisonX, leisonY, leisonZ);
    rendstl->AddActor(actorSphere);
    rendcls->AddActor(actorSphere);

    //set view8
    vtkCamera* camera8 = vtkCamera::New();
    camera8->SetViewUp(0, 0, 1);//设视角位置 
    //camera8->SetPosition(leisonX, leisonY+90, leisonZ);//设观察对象位
    camera8->SetFocalPoint(leisonX, leisonY, leisonZ);//设焦点 
    camera8->ComputeViewPlaneNormal();//自动
    rendcls->SetActiveCamera(camera8);

    this->ui->view8->update();
    this->ui->view5->update();
    this->ui->newWin->show();
}

int QtVTKRenderWindows::calcMinDel()
{
    json j;
    double a = 0, b = 0, c = 0;
    //std::cout << "print again: " << leisonX << " " << leisonY << " " << leisonZ << std::endl;
    double x = leisonX;//this->ui->leison_x->value();
    double y = leisonY;//this->ui->leison_y->value();
    double z = leisonZ;//this->ui->leison_z->value();
    cout << x << " " << y << " " << z << " " << endl;

    double point[4] = { 0 };
    double mini = 0;

    std::string filePath = "D:\\F\\InsertedLines\\";//"D:\\F\\MidLine\\";
    std::vector<std::string> files;
    getFiles(filePath, files);
    //std::cout << std::endl << "111";

    int size = files.size();

    for (int i = 0; i < size-1; i++)
    {
        cout << files[i].c_str() << endl;  //第i条

        std::ifstream jfile(files[i].c_str());
        jfile >> j;
        json j2 = j["markups"][0].at("controlPoints");

        for (int k = 0; k < j2.size(); k++) 
        {   //其中的第k个点
            //std::cout << std::endl << k;
            a = j2[k].at("position")[0];
            b = j2[k].at("position")[1];
            c = j2[k].at("position")[2];

            double del = pow(a-x, 2) + pow(b - y, 2)+ pow(c - z, 2);
            if (mini == 0)mini = del;
            if (del <= mini) 
            { 
                mini = del; 
                id_line = i;
                //std::cout << std::endl <<i<< "mini" << point[3];
            }
        }
    }
    //std::cout << std::endl << "IDIDIDIDIDID" << point[0] << point[1] << point[2] << "id"<<point[3];
    return id_line;
}

void QtVTKRenderWindows::traceback(int id)
{
    getLine(id, 1);
    if (id == 0)return;

    json j;
    std::string filePath = "D:\\F\\InsertedLines\\";//"D:\\F\\MidLine\\";
    std::vector<std::string> files;
    getFiles(filePath, files);

    std::ifstream jfile(files[id].c_str());
    jfile >> j;
    json j2 = j["markups"][0].at("controlPoints");

    double a = j2[0].at("position")[0];
    double b = j2[0].at("position")[1];
    double c = j2[0].at("position")[2];


    int size = files.size();
    for (int i = 0; i < size-1; i++)
    {
        std::ifstream jfile(files[i].c_str());
        jfile >> j;
        json j2 = j["markups"][0].at("controlPoints");
        int s = j2.size()-1;
        double x = j2[s].at("position")[0];
        double y = j2[s].at("position")[1];
        double z = j2[s].at("position")[2];

        if ((a == x) && (b == y) && (c == z))
        {
            int idex = i;
            traceback(idex);
        };
    }
}

void QtVTKRenderWindows::slotExit()
{
  qApp->exit();
}

void QtVTKRenderWindows::resliceMode(int mode)
{
  this->ui->thickModeCheckBox->setEnabled(mode ? 1 : 0);
  this->ui->blendModeGroupBox->setEnabled(mode ? 1 : 0);

  for (int i = 0; i < 3; i++)
    {
    riw[i]->SetResliceMode(mode ? 1 : 0);
    riw[i]->GetRenderer()->ResetCamera();
    riw[i]->Render();
    }
}

void QtVTKRenderWindows::thickMode(int mode)
{
  for (int i = 0; i < 3; i++)
    {
    riw[i]->SetThickMode(mode ? 1 : 0);
    riw[i]->Render();
    }
}

void QtVTKRenderWindows::SetBlendMode(int m)
{
  for (int i = 0; i < 3; i++)
    {
    vtkImageSlabReslice *thickSlabReslice = vtkImageSlabReslice::SafeDownCast(
        vtkResliceCursorThickLineRepresentation::SafeDownCast(
          riw[i]->GetResliceCursorWidget()->GetRepresentation())->GetReslice());
    thickSlabReslice->SetBlendMode(m);
    riw[i]->Render();
    }
}

void QtVTKRenderWindows::SetBlendModeToMaxIP()
{
  this->SetBlendMode(VTK_IMAGE_SLAB_MAX);
}

void QtVTKRenderWindows::SetBlendModeToMinIP()
{
  this->SetBlendMode(VTK_IMAGE_SLAB_MIN);
}

void QtVTKRenderWindows::SetBlendModeToMeanIP()
{
  this->SetBlendMode(VTK_IMAGE_SLAB_MEAN);
}

void QtVTKRenderWindows::ResetViews()
{
  // Reset the reslice image views
  for (int i = 0; i < 3; i++)
    {
    riw[i]->Reset();
    }

  // Also sync the Image plane widget on the 3D top right view with any
  // changes to the reslice cursor.
  for (int i = 0; i < 3; i++)
    {
    vtkPlaneSource *ps = static_cast< vtkPlaneSource * >(
        planeWidget[i]->GetPolyDataAlgorithm());
    ps->SetNormal(riw[0]->GetResliceCursor()->GetPlane(i)->GetNormal());
    ps->SetCenter(riw[0]->GetResliceCursor()->GetPlane(i)->GetOrigin());

    // If the reslice plane has modified, update it on the 3D widget
    this->planeWidget[i]->UpdatePlacement();
    }

  // Render in response to changes.
  this->Render();
}

void QtVTKRenderWindows::Render()
{
  for (int i = 0; i < 3; i++)
    {
    riw[i]->Render();
    }
  this->ui->view3->GetRenderWindow()->Render();
}

void QtVTKRenderWindows::AddDistanceMeasurementToView1()
{
  this->AddDistanceMeasurementToView(1);
}

void QtVTKRenderWindows::AddDistanceMeasurementToView(int i)
{
  // remove existing widgets.
  if (this->DistanceWidget[i])
    {
    this->DistanceWidget[i]->SetEnabled(0);
    this->DistanceWidget[i] = NULL;
    }

  // add new widget
  this->DistanceWidget[i] = vtkSmartPointer< vtkDistanceWidget >::New();
  this->DistanceWidget[i]->SetInteractor(
    this->riw[i]->GetResliceCursorWidget()->GetInteractor());

  // Set a priority higher than our reslice cursor widget
  this->DistanceWidget[i]->SetPriority(
    this->riw[i]->GetResliceCursorWidget()->GetPriority() + 0.01);

  vtkSmartPointer< vtkPointHandleRepresentation2D > handleRep =
    vtkSmartPointer< vtkPointHandleRepresentation2D >::New();
  vtkSmartPointer< vtkDistanceRepresentation2D > distanceRep =
    vtkSmartPointer< vtkDistanceRepresentation2D >::New();
  distanceRep->SetHandleRepresentation(handleRep);
  this->DistanceWidget[i]->SetRepresentation(distanceRep);
  distanceRep->InstantiateHandleRepresentation();
  distanceRep->GetPoint1Representation()->SetPointPlacer(riw[i]->GetPointPlacer());
  distanceRep->GetPoint2Representation()->SetPointPlacer(riw[i]->GetPointPlacer());

  // Add the distance to the list of widgets whose visibility is managed based
  // on the reslice plane by the ResliceImageViewerMeasurements class
  this->riw[i]->GetMeasurements()->AddItem(this->DistanceWidget[i]);

  this->DistanceWidget[i]->CreateDefaultRepresentation();
  this->DistanceWidget[i]->EnabledOn();
}

void QtVTKRenderWindows::ShowMid()
{
    std::string filePath = "D:\\F\\InsertedLines\\";//"D:\\F\\MidLine\\";
    std::vector<std::string> files;
    getFiles(filePath, files);
    int size = files.size();
    cout << "midline!!!" << endl;

    for (int i = 0; i < size-1; i++) { getLine(i,0); }
    /*getLine(lineId);
    lineId++;*/
    //this->ui->textBrowser->append("midlines shown");

    this->ui->view5->update();
    //this->ui->view7->update();
}

void QtVTKRenderWindows::getLine(int id, bool isblue)
{
    vtkSmartPointer<vtkPoints> m_Points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();	//_存放细胞顶点，用于渲染（显示点云所必须的）
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPolyDataMapper> pointMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> pointActor = vtkSmartPointer<vtkActor>::New();

    double x, y, z;

    json j;

    std::string filePath = "D:\\F\\InsertedLines\\";//"D:\\F\\MidLine\\";
    std::vector<std::string> files;
    getFiles(filePath, files);

    //char str[30];
    int size = files.size();
    int nullSize = 0;
    int pointCnt = 0;

    cout << files[id].c_str() << endl;
    //cout << endl << "id" << id << endl;
    std::ifstream jfile(files[id].c_str());
    jfile >> j;
    json j2 = j["markups"][0].at("controlPoints");

    //cout << "size:  "<< j2.size() << endl;

    for (int k = j2.size()-1; k >= 0; k--) {

        x = j2[k].at("position")[0];
        y = j2[k].at("position")[1];
        z = j2[k].at("position")[2];
        //std::cout << "(" << x << "," << y << "," << z << ")" << std::endl;

        m_Points->InsertPoint(pointCnt, x, y, z);		//_加入点信息
        vertices->InsertNextCell(1);		//_加入细胞顶点信息----用于渲染点集
        vertices->InsertCellPoint(pointCnt);

        //save to roadPoints
        if (isblue) 
        { 
            roadPoints->InsertPoint(pointID, x, y, z); 
            pointID++;
        }	

        pointCnt++;
    }

    //_创建待显示数据源
    polyData->SetPoints(m_Points);		//_设置点集
    polyData->SetVerts(vertices);		//_设置渲染顶点
    pointMapper->SetInputData(polyData);

    pointActor->SetMapper(pointMapper);
    if(!isblue)pointActor->GetProperty()->SetColor(1, 0, 0);
    else pointActor->GetProperty()->SetColor(0, 0.5, 1);
    pointActor->GetProperty()->SetAmbient(0.5);
    pointActor->GetProperty()->SetPointSize(2);

    //line
    vtkSmartPointer< vtkPolyLine >aLine1 =
        vtkSmartPointer< vtkPolyLine >::New();
    aLine1->GetPointIds()->SetNumberOfIds(m_Points->GetNumberOfPoints());
    for (int i = 0; i < m_Points->GetNumberOfPoints(); i++) { aLine1->GetPointIds()->SetId(i, i); }

    vtkUnstructuredGrid* grid1 = vtkUnstructuredGrid::New();
    grid1->Allocate(1, 1);
    grid1->InsertNextCell(aLine1->GetCellType(), aLine1->GetPointIds());
    grid1->SetPoints(m_Points);

    vtkSmartPointer< vtkDataSetMapper > mapper1 =
        vtkSmartPointer< vtkDataSetMapper >::New();
    mapper1->SetInputData(grid1);

    vtkActor* actor1 = vtkActor::New();
    actor1->SetMapper(mapper1);
    if (!isblue)actor1->GetProperty()->SetColor(1.0, 0.0, 0.0); //设置颜色
    else actor1->GetProperty()->SetColor(0, 0.5, 1);
    if(isblue)actor1->GetProperty()->SetLineWidth(10);

    rendstl->AddActor(actor1);
    rendstl->AddActor(pointActor);
    rendcls->AddActor(actor1);
    rendcls->AddActor(pointActor);
    rendvtl->AddActor(actor1);
    rendvtl->AddActor(pointActor);
}

void QtVTKRenderWindows::getFiles(std::string path, std::vector<std::string>& files)
{
    //文件句柄
    intptr_t hFile = 0;
    //文件信息
    struct _finddata_t fileinfo;
    std::string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
    {
        do
        {
            //如果是目录,迭代之
            //如果不是,加入列表
            if ((fileinfo.attrib & _A_SUBDIR))
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}


void QtVTKRenderWindows::Control()
{
    double xErr;
    double yErr;
    double minidev = 0.1;

    double wx, wy = 0;

    if (xErr < minidev) { xErr = 0; };
    if (yErr < minidev) { yErr = 0; };
    
    xErrSum = 0.1 * xErrSum + 0.5 * xErr;
    yErrSum = 0.1 * yErrSum + 0.5 * yErr;

    wx = Kp * xErr + Ki * xErrSum + Kd * (xErr - xErrlast);
    wy = Kp * yErr + Ki * yErrSum + Kd * (yErr - yErrlast);
    //wx = constrain(w, -1570, 1570) / 300;

    xErrlast = xErr;
    yErrlast = yErr;

}

void QtVTKRenderWindows::newWin()
{
    cout << endl << "maybeOK????" << endl;

    //QApplication a(ac,av);
    newW.showMidline();
    newW.ZoomIn();
    newW.showMaximized();
    cout << endl << "arrive????" << endl;
}

void QtVTKRenderWindows::registrateMRI()
{
    //源数据 与 目标数据 
    std::string filename = "D:/F/pose/output_tip_pose.txt"; //打开文件流操作
    vtkSmartPointer<vtkVertexGlyphFilter> sourceGlyph = InputDataExtract(filename);  //vtkVertexGlyphFilter 类显示点集
                                                                                     //sourceGlyph: the points obtained using EM sensor
                                                                                     //点数少的点集为source，点数多的点集为target
    // Read a stl file.
    vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName("D:/F/201/STL/aim.stl");
    reader->Update();

    vtkSmartPointer<vtkPolyData> original = reader->GetOutput();

    vtkSmartPointer<vtkPolyData> target = vtkSmartPointer<vtkPolyData>::New();
    target->SetPoints(original->GetPoints());

    //vtkSmartPointer<vtkPolyData> target = vtkSmartPointer<vtkPolyData>::New();
    //target->SetPoints(transformFilter1->GetOutput()->GetPoints());

    vtkSmartPointer<vtkVertexGlyphFilter>targetGlyph = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    targetGlyph->SetInputData(target);                                     //targetGlyph: the points from the airway model stl
    targetGlyph->Update();

    //vtkSmartPointer<vtkVertexGlyphFilter>  targetGlyph = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    //targetGlyph->SetInputData(target);
    //targetGlyph->Update();

    //进行ICP配准求变换矩阵
    vtkSmartPointer<vtkIterativeClosestPointTransform> icptrans = vtkSmartPointer<vtkIterativeClosestPointTransform>::New();
    icptrans->SetSource(sourceGlyph->GetOutput());           //源点集
    icptrans->SetTarget(targetGlyph->GetOutput());           //目标点集
    icptrans->GetLandmarkTransform()->SetModeToRigidBody();  //指定刚性变换
    icptrans->SetMaximumNumberOfIterations(500);             //迭代次数
    icptrans->StartByMatchingCentroidsOn();                  //去偏移（中心归一/重心归一）
    icptrans->Modified();
    icptrans->Update();

    vtkSmartPointer<vtkMatrix4x4> m = icptrans->GetMatrix();
    std::cout << "The resulting matrix is: " << *m << std::endl;
    //使用配准矩阵调整源数据（EM data）得到 solution
    vtkSmartPointer<vtkTransformPolyDataFilter> solution = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    solution->SetInputData(sourceGlyph->GetOutput());
    solution->SetTransform(icptrans);
    solution->Update();

    /////////////////////////////////////////////////////////////
    //Create the source mapper and source actor
    vtkSmartPointer<vtkPolyDataMapper> sourceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    sourceMapper->SetInputConnection(sourceGlyph->GetOutputPort());
    sourceActor = vtkSmartPointer<vtkActor>::New();
    sourceActor->SetMapper(sourceMapper);
    sourceActor->GetProperty()->SetColor(1, 1, 0); //黄色 source
    sourceActor->GetProperty()->SetPointSize(4);
    /*sourceActor->GetProperty()->SetOpacity(0.7);*/

    std::cout << "test1"  << std::endl;

    //Create the solution mapper and solution actor
    vtkSmartPointer<vtkPolyDataMapper> soluMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    soluMapper->SetInputConnection(solution->GetOutputPort());
    soluActor = vtkSmartPointer<vtkActor>::New();
    soluActor->SetMapper(soluMapper);
    soluActor->GetProperty()->SetColor(1, 0, 0);   //红色
    soluActor->GetProperty()->SetPointSize(4);
    /*sourceActor->GetProperty()->SetOpacity(0.7);*/
    rendstl->AddActor(sourceActor);  //将vtkProp类型的对象添加到场景中
    rendstl->AddActor(soluActor);

    soluActor->SetVisibility(RisShow);
    sourceActor->SetVisibility(RisShow);
}

void QtVTKRenderWindows::showRegister()
{
    RisShow = !RisShow;
    soluActor->SetVisibility(RisShow);
    sourceActor->SetVisibility(RisShow);
    this->ui->view5->update();
}