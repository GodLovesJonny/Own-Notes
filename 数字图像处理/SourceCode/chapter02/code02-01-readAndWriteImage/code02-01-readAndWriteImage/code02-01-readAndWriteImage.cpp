/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code02-01-readAndWriteImage.cpp
  ��  ��: ���
  ��  ��: 1.0
  ��  ��: 2014��3��1��
  ��  ��: ����OpenCV��д����ʾͼ��
  ��  ��:
  �����б�:
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ���    2014/03/01     1.0     �����ļ�
***********************************************************/


#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char ** argv)
{
    Mat image, result;

	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;

	/// ����ԭʼͼ��
	image = imread(srcFileName);
	if(image.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}

	cout << "size: " << image.size().height << " , "<< image.size().width << endl;

    namedWindow("Original Image");
    imshow("Original Image", image);

    flip(image,result,1);

    namedWindow("Output Image");
    imshow("Output Image", result);

    imwrite("output.bmp", result);

    waitKey(0);

    return 0;
}
