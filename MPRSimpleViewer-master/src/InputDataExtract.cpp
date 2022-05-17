#include <vtkSmartPointer.h>
#include <vtkSimplePointsReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <Eigen/Dense>
#include <math.h>
#include <vtkVertexGlyphFilter.h>
#include "InputDataExtract.h"
using namespace Eigen;
using namespace std;
double pi = 3.1415926;
//删除字符串中空格，制表符tab等无效字符
string Trim(string& str)
{
	//str.find_first_not_of(" \t\r\n"),在字符串str中从索引0开始，返回首次不匹配"\t\r\n"的位置
	str.erase(0, str.find_first_not_of(" \t\r\n"));
	str.erase(str.find_last_not_of(" \t\r\n") + 1);
	return str;
}

vtkSmartPointer<vtkVertexGlyphFilter> InputDataExtract(string filename)
{
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	ifstream fin(filename);   //读操作（输入）的文件类  创建读取文件的流
	double x, y, z;
	//double Qx, Qy, Qz, Qw;
	//double T[16] = {0.0074, - 0.0929,    0.9956,    0.2117*1000,
	//			   - 0.9997,    0.0192,    0.0092,   -0.0519*1000,
	//			   - 0.0200, - 0.9955,   - 0.0926,    0.4534*1000,
	//				 0,         0,         0,    1.0000};

	string line;
	//getline(fin, line);

	AngleAxisd rollfilter(AngleAxisd(0, Vector3d::UnitX()));
	AngleAxisd pitchfilter(AngleAxisd(0, Vector3d::UnitY()));
	AngleAxisd yawfilter(AngleAxisd(0, Vector3d::UnitZ()));
	Matrix3d R = rollfilter.matrix() * pitchfilter.matrix() * yawfilter.matrix();

	AngleAxisd roll(AngleAxisd(pi/2, Vector3d::UnitX()));
	AngleAxisd pitch(AngleAxisd(0, Vector3d::UnitY()));
	AngleAxisd yaw(AngleAxisd(0, Vector3d::UnitZ()));
	Matrix3d R1 = roll.matrix() * pitch.matrix() * yaw.matrix();

	//Matrix4d EM_model;
	//Matrix3d  << 0.97925,- 0.202366,- 0.0108269,185.659
	//	0.202379 0.979307 0.000137228 209.753
	//	0.0105751 - 0.00232552 0.999941 - 278.526
	//	0 0 0 1

	R = R * R1;
	                             // fin 正在读取的输入流  getline 此函数可读取整行
	while (getline(fin, line))   //整行读取，换行符“\n”区分，遇到文件尾标志eof终止读取 //line 接收输入字符串的string变量名称
	{
		//cout <<"原始字符串："<< line << endl; //整行输出
		istringstream sin(line); //将整行字符串line读入到字符串流istringstream中
		vector<string> fields;   //声明一个字符串向量
		string field;
		while (getline(sin, field, ' ')) //将字符串流sin中的字符读入到field字符串中，以逗号为分隔符
		{
			if(field.length() != 0 && field != " ")
				fields.push_back(field); //将刚刚读取的字符串添加到向量fields中
		}
		/*cout << "fields[3] = " << fields[3] << endl;*/

		//string sQw = Trim(fields[5]);
		//string sQx = Trim(fields[6]);
		//string sQy = Trim(fields[7]);  
		//string sQz = Trim(fields[8]);
 		string sx = Trim(fields[3]);
		string sy = Trim(fields[7]);
		string sz = Trim(fields[11]);

		//字符串转换成double型数字
		Vector3d point;
		istringstream streamx, streamy, streamz;
		streamx.str(sx), streamy.str(sy), streamz.str(sz);
		streamx >> x, streamy >> y, streamz >> z;
		point << x, y, z;
		//cout << point << endl;         //point 是在EM坐标系下
		point = R.inverse() * point;   //point 是在经过R选择之后的坐标系下的
		//新读取的数据赋予点的几何结构
		/*points->InsertNextPoint(point[0] + 180 , point[1] + 209.753, point[2] - 250); */
		points->InsertNextPoint(point[0]-150, point[1]+150, point[2]+200);
	}
	fin.close();  //关闭文件

	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints(points);

	vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
#if VTK_MAJOR_VERSION <= 5  
	glyphFilter->SetInputConnection(polyData->GetProducerPort());
#else
	glyphFilter->SetInputData(polyData);
#endif
	glyphFilter->Update();

	//return EXIT_SUCCESS;
	return glyphFilter;
}