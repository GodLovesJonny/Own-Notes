// CMatrix.h: interface for the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMATRIX_H__CC2F3E15_9F82_11D4_A203_0000E81BB9DD__INCLUDED_)
#define AFX_CMATRIX_H__CC2F3E15_9F82_11D4_A203_0000E81BB9DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//��TYPE������double��������
#define TYPE double

class CMatrix  
{
public:
	//���캯��
	//�޲ι��캯��
	CMatrix();
	//���ò���RowNumber��ColumnNumber�������
	CMatrix(int RowNumber, int ColumnNumber);
	//�������캯�������ô���ľ���Matrix����������һ������
	CMatrix(CMatrix& Matrix);

	//��������
	virtual ~CMatrix();

	//��ָ����RowNumber��ColumnNumber��������
	Create(int RowNumber, int ColumnNumber);

	//��þ��������
	int GetRowNumber();
	//��þ��������
	int GetColumnNumber();

	//����������
	// * ������
	CMatrix operator *(CMatrix &Num2);
	// [] ������
	TYPE* CMatrix::operator[](int Row);
	// *= ������
	void CMatrix::operator *=(TYPE Number);
	// += ������
	void CMatrix::operator +=(CMatrix& Other);
	// = ������
	void CMatrix::operator =(CMatrix& Other);
	// + ������
	CMatrix CMatrix::operator +(CMatrix &Other);

	//���ڵ��Ե�ת������
	void Dump();
	void Fill(TYPE FillNumber);
	//���ɵ�λ��
	void MakeUnit();
private:
	//��������
	int ColumnNumber;
	//��������
	int RowNumber;
	//����Ԫ��
	TYPE *data;
};

#endif // !defined(AFX_CMATRIX_H__CC2F3E15_9F82_11D4_A203_0000E81BB9DD__INCLUDED_)
