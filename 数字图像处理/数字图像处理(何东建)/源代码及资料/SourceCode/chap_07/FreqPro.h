// FreqPro.h: interface for the CFreqPro class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AREAPRO_H__AA3E7726_2EE5_40E0_8074_2039A3018333__INCLUDED_)
#define AFX_AREAPRO_H__AA3E7726_2EE5_40E0_8074_2039A3018333__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "matlib.h"



class CFreqPro : public CObject  
{
	DECLARE_DYNCREATE(CFreqPro)
public:
	CFreqPro();
	CFreqPro(CDibObject *pDibObject);
	virtual ~CFreqPro();

public:
	//����ͼ����Ҷ�任
	Mm FFT2(CDibObject *pDibObject = NULL);
	//����ͼ����Ҷ��任
	BOOL NFFT2(Mm matTransed);

	//����ͼ�����ұ任
	Mm DCT2(CDibObject *pDibObject = NULL);
	//����ͼ��������任
	BOOL NDCT2(Mm matTransed);

	//����ͼ���ֶ�ʲ-������任
	Mm WHT2(CDibObject *pDibObject = NULL);
	//����ͼ���ֶ�ʲ-��������任
	BOOL NWHT2(Mm matTransed);

	//����DibObject�����
	void SetDibObjectClass( CDibObject *pDibObject );

protected:
	//Ϊͼ�����ݸ����������
	Mm GetMatData();
	//Ϊ�����������ͼ������
	BOOL SetBits();

//��ϵ��Ժ���
#ifdef _DEBUG
	virtual void Dump( CDumpContext &dc) const;
	virtual void AssertValid() const;
#endif

private:
	BOOL bIs2Power(int nNum);
	CDibObject *m_pDibObject;

protected:

	//���һά��ɢ���ұ任
	Mm dct(Mm a, Mm n);
	Mm dct(Mm a);
	Mm dct();

	//��ɶ�ά��ɢ���ұ任
	Mm dct2(Mm arg1, Mm mrows, Mm ncols);
	Mm dct2(Mm arg1, Mm mrows);
	Mm dct2(Mm arg1);

	//���һά��ɢ������任
	Mm idct(Mm b, Mm n);
	Mm idct(Mm b);
	Mm idct();

	//��ɶ�ά��ɢ������任
	Mm idct2(Mm arg1, Mm mrows, Mm ncols);
	Mm idct2(Mm arg1, Mm mrows);
	Mm idct2(Mm arg1);

	//�ڲ��м����
	Mm m_matBits, m_matFilter, m_matTransed;
;
};

#endif // !defined(AFX_AREAPRO_H__AA3E7726_2EE5_40E0_8074_2039A3018333__INCLUDED_)
