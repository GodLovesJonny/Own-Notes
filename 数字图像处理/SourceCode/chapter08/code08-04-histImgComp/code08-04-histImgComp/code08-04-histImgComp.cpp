/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code08-04-histImgComp.cpp
  ��  ��: ���ͷ�
  ��  ��: 1.0
  ��  ��: 2014��4��20��
  ��  ��: ʵ��ֱ��ͼƥ�䡣
  ��  ��:
  �����б�:
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ���ͷ�  2014/04/20    1.0     �������ļ�
***********************************************************/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
	Mat srcBas,srcTest1;

	// װ�����ű���������ͬ��ͼ��
	string firstFile, secondFile;
	cout << "Enter the first image file name: ";
	cin >> firstFile;
	cout << "Enter the second image file name: ";
	cin >> secondFile;
	
	srcBas=imread(firstFile);
	if(srcBas.empty())
	{
		cerr << "Failed to load image " << firstFile << endl;
		return -1;
	}

	srcTest1=imread(secondFile);
	if(srcTest1.empty())
	{
		cerr << "Failed to load image " << secondFile << endl;
		return -1;
	}

	/// ��RGBͼ��ռ䣬����Ϊ16*16*16
	int rBins = 16; int gBins = 16;int bBins = 16;
	int histSize[] = { rBins, gBins, bBins};

	// �ȼ������
	float rRngs[] = { 0, 256 };
	float gRngs[] = { 0, 256 };
	float bRngs[] = { 0, 256 };

	const float* ranges[] = { rRngs, gRngs, bRngs };

	// ʹ�õ�0��1��2ͨ��
	int channels[] = { 0, 1, 2 };

	/// ֱ��ͼ
	MatND histBas;
	MatND histTes1;

	/// ����RGBͼ���ֱ��ͼ
	calcHist( &srcBas, 1, channels, Mat(), histBas, 2, histSize, ranges, true, false );
	normalize( histBas, histBas, 0, 1, NORM_MINMAX, -1, Mat() );

	calcHist( &srcTest1, 1, channels, Mat(), histTes1, 2, histSize, ranges, true, false );
	normalize( histTes1, histTes1, 0, 1, NORM_MINMAX, -1, Mat() );

	///Ӧ�ò�ͬ��ֱ��ͼ�Աȷ���
	for( int i = 0; i < 4; i++ )
	{ 
		int compare_method = i;
		double base_test1 = compareHist( histBas, histTes1, compare_method );
		cout<<" Method "<<i<<" Base-Test1  "<< base_test1<<endl;
	}
	
	imshow("HistMatch",srcBas);  
	waitKey(-1);
  return(0);
}