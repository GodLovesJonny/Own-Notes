/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code08-06-surfImgMat.cpp
  ��  ��: ���ͷ�
  ��  ��: 1.0
  ��  ��: 2014��4��20��
  ��  ��: ʵ��surfͼ��ƥ���㷨��
  ��  ��:
  �����б�:
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ���ͷ�  2014/04/20    1.0     �������ļ�
***********************************************************/

#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
	string firstFile, secondFile;
	cout << "Enter the first image file name: ";
	cin >> firstFile;
	cout << "Enter the second image file name: ";
	cin >> secondFile;
	
	Mat img1=imread(firstFile);
	if(img1.empty())
	{
		cerr << "Failed to load image " << firstFile << endl;
		return -1;
	}

	Mat img2=imread(secondFile);
	if(img2.empty())
	{
		cerr << "Failed to load image " << secondFile << endl;
		return -1;
	}


	// ת��Ϊ�Ҷ�ͼ��
	if (img1.channels()>=3)
		cvtColor(img1,img1,CV_RGB2GRAY);

	if (img2.channels()>=3)
		cvtColor(img2,img2,CV_RGB2GRAY);

  int minHessian = 8000;

  SurfFeatureDetector detector( minHessian );

  vector<KeyPoint> keyPnt1, keyPnt2;

  // ���surf�ؼ���
  detector.detect( img1, keyPnt1 );
  detector.detect( img2, keyPnt2 );

  SurfDescriptorExtractor extractor;

  Mat desc1, desc2;

  // ����surf��������
  extractor.compute( img1, keyPnt1, desc1 );
  extractor.compute( img2, keyPnt2, desc2 );

  // ƥ��surf������
  BFMatcher matcher(NORM_L2);
  vector< DMatch > matches;
  matcher.match( desc1, desc2, matches );

  // ��ƥ����
  Mat img_matches;
  drawMatches(img1, keyPnt1, img2, keyPnt2, matches, img_matches,CV_RGB(0,0,0),CV_RGB(0,0,0),Mat(),4);
  // ��ʾƥ����
  imshow("Matches", img_matches );

  waitKey(0);

  return 0;
  }