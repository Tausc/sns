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
//ɾ���ַ����пո��Ʊ��tab����Ч�ַ�
string Trim(string& str)
{
	//str.find_first_not_of(" \t\r\n"),���ַ���str�д�����0��ʼ�������״β�ƥ��"\t\r\n"��λ��
	str.erase(0, str.find_first_not_of(" \t\r\n"));
	str.erase(str.find_last_not_of(" \t\r\n") + 1);
	return str;
}

vtkSmartPointer<vtkVertexGlyphFilter> InputDataExtract(string filename)
{
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	ifstream fin(filename);   //�����������룩���ļ���  ������ȡ�ļ�����
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
	                             // fin ���ڶ�ȡ��������  getline �˺����ɶ�ȡ����
	while (getline(fin, line))   //���ж�ȡ�����з���\n�����֣������ļ�β��־eof��ֹ��ȡ //line ���������ַ�����string��������
	{
		//cout <<"ԭʼ�ַ�����"<< line << endl; //�������
		istringstream sin(line); //�������ַ���line���뵽�ַ�����istringstream��
		vector<string> fields;   //����һ���ַ�������
		string field;
		while (getline(sin, field, ' ')) //���ַ�����sin�е��ַ����뵽field�ַ����У��Զ���Ϊ�ָ���
		{
			if(field.length() != 0 && field != " ")
				fields.push_back(field); //���ոն�ȡ���ַ�����ӵ�����fields��
		}
		/*cout << "fields[3] = " << fields[3] << endl;*/

		//string sQw = Trim(fields[5]);
		//string sQx = Trim(fields[6]);
		//string sQy = Trim(fields[7]);  
		//string sQz = Trim(fields[8]);
 		string sx = Trim(fields[3]);
		string sy = Trim(fields[7]);
		string sz = Trim(fields[11]);

		//�ַ���ת����double������
		Vector3d point;
		istringstream streamx, streamy, streamz;
		streamx.str(sx), streamy.str(sy), streamz.str(sz);
		streamx >> x, streamy >> y, streamz >> z;
		point << x, y, z;
		//cout << point << endl;         //point ����EM����ϵ��
		point = R.inverse() * point;   //point ���ھ���Rѡ��֮�������ϵ�µ�
		//�¶�ȡ�����ݸ����ļ��νṹ
		/*points->InsertNextPoint(point[0] + 180 , point[1] + 209.753, point[2] - 250); */
		points->InsertNextPoint(point[0]-150, point[1]+150, point[2]+200);
	}
	fin.close();  //�ر��ļ�

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