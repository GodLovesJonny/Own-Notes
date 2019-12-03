/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: ocr.h
  ��  ��: ���
  ��  ��: 1.0
  ��  ��: 2014-03-11
  ��  ��: OCR�ඨ���ļ���ʵ�ֳ����ַ��ָ��ʶ��
  ��  ��: ��
  �����б�:
    1. ��
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
       ���   2014-03-11     1.0     �������ļ�
***********************************************************/
#ifndef OCR_H
#define OCR_H

#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/ml/ml.hpp"

#include "Plate.h"
#include "charsegment.h"

using namespace cv;
using namespace std;

#define CV_HORIZONTAL    1
#define CV_VERTICAL    0

class OCR
{
public:
    OCR();
    OCR(string trainFile, int charS = 20, int shinkS = 3);

    ~OCR();

// Get��Set��������ȡ������˽�г�Ա����
public:
    bool getDEBUG();
    void setDEBUG(bool Debug);
    bool getSaveSegments();
    void setSaveSegments(bool isSavedSeg);
    string getStrFilename();
    void setStrFilename(string filename);
    int getCharSize();
    void setCharSize(int charSizeVal);
    int getShinkSize();
    void setShinkSize(int shinkSizeVal);
    bool getIsTrained();
    void setIsTrained(bool trained);
    CvANN_MLP getAnnClassifier();
    void setAnnClassifier(CvANN_MLP annVal);
    CvKNearest getKnnClassifier();
    void setKnnClassifier(CvKNearest knnVal);
    int getknnKVal();
    void setknnKVal(int kval);

public:
    bool run(Plate *input);

private:
    Mat features(Mat input, int size, int num);
    Mat preprocessChar(Mat in);
    int classifyAnn(Mat f);
    void trainAnn(Mat trainData, Mat trainClasses, int nlayers);
    int classifyKnn(Mat f);
    void trainKnn(Mat trainSamples, Mat trainClasses, int k);
    vector< CharSegment > segment(Plate input);
    Mat Preprocess(Mat in, int newSize);
    Mat getVisualHistogram(Mat *hist, int type);
    void drawVisualFeatures(Mat character, Mat hhist, Mat vhist, Mat lowData, int num);
    Mat ProjectedHistogram(Mat img, int t, bool nomerlized = true);
    Mat delMargin(Mat img);
    void delNoCharArea(Mat &in);
    Rect getCharsROI(Mat &in);
    void createCharSegments(vector<CharSegment> &outputPlate, Mat &in);
    bool verifySizes(Mat r);

public:
    static const int numCharacters;
    static const string strCharacters[];

private:
    bool isDebug;
    bool isSaveSegments;
    string strFilename;
    string trainSamFilename;
    int charSize;
    int shinkSize;
    bool isTrained;
    Size plateSize;
    CvANN_MLP annClassifier;
    CvKNearest knnClassifier;
    int knnK;
};

#endif // OCR_H
