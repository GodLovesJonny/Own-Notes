/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code02-02-reduceColorWithAt.cpp
  ��  ��: ���
  ��  ��: 1.0
  ��  ��: 2014��3��1��
  ��  ��: ����at����ͼ�����ݣ�ʵ����ɫ�²�����
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

void colorReduce(Mat& inputImage, Mat& outputImage, int div);

int main(int argc, char ** argv)
{
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;

	/// ����ԭʼͼ��
	Mat image = imread(srcFileName);
	if(image.empty())
	{
		cerr << "Failed to load image file: " << srcFileName << endl;
		return -1;
	}


	imshow("Դͼ��",image);
    Mat result;//����������
    result.create(image.rows,image.cols,image.type());//���Ĵ�С��������ԭͼƬ��ƥ��
    double duration;
    duration = static_cast<double>(cv::getTickCount());

    colorReduce(image,result,64);

    duration = static_cast<double>(cv::getTickCount())-duration;
    duration /= cv::getTickFrequency(); // the elapsed time in m
    cout<<"time is"<<duration<<endl;

    imshow("��ʾ���",result);
    waitKey(0);
}

void colorReduce(Mat& inputImage, Mat& outputImage, int div)
{
    outputImage = inputImage.clone();
    int rows = outputImage.rows;
    int cols = outputImage.cols;
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            outputImage.at<Vec3b>(i,j)[0] = outputImage.at<Vec3b>(i,j)[0]/div*div + div/2;
            outputImage.at<Vec3b>(i,j)[1] = outputImage.at<Vec3b>(i,j)[1]/div*div + div/2;
            outputImage.at<Vec3b>(i,j)[2] = outputImage.at<Vec3b>(i,j)[2]/div*div + div/2;
        }
    }
}
