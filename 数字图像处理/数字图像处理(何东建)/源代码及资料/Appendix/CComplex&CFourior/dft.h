#ifndef FOURIOR
#define FOURIOR

//����������
class CComplex
{
protected:
	//ʵ�����鲿
	double Re,Im;
public:
	//���캯��
	CComplex();
	CComplex(double x,double y);
	//��������
	~CComplex();
	//���ʵ��
	double GetRe();
	//����鲿	
	double GetIm();
	//=�Ų���������
	void operator=(CComplex &cm);
	//+=�Ų���������
	void operator+=(CComplex &cm);
	//-=�Ų���������
	void operator-=(CComplex &cm);
	//*=�Ų���������
	void operator*=(CComplex &cm);
	//*�Ų���������
	CComplex operator*(CComplex &cm);
	//*=�Ų���������
	void operator*=(float var);
	// /=�Ų���������
	void operator/=(double x);
	// /�Ų���������
	CComplex operator/(double x);
	// +�Ų���������
	CComplex operator+(CComplex &cm);
	// -�Ų���������
	CComplex operator-(CComplex &cm);
	//��ģ����
	double abs();
};

////////////////////////////////////////////////////

//����Ҷ�任��
class CFourior
{
private:
	CComplex *Wn;
	BYTE *bWn;
	BOOL bFlag;
	int nBitsNum;
	int nByteNum;
public:
	//���캯��
	CFourior();
	CFourior(int N);
	//��������
	~CFourior();
	//DFT�任
	void DFT(CComplex *Input);
	//FFT�任
	void FFT(CComplex *Input);
	//����
	void SetInverseDFT();
	void SetInverseFFT();
	//���ø���Ҷ�任���ݵ���
	void SetCount(int N);
	//���ÿ��ٸ���Ҷ�任ϵ��
	BOOL SetFftWn();
	//���ø���Ҷ�任ϵ��
	BOOL SetDftWn();
protected:
};

#endif
