/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code07-09-mutualMarkerWatershed.cpp
  ��  ��: ������
  ��  ��: 1.0
  ��  ��: 2014��8��8��
  ��  ��: ͨ����꽻�����Ʊ�ǣ�Marker����ʵ�ֻ��ڱ�ǵķ�ˮ��ָ
  ��  ��:
  �ο�����:
    1. OpenCVʾ�����룺samples/cpp/watershed.cpp
  �����б�:
    1. void onMouseEvent() ����¼��Ļص�����
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ������  2014/08/08    1.0     �������ļ�
***********************************************************/

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/// ����¼��ص�����������û���������
struct UserData
{
	Mat* pSrcImg;		// ԭʼͼ��
	Mat* pMarkerMask;	// ���ģ��
	Point ptPrev;		// ��һ���λ��
};

/**
 * ����¼��Ļص����������ڽ������Ʊ��
 */
static void onMouseEvent( int event, int x, int y, int flags, void* userData)
{
	UserData *pUserData = (UserData *)userData;
	if(!pUserData)
		return;

	if(x < 0 || x >= pUserData->pSrcImg->cols || y < 0 || y >= pUserData->pSrcImg->rows)
		return;

	if(event == CV_EVENT_LBUTTONUP || !(flags & CV_EVENT_FLAG_LBUTTON))
	{
		pUserData->ptPrev = Point(-1,-1);
	}
	else if( event == CV_EVENT_LBUTTONDOWN)
	{
		pUserData->ptPrev = Point(x,y);
	}
	else if( event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON) )
	{
		/// �����������ƶ�ʱ������������ǵ�����
		Point pt(x, y);
		if(pUserData->ptPrev.x < 0)
		{
			pUserData->ptPrev = pt;
		}

		// ���Ƶ����ģ�壨�������ɱ��ͼ��
		line(*pUserData->pMarkerMask, pUserData->ptPrev, pt, Scalar::all(255), 5, 8);

		// ���Ƶ�ԭʼͼ��������ʾ��ǹ��̣�
		line(*pUserData->pSrcImg, pUserData->ptPrev, pt, Scalar::all(255), 5, 8);

		// ���浱ǰ��
		pUserData->ptPrev = pt;

		imshow("Original image", *pUserData->pSrcImg);
	}
}

int main( int argc, char** argv )
{
	/// ��ӡ������Ϣ
	cout << "�ָ�ǰ����ԭͼ���������Թ���������Ŀ���뱳���ı�ǣ�\n"
		"\tESC - �˳�Ӧ�ó���\n"
		"\tr - �����ǣ��ָ�ԭͼ\n"
		"\tw or SPACE - ���ڱ�ǵķ�ˮ��ָ�\n";

	/// ����ԭʼͼ��
	string srcFileName;
	cout << "Enter the source image file name: ";
	cin >> srcFileName;
	Mat srcImg = imread(srcFileName);
	if(srcImg.empty())
	{
		cerr << "Failed to load image: " << srcFileName <<endl;
		return -1;
	}

	/// ����ͼ�񸱱������ڻָ�ԭʼͼ��
	Mat orgImg;
	srcImg.copyTo(orgImg);

	/// �������ģ�壨���ڼ�¼�û������ı�ǣ�
	Mat markerMask;
	cvtColor(srcImg, markerMask, COLOR_BGR2GRAY);

	/// �����Ҷȸ��������ڻ��ԭʼͼ����ָ�����
	Mat grayImg;
	cvtColor(markerMask, grayImg, COLOR_GRAY2BGR);
	markerMask = Scalar::all(0);

	/// ��ʾԭʼͼ��
	imshow("Original image", srcImg);

	/// �������ص�����
	UserData userData;
	userData.pSrcImg = &srcImg;
	userData.pMarkerMask = &markerMask;
	userData.ptPrev = Point(-1, -1);
	setMouseCallback( "Original image", onMouseEvent, &userData);

	/// ��������¼�
	bool bLoop = true;
	while(bLoop)
	{
		int c = waitKey(0);
		switch (c)
		{
		case 27:	// �˳�Ӧ�ó���
			bLoop = false;
			break;
		case 'r':	// �ָ�ԭʼ״̬
			markerMask = Scalar::all(0);
			orgImg.copyTo(srcImg);
			imshow("Original image", srcImg);
			break;
		case 'w':
		case ' ':	// Ӧ�û��ڱ�ǵķ�ˮ��ָ�
			{
				// ��ȡ��ǣ�marker��������
				vector<vector<Point> > contours;
				vector<Vec4i> hierarchy;
				findContours(markerMask, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

				// �������ͼ��ÿ����Ǹ���һ������0������ֵ���Ը�ֵ�����������
				Mat markerImg = Mat::zeros(markerMask.size(), CV_32S);				
				int compCount = 0;
				for(int idx = 0; idx >= 0; idx = hierarchy[idx][0], compCount++)
				{
					drawContours(markerImg, contours, idx, Scalar::all(compCount+1), CV_FILLED, 8, hierarchy);
				}

				// ���������ɫ�����ڸ�������ص���ͨ�ɷָ���һ�������ɫ��
				vector<Vec3b> colorTab;
				colorTab.reserve(compCount);
				for(int i = 0; i < compCount; i++)
				{
					int b = theRNG().uniform(0, 255);
					int g = theRNG().uniform(0, 255);
					int r = theRNG().uniform(0, 255);

					colorTab.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
				}

				// ִ�л��ڱ�ǵķ�ˮ��ָ����㣨����ͼ��Ҫ��CV_8UC3�����ͼ��Ҫ��ΪCV_32S��
				watershed(orgImg, markerImg);

				// �������ͼ����ʾ�ָ���
				Mat wsImg(markerImg.size(), CV_8UC3);
				for(int i = 0; i < markerImg.rows; i++)
				{
					for(int j = 0; j < markerImg.cols; j++)
					{
						int idx = markerImg.at<int>(i,j);
						if(idx == -1)
						{
							// �����ͼ���У���ˮ�����أ����Ϊ-1����Ϊ��ɫ
							wsImg.at<Vec3b>(i,j) = Vec3b(255,255,255);
						}
						else if(idx >= 1 && idx <= compCount)
						{
							// �����ͼ���У�ÿ����ͨ�ɷ֣����Ϊ1��compCount����Ϊĳ����ɫ
							wsImg.at<Vec3b>(i,j) = colorTab[idx-1];
						}
						else
						{
							// δ֪������Ϊ��ɫ��Ӧ�ò���������������
							wsImg.at<Vec3b>(i,j) = Vec3b(0,0,0);
						}
					}
				}

				// ��Ϸָ�����ԭʼ�Ҷ�ͼ�񣨿�ѡ��
				wsImg = wsImg * 0.5 + grayImg * 0.5;

				// ��ʾ�ָ���
				imshow( "Watershed segmentation", wsImg);
			}
			break;
		default:
			break;
		}
	}

	return 0;
}
