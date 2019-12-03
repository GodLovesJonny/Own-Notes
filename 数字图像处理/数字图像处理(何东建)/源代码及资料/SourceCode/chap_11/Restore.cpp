// Restore.cpp: implementation of the CRestore class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dip.h"
#include "Restore.h"


IMPLEMENT_DYNCREATE(CRestore, CObject)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRestore::CRestore()
{


}

CRestore::~CRestore()
{
	exitM();
}

CRestore::CRestore(CDibObject *pDibObject)
{
	m_pDibObject=pDibObject;
	initM(MATCOM_VERSION);
}

/******************************************************************
*
*    ��������InvFilter()
*
*    ��  ������
*  
*    ����ֵ����������
*
*    ��  �ܣ���һ��ͼ��������˲���ԭ
*  
*    ��  ע���ú��������ڶ��ɾ������ɵ�ͼ��ģ�����и�ԭ
*
*******************************************************************/
BOOL CRestore::InvFilter()
{
	/************************************��ȡͼ������******************************/

	//ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);
	
	//��1λ��4λͼ�����κβ���ֱ�ӷ���
	if(m_pDibObject->GetNumBits() != 8) return( FALSE );
	
	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
			
	//�������
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNewWidthBytes, nNumColors;
	

	//���ͼ��ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		m_pDibObject->GetNumBits(),
		&nNewWidthBytes, 8);
	if( pBuffer == NULL ) return( NULL );
	
	//�����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//��õ�ɫ��ָ��
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//���λͼ����ָ��
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		+ nNumColors * sizeof(RGBQUAD)];
	if( pBuffer == NULL ) return( NULL );
 
    

    /******************************��ͼ������ת���ɶ�ά�����Խ��о�������*******************************/
	
	//ͼ���������ֽ���������nWidthBytesΪÿ��ͼ������ռ�õ��ֽ���
	DWORD SizeImage = nWidthBytes * nHeight;

	//ͨ������һ��1��SizeImage��һά����0����Ϊ������������ڴ�
	m_matBits = zeros(1, SizeImage);

	//��������double��ת��Ϊunsigned char�ͣ��Ա㽫ͼ�����ݸ�������
	m_matBits = muint8(m_matBits);

	//��������m_matBits.addr()��þ���ʵ�����ڴ�ָ�룬
	//Ȼ������memcpy()��ͼ������һ���Ը������������
	memcpy(m_matBits.addr(), pBits, SizeImage);

	//����Mm�͵ľ������ڴ����ǰ����к��е�˳��洢�ģ�
	//����Matrix<LIB>C++�Ŀ⺯��reshape()��һά����m_matBits��άΪ
	//nWidthBytes��nHeight�Ķ�ά��������rot90()����������ʱ����ת90�ȣ�
	//�������ΪnHeight��nWidthBytes�Ķ�ά����ʹͼ��ĵ�nHeight-1������
	//�洢�ھ���ĵ�0�У��Ӷ���ɽ�ͼ�����ݰ����µ��ϵ�˳��洢��
	m_matBits = rot90(reshape(m_matBits, nWidthBytes, nHeight));

	//��ͼ���������ֽڿ�Ȳ�ͬ������ϵͳ����Ϊÿ���ֽ���Ϊ4����������
	//����0ɾ�����Լ�С�����С�ӿ촦���ٶȡ�
	if(nWidthBytes != nWidth)
	{
		//�൱��MATLAB�е�X=X(:, 1: nWidth)����
		m_matBits = m_matBits(c_p, colon(1, 1, nWidth));
	}
	//��������unsigned char��ת��Ϊdouble�ͣ��Ա��������
	m_matBits = mdouble(m_matBits);
    
    //����ģ�壬��СΪ7x7
	Mm m_matFilter;
	m_matFilter=zeros(nHeight,nWidth);
	int i,j;
	for(i=1;i<7;i++)
	{
		for(j=1;j<7;j++){
			m_matFilter.r(i,j)=0.02;
		}

	}


    /**********************************��ͼ����о��ģ������****************************/

	//��ԭͼ����п��ٸ���Ҷ�任����ͼ��ӿ���任��Ƶ��
	m_matBits=fft2(m_matBits);
	//��ģ����п��ٸ���Ҷ�任
	m_matFilter=fft2(m_matFilter);

	//Ƶ�����
	double a,b,c,d;
	for(i=1;i<=nHeight;i++)
	{
		for(j=1;j<=nWidth;j++)
		{
			a=m_matBits.r(i,j);
			b=m_matBits.i(i,j);
			c=m_matFilter.r(i,j);
			d=m_matFilter.i(i,j);
			if(c*c+d*d>0.003)
			{
				m_matBits.r(i,j)=(a*c+b*d)/(c*c+d*d);
				m_matBits.i(i,j)=(b*c-a*d)/(c*c+d*d);
			}
		}
	}

	//��ͼ����п����渵��Ҷ�任����ͼ���Ƶ��任������
	m_matBits=ifft2(m_matBits);


	/**********************************ͼ����������**********************************/

	//��Matrix<LIB>C++�Ŀ⺯�����������ݷ�Χ�޶���0��255
	m_matBits = mabs(m_matBits);
	Mm L = m_matBits > 255.0;
	Mm NotL = !L;
	m_matBits = times(m_matBits, NotL) + L * 255.0;

	//��������double��ת��Ϊunsigned char��
	m_matBits = muint8(m_matBits);
	//��ÿ�����ݵ��ֽ�����0����Ϊ4��������
	int nTmp = (int)rem(nWidth, 4);
	int nPadding;
	if(nTmp > 0)
	{
		nPadding = 4 - nTmp;
		m_matBits = cat(2, m_matBits, repmat(muint8(0), 
	(BR(size(m_matBits, 1)), nPadding)));
	}
	//��ͼ�����ݾ���˳ʱ����ת90�ȣ�ԭ��ͬǰ��
	m_matBits = rot90(m_matBits, -1);
	//��ͼ�����ݾ�����ͼ��������
	memcpy(pBits, m_matBits.addr(), (nWidthBytes * nHeight)*sizeof(unsigned char));
	
	return TRUE;
}

/******************************************************************
*
*    ��������ConvNoiseBlur()
*
*    ��  ������
*  
*    ����ֵ����������
*
*    ��  �ܣ���һ��ͼ����о������ģ��
*  
*    ��  ע����
*
*******************************************************************/
BOOL CRestore::ConvNoiseBlur()
{

	/************************************��ȡͼ������******************************/

	//ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);
	
	//��1λ��4λͼ�����κβ���ֱ�ӷ���
	if(m_pDibObject->GetNumBits() != 8) return( FALSE );
	
	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
			
	//�������
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNewWidthBytes, nNumColors;
	

	//���ͼ��ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		m_pDibObject->GetNumBits(),
		&nNewWidthBytes, 8);
	if( pBuffer == NULL ) return( NULL );
	
	//�����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//��õ�ɫ��ָ��
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//���λͼ����ָ��
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		+ nNumColors * sizeof(RGBQUAD)];
	if( pBuffer == NULL ) return( NULL );
 
    

    /******************************��ͼ������ת���ɶ�ά�����Խ��о�������*******************************/
	
	//ͼ���������ֽ���������nWidthBytesΪÿ��ͼ������ռ�õ��ֽ���
	DWORD SizeImage = nWidthBytes * nHeight;

	//ͨ������һ��1��SizeImage��һά����0����Ϊ������������ڴ�
	m_matBits = zeros(1, SizeImage);

	//��������double��ת��Ϊunsigned char�ͣ��Ա㽫ͼ�����ݸ�������
	m_matBits = muint8(m_matBits);

	//��������m_matBits.addr()��þ���ʵ�����ڴ�ָ�룬
	//Ȼ������memcpy()��ͼ������һ���Ը������������
	memcpy(m_matBits.addr(), pBits, SizeImage);

	//����Mm�͵ľ������ڴ����ǰ����к��е�˳��洢�ģ�
	//����Matrix<LIB>C++�Ŀ⺯��reshape()��һά����m_matBits��άΪ
	//nWidthBytes��nHeight�Ķ�ά��������rot90()����������ʱ����ת90�ȣ�
	//�������ΪnHeight��nWidthBytes�Ķ�ά����ʹͼ��ĵ�nHeight-1������
	//�洢�ھ���ĵ�0�У��Ӷ���ɽ�ͼ�����ݰ����µ��ϵ�˳��洢��
	m_matBits = rot90(reshape(m_matBits, nWidthBytes, nHeight));

	//��ͼ���������ֽڿ�Ȳ�ͬ������ϵͳ����Ϊÿ���ֽ���Ϊ4����������
	//����0ɾ�����Լ�С�����С�ӿ촦���ٶȡ�
	if(nWidthBytes != nWidth)
	{
		//�൱��MATLAB�е�X=X(:, 1: nWidth)����
		m_matBits = m_matBits(c_p, colon(1, 1, nWidth));
	}
	//��������unsigned char��ת��Ϊdouble�ͣ��Ա��������
	m_matBits = mdouble(m_matBits);
    


    //����ģ�壬��СΪ7x7
	Mm m_matFilter;
	m_matFilter=zeros(nHeight,nWidth);
	int i,j;
	for(i=1;i<7;i++)
	{
		for(j=1;j<7;j++){
			m_matFilter.r(i,j)=0.02;
		}

	}


   /**************************************��ͼ����о������ģ������******************************/

	//��ԭͼ����п��ٸ���Ҷ�任����ͼ��ӿ���任��Ƶ��
	m_matBits=fft2(m_matBits);
	//��ģ����п��ٸ���Ҷ�任
	m_matFilter=fft2(m_matFilter);

	//Ƶ�����
	for(i=1;i<=nHeight;i++)
	{
		for(j=1;j<=nWidth;j++)
		{
			m_matBits.r(i,j)=m_matBits.r(i,j)*m_matFilter.r(i,j)-m_matBits.i(i,j)*m_matFilter.i(i,j);
			m_matBits.i(i,j)=m_matBits.r(i,j)*m_matFilter.i(i,j)+m_matBits.i(i,j)*m_matFilter.r(i,j);
		}
	}

	//��ͼ���������渵��Ҷ�任,��ͼ���Ƶ��任������
	m_matBits=ifft2(m_matBits);

	for(i=1;i<=nHeight;i++)
	{
		for(j=1;j<=nWidth;j++)
		{
			if((i+j)==((int) (i+j)/8)*8)
			{
				m_matBits.r(i,j)=m_matBits.r(i,j)-16;
			}
		}
	}

	   
	
	/**********************************ͼ����������**********************************/

	//��Matrix<LIB>C++�Ŀ⺯�����������ݷ�Χ�޶���0��255
	m_matBits = mabs(m_matBits);
	Mm L = m_matBits > 255.0;
	Mm NotL = !L;
	m_matBits = times(m_matBits, NotL) + L * 255.0;

	//��������double��ת��Ϊunsigned char��
	m_matBits = muint8(m_matBits);
	//��ÿ�����ݵ��ֽ�����0����Ϊ4��������
	int nTmp = (int)rem(nWidth, 4);
	int nPadding;
	if(nTmp > 0)
	{
		nPadding = 4 - nTmp;
		m_matBits = cat(2, m_matBits, repmat(muint8(0), 
	(BR(size(m_matBits, 1)), nPadding)));
	}
	//��ͼ�����ݾ���˳ʱ����ת90�ȣ�ԭ��ͬǰ��
	m_matBits = rot90(m_matBits, -1);
	//��ͼ�����ݾ�����ͼ��������
	memcpy(pBits, m_matBits.addr(), (nWidthBytes * nHeight)*sizeof(unsigned char));

	return TRUE;
}

/******************************************************************
*
*    ��������wiener()
*
*    ��  ������
*  
*    ����ֵ����������
*
*    ��  �ܣ���һ��ͼ�����ά���˲���ԭ
*  
*    ��  ע���ú��������ڶ��ɾ����������ɵ�ͼ��ģ�����и�ԭ
*
*******************************************************************/
BOOL CRestore::Wiener()
{

	/************************************��ȡͼ������******************************/

	//ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);
	
	//��1λ��4λͼ�����κβ���ֱ�ӷ���
	if(m_pDibObject->GetNumBits() != 8) return( FALSE );
	
	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
			
	//�������
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNewWidthBytes, nNumColors;
	

	//���ͼ��ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		m_pDibObject->GetNumBits(),
		&nNewWidthBytes, 8);
	if( pBuffer == NULL ) return( NULL );
	
	//�����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//��õ�ɫ��ָ��
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//���λͼ����ָ��
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		+ nNumColors * sizeof(RGBQUAD)];
	if( pBuffer == NULL ) return( NULL );
 
    
    /******************************��ͼ������ת���ɶ�ά�����Խ��о�������*******************************/
	
	//ͼ���������ֽ���������nWidthBytesΪÿ��ͼ������ռ�õ��ֽ���
	DWORD SizeImage = nWidthBytes * nHeight;

	//ͨ������һ��1��SizeImage��һά����0����Ϊ������������ڴ�
	m_matBits = zeros(1, SizeImage);

	//��������double��ת��Ϊunsigned char�ͣ��Ա㽫ͼ�����ݸ�������
	m_matBits = muint8(m_matBits);

	//��������m_matBits.addr()��þ���ʵ�����ڴ�ָ�룬
	//Ȼ������memcpy()��ͼ������һ���Ը������������
	memcpy(m_matBits.addr(), pBits, SizeImage);

	//����Mm�͵ľ������ڴ����ǰ����к��е�˳��洢�ģ�
	//����Matrix<LIB>C++�Ŀ⺯��reshape()��һά����m_matBits��άΪ
	//nWidthBytes��nHeight�Ķ�ά��������rot90()����������ʱ����ת90�ȣ�
	//�������ΪnHeight��nWidthBytes�Ķ�ά����ʹͼ��ĵ�nHeight-1������
	//�洢�ھ���ĵ�0�У��Ӷ���ɽ�ͼ�����ݰ����µ��ϵ�˳��洢��
	m_matBits = rot90(reshape(m_matBits, nWidthBytes, nHeight));

	//��ͼ���������ֽڿ�Ȳ�ͬ������ϵͳ����Ϊÿ���ֽ���Ϊ4����������
	//����0ɾ�����Լ�С�����С�ӿ촦���ٶȡ�
	if(nWidthBytes != nWidth)
	{
		//�൱��MATLAB�е�X=X(:, 1: nWidth)����
		m_matBits = m_matBits(c_p, colon(1, 1, nWidth));
	}
	//��������unsigned char��ת��Ϊdouble�ͣ��Ա��������
	m_matBits = mdouble(m_matBits);


    //����ģ��ģ�壬��СΪ7x7
	Mm m_matFilter;
	m_matFilter=zeros(nHeight,nWidth);
	int i,j;
	for(i=1;i<7;i++)
	{
		for(j=1;j<7;j++){
			m_matFilter.r(i,j)=0.02;
		}

	}

	//��������ģ��
	Mm m_matNoise;
	m_matNoise=zeros(nHeight,nWidth);
	for(i=1;i<=nHeight;i++)
	{
		for(j=1;j<=nWidth;j++)
		{
			if((i+j)==((int) ((i+j)/8))*8)
			{
				m_matNoise.r(i,j)=-16;
			}
		}
	}
 

    /************************************��ͼ�����Wiener�˲���ԭ***************************/

	//��ԭͼ����п��ٸ���Ҷ�任����ͼ��ӿ���任��Ƶ��
	m_matBits=fft2(m_matBits);
	//��ģ��ģ����п��ٸ���Ҷ�任
	m_matFilter=fft2(m_matFilter);
	//������ģ����п��ٸ���Ҷ�任
	m_matNoise=fft2(m_matNoise);
	double a,b,c,d,e,f,multi;
	for(i=1;i<=nHeight;i++)
	{
		for(j=1;j<=nWidth;j++)
		{
			a=m_matBits.r(i,j);
			b=m_matBits.i(i,j);
			c=m_matFilter.r(i,j);
			d=m_matFilter.i(i,j);
			e=m_matNoise.r(i,j);
			f=m_matNoise.i(i,j);

			multi=(a*a+b*b)/(a*a+b*b-e*e-f*f);
			if(c*c+d*d>0.003)
			{
				m_matBits.r(i,j)=(a*c+b*d)/(c*c+d*d)/multi;
				m_matBits.i(i,j)=(b*c-a*d)/(c*c+d*d)/multi;
			}
		}
	}

	//�Դ��������п����渵��Ҷ�任����ͼ���Ƶ��任������
	m_matBits=ifft2(m_matBits);


	/**********************************ͼ����������**********************************/

	//��Matrix<LIB>C++�Ŀ⺯�����������ݷ�Χ�޶���0��255
	m_matBits = mabs(m_matBits);
	Mm L = m_matBits > 255.0;
	Mm NotL = !L;
	m_matBits = times(m_matBits, NotL) + L * 255.0;

	//��������double��ת��Ϊunsigned char��
	m_matBits = muint8(m_matBits);
	//��ÿ�����ݵ��ֽ�����0����Ϊ4��������
	int nTmp = (int)rem(nWidth, 4);
	int nPadding;
	if(nTmp > 0)
	{
		nPadding = 4 - nTmp;
		m_matBits = cat(2, m_matBits, repmat(muint8(0), 
	(BR(size(m_matBits, 1)), nPadding)));
	}
	//��ͼ�����ݾ���˳ʱ����ת90�ȣ�ԭ��ͬǰ��
	m_matBits = rot90(m_matBits, -1);
	//��ͼ�����ݾ�����ͼ��������
	memcpy(pBits, m_matBits.addr(), (nWidthBytes * nHeight)*sizeof(unsigned char));

	return TRUE;
}


/******************************************************************
*
*   ��������ConvBlur()
*
*   ��  ������
*  
*   ����ֵ����������
*
*   ��  �ܣ���һ��ͼ����о��ģ��
*  
*   ��  ע����
*
*******************************************************************/
BOOL CRestore::ConvBlur()
{

	/************************************��ȡͼ������******************************/

	//ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);
	
	//��1λ��4λͼ�����κβ���ֱ�ӷ���
	if(m_pDibObject->GetNumBits() != 8) return( FALSE );
	
	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
			
	//�������
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNewWidthBytes, nNumColors;
	

	//���ͼ��ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		m_pDibObject->GetNumBits(),
		&nNewWidthBytes, 8);
	if( pBuffer == NULL ) return( NULL );
	
	//�����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//��õ�ɫ��ָ��
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//���λͼ����ָ��
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		+ nNumColors * sizeof(RGBQUAD)];
	if( pBuffer == NULL ) return( NULL );
 
    
    /******************************��ͼ������ת���ɶ�ά�����Խ��о�������*******************************/
	
	//ͼ���������ֽ���������nWidthBytesΪÿ��ͼ������ռ�õ��ֽ���
	DWORD SizeImage = nWidthBytes * nHeight;

	//ͨ������һ��1��SizeImage��һά����0����Ϊ������������ڴ�
	m_matBits = zeros(1, SizeImage);

	//��������double��ת��Ϊunsigned char�ͣ��Ա㽫ͼ�����ݸ�������
	m_matBits = muint8(m_matBits);

	//��������m_matBits.addr()��þ���ʵ�����ڴ�ָ�룬
	//Ȼ������memcpy()��ͼ������һ���Ը������������
	memcpy(m_matBits.addr(), pBits, SizeImage);

	//����Mm�͵ľ������ڴ����ǰ����к��е�˳��洢�ģ�
	//����Matrix<LIB>C++�Ŀ⺯��reshape()��һά����m_matBits��άΪ
	//nWidthBytes��nHeight�Ķ�ά��������rot90()����������ʱ����ת90�ȣ�
	//�������ΪnHeight��nWidthBytes�Ķ�ά����ʹͼ��ĵ�nHeight-1������
	//�洢�ھ���ĵ�0�У��Ӷ���ɽ�ͼ�����ݰ����µ��ϵ�˳��洢��
	m_matBits = rot90(reshape(m_matBits, nWidthBytes, nHeight));

	//��ͼ���������ֽڿ�Ȳ�ͬ������ϵͳ����Ϊÿ���ֽ���Ϊ4����������
	//����0ɾ�����Լ�С�����С�ӿ촦���ٶȡ�
	if(nWidthBytes != nWidth)
	{
		//�൱��MATLAB�е�X=X(:, 1: nWidth)����
		m_matBits = m_matBits(c_p, colon(1, 1, nWidth));
	}
	//��������unsigned char��ת��Ϊdouble�ͣ��Ա��������
	m_matBits = mdouble(m_matBits);


    
    //����ģ�壬��СΪ7x7
	Mm m_matFilter;
	m_matFilter=zeros(nHeight,nWidth);
	int i,j;
	for(i=1;i<7;i++)
	{
		for(j=1;j<7;j++){
			m_matFilter.r(i,j)=0.02;
		}

	}

    /**************************************��ͼ����о��ģ������****************************/

	//��ԭͼ����п��ٸ���Ҷ�任����ͼ��ӿ���任��Ƶ��
	m_matBits=fft2(m_matBits);
	//��ģ����п��ٸ���Ҷ�任
	m_matFilter=fft2(m_matFilter);

	//Ƶ�����
	for(i=1;i<=nHeight;i++)
	{
		for(j=1;j<=nWidth;j++)
		{
			m_matBits.r(i,j)=m_matBits.r(i,j)*m_matFilter.r(i,j)-m_matBits.i(i,j)*m_matFilter.i(i,j);
			m_matBits.i(i,j)=m_matBits.r(i,j)*m_matFilter.i(i,j)+m_matBits.i(i,j)*m_matFilter.r(i,j);
		}
	}
	//��ͼ�����渵��Ҷ�任����ͼ���Ƶ��任������
	m_matBits=ifft2(m_matBits);


	/**********************************ͼ����������**********************************/

	//��Matrix<LIB>C++�Ŀ⺯�����������ݷ�Χ�޶���0��255
	m_matBits = mabs(m_matBits);
	Mm L = m_matBits > 255.0;
	Mm NotL = !L;
	m_matBits = times(m_matBits, NotL) + L * 255.0;

	//��������double��ת��Ϊunsigned char��
	m_matBits = muint8(m_matBits);
	//��ÿ�����ݵ��ֽ�����0����Ϊ4��������
	int nTmp = (int)rem(nWidth, 4);
	int nPadding;
	if(nTmp > 0)
	{
		nPadding = 4 - nTmp;
		m_matBits = cat(2, m_matBits, repmat(muint8(0), 
	(BR(size(m_matBits, 1)), nPadding)));
	}
	//��ͼ�����ݾ���˳ʱ����ת90�ȣ�ԭ��ͬǰ��
	m_matBits = rot90(m_matBits, -1);
	//��ͼ�����ݾ�����ͼ��������
	memcpy(pBits, m_matBits.addr(), (nWidthBytes * nHeight)*sizeof(unsigned char));

	return TRUE;
}
