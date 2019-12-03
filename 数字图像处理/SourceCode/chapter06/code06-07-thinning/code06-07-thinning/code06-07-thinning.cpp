/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: // code06-07-thinning
  ��  ��: // ���پ����ζ�������骵�
  ��  ��: // 2014��8��4��
  ��  ��: // ����ϸ���㷨��OpenCVʵ��
  �����б�:
    1. Mat thinning(const Mat &binaryImg); //�Զ�ֵͼ�����ϸ������
  ***********************************************************/
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

//*************************************************************************************
//�������ƣ�Mat thinning()
//�������ܣ��Զ�ֵͼ�����ϸ������
//����˵������ϸ����ֵͼ�񣬸�ͼ���б���ɫΪ0��ǰ��ɫ��Ŀ�꣩Ϊ255
//�� �� ֵ������ϸ�����ֵͼ��
//*************************************************************************************
Mat thinning(const Mat &binaryImg)
{
	int i, j, k;
	uchar p[11];
	int pos[9][2] = {{0,0}, {-1,0} , {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1}};
	int cond1, cond2, cond3, cond4, counter=0;
	bool pointsDeleted = true;
	Mat mask, dstImg;

	//ת��Ϊ0,1��ֵͼ��
	dstImg = binaryImg / 255;
	//��û�п�ɾ�����ص�
	while (pointsDeleted)
	{
		mask = Mat::zeros(dstImg.size(), CV_8UC1);//��ʼ��ģ��Ϊȫ0
		pointsDeleted = false;
		for (i=1; i<dstImg.rows-1; i++)
		{
			for (j=1; j<dstImg.cols-1; j++)
			{
				//��ȡ3*3�ṹԪ��p1~p9��Ӧ����ֵ, ����p1Ϊ���ĵ�
				for (k=1; k<10; k++)
					p[k] = dstImg.at<uchar>(i+pos[k-1][0], j+pos[k-1][1]);

				//�����ĵ�Ϊ����ɫ������
				if(p[1]==0) continue;

				//�������ĵ���Χ��������ֵ֮��
				cond1 = 0;
				for (k=2; k<10; k++) cond1 += p[k];

				//����p2~p9��0��1�仯�Ĵ���
				cond2 = 0;
				p[10] = p[2]; //���ڴ���k=8ʱ, p[k+2]Խ�����
				for (k=2; k<10; k+=2)
					cond2 += ((p[k]==0 && p[k+1]==1) + (p[k+1]==0 && p[k+2]==1));

				if(counter%2==0)//ż���ε����ж�����
				{
					cond3 = p[2] * p[4] * p[6];
					cond4 = p[4] * p[6] * p[8];
				}
				else//�����ε����ж�����
				{
					cond3 = p[2] * p[4] * p[8];
					cond4 = p[2] * p[6] * p[8];
				}
				//��ͬʱ��������1~����4
				if ((2<=cond1 && cond1<= 6) && (cond2==1) && (cond3==0) && (cond4==0))
				{
					pointsDeleted = true;
					mask.at<uchar>(i, j) = 1; //д���ɾ�������ص���ģ��
				}
			}
		}
		dstImg &= ~mask; //ͨ���߼������ɾ��Ŀ�����ص�
		counter++;
	}
	//�ָ�Ϊ0, 255��ֵͼ��
	dstImg *= 255;

	return dstImg;
}


int main( int argc, char** argv )
{
	Mat srcImg, binaryImg, dstImg;

	/// װ��ͼ��
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load image " << srcFileName << endl;
		return -1;
	}

	Mat grayImg (srcImg.size(), CV_8U);

	//��Դͼ��ת��Ϊ�Ҷ�ͼ��
	cvtColor( srcImg, grayImg, CV_BGR2GRAY );

	//���ϸ��Ŀ��Ϊ��ɫ������תͼ��
	grayImg = 255 - grayImg;

	//��ֵ������
	threshold(grayImg, binaryImg, 100, 255, THRESH_BINARY);

	//ϸ������
	dstImg = thinning(binaryImg);

	dstImg = 255 - dstImg;
	imwrite("..\\figures\\figure6_21_e.bmp", dstImg);

	imshow( "Before thinning", srcImg );
	imshow( "After thinning", dstImg );

	waitKey(0);
	return 0;
}
