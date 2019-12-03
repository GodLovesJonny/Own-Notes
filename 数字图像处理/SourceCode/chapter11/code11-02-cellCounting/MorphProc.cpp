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


/************************************************************************
*                                                                       *
*            ���º������ͼ�������͡���ʴ�������յ��������                 *
*	                                                                    *
************************************************************************/

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
					//ȡ��ģ����λ�����Ӧ������ֵ
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
		pOutTemp +=  y *nWidthBytes;
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
					//ȡģ���е�����ֵ
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
					Mark=1;
					k=nMaskLen;
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

