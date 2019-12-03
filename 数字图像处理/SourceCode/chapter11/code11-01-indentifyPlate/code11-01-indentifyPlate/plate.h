/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: plate.h
  ��  ��: ���
  ��  ��: 1.0
  ��  ��: 2014-03-11
  ��  ��: Plate�ඨ�壬��¼��������ͼ��ͼ�����ԭͼ�е�λ��
  ��  ��: ��
  �����б�:
    1. ��
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
       ���   2014-03-11     1.0     �������ļ�
***********************************************************/
#ifndef PLATE_H
#define PLATE_H

#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core/core.hpp"

using namespace cv;
using namespace std;

class Plate
{
public:
    Plate();
    Plate(Mat img, Rect pos);
    ~Plate();

// Get��Set��������ȡ������˽�г�Ա����
public:
    Rect getPlatePos();
    void setPlatePos(Rect rectPos);
    Mat getPlateImg();
    void setPlateImg(Mat img);
    vector< string > getStrChars();
    void pushStrChars(string strChar);
    vector< Rect > getCharsPos();
    void pushCharsPos(Rect rectPos);

public:
    string str();
protected:
private:
    Rect platePos;
    Mat plateImg;
    vector< string > strChars;
    vector< Rect > charsPos;
};

#endif // PLATE_H
