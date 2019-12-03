/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code04-02-move.cpp
  ��  ��: �λ���
  ��  ��: 1.0
  ��  ��: 2014-08-11
  ��  ��: ʵ��ͼ���ƽ�Ʋ�����ͼ����ʾ
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

	/// �����任����
	Mat transformMat = Mat::eye(2, 3, CV_32F);
	transformMat.at<float>(0, 2) = 50.0;
	transformMat.at<float>(1, 2) = 50.0;

	/// ����任
	Mat dst(src.cols, src.rows, src.type());
	warpAffine(src, dst, transformMat,
	           dst.size(),
	           INTER_LINEAR, BORDER_CONSTANT, Scalar(0) );

	/// ��ʾԭͼ��
	namedWindow("src", CV_WINDOW_AUTOSIZE);
	imshow("src", src);

    /// ��ʾ�任���ͼ��
	namedWindow("dst", CV_WINDOW_AUTOSIZE);
	imshow("dst", dst);

	waitKey(0);

	return 0;
}
