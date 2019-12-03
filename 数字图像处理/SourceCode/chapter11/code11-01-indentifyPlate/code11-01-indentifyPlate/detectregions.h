/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: detectregions.h
  ��  ��: ���
  ��  ��: 1.0
  ��  ��: 2014-03-11
  ��  ��: DetectRegions�ඨ�壬���ڶ�λ���ָ������
  ��  ��: ��
  �����б�:
    1. ��
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
       ���   2014-03-11     1.0     �������ļ�
***********************************************************/

#ifndef DETECTREGIONS_H
#define DETECTREGIONS_H

#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "plate.h"

using namespace cv;
using namespace std;

class DetectRegions
{
public:
    DetectRegions();
    ~DetectRegions();
// Get��Set��������ȡ������˽�г�Ա����
public:
    bool getSaveRegions();
    void setSaveRegions(bool isSaveRegions);
    bool getShowSteps();
    void setShowSteps(bool isShowSteps);
    string getStrFilename();
    void setStrFilename(string filename);
    Size getPlateSize();
    void setPlateSize(Size inSize);

public:
    vector<Plate> run(Mat input);

private:
    vector<Plate> segment(Mat input);
    bool verifySizes(RotatedRect mr);
    Mat histeq(Mat in);

protected:
private:
    bool saveRegions;
    bool showSteps;
    Size plateSize;
    string strFilename;
};

#endif // DETECTREGIONS_H
