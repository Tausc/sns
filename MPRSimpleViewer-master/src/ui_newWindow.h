/********************************************************************************
** Form generated from reading UI file 'newWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWWINDOW_H
#define UI_NEWWINDOW_H

#include <QVTKWidget.h>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_newWindow
{
public:
    QAction *actionOpenFile;
    QAction *actionExit;
    QAction *actionPrint;
    QAction *actionHelp;
    QAction *actionSave;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout_5;
    QVTKWidget *view3;
    QLabel *label_17;
    QVTKWidget *view1;
    QLabel *label_16;
    QLabel *label_15;
    QVTKWidget *view2;
    QGridLayout *gridLayout_2;
    QLabel *label_3;
    QLabel *view6;
    QLabel *label_2;
    QLabel *flu;
    QPushButton *play;
    QSlider *Slider;
    QLabel *label_14;
    QLabel *label;
    QLabel *label_11;
    QVTKWidget *view7;
    QPushButton *simulate;
    QPushButton *pause;
    QPushButton *follow;
    QLabel *label_4;
    QPushButton *reset;
    QPushButton *stop;
    QVTKWidget *view4;
    QVTKWidget *view5;
    QVTKWidget *view8;
    QCheckBox *contour;

    void setupUi(QMainWindow *newWindow)
    {
        if (newWindow->objectName().isEmpty())
            newWindow->setObjectName(QString::fromUtf8("newWindow"));
        newWindow->resize(1468, 1020);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Icons/help.png"), QSize(), QIcon::Normal, QIcon::Off);
        newWindow->setWindowIcon(icon);
        newWindow->setIconSize(QSize(22, 22));
        actionOpenFile = new QAction(newWindow);
        actionOpenFile->setObjectName(QString::fromUtf8("actionOpenFile"));
        actionOpenFile->setEnabled(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Icons/fileopen.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpenFile->setIcon(icon1);
        actionExit = new QAction(newWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon2);
        actionPrint = new QAction(newWindow);
        actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/Icons/print.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrint->setIcon(icon3);
        actionHelp = new QAction(newWindow);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionHelp->setIcon(icon);
        actionSave = new QAction(newWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/Icons/filesave.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon4);
        centralwidget = new QWidget(newWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        view3 = new QVTKWidget(centralwidget);
        view3->setObjectName(QString::fromUtf8("view3"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(view3->sizePolicy().hasHeightForWidth());
        view3->setSizePolicy(sizePolicy);
        view3->setMinimumSize(QSize(500, 0));
        view3->setMaximumSize(QSize(500, 16777215));

        gridLayout_5->addWidget(view3, 6, 0, 1, 1);

        label_17 = new QLabel(centralwidget);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_17->sizePolicy().hasHeightForWidth());
        label_17->setSizePolicy(sizePolicy1);
        label_17->setMaximumSize(QSize(500, 16777215));
        QFont font;
        font.setFamily(QString::fromUtf8("Agency FB"));
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_17->setFont(font);
        label_17->setStyleSheet(QString::fromUtf8("background-color: rgba(0, 0, 109, 70);"));
        label_17->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(label_17, 5, 0, 1, 1);

        view1 = new QVTKWidget(centralwidget);
        view1->setObjectName(QString::fromUtf8("view1"));
        sizePolicy.setHeightForWidth(view1->sizePolicy().hasHeightForWidth());
        view1->setSizePolicy(sizePolicy);
        view1->setMinimumSize(QSize(500, 0));
        view1->setMaximumSize(QSize(500, 16777215));

        gridLayout_5->addWidget(view1, 2, 0, 1, 1);

        label_16 = new QLabel(centralwidget);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        sizePolicy1.setHeightForWidth(label_16->sizePolicy().hasHeightForWidth());
        label_16->setSizePolicy(sizePolicy1);
        label_16->setMaximumSize(QSize(500, 16777215));
        label_16->setFont(font);
        label_16->setStyleSheet(QString::fromUtf8("background-color: rgba(0, 109, 0, 70);"));
        label_16->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(label_16, 3, 0, 1, 1);

        label_15 = new QLabel(centralwidget);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        sizePolicy1.setHeightForWidth(label_15->sizePolicy().hasHeightForWidth());
        label_15->setSizePolicy(sizePolicy1);
        label_15->setMaximumSize(QSize(500, 16777215));
        label_15->setFont(font);
        label_15->setStyleSheet(QString::fromUtf8("background-color: rgba(109, 0, 0, 70);"));
        label_15->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(label_15, 0, 0, 1, 1);

        view2 = new QVTKWidget(centralwidget);
        view2->setObjectName(QString::fromUtf8("view2"));
        sizePolicy.setHeightForWidth(view2->sizePolicy().hasHeightForWidth());
        view2->setSizePolicy(sizePolicy);
        view2->setMinimumSize(QSize(500, 0));
        view2->setMaximumSize(QSize(500, 16777215));

        gridLayout_5->addWidget(view2, 4, 0, 1, 1);


        horizontalLayout->addLayout(gridLayout_5);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMaximumSize(QSize(720, 16777215));
        label_3->setFont(font);
        label_3->setStyleSheet(QString::fromUtf8("background-color: rgba(0, 0, 0, 70);"));
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_3, 6, 3, 3, 2);

        view6 = new QLabel(centralwidget);
        view6->setObjectName(QString::fromUtf8("view6"));
        view6->setMinimumSize(QSize(720, 720));
        view6->setMaximumSize(QSize(720, 720));

        gridLayout_2->addWidget(view6, 3, 3, 2, 2);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMaximumSize(QSize(720, 16777215));
        label_2->setFont(font);
        label_2->setStyleSheet(QString::fromUtf8("background-color: rgba(114, 80, 0, 70);"));
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_2, 0, 0, 2, 3);

        flu = new QLabel(centralwidget);
        flu->setObjectName(QString::fromUtf8("flu"));
        flu->setMaximumSize(QSize(720, 16777215));

        gridLayout_2->addWidget(flu, 9, 3, 3, 2);

        play = new QPushButton(centralwidget);
        play->setObjectName(QString::fromUtf8("play"));
        play->setMaximumSize(QSize(350, 16777215));

        gridLayout_2->addWidget(play, 2, 3, 1, 1);

        Slider = new QSlider(centralwidget);
        Slider->setObjectName(QString::fromUtf8("Slider"));
        Slider->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(Slider, 9, 0, 1, 3);

        label_14 = new QLabel(centralwidget);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setMaximumSize(QSize(720, 16777215));
        label_14->setFont(font);
        label_14->setStyleSheet(QString::fromUtf8("background-color: rgba(0, 0, 0, 70);"));
        label_14->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_14, 6, 0, 3, 3);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMinimumSize(QSize(720, 0));
        label->setMaximumSize(QSize(720, 16777215));
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("background-color: rgba(114, 80, 0, 70);"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label, 0, 3, 2, 2);

        label_11 = new QLabel(centralwidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        sizePolicy1.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy1);
        label_11->setMaximumSize(QSize(720, 16777215));
        label_11->setFont(font);
        label_11->setStyleSheet(QString::fromUtf8("background-color: rgba(0, 0, 0, 70);"));
        label_11->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_11, 6, 5, 3, 1);

        view7 = new QVTKWidget(centralwidget);
        view7->setObjectName(QString::fromUtf8("view7"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(view7->sizePolicy().hasHeightForWidth());
        view7->setSizePolicy(sizePolicy2);
        view7->setMinimumSize(QSize(0, 0));
        view7->setMaximumSize(QSize(720, 16777215));

        gridLayout_2->addWidget(view7, 10, 0, 1, 3);

        simulate = new QPushButton(centralwidget);
        simulate->setObjectName(QString::fromUtf8("simulate"));

        gridLayout_2->addWidget(simulate, 11, 0, 1, 3);

        pause = new QPushButton(centralwidget);
        pause->setObjectName(QString::fromUtf8("pause"));
        pause->setMaximumSize(QSize(220, 16777215));

        gridLayout_2->addWidget(pause, 2, 1, 1, 1);

        follow = new QPushButton(centralwidget);
        follow->setObjectName(QString::fromUtf8("follow"));
        follow->setMaximumSize(QSize(220, 16777215));

        gridLayout_2->addWidget(follow, 2, 0, 1, 1);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font);
        label_4->setStyleSheet(QString::fromUtf8("background-color: rgba(114, 80, 0, 70);"));
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_4, 0, 5, 2, 1);

        reset = new QPushButton(centralwidget);
        reset->setObjectName(QString::fromUtf8("reset"));
        reset->setMaximumSize(QSize(220, 16777215));

        gridLayout_2->addWidget(reset, 2, 2, 1, 1);

        stop = new QPushButton(centralwidget);
        stop->setObjectName(QString::fromUtf8("stop"));
        stop->setMaximumSize(QSize(350, 16777215));

        gridLayout_2->addWidget(stop, 2, 4, 1, 1);

        view4 = new QVTKWidget(centralwidget);
        view4->setObjectName(QString::fromUtf8("view4"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(view4->sizePolicy().hasHeightForWidth());
        view4->setSizePolicy(sizePolicy3);
        view4->setMaximumSize(QSize(720, 16777215));

        gridLayout_2->addWidget(view4, 9, 5, 3, 1);

        view5 = new QVTKWidget(centralwidget);
        view5->setObjectName(QString::fromUtf8("view5"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(view5->sizePolicy().hasHeightForWidth());
        view5->setSizePolicy(sizePolicy4);
        view5->setMinimumSize(QSize(0, 720));
        view5->setMaximumSize(QSize(720, 16777215));

        gridLayout_2->addWidget(view5, 3, 0, 3, 3);

        view8 = new QVTKWidget(centralwidget);
        view8->setObjectName(QString::fromUtf8("view8"));
        sizePolicy3.setHeightForWidth(view8->sizePolicy().hasHeightForWidth());
        view8->setSizePolicy(sizePolicy3);
        view8->setMinimumSize(QSize(0, 720));

        gridLayout_2->addWidget(view8, 2, 5, 4, 1);

        contour = new QCheckBox(centralwidget);
        contour->setObjectName(QString::fromUtf8("contour"));

        gridLayout_2->addWidget(contour, 5, 3, 1, 2);


        horizontalLayout->addLayout(gridLayout_2);

        newWindow->setCentralWidget(centralwidget);

        retranslateUi(newWindow);

        QMetaObject::connectSlotsByName(newWindow);
    } // setupUi

    void retranslateUi(QMainWindow *newWindow)
    {
        newWindow->setWindowTitle(QApplication::translate("newWindow", "QtVTKRenderWindows", nullptr));
        actionOpenFile->setText(QApplication::translate("newWindow", "Open File...", nullptr));
        actionExit->setText(QApplication::translate("newWindow", "Exit", nullptr));
        actionPrint->setText(QApplication::translate("newWindow", "Print", nullptr));
        actionHelp->setText(QApplication::translate("newWindow", "Help", nullptr));
        actionSave->setText(QApplication::translate("newWindow", "Save", nullptr));
        label_17->setText(QApplication::translate("newWindow", "Transverse section", nullptr));
        label_16->setText(QApplication::translate("newWindow", "Coronal section", nullptr));
        label_15->setText(QApplication::translate("newWindow", "Sagittal section", nullptr));
        label_3->setText(QApplication::translate("newWindow", "fluoroscope", nullptr));
        view6->setText(QString());
        label_2->setText(QApplication::translate("newWindow", "Virtual bronchoscopy", nullptr));
        flu->setText(QString());
        play->setText(QApplication::translate("newWindow", "play", nullptr));
        label_14->setText(QApplication::translate("newWindow", "bronchial tree", nullptr));
        label->setText(QApplication::translate("newWindow", "Real Image", nullptr));
        label_11->setText(QApplication::translate("newWindow", "CT model", nullptr));
        simulate->setText(QApplication::translate("newWindow", "simulate", nullptr));
        pause->setText(QApplication::translate("newWindow", "pause", nullptr));
        follow->setText(QApplication::translate("newWindow", "start", nullptr));
        label_4->setText(QApplication::translate("newWindow", "Zoom In", nullptr));
        reset->setText(QApplication::translate("newWindow", "reset", nullptr));
        stop->setText(QApplication::translate("newWindow", "stop", nullptr));
        contour->setText(QApplication::translate("newWindow", "Find Contour", nullptr));
    } // retranslateUi

};

namespace Ui {
    class newWindow: public Ui_newWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWWINDOW_H
