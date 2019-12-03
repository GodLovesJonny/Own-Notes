/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code07-04-regionGrow.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��7��31��
  ��  ��: ����OpenCV��floodfill����ʵ������������ͨ�����ѡ��
          �������������ӵ㣬ͨ�������������������򣨻ҶȲ��
  ��  ����OpenCVʾ�����룺samples\cpp\ffilldemo.cpp
  �����б�:
    1. void onMouseEvent() ����¼��ص�����
    2. void help() �������
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/07/31     1.0     �������ļ�
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;


const string winName = "RegionGrow"; // ��������
Mat srcImg;	// ԭʼͼ��
Mat dstImg;	// ���ͼ��

/// ������������ز���
int loDiff = 2;		// ��ǰ�������������أ��������򣩵ĻҶȲ�ֵ���޵ľ���ֵ
int upDiff = 2;		// ��ǰ�������������أ��������򣩵ĻҶȲ�ֵ���޵ľ���ֵ
int conType = 4;	// ������ͨ�ԣ�4��8��
int growMode = 0;	// ��������׼��0�����ݵ�ǰ��������������֮��ĻҶȲ�ֵ�жϣ�1�����ݵ�ǰ��������������֮��ĻҶȲ�ֵ�жϣ�

/// ���ص�����
static void onMouseEvent(int event, int x, int y, int flags, void* userdata);

/// ��ӡ����
static void help();

int main( int argc, char** argv )
{
	/// ��ӡ����
	help();

	/// ����ԭʼͼ��
	string srcFileName;
	cout << "Enter the source file name: ";
	cin >> srcFileName;
	srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load the source image." << endl;
		return -1;
	}
	srcImg.copyTo(dstImg);

	/// ����ͼ�񴰿�
	namedWindow(winName);

	/// ��������loDiff��upDiff�Ļ�����
	createTrackbar("loDiff", winName, &loDiff, 128, 0, 0);
	createTrackbar("upDiff", winName, &upDiff, 128, 0, 0);

	/// �������ص�����
	setMouseCallback(winName, onMouseEvent, &srcImg);

	/// ��ʾͼ��
	bool bLoop = true;
	while(bLoop)
	{
		imshow(winName, dstImg);

		int ch = waitKey(0);
		switch (ch)
		{
		case 27:
			bLoop = false;
			break;
		case '4':
			cout << "4-connectivity mode is set\n";
			conType = 4;
			break;
		case '8':
			cout << "8-connectivity mode is set\n";
			conType = 8;
			break;
		case 'f':
			if(growMode == 0)
			{
				growMode = 1;
				cout << "Fixed range floodfill mode is set\n";
			}
			else
			{
				growMode = 0;
				cout << "Floating range floodfill mode is set\n";
			}
			break;
		default:
			break;
		}
	}

    return 0;
}

/**
 * ��ӡ����
 */
static void help()
{
    cout << "Hot keys: \n"
            "\tESC - quit the program\n"
            "\tr - restore the original image\n"
            "\tf - switch floodfill mode between floating range and fixed range\n"
            "\t4 - use 4-connectivity mode\n"
            "\t8 - use 8-connectivity mode\n" << endl;
}

/**
 * ���ص���������ͨ����굥����ȡ�������������ӵ㣬��������������
 * @param event ����¼�����
 * @param x ����¼�����ʱ��ˮƽ�������
 * @param y ����¼�����ʱ�Ĵ�ֱ�������
 * @param flags ����¼���־
 * @param userdata ���ӵ��û�����
 */
static void onMouseEvent(int event, int x, int y, int flags, void* userdata)
{
	if(dstImg.empty())
	{
		return;
	}
	
	/// �������ѡ���������������ӵ�
	if(event == CV_EVENT_FLAG_LBUTTON)
	{
		// ָ�������������ӵ�
		Point seedPoint(x, y);

		// ָ�������������ɫ
		int r = (unsigned char)theRNG() & 255;
		int g = (unsigned char)theRNG() & 255;
		int b = (unsigned char)theRNG() & 255;
		Scalar regionColor = (dstImg.channels() == 1) ? Scalar(r * 0.299 + g * 0.587 + b * 0.114) : Scalar(r, g, b);
	
		/// ʵ����������
		// floodFill����������־����8λΪ���ģʽ����8λΪ��ͨ�����ͣ�
		int fillFlags = conType + 0xFF00 + (growMode == 1 ? CV_FLOODFILL_FIXED_RANGE : 0);
		int regionArea = floodFill(dstImg, seedPoint, regionColor, 0, 
			Scalar(loDiff, loDiff, loDiff), Scalar(upDiff, upDiff, upDiff), fillFlags);

		// ��ʾͼ��
		imshow(winName, dstImg);
		cout << "Region area: " << regionArea << endl;
	}

	/// �����Ҽ��������ͼ��
	if(event == CV_EVENT_FLAG_RBUTTON)
	{
		srcImg.copyTo(dstImg);

		// ��ʾͼ��
		imshow(winName, dstImg);
	}
}