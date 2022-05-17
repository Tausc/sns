#include <QApplication>
#include "QtVTKRenderWindows.h"
#include "midline.h"
#include "newWindow.h"


int main( int argc, char** argv )
{
  // QT Stuff
  QApplication app( argc, argv );

  QtVTKRenderWindows myQtVTKRenderWindows(argc, argv);
  myQtVTKRenderWindows.showMaximized();

  return app.exec();
}

//int main()
//{
//	std::cout << "test";
//	int input = 2;
//	//cin >> input;
//	cout << "input:  " << input << endl;
//	switch (input)
//	{
//	case(0):getContour(); break;
//	case(1):detectCircle1(); break;
//	case(2):detectCircle2(); break;
//	case(3):detectCircle3(); break;
//	};
//	
//	//detectCircle3();
//	std::cout << "testEnd";
//	return 0;
//};

//int main(int argc, char* argv[])
//{
//    QApplication a(argc, argv);
//    cout << "hello";
//    newWindow w;
//    w.showMaximized();
//    return a.exec();
//}

//int main()
//{
//	pickerT();
//}