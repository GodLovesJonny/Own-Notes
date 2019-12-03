// MorphProc.cpp: implementation of the CMorphPro class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MorphProc.h"

IMPLEMENT_DYNCREATE(CMorphPro, CObject)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//Diagnostics and dump member functions, overridden
#ifdef _DEBUG
void CMorphPro::Dump(CDumpContext &dc) const
{
	//call base class function first
	CObject::Dump(dc);
}
#endif

#ifdef _DEBUG
void CMorphPro::AssertValid() const
{
	//call inherited AssertValid first
	CObject::AssertValid();
	//Check CDibObject members...
	ASSERT(m_pDibObject != NULL);	//Must exist
}
#endif

/************************************************************************
*                                                                       *
*                           ��ѧ��̬ѧ������                            *
*                                                                       *
*ע�⣺����ѧ��̬ѧ���У�Ϊ������Ҫ���CDibObjectͼ����Ϊ256ɫ�Ҷ�ͼ*
*	   �񲻹��ڴ�����ֻ�õ����ɫ���е�0��255����ֵ��                   *
************************************************************************/

////////////////////////////////////////////////////////////////////////
//���캯��CMorphPro()    
//----------------------------------------------------------------------
//�������ܣ�����һ��CMorphPro��Ķ����粻����CDibObject���󡣵�һ�ε�   
//			��ĳһ��������ʱ�������һ��CDibObject����ָ�롣
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//��    �أ���
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CMorphPro::CMorphPro()
{

}

////////////////////////////////////////////////////////////////////////
//���캯��CMorphPro()    
//----------------------------------------------------------------------
//�������ܣ�����һ��CMorphPro��Ķ��󲢴���CDibObject�������еĲ�����
//			��Ըö���ֱ����һ��������Ϊ����������ͼ��������
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject
//----------------------------------------------------------------------
//��    �أ���
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CMorphPro::CMorphPro(CDibObject *pDibObject)
{
	m_pDibObject = pDibObject;
}

//��������
CMorphPro::~CMorphPro()
{

}

////////////////////////////////////////////////////////////////////////
//void SetDibObjectClass(CDibObject *pDibObject)   
//----------------------------------------------------------------------
//�������ܣ�������ΪCMorphPro�����ָ��һ��CDibObject����ָ��
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject, Ĭ��ΪNULL��
//----------------------------------------------------------------------
//��    �أ��ޡ�
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CMorphPro::SetDibObjectClass(CDibObject *pDibObject)
{
	m_pDibObject = pDibObject;
}

/************************************************************************
*                                                                       *
*            ���º���������������ࡢ�롢�����Ȳ���                 *
*	                                                                    *
************************************************************************/

////////////////////////////////////////////////////////////////////////
//BOOL IsResultBlank()   
//----------------------------------------------------------------------
//�������ܣ��������ж�ͼ�������Ƿ�ȫΪ��
//----------------------------------------------------------------------
//����˵����unsigned char *pIn		ָ��ͼ�����ݵ�ָ��
//			LONG lSize				ͼ�����ݵĴ�С��nWidthBytes*nHeight��
//----------------------------------------------------------------------
//��    �أ�BOOL
//			��Ϊ��ʱ����TRUE��Ϊ��ʱ����FALSE��
//----------------------------------------------------------------------
//ע    �⣺�˺�������Ϊ�����ͣ�ֻ����CMorphPro����ʹ�á�
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::IsResultBlank(unsigned char *pIn, LONG lSize)
{
	BOOL bMark = TRUE;
	LONG i;
	for(i = 0; i < lSize; i++)
	{
		if(*(pIn + i) != 0x00)
		{
			i = lSize;
			bMark = FALSE;
		}
	}
	return(bMark);
}

////////////////////////////////////////////////////////////////////////
//void ReverseBuffer()   
//----------------------------------------------------------------------
//�������ܣ��������ж�ͼ�����ݽ��з������
//----------------------------------------------------------------------
//����˵����unsigned char *pIn		ָ��ͼ�����ݵ�ָ��
//			LONG lSize				ͼ�����ݵĴ�С��nWidthBytes*nHeight��
//----------------------------------------------------------------------
//��    �أ���
//----------------------------------------------------------------------
//ע    �⣺�˺�������Ϊ�����ͣ�ֻ����CMorphPro����ʹ�á�
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CMorphPro::ReverseBuffer(unsigned char *pIn, LONG lSize)
{
	LONG i;
	BYTE  Buffer;
	for(i = 0;i < lSize; i++)
	{
		Buffer = *(pIn + i);
		//���з�������
		*(pIn + i) = (BYTE)(255 - Buffer);
	}
}

////////////////////////////////////////////////////////////////////////
//void AndBuffer()   
//----------------------------------------------------------------------
//�������ܣ��������Դ��������ͼ�����ݽ��������
//----------------------------------------------------------------------
//����˵����unsigned char *pOut		ָ�����ͼ�����ݵ�ָ��
//			unsigned char *pIn		ָ������ͼ�����ݵ�ָ��
//			LONG lSize				ͼ�����ݵĴ�С��nWidthBytes*nHeight��
//----------------------------------------------------------------------
//��    �أ���
//----------------------------------------------------------------------
//ע    �⣺�˺�������Ϊ�����ͣ�ֻ����CMorphPro����ʹ�á�
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CMorphPro::AndBuffer(unsigned char *pOut, unsigned char *pIn,
						   LONG lSize)
{
	LONG i;
	BYTE Buffer;
	for(i = 0; i < lSize; i++)
	{
		Buffer = *(pIn + i);
		//ִ�������
		*(pOut + i) &= Buffer;
	}
}

////////////////////////////////////////////////////////////////////////
//void OrBuffer()   
//----------------------------------------------------------------------
//�������ܣ��������Դ��������ͼ�����ݽ��л����
//----------------------------------------------------------------------
//����˵����unsigned char *pOut		ָ�����ͼ�����ݵ�ָ��
//			unsigned char *pIn		ָ������ͼ�����ݵ�ָ��
//			LONG lSize				ͼ�����ݵĴ�С��nWidthBytes*nHeight��
//----------------------------------------------------------------------
//��    �أ���
//----------------------------------------------------------------------
//ע    �⣺�˺�������Ϊ�����ͣ�ֻ����CMorphPro����ʹ�á�
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CMorphPro::OrBuffer(unsigned char *pOut, unsigned char *pIn, 
						  LONG lSize)
{
	LONG i;
	BYTE Buffer;
	for(i = 0; i < lSize; i++)
	{
		Buffer = *(pIn + i);
		//ִ�л����
		*(pOut + i) |= Buffer;
	}
}

////////////////////////////////////////////////////////////////////////
//void XorBuffer()   
//----------------------------------------------------------------------
//�������ܣ��������Դ��������ͼ�����ݽ���������
//----------------------------------------------------------------------
//����˵����unsigned char *pOut		ָ�����ͼ�����ݵ�ָ��
//			unsigned char *pIn		ָ������ͼ�����ݵ�ָ��
//			LONG lSize				ͼ�����ݵĴ�С��nWidthBytes*nHeight��
//----------------------------------------------------------------------
//��    �أ���
//----------------------------------------------------------------------
//ע    �⣺�˺�������Ϊ�����ͣ�ֻ����CMorphPro����ʹ�á�
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CMorphPro::XorBuffer(unsigned char *pOut, unsigned char *pIn, 
						   LONG lSize)
{
	LONG i;
	BYTE Buffer;
	for(i = 0; i < lSize; i++)
	{
		Buffer = *(pIn + i);
		//ִ��������
		*(pOut + i) ^= Buffer;
	}
}

////////////////////////////////////////////////////////////////////////
//void CopyBuffer()   
//----------------------------------------------------------------------
//�������ܣ��������Դ��������ͼ�����ݽ��и��Ʋ���
//----------------------------------------------------------------------
//����˵����unsigned char *pOut		ָ�����ͼ�����ݵ�ָ��
//			unsigned char *pIn		ָ������ͼ�����ݵ�ָ��
//			LONG lSize				ͼ�����ݵĴ�С��nWidthBytes*nHeight��
//----------------------------------------------------------------------
//��    �أ���
//----------------------------------------------------------------------
//ע    �⣺�˺�������Ϊ�����ͣ�ֻ����CMorphPro����ʹ�á�
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CMorphPro::CopyBuffer(unsigned char *pOut, unsigned char *pIn,
							LONG lSize)
{
	if(pOut == NULL || pIn == NULL)
	{
		AfxMessageBox("���������緳���ָ��Ϊ�գ�");
		return;
	}
	//��pIn�е��������pOut
	memcpy(pOut, pIn, lSize);
}

////////////////////////////////////////////////////////////////////////
//void MoveBuffer()   
//----------------------------------------------------------------------
//�������ܣ��������Դ��������ͼ�����ݽ����ƶ�����
//----------------------------------------------------------------------
//����˵����unsigned char *pOut		ָ�����ͼ�����ݵ�ָ��
//			unsigned char *pIn		ָ������ͼ�����ݵ�ָ��
//			LONG lSize				ͼ�����ݵĴ�С��nWidthBytes*nHeight��
//----------------------------------------------------------------------
//��    �أ���
//----------------------------------------------------------------------
//ע    �⣺�˺�������Ϊ�����ͣ�ֻ����CMorphPro����ʹ�á�
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CMorphPro::MoveBuffer(unsigned char *pOut, unsigned char *pIn, 
							LONG lSize)
{
	if(pOut == NULL || pIn == NULL)
	{
		AfxMessageBox("���������緳���ָ��Ϊ�գ�");
		return;
	}

	//��pIn�е��������pOut
	memcpy(pOut, pIn, lSize);
	//��pIn�е�������0
	memset(pIn, 0x00, lSize);
}

////////////////////////////////////////////////////////////////////////
//void CleanBuffer()   
//----------------------------------------------------------------------
//�������ܣ��������Դ��������ͼ�����ݽ����������
//----------------------------------------------------------------------
//����˵����unsigned char *pIn		ָ��ͼ�����ݵ�ָ��
//			LONG lSize				ͼ�����ݵĴ�С��nWidthBytes*nHeight��
//----------------------------------------------------------------------
//��    �أ���
//----------------------------------------------------------------------
//ע    �⣺�˺�������Ϊ�����ͣ�ֻ����CMorphPro����ʹ�á�
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CMorphPro::CleanBuffer(unsigned char *pIn, LONG lSize)
{
	if(pIn == NULL)
	{
		AfxMessageBox("���������緳���ָ��Ϊ�գ�");
		return;
	}

	//��pIn�е�������0
	memset(pIn, 0x00, lSize);
}

////////////////////////////////////////////////////////////////////////
//unsigned char GetBitsPoint()   
//----------------------------------------------------------------------
//�������ܣ���������ȡ�����pDibObjectָ����CMorphPro���ͼ��������
//			������ָ�롣
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//��    �أ�unsigned char
//          ����ͼ����������ָ�롣
//----------------------------------------------------------------------
//ע    �⣺�˺�������Ϊ�����ͣ�ֻ����CMorphPro����ʹ�á�
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
unsigned char *CMorphPro::GetBitsPoint()
{
	//ͼ��ָ��Ϊ��
	if(m_pDibObject == NULL) return(NULL);
		
	//�������
	unsigned char *pBuffer, *pBits; 
	int nWidthBytes;
	int nNumColors = m_pDibObject->GetNumColors();

	//ԭͼ��ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
											m_pDibObject->GetNumBits() );
	if( pBuffer == NULL )
	{
		return(NULL);
	}

	//ͼ������ָ��
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+
								sizeof(BITMAPINFOHEADER)+
								nNumColors*sizeof(RGBQUAD)];
	//����ͼ������ָ��
	return( pBits );
}

////////////////////////////////////////////////////////////////////////
//BOOL MakeErosion()   
//----------------------------------------------------------------------
//�������ܣ���������ͼ������ִ�и�ʴ������
//----------------------------------------------------------------------
//����˵����int		*nMask			�ṹԪ������ָ��
//			int		 nMaskLen		�ṹԪ�س��ȣ��Ե���Ϊ������λ��
//			unsigned char *pOut		���ͼ������ָ��
//			unsigned char *pIn		����ͼ������ָ��
//			int		 nWidthBytes	ͼ���ȣ����ֽڱ�ʾ��
//			int		 nWidth			ͼ���ȣ������ر�ʾ��
//			int		 nHeight		ͼ��߶�
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
//ע    �⣺�˺�������Ϊ�����ͣ�ֻ����CMorphPro����ʹ�á�
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::MakeErosion(int *nMask, int nMaskLen, 
 					unsigned char *pOut, unsigned char *pIn,
					int nWidthBytes, int nWidth, int nHeight)
{
	//�������ͼ������Ϊ�գ����޷���ɲ�����ֱ�ӷ��ء�
	if(pOut == NULL || pIn == NULL) return FALSE;
	
	//�������
	int x, y, k;
	unsigned char Mark;

	//ִ�и�ʴ����
	for( y = 0; y < nHeight; y++ )
	{
		unsigned char *pOutTemp = pOut;
		pOutTemp +=  y * nWidthBytes;
		for( x = 0; x < nWidth; x++ )
		{
			Mark = 1;
			for (k = 0; k < nMaskLen; k++)
			{
				//���ܴ���߽�����
				if ((x + nMask[2 * k] >= 0 ) &&
					(x + nMask[2 * k] < nWidth) &&
					(y + nMask[2 * k + 1] >= 0) &&
					(y + nMask[2 * k + 1] < nHeight))
				{
					unsigned char Data;
					//ȡ��ģ����λ�����Ӧ������ֵ
					unsigned char *pTemp = pIn;
					pTemp +=  y * nWidthBytes;
					pTemp += nMask[2 * k + 1] * nWidthBytes;
					Data = pTemp[x + nMask[2 * k]];
					if(Data != 255)
					{ 
						Mark = 0;
						k = nMaskLen;
					}
				}
				else
				{
					Mark = 0;
					k = nMaskLen;
				}
			}
			if (Mark == 1)	pOutTemp[x] = 255;
		}
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL MakeDilation()   
//----------------------------------------------------------------------
//�������ܣ���������ͼ������ִ�����Ͳ�����
//----------------------------------------------------------------------
//����˵����int		*nMask			�ṹԪ������ָ��
//			int		 nMaskLen		�ṹԪ�س��ȣ��Ե���Ϊ������λ��
//			unsigned char *pOut		���ͼ������ָ��
//			unsigned char *pIn		����ͼ������ָ��
//			int		 nWidthBytes	ͼ���ȣ����ֽڱ�ʾ��
//			int		 nWidth			ͼ���ȣ������ر�ʾ��
//			int		 nHeight		ͼ��߶�
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
//ע    �⣺�˺�������Ϊ�����ͣ�ֻ����CMorphPro����ʹ�á�
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::MakeDilation(int *nMask, int nMaskLen, 
 					unsigned char *pOut, unsigned char *pIn,
					int nWidthBytes, int nWidth, int nHeight)
{
	//�������ͼ������Ϊ�գ����޷���ɲ�����ֱ�ӷ��ء�
	if(pOut == NULL || pIn == NULL) return FALSE;
	
	//�������
	int x, y, k;
	unsigned char Mark;

	for( y = 0; y < nHeight; y++ )
	{
		unsigned char *pOutTemp = pOut;
		pOutTemp +=  y * nWidthBytes;
		for( x = 0; x < nWidth; x++ )
		{
			Mark = 0;
			for (k = 0; k < nMaskLen; k++)
			{
				//���ܴ���߽�����
				if ((x + nMask[2 * k] >= 0 ) &&
					(x + nMask[2 * k] < nWidth) &&
					(y + nMask[2 * k + 1] >= 0) &&
					(y + nMask[2 * k + 1] < nHeight))
				{
					unsigned char Data;
					//ȡģ���е�����ֵ
					unsigned char *pTemp = pIn;
					pTemp +=  y * nWidthBytes;
					pTemp += nMask[2 * k + 1] * nWidthBytes;
					Data = pTemp[x + nMask[2 * k]];
					if(Data == 255)
					{ 
						Mark = 1;
						k = nMaskLen;
					}
				}
			}
			if (Mark == 1)	pOutTemp[x] = 255;
		}
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL Erosion()   
//----------------------------------------------------------------------
//�������ܣ���������CDibObject�����е�ͼ����и�ʴ���㡣
//----------------------------------------------------------------------
//����˵����int			 *nMask			�ṹԪ������ָ��
//			int			 nMaskLen		�ṹԪ�س��ȣ��Ե���Ϊ������λ��
//			CDibObject *pDibObject	���ͼ������ָ��
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::Erosion(int *nMask, int nMaskLen,
							CDibObject *pDibObject)
{
	//ʹ�ô����CDibObject����
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );
	
	//����8λ�Ҷ�ͼ��
	int nNumBits = m_pDibObject->GetNumBits(); 
	if( nNumBits != 8 ) return( FALSE );

	//���ͼ���Ⱥ͸߶ȼ��ֽڿ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	int nWidthBytes = m_pDibObject->WidthBytes(8, nWidth);

	//ͼ����������С
	DWORD dwSize = nWidthBytes * nHeight;
	
	//���ͼ��������ָ��
	unsigned char *pOldBuffer = GetBitsPoint();
	
	//Ϊ��ͼ������ڴ�
	HGLOBAL hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwSize );

	if( hNewDib == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ������ָ��
	unsigned char *pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	
    if (pNewBuffer == NULL || pOldBuffer == NULL)	
	{
		AfxMessageBox("����緳�����λ����");
		return (FALSE);
	}

	//��ԭͼ�������ƶ�����ͼ���У�ԭͼ���������㣩
	MoveBuffer(pNewBuffer, pOldBuffer, (LONG)dwSize);

	//����MakeErosion()�����ͺ������и�ʴ����
	MakeErosion(nMask, nMaskLen, pOldBuffer, pNewBuffer, nWidthBytes, nWidth, nHeight);
	
	//���ڴ�����ͽ�����ʹ�õ��ڴ��ͷ�
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	::GlobalFree( hNewDib );
	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL Dilation()   
//----------------------------------------------------------------------
//�������ܣ���������CDibObject�����е�ͼ������������㡣
//----------------------------------------------------------------------
//����˵����int			 *nMask			�ṹԪ������ָ��
//			int			 nMaskLen		�ṹԪ�س��ȣ��Ե���Ϊ������λ��
//			CDibObject *pDibObject	���ͼ������ָ��
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::Dilation(int *nMask, int nMaskLen,
							CDibObject *pDibObject)
{
	//ʹ�ô����CDibObject����
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );
	
	//����8λ�Ҷ�ͼ��
	int nNumBits = m_pDibObject->GetNumBits(); 
	if( nNumBits != 8 ) return( FALSE );

	//���ͼ���Ⱥ͸߶�
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	int nWidthBytes = m_pDibObject->WidthBytes(8, nWidth);

	//ͼ����������С
	DWORD dwSize = nWidthBytes * nHeight;
	
	//���ͼ��������ָ��
	unsigned char *pOldBuffer = GetBitsPoint();
	
	//Ϊ��ͼ������ڴ�
	HGLOBAL hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwSize );

	if( hNewDib == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ������ָ��
	unsigned char *pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	
    if (pNewBuffer == NULL || pOldBuffer == NULL)	
	{
		AfxMessageBox("����緳�����λ����");
		return (FALSE);
	}

	//��ԭͼ�������ƶ�����ͼ���У�ԭͼ���������㣩
	MoveBuffer(pNewBuffer, pOldBuffer, (LONG)dwSize);

	//����MakeDilation()�����ͺ������и�ʴ����
	MakeDilation(nMask, nMaskLen, pOldBuffer, pNewBuffer, nWidthBytes, nWidth, nHeight);
	
	//���ڴ�����ͽ�����ʹ�õ��ڴ��ͷ�
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	::GlobalFree( hNewDib );
	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL Opening()   
//----------------------------------------------------------------------
//�������ܣ���������CDibObject�����е�ͼ����п����㡣
//----------------------------------------------------------------------
//����˵����int			 *nMask			�ṹԪ������ָ��
//			int			 nMaskLen		�ṹԪ�س��ȣ��Ե���Ϊ������λ��
//			CDibObject *pDibObject	���ͼ������ָ��
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::Opening(int *nMask, int nMaskLen,
							CDibObject *pDibObject)
{
	//�Դ����CDibObject������и�ʴ
	if(Erosion(nMask, nMaskLen, pDibObject) != TRUE) return( FALSE );

	//���ṹԪ���Ƿ�Գ�
	int nSumX = 0, nSumY = 0;
	int i;
	for(i = 0; i < nMaskLen; i++)
	{
		nSumX += nMask[2 * i];
		nSumY += nMask[2 * i + 1];
	}

	//CString s;
	//s.Format("nSumX=%d, nSumY=%d",nSumX, nSumY);
	//AfxMessageBox(s);

	if(nSumX != 0 || nSumY != 0)
	{
		for(i = 0; i < nMaskLen; i++)
		{
			nMask[2 * i] = -nMask[2 * i];
			nMask[2 * i + 1] = -nMask[2 * i + 1];
		}
	}
	
	//�Դ����CDibObject�����������
	if(Dilation(nMask, nMaskLen, pDibObject) != TRUE) return( FALSE );
	
	return(TRUE);
}

////////////////////////////////////////////////////////////////////////
//BOOL Closing()   
//----------------------------------------------------------------------
//�������ܣ���������CDibObject�����е�ͼ����б����㡣
//----------------------------------------------------------------------
//����˵����int			 *nMask			�ṹԪ������ָ��
//			int			 nMaskLen		�ṹԪ�س��ȣ��Ե���Ϊ������λ��
//			CDibObject *pDibObject	���ͼ������ָ��
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::Closing(int *nMask, int nMaskLen,
							CDibObject *pDibObject)
{
	//�Դ����CDibObject�����������
	if(Dilation(nMask, nMaskLen, pDibObject) != TRUE) return( FALSE );

	//���ṹԪ���Ƿ�Գ�
	int nSumX = 0, nSumY = 0;
	int i;
	for(i = 0; i < nMaskLen; i++)
	{
		nSumX += nMask[2 * i];
		nSumY += nMask[2 * i + 1];
	}

	if(nSumX != 0 || nSumY != 0)
	{
		for(i = 0; i < nMaskLen; i++)
		{
			nMask[2 * i] = -nMask[2 * i];
			nMask[2 * i + 1] = -nMask[2 * i + 1];
		}
	}

	//�Դ����CDibObject������и�ʴ
	if(Erosion(nMask, nMaskLen, pDibObject) != TRUE) return( FALSE );
	
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//BOOL Closing()   
//----------------------------------------------------------------------
//�������ܣ���������CDibObject�����е�ͼ����л��л����б任��
//----------------------------------------------------------------------
//����˵����int			 *nHmask		���нṹԪ������ָ��
//			int			 nHmaskLen		���нṹԪ�س��ȣ�������
//			int			 *nMmask		�����нṹԪ������ָ��
//			int			 nMmaskLen		�����нṹԪ�س��ȣ�������
//			CDibObject *pDibObject	Ĭ��ΪNULL��
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::HitMissTran(int *nHmask, int nHmaskLen,
							int *nMmask, int nMmaskLen,
							CDibObject *pDibObject)
{
	//ʹ�ô����CDibObject����
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//����8λ�Ҷ�ͼ��
	int nNumBits = m_pDibObject->GetNumBits(); 
	if( nNumBits != 8 ) return( FALSE );

	//���ͼ���Ⱥ͸߶ȼ��ֽڿ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	int nWidthBytes = m_pDibObject->WidthBytes(8, nWidth);

	//ͼ����������С���ֽڣ�
	DWORD dwSize = nWidthBytes * nHeight;
	
	//ͼ��������ָ��
	unsigned char *pOldBuffer = GetBitsPoint();
	
	//Ϊ��ͼ������ڴ�
	HGLOBAL hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, 2 * dwSize );

	if( hNewDib == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ������ָ��
	unsigned char *pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	
    if (pNewBuffer == NULL || pOldBuffer == NULL)	
	{
		AfxMessageBox("����緳�����λ����");
		return (FALSE);
	}

	//����ͼ������
	CopyBuffer(pNewBuffer + dwSize, pOldBuffer, (LONG)dwSize);

	//���б任
	MoveBuffer(pNewBuffer, pOldBuffer, (LONG)dwSize);
	MakeErosion(nHmask, nHmaskLen, pOldBuffer, pNewBuffer, nWidthBytes, nWidth, nHeight);

	//�����б任
	ReverseBuffer(pNewBuffer + dwSize, (LONG)dwSize);
	MoveBuffer(pNewBuffer, pNewBuffer + dwSize, (LONG)dwSize);
	MakeErosion(nMmask, nMmaskLen, pNewBuffer + dwSize, pNewBuffer, nWidthBytes, 
		        nWidth, nHeight);

	//�󽻼�
	AndBuffer(pOldBuffer, pNewBuffer + dwSize, (LONG)dwSize);
	
	//���ڴ�����ͽ�����ʹ�õ��ڴ��ͷ�
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	::GlobalFree( hNewDib );
	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL Thining()   
//----------------------------------------------------------------------
//�������ܣ���������CDibObject�����е�ͼ����б������㡣
//----------------------------------------------------------------------
//����˵����int			 *nHmask		���нṹԪ������ָ��
//			int			 nHmaskLen		���нṹԪ�س��ȣ�������
//			int			 *nMmask		�����нṹԪ������ָ��
//			int			 nMmaskLen		�����нṹԪ�س��ȣ�������
//			CDibObject *pDibObject	Ĭ��ΪNULL��
//			BOOL		 *bIsBlank		����Ƿ�Ϊȫ�ڡ�
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::Thining(int *nHmask, int nHmaskLen,
							int *nMmask, int nMmaskLen,
							CDibObject *pDibObject,
							BOOL *bIsBlank)
{
	//ʹ�ô����CDibObject����
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//����8λ�Ҷ�ͼ��
	int nNumBits = m_pDibObject->GetNumBits(); 
	if( nNumBits != 8 ) return( FALSE );

	//���ͼ���Ⱥ͸߶�
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	int nWidthBytes = m_pDibObject->WidthBytes(8, nWidth);

	//ͼ����������С���ֽڣ�
	DWORD dwSize = nWidthBytes * nHeight;
	
	//ͼ��������ָ��
	unsigned char *pOldBuffer = GetBitsPoint();
	
	//Ϊ��ͼ������ڴ�
	HGLOBAL hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, 3 * dwSize );

	if( hNewDib == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ������ָ��
	unsigned char *pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	
    if (pNewBuffer == NULL || pOldBuffer == NULL)	
	{
		AfxMessageBox("����緳�����λ����");
		return (FALSE);
	}

	//����ͼ������
	CopyBuffer(pNewBuffer + dwSize, pOldBuffer, (LONG)dwSize);
	CopyBuffer(pNewBuffer + 2 * dwSize, pOldBuffer, (LONG)dwSize);

	//���б任
	MoveBuffer(pNewBuffer, pOldBuffer, (LONG)dwSize);
	MakeErosion(nHmask, nHmaskLen, pOldBuffer, pNewBuffer, nWidthBytes, nWidth, nHeight);

	//�����б任
	ReverseBuffer(pNewBuffer + dwSize, (LONG)dwSize);
	MoveBuffer(pNewBuffer, pNewBuffer + dwSize, (LONG)dwSize);
	MakeErosion(nMmask, nMmaskLen, 	pNewBuffer + dwSize, pNewBuffer, nWidthBytes, 
				nWidth, nHeight);

	//�󽻼�
	AndBuffer(pOldBuffer, pNewBuffer + dwSize, (LONG)dwSize);
	
	//��ԭͼ���ȥ���л����к��ͼ��
	XorBuffer(pOldBuffer, pNewBuffer + 2 * dwSize, (LONG)dwSize);

	XorBuffer(pNewBuffer + 2 * dwSize, pOldBuffer, (LONG)dwSize);

	*bIsBlank = IsResultBlank(pNewBuffer + 2 * dwSize, (LONG)dwSize);

	//���ڴ�����ͽ�����ʹ�õ��ڴ��ͷ�
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	::GlobalFree( hNewDib );
	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL Thicking()   
//----------------------------------------------------------------------
//�������ܣ���������CDibObject�����е�ͼ����к����㡣
//----------------------------------------------------------------------
//����˵����int			 *nHmask		���нṹԪ������ָ��
//			int			 nHmaskLen		���нṹԪ�س��ȣ�������
//			int			 *nMmask		�����нṹԪ������ָ��
//			int			 nMmaskLen		�����нṹԪ�س��ȣ�������
//			CDibObject *pDibObject	Ĭ��ΪNULL��
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::Thicking(int *nHmask, int nHmaskLen,
							int *nMmask, int nMmaskLen,
							CDibObject *pDibObject)
{
	//ʹ�ô����CDibObject����
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//����8λ�Ҷ�ͼ��
	int nNumBits = m_pDibObject->GetNumBits(); 
	if( nNumBits != 8 ) return( FALSE );

	//���ͼ���Ⱥ͸߶�
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	int nWidthBytes = m_pDibObject->WidthBytes(8, nWidth);

	//ͼ����������С
	DWORD dwSize = nWidthBytes * nHeight;
	
	//ͼ��������ָ��
	unsigned char *pOldBuffer = GetBitsPoint();
	
	//Ϊ��ͼ������ڴ�
	HGLOBAL hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, 3 * dwSize );

	if( hNewDib == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ������ָ��
	unsigned char *pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	
    if (pNewBuffer == NULL || pOldBuffer == NULL)	
	{
		AfxMessageBox("����緳�����λ����");
		return (FALSE);
	}

	//����ͼ������
	CopyBuffer(pNewBuffer + dwSize, pOldBuffer, (LONG)dwSize);
	CopyBuffer(pNewBuffer + 2 * dwSize, pOldBuffer, (LONG)dwSize);

	//���б任
	MoveBuffer(pNewBuffer, pOldBuffer, (LONG)dwSize);
	MakeErosion(nHmask, nHmaskLen, pOldBuffer, pNewBuffer, nWidthBytes, nWidth, nHeight);

	//�����б任
	ReverseBuffer(pNewBuffer + dwSize, (LONG)dwSize);
	MoveBuffer(pNewBuffer, pNewBuffer + dwSize, (LONG)dwSize);
	MakeErosion(nMmask, nMmaskLen, pNewBuffer + dwSize, pNewBuffer, nWidthBytes, 
				nWidth, nHeight);

	//�󽻼�
	AndBuffer(pOldBuffer, pNewBuffer + dwSize, (LONG)dwSize);
	
	//��ԭͼ���ϻ��л����б任���ͼ��
	OrBuffer(pOldBuffer, pNewBuffer + 2 * dwSize, (LONG)dwSize);
	
	//���ڴ�����ͽ�����ʹ�õ��ڴ��ͷ�
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	::GlobalFree( hNewDib );
	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL OutEdge()   
//----------------------------------------------------------------------
//�������ܣ���������CDibObject�����е�ͼ�������߽��⡣
//----------------------------------------------------------------------
//����˵����int			 *nmask			���нṹԪ������ָ��
//			int			 nmaskLen		���нṹԪ�س��ȣ�������
//			CDibObject *pDibObject	Ĭ��ΪNULL��
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::OutEdge(int *nMask, int nMaskLen,
							CDibObject *pDibObject)
{
	//ʹ�ô����CDibObject����
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//����8λ�Ҷ�ͼ��
	int nNumBits = m_pDibObject->GetNumBits(); 
	if( nNumBits != 8 ) return( FALSE );

	//���ͼ���Ⱥ͸߶�
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	int nWidthBytes = m_pDibObject->WidthBytes(8, nWidth);

	//ͼ����������С���ֽڣ�
	DWORD dwSize = nWidthBytes * nHeight;
	
	//ͼ��������ָ��
	unsigned char *pOldBuffer = GetBitsPoint();
	
	//Ϊ��ͼ������ڴ�
	HGLOBAL hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, 2 * dwSize );

	if( hNewDib == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ������ָ��
	unsigned char *pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	
    if (pNewBuffer == NULL || pOldBuffer == NULL)	
	{
		AfxMessageBox("����緳�����λ����");
		return (FALSE);
	}

	//����ͼ������
	CopyBuffer(pNewBuffer + dwSize, pOldBuffer, (LONG)dwSize);

	//����
	MoveBuffer(pNewBuffer, pOldBuffer, (LONG)dwSize);
	MakeDilation(nMask, nMaskLen, pOldBuffer, pNewBuffer, nWidthBytes, nWidth, nHeight);

	//�����ͺ�Ľ����ԭͼ
	XorBuffer(pOldBuffer, pNewBuffer + dwSize, (LONG)dwSize);
	
	//���ڴ�����ͽ�����ʹ�õ��ڴ��ͷ�
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	::GlobalFree( hNewDib );
	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL InEdge()   
//----------------------------------------------------------------------
//�������ܣ���������CDibObject�����е�ͼ������ڱ߽��⡣
//----------------------------------------------------------------------
//����˵����int			 *nmask			���нṹԪ������ָ��
//			int			 nmaskLen		���нṹԪ�س��ȣ�������
//			CDibObject *pDibObject	Ĭ��ΪNULL��
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::InEdge(int *nMask, int nMaskLen,
							CDibObject *pDibObject)
{
	//ʹ�ô����CDibObject����
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//����8λ�Ҷ�ͼ��
	int nNumBits = m_pDibObject->GetNumBits(); 
	if( nNumBits != 8 ) return( FALSE );

	//���ͼ���Ⱥ͸߶�
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	int nWidthBytes = m_pDibObject->WidthBytes(8, nWidth);

	//ͼ����������С���ֽڣ�
	DWORD dwSize = nWidthBytes * nHeight;
	
	//ͼ��������ָ��
	unsigned char *pOldBuffer = GetBitsPoint();
	
	//Ϊ��ͼ������ڴ�
	HGLOBAL hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, 2 * dwSize );

	if( hNewDib == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ������ָ��
	unsigned char *pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	
    if (pNewBuffer == NULL || pOldBuffer == NULL)	
	{
		AfxMessageBox("����緳�����λ����");
		return (FALSE);
	}

	//����ͼ������
	CopyBuffer(pNewBuffer + dwSize, pOldBuffer, (LONG)dwSize);

	//��ʴ
	MoveBuffer(pNewBuffer, pOldBuffer, (LONG)dwSize);
	MakeErosion(nMask, nMaskLen, pOldBuffer, pNewBuffer, nWidthBytes, nWidth, nHeight);

	//��ԭͼ����ʴ��Ľ��
	XorBuffer(pOldBuffer, pNewBuffer + dwSize, (LONG)dwSize);
	
	//���ڴ�����ͽ�����ʹ�õ��ڴ��ͷ�
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	::GlobalFree( hNewDib );
	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL MorphGrads()   
//----------------------------------------------------------------------
//�������ܣ���������CDibObject�����е�ͼ�������̬ѧ�ݶ����㡣
//----------------------------------------------------------------------
//����˵����int			 *nMask		���нṹԪ������ָ��
//			int			 nMaskLen		���нṹԪ�س��ȣ�������
//			CDibObject *pDibObject	Ĭ��ΪNULL��
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::MorphGrads(int *nMask, int nMaskLen,
							CDibObject *pDibObject)
{
	//ʹ�ô����CDibObject����
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//����8λ�Ҷ�ͼ��
	int nNumBits = m_pDibObject->GetNumBits(); 
	if( nNumBits != 8 ) return( FALSE );

	//���ͼ���Ⱥ͸߶�
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	int nWidthBytes = m_pDibObject->WidthBytes(8, nWidth);

	//ͼ����������С���ֽڣ�
	DWORD dwSize = nWidthBytes * nHeight;
	
	//ͼ��������ָ��
	unsigned char *pOldBuffer = GetBitsPoint();
	
	//Ϊ��ͼ������ڴ�
	HGLOBAL hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, 2 * dwSize );

	if( hNewDib == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ������ָ��
	unsigned char *pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	
    if (pNewBuffer == NULL || pOldBuffer == NULL)	
	{
		AfxMessageBox("����緳�����λ����");
		return (FALSE);
	}

	//����ͼ������
	CopyBuffer(pNewBuffer + dwSize, pOldBuffer, (LONG)dwSize);

	//����
	MoveBuffer(pNewBuffer, pOldBuffer, (LONG)dwSize);
	MakeDilation(nMask, nMaskLen, pOldBuffer, pNewBuffer, nWidthBytes, nWidth, nHeight);

	//��ʴ
	MoveBuffer(pNewBuffer, pNewBuffer + dwSize, (LONG)dwSize);
	MakeErosion(nMask, nMaskLen, pNewBuffer + dwSize, pNewBuffer, nWidthBytes, 
				nWidth, nHeight);

	//�����ͺ�Ľ������ʴ��Ľ��
	XorBuffer(pOldBuffer, pNewBuffer + dwSize, (LONG)dwSize);
	
	//���ڴ�����ͽ�����ʹ�õ��ڴ��ͷ�
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
	::GlobalFree( hNewDib );
	return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//BOOL ThiningDIB()   
//----------------------------------------------------------------------
//�������ܣ���������CDibObject�����е�ͼ�����ϸ�����㡣
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject	Ĭ��ΪNULL��
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CMorphPro::ThiningDIB(CDibObject *pDibObject)
{
	//ʹ�ô����CDibObject����
	if( pDibObject != NULL ) m_pDibObject = pDibObject;
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );


	//�������
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;
	int lWidth,lHeight;
	
	//���ͼ��ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( NULL );

	//�����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//��õ�ɫ��ָ��
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//���λͼ����ָ��
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+nNumColors*sizeof(RGBQUAD)];

	lWidth=m_pDibObject->GetWidth();
	lHeight=m_pDibObject->GetHeight();

	/////////////////////////////////////////////////////////
		// ָ��Դͼ���ָ��
	LPSTR	lpSrc;
	
	// ָ�򻺴�ͼ���ָ��
	LPSTR	lpDst;
	
	// ָ�򻺴�DIBͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//����
	BOOL bModified;

	//ѭ������
	long i;
	long j;
	int  n;
	int  m;

	//�ĸ�����
	BOOL bCondition1;
	BOOL bCondition2;
	BOOL bCondition3;
	BOOL bCondition4;

	//������
	unsigned char nCount;
	
	//����ֵ
	unsigned char pixel;

	//5��5������������ֵ
	unsigned char neighbour[5][5];

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}
	
	// �����ڴ�
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (char *)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);

	bModified=TRUE;

	while(bModified)
	{

		bModified = FALSE;
		// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
		lpDst = (char *)lpNewDIBBits;
		memset(lpDst, (BYTE)255, lWidth * lHeight);

		for(j = 2; j <lHeight-2; j++)
		{
			for(i = 2;i <lWidth-2; i++)
			{

				bCondition1 = FALSE;
				bCondition2 = FALSE;
				bCondition3 = FALSE;
				bCondition4 = FALSE;

				//����ʹ��5��5�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ�������Χ�ļ��кͼ�������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (char *)pBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (char *)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)*lpSrc;

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if(pixel != 255 && *lpSrc != 0)
					//return FALSE;
					continue;
				//���Դͼ���е�ǰ��Ϊ��ɫ��������
				else if(pixel == 255)
					continue;

				//��õ�ǰ�����ڵ�5��5����������ֵ����ɫ��0������ɫ��1����
				for (m = 0;m < 5;m++ )
				{
					for (n = 0;n < 5;n++)
					{
						neighbour[m][n] =(255 - (unsigned char)*(lpSrc + ((4 - m) - 2)*lWidth + n - 2 )) / 255;
					}
				}
				//����ж�������
				//�ж�2<=NZ(P1)<=6
				nCount =  neighbour[1][1] + neighbour[1][2] + neighbour[1][3] \
						+ neighbour[2][1] + neighbour[2][3] + \
						+ neighbour[3][1] + neighbour[3][2] + neighbour[3][3];
				if ( nCount >= 2 && nCount <=6)
					bCondition1 = TRUE;

				//�ж�Z0(P1)=1
				nCount = 0;
				if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
					nCount++;
				if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
					nCount++;
				if (neighbour[2][1] == 0 && neighbour[3][1] == 1)
					nCount++;
				if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
					nCount++;
				if (neighbour[3][2] == 0 && neighbour[3][3] == 1)
					nCount++;
				if (neighbour[3][3] == 0 && neighbour[2][3] == 1)
					nCount++;
				if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
					nCount++;
				if (neighbour[1][3] == 0 && neighbour[1][2] == 1)
					nCount++;
				if (nCount == 1)
					bCondition2 = TRUE;

				//�ж�P2*P4*P8=0 or Z0(p2)!=1
				if (neighbour[1][2]*neighbour[2][1]*neighbour[2][3] == 0)
					bCondition3 = TRUE;
				else
				{
					nCount = 0;
					if (neighbour[0][2] == 0 && neighbour[0][1] == 1)
						nCount++;
					if (neighbour[0][1] == 0 && neighbour[1][1] == 1)
						nCount++;
					if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
						nCount++;
					if (neighbour[2][1] == 0 && neighbour[2][2] == 1)
						nCount++;
					if (neighbour[2][2] == 0 && neighbour[2][3] == 1)
						nCount++;
					if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
						nCount++;
					if (neighbour[1][3] == 0 && neighbour[0][3] == 1)
						nCount++;
					if (neighbour[0][3] == 0 && neighbour[0][2] == 1)
						nCount++;
					if (nCount != 1)
						bCondition3 = TRUE;
				}

				//�ж�P2*P4*P6=0 or Z0(p4)!=1
				if (neighbour[1][2]*neighbour[2][1]*neighbour[3][2] == 0)
					bCondition4 = TRUE;
				else
				{
					nCount = 0;
					if (neighbour[1][1] == 0 && neighbour[1][0] == 1)
						nCount++;
					if (neighbour[1][0] == 0 && neighbour[2][0] == 1)
						nCount++;
					if (neighbour[2][0] == 0 && neighbour[3][0] == 1)
						nCount++;
					if (neighbour[3][0] == 0 && neighbour[3][1] == 1)
						nCount++;
					if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
						nCount++;
					if (neighbour[3][2] == 0 && neighbour[2][2] == 1)
						nCount++;
					if (neighbour[2][2] == 0 && neighbour[1][2] == 1)
						nCount++;
					if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
						nCount++;
					if (nCount != 1)
						bCondition4 = TRUE;
				}
				if(bCondition1 && bCondition2 && bCondition3 && bCondition4)
				{
					*lpDst = (unsigned char)255;
					bModified = TRUE;
				}
				else
				{
					*lpDst = (unsigned char)0;
				}
			}
		}
			// ���Ƹ�ʴ���ͼ��
			memcpy(pBits, lpNewDIBBits, lWidth * lHeight);


	}
	// ���Ƹ�ʴ���ͼ��
	memcpy(pBits, lpNewDIBBits, lWidth * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// ����
	return TRUE;
}
