/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: charsegment.h
  ��  ��: ���
  ��  ��: 1.0
  ��  ��: 2014-03-11
  ��  ��: CharSegment�ඨ�壬��¼�ַ�ͼ�����ڳ����е�λ��
  ��  ��: ��
  �����б�:
    1. ��
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
       ���   2014-03-11     1.0     �������ļ�
***********************************************************/
#ifndef CHARSEGMENT_H
#define CHARSEGMENT_H

#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core/core.hpp"

using namespace cv;
using namespace std;

class CharSegment
{
public:
    CharSegment();
    CharSegment(Mat matImg, Rect rectPos);
    ~CharSegment();

    // Get��Set��������ȡ������˽�г�Ա����
public:
    Mat getCharImg();
    void setCharImg(Mat img);
    Rect getCharPos();
    void setCharPos(Rect rectPos);

public:

protected:
private:
    Mat charImg;
    Rect charPos;
};

#endif // CHARSEGMENT_H
