// Pretreatment.cpp: implementation of the CPretreatment class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImgPro.h"
#include "Pretreatment.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPretreatment::CPretreatment()
{
	m_pDibObject=NULL;
}

CPretreatment::CPretreatment(CDibObject *pDibObject)
{
	m_pDibObject=(CPretreatment *)pDibObject;
}

CPretreatment::~CPretreatment()
{

}
////////////////////////////////////////////////////////////////////////
//BOOL IsGrayImage(CDibObject *pDibObject)
//----------------------------------------------------------------------
//�������ܣ��������жϴ����CDibObject�����е�ͼ���Ƿ�Ϊ256���Ҷ�ͼ��
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject, Ĭ��ΪNULL
//----------------------------------------------------------------------
//���أ�BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPretreatment::IsGrayImage(CDibObject *pDibObject)
{
//ʹ�ô����CDibObject����
	if( pDibObject != NULL ) m_pDibObject =(CPretreatment *) pDibObject;
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//����8λͼ�񣬱ز���256���Ҷ�ͼ�񣬲�������FALSE
	if( m_pDibObject->GetNumBits() != 8 ) return( FALSE );

	//�������
	unsigned char *pBuffer;
	RGBQUAD *pPalette;
	int nWidthBytes, i;

	//pBuffer: ���λͼ����ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( FALSE );

	//pPalette����õ�ɫ�����ݵ�ַ
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+
									sizeof(BITMAPINFOHEADER)];

	//����Ƿ�Ϊ256ɫ�Ҷȵ�ɫ��
	for( i=0; i<256; i++ )
	{
		if( pPalette[i].rgbRed != pPalette[i].rgbGreen 
			|| pPalette[i].rgbRed != pPalette[i].rgbBlue
			|| pPalette[i].rgbGreen != pPalette[i].rgbBlue )
			return( FALSE);
	}
	::GlobalUnlock( m_pDibObject->GetDib() );

	return( TRUE );
}

BOOL CPretreatment::MakeGray(CDibObject *pDibObject)
{
	//û�д�����Ӧָ���޷���ɲ���
	if(m_pDibObject == NULL)
	{
		AfxMessageBox("û�д���ԭʼͼ���޷���ɲ�����");
		return FALSE;
	}

	if(pDibObject == NULL)
	{
		AfxMessageBox("û�д���ͨ��ָ�룬�޷���ɲ�����");
		return FALSE;
	}

	//����256ɫ�Ҷ�ͼ������ת��
	if(IsGrayImage(m_pDibObject))
	{
		AfxMessageBox("����256ɫ�Ҷ�ͼ�񣬲��ܽ���ͨ�����룡");
		return FALSE;
	}

	//CDibObject����ָ��
	if( pDibObject != NULL ) m_pDibObject = (CPretreatment *)pDibObject;
	//��δָ�� CDibObject ����ָ�뷵��FALSE
	if( m_pDibObject == NULL ) return( FALSE );
	//����8λ��ͼ�񲻽��д���
	if( m_pDibObject->GetNumBits() < 8 ) return( FALSE );


	//��ȡԭͼ���ֽڿ�Ⱥ�ת�����8λ256ɫ�Ҷ�ͼ����ֽڿ��
	int nOldWidthBytes, nNewWidthBytes;
	char *pBuffer = (char *) m_pDibObject->GetDIBPointer( &nOldWidthBytes, 8, &nNewWidthBytes );
	if( pBuffer == NULL ) return( FALSE );

	//�������
	BITMAPINFOHEADER *pOldBIH, *pNewBIH;
	BITMAPFILEHEADER *pOldBFH, *pNewBFH;
	RGBQUAD *pOldRGBPalette, *pNewRGBPalette;
	unsigned char *pOldBits, *pNewBits, *pTemp, *pNewTemp;
	int nNumColors, nNumNewColors;

	//��ȡ�ļ�ͷָ��
	pOldBFH = (BITMAPFILEHEADER *) pBuffer;
	//��ȡ��Ϣͷָ��
	pOldBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//��ȡ��ɫ��ָ��
	pOldRGBPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+
									sizeof(BITMAPINFOHEADER)];
	//ԭͼ����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//��ͼ����ɫ��
	nNumNewColors = 256;
	//��ȡԭͼ������ָ��
	pOldBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)
		+sizeof(BITMAPINFOHEADER)+nNumColors*sizeof(RGBQUAD)];

	//Ϊ��ͼ������ڴ�
	HGLOBAL hGlobal;
	//��ͼ�����ֽ���
	DWORD dwSize;
	dwSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) +
			256 * sizeof( RGBQUAD ) + 
			m_pDibObject->GetHeight() * nNewWidthBytes;
	hGlobal = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwSize );
	if( hGlobal == NULL )
	{
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	pBuffer = (char *) ::GlobalLock( hGlobal );
	if( pBuffer == NULL )
	{
		::GlobalFree( hGlobal );
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//�����ͼ����ļ�ͷָ��
	pNewBFH = (BITMAPFILEHEADER *) pBuffer;
	//�����ͼ�����Ϣͷָ��
	pNewBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//�����ͼ��ĵ�ɫ��ָ��
	pNewRGBPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)
						+sizeof(BITMAPINFOHEADER)];
	//����ԭͼ���ļ�ͷ���ݵ���ͼ���ļ�ͷ
	*pNewBFH = *pOldBFH;
	//����ԭͼ����Ϣͷ���ݵ���ͼ����Ϣͷ
	*pNewBIH = *pOldBIH;

	//ѭ����������
	int i, j = 256, x, y;

	pNewBIH->biBitCount = 8;
	pNewBIH->biSizeImage = nNewWidthBytes * m_pDibObject->GetHeight();
	pNewBIH->biClrUsed = 256;
	pNewBFH->bfSize =	sizeof( BITMAPFILEHEADER ) + 
						sizeof( BITMAPINFOHEADER ) + 
						256 * sizeof( RGBQUAD ) + 
						pNewBIH->biSizeImage;
	pNewBFH->bfOffBits =	sizeof( BITMAPFILEHEADER ) + 
							sizeof( BITMAPINFOHEADER ) + 
							nNumNewColors * sizeof( RGBQUAD );
	pNewBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) +
		sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)];
	m_pDibObject->SetPaletteBytes( 256 * sizeof( RGBQUAD ));
	
	//����256ɫ�Ҷȵ�ɫ��
	for( i = 0; i < j; i++ )
	{
		pNewRGBPalette[i].rgbRed = i;
		pNewRGBPalette[i].rgbGreen = i;
		pNewRGBPalette[i].rgbBlue = i;
	}


	unsigned char *pLookup;	//��ɫ����ұ�
	DWORD dwGray;	//�Ҷȼ���

	switch( m_pDibObject->GetNumBits() )
	{
	case 8:		//256ɫͼ��
		pLookup = new unsigned char [256];
		if( pLookup == NULL ) break;
		memset( pLookup, 0, 256 );	//��ɫ����ұ���0��256�

		//������Y=0.3R+0.59G+0.11B����ɫͼ��ת��Ϊ�Ҷ�ͼ��
		for( i=0; i<256; i++ )
		{
			dwGray = ( (DWORD) pOldRGBPalette[i].rgbRed * 30 +
				(DWORD) pOldRGBPalette[i].rgbGreen * 59 +
				(DWORD) pOldRGBPalette[i].rgbBlue * 11 ) / 100;
			pLookup[i] = (unsigned char) dwGray;
			}
		for( y = 0; y < pOldBIH->biHeight; y++ )
		{
			pTemp = pOldBits;	//λͼ������ʼָ��
			pTemp += y * nOldWidthBytes;	//λͼ������һ����ʼָ��
			//ת���ɻҶ�����
			for( x = 0; x < pOldBIH->biWidth; x++ ) pTemp[x] = pLookup[pTemp[x]];
		}
		delete [] pLookup;	//�ͷ�pLookup���ұ���ռ�ڴ�
		
		memcpy( pNewBits, pOldBits, nNewWidthBytes * m_pDibObject->GetHeight());
		
		break;
	case 16:	//16λɫ���ɫͼ��
		unsigned char ucRed, ucGreen, ucBlue;
		for( y=0; y<pOldBIH->biHeight; y++ )
		{
			//λͼ������ʼָ��
			pTemp = pOldBits;	
			pNewTemp = pNewBits;
			//λͼ������һ����ʼָ��
			pTemp += y * nOldWidthBytes;	
			pNewTemp += y * nNewWidthBytes;

			for( x=0; x<pOldBIH->biWidth; x++ )
			{
				GETRGB555( ucRed, ucGreen, ucBlue, &pTemp[x*2] );
				//������Y=0.3R+0.59G+0.11B����ɫͼ��ת��Ϊ�Ҷ�ͼ��
				dwGray = (ucRed * 30 + ucGreen * 59 +ucBlue * 11) / 100;
				//����ͼ�����ݸ�ֵ
				pNewTemp[x] = (unsigned char)dwGray;
			}
		}
		break;
	case 24:	//24λ���ɫͼ��
		for( y=0; y<pOldBIH->biHeight; y++ )
		{
			//λͼ������ʼָ��
			pTemp = pOldBits;	
			pNewTemp = pNewBits;
			//λͼ������һ����ʼָ��
			pTemp += y * nOldWidthBytes;	
			pNewTemp += y * nNewWidthBytes;

			//������Y=0.3R+0.59G+0.11B����ɫͼ��ת��Ϊ�Ҷ�ͼ��
			for( x=0; x<pOldBIH->biWidth; x++ )
			{
				dwGray = ( (DWORD) pTemp[x*3+2] * 30	//��ɫ
					+(DWORD) pTemp[x*3+1] * 59		//��ɫ
					+(DWORD) pTemp[x*3] * 11		//��ɫ
					) / 100;
					//����ͼ�����ݸ�ֵ
				pNewTemp[x] = (unsigned char)dwGray;
			}
		}
		break;
	case 32:	//32λ���ɫͼ��
		for( y=0; y<pOldBIH->biHeight; y++ )
		{
			//λͼ������ʼָ��
			pTemp = pOldBits;	
			pNewTemp = pNewBits;
			//λͼ������һ����ʼָ��
			pTemp += y * nOldWidthBytes;	
			pNewTemp += y * nNewWidthBytes;
		
			for( x=0; x<pOldBIH->biWidth; x++ )
			{
				//������Y=0.3R+0.59G+0.11B����ɫͼ��ת��Ϊ�Ҷ�ͼ��
				dwGray = ( (DWORD) pTemp[x*4] * 30 //��ɫ
					+(DWORD) pTemp[x*4+1] * 59	//��ɫ
					+(DWORD) pTemp[x*4+2] * 11 //��ɫ
					) / 100;
				//����ͼ�����ݸ�ֵ
				pNewTemp[x] = (unsigned char)dwGray;
			}	
		}
		break;
	}
			
	::GlobalUnlock( m_pDibObject->GetDib() );
	::GlobalFree( m_pDibObject->GetDib() );
	::GlobalUnlock( hGlobal );
	m_pDibObject->SetDib( hGlobal );

	m_pDibObject->ProcessImageHeader();

	m_pDibObject->m_nLastError = IMAGELIB_SUCCESS;

	return( TRUE );
}


////////////////////////////////////////////////////////////////////////
//BOOL AutoLight()
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObject�����е�ͼ����лҶ�����任�����
//			���д˵���֮ǰû��ָ��һ��CDibObject����ָ�룬������ڵ���
//			ʱ����ָ�����κ�δ���������ֵ��Ĭ�ϵ�-1����ֵ��������Ϊͼ��
//			�����ֵ�����ֲ������nX1��nY1������Ϊ0��nX2������Ϊͼ����
//			��1��nY2������Ϊͼ��߶ȼ�1����Ҫ������ͼ���Ͻ��в���ʱ����
//			�õķ����ǲ�����nX1��nY1��nX2��nY2ֵ���������ǻᱻĬ��Ϊ����
//			ͼ��
//----------------------------------------------------------------------
//����˵����BYTE bX1	�Ҷ������һ�����x����
//			BYTE bY1	�Ҷ������һ�����y����
//			BYTE bX2	�Ҷ�����ڶ������x����
//			BYTE bY2	�Ҷ�����ڶ������y����
//			int	nX1, Ĭ��Ϊ-1
//			int	nY1, Ĭ��Ϊ-1
//			int	nX2, Ĭ��Ϊ-1
//			int	nY2, Ĭ��Ϊ-1
//			CDibObject *pDibObject, Ĭ��ΪNULL
//----------------------------------------------------------------------
//���أ�BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPretreatment::AutoLight(
									   BYTE bX1,
									   BYTE bY1,
									   BYTE bX2,
									   BYTE bY2,
									   int nX1,
									   int nY1,
									   int nX2,
									   int nY2, 
									   CDibObject *pDibObject)
{
	// �Ҷ�ӳ���
	unsigned char *pMap = new unsigned char[256];

	// ����Ҷ�ӳ���
	for (int i = 0; i <= bX1; i++)
	{
		// �ж�bX1�Ƿ����0����ֹ��ĸΪ0��
		if (bX1 > 0)
		{
			// ���Ա任
			pMap[i] = (BYTE) bY1 * i / bX1;
		}
		else
		{
			// ֱ�Ӹ�ֵΪ0
			pMap[i] = 0;
		}
	}
	for (; i <= bX2; i++)
	{
		// �ж�bX1�Ƿ����bX2����ֹ��ĸΪ0��
		if (bX2 != bX1)
		{
			// ���Ա任
			pMap[i] = bY1 + (BYTE) ((bY2 - bY1) * (i - bX1) / (bX2 - bX1));
		}
		else
		{
			// ֱ�Ӹ�ֵΪbY1
			pMap[i] = bY1;
		}
	}

	for (; i < 256; i++)
	{
		// �ж�bX2�Ƿ����255����ֹ��ĸΪ0��
		if (bX2 != 255)
		{
			// ���Ա任
			pMap[i] = bY2 + (BYTE) ((255 - bY2) * (i - bX2) / (255 - bX2));
		}
		else
		{
			// ֱ�Ӹ�ֵΪ255
			pMap[i] = 255;
		}
	}

	//CDibObject����ָ��
	if( pDibObject != NULL ) m_pDibObject =(CPretreatment *) pDibObject;
	//��δָ�� CDibObject ����ָ�뷵��FALSE
	if( m_pDibObject == NULL ) return( FALSE );
	BOOL bLessThanHalf, bCompleteImage;	

	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2,
										&bCompleteImage, 
										&bLessThanHalf );
	//�������
	unsigned char *pBuffer, *pBits, *pTemp;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors, x, y;
	
	//pBuffer: ���λͼ����ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( FALSE );
	
	//pBIH�����λͼ��Ϣͷ��ַ
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors����õ�ɫ���е���ɫ����ͼ��Ϊ16λɫ�����ʱΪ0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette����õ�ɫ�����ݵ�ַ
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits�����λͼ���ݵ�ַ
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
		nNumColors*sizeof(RGBQUAD)];
	
	LOGPALETTE *pLogPal = m_pDibObject->GetLogPal();
	CPalette *_pPalette = m_pDibObject->GetPalette();
	
	switch( m_pDibObject->GetNumBits() )
	{
	case 1:
		for( y=nY1; y<=nY2; y++ )
		{
			unsigned char Mask;
			pTemp = pBits;
			pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y ) 
				* nWidthBytes ) + ( nX1 / 8 ) );
			for( x=nX1; x<=nX2; x++ )
			{
				Mask = 0x80 >> ( x & 7 );
				if( pTemp[0] & Mask ) pTemp[0] &= ( Mask ^ 0xff );
				else pTemp[0] |= Mask;
				if( ( x & 7 ) == 7 ) pTemp++;
			}
		}
		break;
	case 4:
		if( bCompleteImage )
		{
			_pPalette->DeleteObject();
			for( i=0; i<nNumColors; i++ )
			{
				pPalette[i].rgbRed = pMap[pPalette[i].rgbRed];
				pPalette[i].rgbGreen = pMap[pPalette[i].rgbGreen];
				pPalette[i].rgbBlue = pMap[pPalette[i].rgbBlue];
			}
			LOGPALETTE *pLogPal = m_pDibObject->CreateLogPalette( 
								pPalette, nNumColors );
			if( pLogPal != NULL )
			{
				_pPalette->CreatePalette( pLogPal );
				delete [] pLogPal;
			}
		}
		else
		{
			unsigned char ucRed, ucGreen, ucBlue, Data;
			for( y=nY1; y<=nY2; y++ )
			{
				pTemp = pBits;
				pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y )
					* nWidthBytes ) + ( nX1 / 2 ) );
				for( x=nX1; x<=nX2; x++ )
				{
					Data = *pTemp;
					if( !( x & 1 ) ) Data >>= 4;
					else Data &= 0x0f;
					ucRed = pPalette[Data].rgbRed;
					ucGreen = pPalette[Data].rgbGreen;
					ucBlue = pPalette[Data].rgbBlue;
					ucRed = pMap[ucRed];
					ucGreen = pMap[ucGreen];
					ucBlue = pMap[ucBlue];
					Data = (unsigned char) m_pDibObject->GetNearestIndex( ucRed, ucGreen, 
						ucBlue, pPalette, nNumColors );
					if( !( x & 1 ) )
					{
						Data <<= 4;
						(*pTemp) &= 0x0f;
						(*pTemp) |= Data;
					}
					else
					{
						(*pTemp) &= 0xf0;
						(*pTemp) |= Data;
						pTemp++;
					}
				}
			}
		}
		break;
	case 8:
		if( bCompleteImage )
		{
			_pPalette->DeleteObject();
			for( i=0; i<nNumColors; i++ )
			{
				pPalette[i].rgbRed = pMap[pPalette[i].rgbRed];
				pPalette[i].rgbGreen = pMap[pPalette[i].rgbGreen];
				pPalette[i].rgbBlue = pMap[pPalette[i].rgbBlue];
			}
			LOGPALETTE *pLogPal = m_pDibObject->CreateLogPalette( pPalette, nNumColors );
			if( pLogPal != NULL )
			{
				_pPalette->CreatePalette( pLogPal );
				delete [] pLogPal;
			}
		}
		else
		{
			unsigned char ucRed, ucGreen, ucBlue, Data;
			for( y=nY1; y<=nY2; y++ )
			{
				pTemp = pBits;
				pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y )
					* nWidthBytes ) + nX1 );
				for( x=nX1; x<=nX2; x++ )
				{
					Data = *pTemp;
					ucRed = pPalette[Data].rgbRed;
					ucGreen = pPalette[Data].rgbGreen;
					ucBlue = pPalette[Data].rgbBlue;
					ucRed = pMap[ucRed];
					ucGreen = pMap[ucGreen];
					ucBlue = pMap[ucBlue];
					Data = (unsigned char) m_pDibObject->GetNearestIndex( ucRed, 
						ucGreen, ucBlue, pPalette, nNumColors );
					*pTemp++ = Data;
				}
			}
		}
		break;
	case 16:
		unsigned char ucRed, ucGreen, ucBlue;
		for( y=0; y< nY2; y++ )
		{
			pTemp = pBits;
			pTemp += ( ( m_pDibObject->GetHeight() - 1 - y ) 
						* nWidthBytes );
			pTemp += ( nX1 * 2 );
			for( x=nX1; x<=nX2; x++ )
			{
				GETRGB555( ucRed, ucGreen, ucBlue, pTemp );
				ucRed = pMap[ucRed];
				ucGreen = pMap[ucGreen];
				ucBlue = pMap[ucBlue];
				PUTRGB555( ucRed, ucGreen, ucBlue, pTemp );
				pTemp += 2;
			}
		}
		break;
	case 24:
		for( y=nY1; y<=nY2; y++ )
		{
			pTemp = pBits;
			pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y )
				* nWidthBytes ) + ( nX1 * 3 ) );
			for( x=nX1; x<=nX2; x++ )
			{
				pTemp[0] = pMap[pTemp[0]];
				pTemp[1] = pMap[pTemp[1]];
				pTemp[2] = pMap[pTemp[2]];
				pTemp += 3;
			}
		}
		break;
	case 32:
		for( y=nY1; y<=nY2; y++ )
		{
			pTemp = pBits;
			pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y ) 
				* nWidthBytes ) + ( nX1 * 4 ) );
			for( x=nX1; x<=nX2; x++ )
			{
				pTemp[0] = pMap[pTemp[0]];
				pTemp[1] = pMap[pTemp[1]];
				pTemp[2] = pMap[pTemp[2]];
				pTemp[3] = pMap[pTemp[3]];
				pTemp += 4;
			}
		}
		break;
	}
	
	delete [] pMap;
	
	::GlobalUnlock( m_pDibObject->GetDib() );
	
	m_pDibObject->m_nLastError = IMAGELIB_SUCCESS;
	
	// ����
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//int *GetHistogram()   
//----------------------------------------------------------------------
//�������ܣ���������ô����CDibObject������ͼ���ֱ��ͼ��������д˵�
//			��֮ǰû��ָ��һ��CDibObject����ָ�룬������ڵ���ʱ����ָ
//			�����κ�δ���������ֵ��Ĭ�ϵ�-1����ֵ��������Ϊͼ������ֵ
//			�����ֲ������nX1��nY1������Ϊ0��nX2������Ϊͼ���ȼ�1��nY2
//			������Ϊͼ��߶ȼ�1����Ҫ������ͼ���Ͻ��в���ʱ����õķ���
//			�ǲ�����nX1��nY1��nX2��nY2ֵ���������ǻᱻĬ��Ϊ����ͼ��
//----------------------------------------------------------------------
//����˵����int	nX1, Ĭ��Ϊ-1
//			int	nY1, Ĭ��Ϊ-1
//			int	nX2, Ĭ��Ϊ-1
//			int	nY2, Ĭ��Ϊ-1
//			CDibObject *pDibObject, Ĭ��ΪNULL��
//----------------------------------------------------------------------
//��    �أ�ֱ��ͼ����ָ��*pBuffer���У�
//			nHistogramBuffer[]		�洢����ֱ��ͼ����
//			nHistogramBuffer[256]	�洢��ɫֱ��ͼ����
//			nHistogramBuffer[512]	�洢��ɫֱ��ͼ����
//			nHistogramBuffer[768]	�洢��ֱ��ͼ����
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////

int * CPretreatment::GetHistogram(  int nX1, 
									int nY1,
									int nX2,
									int nY2,
									CDibObject *pDibObject)
{
	//ʹ�ô����CDibObject����
	if( pDibObject != NULL ) m_pDibObject =(CPretreatment *) pDibObject;
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//���������
	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2 );

	//�������
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;

	//���ͼ��ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( NULL );

	//�����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//��õ�ɫ��ָ��
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//���λͼ����ָ��
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+nNumColors*sizeof(RGBQUAD)];

	//����ֱ��ͼ����
	int *nHistogramBuffer = CreateHistogram( nX1, nY1, nX2, nY2, pBits, pPalette, nWidthBytes );

	::GlobalUnlock( m_pDibObject->GetDib() );

	return( nHistogramBuffer );

}

int * CPretreatment::CreateHistogram(int nX1, int nY1, int nX2, int nY2, unsigned char *pData, RGBQUAD *pPalette, int nWidthBytes,CDibObject *pDibObject)
{

	//ͼ��ָ��Ϊ�գ��޷���������
	if( pDibObject != NULL ) m_pDibObject =(CPretreatment *) pDibObject;
	if( m_pDibObject == NULL ) return( FALSE );

	//����ֱ��ͼ���ݻ����������飩
	int *pBuffer = new int [256 * 4];
	//����ֱ��ͼ���ݻ�����ʧ��
	if( pBuffer == NULL ) return( NULL );
	//ֱ��ͼ���ݻ���������
	memset( pBuffer, 0, ( 256 * 4) * sizeof( int ) );

	//��������
	DWORD dwGray;
	int x, y;
	unsigned char *pTemp, ucRed, ucGreen, ucBlue;

	//ͼ��ĸ߶�
	int nHeight = m_pDibObject->GetHeight();

	switch( m_pDibObject->GetNumBits() )
	{
	case 1:				//ÿ����λ��Ϊ1��������
		break;
	case 4:				//ÿ����λ��Ϊ4��������
		break;
	case 8:				//ÿ����λ��Ϊ8		
		for( y = nY1; y <= nY2; y++ )
		{
			//����ָ�붨λ��ͼ��������ʼλ��
			pTemp = pData;
			//����ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
			pTemp += ( ( nHeight - 1 - y ) * nWidthBytes );
			//����ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
			pTemp += nX1;
			for( x = nX1; x <= nX2; x++ )
			{
				//pTemp[x]Ϊ��ǰ����ֵ����Ϊ��ɫ���������ֵ��
				//�Դ�Ϊ������ȡ����ɫ�������Ӧ����������ֵ��
				ucRed   = pPalette[pTemp[x]].rgbRed;
				ucGreen = pPalette[pTemp[x]].rgbGreen;
				ucBlue  = pPalette[pTemp[x]].rgbBlue;

				//����ϵL=0.3R+0.59G+0.11B���õ�����ֵ
				dwGray  = ( (DWORD) ucRed * 30 +
					        (DWORD) ucGreen * 59 +
					        (DWORD) ucBlue * 11 ) / 100;
				dwGray &= 0x000000ff;

				//����ֱ��ͼ����
				pBuffer[dwGray]++;
				//��ɫֱ��ͼ����
				pBuffer[256 + ucRed]++;
				//��ɫֱ��ͼ����
				pBuffer[512 + ucGreen]++;
				//��ɫֱ��ͼ����
				pBuffer[768 + ucBlue]++;

			}
		}
		break;
	case 16:				//ÿ����λ��Ϊ16
		for( y = nY1; y <= nY2; y++ )
		{
			//����ָ�붨λ��ͼ��������ʼλ��
			pTemp = pData;
			//����ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
			pTemp += ( ( nHeight - 1 - y ) * nWidthBytes );
			//����ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
			pTemp += ( nX1 * 2 );
			for( x = nX1; x <= nX2; x++ )
			{
				//��ȡ��ԭɫ����
				GETRGB555( ucRed, ucGreen, ucBlue, pTemp );

				//����ϵL=0.3R+0.59G+0.11B���õ�����ֵ
				dwGray = ( (DWORD) ucRed * 30 +
					       (DWORD) ucGreen * 59 +
					       (DWORD) ucBlue * 11 ) / 100;
				dwGray &= 0x000000ff;

				//����ֱ��ͼ����
				pBuffer[dwGray]++;
				//��ɫֱ��ͼ����
				pBuffer[256 + ucRed]++;
				//��ɫֱ��ͼ����
				pBuffer[512 + ucGreen]++;
				//��ɫֱ��ͼ����
				pBuffer[768 + ucBlue]++;

				//����ָ���2
				pTemp += 2;
			}
		}
		break;
	case 24:				//ÿ����λ��Ϊ24
		for( y = nY1; y < nY2; y++ )
		{
			//����ָ�붨λ��ͼ��������ʼλ��
			pTemp = pData;
			//����ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
			pTemp += ( ( nHeight - 1 - y ) * nWidthBytes );
			//����ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
			pTemp += ( nX1 * 3 );

			for( x=nX1; x<=nX2; x++ )
			{
				//��ȡ������ɫ����ԭɫ��
				ucRed   = pTemp[x * 3 + 2];
				ucGreen = pTemp[x * 3 + 1];
				ucBlue  = pTemp[x * 3];

				//����ϵL=0.3R+0.59G+0.11B���õ�����ֵ
				dwGray  = ( (DWORD) ucRed * 30 +
					        (DWORD) ucGreen * 59 +
					        (DWORD) ucBlue * 11 ) / 100;
				dwGray &= 0x000000ff;

				//����ֱ��ͼ����
				pBuffer[dwGray]++;
				//��ɫֱ��ͼ����
				pBuffer[256 + ucRed]++;
				//��ɫֱ��ͼ����
				pBuffer[512 + ucGreen]++;
				//��ɫֱ��ͼ����
				pBuffer[768 + ucBlue]++;

				//����ָ���3
				pTemp += 3;
			}
		}
		break;
	case 32:				//ÿ����λ��Ϊ24
		for( y = nY1; y <= nY2; y++ )
		{
			//����ָ�붨λ��ͼ��������ʼλ��
			pTemp = pData;
			//����ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
			pTemp += ( ( nHeight - 1 - y ) * nWidthBytes );
			//����ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
			pTemp += ( nX1 * 4 );

			for( x = nX1; x <= nX2; x++ )
			{
				//��ȡ������ɫ����ԭɫ��
				GETRGB888( ucRed, ucGreen, ucBlue, pTemp );

				//����ϵL=0.3R+0.59G+0.11B���õ�����ֵ
				dwGray = ( (DWORD) ucRed * 30 +
					       (DWORD) ucGreen * 59 +
					       (DWORD) ucBlue * 11 ) / 100;

				dwGray &= 0x000000ff;

				//����ֱ��ͼ����
				pBuffer[dwGray]++;
				//��ɫֱ��ͼ����
				pBuffer[256 + ucRed]++;
				//��ɫֱ��ͼ����
				pBuffer[512 + ucGreen]++;
				//��ɫֱ��ͼ����
				pBuffer[768 + ucBlue]++;

				pTemp += 4;
			}
		}
		break;
	}	
	return( pBuffer );
}


BOOL CPretreatment::LinerTran(float fa, float fb,
									 int nX1,
									 int nY1,
									 int nX2,
									 int nY2,
									 CDibObject *pDibObject)
{
	//CDibObject����ָ��
	if( pDibObject != NULL ) m_pDibObject =(CPretreatment *) pDibObject;
	//��δָ�� CDibObject ����ָ�뷵��FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	BOOL bLessThanHalf, bCompleteImage;	

	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2,
										&bCompleteImage, 
										&bLessThanHalf );
	//�������
	unsigned char *pBuffer, *pBits, *pTemp;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors, x, y, i;
	
	//pBuffer: ���λͼ����ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( FALSE );
	
	//pBIH�����λͼ��Ϣͷ��ַ
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors����õ�ɫ���е���ɫ����ͼ��Ϊ16λɫ�����ʱΪ0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette����õ�ɫ�����ݵ�ַ
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits�����λͼ���ݵ�ַ
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
		nNumColors*sizeof(RGBQUAD)];
	
	LOGPALETTE *pLogPal = m_pDibObject->GetLogPal();
	CPalette *_pPalette = m_pDibObject->GetPalette();

	unsigned char *pMap = new unsigned char[256];
	
	// ����Ҷ�ӳ���
	for (i = 0; i <255; i++)
	{
		float fTemp = fa * (float)(i) + fb;
		// �ж��Ƿ񳬳���Χ
		if (fTemp >= 255)
		{
			// ֱ�Ӹ�ֵΪ255
			pMap[i] = 255;
		}
		else if (fTemp < 0)
		{
			// ֱ�Ӹ�ֵΪ0
			pMap[i] = 0;
		}
		else
		{
			// ��������
			pMap[i] = (unsigned char) (fTemp + 0.5);
		}
	}
	pMap[255]=255;		
	switch( m_pDibObject->GetNumBits() )
	{
	case 1:
		for( y=nY1; y<=nY2; y++ )
		{
			unsigned char Mask;
			pTemp = pBits;
			pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y ) 
				* nWidthBytes ) + ( nX1 / 8 ) );
			for( x=nX1; x<=nX2; x++ )
			{
				Mask = 0x80 >> ( x & 7 );
				if( pTemp[0] & Mask ) pTemp[0] &= ( Mask ^ 0xff );
				else pTemp[0] |= Mask;
				if( ( x & 7 ) == 7 ) pTemp++;
			}
		}
		break;
	case 4:
		if( bCompleteImage )
		{
			_pPalette->DeleteObject();
			for( i=0; i<nNumColors; i++ )
			{
				pPalette[i].rgbRed = pMap[pPalette[i].rgbRed];
				pPalette[i].rgbGreen = pMap[pPalette[i].rgbGreen];
				pPalette[i].rgbBlue = pMap[pPalette[i].rgbBlue];
			}
			LOGPALETTE *pLogPal = m_pDibObject->CreateLogPalette( 
								pPalette, nNumColors );
			if( pLogPal != NULL )
			{
				_pPalette->CreatePalette( pLogPal );
				delete [] pLogPal;
			}
		}
		else
		{
			unsigned char ucRed, ucGreen, ucBlue, Data;
			for( y=nY1; y<=nY2; y++ )
			{
				pTemp = pBits;
				pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y )
					* nWidthBytes ) + ( nX1 / 2 ) );
				for( x=nX1; x<=nX2; x++ )
				{
					Data = *pTemp;
					if( !( x & 1 ) ) Data >>= 4;
					else Data &= 0x0f;
					ucRed = pPalette[Data].rgbRed;
					ucGreen = pPalette[Data].rgbGreen;
					ucBlue = pPalette[Data].rgbBlue;
					ucRed = pMap[ucRed];
					ucGreen = pMap[ucGreen];
					ucBlue = pMap[ucBlue];
					Data = (unsigned char) m_pDibObject->GetNearestIndex( ucRed, ucGreen, 
						ucBlue, pPalette, nNumColors );
					if( !( x & 1 ) )
					{
						Data <<= 4;
						(*pTemp) &= 0x0f;
						(*pTemp) |= Data;
					}
					else
					{
						(*pTemp) &= 0xf0;
						(*pTemp) |= Data;
						pTemp++;
					}
				}
			}
		}
		break;
	case 8:
		if(IsGrayImage())
		{
			for( y=nY1; y<=nY2; y++ )
			{
				pTemp = pBits;
				pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y )
					* nWidthBytes ) + nX1 );
				for( x=nX1; x<=nX2; x++ )
				{
					pTemp[x] = pMap[pTemp[x]];
				}
			}
		}
		else if( bCompleteImage )
		{
			_pPalette->DeleteObject();
			for( i=0; i<nNumColors; i++ )
			{
				pPalette[i].rgbRed = pMap[pPalette[i].rgbRed];
				pPalette[i].rgbGreen = pMap[pPalette[i].rgbGreen];
				pPalette[i].rgbBlue = pMap[pPalette[i].rgbBlue];
			}
			LOGPALETTE *pLogPal = m_pDibObject->CreateLogPalette( pPalette, nNumColors );
			if( pLogPal != NULL )
			{
				_pPalette->CreatePalette( pLogPal );
				delete [] pLogPal;
			}
		}
		else
		{
			unsigned char ucRed, ucGreen, ucBlue, Data;
			for( y=nY1; y<=nY2; y++ )
			{
				pTemp = pBits;
				pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y )
					* nWidthBytes ) + nX1 );
				for( x=nX1; x<=nX2; x++ )
				{
					Data = *pTemp;
					ucRed = pPalette[Data].rgbRed;
					ucGreen = pPalette[Data].rgbGreen;
					ucBlue = pPalette[Data].rgbBlue;
					ucRed = pMap[ucRed];
					ucGreen = pMap[ucGreen];
					ucBlue = pMap[ucBlue];
					Data = (unsigned char) m_pDibObject->GetNearestIndex( ucRed, 
						ucGreen, ucBlue, pPalette, nNumColors );
					*pTemp++ = Data;
				}
			}
		}
		break;
	case 16:
		unsigned char ucRed, ucGreen, ucBlue;
		for( y=0; y< nY2; y++ )
		{
			pTemp = pBits;
			pTemp += ( ( m_pDibObject->GetHeight() - 1 - y ) 
						* nWidthBytes );
			pTemp += ( nX1 * 2 );
			for( x=nX1; x<=nX2; x++ )
			{
				GETRGB555( ucRed, ucGreen, ucBlue, pTemp );
				ucRed = pMap[ucRed];
				ucGreen = pMap[ucGreen];
				ucBlue = pMap[ucBlue];
				PUTRGB555( ucRed, ucGreen, ucBlue, pTemp );
				pTemp += 2;
			}
		}
		break;
	case 24:
		for( y=nY1; y<=nY2; y++ )
		{
			pTemp = pBits;
			pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y )
				* nWidthBytes ) + ( nX1 * 3 ) );
			for( x=nX1; x<=nX2; x++ )
			{
				pTemp[0] = pMap[pTemp[0]];
				pTemp[1] = pMap[pTemp[1]];
				pTemp[2] = pMap[pTemp[2]];
				pTemp += 3;
			}
		}
		break;
	case 32:
		for( y=nY1; y<=nY2; y++ )
		{
			pTemp = pBits;
			pTemp += ( ( ( m_pDibObject->GetHeight() - 1 - y ) 
				* nWidthBytes ) + ( nX1 * 4 ) );
			for( x=nX1; x<=nX2; x++ )
			{
				pTemp[0] = pMap[pTemp[0]];
				pTemp[1] = pMap[pTemp[1]];
				pTemp[2] = pMap[pTemp[2]];
				pTemp[3] = pMap[pTemp[3]];
				pTemp += 4;
			}
		}
		break;
	}
	
	delete [] pMap;
	
	::GlobalUnlock( m_pDibObject->GetDib() );
	
	m_pDibObject->m_nLastError = IMAGELIB_SUCCESS;
	
	// ����
	return TRUE;
}

BOOL CPretreatment::MedianFilter(int nType, int nTempW, int nTempH, int nTempXc, int nTempYc, int nX1, int nY1, int nX2, int nY2)
{
	//ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);
	
	//��1λ��4λͼ�����κβ���ֱ�ӷ���
	if(m_pDibObject->GetNumBits() != 8)
	{
		// ��ʾ�û��������ô���
		AfxMessageBox("ֻ֧��8λͼ��,���������룡");
		
		// ����
		return( FALSE );
	}
	
	//���������
	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2 );
	
	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	
	//�Ա߽����ز�������
	if( nX1 < nTempXc ) nX1 = nTempXc;
	if( nY1 < nTempYc ) nY1 = nTempYc;
	if( nX2 >= nWidth - nTempW + nTempXc) nX2 = nWidth - nTempW + nTempXc + 1;
	if( nY2 >= nHeight - nTempH + nTempYc) nY2 = nHeight - nTempH + nTempYc + 1;
	
	//�������
	unsigned char Data;
	//������ͼ�����ݲ����йصı���
	unsigned char *pOldBuffer, *pNewBuffer, 
				  *pOldBits, *pNewBits, 
				  *pOldTemp, *pNewTemp,
				  *pNeighborTemp;

	BITMAPFILEHEADER *pOldBFH, *pNewBFH;

	BITMAPINFOHEADER *pOldBIH, *pNewBIH;

	RGBQUAD *pOldPalette, *pNewPalette;

	int nWidthBytes, nNumColors, x, y, i, j;
	
	//��ȡԭͼ��ָ��
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );
	
	//ԭͼ���ļ�ͷ
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//ԭͼ����Ϣͷ
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//ԭͼ����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//ԭͼ���ɫ��ָ��
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//ԭͼ������ָ��
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
											 + nNumColors * sizeof(RGBQUAD)];
	
	DWORD dwNewSize;
	HGLOBAL hNewDib;
	
	//��ͼ���ļ���С�����ֽ�Ϊ��λ��
	dwNewSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + 
				nNumColors * sizeof( RGBQUAD ) + nWidthBytes * nHeight;

	//Ϊ��ͼ������ڴ�
	hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );

	//�ڴ����ʧ��
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
	
	//��ͼ���ļ�ͷ
	pNewBFH = (BITMAPFILEHEADER *) pNewBuffer;
	//��ͼ����Ϣͷ
	pNewBIH = (BITMAPINFOHEADER *) &pNewBuffer[sizeof(BITMAPFILEHEADER)];
	//��ͼ���ɫ��ָ��
	pNewPalette = (RGBQUAD *) &pNewBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//��ͼ������ָ��
	pNewBits = (unsigned char *) &pNewBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
											 + nNumColors*sizeof(RGBQUAD)];
	
	//����ԭͼ�����ݵ���ͼ��

	//�����ļ�ͷ
	memcpy(pNewBFH, pOldBFH, sizeof(BITMAPFILEHEADER));
	
	//������Ϣͷ
	memcpy(pNewBIH, pOldBIH, sizeof(BITMAPINFOHEADER));
	
	//���Ƶ�ɫ��
	for(i = 0; i < nNumColors; i++ ) pNewPalette[i] = pOldPalette[i];
	
	//����ͼ������
	memcpy( pNewBits, pOldBits, nWidthBytes * nHeight );

	//����ģ��ĳߴ�
	int nSize = nTempW * nTempH;
	
	//���������ֵ�õ���ʱ����
	unsigned char *pGray = new unsigned char [nTempW * nTempH];
	if(pGray == NULL) return( NULL );
	memset(pGray, 0, (nSize) * sizeof(unsigned char));

	unsigned char nData;

	//8λͼ��
		//��λ��
		for(y = nY1; y < nY2; y++ )
		{
			//ԭͼ������ָ�붨λ����ʼλ��
			pOldTemp = pOldBits;
			//ԭͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
			pOldTemp += (nHeight - 1 - y) * nWidthBytes;
			//ԭͼ������ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
			pOldTemp += nX1;
			
			//��ͼ������ָ�붨λ����ʼλ��
			pNewTemp = pNewBits;
			//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
			pNewTemp += (nHeight - 1 - y) * nWidthBytes;
			//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
			pNewTemp += nX1;
			
			//��λ��
			for(x = nX1; x < nX2; x++)
			{
				//ȡ��ԭͼ��nTempW��nTempH�����ڵ�����ֵ
				for (i = 0; i < nTempH; i++)
				{
					for (j = 0; j < nTempW; j++)
					{
						//����ָ��ָ��ǰ����
						pNeighborTemp = pOldTemp;
						//����ָ��ָ��ԭͼ���y - nTempYc + i�С�
						pNeighborTemp += nWidthBytes * (nTempYc - i);
						//��x - nTempXc + j�����ص�ָ��
						pNeighborTemp += (j - nTempXc);
						
						//��ȡ����ֵ
						Data = *pNeighborTemp;

						pGray[i*nTempW+j] = ( pOldPalette[Data].rgbRed * 30 +
										pOldPalette[Data].rgbGreen * 59 +
										pOldPalette[Data].rgbBlue * 11 ) / 100;
					}
				}

				//ָ���������㷨����ʶ����ð�������㷨��
				if(nType < 0 || nType > 1) nType =0;

				switch( nType )
				{
				case 0: //ð������
					{
						int nTemp;
						int nLast = nSize - 1;
						bool bSorted = true;
						
						do
						{
							bSorted = true;
							for (int i = 0; i < nLast; i++)
							{
								if (pGray[i] > pGray[i + 1])
								{	
									nTemp =pGray[i];
									pGray[i] = pGray[i + 1];
									pGray[i + 1] = nTemp;
									bSorted = false;
								}
								
							}
							nLast--;
						} while (!bSorted);
						
					}
					nData = pGray[nSize / 2];
					break;
				
				case 1://���������㷨
					{
						int i;
						int nHisto[256];
						for( i = 0; i < 256; i++) nHisto[ i ] = 0;
						//ͳ����ͬ�Ҷ�ֵ���ֵĴ���				
						for( i = 0; i < nSize; i++)
						{    
							nHisto[(int)pGray[i]]++;
						}

						//ȡֱ��ͼ����ֵ
						int nCount = 0;
						for( i = 0; i < 256; i++)
						{ 
							//���ֱ��ͼ����ͬ�Ҷ�ֵ���ִ������ۼӺ�
							nCount += nHisto[i];
							if(nCount >= (int)(nSize / 2 + 1))
							{  
								nData = i;
								break;
							}
						}			
					}
					break;
					
				}

				//��������������ͼ��
				*pNewTemp = (unsigned char) m_pDibObject->GetNearestIndex( 
					pNewPalette[nData].rgbRed, pNewPalette[nData].rgbGreen, 
					pNewPalette[nData].rgbBlue,	pNewPalette, nNumColors );

				//�¾�ͼ������ָ���1
				pOldTemp++;
				pNewTemp++;
				
			}
		}
	
	//�ͷ��ڴ�
	delete [] pGray;

	::GlobalUnlock( m_pDibObject->GetDib() );
    ::GlobalFree( m_pDibObject->GetDib() );
	::GlobalUnlock( hNewDib );
    m_pDibObject->SetDib( hNewDib );
    return(TRUE);
}

BOOL CPretreatment::TemplateSmooth(float *fpArray, float fCoef, int nTempW, int nTempH, int nTempXc, int nTempYc, int nX1, int nY1, int nX2, int nY2)
{
	//ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);

	//ֻ����8λͼ��
	if(m_pDibObject->GetNumBits() != 8) 
	{
		AfxMessageBox("Ŀǰֻ֧��8λ�Ҷ�ͼ��Ĵ���");
		return( FALSE );
	}

	//���������
	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2 );

	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();

	//�Ա߽����ز�������
	if( nX1 < nTempXc ) nX1 = nTempXc;
	if( nY1 < nTempYc ) nY1 = nTempYc;
	if( nX2 > nWidth - nTempW + nTempXc + 1) nX2 = nWidth - nTempW + nTempXc + 1;
	if( nY2 > nHeight - nTempH + nTempYc + 1) nY2 = nHeight - nTempH + nTempYc + 1;

	//�������
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNewWidthBytes, nNumColors;
	DWORD dwNewSize;

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

	dwNewSize = nNewWidthBytes * nHeight;

	//�������ڴ洢ɫ��ֵ����ʱ����
	double *pHue = new double [dwNewSize];
	if(pHue == NULL) return(FALSE);
	memset(pHue, 0, dwNewSize * sizeof(double));

	//�������ڴ洢���Ͷ�ֵ����ʱ����
	double *pSaturation = new double [dwNewSize];
	if(pSaturation == NULL) return(FALSE);
	memset(pSaturation, 0, dwNewSize * sizeof(double));

	//�������ڴ洢����ֵ����ʱ����
	unsigned char *pLight = new unsigned char [dwNewSize];
	if(pLight == NULL) return(FALSE);
	memset(pLight, 0, dwNewSize * sizeof(unsigned char));

	float *fTempArray;

	//Ĭ��Ϊ3��3�ĸ�ͨ�˲���1ģ��
	static float fpDefaultArray[] = {1.0, 1.0, 1.0, 
							         1.0, 1.0, 1.0,
							         1.0, 1.0, 1.0};

	//û�д���ģ�壬��Ĭ��ģ��
	if( fpArray == NULL ) 
	{
		fTempArray = fpDefaultArray;
		fCoef = 9;
	}
	//���ô����ģ��
	else fTempArray = fpArray;

	//����Template��������


//��ȡͼ��߶ȣ�������Ϊ��λ��
	nHeight = m_pDibObject->GetHeight();

	//�������
	unsigned char Data;

	//������ͼ�����ݲ����йصı���
	unsigned char *pOldBits, *pNewBits, 
				  *pOldTemp, *pNewTemp,
				  *pNeighborTemp;
	int  x, y, i, j;

	//ԭͼ����ɫ��
	nNumColors = m_pDibObject->GetNumColors();

	//ԭͼ������ָ��
	pOldBits = pBits;

	HGLOBAL hNewDib;

	//��ͼ���ļ���С�����ֽ�Ϊ��λ��
	dwNewSize = nWidthBytes * nHeight;
	//Ϊ��ͼ������ڴ�
	hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	//�ڴ����ʧ��
	if( hNewDib == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ��ָ��
	pNewBits = (unsigned char *) ::GlobalLock( hNewDib );
	if( pNewBits == NULL )
	{
		::GlobalFree( hNewDib );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		return( FALSE );
	}

	//����ͼ������
	memcpy( pNewBits, pOldBits, nWidthBytes * nHeight );

	//�������������õ���ʱ����
	float *pGray = new float [nTempW * nTempH];
	if(pGray == NULL) return( NULL );
	memset(pGray, 0, (nTempW * nTempH) * sizeof(float));
	
	//��λ��
	for(y = nY1; y < nY2; y++ )
	{
		//ԭͼ������ָ�붨λ����ʼλ��
		pOldTemp = pOldBits;
		//ԭͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
		pOldTemp += (nHeight -1 - y) * nWidthBytes;
		//ԭͼ������ָ�붨λ��ͼ������ÿ�е���ʼnX1-1λ��
		pOldTemp += nX1;
		
		//��ͼ������ָ�붨λ����ʼλ��
		pNewTemp = pNewBits;
		//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
		pNewTemp += (nHeight -1 - y) * nWidthBytes;
		//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
		pNewTemp += nX1;
		
		//��λ��
		for(x = nX1; x < nX2; x++)
		{
			//ȡ��ԭͼ��nTempW��nTempH�����ڵ�����ֵ
			for (i = 0; i < nTempH; i++)
			{
				for (j = 0; j < nTempW; j++)
				{
					//����ָ��ָ��ǰ����
					pNeighborTemp = pOldTemp;
					//����ָ��ָ��ԭͼ���y - nTempYc + i�С�
					pNeighborTemp += nWidthBytes * (nTempYc - i);
					//��x - nTempXc + j�����ص�ָ��
					pNeighborTemp += (j - nTempXc);
					
					//��ȡ����ֵ
					Data = *pNeighborTemp;

					pGray[i * nTempW + j] = (float)Data;
				}
			}

			//����˳ߴ�
			int nSize = nTempW * nTempH;
			
			//�Ժ�ɫ���������
		//	Convolution(pGray, fpArray, fCoef, nSize, &Data);

			float Sum = 0, fResult;

			//������
			for(i = 0; i < nSize; i++)
			{
				Sum += pGray[i] * fpArray[i];
			}

			//ִ�о����Ľ��
			fResult = Sum / fCoef;

			//�����ֵ
			fResult = (float)fabs(fResult);
			
			//�ж��Ƿ񳬹�255
			if(fResult > 255.0 )
			{
				// ֱ�Ӹ�ֵΪ255
				fResult = 255.0;
			}
			
			//�Ծ������������룬��ת����unsigned char������Ϊ��󷵻ؽ��
			Data = (unsigned char) (fResult + 0.5);		
			
			//��������������ͼ��
			*pNewTemp = Data;
			
			//�¾�ͼ������ָ���1
			pOldTemp++;
			pNewTemp++;
			
		}
	}
	
	//�ͷ��ڴ�
	delete [] pGray;

	//����ͼ������
	memcpy( pOldBits, pNewBits, nWidthBytes * nHeight );

	::GlobalUnlock( hNewDib );
	::GlobalFree( hNewDib );

	//�ڴ����
	::GlobalUnlock(m_pDibObject->GetDib());

	return( TRUE );


}

int CPretreatment::ThresholdAuto(CDibObject *pDibObject)
{
	if( pDibObject != NULL ) m_pDibObject =(CPretreatment *) pDibObject;
	if( m_pDibObject == NULL ) return( -1 );	//��δָ�� CDibObject ����ָ�뷵��FALSE

	unsigned char *pBuffer, *pBits;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors, i;

	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( -1 );

	//pBIH�����λͼ��Ϣͷ��ַ
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors����õ�ɫ���е���ɫ����ͼ��Ϊ16λɫ�����ʱΪ0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette����õ�ɫ�����ݵ�ַ
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits�����λͼ���ݵ�ַ
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
										nNumColors*sizeof(RGBQUAD)];
	if (pBIH->biBitCount != 8) return (-1);
	//��ȡ�Ҷ�ͼ��Ŀ�Ⱥ͸߶ȣ������ر�ʾ��
	int nWidth = pBIH->biWidth;
	int nHeight = pBIH->biHeight;
	
	int *pHistogram = GetHistogram();
	
	long MaxPiex1 = 0;
	long MaxPiex2 = 0;
	unsigned char MaxGray1, MaxGray2;

	for(i = 0; i < 256; i++)
	{
		if ( pHistogram[i] > MaxPiex1)
		{
			MaxPiex1 = pHistogram[i];
			MaxGray1 = i;
		}
	}

	for(i = 0; i < 256; i++)
	{
		if ( pHistogram[i] > MaxPiex2 && pHistogram[i] < MaxPiex1)
		{
			MaxPiex2 = pHistogram[i];
			MaxGray2 = i;
		}
	}

	unsigned char Temp;
	
	if(MaxGray1 > MaxGray2)
	{
		Temp = MaxGray1;
		MaxGray1 = MaxGray2;
		MaxGray2 = MaxGray1;
	}

	int Thresh;

	long MinPiex = nWidth * nHeight;

	for(i = MaxGray1; i <= MaxGray2; i++)
	{
		if(pHistogram[i] < MinPiex) Thresh = i;
	}

	delete [] pHistogram;

	return(Thresh);
}

BOOL CPretreatment::Threshold(int Thresh, int Mode, BOOL bSetPalette, CDibObject *pDibObject)
{
	//CDibObject����ָ��
	if( pDibObject != NULL ) m_pDibObject =(CPretreatment *) pDibObject;
	//��δָ�� CDibObject ����ָ�뷵��FALSE
	if( m_pDibObject == NULL ) return( -1 );
	if( !IsGrayImage() ) return( -1 );

	unsigned char *pBuffer, *pBits, *pTemp;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors, i;

	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( -1 );

	//pBIH�����λͼ��Ϣͷ��ַ
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors����õ�ɫ���е���ɫ����ͼ��Ϊ16λɫ�����ʱΪ0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette����õ�ɫ�����ݵ�ַ
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits�����λͼ���ݵ�ַ
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
										nNumColors*sizeof(RGBQUAD)];

	//��ȡ�Ҷ�ͼ��Ŀ�Ⱥ͸߶ȣ������ر�ʾ��
	int nWidth = pBIH->biWidth;
	int nHeight = pBIH->biHeight;

	// �Ҷ�ӳ���
	unsigned char pMap[256];
	
	for( i = 0; i <= Thresh; i++) pMap[i] = 0;
	for( i = Thresh + 1; i <= 255; i++) pMap[i] =255;

	for ( i = 0; i < nHeight; i ++)
	{
		pTemp = pBits;
		pTemp += i * nWidthBytes;	//λͼ������һ����ʼָ��
		for ( int j = 0; j < nWidth; j ++)
		{
			pTemp[j] = pMap[pTemp[j]];
		}
	}
	m_pDibObject->ProcessPalette();
		if( bSetPalette && m_pDibObject->m_nPaletteBytes > 0 )
		{
			CWindowDC WindowDC( NULL );
			m_pDibObject->SetPalette( &WindowDC );
		}

	::GlobalUnlock( m_pDibObject->GetDib() );

	m_pDibObject->m_nLastError = IMAGELIB_SUCCESS;


	return TRUE;
}
////////////////////////////////////////////////////////////////////////
//int FindThresh( CDibObject *pDibObject )   
//----------------------------------------------------------------------
//�������ܣ��������Դ����CDibObject�����е�ͼ�����б������ȷ����ֵ��
//			�������ж��������������ڵ���255��ͼ�������ֱ��ͼ������Ϊ��
//			����ֵ������Դ��������������С��255��ͼ��ֱ�Ӳ���ͼ��������
//			Ϊ�����ֵ������Դ�������ɴ����߼����ٶȡ�
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject, Ĭ��ΪNULL��
//----------------------------------------------------------------------
//��    �أ�int ��ֵ
//----------------------------------------------------------------------
//ע��		ֻ��8λ�Ҷ�ͼ����Ч
////////////////////////////////////////////////////////////////////////
int CPretreatment::ThresholdJustAnlyze(CDibObject *pDibObject)
{
	//CDibObject����ָ��
	if( pDibObject != NULL ) m_pDibObject =(CPretreatment *) pDibObject;
	//��δָ�� CDibObject ����ָ�뷵��FALSE
	if( m_pDibObject == NULL ) return( -1 );

	unsigned char *pBuffer, *pBits;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors, i;

	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( -1 );

	//pBIH�����λͼ��Ϣͷ��ַ
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors����õ�ɫ���е���ɫ����ͼ��Ϊ16λɫ�����ʱΪ0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette����õ�ɫ�����ݵ�ַ
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits�����λͼ���ݵ�ַ
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
										nNumColors*sizeof(RGBQUAD)];
	if (pBIH->biBitCount != 8) return (-1);

	//��ȡ�Ҷ�ͼ��Ŀ�Ⱥ͸߶ȣ������ر�ʾ��
	int nWidth = pBIH->biWidth;
	int nHeight = pBIH->biHeight;
	
	int *nHistogramBuffer = GetHistogram();
	
	float Sigma[256];
	float Mean1, Mean2;
	
	//Sigma[256]��������
	for( i = 0;i < 256; i++) Sigma[i] = 0.0;
	
	int nLevelLow = 0;
	int nLevelHigh = 0;
	
	//������С�Ҷ�ֵ
	i = 0;
	while(nLevelLow == 0)
	{
		if( nHistogramBuffer[i] != 0 )
		{
			nLevelLow = i;
		}
		i++;
	}

	//�������Ҷ�ֵ
	i = 255;
	while(nLevelHigh == 0)
	{
		if( nHistogramBuffer[i] != 0 )
		{
			nLevelHigh = i;
		}
		i--;
	}

	//thѭ����С����ֵth�����ع�Ϊ��һ�࣬����th�����ع�Ϊ�ڶ���
	//th=0��1��2������nLevel��
	for(int th = nLevelLow; th <= nLevelHigh; th++)   
	{
		long lGrayLow = 0;
		long lGrayHigh = 0;
		long lSumLow = 0;
		long lSumHigh = 0;
		for(i = nLevelLow; i < th; i++)
		{
			lSumLow += nHistogramBuffer[i];
			lGrayLow += i * nHistogramBuffer[i];
		}
		for(i = th; i <= nLevelHigh; i++)
		{
			lSumHigh += nHistogramBuffer[i];
			lGrayHigh += i * nHistogramBuffer[i];
		}
		
		if( lSumLow > 0 )
		{
			Mean1 = (float)lGrayLow / lSumLow;
		}
		else
		{
			Mean1 = 0.0;
		}
		if( lSumHigh > 0)
		{
			Mean2 = (float)lGrayHigh / lSumHigh;
		}
		else
		{
			Mean2 = 0.0;
		}
		
		//��������֮��ķ���
		Sigma[th] = (float)lSumLow * lSumHigh 
			* (Mean1 - Mean2) * (Mean1 - Mean2);
	}
	
	//thѭ���������󷽲�������ֵ
	float MaxSigma = Sigma[nLevelLow];
	int Thresh = 0;
	for( i = nLevelLow + 1; i <= nLevelHigh; i++)
	{
		if(MaxSigma < Sigma[i])
		{
			MaxSigma = Sigma[i];
			Thresh = i;
		}
	}
	
	delete [] nHistogramBuffer;
	return(Thresh);

}
////////////////////////////////////////////////////////////////////////
//BOOL TemplateOperation()   
//----------------------------------------------------------------------
//�������ܣ��ú�����ָ����ģ�壨�����С������ͼ�������������ݽ���ģ���
//			��������nTempHָ��ģ��ĸ߶ȣ�����nTempWָ��ģ��Ŀ�ȣ�����
//			nTempXc��nTempYcָ��ģ�������Ԫ�����꣬����fpArrayΪָ��ģ
//			��Ԫ�������ָ�룬fCoefָ��ģ��ϵ����
//----------------------------------------------------------------------
//����˵����float *fpArray			ָ��ģ�������ָ��
//			float fCoef				ģ��ϵ��
//			int   nTempW			ģ��Ŀ��
//			int   nTempH			ģ��ĸ߶�
//			int   nTempXc			ģ�������Ԫ��X���� ( <= nTempW - 1)
//			int   nTempYc			ģ�������Ԫ��Y���� ( <= nTempH - 1)
//			unsigned char *pData	ͼ������ָ��
//			int nWidthBytes			ͼ���ֽڿ��
//			int	  nX1				����������߽�		
//			int   nY1				���������ϱ߽�
//			int	  nX2				���������ұ߽�
//			int	  nY2				���������±߽�
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�ʱ����TRUE��ʧ��ʱ����FALSE��
//----------------------------------------------------------------------
//ע    �⣺�˺�������Ϊ�����ͣ�ֻ����CAreaPro����ʹ�á�
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPretreatment::TemplateOperation(float *fpArray, float fCoef, 
						int nTempW, int nTempH, int nTempXc, int nTempYc,
						unsigned char *pData, int nWidthBytes, 
						int nX1, int nY1, int nX2, int nY2)
{
	//��ȡͼ��߶ȣ�������Ϊ��λ��
	int nHeight = m_pDibObject->GetHeight();

	//�������
	unsigned char Data;

	//������ͼ�����ݲ����йصı���
	unsigned char *pOldBits, *pNewBits, 
				  *pOldTemp, *pNewTemp,
				  *pNeighborTemp;
	int nNumColors, x, y, i, j;
	DWORD dwNewSize;

	//ԭͼ����ɫ��
	nNumColors = m_pDibObject->GetNumColors();

	//ԭͼ������ָ��
	pOldBits = pData;

	HGLOBAL hNewDib;

	//��ͼ���ļ���С�����ֽ�Ϊ��λ��
	dwNewSize = nWidthBytes * nHeight;
	//Ϊ��ͼ������ڴ�
	hNewDib = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	//�ڴ����ʧ��
	if( hNewDib == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ��ָ��
	pNewBits = (unsigned char *) ::GlobalLock( hNewDib );
	if( pNewBits == NULL )
	{
		::GlobalFree( hNewDib );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		return( FALSE );
	}

	//����ͼ������
	memcpy( pNewBits, pOldBits, nWidthBytes * nHeight );

	//�������������õ���ʱ����
	float *pGray = new float [nTempW * nTempH];
	if(pGray == NULL) return( NULL );
	memset(pGray, 0, (nTempW * nTempH) * sizeof(float));
	
	//��λ��
	for(y = nY1; y < nY2; y++ )
	{
		//ԭͼ������ָ�붨λ����ʼλ��
		pOldTemp = pOldBits;
		//ԭͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
		pOldTemp += (nHeight -1 - y) * nWidthBytes;
		//ԭͼ������ָ�붨λ��ͼ������ÿ�е���ʼnX1-1λ��
		pOldTemp += nX1;
		
		//��ͼ������ָ�붨λ����ʼλ��
		pNewTemp = pNewBits;
		//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
		pNewTemp += (nHeight -1 - y) * nWidthBytes;
		//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
		pNewTemp += nX1;
		
		//��λ��
		for(x = nX1; x < nX2; x++)
		{
			//ȡ��ԭͼ��nTempW��nTempH�����ڵ�����ֵ
			for (i = 0; i < nTempH; i++)
			{
				for (j = 0; j < nTempW; j++)
				{
					//����ָ��ָ��ǰ����
					pNeighborTemp = pOldTemp;
					//����ָ��ָ��ԭͼ���y - nTempYc + i�С�
					pNeighborTemp += nWidthBytes * (nTempYc - i);
					//��x - nTempXc + j�����ص�ָ��
					pNeighborTemp += (j - nTempXc);
					
					//��ȡ����ֵ
					Data = *pNeighborTemp;

					pGray[i * nTempW + j] = (float)Data;
				}
			}

			//����˳ߴ�
			int nSize = nTempW * nTempH;
			
			//�Ժ�ɫ���������
			Convolution(pGray, fpArray, fCoef, nSize, &Data);
			
			//��������������ͼ��
			*pNewTemp = Data;
			
			//�¾�ͼ������ָ���1
			pOldTemp++;
			pNewTemp++;
			
		}
	}
	
	//�ͷ��ڴ�
	delete [] pGray;

	//����ͼ������
	memcpy( pOldBits, pNewBits, nWidthBytes * nHeight );

	::GlobalUnlock( hNewDib );
	::GlobalFree( hNewDib );
	
	return( TRUE );
}

BOOL CPretreatment::GradSharping(int Threshold, int nX1, int nY1, int nX2, int nY2,  
							CDibObject *pDibObject)
{
	//ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);

	//ֻ����8λͼ��
	if(m_pDibObject->GetNumBits() != 8) 
	{
		AfxMessageBox("Ŀǰֻ֧��8λ�Ҷ�ͼ��Ĵ���");
		return( FALSE );
	}

	//���������
	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2 );

	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();

	int nTempXc = 0;
	int nTempYc = 0;
	int nTempW  = 2;
	int nTempH  = 2;

	//�Ա߽����ز�������
	if( nX1 < nTempXc ) nX1 = nTempXc;
	if( nY1 < nTempYc ) nY1 = nTempYc;
	if( nX2 > nWidth - nTempW + nTempXc + 1) nX2 = nWidth - nTempW + nTempXc + 1;
	if( nY2 > nHeight - nTempH + nTempYc + 1) nY2 = nHeight - nTempH + nTempYc + 1;

	//�������
	unsigned char *pBuffer, *pBits, *pNewBits1, *pNewBits2;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;
	DWORD dwNewSize;

	//���ͼ��ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		                                       m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( NULL );

	//�����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//��õ�ɫ��ָ��
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//���λͼ����ָ��
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		                               + nNumColors * sizeof(RGBQUAD)];

	HGLOBAL hNewDib1, hNewDib2;

	//��ͼ���ļ���С�����ֽ�Ϊ��λ��
	dwNewSize = nWidthBytes * nHeight;
	//Ϊ��ͼ������ڴ�
	hNewDib1 = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	//�ڴ����ʧ��
	if( hNewDib1 == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ��ָ��
	pNewBits1 = (unsigned char *) ::GlobalLock( hNewDib1 );
	if( pNewBits1 == NULL )
	{
		::GlobalFree( hNewDib1 );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		return( FALSE );
	}

	//����ͼ������
	memcpy(pNewBits1, pBits, nWidthBytes * nHeight);

	//Ϊ��ͼ������ڴ�
	hNewDib2 = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwNewSize );
	//�ڴ����ʧ��
	if( hNewDib2 == NULL )
	{
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return( FALSE );
	}

	//��ͼ��ָ��
	pNewBits2 = (unsigned char *) ::GlobalLock( hNewDib2 );
	if( pNewBits2 == NULL )
	{
		::GlobalFree( hNewDib2 );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		return( FALSE );
	}

	//����ͼ������
	memcpy(pNewBits2, pBits, nWidthBytes * nHeight);

	float aTemplate[4];

	//Roberts H1ģ��
	aTemplate[0] =  1.0;
	aTemplate[1] =  0.0;
	aTemplate[2] = -1.0;
	aTemplate[3] =  0.0;

	//����Template��������
	if(!TemplateOperation(aTemplate, 1.0, nTempW, nTempH, nTempXc, nTempYc,
						pNewBits1, nWidthBytes, 
						nX1, nY1, nX2, nY2))
	{
		return(FALSE);
	}

	//Roberts H2ģ��
	aTemplate[0] =  0.0;
	aTemplate[1] =  1.0;
	aTemplate[2] =  0.0;
	aTemplate[3] = -1.0;

	//����Template��������
	if(!TemplateOperation(aTemplate, 1.0, nTempW, nTempH, nTempXc, nTempYc,
						pNewBits2, nWidthBytes, 
						nX1, nY1, nX2, nY2))
	{
		return(FALSE);
	}

	//������ͼ�����ݲ����йصı���
	unsigned char *pOldTemp, *pNewTemp1, *pNewTemp2;
	int x, y;

	DWORD dwTemp;

	switch(m_pDibObject->GetNumBits())
	{
	case 8:			//8λͼ��
		//��λ��
		for(y = nY1; y <= nY2; y++ )
		{
			//ԭͼ������ָ�붨λ����ʼλ��
			pOldTemp = pBits;
			//ԭͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
			pOldTemp += y * nWidthBytes;
			//ԭͼ������ָ�붨λ��ͼ������ÿ�е���ʼnX1-1λ��
			pOldTemp += nX1;

			//��ͼ������ָ�붨λ����ʼλ��
			pNewTemp1 = pNewBits1;
			//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
			pNewTemp1 += y * nWidthBytes;
			//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
			pNewTemp1 += nX1;

			//��ͼ������ָ�붨λ����ʼλ��
			pNewTemp2 = pNewBits2;
			//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼ��λ��
			pNewTemp2 += y * nWidthBytes;
			//��ͼ������ָ�붨λ��ͼ������ÿ�е���ʼnX1λ��
			pNewTemp2 += nX1;

			//��λ��
			for(x = nX1; x <= nX2; x++)
			{
				//����Ŀ��ͼ���е�ǰ�����������ֵ
				dwTemp = (DWORD)(pNewTemp1[x] + pNewTemp2[x]);
				//dwTemp = (DWORD)(sqrt(pNewTemp1[x] * pNewTemp1[x] 
				//					+ pNewTemp2[x] * pNewTemp2[x]));
				//�ж��Ƿ�С����ֵ
				if(dwTemp < 255)
				{
					//�ж��Ƿ������ֵ������С�ڵ�������Ҷ�ֵ����
					if((int)dwTemp >= Threshold)
					{
						pOldTemp[x] = (unsigned char)dwTemp;
					}
				}
				else
				{
					//ֱ�Ӹ�ֵΪ255
					pOldTemp[x] = 255;
				}
			}
		}
		break;
	}

	//�ڴ����
	::GlobalUnlock(m_pDibObject->GetDib());

	//�ͷŲ���ʹ�õ��ڴ�
	::GlobalUnlock( hNewDib1 );
	::GlobalFree( hNewDib1 );
	::GlobalUnlock( hNewDib2 );
	::GlobalFree( hNewDib2 );

	return( TRUE );
}
////////////////////////////////////////////////////////////////////////
//BOOL LapSharping()   
//----------------------------------------------------------------------
//�������ܣ��ú�����ָ����ģ�壨�����С������ָ��m_pDibObject�е�ͼ��
//			�������Laplacian�񻯲���������nTempHָ��ģ��ĸ߶ȣ�����
//			nTempWָ��ģ��Ŀ�ȣ�����nTempXc��nTempYcָ��ģ�������Ԫ��
//			���꣬����fpArrayΪָ��ģ��Ԫ�������ָ�룬fCoefָ��ϵ������
//			��δָ���Ļ�Ĭ��Ϊ-1�����꽫����Ϊͼ��ı�Եֵ�����磺nX1��
//			nY1�ᱻ��Ϊ0��nX2��nY2�ᱻ��Ϊͼ��Ŀ�͸ߡ�������ͼ����е�
//			ͨ�˲�����÷����ǲ������κβ���������ʹ��ԭ�ȴ����
//			CDibObject����ָ�롣
//----------------------------------------------------------------------
//����˵����float *fpArray	ָ��ģ�������ָ��
//			float fCoef		ģ��ϵ��
//			int   nTempW	ģ��Ŀ��
//			int   nTempH	ģ��ĸ߶�
//			int   nTempXc	ģ�������Ԫ��X���� ( <= nTempW - 1)
//			int   nTempYc	ģ�������Ԫ��Y���� ( <= nTempH - 1)
//			int	  nX1		Ĭ��Ϊ-1
//			int   nY1		Ĭ��Ϊ-1
//			int	  nX2		Ĭ��Ϊ-1
//			int	  nY2		Ĭ��Ϊ-1
//----------------------------------------------------------------------
//��    �أ�BOOL
//			�ɹ�ʱ����TRUE��ʧ��ʱ����FALSE��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CPretreatment::LapSharping(float *fpArray, float fCoef, int nTempW, 
						int nTempH, int nTempXc, int nTempYc,
						int nX1, int nY1, int nX2, int nY2)
{
	//ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return(FALSE);

	//ֻ����8λͼ��
	if(m_pDibObject->GetNumBits() != 8) 
	{
		AfxMessageBox("Ŀǰֻ֧��8λ�Ҷ�ͼ��Ĵ���");
		return( FALSE );
	}

	//���������
	m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2 );

	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();

	//�Ա߽����ز�������
	if( nX1 < nTempXc ) nX1 = nTempXc;
	if( nY1 < nTempYc ) nY1 = nTempYc;
	if( nX2 > nWidth - nTempW + nTempXc + 1) nX2 = nWidth - nTempW + nTempXc + 1;
	if( nY2 > nHeight - nTempH + nTempYc + 1) nY2 = nHeight - nTempH + nTempYc + 1;

	//�������
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNewWidthBytes, nNumColors;
	DWORD dwNewSize;

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

	dwNewSize = nNewWidthBytes * nHeight;

	//�������ڴ洢ɫ��ֵ����ʱ����
	double *pHue = new double [dwNewSize];
	if(pHue == NULL) return(FALSE);
	memset(pHue, 0, dwNewSize * sizeof(double));

	//�������ڴ洢���Ͷ�ֵ����ʱ����
	double *pSaturation = new double [dwNewSize];
	if(pSaturation == NULL) return(FALSE);
	memset(pSaturation, 0, dwNewSize * sizeof(double));

	//�������ڴ洢����ֵ����ʱ����
	unsigned char *pLight = new unsigned char [dwNewSize];
	if(pLight == NULL) return(FALSE);
	memset(pLight, 0, dwNewSize * sizeof(unsigned char));

	float *fTempArray;

	//Ĭ��Ϊ3��3�ĸ�ͨ�˲���1ģ��
	static float fpDefaultArray[] = {-1.0, -1.0, -1.0, 
							         -1.0,  9.0, -1.0,
							         -1.0, -1.0, -1.0};

	//û�д���ģ�壬��Ĭ��ģ��
	if( fpArray == NULL ) fTempArray = fpDefaultArray;
	//���ô����ģ��
	else fTempArray = fpArray;

	//����Template��������
	if(!TemplateOperation(fTempArray, fCoef, nTempW, nTempH, nTempXc, nTempYc,
						pBits, nWidthBytes, nX1, nY1, nX2, nY2))
	{
		return(FALSE);
	}

	//�ڴ����
	::GlobalUnlock(m_pDibObject->GetDib());

	return( TRUE );
}
////////////////////////////////////////////////////////////////////////
//BOOL Convolution()   
//----------------------------------------------------------------------
//�������ܣ���������ָ��������������о�����㡣
//----------------------------------------------------------------------
//����˵����float *fpData		��ִ�о��������
//			float *fpKernel		���������
//			float fCoef			���ϵ��
//			int nSize			����ߴ�
//			unsigned char *nResult		������
//----------------------------------------------------------------------
//��    �أ��ޡ�
//----------------------------------------------------------------------
//ע    �⣺�˺�������Ϊ˽���ͣ�ֻ����CAreaPro����ʹ��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CPretreatment::Convolution(float *fpData, float *fpKernel, float fCoef, 
						   int nSize, unsigned char *nResult)
{

	int i;
	float Sum = 0, fResult;

	//������
	for(i = 0; i < nSize; i++)
	{
		Sum += fpData[i] * fpKernel[i];
	}

	//ִ�о����Ľ��
	fResult = Sum / fCoef;

	//�����ֵ
	fResult = (float)fabs(fResult);
	
	//�ж��Ƿ񳬹�255
	if(fResult > 255.0 )
	{
		// ֱ�Ӹ�ֵΪ255
		fResult = 255.0;
	}
	
	//�Ծ������������룬��ת����unsigned char������Ϊ��󷵻ؽ��
	*nResult = (unsigned char) (fResult + 0.5);			
}

BOOL CPretreatment::IsBlackandWhite256(CDibObject *pDibObject)
{
//ʹ�ô����CDibObject����
	if( pDibObject != NULL ) m_pDibObject =(CPretreatment *) pDibObject;
	//��CDibObject����, ����FALSE
	if( m_pDibObject == NULL ) return( FALSE );

	//����8λͼ�񣬱ز���256���Ҷ�ͼ�񣬲�������FALSE
	if( !IsGrayImage() ) return( FALSE );

	unsigned char *pBuffer, *pBits, *pTemp;
	BITMAPINFOHEADER *pBIH;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;

	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
												m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return( -1 );

	//pBIH�����λͼ��Ϣͷ��ַ
	pBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	//nNumColors����õ�ɫ���е���ɫ����ͼ��Ϊ16λɫ�����ʱΪ0
	nNumColors = m_pDibObject->GetNumColors();
	//pPalette����õ�ɫ�����ݵ�ַ
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//pBits�����λͼ���ݵ�ַ
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+
										nNumColors*sizeof(RGBQUAD)];
	//��ȡ�Ҷ�ͼ��Ŀ�Ⱥ͸߶ȣ������ر�ʾ��
	int nWidth = pBIH->biWidth;
	int nHeight = pBIH->biHeight;

	for(int y = 0; y < nHeight; y++)
	{
		pTemp = pBits;
		pTemp += y * nWidthBytes;	//λͼ������һ����ʼָ��
		for(int x = 0; x < nWidth; x++)
		{
			if( pTemp[x] != 0 && pTemp[x] != 255) return(FALSE);			
		}
	}

	return(TRUE);
}
////////////////////////////////////////////////////////////////////////
//void CPretreatment::FillHole(CDibObject *m_pDibObject)
//----------------------------------------------------------------------
//�������ܣ����׶�
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject
//----------------------------------------------------------------------
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CPretreatment::FillHole(CDibObject *m_pDibObject)
{
	//ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return;


	//���������
	//m_pDibObject->NormalizeCoordinates( &nX1, &nY1, &nX2, &nY2 );

	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();

	//�������
	unsigned char *pBuffer, *pBits;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;
	//DWORD dwNewSize;

	//���ͼ��ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		                                       m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return;

	//�����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//��õ�ɫ��ָ��
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//���λͼ����ָ��
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		                               + nNumColors * sizeof(RGBQUAD)];



	//������ͼ�����ݲ����йصı���
	unsigned char *pOldTemp, *pOldBits;
	int i, j,k;

    pOldTemp=pBits;
	//pNewTemp=pNew;
	
	int flag_right=0;
	int flag_left=0;
	int flag_up=0;
	int flag_down=0;

	for(i=nHeight;i>0;i--)
	{
		pOldBits=pOldTemp;
		//ԭͼ������ָ�붨λ��ͼ������ÿ�е�����λ��
		pOldBits =pOldBits + (i - 1 ) * nWidthBytes;
		//pNewBits=pNewTemp;
		//��ͼ������ָ�붨λ��ͼ������ÿ�е�����λ��
		//pNewBits =pNewBits + (i - 1 ) * nWidthBytes;
		
		for(j=0;j<nWidthBytes;j++)
		{
		//��������
			if(*pOldBits==255) 
			{
				for(k=1;k<20;k++)
				{
					if((pOldBits+k)<(pOldBits-j+nWidthBytes) && *(pOldBits+k)==0 ) flag_right=1;
					if((k<j) && (*(pOldBits-k)==0)) flag_left=1;
					if(((pOldBits+(k-1)*nWidthBytes)<pOldTemp+(nHeight-1)*nWidthBytes) && (*(pOldBits+(k-1)*nWidthBytes)==0)) flag_up=1;
					if((pOldBits-(k-1)*nWidthBytes)>pOldTemp && (*(pOldBits-(k-1)*nWidthBytes)==0) ) flag_down=1;
				
					if(flag_right==1 && flag_left==1 && flag_up==1 &&flag_down==1) 
					
					{
						*pOldBits=0;
						//*pNewBits=0;
					}
				}
			}

			flag_right=0;
			flag_left=0;
			flag_up=0;
			flag_down=0;

			//pNewBits++;
			pOldBits++;
		}
	}

	::GlobalUnlock(m_pDibObject->GetDib());
	return;
}
////////////////////////////////////////////////////////////////////////
//void CPretreatment::CountNumber()
//----------------------------------------------------------------------
//�������ܣ�ͳ�Ƹ������������
//----------------------------------------------------------------------
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CPretreatment::CountNumber()
{
	//ͼ��ָ��Ϊ�գ��޷���������
	if(m_pDibObject == NULL) return;

	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pDibObject->GetWidth();
	int nHeight = m_pDibObject->GetHeight();
	//�������
	unsigned char *pBuffer, *pBits,*pTemp;
	RGBQUAD *pPalette;
	int nWidthBytes, nNumColors;

	//���ͼ��ָ��
	pBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes, 
		                                       m_pDibObject->GetNumBits() );
	if( pBuffer == NULL ) return;


	//�����ɫ��
	nNumColors = m_pDibObject->GetNumColors();
	//��õ�ɫ��ָ��
	pPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
	//���λͼ����ָ��
	pBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		                               + nNumColors * sizeof(RGBQUAD)];

	//������Ǿ������ڴ洢���
	int *label1=new int[nWidthBytes*nHeight];
	int *label2=new int[nWidthBytes*nHeight];
	//����ž�������
	memset(label1, 0, nWidthBytes*nHeight*sizeof(int));
	memset(label2, 0, nWidthBytes*nHeight*sizeof(int));

	//�����
	#define label1(ROW,COL) label1[nWidthBytes*ROW+COL]
	#define label2(ROW,COL) label2[nWidthBytes*ROW+COL]
	//CString str;
	//str.Format("%d",count);
	
	int count;
	int i,j,k;
	int left,above;
	pTemp=pBits;
	// ��һ��ɨ�裬ע�����ϵ��£������ҵ�ɨ��˳��
	count=0;
	for(i = 0; i < nHeight; i++)
		for(j = 0; j < nWidthBytes; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			//lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (cyDIB - 1 - i) + j;
			pBits =pTemp + (nHeight-i) * nWidthBytes+j;
			/*if(i==1)
				for(k=0;k<cxDIB;k++)
					TRACE("%d ",label1(0,k));*/
			
			if(*pBits==255)//��ǰ����Ϊ��ɫ������Ϊ0
			 	label1(i,j)=0;  
			else//������ɫ���أ����ж���Χ�����Ƿ񱻱�ŵ�����������Ƿ�������ر��
			{
				if(i==0)//��ͷ�е�������
				{
					if(label1(i,j-1)==0)//�����ߵ�����δ�����								
						label1(i,j)=++count;
					else //��������Ϊ�����������ͬ�ı��
						label1(i,j)=label1(i,j-1);
					continue;
				}
				if(j==0)//��ͷ�е�������
				{
					if(label1(i-1,j)==0)//����ϱߵ�����δ�����
						label1(i,j)=++count;
					else //��������Ϊ���ϱ�������ͬ�ı��
						label1(i,j)=label1(i-1,j);
					continue;
				}

				//�����������ͷ�С�ͷ�е�һ�����ء��Ա��ͼ����д���
				left=label1(i,(j-1));
				above=label1((i-1),j);
	
				//�����ߺ��ϱ߶��Ѿ�������ˣ������߱�Ų�ͬ����ȡ�ϱߵ�ֵ��������ߴ����ر�
				if( left!=0 && above!=0 && left!=above)
				{
					label1(i,j)=above;
					//ͬʱ�ı䱾��֮ǰ�ġ�����������ڽӵĴ�����
					for(k=j-1;k>=0;k--)
					{						
						if(label1(i,k)!=0)
							label1(i,k)=above;
						else   break;
					}
				}
				//�����ߺ��ϱ߶��Ѿ�������ˣ��Ҷ��߱����ͬ������ȡ�ϡ���ı����Ϊ��ǰ���صı�ż���
				else if( left!=0 && above!=0 && left==above)
					label1(i,j)=left;
				
				else if( left!=0) //���ֻ����ߵı�����ˣ���ȡ��ߵı��
					label1(i,j)=left;
				
				else if( above!=0) //���ֻ���ϱߵı�����ˣ���ȡ�ϱߵı��
					label1(i,j)=above;
				
				else if(above==0 && left==0) //�����ߡ��ϱ߶��ǰ�ɫ�����򴴽�һ���±��
					label1(i,j)=++count;
				else ;
			}	
		}
			
	//��һ��ɨ�裬ͳ��ϸ����Ŀ����ͳ��ÿ��ϸ�������
	int temp;
	const int NMAX=250;
	//int area[250];
	//area=new int[250];//area[0]����
	area=new int[NMAX];//area[0]����

	//��1��ʼ����ÿ����ŵ����ؽ�����������
	count=0;
	for(k=1;k<NMAX;k++)
	{
		temp=0;
		for(i = 0; i < nHeight; i++)
			for(j = 0; j < nWidthBytes; j++)
				if(label1(i,j)==k)  temp++;			
		if(temp>50)	 //���С��50�ĵ���ë�̺�����
		{ 
			count++;
			area[count]=temp; 

			for(i = 0; i < nHeight; i++)
				for(j = 0; j < nWidthBytes; j++)
				 	if(label1(i,j)==k)
					 	label2(i,j)=count;				
		}

		 if(temp>1000)
		{
		count ++;
		area[count]=temp/2; 

		count ++;
		area[count]=temp/2; 
		 }
	}

	finalcount=count;//�洢����ͳ�Ƶ�ϸ����Ŀ

	//���ϸ�����������
	CString str,str1;

	str.Format("����ϸ�� %2d ��,\n���� \n",count);
	for(int ii=1;ii<=finalcount;ii++)
	{
		
		str1.Format ("�� %2d ��ϸ�������Ϊ��%4d ",ii,area[ii]);
		if(ii % 3==0)
		{
			str1=str1+"\r\n";
		}
		str.Insert (str.GetLength(),str1);
		}
	AfxMessageBox(str,MB_ICONINFORMATION);

	//�ͷ��ڴ�

	::GlobalUnlock( m_pDibObject->GetDib() );

	return;
}

