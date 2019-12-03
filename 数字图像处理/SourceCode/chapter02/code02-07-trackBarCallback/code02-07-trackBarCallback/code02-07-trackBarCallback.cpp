/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code02-07-trackBarCallback.cpp
  ��  ��: ���
  ��  ��: 1.0
  ��  ��: 2014��3��1��
  ��  ��: ���û���ؼ���̬������ɫ�²����ļ�����
  ��  ��:
  �����б�:
      1. void colorReduce����ɫ�²���
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ���    2014/03/01     1.0     �����ļ�
***********************************************************/

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

//ȫ�ֱ���
const int slider_max = 64;
int slider;
Mat image;
Mat result;

void colorReduce(Mat& inputImage, Mat& outputImage, int div);
void on_trackbar(int pos, void *);

int main(int argc, char ** argv)
{
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

    namedWindow("Դͼ��");
    namedWindow("��ʾ���");

    slider = 0;
    createTrackbar("ColorReduce", "��ʾ���", &slider, slider_max, on_trackbar);

    imshow("Դͼ��",image);
    imshow("��ʾ���",image);

    //colorReduce(image,result,64);

    //imshow("��ʾ���",result);
    waitKey(0);
}

void colorReduce(Mat& inputImage, Mat& outputImage, int div)
{
    outputImage = inputImage.clone();
    int rows = outputImage.rows;
    int cols = outputImage.cols*outputImage.channels();
    if (outputImage.isContinuous())
    {
        cols = cols * rows;
        rows = 1; // 1ά����
    }

    for(int i = 0; i < rows; i++)
    {
        //uchar* data = inputImage.ptr<uchar>(i);
        uchar* dataout = outputImage.ptr<uchar>(i);
        for(int j = 0; j < cols; j++)
        {
            dataout[j] = dataout[j]/div*div + div/2;
        }
    }
}

// trackbar�¼��Ļص�����
void on_trackbar(int pos, void *)
{

    if(pos <= 0)
        result = image;
    else
        colorReduce(image, result, pos);

    imshow("��ʾ���", result);
}
