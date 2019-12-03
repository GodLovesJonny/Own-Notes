/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: code08-05-siftImgMat.cpp
  ��  ��: ���ͷ�
  ��  ��: 1.0
  ��  ��: 2014��4��20��
  ��  ��: ʵ��siftͼ��ƥ���㷨��
  ��  ��:
  �����б�:
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
      ���ͷ�  2014/04/20    1.0     �������ļ�
***********************************************************/

#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp"  
#include "opencv2/nonfree/nonfree.hpp"  
#include "opencv2/nonfree/features2d.hpp"  

#include <iostream>  
#include <stdio.h>  
#include <stdlib.h>  
  
using namespace cv;  
using namespace std;  
  
int main(int argc, char** argv)  
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

	int thresh = 125;
	int max_thresh = 255;

    initModule_nonfree();//��ʼ��ģ��

    Ptr<FeatureDetector> detector = FeatureDetector::create( "SIFT" );//����SIFT���������  
    Ptr<DescriptorExtractor> descriptor_extractor = DescriptorExtractor::create( "SIFT" );//������������������  
    Ptr<DescriptorMatcher> descriptor_matcher = DescriptorMatcher::create( "BruteForce" );//��������ƥ����  
    if( detector.empty() || descriptor_extractor.empty() )  
        cout<<"fail to create detector!";  
  
  
    //��������  
    vector<KeyPoint> keyPnt1,keyPnt2;  
    detector->detect( img1, keyPnt1 );//���img1�е�SIFT�����㣬�洢��keyPnt1��  
    detector->detect( img2, keyPnt2 );  
    cout<<"ͼ��1���������:"<<keyPnt1.size()<<endl;  
    cout<<"ͼ��2���������:"<<keyPnt2.size()<<endl;  
  
    //����������������������Ӿ��󣬼�������������  
    Mat desc1,desc2;  
    descriptor_extractor->compute( img1, keyPnt1, desc1 );  
    descriptor_extractor->compute( img2, keyPnt2, desc2 );  

    cout<<"ͼ��1�������������С��"<<desc1.size()  
        <<"����������������"<<desc1.rows<<"��ά����"<<desc1.cols<<endl;  
    cout<<"ͼ��2�������������С��"<<desc2.size()  
        <<"����������������"<<desc2.rows<<"��ά����"<<desc2.cols<<endl;  
  
    //����������  
    Mat img_keyPnt1,img_keyPnt2;  
    drawKeypoints(img1,keyPnt1,img_keyPnt1,Scalar::all(-1),0);  
    drawKeypoints(img2,keyPnt2,img_keyPnt2,Scalar::all(-1),0);   
  
    //����ƥ��  
    vector<DMatch> matches;//ƥ����  
    descriptor_matcher->match( desc1, desc2, matches );//ƥ������ͼ�����������  
    cout<<"Match������"<<matches.size()<<endl;  
  
    //����ƥ�����о����������Сֵ  
    //������ָ���������������ŷʽ���룬�������������Ĳ��죬ֵԽС��������������Խ�ӽ�  
    double max_dist = 0;  
    double min_dist = 100;  
    for(int i=0; i<matches.size(); i++)  
    {  
        double dist = matches[i].distance;  
        if(dist < min_dist) min_dist = dist;  
        if(dist > max_dist) max_dist = dist;  
    }  
    cout<<"�����룺"<<max_dist<<endl;  
    cout<<"��С���룺"<<min_dist<<endl;  
  
    //ɸѡ���Ϻõ�ƥ���  
    vector<DMatch> goodMatches;  
    for(int i=0; i<matches.size(); i++)  
    {  
        if(matches[i].distance < 0.31 * max_dist)  
        {  
            goodMatches.push_back(matches[i]);  
        }  
    }  
    cout<<"goodMatch������"<<goodMatches.size()<<endl;  
  
    //����ƥ����  
    Mat img_matches;  
    drawMatches(img1,keyPnt1,img2,keyPnt2,goodMatches,img_matches,  
                CV_RGB(0,0,0),CV_RGB(0,255,0),Mat(),4);  
  
    imshow("MatchSIFT",img_matches);  
    waitKey(0);  
    return 0;  
}  