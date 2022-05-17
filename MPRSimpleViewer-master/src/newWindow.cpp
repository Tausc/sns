#include "newWindow.h"
#include "ui_newWindow.h"
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkAutoInit.h>

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
#include "vtkProperty.h"
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

#include <vtkSphereSource.h>
#include <QtWidgets/QApplication>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <QSlider>
#include "nlohmann\json.hpp"
using namespace std;
using json = nlohmann::json;
using namespace cv;

class vtkResliceCursorCallback : public vtkCommand
{
public:

    static vtkResliceCursorCallback* New()
    {
        return new vtkResliceCursorCallback;
    }

    void Execute(vtkObject* caller, unsigned long ev,
        void* callData)
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
            dynamic_cast<vtkImagePlaneWidget*>(caller);
        if (ipw)
        {
            double* wl = static_cast<double*>(callData);

            if (ipw == this->IPW[0])
            {
                this->IPW[1]->SetWindowLevel(wl[0], wl[1], 1);
                this->IPW[2]->SetWindowLevel(wl[0], wl[1], 1);
            }
            else if (ipw == this->IPW[1])
            {
                this->IPW[0]->SetWindowLevel(wl[0], wl[1], 1);
                this->IPW[2]->SetWindowLevel(wl[0], wl[1], 1);
            }
            else if (ipw == this->IPW[2])
            {
                this->IPW[0]->SetWindowLevel(wl[0], wl[1], 1);
                this->IPW[1]->SetWindowLevel(wl[0], wl[1], 1);
            }
        }

        vtkResliceCursorWidget* rcw = dynamic_cast<
            vtkResliceCursorWidget*>(caller);
        if (rcw)
        {
            vtkResliceCursorLineRepresentation* rep = dynamic_cast<
                vtkResliceCursorLineRepresentation*>(rcw->GetRepresentation());
            // Although the return value is not used, we keep the get calls
            // in case they had side-effects
            rep->GetResliceCursorActor()->GetCursorAlgorithm()->GetResliceCursor();
            for (int i = 0; i < 3; i++)
            {
                vtkPlaneSource* ps = static_cast<vtkPlaneSource*>(
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
    vtkResliceCursorWidget* RCW[3];
};

newWindow::newWindow()
{
	this->ui = new Ui_newWindow;
	this->ui->setupUi(this);

    cout << "showNEW";// << endl;

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


    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkInteractorStyleTrackballCamera* style = //常用的方式 移动摄像机  
        vtkInteractorStyleTrackballCamera::New();
    renderWindowInteractor->SetInteractorStyle(style);
    renderWindowInteractor->SetRenderWindow(this->ui->view7->GetRenderWindow());

    this->ui->view7->GetRenderWindow()->SetInteractor(renderWindowInteractor);
    this->ui->view7->GetRenderWindow()->Render();
    this->ui->view7->GetRenderWindow()->AddRenderer(rendstl);

    //vtkRenderWindowInteractor* iren = this->ui->view4->GetInteractor();
    this->ui->view5->GetRenderWindow()->Render();
    this->ui->view5->GetRenderWindow()->AddRenderer(rendvtl);
    //this->ui->view8->GetRenderWindow()->AddRenderer(rendcls);
    vtkRenderWindowInteractor* iren = this->ui->view4->GetInteractor();


    //fluo
    QString filename("D:\\F\\201\\line\\1.jpg");
    QImage* img = new QImage;
    img->load(filename);
    this->ui->flu->setPixmap(QPixmap::fromImage(*img));
    //this->ui->view6->resize(img.size());
    this->ui->flu->show();
           
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
        vtkResliceCursorLineRepresentation* rep =
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

    this->ui->view4->GetRenderWindow()->AddRenderer(ren);

    //this->ui->view5->GetRenderWindow()->AddRenderer(rendstl);
    //vtkRenderWindowInteractor* iren5 = this->ui->view5->GetInteractor();

    for (int i = 0; i < 3; i++)
    {
        planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
        planeWidget[i]->SetInteractor(iren);
        planeWidget[i]->SetPicker(picker);
        planeWidget[i]->RestrictPlaneToVolumeOn();
        double color[3] = { 0, 0, 0 };
        color[i] = 1;
        planeWidget[i]->GetPlaneProperty()->SetColor(color);

        color[0] /= 4.0;
        color[1] /= 4.0;
        color[2] /= 4.0;
        riw[i]->GetRenderer()->SetBackground(color);

        planeWidget[i]->SetTexturePlaneProperty(ipwProp);
        planeWidget[i]->TextureInterpolateOff();
        planeWidget[i]->SetResliceInterpolateToLinear();
        planeWidget[i]->SetInputConnection(reader->GetOutputPort());
        planeWidget[i]->SetPlaneOrientation(i);
        planeWidget[i]->SetSliceIndex(imageDims[i] / 2);
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
            vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk);
        riw[i]->GetResliceCursorWidget()->AddObserver(
            vtkResliceCursorWidget::WindowLevelEvent, cbk);
        riw[i]->GetResliceCursorWidget()->AddObserver(
            vtkResliceCursorWidget::ResliceThicknessChangedEvent, cbk);
        riw[i]->GetResliceCursorWidget()->AddObserver(
            vtkResliceCursorWidget::ResetCursorEvent, cbk);
        riw[i]->GetInteractorStyle()->AddObserver(
            vtkCommand::WindowLevelEvent, cbk);

        // Make them all share the same color map.
        riw[i]->SetLookupTable(riw[0]->GetLookupTable());
        planeWidget[i]->GetColorMap()->SetLookupTable(riw[0]->GetLookupTable());
        //planeWidget[i]->GetColorMap()->SetInput(riw[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->GetColorMap()->GetInput());
        planeWidget[i]->SetColorMap(riw[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->GetColorMap());

    }

    this->ui->view1->show();
    this->ui->view2->show();
    this->ui->view3->show();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(importFrame()));//import frame when timeout
    timerVB = new QTimer(this);
    connect(timerVB, SIGNAL(timeout()), this, SLOT(follow1step()));//import frame when timeout
    timerSTL = new QTimer(this);
    connect(timerSTL, SIGNAL(timeout()), this, SLOT(sim1step()));//import frame when timeout

    // Set up action signals and slots
    connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(this->ui->play, SIGNAL(pressed()), this, SLOT(videoPlay()));
    connect(this->ui->stop, SIGNAL(pressed()), this, SLOT(videoStop()));
    connect(this->ui->Slider, SIGNAL(valueChanged(int)), this, SLOT(stlTrans()));
    connect(this->ui->simulate, SIGNAL(pressed()), this, SLOT(Sim()));

    connect(this->ui->follow, SIGNAL(pressed()), this, SLOT(VBfollow()));
    connect(this->ui->reset, SIGNAL(pressed()), this, SLOT(VBstop()));
    connect(this->ui->pause, SIGNAL(pressed()), this, SLOT(VBpause()));
    //connect(this->ui->contour, SIGNAL(pressed()), this, SLOT(detectCircle()));
    //this->ui->contour->hide();


    //draw VBsphere
    vtkSmartPointer<vtkSphereSource> VBsphere =
        vtkSmartPointer<vtkSphereSource>::New();
    VBsphere->SetRadius(0.2);
    VBsphere->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper3 = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper3->SetInputConnection(VBsphere->GetOutputPort());

    actorSphereVB = vtkSmartPointer<vtkActor>::New();
    //actorSphereVB->GetProperty()->SetOpacity(0.8);//透明度
    actorSphereVB->SetMapper(mapper3);
    actorSphereVB->GetProperty()->SetColor(1, 0, 1);

    //draw sphere
    vtkSmartPointer<vtkSphereSource> sphere =
        vtkSmartPointer<vtkSphereSource>::New();
    sphere->SetRadius(5.0);             // 设置半径
    sphere->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper2->SetInputConnection(sphere->GetOutputPort());

    actorSphere = vtkSmartPointer<vtkActor>::New();
    actorSphere->SetMapper(mapper2);
    actorSphere->GetProperty()->SetColor(0.9, 0.7, 0);

    //draw cone(sphere)
    vtkSmartPointer<vtkSphereSource> cone =
        vtkSmartPointer<vtkSphereSource>::New();
    cone->SetRadius(6.0);
    cone->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper1->SetInputConnection(cone->GetOutputPort());

    actorCone = vtkSmartPointer<vtkActor>::New();
    actorCone->SetMapper(mapper1);
    actorCone->GetProperty()->SetColor(0, 1, 0);


    double point[3] = { 20,-140,100 };
    double pose[3] = { 0,-1,0 };

    //drawCone();
    vtkSmartPointer<vtkPoints> m_Points = vtkSmartPointer<vtkPoints>::New();
    m_Points->InsertPoint(0, 20, -140, 100);
    m_Points->InsertPoint(1, 20, -143, 100);

    //line
    actorDir = vtkActor::New();
    actorDir->SetMapper(mapper1);
    actorShape = vtkActor::New();
    actorShape->SetMapper(mapper1);

    rendstl->AddActor(actorDir);
    rendcls->AddActor(actorDir);

    assembleNewStl(point, pose);
}

void newWindow::VBfollow()
{
    //int interval = round(speed);
    timerVB->start(speed);
}

void newWindow::VBstop()
{
    VBcnt = roadPoints->GetNumberOfPoints() - 1;
    timerVB->stop();
    follow1step();
    timerVB->stop();
    //VBpause();
}

void newWindow::VBpause()
{
    timerVB->stop();
}

void newWindow::follow1step()
{
    if (VBcnt < 20)
    {
        timerVB->stop();
        return;
    }

    // sphere
    int k = VBcnt - 20;
    double pointCone[3] = { 0 };
    pointCone[0] = roadPoints->GetPoint(k)[0];
    pointCone[1] = roadPoints->GetPoint(k)[1];
    pointCone[2] = roadPoints->GetPoint(k)[2];

    double x = 0, y = 0, z = 0;
    x = roadPoints->GetPoint(VBcnt)[0];
    y = roadPoints->GetPoint(VBcnt)[1];
    z = roadPoints->GetPoint(VBcnt)[2];

    // VB
    int j = VBcnt - 30;
    double a = 0, b = 0, c = 0;
    if (VBcnt < 31)
    {
        a = roadPoints->GetPoint(0)[0]-x;
        b = roadPoints->GetPoint(0)[1]-y;
        c = roadPoints->GetPoint(0)[2]-z;
    }
    else {
        for (int t = 0; t < 5; t++)
        {
            a = a + roadPoints->GetPoint(j + t)[0];
            b = b + roadPoints->GetPoint(j + t)[1];
            c = c + roadPoints->GetPoint(j + t)[2];
        }
        a = a / 5 - x;//pointCone[0];
        b = b / 5 - y;//pointCone[1];
        c = c / 5 - z;//pointCone[2];
    }

    double pose[3] = { a,b,c };
    double point[3] = { x,y,z };

    setVBpose(point, pose, true);

    //Eigen::Matrix3d rotMatrix;
    //Eigen::Vector3d vectorBefore(0, 0, -1);
    //Eigen::Vector3d vectorAfter(a, b, c);
    //vectorAfter.normalize();
    ////cout << a << b << c;
    ////cout << "vector:  " << vectorAfter.transpose() << endl;

    //rotMatrix = Eigen::Quaterniond::FromTwoVectors(vectorBefore, vectorAfter).toRotationMatrix();
    ////cout << "rotMatrix =\n" << rotMatrix << endl;

    //Eigen::Vector3d ea = rotMatrix.eulerAngles(2, 1, 0);

    ////cout << "euler_angle.transpose() = " << ea.transpose() << endl << endl;
    ////yaw pitch roll  Z Y X
    //a = ea(0) / 3.14 * 180;
    //b = 90 + ea(1) / 3.14 * 180;
    //c = ea(2) / 3.14 * 180;
    //double euler[3] = { a,b,c };

    //assembleNewStl(point, euler);

    setVBSphere(pointCone);
    VBcnt--;
}

void newWindow::assembleNewStl(double* point, double* pose)
{
    rendstl->RemoveActor(actorDir);
    rendcls->RemoveActor(actorDir);

    double x = point[0];
    double y = point[1];
    double z = point[2];

    double a = pose[0];
    double b = pose[1];
    double c = pose[2];
    double sum = sqrt(a * a + b * b + c * c);
    a = 30 * a / sum;
    b = 30 * b / sum;
    c = 30 * c / sum;
    cout << "a" << a << " " << "b" << b << " " << "c" << c << " ";

    actorCone->SetPosition(x, y, z);
   //actorCone->SetOrientation(a, b, c);
    rendstl->AddActor(actorCone);
    rendcls->AddActor(actorCone);

    vtkSmartPointer<vtkPoints> m_Points = vtkSmartPointer<vtkPoints>::New();

    m_Points->InsertPoint(0, x, y, z);
    m_Points->InsertPoint(1, x+a, y+b, z+c);

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

    actorDir = vtkActor::New();
    actorDir->SetMapper(mapper1);
    actorDir->GetProperty()->SetColor(0, 1, 0);
    actorDir->GetProperty()->SetOpacity(0.8);
    actorDir->GetProperty()->SetLineWidth(4);

    rendstl->AddActor(actorDir);
    rendcls->AddActor(actorDir);
    //rendvtl->AddActor(actorDir);

    this->ui->view7->update();
    this->ui->view8->update();
}

void newWindow::setVBpose(double* point, double* pose, bool isFollow = false)
{
    Eigen::Vector3d cameraV;
    double x = point[0];
    double y = point[1];
    double z = point[2];

    double a = pose[0];
    double b = pose[1];
    double c = pose[2];

    if (isFollow)
    {
        cameraV << a, b, c;
        cameraV.normalize();
    }
    else
    {
        double total = a + b + c;
        a = fmod(a, 360);
        b = fmod(b, 360);
        c = fmod(c, 360);

        cameraV << 0, 0, -1;

        Eigen::Vector3d ea(a * 0.01745, (b - 90) * 0.01745, c * 0.01745);

        //欧拉角转换为旋转矩阵 0 1 2 对应 z y x
        Eigen::Matrix3d rotation_matrix_4;
        rotation_matrix_4 = Eigen::AngleAxisd(ea[0], Eigen::Vector3d::UnitZ()) *
            Eigen::AngleAxisd(ea[1], Eigen::Vector3d::UnitY()) *
            Eigen::AngleAxisd(ea[2], Eigen::Vector3d::UnitX());
        //cout << "rotation matrix_4 =\n" << rotation_matrix_4 << endl;

        cameraV = rotation_matrix_4 * cameraV;
    }

    cameraV = cameraV * 100;
    //set camera
    vtkCamera* aCamera = vtkCamera::New();
    //aCamera->SetViewUp(a, b, c);//设视角位置 
    aCamera->SetPosition(x, y, z);//设观察对象位
    //aCamera->SetFocalPoint(x, y, z-1);
    aCamera->SetFocalPoint(x + cameraV[0], y + cameraV[1], z + cameraV[2]);//设焦点 
    aCamera->ComputeViewPlaneNormal();//自动
    rendvtl->SetActiveCamera(aCamera);

    this->ui->view5->update();
}

void newWindow::setVBSphere(double* point)
{
    double x = point[0];
    double y = point[1];
    double z = point[2];

    actorSphereVB->SetPosition(x, y, z);
    rendvtl->AddActor(actorSphereVB);

    this->ui->view5->update();
}

void newWindow::stlTrans()
{
    double pos = this->ui->Slider->value();
    double p = pos / 100;
    //cout << "pos" << pos << "p" << p << endl;
    actorSTL->GetProperty()->SetOpacity(p);
    rendstl->AddActor(actorSTL);
    rendcls->AddActor(actorSTL);
    this->ui->view7->update();
    this->ui->view8->update();
}


void newWindow::importFrame()
{
    capture >> frame;
    if (!frame.empty()) { cvtColor(frame, frame, CV_BGR2RGB); }//only RGB of Qt
    else { videoStop(); }
    QImage srcQImage = QImage((uchar*)(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);

    if (this->ui->contour->isChecked()&& !frame.empty())
    {
        Mat blur;
        GaussianBlur(frame, blur, Size(15, 15), 0);
        Mat gr;
        cvtColor(blur, gr, CV_BGR2GRAY);
        //equalizeHist(gr, gr);

        Mat bw; 
        double minValue; 
        cv::minMaxLoc(gr, &minValue, NULL, NULL, NULL);

        if (minValue < 70)
        {
            threshold(gr, bw, 70, 255, cv::THRESH_BINARY_INV);
            vector<vector<Point>> contours;
            vector<Vec4i> hierarchy;
            findContours(bw, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
            for (int idx = 0; idx >= 0; idx = hierarchy[idx][0])
            {
                Point2f center;
                float radius;
                minEnclosingCircle(contours[idx], center, radius);
                radius = radius - 10;

                if ((radius > 50))
                {
                    circle(frame, Point(center.x, center.y), radius, Scalar(0, 255, 255), 2);
                    circle(frame, Point(center.x, center.y), 2, Scalar(0, 255, 255), 2);
                }
            }

            srcQImage = QImage((uchar*)(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);
        }
    }
    //else if(!frame.empty()){ videoStop(); }

    this->ui->view6->setPixmap(QPixmap::fromImage(srcQImage));
    this->ui->view6->resize(srcQImage.size());
    this->ui->view6->show();


}

void newWindow::videoPlay()
{
    capture.open("D:\\F\\201\\line\\2.mp4");
    timer->start(100);// Start timing, Signal out when timeout 33
    this->ui->contour->show();

    //QMediaPlayer* player;
    //player->setMedia(QMediaContent(QUrl::fromLocalFile("D:\\F\\201\\line\\3.avi")));
    //player->setVideoOutput(this->ui->view6);
}

void newWindow::videoStop()
{
    timer->stop();
    capture.release();
}

void newWindow::Sim()
{
    double x, y, z;
    json j;
    STLcnt = 0;
    STLsum = 0;
    std::string filePath = "D:\\F\\noisyLine\\";//"D:\\F\\MidLine\\";
    std::vector<std::string> files;
    getFiles(filePath, files);
    int size = files.size();
    cout << "size" << size;

    vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();	//_存放细胞顶点，用于渲染（显示点云所必须的）

    for (int id = 0; id <size; id++) {

        //char str[30];
        int size = files.size();
        int nullSize = 0;
        
        std::ifstream jfile(files[id].c_str());
        jfile >> j;
        json j2 = j["markups"][0].at("controlPoints");

        //cout << "size:  "<< j2.size() << endl;
        cout << "j2size" << j2.size() << " ";
        for (int k = 0; k < j2.size(); k++) {
            
            x = j2[k].at("position")[0];
            y = j2[k].at("position")[1];
            z = j2[k].at("position")[2];
            //std::cout << "(" << x << "," << y << "," << z << ")" << std::endl;
            vertices->InsertNextCell(1);		//_加入细胞顶点信息----用于渲染点集
            vertices->InsertCellPoint(STLsum);

            scpPoints->InsertPoint(STLsum, x, y, z);
            STLsum++;
        }        
    }
    cout << "STLsum" << STLsum;

    timerSTL->start(speed);
}

void newWindow::sim1step()
{
    if (STLsum-STLcnt < 10)
    {
        timerSTL->stop();
        return;
    }
    double x = scpPoints->GetPoint(STLcnt)[0];
    double y = scpPoints->GetPoint(STLcnt)[1];
    double z = scpPoints->GetPoint(STLcnt)[2];
    double point[3] = { x,y,z };
    int j = STLcnt + 10;
    double a = scpPoints->GetPoint(j)[0] - x;
    double b = scpPoints->GetPoint(j)[1] - y;
    double c = scpPoints->GetPoint(j)[2] - z;
    double pose[3] = { a,b,c };
    /*cout << "x" <<x<< " " << "y" <<y<< " " << "z" <<z<< " " << "cnt" << STLcnt;*/
    assembleNewStl(point, pose);

    //shape sensor
    if (!init) {
        rendstl->RemoveActor(actorShape);
        rendcls->RemoveActor(actorShape);
        init = false;
    }
    vtkSmartPointer<vtkPoints> m_Points = vtkSmartPointer<vtkPoints>::New();
    for (int t = 0; t <= STLcnt; t++) {
        double px = scpPoints->GetPoint(t)[0];
        double py = scpPoints->GetPoint(t)[1];
        double pz = scpPoints->GetPoint(t)[2];
        m_Points->InsertPoint(t, px, py, pz);
    }
      
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

    actorShape = vtkActor::New();
    actorShape->SetMapper(mapper1);
    actorShape->GetProperty()->SetColor(1, 0.12, 0.38);
    actorShape->GetProperty()->SetOpacity(0.8);
    actorShape->GetProperty()->SetLineWidth(8);

    rendstl->AddActor(actorShape);
    rendcls->AddActor(actorShape);
    //rendvtl->AddActor(actorDir);

    this->ui->view7->update();
    this->ui->view8->update();
    STLcnt++;
}



void newWindow::ZoomIn()
{
    ifstream srcFile("D:\\leison.txt", ios::in); //以文本模式打开txt文件
    double x, y, z;
    srcFile >> x >> y >> z;

    vtkCamera* camera8 = vtkCamera::New();
    camera8->SetPosition(0, 100, -20);//设观察对象位
    camera8->SetViewUp(0, 0, 1);//设视角位置 
    camera8->SetFocalPoint(x, y, z);//设焦点 
    camera8->ComputeViewPlaneNormal();//自动
    rendcls->SetActiveCamera(camera8);

    this->ui->view8->GetRenderWindow()->AddRenderer(rendcls);
    this->ui->view8->update();
}

void newWindow::showMidline()
{
    vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();	//_存放细胞顶点，用于渲染（显示点云所必须的）
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPolyDataMapper> pointMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> pointActor = vtkSmartPointer<vtkActor>::New();
    ifstream srcFile("D:\\mid.txt", ios::in); //以文本模式打开txt文件
    double x, y, z;
    int pointID = 0;
    while (srcFile >> x >> y >> z)    //数字读取方式
    {
        roadPoints->InsertPoint(pointID, x, y, z);
        vertices->InsertNextCell(1);		//_加入细胞顶点信息----用于渲染点集
        vertices->InsertCellPoint(pointID);
        pointID++;
    }
    srcFile.close();
    VBcnt = roadPoints->GetNumberOfPoints() - 1;

    //_创建待显示数据源
    polyData->SetPoints(roadPoints);		//_设置点集
    polyData->SetVerts(vertices);		//_设置渲染顶点
    pointMapper->SetInputData(polyData);

    pointActor->SetMapper(pointMapper);
    pointActor->GetProperty()->SetColor(0, 0.5, 1);
    pointActor->GetProperty()->SetAmbient(0.5);
    pointActor->GetProperty()->SetPointSize(2);

    //line
    vtkSmartPointer< vtkPolyLine >aLine1 =
        vtkSmartPointer< vtkPolyLine >::New();
    aLine1->GetPointIds()->SetNumberOfIds(roadPoints->GetNumberOfPoints());
    for (int i = 0; i < roadPoints->GetNumberOfPoints(); i++) { aLine1->GetPointIds()->SetId(i, i); }

    vtkUnstructuredGrid* grid1 = vtkUnstructuredGrid::New();
    grid1->Allocate(1, 1);
    grid1->InsertNextCell(aLine1->GetCellType(), aLine1->GetPointIds());
    grid1->SetPoints(roadPoints);

    vtkSmartPointer< vtkDataSetMapper > mapper1 =
        vtkSmartPointer< vtkDataSetMapper >::New();
    mapper1->SetInputData(grid1);

    vtkActor* actorl = vtkActor::New();
    actorl->SetMapper(mapper1);
    actorl->GetProperty()->SetColor(0, 0.5, 1);
    actorl->GetProperty()->SetLineWidth(7);
    actorl->GetProperty()->SetOpacity(0.5);


    ifstream leiFile("D:\\leison.txt", ios::in); //以文本模式打开txt文件
    double leisonX, leisonY, leisonZ;
    leiFile >> leisonX >> leisonY >> leisonZ;
    actorSphere->SetPosition(leisonX, leisonY, leisonZ);
    rendstl->AddActor(actorSphere);
    rendcls->AddActor(actorSphere);

    rendcls->AddActor(actorl);
    rendcls->AddActor(pointActor);
    rendstl->AddActor(actorl);
    rendstl->AddActor(pointActor);
    rendvtl->AddActor(actorl);
    rendvtl->AddActor(pointActor);
}

void newWindow::getFiles(std::string path, std::vector<std::string>& files)
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

void newWindow::slotExit()
{
    qApp->exit();
}

//#include "moc_newWindow.cpp"