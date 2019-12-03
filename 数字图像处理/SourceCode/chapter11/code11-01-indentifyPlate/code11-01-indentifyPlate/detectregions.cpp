/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: detectregions.cpp
  ��  ��: ���
  ��  ��: 1.0
  ��  ��: 2014-03-11
  ��  ��: DetectRegions��ʵ�֣����ڶ�λ���ָ������
  ��  ��: ��
  �����б�:
    1. ��
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
       ���   2014-03-11     1.0     �������ļ�
***********************************************************/

#include <ctime>

#include "detectregions.h"

DetectRegions::DetectRegions() : saveRegions(false),
    showSteps(false)
{
    plateSize = Size(160, 51);
}

DetectRegions::~DetectRegions()
{
    //dtor
}

bool DetectRegions::getSaveRegions()
{
    return saveRegions;
}
void DetectRegions::setSaveRegions(bool isSaveRegions)
{
    saveRegions = isSaveRegions;
}
bool DetectRegions::getShowSteps()
{
    return showSteps;
}
void DetectRegions::setShowSteps(bool isShowSteps)
{
    showSteps = isShowSteps;
}
string DetectRegions::getStrFilename()
{
    return strFilename;
}
void DetectRegions::setStrFilename(string filename)
{
    strFilename = filename;
}

Size DetectRegions::getPlateSize()
{
    return plateSize;
}
void DetectRegions::setPlateSize(Size inSize)
{
    plateSize = inSize;
}

// �ÿ�߱ȼ������������Ƿ���ϳ�������
bool DetectRegions::verifySizes(RotatedRect mr)
{
    /// ������Щ������Ϊ��Ա�������Ա������ã����ͨ����
    /// 2.0�汾��һ��Ҫ�Ľ�
    // �ݲ�ϵ��
    float error = 0.4;
    // ���Ƴߴ�: 440x140 ��߱ȣ�3.4
    float aspect = 440.0 / 140.0;

    // �����Χ��С���ֵ��������һ��Χ�����򶼽�������
    int min = 35 * aspect * 35; // ��С���
    int max = 155 * aspect * 155; // ������

    // ���ƿ�߱��ݲΧ����С���ֵ
    float rmin = aspect - aspect * error;
    float rmax = aspect + aspect * error;

    // �������
    int area = mr.size.height * mr.size.width;
    // �����߱ȣ���/��
    float r = (float)mr.size.width / (float)mr.size.height;

    // r < 1���¼����߱ȣ���/��
    if (r < 1)
    {
        r = (float)mr.size.height / (float)mr.size.width;
    }

    // �ǳ������򷵻�true�����򷵻�false
    if (( area < min || area > max ) || ( r < rmin || r > rmax ))
    {
        return false;
    }
    else
    {
        return true;
    }
}

// ֱ��ͼ���⻯
Mat DetectRegions::histeq(Mat in)
{
    Mat out(in.size(), in.type());

    if (in.channels() == 3)
    {
        // 3ͨ����ɫͼ��ת��Ϊhsv��ɫ�ռ�
        // ��H����ͨ�����о��⻯
        // �ٺϲ���ת��ΪRGBͼ��
        Mat hsv;
        vector<Mat> hsvSplit;

        cvtColor(in, hsv, CV_BGR2HSV);

        split(hsv, hsvSplit);

        // ��H����ͨ�����о��⻯
        equalizeHist(hsvSplit[2], hsvSplit[2]);

        merge(hsvSplit, hsv);

        cvtColor(hsv, out, CV_HSV2BGR);
    }
    else if (in.channels() == 1)
    {
        equalizeHist(in, out);
    }

    return out;
}

// �������
vector<Plate> DetectRegions::run(Mat input)
{
    // ��⳵�����򣬲���÷ָ��Ŀ�������ͼ������ԭλ��
    vector<Plate> detectedPlates = segment(input);

    // ���ؼ�⵽�Ŀ����ǳ��Ƶ�����
    return detectedPlates;
}

// �ָ�ͼ��
vector<Plate> DetectRegions::segment(Mat input)
{
    // ������
    vector<Plate> output;

    // ��ͼ��ת��Ϊ�Ҷ�ͼ��
    Mat imgGray;
    cvtColor(input, imgGray, CV_BGR2GRAY);
    // ƽ��ȥ�루5*5��˹ģ�壩
    blur(imgGray, imgGray, Size(5, 5));

    // ���õ��ǳ���������ڴ�����ֱ��Ե���г��Ʒָ�
    Mat imgSobel;
    // ����Sobel�˲�����һ��ˮƽ�����������Ҵ�ֱ��Ե
    Sobel(imgGray, imgSobel, CV_8U, 1, 0, 3, 1, 0, BORDER_DEFAULT);
    //Canny( imgGray, imgSobel, 50, 180, 3);
    // ��ʾ�м䴦����
    if (showSteps)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmp/" << strFilename << "_Canny" << ".jpg";
        imwrite(ss.str(), imgSobel);
        imshow("Canny", imgSobel);
    }

    // ͼ���ֵ��
    Mat imgThreshold;
    // Otsu����Ӧ��ֵ�㷨���ͼ���ֵ������ֵ
    threshold(imgSobel, imgThreshold,
              0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);

    // ��ʾ�м䴦����
    if (showSteps)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmp/" << strFilename << "_Threshold" << ".jpg";
        imwrite(ss.str(), imgThreshold);
        imshow("Threshold", imgThreshold);
    }

    // ��̬ѧ�ղ���
    // ���ñղ�����ȥ��ÿ����ֱ��Ե��֮��Ŀհף�
    // ���������а���������Ե������
    /// Ӧ����̬ѧ�����˳ߴ�Size(17, 1)����Ϊ���Ա������
    /// �Ա��ڸ��ݲ�ͬ������в������ã����ͨ����
    /// 2.0�汾��һ��Ҫ�Ľ�
    Mat elementMorph = getStructuringElement(MORPH_RECT, Size(17, 1));
    morphologyEx(imgThreshold, imgThreshold, CV_MOP_CLOSE, elementMorph);

    // ��ʾ�м䴦����
    if(showSteps)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmp/" << strFilename << "_Close" << ".jpg";
        imwrite(ss.str(), imgThreshold);
        imshow("Close", imgThreshold);
    }

    // ����ⲿ����
    vector< vector< Point > > contours;
    findContours(imgThreshold,
                 contours, // �����洢����
                 CV_RETR_EXTERNAL, // �������������
                 CV_CHAIN_APPROX_NONE); // ��¼���洢����������

    /// ���鴦��֮ǰͨ��������������н�cosֵ�������쳣�㣨ë�̣�
    /// 2.0�汾��һ��Ҫ�Ľ�

    // ���õ����������⵽��ÿһ������
    vector< vector<Point> >::iterator itc = contours.begin();
    vector< RotatedRect > rects;

    // ����������Ӿ��εĿ�߱Ⱥ����������ɾ����������
    while (itc != contours.end())
    {
        // ���������С��Ӿ���
        RotatedRect mr = minAreaRect(Mat(*itc));

        // ����Ҫ�����������޳�������
        if ( !verifySizes(mr))
        {
            itc = contours.erase(itc);
        }
        else
        {
            ++itc;
            rects.push_back(mr);
        }
    }

    // ��������
    Mat result;
    if (showSteps)
    {
        input.copyTo(result);
        drawContours(result, contours,
                     -1,                // ������������
                     Scalar(255,0,0),   // ����ɫ����
                     1);                // �����������Ϊ1
    }

    // ����ʹ��floodfill�㷨����ָ����ɫ���ĳһ�ܱ�����
    // ��ˮ�㷨����ȡ��������
    for (int i = 0; i < (int)rects.size(); i++)
    {
        if(showSteps)
        {
            // ��������Ӿ�������ΪԲ�ģ��뾶ȡ3��������ɫԲ
            circle(result, rects[i].center, 3, Scalar(0, 255, 0), -1);
        }

        // ������Ӿ��ο����Сֵ
        float minSize = (rects[i].size.width < rects[i].size.height) ?
                        rects[i].size.width : rects[i].size.height;
        //minSize = minSize - minSize * 0.5;
        minSize *= 0.5;

        // ���ȡ���ĵ㸽����5���㣨floodfill�㷨����
        // �������������
        srand ( time(NULL) );

        // ��ʼ��floodfill�㷨�����ͱ���
        Mat mask;
        mask.create(input.rows + 2, input.cols + 2, CV_8UC1);
        mask = Scalar::all(0);

        /// ����Ϊ���Ա������
        /// �Ա��ڸ��ݲ�ͬ������в������ã����ͨ����
        /// 2.0�汾��һ��Ҫ�Ľ�
        int loDiff = 40;
        int upDiff = 40;
        int connectivity = 4;
        int newMaskVal = 255;
        int NumSeeds = 10;

        Rect ccomp;

        int flags = connectivity + (newMaskVal << 8 )
                    + CV_FLOODFILL_FIXED_RANGE + CV_FLOODFILL_MASK_ONLY;

        for (int j = 0; j < NumSeeds; j++)
        {
            // �����������
            Point seed;

            seed.x = rects[i].center.x
                     + rand() % (int)minSize - (minSize / 2);
            seed.y = rects[i].center.y
                     + rand() % (int)minSize - (minSize / 2);

            // С��0����ֵΪ0
            if (seed.x < 0)
            {
                seed.x = 0;
            }
            if (seed.y < 0)
            {
                seed.y = 0;
            }

            if(showSteps)
            {
                circle(result, seed, 1, Scalar(0, 255, 255), -1);
            }

            floodFill(input, mask, seed,
                      Scalar(255, 0, 0),
                      &ccomp,
                      Scalar(loDiff, loDiff, loDiff),
                      Scalar(upDiff, upDiff, upDiff),
                      flags);
        }

        // ��ʾ�м䴦����
        if (showSteps)
        {
            stringstream ss(stringstream::in | stringstream::out);
            ss << "tmp/" << strFilename << "_MASK" << ".jpg";
            imwrite(ss.str(), mask);
            imshow("MASK", mask);
        }

        // ����Щ�ü������ٴ����ݺ�Ⱥ��������������֤��
        // ��ȥ��ͼ�����ת�����ü�ͼ��ͳһ�ߴ磬���⻯ͼ��ĻҶ�

        // ��¼floodfill�㷨�õ���maskͼ���е���������
        vector< Point > pointsInterest;
        Mat_< uchar >::iterator itMask = mask.begin<uchar>();
        Mat_< uchar >::iterator end = mask.end<uchar>();

        for ( ; itMask != end; ++itMask)
        {
            if (*itMask == 255)
            {
                pointsInterest.push_back(itMask.pos());
            }
        }

        // ���������С��Ӿ���
        RotatedRect minRect = minAreaRect(pointsInterest);

        // ����Ҫ��(��߱Ⱥ����)
        if (verifySizes(minRect))
        {
            // ���ƽ��
            if(showSteps)
            {
                // ȡ�þ����ĸ��������꣬�����ƾ���
                Point2f rectPoints[4];
                minRect.points(rectPoints);
                // ����ѭ������j=3ʱ��(j + 1) % 4 = 0��
                // �ص���ʼ�㣬�ܺ���ķ���Ӵ
                for (int j = 0; j < 4; j++)
                {
                    line(result,
                         rectPoints[j],
                         rectPoints[(j + 1) % 4],
                         Scalar(0, 0, 255), 1, 8 );
                }
            }

            // ������ת����
            float r = (float)minRect.size.width
                      / (float)minRect.size.height;
            float angle = minRect.angle;

            if (r < 1)
            {
                angle = 90 + angle;
            }
            Mat rotmat = getRotationMatrix2D(minRect.center, angle, 1);

            // У��ͼ��
            Mat imgRotated;
            warpAffine(input, imgRotated,
                       rotmat, input.size(),
                       CV_INTER_CUBIC);

            // �ü�ͼ��
            Size rectSize = minRect.size;
            if (r < 1)
            {
                swap(rectSize.width, rectSize.height);
            }
            Mat imgCrop;
            getRectSubPix(imgRotated, rectSize,
                          minRect.center, imgCrop);

            Mat resultResized;
            // ע����ò���plateSize�������ã�Ĭ��ΪSize(160, 51)
            resultResized.create(plateSize, CV_8UC3);
            resize(imgCrop, resultResized,
                   resultResized.size(), 0, 0, INTER_CUBIC);

            // ֱ��ͼ���⻯
            Mat grayResult;
            cvtColor(resultResized, grayResult, CV_BGR2GRAY);
            blur(grayResult, grayResult, Size(3, 3));
            grayResult = histeq(grayResult);

            // ����ȡ������洢Ϊͼ����ΪSVMѵ������
            if (saveRegions)
            {
                stringstream ss(stringstream::in | stringstream::out);
                ss << "tmp/" << strFilename << "_" << i << ".jpg";
                imwrite(ss.str(), grayResult);
            }
            // ������
            output.push_back(Plate(grayResult, minRect.boundingRect()));
        }
    }

    // ��ʾ�м䴦����
    if(showSteps)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmp/" << strFilename << "_Contours" << ".jpg";
        imwrite(ss.str(), result);
        imshow("Contours", result);
    }

    return output;
}

