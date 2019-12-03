/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code04-03-flip.cpp
  ��  ��: �λ���
  ��  ��: 1.0
  ��  ��: 2014-08-11
  ��  ��: ����ͼ��ľ���任����ʾ����ͼ��
  ��  ��: ��
  �����б�:
    1. ��
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      �λ���   2014-08-11     1.0     �������ļ�
***********************************************************/

#include <iostream>
#include <cmath>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	/// ����ͼ��
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat src = imread(srcFileName, 1);
	if(src.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

	/// ����任
	Mat dst;
	flip(src, dst, 1);

	/// ��ʾԭͼ��
	namedWindow("src", CV_WINDOW_AUTOSIZE);
	imshow("src", src);

	/// ��ʾ����ͼ��
	namedWindow("dst", CV_WINDOW_AUTOSIZE);
	imshow("dst", dst);

	waitKey(0);

	return 0;
}
