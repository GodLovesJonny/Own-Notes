// GeoTrans.cpp: implementation of the CGeoTrans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GeoTrans.h"
#include "math.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CGeoTrans, CObject)

//Diagnostics and dump member functions, overridden
#ifdef _DEBUG
void CGeoTrans::Dump(CDumpContext &dc) const
{
	//call base class function first
	CObject::Dump(dc);
}
#endif

#ifdef _DEBUG
void CGeoTrans::AssertValid() const
{
	//call inherited AssertValid first
	CObject::AssertValid();
	//Check CDibObject members...
	ASSERT(m_pDibObject != NULL);	//Must exist
}
#endif


/***********************************************************************
*                                                                      *
*                               ���δ�����                             *
*                                                                      *
***********************************************************************/

////////////////////////////////////////////////////////////////////////
//���캯��CGeoTrans()    
//----------------------------------------------------------------------
//�������ܣ�����һ��CGeoTrans��Ķ����粻����CDibObject���󡣵�   
//			һ�ε���ĳһ��������ʱ�������һ��CDibObject����ָ�롣
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//��    �أ���
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CGeoTrans::CGeoTrans()
{

}

////////////////////////////////////////////////////////////////////////
//���캯��CGeoTrans()    
//----------------------------------------------------------------------
//�������ܣ�����һ��CGeoTrans��Ķ��󲢴���CDibObject�������е�
//			��������Ըö���ֱ����һ��������Ϊ����������ͼ��������
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//��    �أ���
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CGeoTrans::CGeoTrans( CDibObject *pDibObject )
{
	m_pDibObject = pDibObject;
}

CGeoTrans::~CGeoTrans()
{
}

////////////////////////////////////////////////////////////////////////
//void SetDibObjectClass(CDibObject *pDibObject)   
//----------------------------------------------------------------------
//�������ܣ�������ΪCGeoTrans�����ָ��һ��CDibObject����ָ��
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject, Ĭ��ΪNULL��
//----------------------------------------------------------------------
//��    �أ��ޡ�
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CGeoTrans::SetDibObjectClass( CDibObject *pDibObject )
{
	m_pDibObject = pDibObject;
}


////////////////////////////////////////////////////////////////////////
//BOOL TranslationPixel()  
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObject�����е�ͼ�����ƽ�Ʋ������������
//			�˵���֮ǰû��ָ��һ��CDibObject����ָ�룬������ڵ���ʱ��
//			��ָ����
//----------------------------------------------------------------------
//����˵����long  lXOffset     - X��ƽ��������������
//			long  lYOffset     - Y��ƽ��������������
//			CDibObject *pDibObject, Ĭ��ΪNULL
//----------------------------------------------------------------------
//���أ�BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
//ע    �⣺�ú�������ˮƽ�ƶ�DIBͼ�񡣺�������ı�ͼ��Ĵ�С���Ƴ��Ĳ���
//			ͼ�񽫽�ȥ���հײ����ð�ɫ��䡣�ú�������ƶ�ͼ��
////////////////////////////////////////////////////////////////////////
BOOL CGeoTrans::TranslationPixel(long lXOffset, 
									  long lYOffset, 
									  CDibObject *pDibObject)
{
	//ʹ�ô����CDibObject����
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//���ͼ���Ⱥ͸߶�
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();

	//�������
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits, *pNewBits;
	unsigned char *pOldTemp, *pNewTemp;
	BITMAPFILEHEADER *pOldBFH;
	BITMAPINFOHEADER *pOldBIH;
	RGBQUAD *pOldPalette;
	int nWidthBytes, nNumColors, x, y;

	//ԭͼ��ָ��
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
																m_pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );

	//ԭͼ���ļ�ͷָ��
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//ԭͼ����Ϣͷָ��
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//ԭͼ����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//ԭͼ���ɫ��ָ��
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)
									+sizeof(BITMAPINFOHEADER)];
	//ԭͼ������ָ��
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)
											+sizeof(BITMAPINFOHEADER)
											+nNumColors*sizeof(RGBQUAD)];

	//Ϊ��ͼ�������������ڴ�
	DWORD dwNewSize;
	HGLOBAL hNewDibBits;
	//��ͼ����������С
	dwNewSize = nWidthBytes * nHeight;
	//Ϊ��ͼ������ڴ�
	hNewDibBits = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	if( hNewDibBits == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ��ָ��
	pNewBits = (unsigned char *) ::GlobalLock( hNewDibBits );
	if( pNewBits == NULL )
	{
		::GlobalFree( hNewDibBits );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��255����ɫ�������ͼ��������
	memset(pNewBits, (BYTE)255, nWidthBytes * nHeight );
	
	//��ͼ��λ��ȷ�����ƶ��ֽ���
	int nMovedBits = 1;
	switch( m_pDibObject->GetNumBits() )
	{
	case 8:	
		nMovedBits = 1;
		break;
	case 16:
		nMovedBits = 2;
		break;
	case 24:
		nMovedBits = 3;
		break;
	case 32:
		nMovedBits = 4;
		break;
	}

	//ÿ��
	for(y = 0; y < nHeight; y++)
	{
		pNewTemp = pNewBits;
		//ע������DIB��ͼ���һ����ʵ���������һ�е�λ�á�
		pNewTemp +=  (nHeight - 1 - y) * nWidthBytes;
		//ÿ��
		for(x = 0; x < nWidth; x++)
		{
			//ָ����DIB��y�У���x�����ص�ָ��
			//�����������ԴDIB�е�����
			int x0 = x - lXOffset;
			int y0 = y - lYOffset;
			
			//�ж��Ƿ���Դͼ��Χ��
			if( (x0 >= 0) && (x0 < nWidth) 
				&& (y0 >= 0) && (y0 < nHeight))
			{
				//ָ��ԴDIB��y0�У���x0�����ص�ָ��
				//ͬ��Ҫע��DIB���µ��õ�����
				pOldTemp = pOldBits;
				pOldTemp += (nHeight - 1 - y0) * nWidthBytes;
				pOldTemp += x0 * nMovedBits;
				//��������
				memcpy( pNewTemp, pOldTemp, nMovedBits );
			}
			pNewTemp += nMovedBits;
		}
	}
	
	//����ͼ���������ԭͼ��������
	memcpy( pOldBits, pNewBits, nWidthBytes * nHeight );

	::GlobalFree( hNewDibBits );
	::GlobalUnlock( m_pDibObject->GetDib() );

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL TranslationLine()  
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObject�����е�ͼ�����ƽ�Ʋ������������
//			�˵���֮ǰû��ָ��һ��CDibObject����ָ�룬������ڵ���ʱ��
//			��ָ����
//----------------------------------------------------------------------
//����˵����long  lXOffset     - X��ƽ��������������
//			long  lYOffset     - Y��ƽ��������������
//			CDibObject *pDibObject, Ĭ��ΪNULL
//----------------------------------------------------------------------
//���أ�BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
//ע    �⣺�ú�������ˮƽ�ƶ�DIBͼ�񡣺�������ı�ͼ��Ĵ�С���Ƴ��Ĳ���
//			ͼ�񽫽�ȥ���հײ����ð�ɫ��䡣�ú��������ƶ�ͼ��
////////////////////////////////////////////////////////////////////////
BOOL CGeoTrans::TranslationLine(long lXOffset, 
									 long lYOffset, 
									 CDibObject *pDibObject)
{
	//ʹ�ô����CDibObject����
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//���ͼ���Ⱥ͸߶�
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();

	//�������
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits, *pNewBits;
	unsigned char *pOldTemp, *pNewTemp;
	BITMAPFILEHEADER *pOldBFH;
	BITMAPINFOHEADER *pOldBIH;
	RGBQUAD *pOldPalette;
	int nWidthBytes, nNumColors, i;

	//ԭͼ��ָ��
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
																m_pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );

	//ԭͼ���ļ�ͷָ��
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//ԭͼ����Ϣͷָ��
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//ԭͼ����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//ԭͼ���ɫ��ָ��
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)
									+sizeof(BITMAPINFOHEADER)];
	//ԭͼ������ָ��
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)
											+sizeof(BITMAPINFOHEADER)
											+nNumColors*sizeof(RGBQUAD)];

	//Ϊ��ͼ�������������ڴ�
	DWORD dwNewSize;
	HGLOBAL hNewDibBits;
	//��ͼ����������С
	dwNewSize = nWidthBytes * nHeight;
	//Ϊ��ͼ������ڴ�
	hNewDibBits = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	if( hNewDibBits == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ��ָ��
	pNewBits = (unsigned char *) ::GlobalLock( hNewDibBits );
	if( pNewBits == NULL )
	{
		::GlobalFree( hNewDibBits );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��255����ɫ�������ͼ��������
	memset(pNewBits, (BYTE)255, nWidthBytes * nHeight );
	
	//��ͼ��λ��ȷ�����ƶ��ֽ���
	int nMovedBits = 1;
	switch( m_pDibObject->GetNumBits() )
	{
	case 8:	
		nMovedBits = 1;
		break;
	case 16:
		nMovedBits = 2;
		break;
	case 24:
		nMovedBits = 3;
		break;
	case 32:
		nMovedBits = 4;
		break;
	}

	//ƽ�ƺ�ʣ��ͼ����Դͼ���е�λ�ã���������
	CRect	rectSrc;
	
	//ƽ�ƺ�ʣ��ͼ������ͼ���е�λ�ã���������
	CRect	rectDst;

	//ָ��ͼ���Ƿ�ȫ����ȥ��������
	BOOL	bVisible;
	//����ֵ
	bVisible = TRUE;

	//����rectSrc��rectDst��X����
	if (lXOffset <= -nWidth)
	{
		//X�᷽��ȫ���Ƴ���������
		bVisible = FALSE;
	}
	else if (lXOffset <= 0)
	{
		//�ƶ�����ͼ�������Ͻ�X����Ϊ0
		rectDst.left = 0;
		
		//�ƶ�����ͼ�������½�X����Ϊ
		//lWidth - |lXOffset| = lWidth + lXOffset
		rectDst.right = nWidth + lXOffset;
	}
	else if (lXOffset < nWidth)
	{
		//�ƶ�����ͼ�������Ͻ�X����ΪlXOffset
		rectDst.left = lXOffset;
		
		//�ƶ�����ͼ�������½�X����ΪlWidth
		rectDst.right = nWidth;
	}
	else
	{
		//X�᷽��ȫ���Ƴ���������
		bVisible = FALSE;
	}
	
	//ƽ�ƺ�ʣ��ͼ����Դͼ���е�X����
	rectSrc.left = rectDst.left - lXOffset;
	rectSrc.right = rectDst.right - lXOffset;
	
	//����rectSrc��rectDst��Y����
	if (lYOffset <= -nHeight)
	{
		//Y�᷽��ȫ���Ƴ���������
		bVisible = FALSE;
	}
	else if (lYOffset <= 0)
	{
		//�ƶ�����ͼ�������Ͻ�Y����Ϊ0
		rectDst.top = 0;
		
		//�ƶ�����ͼ�������½�Y����Ϊ
		//lHeight - |lYOffset| = lHeight + lYOffset
		rectDst.bottom = nHeight + lYOffset;
	}
	else if (lYOffset < nHeight)
	{
		//�ƶ�����ͼ�������Ͻ�Y����ΪlYOffset
		rectDst.top = lYOffset;
		
		//�ƶ�����ͼ�������½�Y����ΪlHeight
		rectDst.bottom = nHeight;
	}
	else
	{
		//X�᷽��ȫ���Ƴ���������
		bVisible = FALSE;
	}
	
	//ƽ�ƺ�ʣ��ͼ����Դͼ���е�Y����
	rectSrc.top = rectDst.top - lYOffset;
	rectSrc.bottom = rectDst.bottom - lYOffset;

	//����в���ͼ��ɼ�
	if (bVisible)
	{

		//ƽ��ͼ��
		for(i = 0; i < (rectSrc.bottom - rectSrc.top); i++)
		{
			//Ҫ�����������㣬ע������DIBͼ�����������µ��õģ�
			//��һ�������Ǳ��������һ�У���˸����������㲻��
			//pOldBits + nWidthBytes * (i + rectSrc.top) + 
			//rectSrc.left��
			//���� pOldBits + nWidthBytes * (nHeight - i - rectSrc.top 
			//- 1) + rectSrc.left��
			
			//��λ��ͼ��ָ��
			pNewTemp = pNewBits;
			pNewTemp += (nHeight - i - rectDst.top - 1) * nWidthBytes
			            + rectDst.left * nMovedBits;

			//��λԴͼ��ָ��
			pOldTemp = pOldBits;
			pOldTemp += (nHeight - i - rectSrc.top - 1) * nWidthBytes
				        + rectSrc.left * nMovedBits;
			//����ÿһ�У����ΪrectSrc.right - rectSrc.left
			memcpy( pNewTemp, pOldTemp, (rectSrc.right - rectSrc.left) * nMovedBits );
		}
	}
	
	//����ͼ���������ԭͼ��������
	memcpy( pOldBits, pNewBits, nWidthBytes * nHeight );

	::GlobalFree( hNewDibBits );
	::GlobalUnlock( m_pDibObject->GetDib() );

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL Mirror()  
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObject�����е�ͼ����о���������������
//			�˵���֮ǰû��ָ��һ��CDibObject����ָ�룬������ڵ���ʱ��
//			��ָ����
//----------------------------------------------------------------------
//����˵����BOOL  bDirection	����ķ���TRUE��ʾˮƽ����FALSE��ʾ
//								��ֱ����
//			CDibObject *pDibObject, Ĭ��ΪNULL
//----------------------------------------------------------------------
//���أ�BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CGeoTrans::Mirror(BOOL bDirection,
							CDibObject *pDibObject)
{
	//ʹ�ô����CDibObject����
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//���ͼ���Ⱥ͸߶�
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();

	//�������
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits, *pNewBits;
	unsigned char *pOldTemp, *pNewTemp;
	unsigned char *pTemp1, *pTemp2;
	BITMAPFILEHEADER *pOldBFH;
	BITMAPINFOHEADER *pOldBIH;
	RGBQUAD *pOldPalette;
	int nWidthBytes, nNumColors, x, y;

	//ԭͼ��ָ��
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
																m_pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );

	//ԭͼ���ļ�ͷָ��
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//ԭͼ����Ϣͷָ��
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//ԭͼ����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//ԭͼ���ɫ��ָ��
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)
									+sizeof(BITMAPINFOHEADER)];
	//ԭͼ������ָ��
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)
											+sizeof(BITMAPINFOHEADER)
											+nNumColors*sizeof(RGBQUAD)];

	//Ϊ��ͼ�������������ڴ�
	DWORD dwNewSize;
	HGLOBAL hNewDibBits;
	//��ͼ����������С
	dwNewSize = nWidthBytes * nHeight;
	//Ϊ��ͼ������ڴ�
	hNewDibBits = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	if( hNewDibBits == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ��ָ��
	pNewBits = (unsigned char *) ::GlobalLock( hNewDibBits );
	if( pNewBits == NULL )
	{
		::GlobalFree( hNewDibBits );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��255����ɫ�������ͼ��������
	memset(pNewBits, (BYTE)255, nWidthBytes * nHeight );
	
	//��ͼ��λ��ȷ�����ƶ��ֽ���
	int nMovedBits = 1;
	switch( m_pDibObject->GetNumBits() )
	{
	case 8:	
		nMovedBits = 1;
		break;
	case 16:
		nMovedBits = 2;
		break;
	case 24:
		nMovedBits = 3;
		break;
	case 32:
		nMovedBits = 4;
		break;
	}

	//�жϾ���ʽ
	if (bDirection)
	{
		//ˮƽ����
		
		//��ֱ��������
		int nMiddleX = nWidth / 2;
		
		//���ͼ��ÿ�н��в���
		for(y = 0; y < nHeight; y++)
		{
			//ָ��ͼ��ĵ�����y��
			pOldTemp = pOldBits;
			pOldTemp +=  y * nWidthBytes;
			pNewTemp = pNewBits;
			pNewTemp +=  y * nWidthBytes;
			
			//���ÿ��ͼ����벿�ֽ��в���
			for(x = 0; x <= nMiddleX; x++)
			{
				//��Դͼ���x�����ظ��Ƶ���ͼ������x������
				pTemp1 = pOldTemp + x * nMovedBits;
				pTemp2 = pNewTemp + (nWidth -1 - x) * nMovedBits;
				memcpy(pTemp2, pTemp1, nMovedBits);
				//��Դͼ������x�����ظ��Ƶ���ͼ���x������
				pTemp1 = pOldTemp + (nWidth -1 - x) * nMovedBits;
				pTemp2 = pNewTemp + x * nMovedBits;
				memcpy(pTemp2, pTemp1, nMovedBits);
			}
		
		}
	}
	else
	{
		//��ֱ����

		//ˮƽ��������
		int nMiddleY = nHeight / 2;
		//����ϰ�ͼ����в���
		for(y = 0; y <= nMiddleY; y++)
		{
			
			//ָ��Դͼ������y����������ָ��
			pOldTemp = pOldBits;
			pOldTemp +=  y * nWidthBytes;

			//ָ����ͼ���y����������ָ��
			pNewTemp = pNewBits;
			pNewTemp +=  (nHeight - 1 - y) * nWidthBytes;
			
			//��Դͼ������y�����ظ��Ƶ���ͼ���y��
			memcpy(pNewTemp, pOldTemp, nWidthBytes);

			//ָ��Դͼ���y����������ָ��
			pOldTemp = pOldBits;
			pOldTemp +=  (nHeight - 1 - y) * nWidthBytes;

			//ָ����ͼ������y����������ָ��
			pNewTemp = pNewBits;
			pNewTemp +=  y * nWidthBytes;
			
			//��Դͼ���y�����ظ��Ƶ���ͼ������y��
			memcpy(pNewTemp, pOldTemp, nWidthBytes);
		}
	}

	//����ͼ���������ԭͼ��������
	memcpy( pOldBits, pNewBits, nWidthBytes * nHeight );

	::GlobalFree( hNewDibBits );
	::GlobalUnlock( m_pDibObject->GetDib() );


	return TRUE;
}

/**********************************************************************
* �������ƣ�BOOL Transpose()
* �������ܣ��������Դ����CDibObject�����е�ͼ�����ת�ò�����������д�
*			����֮ǰû��ָ��һ��CDibObject����ָ�룬������ڵ���ʱ����ָ����
* ����˵����CDibObject *pDibObject, Ĭ��ΪNULL
* ����ֵ: BOOL  - ת�óɹ�����TRUE�����򷵻�FALSE��
**********************************************************************/
BOOL CGeoTrans::Transpose(CDibObject *pDibObject)
{
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//���ͼ���Ⱥ͸߶�
	int nOldWidth = m_pDibObject->GetWidth();
	int nOldHeight = m_pDibObject->GetHeight();
	//ת�ú�ͼ��Ŀ�Ⱥ͸߶�
	int nNewWidth = nOldHeight;
	int nNewHeight = nOldWidth;

	//�������
	unsigned char *pOldBuffer, *pNewBuffer; 
	unsigned char *pOldBits, *pNewBits;
	unsigned char *pOldTemp, *pNewTemp;
	BITMAPFILEHEADER *pOldBFH, *pNewBFH;
	BITMAPINFOHEADER *pOldBIH, *pNewBIH;
	RGBQUAD *pOldPalette, *pNewPalette;
	int nOldWidthBytes, nNewWidthBytes, nNumColors, x, y;

	//ԭͼ��ָ��
	pOldBuffer = (unsigned char *) 
				m_pDibObject->GetDIBPointer( &nOldWidthBytes, 
				m_pDibObject->GetNumBits(), 
				&nNewWidthBytes, nNewWidth );
	if( pOldBuffer == NULL ) return( FALSE );

	//ԭͼ���ļ�ͷָ��
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//ԭͼ����Ϣͷָ��
	pOldBIH = (BITMAPINFOHEADER *) 
			&pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//ԭͼ����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//ԭͼ���ɫ��ָ��
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)
									+sizeof(BITMAPINFOHEADER)];
	//ԭͼ������ָ��
	pOldBits = (unsigned char *)
				&pOldBuffer[sizeof(BITMAPFILEHEADER)
				+sizeof(BITMAPINFOHEADER)
				+nNumColors*sizeof(RGBQUAD)];

	DWORD dwNewSize;
	HGLOBAL hNewDib;

	//��ͼ���С�������ļ�ͷ����Ϣͷ����ɫ���ͼ�����ݣ�
	dwNewSize = (DWORD) nNumColors * sizeof( RGBQUAD ) +
						sizeof( BITMAPFILEHEADER ) + 
						sizeof( BITMAPINFOHEADER ) + 
						nNewWidthBytes * nNewHeight;
	//Ϊ��ͼ������ڴ�
	hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	if( hNewDib == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ��ָ��
	pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	if( pNewBuffer == NULL )
	{
		::GlobalFree( hNewDib );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ���ļ�ͷָ��
	pNewBFH = (BITMAPFILEHEADER *) pNewBuffer;
	//��ͼ����Ϣͷָ��
	pNewBIH = (BITMAPINFOHEADER *) &pNewBuffer[sizeof(BITMAPFILEHEADER)];
	//��ͼ���ɫ��ָ��
	pNewPalette = (RGBQUAD *) &pNewBuffer[sizeof(BITMAPFILEHEADER)
							+sizeof(BITMAPINFOHEADER)];
	//��ͼ������ָ��
	pNewBits = (unsigned char *) &pNewBuffer[sizeof(BITMAPFILEHEADER)
									+sizeof(BITMAPINFOHEADER)
									+nNumColors*sizeof(RGBQUAD)];

	//��255����ɫ�������ͼ��������
	memset(pNewBits, (BYTE)255, nNewWidthBytes * nNewHeight );

	//��ԭͼ���ļ�ͷ���������ͼ���ļ�ͷ
	memcpy( pNewBFH, pOldBFH, sizeof(BITMAPFILEHEADER) );

	//��ԭͼ����Ϣͷ���������ͼ����Ϣͷ
	memcpy( pNewBIH, pOldBIH, sizeof(BITMAPINFOHEADER) );

	//�����ļ����ֽ���
	pNewBFH->bfSize = dwNewSize;

	//������ͼ��Ŀ�Ⱥ͸߶�
	pNewBIH->biWidth = nNewWidth;
	pNewBIH->biHeight = nNewHeight;

	//pNewBIH->biBitCount = nNewBitsPerPixel;
	pNewBIH->biSizeImage = nNewWidthBytes * nNewHeight;
	pNewBIH->biClrUsed = nNumColors;
	pNewBFH->bfSize = sizeof( BITMAPFILEHEADER ) 
					+ sizeof( BITMAPINFOHEADER ) 
					+ nNumColors * sizeof( RGBQUAD ) 
					+ pNewBIH->biSizeImage;
	pNewBFH->bfOffBits = sizeof( BITMAPFILEHEADER ) 
						+ sizeof( BITMAPINFOHEADER ) 
						+ nNumColors * sizeof( RGBQUAD );

	//��ԭͼ���ɫ�������ͼ���ɫ��
	if(nNumColors != 0)
		memcpy( pNewPalette, pOldPalette, nNumColors*sizeof(RGBQUAD) );

	//��ͼ��λ��ȷ�����ƶ��ֽ���
	int nMovedBits = 1;
	switch( m_pDibObject->GetNumBits() )
	{
	case 8:	
		nMovedBits = 1;
		break;
	case 16:
		nMovedBits = 2;
		break;
	case 24:
		nMovedBits = 3;
		break;
	case 32:
		nMovedBits = 4;
		break;
	}

	// ���ͼ��ÿ�н��в���
	for(y = 0; y < nOldHeight; y++)
	{
		//ָ��Դͼ���y��
		pOldTemp = pOldBits;
		pOldTemp += (nOldHeight - 1 -y) * nOldWidthBytes;
		// ���ÿ��ͼ��ÿ�н��в���
		for(x = 0; x < nOldWidth; x++)
		{
			//ָ��ת�ú��ͼ���x�У���y�����ص�ָ��
			//ע��˴���Ⱥ͸߶�Ӧ�û���
			pNewTemp = pNewBits;
			pNewTemp += (nNewHeight - 1 - x) * nNewWidthBytes;
			pNewTemp += y * nMovedBits;
			
			//��������
			memcpy(pNewTemp, pOldTemp, nMovedBits);
			pOldTemp += nMovedBits;			
		}
	}

	//���ڴ�����ͽ�����ʹ�õ��ڴ��ͷ�
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalFree( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	//����ͼ������Ϊ��ǰͼ��
	m_pDibObject->SetDib( hNewDib );

	m_pDibObject->ProcessImageHeader();

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL ZoomImage()  
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObject�����е�ͼ��������Ų������������
//			�˵���֮ǰû��ָ��һ��CDibObject����ָ�룬������ڵ���ʱ��
//			��ָ����
//----------------------------------------------------------------------
//����˵����float fXZoomRatio	- X�᷽�����ű���
//			float fYZoomRatio	- Y�᷽�����ű���
//			BOOL bBilinear		- TRUEΪ˫���Բ�ֵ��FALSEΪ���ڽ���ֵ
//			CDibObject *pDibObject, Ĭ��ΪNULL
//----------------------------------------------------------------------
//���أ�BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CGeoTrans::ZoomImage(float fXZoomRatio, 
							   float fYZoomRatio,
							   BOOL bBilinear,
							   CDibObject *pDibObject)
{
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//���ͼ���Ⱥ͸߶�
	int nOldWidth = m_pDibObject->GetWidth();
	int nOldHeight = m_pDibObject->GetHeight();

	// �������ź��ͼ��ʵ�ʿ��
	int nNewWidth, nNewHeight;
	// �˴�ֱ�Ӽ�0.5������ǿ������ת��ʱ���������룬����ֱ�ӽ�ȥС������
	nNewWidth = (int)(nOldWidth * fXZoomRatio + 0.5);
	
	// �������ź��ͼ��߶�
	nNewHeight = (int)(nOldHeight * fYZoomRatio + 0.5);

	//�������
	unsigned char *pOldBuffer, *pNewBuffer; 
	unsigned char *pOldBits, *pNewBits;
	unsigned char *pOldTemp, *pNewTemp;
	BITMAPFILEHEADER *pOldBFH, *pNewBFH;
	BITMAPINFOHEADER *pOldBIH, *pNewBIH;
	RGBQUAD *pOldPalette, *pNewPalette;
	int nOldWidthBytes, nNewWidthBytes, nNumColors, x, y;

	//ԭͼ��ָ��
	pOldBuffer = (unsigned char *) 
				m_pDibObject->GetDIBPointer( &nOldWidthBytes, 
				m_pDibObject->GetNumBits(), 
				&nNewWidthBytes, nNewWidth );
	if( pOldBuffer == NULL ) return( FALSE );

	//ԭͼ���ļ�ͷָ��
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//ԭͼ����Ϣͷָ��
	pOldBIH = (BITMAPINFOHEADER *) 
			&pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//ԭͼ����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//ԭͼ���ɫ��ָ��
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)
									+sizeof(BITMAPINFOHEADER)];
	//ԭͼ������ָ��
	pOldBits = (unsigned char *)
				&pOldBuffer[sizeof(BITMAPFILEHEADER)
				+sizeof(BITMAPINFOHEADER)
				+nNumColors*sizeof(RGBQUAD)];

	DWORD dwNewSize;
	HGLOBAL hNewDib;

	//��ͼ���С�������ļ�ͷ����Ϣͷ����ɫ���ͼ�����ݣ�
	dwNewSize = (DWORD) nNumColors * sizeof( RGBQUAD ) +
						sizeof( BITMAPFILEHEADER ) + 
						sizeof( BITMAPINFOHEADER ) + 
						nNewWidthBytes * nNewHeight;
	//Ϊ��ͼ������ڴ�
	hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	if( hNewDib == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ��ָ��
	pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	if( pNewBuffer == NULL )
	{
		::GlobalFree( hNewDib );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ���ļ�ͷָ��
	pNewBFH = (BITMAPFILEHEADER *) pNewBuffer;
	//��ͼ����Ϣͷָ��
	pNewBIH = (BITMAPINFOHEADER *) &pNewBuffer[sizeof(BITMAPFILEHEADER)];
	//��ͼ���ɫ��ָ��
	pNewPalette = (RGBQUAD *) &pNewBuffer[sizeof(BITMAPFILEHEADER)
							+sizeof(BITMAPINFOHEADER)];
	//��ͼ������ָ��
	pNewBits = (unsigned char *) &pNewBuffer[sizeof(BITMAPFILEHEADER)
									+sizeof(BITMAPINFOHEADER)
									+nNumColors*sizeof(RGBQUAD)];

	//��255����ɫ�������ͼ��������
	memset(pNewBits, (BYTE)255, nNewWidthBytes * nNewHeight );

	//��ԭͼ���ļ�ͷ���������ͼ���ļ�ͷ
	memcpy( pNewBFH, pOldBFH, sizeof(BITMAPFILEHEADER) );

	//��ԭͼ����Ϣͷ���������ͼ����Ϣͷ
	memcpy( pNewBIH, pOldBIH, sizeof(BITMAPINFOHEADER) );

	//�����ļ����ֽ���
	pNewBFH->bfSize = dwNewSize;

	//������ͼ��Ŀ�Ⱥ͸߶�
	pNewBIH->biWidth = nNewWidth;
	pNewBIH->biHeight = nNewHeight;

	//pNewBIH->biBitCount = nNewBitsPerPixel;
	pNewBIH->biSizeImage = nNewWidthBytes * nNewHeight;
	pNewBIH->biClrUsed = nNumColors;
	pNewBFH->bfSize = sizeof( BITMAPFILEHEADER ) 
					+ sizeof( BITMAPINFOHEADER ) 
					+ nNumColors * sizeof( RGBQUAD ) 
					+ pNewBIH->biSizeImage;
	pNewBFH->bfOffBits = sizeof( BITMAPFILEHEADER ) 
						+ sizeof( BITMAPINFOHEADER ) 
						+ nNumColors * sizeof( RGBQUAD );

	//��ԭͼ���ɫ�������ͼ���ɫ��
	if(nNumColors != 0)
		memcpy( pNewPalette, pOldPalette, nNumColors*sizeof(RGBQUAD) );

	//��ͼ��λ��ȷ�����ƶ��ֽ���
	int nMovedBits = 1;
	switch( m_pDibObject->GetNumBits() )
	{
	case 8:	
		nMovedBits = 1;
		break;
	case 16:
		nMovedBits = 2;
		break;
	case 24:
		nMovedBits = 3;
		break;
	case 32:
		nMovedBits = 4;
		break;
	}

	//���ͼ��ÿ�н��в���
	for(y = 0; y < nNewHeight; y++)
	{
		//ָ����ͼ���y��
		//ע��˴���Ⱥ͸߶�����ͼ��Ŀ�Ⱥ͸߶�
		pNewTemp = pNewBits;
		pNewTemp += (nNewHeight - 1 - y) * nNewWidthBytes;
		//���ͼ��ÿ�н��в���
		for(x = 0; x < nNewWidth; x++)
		{
			//�����������Դͼ���е�����
			int y0 = (long) (y / fYZoomRatio + 0.5);
			int x0 = (long) (x / fXZoomRatio + 0.5);

			//�ж��Ƿ���Դͼ��Χ��
			if( (x0 >= 0) && (x0 < nOldWidth) && 
				(y0 >= 0) && (y0 < nOldHeight))
			{
				//��˫���Բ�ֵ
				if(bBilinear)
				{
					unsigned char *pTemp = Interpolation (nOldWidth, nOldHeight, 
						(float)x0, (float)y0, 
						nOldWidthBytes, nMovedBits, pOldBits);
					//��������
					memcpy(pNewTemp, pTemp, nMovedBits);
					
					delete [] pTemp ;
				}
				else
				{				
					//ָ��Դͼ���y0�У���x0������
					//ע��˴���Ⱥ͸߶�Ӧ�û���
					pOldTemp = pOldBits;
					pOldTemp += (nOldHeight - 1 - y0) * nOldWidthBytes;
					pOldTemp += x0 * nMovedBits;
					
					//��������
					memcpy(pNewTemp, pOldTemp, nMovedBits);
				}
			}
			pNewTemp += nMovedBits;

		}
		
	}

	//���ڴ�����ͽ�����ʹ�õ��ڴ��ͷ�
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalFree( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	//����ͼ������Ϊ��ǰͼ��
	m_pDibObject->SetDib( hNewDib );

	m_pDibObject->ProcessImageHeader();

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL Rotate()  
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObject�����е�ͼ�������ת�������������
//			�˵���֮ǰû��ָ��һ��CDibObject����ָ�룬������ڵ���ʱ��
//			��ָ����
//----------------------------------------------------------------------
//����˵����int iRotateAngle	- ��ת�ĽǶȣ�0-360�ȣ�
//			BOOL bBilinear		- TRUEΪ˫���Բ�ֵ��FALSEΪ���ڽ���ֵ
//			CDibObject *pDibObject, Ĭ��ΪNULL
//----------------------------------------------------------------------
//���أ�BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
//ע	�⣺�ú���������ͼ������Ϊ������תͼ�񣬵��øú������Զ�����ͼ��
//			����ʾ���е����ء������в������ڽ���ֵ�㷨���в�ֵ��
////////////////////////////////////////////////////////////////////////
BOOL CGeoTrans::Rotate(int nRotateAngle,
							BOOL bBilinear,
							CDibObject *pDibObject)
{
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//���ͼ���Ⱥ͸߶�
	int nOldWidth = m_pDibObject->GetWidth();
	int nOldHeight = m_pDibObject->GetHeight();

	//��ת�Ƕȣ����ȣ�
	float	fRotateAngle;
	
	//��ת�Ƕȵ����Һ�����ֵ
	float	fSina, fCosa;
	
	//Դͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float	fSrcX1, fSrcY1;
	float	fSrcX2, fSrcY2;
	float	fSrcX3, fSrcY3;
	float	fSrcX4, fSrcY4;
	
	//��ת���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float	fDstX1, fDstY1;
	float	fDstX2, fDstY2;
	float	fDstX3, fDstY3;
	float	fDstX4, fDstY4;
	
	//�����м䳣��
	float	f1,f2;

	//����ת�ǶȴӶ�ת��������
	fRotateAngle = (float) RADIAN(nRotateAngle);

	//������ת�Ƕȵ�����
	fSina = (float) sin((double)fRotateAngle);
	
	//������ת�Ƕȵ�����
	fCosa = (float) cos((double)fRotateAngle);
	
	//����ԭͼ���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	fSrcX1 = (float) (- (nOldWidth  - 1) / 2);
	fSrcY1 = (float) (  (nOldHeight - 1) / 2);
	fSrcX2 = (float) (  (nOldWidth  - 1) / 2);
	fSrcY2 = (float) (  (nOldHeight - 1) / 2);
	fSrcX3 = (float) (- (nOldWidth  - 1) / 2);
	fSrcY3 = (float) (- (nOldHeight - 1) / 2);
	fSrcX4 = (float) (  (nOldWidth  - 1) / 2);
	fSrcY4 = (float) (- (nOldHeight - 1) / 2);
	
	//������ͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;
	
	//������ת���ͼ��ʵ�ʿ��
	int nNewWidth  = (long) ( max( fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2) ) + 0.5);
	
	//������ת���ͼ��߶�
	int nNewHeight = (long) ( max( fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2) )  + 0.5);
	
	//��������
	f1 = (float) (-0.5 * (nNewWidth - 1) * fCosa - 0.5 * (nNewHeight - 1) * fSina
		+ 0.5 * (nOldWidth  - 1));
	f2 = (float) ( 0.5 * (nNewWidth - 1) * fSina - 0.5 * (nNewHeight - 1) * fCosa
		+ 0.5 * (nOldHeight - 1));

	//�������
	unsigned char *pOldBuffer, *pNewBuffer; 
	unsigned char *pOldBits, *pNewBits;
	unsigned char *pOldTemp, *pNewTemp;
	BITMAPFILEHEADER *pOldBFH, *pNewBFH;
	BITMAPINFOHEADER *pOldBIH, *pNewBIH;
	RGBQUAD *pOldPalette, *pNewPalette;
	int nOldWidthBytes, nNewWidthBytes, nNumColors, x, y;

	//ԭͼ��ָ��
	pOldBuffer = (unsigned char *) 
				m_pDibObject->GetDIBPointer( &nOldWidthBytes, 
				m_pDibObject->GetNumBits(), 
				&nNewWidthBytes, nNewWidth );
	if( pOldBuffer == NULL ) return( FALSE );

	//ԭͼ���ļ�ͷָ��
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//ԭͼ����Ϣͷָ��
	pOldBIH = (BITMAPINFOHEADER *) 
			&pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//ԭͼ����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//ԭͼ���ɫ��ָ��
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)
									+sizeof(BITMAPINFOHEADER)];
	//ԭͼ������ָ��
	pOldBits = (unsigned char *)
				&pOldBuffer[sizeof(BITMAPFILEHEADER)
				+sizeof(BITMAPINFOHEADER)
				+nNumColors*sizeof(RGBQUAD)];

	DWORD dwNewSize;
	HGLOBAL hNewDib;

	//��ͼ���С�������ļ�ͷ����Ϣͷ����ɫ���ͼ�����ݣ�
	dwNewSize = (DWORD) nNumColors * sizeof( RGBQUAD ) +
						sizeof( BITMAPFILEHEADER ) + 
						sizeof( BITMAPINFOHEADER ) + 
						nNewWidthBytes * nNewHeight;
	//Ϊ��ͼ������ڴ�
	hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	if( hNewDib == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ��ָ��
	pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	if( pNewBuffer == NULL )
	{
		::GlobalFree( hNewDib );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ���ļ�ͷָ��
	pNewBFH = (BITMAPFILEHEADER *) pNewBuffer;
	//��ͼ����Ϣͷָ��
	pNewBIH = (BITMAPINFOHEADER *) &pNewBuffer[sizeof(BITMAPFILEHEADER)];
	//��ͼ���ɫ��ָ��
	pNewPalette = (RGBQUAD *) &pNewBuffer[sizeof(BITMAPFILEHEADER)
							+sizeof(BITMAPINFOHEADER)];
	//��ͼ������ָ��
	pNewBits = (unsigned char *) &pNewBuffer[sizeof(BITMAPFILEHEADER)
									+sizeof(BITMAPINFOHEADER)
									+nNumColors*sizeof(RGBQUAD)];

	//��255����ɫ�������ͼ��������
	memset(pNewBits, (BYTE)255, nNewWidthBytes * nNewHeight );

	//��ԭͼ���ļ�ͷ���������ͼ���ļ�ͷ
	memcpy( pNewBFH, pOldBFH, sizeof(BITMAPFILEHEADER) );

	//��ԭͼ����Ϣͷ���������ͼ����Ϣͷ
	memcpy( pNewBIH, pOldBIH, sizeof(BITMAPINFOHEADER) );

	//�����ļ����ֽ���
	pNewBFH->bfSize = dwNewSize;

	//������ͼ��Ŀ�Ⱥ͸߶�
	pNewBIH->biWidth = nNewWidth;
	pNewBIH->biHeight = nNewHeight;

	//pNewBIH->biBitCount = nNewBitsPerPixel;
	pNewBIH->biSizeImage = nNewWidthBytes * nNewHeight;
	pNewBIH->biClrUsed = nNumColors;
	pNewBFH->bfSize = sizeof( BITMAPFILEHEADER ) 
					+ sizeof( BITMAPINFOHEADER ) 
					+ nNumColors * sizeof( RGBQUAD ) 
					+ pNewBIH->biSizeImage;
	pNewBFH->bfOffBits = sizeof( BITMAPFILEHEADER ) 
						+ sizeof( BITMAPINFOHEADER ) 
						+ nNumColors * sizeof( RGBQUAD );

	//��ԭͼ���ɫ�������ͼ���ɫ��
	if(nNumColors != 0)
		memcpy( pNewPalette, pOldPalette, nNumColors*sizeof(RGBQUAD) );

	//��ͼ��λ��ȷ�����ƶ��ֽ���
	int nMovedBits = 1;
	switch( m_pDibObject->GetNumBits() )
	{
	case 8:	
		nMovedBits = 1;
		break;
	case 16:
		nMovedBits = 2;
		break;
	case 24:
		nMovedBits = 3;
		break;
	case 32:
		nMovedBits = 4;
		break;
	}

	//���ͼ��ÿ�н��в���
	for(y = 0; y < nNewHeight; y++)
	{
		//ָ����ͼ���y��
		//ע��˴���Ⱥ͸߶�����ͼ��Ŀ�Ⱥ͸߶�
		pNewTemp = pNewBits;
		pNewTemp += (nNewHeight - 1 - y) * nNewWidthBytes;
		//���ͼ��ÿ�н��в���
		for(x = 0; x < nNewWidth; x++)
		{
			//�����������Դͼ���е�����
			int y0 = (long) (-((float) x) * fSina + ((float) y)
					* fCosa + f2 + 0.5);
			int x0 = (long) ( ((float) x) * fCosa + ((float) y)
					* fSina + f1 + 0.5);

			//�ж��Ƿ���Դͼ��Χ��
			if( (x0 >= 0) && (x0 < nOldWidth) && 
				(y0 >= 0) && (y0 < nOldHeight))
			{
				//��˫���Բ�ֵ
				if(bBilinear)
				{
					unsigned char *pTemp = Interpolation (nOldWidth, nOldHeight, 
						(float)x0, (float)y0, 
						nOldWidthBytes, nMovedBits, pOldBits);
					//��������
					memcpy(pNewTemp, pTemp, nMovedBits);
					
					delete [] pTemp ;
				}
				else
				{				
					//ָ��Դͼ���y0�У���x0������
					//ע��˴���Ⱥ͸߶�Ӧ�û���
					pOldTemp = pOldBits;
					pOldTemp += (nOldHeight - 1 - y0) * nOldWidthBytes;
					pOldTemp += x0 * nMovedBits;
					
					//��������
					memcpy(pNewTemp, pOldTemp, nMovedBits);
				}
			}
			pNewTemp += nMovedBits;
		}
		
	}

	//���ڴ�����ͽ�����ʹ�õ��ڴ��ͷ�
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalFree( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	//����ͼ������Ϊ��ǰͼ��
	m_pDibObject->SetDib( hNewDib );

	m_pDibObject->ProcessImageHeader();

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL Interpolation()  
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObject�����е�ͼ�������ת�������������
//			�˵���֮ǰû��ָ��һ��CDibObject����ָ�룬������ڵ���ʱ��
//			��ָ����
//----------------------------------------------------------------------
//����˵����int nWidth		- Դͼ���ȣ���������
//			int nHeight		- Դͼ��߶ȣ���������
//			float x			- ��ֵԪ�ص�x����
//			float y			- ��ֵԪ�ص�y����
//			int nWidthBytes	- ͼ���ֽڿ��
//			int nMovedBits	- һ���ƶ����ֽ���
//			LPSTR lpDIBBits	- ָ��ԴDIBͼ��ָ��
//----------------------------------------------------------------------
//��	�أ�unsigned char	- ���ز�ֵ��������
//----------------------------------------------------------------------
//ע	�⣺�ú�������˫���Բ�ֵ�㷨����������ֵ�����ڳ���ͼ��Χ�����أ�
//			ֱ�ӷ���255��
////////////////////////////////////////////////////////////////////////
unsigned char *CGeoTrans::Interpolation(int nWidth, 
											int nHeight, 
											float x, 
											float y,
											int nWidthBytes,
											int nMovedBits,
											unsigned char *pBits)
{
	//�ĸ����ٽ����ص�����(x1, y1), (x2, y1), (x1, y2), (x2, y2)
	long	x1, x2;
	long	y1, y2;
	
	//�ĸ����ٽ�����ֵ
	unsigned char	f1, f2, f3, f4;
	
	//������ֵ�м�ֵ
	unsigned char	f12, f34;
	
	//����һ��ֵ���������������С�ڸ�ֵʱ��Ϊ������ͬ
	float	EXP;
	
	//ֱ��ͼ��������
	unsigned char *pData = new unsigned char [nMovedBits];
	int i;

	//��ֵ
	EXP = (float) 0.0001;
	
	//�����ĸ����ٽ����ص�����
	x1 = (long) x;
	x2 = x1 + 1;
	y1 = (long) y;
	y2 = y1 + 1;

	//���ݲ�ͬ����ֱ���
	if( (x < 0) || (x > nWidth - 1) || (y < 0) || (y > nHeight - 1))
	{
		for(i = 0; i < nMovedBits; i++)
		{
			pData[i] =(unsigned char) 255;
		}
		//Ҫ����ĵ㲻��Դͼ��Χ�ڣ�ֱ�ӷ���255��
		return(pData);
	}
	else
	{
		if (fabs(x - nWidth + 1) <= EXP)
		{
			//Ҫ����ĵ���ͼ���ұ�Ե��
			if (fabs(y - nHeight + 1) <= EXP)
			{
				//Ҫ����ĵ�������ͼ�������½���һ�����أ�ֱ�ӷ��ظõ�����ֵ
				for(i = 0; i < nMovedBits; i++)
				{
					pData[i] = *(pBits + nWidthBytes * (nHeight - 1 - y1) + x1 * nMovedBits + i);
				}

				//���ز�ֵ���
				return(pData);
			}
			else
			{
				//��ͼ���ұ�Ե���Ҳ������һ�㣬ֱ��һ�β�ֵ����
				for(i = 0; i < nMovedBits; i++)
				{
					f1 = *(pBits + nWidthBytes * (nHeight - 1 - y1) + x1 * nMovedBits + i);
					f3 = *(pBits + nWidthBytes * (nHeight - 1 - y1) + x2 * nMovedBits + i);
					pData[i] =(unsigned char)(f1 + (y -y1) * (f3 - f1));
				}

				//���ز�ֵ���
				return(pData);
			}
		}
		else if (fabs(y - nHeight + 1) <= EXP)
		{
			//Ҫ����ĵ���ͼ���±�Ե���Ҳ������һ�㣬ֱ��һ�β�ֵ����
			for(i = 0; i < nMovedBits; i++)
			{
				f1 = *(pBits + nWidthBytes * (nHeight - 1 - y1) + x1 * nMovedBits + i);
				f2 = *(pBits + nWidthBytes * (nHeight - 1 - y2) + x1 * nMovedBits + i);
				pData[i] =(unsigned char)(f1 + (x -x1) * (f2 - f1));
			}
			
			//���ز�ֵ���
			return(pData);
		}
		else
		{
			//�����ĸ����ٽ�����ֵ
			for(i = 0; i < nMovedBits; i++)
			{
				// �����ĸ����ٽ�����ֵ
				f1 = *(pBits + nWidthBytes * (nHeight - 1 - y1) + x1 * nMovedBits + i);
				f2 = *(pBits + nWidthBytes * (nHeight - 1 - y2) + x1 * nMovedBits + i);
				f3 = *(pBits + nWidthBytes * (nHeight - 1 - y1) + x2 * nMovedBits + i);
				f4 = *(pBits + nWidthBytes * (nHeight - 1 - y2) + x2 * nMovedBits + i);

				//��ֵ1
				f12 = (unsigned char)(f1 + (x - x1) * (f2 - f1));
				
				//��ֵ2
				f34 = (unsigned char)(f3 + (x - x1) * (f4 - f3));

				pData[i] = (unsigned char)(f12 + (y -y1) * (f34 - f12));
				//char s[200];
				//wsprintf(s,"pData[%d]=%d", i, pData[i]);
				//AfxMessageBox(s);
			}
			
			//���ز�ֵ���
			return(pData);
		}
	}
}