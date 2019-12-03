/************************************************************
  Copyright (C), 2008-2014, �������ӿƼ���ѧ�������Ȩ����.
  �ļ���: ocr.cpp
  ��  ��: ���
  ��  ��: 1.0
  ��  ��: 2014-03-11
  ��  ��: OCR��ʵ���ļ���ʵ�ֳ����ַ��ָ��ʶ��
  ��  ��: ��
  �����б�:
    1. ��
  �޸ļ�¼:
      <����>  <�޸�ʱ��>   <�汾>   <����>
       ���   2014-03-11     1.0     �������ļ�
***********************************************************/

#include "ocr.h"

// ʶ�������ұ�
const int OCR::numCharacters = 71;
const string OCR::strCharacters[] = {"0","1","2","3","4","5","6","7","8","9",
                                     "A","B","C","D","E","F","G","H","J","K",
                                     "L","M","N","P","Q","R","S","T","U","V",
                                     "W","X","Y","Z",
                                     "��","��","��","��","��","��","��","��",
                                     "��","��","��","��","��","��","³","ԥ",
                                     "��","��","��","��","��","��","��","��",
                                     "��","��","��","��","��","��","��","��",
                                     "��","ʹ","��","ѧ","��"
                                    };
OCR::OCR()
{
    isDebug = false;
    isTrained = false;
    isSaveSegments = false;
    charSize = 20;
    shinkSize = 3;
}

OCR::OCR(string trainFile, int charS, int shinkS)
{
    isDebug = false;
    isTrained = false;
    isSaveSegments = false;
    charSize = charS;
    shinkSize = shinkS;

    trainSamFilename = trainFile;

    // ��������XML�ļ��ڵ�����
    stringstream ss(stringstream::in | stringstream::out);
    ss.str("");
    ss << "TrainingDataF" << charSize * shinkSize / 4;

    // ����ѵ������XML�ļ�
    FileStorage fs;
    fs.open(trainSamFilename, FileStorage::READ);
    Mat TrainingData;
    Mat Classes;
    fs[ss.str()] >> TrainingData;
    fs["classes"] >> Classes;

    // ѵ���˹������������
    //trainAnn(TrainingData, Classes, 10);
    // ѵ��k-����ڷ�����
    trainKnn(TrainingData, Classes, 10);
}
OCR::~OCR()
{
    //dtor
}

bool OCR::getDEBUG()
{
    return isDebug;
}
void OCR::setDEBUG(bool Debug)
{
    isDebug = Debug;
}
bool OCR::getSaveSegments()
{
    return isSaveSegments;
}
void OCR::setSaveSegments(bool isSavedSeg)
{
    isSaveSegments = isSavedSeg;
}
string OCR::getStrFilename()
{
    return strFilename;
}
void OCR::setStrFilename(string filename)
{
    strFilename = filename;
}
int OCR::getCharSize()
{
    return charSize;
}
void OCR::setCharSize(int charSizeVal)
{
    charSize = charSizeVal;

    // ��������XML�ļ��ڵ�����
    stringstream ss(stringstream::in | stringstream::out);
    ss.str("");
    ss << "TrainingDataF" << charSize * shinkSize / 4;

    // ����ѵ������XML�ļ�
    FileStorage fs;
    fs.open(trainSamFilename, FileStorage::READ);
    Mat TrainingData;
    Mat Classes;
    fs[ss.str()] >> TrainingData;
    fs["classes"] >> Classes;

    // ѵ���˹������������
    //trainAnn(TrainingData, Classes, 10);
    // ѵ��k-����ڷ�����
    trainKnn(TrainingData, Classes, 10);
}
int OCR::getShinkSize()
{
    return shinkSize;
}
void OCR::setShinkSize(int shinkSizeVal)
{
    shinkSize = shinkSize;

    // ��������XML�ļ��ڵ�����
    stringstream ss(stringstream::in | stringstream::out);
    ss.str("");
    ss << "TrainingDataF" << charSize * shinkSize / 4;

    // ����ѵ������XML�ļ�
    FileStorage fs;
    fs.open(trainSamFilename, FileStorage::READ);
    Mat TrainingData;
    Mat Classes;
    fs[ss.str()] >> TrainingData;
    fs["classes"] >> Classes;

    // ѵ���˹������������
    //trainAnn(TrainingData, Classes, 10);
    // ѵ��k-����ڷ�����
    trainKnn(TrainingData, Classes, 10);
}
bool OCR::getIsTrained()
{
    return isTrained;
}
void OCR::setIsTrained(bool isTrained)
{
    isTrained = isTrained;
}
CvANN_MLP OCR::getAnnClassifier()
{
    return annClassifier;
}
void OCR::setAnnClassifier(CvANN_MLP annVal)
{
    annClassifier = annVal;
}
CvKNearest OCR::getKnnClassifier()
{
    return knnClassifier;
}
void OCR::setKnnClassifier(CvKNearest knnVal)
{
    knnClassifier = knnVal;
}

int OCR::getknnKVal()
{
    return knnK;
}
void OCR::setknnKVal(int kval)
{
    knnK = kval;
}

// ���α任��ͳһ�ַ�ͼ���С ���ں���ʶ��
Mat OCR::preprocessChar(Mat in)
{
    int h = in.rows;
    int w = in.cols;

    // �����任����
    Mat transformMat = Mat::eye(2, 3, CV_32F);
    int m = max(w, h);
    transformMat.at<float>(0, 2) = m / 2 - w / 2;
    transformMat.at<float>(1, 2) = m / 2 - h / 2;

    Mat warpImage(m, m, in.type());

    // ����任
    warpAffine(in, warpImage, transformMat,
               warpImage.size(),
               INTER_LINEAR, BORDER_CONSTANT, Scalar(0) );

    Mat out;
    // ���Ŵ�С
    resize(warpImage, out, Size(charSize, charSize));

    return out;
}

// �ַ�ȷ�ϣ����ַ��������棬���򷵻ؼ�
bool OCR::verifySizes(Mat r)
{
    // �ַ���߱ȣ�1/2��
    float aspect = 45.0f / 90.0f;
    float charAspect = (float)r.cols / (float)r.rows;
    float error = 0.35;
    float minHeight = 15;
    float maxHeight = 46;
    // �ַ�1�Ŀ�߱���������һ�£��˴���Ϊ0.2
    float minAspect = 0.2;
    // ����߱�
    float maxAspect = aspect + aspect * error;

    // ����������������з������صĸ�����
    float area = countNonZero(r);
    // ��������������ˣ�
    float bbArea = r.cols * r.rows;
    // �������ռ�ı���
    float percPixels = area / bbArea;

    if (isDebug)
    {
        cout << "Aspect: "<< aspect << " ["<< minAspect << "," << maxAspect << "] "  << "Area "<< percPixels <<" Char aspect " << charAspect  << " Height char "<< r.rows << "\n";
    }

    // �ж��Ƿ���������
    if (percPixels < 0.8
            && charAspect > minAspect && charAspect < maxAspect
            && r.rows >= minHeight && r.rows < maxHeight)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// ����ϡ��±߽�
// ����һ��������ֵ�ڰױ仯�Ĵ����ж���һ���ǲ�������
// ��ֵ��Ϊ10��
/**
�������㷨Ϊ�� ������������ɨ�����ˮƽ���ϵ����ضϵ�����
�����������������ĿС��ĳ������ֵ�У��򽫸���ȫ������,��������
����������е����������������ĳһ�̶���ֵ��˵��ɨ�赽���Ƶ��ַ�λ�ã�
��ô�ͽ�֮ǰ��С��ĳһ�̶���ֵ������������е��� ͬ�����Խ��±߿�õ���
ͬ�� �������Ƶķ��������Խ��������ұ߿�Ҳȥ������
"����OPENCV�ĳ�������ʶ��ϵͳ�о�_�������ӿƼ���ѧ_��˼Դ"˶ʿ���ĵ�5��P33
2.0���ʵ��
**/
Mat OCR::delMargin(Mat binPlate)
{
    Mat cropMarg;
    binPlate.copyTo(cropMarg);

    for (int i = 0; i < binPlate.rows; i++)
    {
        uchar pixelTmp = binPlate.at<uchar>(i, 0);
        int flipCounter = 0;
        for (int j = 1; j < binPlate.cols; j++)
        {
            if (binPlate.at<uchar>(i, j) != pixelTmp)
            {
                flipCounter++;
                pixelTmp = binPlate.at<uchar>(i, j);
            }
        }
        // 1��������ֵ�仯����10�Σ�Ӧɾ��
        /// �ɼ������10�α仯������3�У���ɾ��
        if (flipCounter < 10)
        {
            for (int k = 0; k < binPlate.cols; k++)
            {
                cropMarg.at<uchar>(i, k) = 0;
            }
        }
    }
    return cropMarg;
}

// ɾ������ͱ���������Ҫ�������
void OCR::delNoCharArea(Mat &in)
{
    // ɾ��������Ҫ�������
    Mat imgContours;
    in.copyTo(imgContours);

    // ���ҿ��ܵ�����
    vector< vector< Point > > contours;
    findContours(imgContours,
                 contours, // ��������
                 CV_RETR_EXTERNAL, // �ⲿ����
                 CV_CHAIN_APPROX_NONE); // ��¼���е�

    /// ���鴦��֮ǰͨ��������������н�cosֵ�������쳣�㣨ë�̣�
    /// 2.0�汾��һ��Ҫ�Ľ�

    // ����ÿ������
    vector<vector<Point> >::iterator itc = contours.begin();

    // ɾ������ͱ���������Ҫ�������
    while (itc != contours.end())
    {
        // ������������Ӿ���
        Rect mr = boundingRect(Mat(*itc));
        Mat auxRoi(in, mr);

        // �����߱ȣ����� / խ�ߣ�
        float r = 0.0;
        if( mr.width > mr.height)
        {
            r = (float)mr.width / (float)mr.height;
        }
        else
        {
            r = (float)mr.height / (float)mr.width;
        }

        if (r > 10.0)
        {
            // ������0
            auxRoi = Scalar(0);
            ++itc;
            continue;
        }

        // ���������
        float area = contourArea(*itc);
        // ��Ӿ������
        float bbArea = mr.height * mr.width;
        // ����ٷֱ�
        float percPixels = area / bbArea;
        if (percPixels >= 0.8)
        {
            auxRoi = Scalar(0);
            ++itc;
            continue;
        }
        ++itc;
    }
}

// �����ַ���ȡ����
Rect OCR::getCharsROI(Mat &in)
{
    // ����ˮƽ����ͶӰ����ͼ�����±߽�
    Mat ProjectedHistH = ProjectedHistogram(in, CV_HORIZONTAL, false);

    // ȡ�м�ֵ��1/3��Ϊ��ֵ
    int midPos = ProjectedHistH.cols / 2;
    int midHistH = ProjectedHistH.at<int>(midPos) / 3;

    int minH = 0;
    int maxH = ProjectedHistH.cols;

    // �Զ����´���
    for (int i = 0; i < ProjectedHistH.cols; i++)
    {
        if (ProjectedHistH.at<int>(i) > midHistH)
        {
            minH = i;
            break;
        }
    }
    // �������ϴ���
    for (int i = ProjectedHistH.cols - 1; i > minH; i--)
    {
        if (ProjectedHistH.at<int>(i) > midHistH)
        {
            maxH = i;
            break;
        }
    }
    // �������±߽紦���ĸ���Ȥ����
    Rect rectH(0, minH, in.cols, maxH - minH);
    Mat cropedH(in, rectH);

    // ���ô�ֱ����ͶӰ����ȡ��ˮƽ�߽�
    Mat ProjectedHistV = ProjectedHistogram(cropedH, CV_VERTICAL, false);

    int minV = 0;
    int maxV = ProjectedHistV.cols;
    vector< int > xPos;
    int flipFlag = 1;

    // ͳ�Ƹ�����ͶӰ�߽�ˮƽ���꣬ѹ��xPos����
    for (int i = 0; i < ProjectedHistV.cols; i++)
    {
        if(flipFlag)
        {
            // �����䣨��������߽磩
            if (ProjectedHistV.at<int>(i) > 5)
            {
                xPos.push_back(i);
                flipFlag = 0;
                continue;
            }
        }
        else
        {
            // �����䣨�������ұ߽磩
            if (ProjectedHistV.at<int>(i) < 5)
            {
                xPos.push_back(i);
                flipFlag = 1;
                continue;
            }
        }
    }

    // ���ұ߽��ֵ
    minV = xPos.empty() ? 0 : xPos.at(0);
    maxV = ProjectedHistV.cols;
    int xEnd = xPos.size();

    // ��ֱ����ͶӰ�����������7���ַ�����δ�ܼ����ַ�����
    if (xEnd >= 14)
    {
        // ����߿�ʼ����������߽�
        for (int i = 1; i < xEnd; i += 2)
        {
            if ((xPos.at(i) - xPos.at(i - 1)) > 3)
            {
                minV = xPos.at(i - 1);
                break;
            }
        }
        // ���ұ߿�ʼ���������ұ߽�
        for (int i = xEnd - 2 ; i > minV; i -= 2)
        {
            if ((xPos.at(i + 1) - xPos.at(i)) > 3)
            {
                maxV = xPos.at(i + 1);
                break;
            }
        }

        // �������������ұ߿������ַ����򣬵����ұ߽�
        // ��Ҫ��Ϊ���ұ���������ʱ����λ���޷�ѹ��xPos
        if ((ProjectedHistV.cols - maxV) > 5)
        {
            maxV = in.cols;
        }
    }

    // �жϲü�����Ϸ���
    if (minV >= maxV || minH >= maxH)
    {
        // �����ü�����
        Rect rectROI(0, 0, 0, 0);
        return rectROI;
    }
    else
    {
        // �����ü�����
        Rect rectROI(minV, minH, maxV - minV, maxH - minH);
        return rectROI;
    }
}

// ���ô�ֱ����ͶӰ�ָ��ַ�
void OCR::createCharSegments(vector<CharSegment> &outputPlate, Mat &in)
{
    // ���ɴ�ֱ����ͶӰ
    Mat projV = ProjectedHistogram(in, CV_VERTICAL, false);

    if(isDebug)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmp/" << strFilename << "_CropedPlate" << ".jpg";
        imwrite(ss.str(), in);
        imshow("Croped Plate", in);
        //waitKey(0);
    }

    // �����ַ�ˮƽ�ָ�λ������
    vector< int > charSegPosX;
    int flipFlag = 1;
    for (int i = 0; i < projV.cols; i++)
    {
        if(flipFlag)
        {
            if (projV.at<int>(i) > 3)
            {
                charSegPosX.push_back(i);
                flipFlag = 0;
                continue;
            }
        }
        else
        {
            if (projV.at<int>(i) < 3)
            {
                charSegPosX.push_back(i);
                flipFlag = 1;
                continue;
            }
        }
    }

    // �ָ��ַ�����
    Rect charRect(0, 0, in.cols, in.rows);

    // ��ʾ���
    Mat result;
    if (isDebug)
    {
        in.copyTo(result);
        cvtColor(result, result, CV_GRAY2RGB);
    }

    // ����ÿ���ַ�ռ�е�������
    float scalePerPixe = 409 / in.cols;
    int pixelsPerChar = 45 / scalePerPixe;
    int minPPC = pixelsPerChar - 6;
    int maxPPC = pixelsPerChar + 6;

    if (charSegPosX.size() >= 14)
    {
        for (int i = 1; i < (int)charSegPosX.size(); i += 2)
        {
            int xPixels = charSegPosX.at(i) - charSegPosX.at(i - 1);
            if (xPixels > minPPC && xPixels < maxPPC)
            {
                charRect.x = charSegPosX.at(i - 1);
                charRect.width = xPixels;
                Mat auxRoi(in, charRect);
                //auxRoi = 255 - auxRoi;
                auxRoi = preprocessChar(auxRoi);
                outputPlate.push_back(CharSegment(auxRoi, charRect));
                if (isDebug)
                {
                    rectangle(result, charRect, Scalar(0, 255, 0));
                }
            }
        }
        // ����Ϊ���������ұ߽���ܴ����ַ��������⴦��
        if (charSegPosX.size() % 2 == 1)
        {
            int xPixels = in.cols - charSegPosX.at(charSegPosX.size() - 1);
            if (xPixels > minPPC && xPixels < maxPPC)
            {
                charRect.x = charSegPosX.at(charSegPosX.size() - 1);
                charRect.width = xPixels;
                Mat auxRoi(in, charRect);
                //auxRoi = 255 - auxRoi;
                auxRoi = preprocessChar(auxRoi);
                outputPlate.push_back(CharSegment(auxRoi, charRect));
                if (isDebug)
                {
                    rectangle(result, charRect, Scalar(0, 255, 0));
                }
            }
        }
    }

    if(isDebug)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmp/" << strFilename << "_SegmentedChars" << ".jpg";
        imwrite(ss.str(), result);
        imshow("Segmented Chars", result);
    }
}

// �ָ��ַ�
vector<CharSegment> OCR::segment(Plate plate)
{
    Mat input = plate.getPlateImg();

    vector<CharSegment> output;

    // ��ֵ��
    Mat imgThreshold;
    threshold(input, imgThreshold, 161, 255, CV_THRESH_BINARY);

    if(isDebug)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmp/" << strFilename << "_BinaryPalate" << ".jpg";
        imwrite(ss.str(), imgThreshold);
    }

    // ɾ������ͱ���������Ҫ�������
    delNoCharArea(imgThreshold);

    if(isDebug)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmp/" << strFilename << "_BinaryPalate_delNoChar" << ".jpg";
        imwrite(ss.str(), imgThreshold);
    }

    // ȥ�����±�Ե����ÿ�кڰױ仯��ȥ����<10��ȥ����һ�У�
    imgThreshold = delMargin(imgThreshold);

    if (isDebug)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmp/" << strFilename << "_Thresholdplate_delMargin" << ".jpg";
        imwrite(ss.str(), imgThreshold);
        imshow("Threshold plate", imgThreshold);
    }

    // ���ô�ֱ��ˮƽ����ͶӰ�����ַ�����
    Rect ROI = getCharsROI(imgThreshold);
    // δ���ҵ��ַ������˳��ָ�
    if (ROI.size().width == 0 || ROI.size().height == 0)
    {
        return output;
    }

    /// 2.0������롰��̾�š�ȥճ��
    /// ���ɲο����Ŵ�ѧ˶ʿ���ġ�����OpenCV2.x�ĳ���ʶ��ϵͳ�����ʵ�֡���6�� P47��

    imgThreshold = imgThreshold(ROI);

    if (isDebug)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << "tmp/" << strFilename << "_Thresholdplate_ROI" << ".jpg";
        imwrite(ss.str(), imgThreshold);
        imshow("Threshold plate", imgThreshold);
    }

    // �ַ��ָ�
    createCharSegments(output, imgThreshold);

    if(isDebug)
    {
        cout << "Num chars: " << output.size() << "\n";
    }

    return output;
}

// �������ͶӰ
Mat OCR::ProjectedHistogram(Mat img, int t, bool nomerlized)
{
    // ˮƽ��ֱ�����ж�
    int sz=(t) ? img.rows : img.cols;
    Mat mhist = Mat::zeros(1, sz, CV_32F);

    // �������ͶӰ
    for (int j = 0; j < sz; j++)
    {
        Mat data = (t) ? img.row(j) : img.col(j);
        mhist.at<float>(j) = countNonZero(data);
    }

    if (nomerlized)
    {
        // ��һ��
        double min, max;
        minMaxLoc(mhist, &min, &max);

        if (max > 0)
        {
            mhist.convertTo(mhist, -1 , 1.0f / max, 0);
        }
    }
    else
    {
        // �����й�һ������ת��Ϊint��������
        mhist.convertTo(mhist, CV_32SC1);
    }

    return mhist;
}

// ��ʾ����ͶӰ
// ���ݻ���ͶӰֵ������������
Mat OCR::getVisualHistogram(Mat *hist, int type)
{
    /// �����Ϊ��Ա���������ṩ���ú����������ͨ����
    /// 2.0����
    int size = 100;
    Mat imHist;

    if (type == CV_HORIZONTAL)
    {
        imHist.create(Size(size, hist->cols), CV_8UC3);
    }
    else
    {
        imHist.create(Size(hist->cols, size), CV_8UC3);
    }

    imHist = Scalar(55, 55, 55);

    for (int i = 0; i < hist->cols; i++)
    {
        float value = hist->at<float>(i);
        int maxval = (int)(value * size);

        Point pt1;
        Point pt2, pt3, pt4;

        if(type == CV_HORIZONTAL)
        {
            pt1.x = pt3.x = 0;
            pt2.x = pt4.x = maxval;
            pt1.y = pt2.y = i;
            pt3.y = pt4.y = i + 1;

            line(imHist, pt1, pt2, CV_RGB(220, 220, 220), 1, 8, 0);
            line(imHist, pt3, pt4, CV_RGB(34, 34, 34), 1, 8, 0);

            pt3.y = pt4.y = i + 2;
            line(imHist, pt3, pt4, CV_RGB(44, 44, 44), 1, 8, 0);
            pt3.y = pt4.y = i + 3;
            line(imHist, pt3, pt4, CV_RGB(50, 50, 50), 1, 8, 0);
        }
        else
        {
            pt1.x = pt2.x = i;
            pt3.x = pt4.x = i + 1;
            pt1.y = pt3.y = size;
            pt2.y = pt4.y = size - maxval;

            line(imHist, pt1, pt2, CV_RGB(220, 220, 220), 1, 8, 0);
            line(imHist, pt3, pt4, CV_RGB(34, 34, 34), 1, 8, 0);

            pt3.x = pt4.x = i + 2;
            line(imHist, pt3, pt4, CV_RGB(44, 44, 44), 1, 8, 0);
            pt3.x = pt4.x = i + 3;
            line(imHist, pt3, pt4, CV_RGB(50, 50, 50), 1, 8, 0);
        }
    }

    return imHist ;
}

// ��ʾԭͼ��ˮƽ�ʹ�ֱ����ͶӰ���Ŵ�ͼ��
void OCR::drawVisualFeatures(Mat character, Mat hhist, Mat vhist, Mat lowData, int num)
{
    // 6����ԭͼ���С���м仭�ָ��ߣ���1������
    int width = charSize * 6 + 1;
    int height = charSize * 6 + 1;
    Mat img(width, height, CV_8UC3, Scalar(0,0,0));
    Mat ch;
    Mat ld;

    cvtColor(character, ch, CV_GRAY2RGB);

    // �Ŵ�ԭͼ5��
    resize(lowData, ld,
           Size(width - charSize - 1, height - charSize - 1),
           0, 0, INTER_NEAREST );

    cvtColor(ld, ld, CV_GRAY2RGB);

    Mat hh = getVisualHistogram(&hhist, CV_HORIZONTAL);
    Mat hv = getVisualHistogram(&vhist, CV_VERTICAL);

    Mat subImg = img(Rect(0, height - charSize, charSize, charSize));
    ch.copyTo(subImg);

    subImg = img(Rect(charSize + 1,
                      height - charSize,
                      width - charSize - 1,
                      charSize));
    resize(hh, hh, Size(width - charSize - 1, charSize));
    hh.copyTo(subImg);

    subImg = img(Rect(0, 0, charSize, height - charSize -1));
    resize(hv, hv, Size(charSize,  height - charSize -1));
    hv.copyTo(subImg);

    subImg = img(Rect(charSize + 1, 0,
                      width - charSize -1,
                      height - charSize -1));
    ld.copyTo(subImg);

    line(img,
         Point(0, height - charSize -1),
         Point(width, height - charSize -1),
         Scalar(0,0,255));
    line(img,
         Point(charSize, 0),
         Point(charSize, height),
         Scalar(0,0,255));

    stringstream ss(stringstream::in | stringstream::out);
    ss << "tmp/" << strFilename << "_VisualFeatures" << num << ".jpg";
    imwrite(ss.str(), img);
    imshow("Visual Features", img);

    //waitKey(0);
}

// ��ȡ����
Mat OCR::features(Mat in, int sizeData, int num)
{
    // ˮƽ�ʹ�ֱ����ͶӰ����
    Mat vhist = ProjectedHistogram(in, CV_VERTICAL);
    Mat hhist = ProjectedHistogram(in, CV_HORIZONTAL);

    // �ز���
    Mat lowData;
    resize(in, lowData, Size(sizeData, sizeData) );

    if (isDebug)
    {
        drawVisualFeatures(in, hhist, vhist, lowData, num);
    }

    // ������������
    int numCols = vhist.cols + hhist.cols
                  + lowData.rows * lowData.cols;

    Mat out = Mat::zeros(1, numCols, CV_32F);

    // Ϊ����������ֵ
    int j = 0;
    // ��ֱ����ͶӰֵ
    for (int i = 0; i < vhist.cols; i++)
    {
        out.at<float>(j)=vhist.at<float>(i);
        j++;
    }
    // ˮƽ����ͶӰֵ
    for (int i=0; i<hhist.cols; i++)
    {
        out.at<float>(j) = hhist.at<float>(i);
        j++;
    }
    // ԭͼ������ֵ
    for (int x = 0; x < lowData.cols; x++)
    {
        for (int y = 0; y < lowData.rows; y++)
        {
            out.at<float>(j) = (float)lowData.at<unsigned char>(x, y);
            j++;
        }
    }

    /// ������������45�ȷ������ͶӰ�������׼ȷ��
    /// 2.0���Ľ�

    if(isDebug)
    {
        //cout << out << "\n===========================================\n";
    }
    return out;
}

// ѵ���˹�������
void OCR::trainAnn(Mat TrainData, Mat classes, int nlayers)
{
    /// һֱδ�ܵ���ͨ��������������numCharacters���õ�ԭ��
    /// 2.0����飬���ڻ����ȷ���

    Mat layers(1, 3, CV_32SC1);
    layers.at<int>(0) = TrainData.cols;
    layers.at<int>(1) = nlayers;
    layers.at<int>(2) = numCharacters;
    annClassifier.create(layers, CvANN_MLP::SIGMOID_SYM, 1, 1);

    // ׼������
    Mat trainClasses;
    trainClasses.create( TrainData.rows, numCharacters, CV_32FC1 );
    for ( int i = 0; i <  trainClasses.rows; i++ )
    {
        for ( int k = 0; k < trainClasses.cols; k++ )
        {
            if ( k == classes.at<int>(i) )
            {
                trainClasses.at<float>(i,k) = 1;
            }
            else
            {
                trainClasses.at<float>(i,k) = 0;
            }
        }
    }
    Mat weights( 1, TrainData.rows, CV_32FC1, Scalar::all(1) );

    // ѵ��ѧϰ
    annClassifier.train( TrainData, trainClasses, weights );

    isTrained = true;
}

// �˹����������
int OCR::classifyAnn(Mat f)
{
    Mat output(1, numCharacters, CV_32FC1);
    annClassifier.predict(f, output);
    Point maxLoc;
    double maxVal;
    minMaxLoc(output, 0, &maxVal, 0, &maxLoc);

    return maxLoc.x;
}

// k-����ڷ���������
int OCR::classifyKnn(Mat f)
{
    int response = (int)knnClassifier.find_nearest( f, knnK );
    return response;
}

// ѵ��k-����ڷ�����
void OCR::trainKnn(Mat trainSamples, Mat trainClasses, int k)
{
    knnK = k;

    knnClassifier.train( trainSamples, trainClasses, Mat(), false, knnK );
}

// ���������ʶ��
bool OCR::run(Plate *input)
{
    // �ָ���е��ַ�
    vector<CharSegment> segments = segment(*input);

    if (segments.size() == 7)
    {
        for (int i = 0; i < (int)segments.size(); i++)
        {
            // ��ÿ���ַ���С�任Ϊͳһ�ߴ��С
            Mat ch = segments[i].getCharImg();
            if (isSaveSegments)
            {
                stringstream ss(stringstream::in | stringstream::out);
                ss << "tmpChars/" << strFilename << "_" << i << ".jpg";
                imwrite(ss.str(), ch);
            }
            // ��ȡ��������
            Mat f = features(ch, charSize * shinkSize / 4, i);
            // ���˹����������������ʶ��õ������������ֵ
            //int character = classifyAnn(f);
            // ��k-����ڷ���������ʶ��õ������������ֵ
            int character = classifyKnn(f);
            // �ڽ������ȡ���ַ������������ַ�������λ������д��ʶ����
            input->pushStrChars(strCharacters[character]);
            input->pushCharsPos(segments[i].getCharPos());
        }
        return true;
    }
    return false;
}


