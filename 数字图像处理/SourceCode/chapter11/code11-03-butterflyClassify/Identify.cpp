// Identify.cpp: implementation of the CIdentify class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Insects.h"
#include "Identify.h"
#include "GeoTrans.h"     
#include "math.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIdentify::CIdentify()
{
	//��ʼ������
	m_pIndentify=NULL;
	m_antennaPara=0.0;
	m_bellyPara=0.0;
	m_swingPara=0;
	m_antennaFactor=0.0;
	m_bellyFactor=0.0;
}

CIdentify::CIdentify(CDibObject *pDib)
{
	//��ʼ������
	m_pIndentify=(CIdentify *)pDib;
	m_antennaPara=0.0;
	m_bellyPara=0.0;
	m_swingPara=0;
	m_antennaFactor=0.0;
	m_bellyFactor=0.0;
}

CIdentify::~CIdentify()
{
	CDibObject::~CDibObject();
}

BOOL CIdentify::AntennaSep()
{
	/*======ͼ��ָ��Ϊ�գ��޷���������=======*/
	if(m_pIndentify==NULL)
	{
		AfxMessageBox("��δ��ͼ��������������!");
		return FALSE;
	}
	
	/*==��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��===*/
	int nWidth = m_pIndentify->GetWidth();
	int nHeight = m_pIndentify->GetHeight();

	/*==������ͼ�����ݲ����йصı���===*/
	unsigned char *pOldBuffer, *pNewBuffer, *pOldBits, *pNewBits,*pOldTemp, *pNewTemp;

	/*==BMP�ļ��ṹ��������===*/
	BITMAPFILEHEADER *pOldBFH, *pNewBFH;
	BITMAPINFOHEADER *pOldBIH, *pNewBIH;
	RGBQUAD *pOldPalette, *pNewPalette;

	int nWidthBytes, nNumColors, i,pixNum,blank_pix,flag,nums=0;
	
	/*======��ȡԭͼ��ָ��=======*/
	pOldBuffer = (unsigned char *) m_pIndentify->GetDIBPointer( &nWidthBytes, 
												m_pIndentify->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );
	
	//ԭͼ���ļ�ͷ
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//ԭͼ����Ϣͷ
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//ԭͼ����ɫ��
	nNumColors = m_pIndentify->GetNumColors();
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
		m_pIndentify->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pIndentify->GetDib() );
		return( FALSE );
	}
	
	//��ͼ��ָ��
	pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	if( pNewBuffer == NULL )
	{
		::GlobalFree( hNewDib );
		m_pIndentify->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pIndentify->GetDib() );
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

	/************************************************/
	/*================���Ƿָ��==================*/
	/************************************************/

	/*===������������ͼ�����ݵ�����ֵΪ255����Ϊ��ɫ==*/
	memset( pNewBits, 255, nWidthBytes * nHeight);
    pOldTemp=pOldBits;
	pNewTemp=pNewBits;
	
	int top_dos=0,bottom_dos=0;
	float max_width=0.0;

	for(i=nHeight;i>nHeight/2;i--)
	{
		pOldBits=pOldTemp;
		//ԭͼ������ָ�붨λ��ͼ������ÿ�е�����λ��
		pOldBits =pOldBits + (i - 1 ) * nWidthBytes+nWidthBytes/2-1;
		pNewBits=pNewTemp;
		//��ͼ������ָ�붨λ��ͼ������ÿ�е�����λ��
		pNewBits =pNewBits + (i - 1 ) * nWidthBytes+nWidthBytes/2-1;

		//��������
		
		pixNum=0,blank_pix=0,flag=0;
		if(*(pOldBits+1)==255) 
		{
			for(int j=1;j<=nWidthBytes/3;j++)
			{	
				if(*pOldBits<255) 
				{
					pixNum++;
					pNewBits--;
					pOldBits--;
					while(*pOldBits==255 ) 
					{
						blank_pix++;
						pNewBits--;
						pOldBits--;
						if(blank_pix>=2) 
						{
							flag=1;
							break;
						}
					}
					if(flag==1) break;
				}
				else
				{
					pNewBits--;
					pOldBits--;
				}
			}
		}
		if(pixNum<nWidthBytes/20 && flag==1)
		{
			nums++;
			memcpy(pNewBits+1, pOldBits+1,pixNum+blank_pix);
			//���㴥�ǵ������
			if((float)pixNum>max_width) max_width=(float)pixNum;
			if(nums==1) top_dos=i;
			else
				bottom_dos=i;
		}
	}

	/***************�����ǻ�ȡ���ǵ������Ϣ****************/
	float total_pixs=0,part_pixs=0;

	for(i=top_dos;i>(bottom_dos+top_dos)/2+1;i--)
	{	
		pNewBits=pNewTemp;
		pNewBits =pNewBits + (i-1) * nWidthBytes;
		for(int j=0;j<nWidthBytes;j++)
		{
			if(*pNewBits!=255)
				part_pixs++;
			pNewBits++;
		}
	}
	for(i=top_dos;i>bottom_dos+1;i--)
	{	
		pNewBits=pNewTemp;
		pNewBits =pNewBits + (i-1) * nWidthBytes;
		for(int j=0;j<nWidthBytes;j++)
		{
			if(*pNewBits!=255)
				total_pixs++;
			pNewBits++;
		}
	}
	m_antennaPara=part_pixs/total_pixs;
	m_antennaFactor=max_width/nWidth;

	::GlobalUnlock( m_pIndentify->GetDib() );
    ::GlobalFree( m_pIndentify->GetDib() );
	::GlobalUnlock( hNewDib );
    m_pIndentify->SetDib( hNewDib );

    return(TRUE);

}
BOOL CIdentify::BellySep()
{
	//ͼ��ָ��Ϊ�գ��޷���������
	if(m_pIndentify==NULL)
	{
		AfxMessageBox("��δ��ͼ��������������!");
		return FALSE;
	}
	
	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pIndentify->GetWidth();
	int nHeight = m_pIndentify->GetHeight();
	//������ͼ�����ݲ����йصı���
	unsigned char *pOldBuffer, *pNewBuffer, 
				  *pOldBits, *pNewBits,
				  *pOldTemp, *pNewTemp;


	//BMP�ļ��ṹ��������
	BITMAPFILEHEADER *pOldBFH, *pNewBFH;
	BITMAPINFOHEADER *pOldBIH, *pNewBIH;
	RGBQUAD *pOldPalette, *pNewPalette;

	int nWidthBytes, nNumColors, i,flag=0;
	int max=0,maxLength=0,index=0;
	//��ȡԭͼ��ָ��
	pOldBuffer = (unsigned char *) m_pIndentify->GetDIBPointer( &nWidthBytes, 
												m_pIndentify->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );
	
	//ԭͼ���ļ�ͷ
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//ԭͼ����Ϣͷ
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//ԭͼ����ɫ��
	nNumColors = m_pIndentify->GetNumColors();
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
		m_pIndentify->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pIndentify->GetDib() );
		return( FALSE );
	}
	
	//��ͼ��ָ��
	pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	if( pNewBuffer == NULL )
	{
		::GlobalFree( hNewDib );
		m_pIndentify->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pIndentify->GetDib() );
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

	//������������ͼ������Ϊ255����Ϊ��ɫ
	memset( pNewBits, 255, nWidthBytes * nHeight);
    pOldTemp=pOldBits;
	pNewTemp=pNewBits;


	/************************************************/
	/*================ �ָ������==================*/
	/************************************************/

	int right_nums,left_nums,top_dos=0,bottom_dos=0,n1,n2,n_left,n_right,flg1,flg2;

	for(i=0;i<=nHeight;i++)
	{
		pOldBits=pOldTemp;
		//ԭͼ������ָ�붨λ��ͼ������ÿ�е�����λ��
		pOldBits =pOldBits + i * nWidthBytes+ nWidthBytes/2-1;

		pNewBits=pNewTemp;
		//��ͼ������ָ�붨λ��ͼ������ÿ�е�����λ��
		pNewBits =pNewBits + i * nWidthBytes+ nWidthBytes/2-1;
		//��ʼ������
		right_nums=0,left_nums=0,n1=0,n2=0,n_left=0,n_right=0,flg1=0,flg2=0;
		//�����ǰ����Ϊ��ɫ��ɨ�����һ��
		if(*pOldBits==255)
		{
			while(*pOldBits==255)
			{
				pOldBits++;
				pNewBits++;
				n1++;
				while(*pOldBits<255 && n1<nWidthBytes/20)
				{
					memcpy(pNewBits,pOldBits,1);
					pOldBits++;
					pNewBits++;
					if(*pOldBits==255)
					{	
						flg1=1;
						break;
					}
					if(flg1==1) break;
				}
				
			}
			pOldBits=pOldTemp;
			//ԭͼ������ָ�붨λ��ͼ������ÿ�е�����λ��
			pOldBits =pOldBits + i * nWidthBytes+ nWidthBytes/2-1;

			pNewBits=pNewTemp;
			//��ͼ������ָ�붨λ��ͼ������ÿ�е�����λ��
			pNewBits =pNewBits + i * nWidthBytes+ nWidthBytes/2-1;

			while(*pOldBits==255)
			{
				pOldBits--;
				pNewBits--;
				n2++;
				while(*pOldBits<255 && n2<nWidthBytes/20)
				{
					memcpy(pNewBits,pOldBits,1);
					pOldBits--;
					pNewBits--;
					if(*pOldBits==255)
					{	
						flg2=1;
						break;
					}
					if(flg2==1) break;
				}	
			}

		}
		else
		{
			//���ұ߽��д���
			while(*pOldBits<255 && right_nums<nWidthBytes/2)
			{
				right_nums++;
				pNewBits++;
				pOldBits++;

				//������ر߽�
				if(right_nums>nWidthBytes/4) 
				{
					flag=1;
					break;
				}
				//��������Ϊ��ɫ�ط������������˳�
				if(*pOldBits==255)
				{
					memcpy(pNewBits-right_nums-1,pOldBits-right_nums-1,right_nums+1);
					break;
				}
			}

			pOldBits=pOldTemp;
			pOldBits=pOldBits + i * nWidthBytes + nWidthBytes/2-1;

			pNewBits=pNewTemp;
			pNewBits=pNewBits + i * nWidthBytes + nWidthBytes/2-1;

			//����߽��д���
			while(*pOldBits<255 && left_nums<nWidthBytes/2)
			{
				left_nums++;
				pNewBits--;
				pOldBits--;

				if(left_nums>nWidthBytes/4) 
				{
					flag=1;
					break;
				}
				if(*pOldBits==255)
				{
					memcpy(pNewBits,pOldBits,left_nums-1);
					break;
				}
			}
			if(flag==1) break;
		}
	}
	
	/***************�����ǻ�ȡ�����������Ϣ****************/
	float nums=0,width_pixs=0,height_pixs=0,total_pixs=0;

	for(i=1;i<=nHeight;i++)
	{	
		pNewBits=pNewTemp;
		pNewBits =pNewBits + (i-1) * nWidthBytes;
		for(int j=0;j<nWidthBytes;j++)
		{
			if(*pNewBits!=255)
			{
				nums++;
				total_pixs++;
				if(nums==1) 
				{
					bottom_dos=i;
				}
				else
				{
					top_dos=i;		
				}
			}
			pNewBits++;
		}
	}
	
	height_pixs=(float)(top_dos-bottom_dos+1);
	width_pixs=total_pixs/height_pixs;


	m_bellyFactor=m_bellyPara=width_pixs/nWidthBytes;

	::GlobalUnlock( m_pIndentify->GetDib() );
    ::GlobalFree( m_pIndentify->GetDib() );
	::GlobalUnlock( hNewDib );
    m_pIndentify->SetDib( hNewDib );
    return(TRUE);
}


BOOL CIdentify::SwingSep()
{
	//ͼ��ָ��Ϊ�գ��޷���������
	if(m_pIndentify==NULL)
	{
		AfxMessageBox("��δ��ͼ��������������!");
		return FALSE;
	}

	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pIndentify->GetWidth();
	int nHeight = m_pIndentify->GetHeight();
	//������ͼ�����ݲ����йصı���
	unsigned char *pOldBuffer, *pNewBuffer, 
				  *pOldBits, *pNewBits,
				  *pOldTemp, *pNewTemp;


	//BMP�ļ��ṹ��������
	BITMAPFILEHEADER *pOldBFH, *pNewBFH;
	BITMAPINFOHEADER *pOldBIH, *pNewBIH;
	RGBQUAD *pOldPalette, *pNewPalette;

	int nWidthBytes, nNumColors, i;
	int max=0,maxLength=0,index=0;
	//��ȡԭͼ��ָ��
	pOldBuffer = (unsigned char *) m_pIndentify->GetDIBPointer( &nWidthBytes, 
												m_pIndentify->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );
	
	//ԭͼ���ļ�ͷ
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//ԭͼ����Ϣͷ
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//ԭͼ����ɫ��
	nNumColors = m_pIndentify->GetNumColors();
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
		m_pIndentify->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pIndentify->GetDib() );
		return( FALSE );
	}
	
	//��ͼ��ָ��
	pNewBuffer = (unsigned char *) ::GlobalLock( hNewDib );
	if( pNewBuffer == NULL )
	{
		::GlobalFree( hNewDib );
		m_pIndentify->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pIndentify->GetDib() );
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

	//������������ͼ������Ϊ255����Ϊ��ɫ
	memset( pNewBits, 255, nWidthBytes * nHeight);
    pOldTemp=pOldBits;
	pNewTemp=pNewBits;


	int nums,flag=0,depart_point,tempNums,n=0,flg=0,nn=0;
	for(i=0;i<nHeight;i++)
	{
		pOldBits=pOldTemp;
		//ԭͼ������ָ�붨λ��ͼ������ÿ�е�����λ��
		pOldBits =pOldBits + i * nWidthBytes+ nWidthBytes/2-1;

		nums=0;
		if(*pOldBits!=255)
		{
			while(*pOldBits<255 && nums<nWidthBytes/2)
			{

				nums++;
				pOldBits--;
				if(nums>nWidthBytes/4) 
				{
					flag=1;
					depart_point=nWidthBytes/2-tempNums;
					break;

				}
				if(*pOldBits==255)
				{
					tempNums=nums;
					break;
				}
			}  
		}
		
		if(flag==1) break;
	}

	for(i=nHeight;i>0;i--)
	{
		pOldBits=pOldTemp;
		//ԭͼ������ָ�붨λ��ͼ������ÿ�еĿ�ʼλ��
		pOldBits =pOldBits + (i - 1 ) * nWidthBytes;
		pNewBits=pNewTemp;
		//��ͼ������ָ�붨λ��ͼ������ÿ�еĿ�ʼλ��
		pNewBits =pNewBits + (i - 1 ) * nWidthBytes;
		//��������
		max=0;
		for(int j=1;j<depart_point;j++)
		{
			if(*pOldBits<255) 
			{
				memcpy( pNewBits, pOldBits, 1);
				pNewBits++;
				pOldBits++;
				if(*pOldBits==255) break;
			}
			else
			{
				max++;
				pNewBits++;
				pOldBits++;
				if(max>3*depart_point/4) break;
			}

		}
	}

	for(i=depart_point/2;i>0;i--)
	{
		pNewBits=pNewTemp;
		//��ͼ������ָ�붨λ��ͼ������ÿһ�еĿ�ʼλ��
		pNewBits =pNewBits + (nHeight-1)* nWidthBytes+i-1;
		//��������
		int j=0;
		while(j<nHeight)
		{
			if(*pNewBits<255) 
			{
				j++;
				pNewBits=pNewBits-nWidthBytes;
				nn=0;
				while(*pNewBits==255 && j<nHeight-1)
				{
					j++;
					nn++;
					pNewBits=pNewBits-nWidthBytes;
					if(*pNewBits!=255 && nn<=5)
					{
						flg=1;
						index=nHeight-j+nn/2+1;
						break; 
					}
				}
			}
			else
			{
				j++;
				pNewBits=pNewBits-nWidthBytes;
			}
			if(flg==1) break;
		}
	}
	if(flg==1)
	{	
		pNewBits=pNewTemp;
		pNewBits=pNewBits+(index-1)*nWidthBytes;
	    memset( pNewBits, 255, nWidthBytes);
		m_swingPara=2;
	}
	else m_swingPara=1;

	::GlobalUnlock( m_pIndentify->GetDib() );
    ::GlobalFree( m_pIndentify->GetDib() );
	::GlobalUnlock( hNewDib );
    m_pIndentify->SetDib( hNewDib );
    return(TRUE);
}
//��ȡ���ǵ���ز���
float CIdentify::GetAntennaPara()
{
	return m_antennaPara;
}
//��ȡ��������ز���
float CIdentify::GetBellyPara()
{
	return m_bellyPara;
}
//��ȡ������ز���
int CIdentify::GetSwingPara()
{
	return m_swingPara;
}

//��ȡ���ǵı�������
float CIdentify::GetAntennaFactor()
{
	return m_antennaFactor;
}
//��ȡ�����ı�������
float CIdentify::GetBellyFactor()
{
	return m_bellyFactor;
}
/***********��ȡƫ����***********/
float CIdentify::GetEccentricity()
{
	/*======ͼ��ָ��Ϊ�գ��޷���������=======*/
	if(m_pIndentify==NULL)
	{
		AfxMessageBox("��δ��ͼ��������������!");
		return FALSE;
	}
	
	/*==��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��===*/
	int nWidth = m_pIndentify->GetWidth();
	int nHeight = m_pIndentify->GetHeight();

	/*==������ͼ�����ݲ����йصı���===*/
	unsigned char *pOldBuffer, *pOldBits,*pOldTemp;


	int nWidthBytes, nNumColors, i,top=0,bottom=0;
	
	/*======��ȡԭͼ��ָ��=======*/
	pOldBuffer = (unsigned char *) m_pIndentify->GetDIBPointer( &nWidthBytes, 
												m_pIndentify->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );
	
	//ԭͼ����ɫ��
	nNumColors = m_pIndentify->GetNumColors();
	//ԭͼ������ָ��
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
											 + nNumColors * sizeof(RGBQUAD)];
	pOldTemp=pOldBits;
	for(i=nHeight;i>0;i--)
	{
		pOldBits=pOldTemp;
		//ԭͼ������ָ�붨λ��ͼ������ÿ�е�����λ��
		pOldBits =pOldBits + (i - 1 ) * nWidthBytes+nWidthBytes/2-1;
		if(*pOldBits!=255) {top=i;break;}
	}
	for(i=1;i<nHeight;i++)
	{
		pOldBits=pOldTemp;
		//ԭͼ������ָ�붨λ��ͼ������ÿ�е�����λ��
		pOldBits =pOldBits + (i - 1 ) * nWidthBytes+nWidthBytes/2-1;
		if(*pOldBits!=255) {bottom=i;break;}
	}

	return (float)nWidthBytes/(float)(top-bottom);

}

float CIdentify::GetArea()
{
	/*======ͼ��ָ��Ϊ�գ��޷���������=======*/
	if(m_pIndentify==NULL)
	{
		AfxMessageBox("��δ��ͼ��������������!");
		return FALSE;
	}
	
	/*==��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��===*/
	int nWidth = m_pIndentify->GetWidth();
	int nHeight = m_pIndentify->GetHeight();

	/*==������ͼ�����ݲ����йصı���===*/
	unsigned char *pOldBuffer, *pOldBits,*pOldTemp;


	int nWidthBytes, nNumColors, i,j;
	float area=0.0;
	
	/*======��ȡԭͼ��ָ��=======*/
	pOldBuffer = (unsigned char *) m_pIndentify->GetDIBPointer( &nWidthBytes, 
												m_pIndentify->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );
	
	//ԭͼ����ɫ��
	nNumColors = m_pIndentify->GetNumColors();
	//ԭͼ������ָ��
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
											 + nNumColors * sizeof(RGBQUAD)];
	pOldTemp=pOldBits;
	for(i=nHeight;i>0;i--)
	{
		pOldBits=pOldTemp;
		//ԭͼ������ָ�붨λ��ͼ������ÿ�еĿ�ʼλ��
		pOldBits =pOldBits + (i - 1 ) * nWidthBytes;
		for(j=0;j<nWidthBytes;j++)
		{
			if(*pOldBits!=255) area=area+1;
			pOldBits++;
		}
		
	}
	return area;
}

/********�������ܣ������������ȶԴ����ͼ�����������ȡ****/
////////////////////////////////////////////////////////////////////////
BOOL CIdentify::ContourTrace()
{

   //ͼ��ָ��Ϊ�գ��޷���������
	if(m_pIndentify == NULL) return(FALSE);
	
	//ֻ֧��8λͼ��
	if(m_pIndentify->GetNumBits() != 8)
	{
		// ��ʾ�û��������ô���
		AfxMessageBox("ֻ֧��8λͼ��,���������룡");
		
		// ����
		return( FALSE );
	}
	
	//��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��
	int nWidth = m_pIndentify->GetWidth();
	int nHeight = m_pIndentify->GetHeight();
	
	//�������
	unsigned char *pOldBuffer; 
	unsigned char *pOldBits, *pNewBits;
	unsigned char *pOldTemp, *pNewTemp;
	BITMAPFILEHEADER *pOldBFH;
	BITMAPINFOHEADER *pOldBIH;
	RGBQUAD *pOldPalette;
	int nWidthBytes, nNumColors, x, y;
	
	//�˸�����
	unsigned char n,e,s,w,ne,se,nw,sw;

	//��ʱ�������ֵ
	unsigned char pixel;

	//ԭͼ��ָ��
	pOldBuffer = (unsigned char *) m_pIndentify->GetDIBPointer(&nWidthBytes, 
																m_pIndentify->GetNumBits() );
	if( pOldBuffer == NULL ) return( FALSE );

	//ԭͼ���ļ�ͷָ��
	pOldBFH = (BITMAPFILEHEADER *) pOldBuffer;
	//ԭͼ����Ϣͷָ��
	pOldBIH = (BITMAPINFOHEADER *) &pOldBuffer[sizeof(BITMAPFILEHEADER)];
	//ԭͼ����ɫ��
	nNumColors = m_pIndentify->GetNumColors();
	//ԭͼ���ɫ��ָ��
	pOldPalette = (RGBQUAD *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	//ԭͼ������ָ��
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)
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
		m_pIndentify->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pIndentify->GetDib() );
		return( FALSE );
	}

	//��ͼ��ָ��
	pNewBits = (unsigned char *) ::GlobalLock( hNewDibBits );
	if( pNewBits == NULL )
	{
		::GlobalFree( hNewDibBits );
		m_pIndentify->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pIndentify->GetDib() );
		return( FALSE );
	}

	//��255����ɫ�������ͼ��������
	memset(pNewBits, (BYTE)255, nWidthBytes * nHeight );

	for(y = 1; y < nHeight - 1; y++)
	{
		//ָ��Դͼ������y��
		pOldTemp = pOldBits;
		pOldTemp +=  y * nWidthBytes;
		//ָ��Ŀ��ͼ������y��			
		pNewTemp = pNewBits;
		pNewTemp +=  y * nWidthBytes;
		for(x = 1; x < nWidth - 1; x++)
		{
			//ȡ�õ�ǰָ�봦������ֵ
			pixel = *pOldTemp;

			if(pixel == 0)
			{
				*pNewTemp = 0;
				nw = *(pOldTemp + nWidthBytes - 1);
				n  = *(pOldTemp + nWidthBytes);
				ne = *(pOldTemp + nWidthBytes + 1);
				w = *(pOldTemp - 1);
				e = *(pOldTemp + 1);
				sw = *(pOldTemp - nWidthBytes - 1);
				s  = *(pOldTemp - nWidthBytes );
				se = *(pOldTemp - nWidthBytes + 1);
				//������ڵİ˸��㶼�Ǻڵ�
				if(nw + n + ne + w + e + sw + s + se == 0)
				{
					*pNewTemp = 255;
				}
			}
			pOldTemp++;			
			pNewTemp++;
		}
	}

	//����ͼ���������ԭͼ��������
	memcpy( pOldBits, pNewBits, nWidthBytes * nHeight );

	::GlobalFree( hNewDibBits );
	::GlobalUnlock( m_pIndentify->GetDib() );
	
	return (TRUE);
}

////////////////////////////////////////////////////////////////////////
//���ܣ��ԻҶ�ͼ������������ٲ���������,����洢��TraceArray��
////////////////////////////////////////////////////////////////////////
BOOL CIdentify::EdgeTrace()
{
	if(m_pIndentify==NULL) return FALSE;
	if(m_pIndentify->GetNumBits()!=8){AfxMessageBox("����8λ�Ҷ�ͼ��");return FALSE;}
	
	unsigned char *pOld,*pBits,*pTemp,*pNew,*pNewBits,*pNewTemp;
	int nWidthBytes, nNumColors;
	DWORD offset,BufSize;
	pOld = (BYTE *) m_pIndentify->GetDIBPointer(&nWidthBytes);
	if(pOld == NULL) return FALSE;
	
	nNumColors = m_pIndentify->GetNumColors();
	offset=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+nNumColors*sizeof(RGBQUAD);
	pBits =(unsigned char *)&pOld[offset];
	int nHeight=m_pIndentify->GetHeight();
	int nWidth=m_pIndentify->GetWidth();
	BufSize=nWidthBytes*nHeight ;
	HGLOBAL hNewDib;
	hNewDib = ::GlobalAlloc(GHND,offset+BufSize );
	if( hNewDib == NULL ){
		m_pIndentify->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pIndentify->GetDib() );
		return FALSE;}
	pNew = (unsigned char *) ::GlobalLock( hNewDib );
	if( pNew == NULL )
	{
		::GlobalFree( hNewDib );
		m_pIndentify->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pIndentify->GetDib());
		return FALSE;
	}
	pNewBits = (unsigned char *)&pNew[offset];
	memcpy(pNew,pOld,offset);
	//�趨��ʼֵΪ255
	memset(pNewBits,(BYTE)255,BufSize);
	
	//�Ƿ��ҵ���ʼ�㼰�ص���ʼ��
	bool bFindStartPoint;
	//�Ƿ�ɨ�赽һ���߽��
	bool bFindPoint;
	//��ʼ�߽���뵱ǰ�߽��
	CPoint StartPoint,CurPoint;
	//�˸��������ʼɨ�跽�����������Ϸ����Ϸ������Ϸ����ҷ������·����·������·����󷽡�
	int Direction[8][2]={{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};
	int BeginDirect;
	
	//������������е�����
	TraceArray.RemoveAll();
	//����һ��EdgePoint�ͽṹ��Ա������ű߽�����Ϣ
	EdgePoint m_EdgePoint;
	
	//���ҵ������·��ı߽��
	bFindStartPoint=false;
	int i,j;
	for(j=0;j<nHeight && !bFindStartPoint;j++)
	{
		for(i=0;i<nWidth && !bFindStartPoint;i++)
		{
			//ָ��ԭͼ������j�У���i�����ص�ָ��
			pTemp=pBits+nWidthBytes*j+i;
			//ȡ�õ�ǰָ�봦������ֵ
			if(*pTemp==0)
			{
				bFindStartPoint=true;
				StartPoint.x=i;
				StartPoint.y=j;
				//ָ��Ŀ��ͼ��ĵ�����j�У���i�����ص�ָ��
				pNewTemp=pNewBits+nWidthBytes*j+i;
				*pNewTemp=(BYTE)0;
			}
		}
	}
	//������ʼ���������·�������ʼɨ�������Ϸ���
	BeginDirect=0;
	//���ٱ߽�
	bFindStartPoint=false;
	//����ʼ�㿪ʼɨ��
	CurPoint.x=StartPoint.x;
	CurPoint.y=StartPoint.y;
	while(!bFindStartPoint)
	{
		bFindPoint=false;
		while(!bFindPoint)
		{
			//Ҫע�������жϵ�˳��
			//�жϵ�ǰ���������ϵĵ��Ƿ񳬳�ͼ����߽�
			if((CurPoint.x==0) && (CurPoint.x+Direction[BeginDirect][0]<0)) BeginDirect=1;
			//�жϵ�ǰ���������ϵĵ��Ƿ񳬳�ͼ���ϱ߽�
			if((CurPoint.y==nHeight-1) && (CurPoint.y+Direction[BeginDirect][1]>nHeight-1)) BeginDirect=3;
			//�жϵ�ǰ���������ϵĵ��Ƿ񳬳�ͼ���ұ߽�
			if((CurPoint.x==nWidth-1) && (CurPoint.x+Direction[BeginDirect][0]>nWidth-1)) BeginDirect=5;
			//�жϵ�ǰ���������ϵĵ��Ƿ񳬳�ͼ���±߽�
			if((CurPoint.y==0) && (CurPoint.y+Direction[BeginDirect][1]<0)) BeginDirect=7;
			
			
			//��ɨ�跽��鿴һ������
			pTemp=pBits+nWidthBytes*(CurPoint.y+Direction[BeginDirect][1])+(CurPoint.x+Direction[BeginDirect][0]);
			if(*pTemp==0)
			{
				bFindPoint=true;

				/////////////////////////////////////////////////////
				//�ṹ�����м�¼�ĵ�һ�����������±߽��,����ʼ��
				//����Ϊ����һ���ʸ������
				m_EdgePoint.CurPoint.x = CurPoint.x;
				m_EdgePoint.CurPoint.y = CurPoint.y;
				m_EdgePoint.nCurVerct = (BYTE)BeginDirect;
				TraceArray.Add(m_EdgePoint);
				////////////////////////////////////////////////////

				CurPoint.x=CurPoint.x+Direction[BeginDirect][0];
				CurPoint.y=CurPoint.y+Direction[BeginDirect][1];
				if(CurPoint.y==StartPoint.y && CurPoint.x==StartPoint.x) bFindStartPoint=true;
				pNewTemp = pNewBits + nWidthBytes * CurPoint.y + CurPoint.x;
				*pNewTemp= (unsigned char)0;
				

				//ɨ��ķ�����ʱ����ת����
				BeginDirect--;
				if(BeginDirect == -1)
					BeginDirect = 7;
				BeginDirect--;
				if(BeginDirect == -1)
					BeginDirect = 7;
			}
			else
			{
				//ɨ�跽��˳ʱ����תһ��
				BeginDirect++;
				if(BeginDirect == 8)
					BeginDirect = 0;
			}
		}
	}
	::GlobalUnlock(m_pIndentify->GetDib());
	::GlobalFree(m_pIndentify->GetDib());
	::GlobalUnlock(hNewDib );
	
	//����ͼ������Ϊ��ǰͼ��
	m_pIndentify->SetDib( hNewDib );
	return true;
}


////////////////////////////////////////////////////////////
//���ܣ��ɱ߽����������������ܳ�
////////////////////////////////////////////////////////////
double CIdentify::GetPeriByChainCode()
{
	//ˮƽ����ֱ��б��������
	int nHorNum = 0,nVerNum = 0,nDiaNum = 0;
	//ѭ������
	int i;
	//�߽��ṹ�����
	EdgePoint m_EdgePoint;
	//��ǰʸ�����򣬼����뷽��
	BYTE CurVerct;
	//�߽��������TraceArray�Ĵ�С
	int nNumNode;

	nNumNode = TraceArray.GetSize();
	if(nNumNode<1)
	{
		AfxMessageBox("�Ƿ�û�н�����������?\n�������ȷ����������!");
		return FALSE;
	}

	for(i=0;i<nNumNode;i++)
	{
		//��ȡ��i����������
		m_EdgePoint = TraceArray.GetAt(i);
		CurVerct = m_EdgePoint.nCurVerct;
		//ˮƽ����������
		if(CurVerct==0 || CurVerct==4)
			nHorNum++;
		//��ֱ����������
		else if(CurVerct==2 || CurVerct==6)
			nVerNum++;
		//б��������
		else
			nDiaNum++;
	}
	//�����ɱ߽����������ܳ�
	return  int(nHorNum+nVerNum+nDiaNum*1.414);
	
}

POINT CIdentify::GetCentroid()
{
	/*======ͼ��ָ��Ϊ�գ��޷���������=======*/
	if(m_pIndentify==NULL)
	{
		AfxMessageBox("��δ��ͼ��������������!");
	}
	
	/*==��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��===*/
	int nWidth = m_pIndentify->GetWidth();
	int nHeight = m_pIndentify->GetHeight();

	/*==������ͼ�����ݲ����йصı���===*/
	unsigned char *pOldBuffer,  *pOldBits,*pOldTemp;

	int nWidthBytes, nNumColors,i,j;
	
	/*======��ȡԭͼ��ָ��=======*/
	pOldBuffer = (unsigned char *) m_pIndentify->GetDIBPointer( &nWidthBytes, 
												m_pIndentify->GetNumBits() );
	//ԭͼ����ɫ��
	nNumColors = m_pIndentify->GetNumColors();
	//ԭͼ������ָ��
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
											 + nNumColors * sizeof(RGBQUAD)];
	POINT point;
	long total_x=0,total_y=0,total_pix=0;

	pOldTemp=pOldBits;
	for(i=0;i<nHeight;i++)
	{
		pOldBits=pOldTemp;
		pOldBits=pOldBits+nWidthBytes*i;
		for(j=0;j<nWidthBytes;j++)
		{
			if(*pOldBits==0)
			{
				total_x+=i;
				total_y+=j;
				total_pix++;
			}
			pOldBits++;
		}
	}
    //��ȡ����
	point.x=total_x/total_pix;
	point.y=total_y/total_pix;

	return point;
}

float CIdentify::GetSquare()
{
	/*======ͼ��ָ��Ϊ�գ��޷���������=======*/
	if(m_pIndentify==NULL)
	{
		AfxMessageBox("��δ��ͼ��������������!");
	}
	
	/*==��ȡͼ���Ⱥ͸߶ȣ�������Ϊ��λ��===*/
	int nWidth = m_pIndentify->GetWidth();
	int nHeight = m_pIndentify->GetHeight();

	/*==������ͼ�����ݲ����йصı���===*/
	unsigned char *pOldBuffer,  *pOldBits,*pOldTemp;

	int nWidthBytes, nNumColors,i,j;
	
	/*======��ȡԭͼ��ָ��=======*/
	pOldBuffer = (unsigned char *) m_pIndentify->GetDIBPointer( &nWidthBytes, 
												m_pIndentify->GetNumBits() );
	//ԭͼ����ɫ��
	nNumColors = m_pIndentify->GetNumColors();
	//ԭͼ������ָ��
	pOldBits = (unsigned char *) &pOldBuffer[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
											 + nNumColors * sizeof(RGBQUAD)];
	POINT point;
	long total_x=0,total_y=0,total_pix=0,square_11=0,square_02=0,square_20=0,square=0;
	double angle=0;
	double radian=0.0;

	pOldTemp=pOldBits;
	for(i=0;i<nHeight;i++)
	{
		pOldBits=pOldTemp;
		pOldBits=pOldBits+nWidthBytes*i;
		for(j=0;j<nWidthBytes;j++)
		{
			if(*pOldBits==0)
			{
				total_x+=i;
				total_y+=j;
				total_pix++;
			}
			pOldBits++;
		}
	}

    //��ȡ����
	point.x=total_x/total_pix;
	point.y=total_y/total_pix;

	//�����ľ�
	for(i=0;i<nHeight;i++)
	{
		pOldBits=pOldTemp;
		pOldBits=pOldBits+nWidthBytes*i;
		for(j=0;j<nWidthBytes;j++)
		{
			if(*pOldBits==0)
			{
				square_11=square_11+(i-point.x)*(j-point.y);
				square_20=square_20+(i-point.x)*(i-point.x);
				square_02=square_02+(j-point.y)*(j-point.y);
			}
			pOldBits++;
		}
	}

	radian=atan(2*square_11/(double)(square_20-square_02));
	angle=(radian/3.1416)*360;

	CGeoTrans GeoTrans(m_pIndentify);
	GeoTrans.Rotate((int)(angle+0.5));

	nWidth=m_pIndentify->GetWidth();
	nHeight=m_pIndentify->GetHeight();

	/*==������ͼ�����ݲ����йصı���===*/
	unsigned char *pOldBuffer1,  *pOldBits1,*pOldTemp1;

	int nWidthBytes1, nNumColors1;
	
	/*======��ȡԭͼ��ָ��=======*/
	pOldBuffer1 = (unsigned char *) m_pIndentify->GetDIBPointer( &nWidthBytes1, 
												m_pIndentify->GetNumBits() );
	//ԭͼ����ɫ��
	nNumColors1 = m_pIndentify->GetNumColors();
	//ԭͼ������ָ��
	pOldBits1 = (unsigned char *) &pOldBuffer1[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
											 + nNumColors1 * sizeof(RGBQUAD)];
	total_pix=0; 
	pOldTemp1=pOldBits1;
	for(i=0;i<nHeight;i++)
	{
		pOldBits1=pOldTemp1;
		pOldBits1=pOldBits1+nWidthBytes1*i;
		for(j=0;j<nWidth;j++)
		{
			if(*pOldBits1==0)
			{
				square=square+(i-point.x)*(j-point.y);
				total_pix++;
			}
			pOldBits1++;
		}
	}
	//���ع�һ����Ķ������ľ�
	return (float)square/(float)(total_pix*total_pix);
} 
