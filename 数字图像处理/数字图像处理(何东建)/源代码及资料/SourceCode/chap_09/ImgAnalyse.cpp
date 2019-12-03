// ImgAnalyse.cpp: implementation of the CImgAnalyse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dip.h"
#include "ImgAnalyse.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CImgAnalyse, CObject)

//Diagnostics and dump member functions, overridden
#ifdef _DEBUG
void CImgAnalyse::Dump(CDumpContext &dc) const
{
	//call base class function first
	CObject::Dump(dc);
}
#endif

#ifdef _DEBUG
void CImgAnalyse::AssertValid() const
{
	//call inherited AssertValid first
	CObject::AssertValid();
	//Check CDibObject members...
	ASSERT(m_pDibObject != NULL);	//Must exist
}
#endif

/***********************************************************************
*                                                                      *
*                             ͼ������������                           *
*                                                                      *
***********************************************************************/

////////////////////////////////////////////////////////////////////////
//���캯��CImgAnalyse()    
//----------------------------------------------------------------------
//�������ܣ�����һ��CImgAnalyse��Ķ����粻����CDibObject���󡣵�   
//			һ�ε���ĳһ��������ʱ�������һ��CDibObject����ָ�롣
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//��    �أ���
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CImgAnalyse::CImgAnalyse()
{

}

////////////////////////////////////////////////////////////////////////
//���캯��CImgAnalyse()    
//----------------------------------------------------------------------
//�������ܣ�����һ��CImgAnalyse��Ķ��󲢴���CDibObject�������е�
//			��������Ըö���ֱ����һ��������Ϊ����������ͼ��������
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//��    �أ���
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CImgAnalyse::CImgAnalyse( CDibObject *pDibObject )
{
	m_pDibObject = pDibObject;
}

CImgAnalyse::~CImgAnalyse()
{

}

////////////////////////////////////////////////////////////////////////
//void SetDibObjectClass(CDibObject *pDibObject)   
//----------------------------------------------------------------------
//�������ܣ�������ΪCImgAnalyse�����ָ��һ��CDibObject����ָ��
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject, Ĭ��ΪNULL��
//----------------------------------------------------------------------
//��    �أ��ޡ�
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CImgAnalyse::SetDibObjectClass( CDibObject *pDibObject )
{
	m_pDibObject = pDibObject;
}

////////////////////////////////////////////////////////////////////////
//������BOOL EdgeTrace(CDibObject *pDibObject)
//���ܣ��ԻҶ�ͼ������������ٲ���������,����洢��TraceArray��
//˵����ֻ�Զ�ֵ����ĻҶ�ͼ�����һ����ͨ�ɷ֣�
//      ����֮ǰ��Ӧ�˳�����������ʹ�û�ֵ ������ȥ�벢ƽ���߽硣
//���أ����ٳɹ��򷵻�TRUE
////////////////////////////////////////////////////////////////////////
BOOL CImgAnalyse::EdgeTrace(CDibObject *pDibObject)
{
	if(m_pDibObject==NULL) return FALSE;
	if(m_pDibObject->GetNumBits()!=8){AfxMessageBox("����8λ�Ҷ�ͼ��");return FALSE;}
	
	unsigned char *pOld,*pBits,*pTemp,*pNew,*pNewBits,*pNewTemp;
	int nWidthBytes, nNumColors;
	DWORD offset,BufSize;
	pOld = (BYTE *) m_pDibObject->GetDIBPointer(&nWidthBytes);
	if(pOld == NULL) return FALSE;
	
	nNumColors = m_pDibObject->GetNumColors();
	offset=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+nNumColors*sizeof(RGBQUAD);
	pBits =(unsigned char *)&pOld[offset];
	int nHeight=m_pDibObject->GetHeight();
	int nWidth=m_pDibObject->GetWidth();
	BufSize=nWidthBytes*nHeight ;
	HGLOBAL hNewDib;
	hNewDib = ::GlobalAlloc(GHND,offset+BufSize );
	if( hNewDib == NULL ){
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib() );
		return FALSE;}
	pNew = (unsigned char *) ::GlobalLock( hNewDib );
	if( pNew == NULL )
	{
		::GlobalFree( hNewDib );
		m_pDibObject->m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
		::GlobalUnlock( m_pDibObject->GetDib());
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
				//����Ϊ����һ���ʸ������2003��2��20���޸�
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
				
				/*//�ṹ�����м�¼�ĵ�һ�����������±߽�����һ���ڵ㣬����Ϊ
				//�õ�ĵ�ǰʸ�������һ����Ϊ�����µ㣬2002��5��18��
				m_EdgePoint.CurPoint.x = CurPoint.x;
				m_EdgePoint.CurPoint.y = CurPoint.y;
				m_EdgePoint.nCurVerct = (BYTE)BeginDirect;
				TraceArray.Add(m_EdgePoint);*/

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
	::GlobalUnlock(m_pDibObject->GetDib());
	::GlobalFree(m_pDibObject->GetDib());
	::GlobalUnlock(hNewDib );
	
	//����ͼ������Ϊ��ǰͼ��
	m_pDibObject->SetDib( hNewDib );
	return true;
}

////////////////////////////////////////////////////////////////////////
//������Calibrate()
//���ܣ���ͼ����гߴ�궨
//������X_Scale��Y_Scale��XY_Scale�ֱ�Ϊˮƽ����ֱ�ͶԽǷ���ı궨ϵ����
//      realSizeΪʵ�ʳߴ磬��λΪmm,�궨���ϵ����λΪmm/pixel��
//���أ���
////////////////////////////////////////////////////////////////////////
void CImgAnalyse::Calibrate(float *X_Scale, float *Y_Scale, float *XY_Scale, float realSize)
{
	if(m_pDibObject == NULL) return;
	if(m_pDibObject->GetNumBits()!=8)
	{
		AfxMessageBox("����8λ�Ҷ�ͼ��");
		return;
	}
	
	unsigned char *pOld,*pBits,*pTemp;
	int nWidthBytes;
	DWORD offset;
	pOld = (BYTE *) m_pDibObject->GetDIBPointer(&nWidthBytes);
	if(pOld == NULL) return;
	
	offset=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
	pBits =(unsigned char *)&pOld[offset];

	int nHeight = m_pDibObject->GetHeight();
	int nWidth = m_pDibObject->GetWidth();

	int i,j,y1,y2,x1,x2;
	//�������£�������������ֵΪ0(��)�ĵ�һ�����ص㣬��Ϊ�ο�������е㣬����y����Ϊy1��
	for(j=0;j<nHeight;j++)
	{
		pTemp = pBits+(nHeight-j-1)*nWidthBytes;
		for(i=0;i<nWidth;i++)
			if(pTemp[i] == 0)
			{
				y1 = j;
				//��������ѭ��
				j = nHeight;
				break;
			}
	}
	//�������ϣ�������������ֵΪ0(��)�ĵ�һ�����ص㣬��Ϊ�ο�������е㣬����y����Ϊy2��
	for(j=0;j<nHeight;j++)
	{
		pTemp = pBits+j*nWidthBytes;
		for(i=0;i<nWidth;i++)
			if(pTemp[i] == 0)
			{
				y2 = nHeight-j-1;
				//��������ѭ��
				j = nHeight;
				break;
			}
	}
	//�������������ҳ��ο���������һ��Ϊ0(��)������,��Ϊx1
	for(i=0;i<nWidth;i++)
	{
		for(j=0;j<nHeight;j++)
		{
			pTemp = pBits+j*nWidthBytes+i;
			if(*pTemp == 0)
			{
				x1 = i;
				//��������ѭ��
				i = nWidth;
				break;
			}
		}
	}
	//�������������ҳ��ο�������ұ�һ��0(��)������,��Ϊx2
	for(i=nWidth-1;i>=0;i--)
	{
		for(j=0;j<nHeight;j++)
		{
			pTemp = pBits+j*nWidthBytes+i;
			if(*pTemp == 0)
			{
				x2 = i;
				//��������ѭ��
				i = -1;
				break;
			}
		}
	}
	//����궨ϵ��
	*X_Scale = realSize/(x2-x1);
	*Y_Scale = realSize/(y2-y1);
	*XY_Scale = (float)sqrt((*X_Scale)*(*X_Scale) + (*Y_Scale)*(*Y_Scale));
}

////////////////////////////////////////////////////////////////////////
//������GetEdgePointNum()
//���ܣ���ȡ�߽����
//���أ����ر߽����
////////////////////////////////////////////////////////////////////////
int CImgAnalyse::GetEdgePointNum()
{
	return	(TraceArray.GetSize());
}

////////////////////////////////////////////////////////////////////////
//������AreaByChain()
//���ܣ��ɱ߽���������������
//���أ�BOOL	������ȷ�򷵻�TRUE
////////////////////////////////////////////////////////////////////////
BOOL CImgAnalyse::AreaByChain(float *fArea)
{
	//ѭ������
	int i;
	//�߽��ṹ�����
	EdgePoint m_EdgePoint;
	//�߽��������TraceArray�Ĵ�С
	int nNumNode;

	//�ֱ����ڴ洢������ʽ��X��������2��
	//ע�⣺���ڱ߽���ٵ����ã��������������е�˳������Ϊ
	//���Ϸ����Ϸ������Ϸ����ҷ������·����·������·�����
	//���⣺TraceArray�д洢�������볣��������ͬ����ԭ�������½ǣ�Y����
	int DetaX[8] = {-1,0,1,1,1,0,-1,-1};
	int Alfa[8] = {1,0,-1,0,1,0,-1,0};

	float sum = 0.0f;
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
		sum += m_EdgePoint.CurPoint.y*DetaX[m_EdgePoint.nCurVerct]+0.5f*Alfa[m_EdgePoint.nCurVerct];
	}
	//�����ɱ߽������������
	*fArea = sum;

	return (TRUE);
}

////////////////////////////////////////////////////////////
//������BOOL AreaByGreen(m_fXScale,m_fYScale,float *fArea)
//���ܣ����ø��ֹ�ʽ���������ڵ������Ҫ���ñ߽������
//������m_fXScale��m_fYScale�ֱ�ΪX��Y����ı궨ϵ��
///////////////////////////////////////////////////////////
BOOL CImgAnalyse::AreaByGreen(float m_fXScale, float m_fYScale, float *fArea)
{
	//ѭ������
	int i;
	//�߽��ṹ�����
	EdgePoint m_EdgePoint;
	//�߽��������TraceArray�Ĵ�С
	int nNumNode;
	//��ǰ�����һ�������, ��һ�������
	int nCurX,nCurY,nNextX,nNextY,nFirstX,nFirstY;

	float sum = 0.0f;

	nNumNode = TraceArray.GetSize();

	if(nNumNode<1)
	{
		AfxMessageBox("�Ƿ�û�н�����������?\n�������ȷ����������!");
		return FALSE;
	}

	//��ȡ��0����������
	m_EdgePoint = TraceArray.GetAt(0);
	nCurX = m_EdgePoint.CurPoint.x;
	nCurY = m_EdgePoint.CurPoint.y;
	
	nFirstX = nCurX;
	nFirstY = nCurY;
	for(i=1;i<nNumNode;i++)
	{
		//��ȡ��i����������
		m_EdgePoint = TraceArray.GetAt(i);
		nNextX = m_EdgePoint.CurPoint.x;
		nNextY = m_EdgePoint.CurPoint.y;
		//�Ե�ǰ������һ���X��Y����ĳ˻����ۼ�
		sum += (nCurY*nNextX - nCurX*nNextY);
		nCurX = nNextX;
		nCurY = nNextY;
	}
	//�������һ�����0���X��Y�ĳ˻���
	sum += (nNextY*nFirstX - nNextX*nFirstY);
	//�����ɱ߽������������
	 *fArea = (float)(0.5*m_fXScale*m_fYScale*sum);
	 return (TRUE);
}

////////////////////////////////////////////////////////////
//������BOOL PeriByChainCode()
//���ܣ��ɱ߽����������������ܳ�
//�������ֱ�Ϊˮƽ����ֱ��б����ı궨ϵ��
////////////////////////////////////////////////////////////
BOOL CImgAnalyse::PeriByChainCode(float m_fXScale, float m_fYScale, float m_fXYScale, float *fP)
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
	*fP = (nHorNum*m_fXScale+nVerNum*m_fYScale+nDiaNum*m_fXYScale);
	return (TRUE);
}

//////////////////////////////////////////////////////////////////
//������FourierFactor(int *nDimSize)
//���ܣ�����Ҷ�任��
//������nDimSize	int��ָ�룬���ڴ��������С
//���أ�double��	�任���Ƶ������
double * CImgAnalyse::FourierFactor(int *nDimSize)
{
	int j;
	EdgePoint m_EdgePoint;
	int nNumNode;
	nNumNode = TraceArray.GetSize();
	if(nNumNode<1)
	{
		AfxMessageBox("�Ƿ�û�н�����������?\n�������ȷ����������!");
		return NULL;
	}

	//���ڴ��������С
	*nDimSize = nNumNode;
	//��ʼ��Ϊһά����
	mFourier = zeros(1,nNumNode);
	//ת��Ϊ��������
	mFourier = mcomplex(mFourier);

	for(j=0;j<nNumNode;j++)
	{
		m_EdgePoint = TraceArray.GetAt(j);
		//��X���긳��ʵ��
		mFourier.r(j+1) = m_EdgePoint.CurPoint.x;
		//��Y���긳���鲿
		mFourier.i(j+1) = m_EdgePoint.CurPoint.y;
	}
	//����Matlab�µ�һά����Ҷ�任����
	mFourier = fft(mFourier);
	//������ģ
	mFourier = msqrt(dot_mul(real(mFourier),real(mFourier))+dot_mul(imag(mFourier),imag(mFourier)));

	double * mdata;
	mdata = new double[nNumNode];
	memcpy(mdata, mFourier.addr(),nNumNode*sizeof(double));
	return mdata;
}

//////////////////////////////////////////////////////////////
//������LabelArea(CDibObject *pDibObject)
//���ܣ��Բ�ͬ����ͨ�ɷ�����ͬ�ı��,ͬʱ����������������������
//˵����ֻ��8λ��ֵ�Ҷ�ͼ��(0��255����ֵ)��Ч���辭��ֵ����
//��������pDibObjectΪ�գ���ʹ�õ�ǰ��ͼ�����
//////////////////////////////////////////////////////////////
BOOL CImgAnalyse::LabelArea(CDibObject *pDibObject)
{
	if(pDibObject!=NULL) m_pDibObject=pDibObject;
	if(m_pDibObject==NULL) return(FALSE);
	int nWidth,nHeight;
	nWidth=m_pDibObject->GetWidth();
	nHeight=m_pDibObject->GetHeight();
	//��ǰ���ص�����������
	int direct[8][2]={{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}};
	int lx,ly;//��ǰ����ڵ�����
	unsigned char *pOldBuffer;
	unsigned char *pNewBuffer,*pNewBits,*pNewTemp;
	int nWidthBytes, nNumColors, x, y, i, j, k;
	HGLOBAL hNewDib;
	pOldBuffer = (unsigned char *) m_pDibObject->GetDIBPointer( &nWidthBytes,m_pDibObject->GetNumBits());
	if(pOldBuffer == NULL) return(FALSE);
	nNumColors = m_pDibObject->GetNumColors();
	//λͼ����ƫ����
	DWORD offset=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+nNumColors*sizeof(RGBQUAD);
	//λͼ��������С
	DWORD dwNewSize=offset+nHeight*nWidthBytes;
	hNewDib=::GlobalAlloc(GHND,dwNewSize);
	if(hNewDib==NULL){AfxMessageBox("Alloc memory error!");return FALSE;}
	pNewBuffer=(unsigned char *)::GlobalLock(hNewDib);
	if(pNewBuffer==NULL)
	{
		::GlobalFree(hNewDib);
		AfxMessageBox("Lock memory error!");
		return FALSE;
	}
	pNewBits=(unsigned char *)&pNewBuffer[offset];
	memcpy(pNewBuffer,pOldBuffer,dwNewSize);

	LabelArray.RemoveAll();
	int label=100;//��ʼ��Ǻ�
	LabelResult m_LabelResult;//��ű�ǽ���Ľṹ��
	int NumPixels;//�������
	int xStart,yStart;//������ʼ��
	long cx,cy;//������������
	switch(m_pDibObject->GetNumBits())
	{
	case 8:
		for(y=0;y<nHeight;y++)
		{
			pNewTemp=pNewBits;
			pNewTemp+=y*nWidthBytes;
			for(x=0;x<nWidth; x++)
			{
				if(pNewTemp[x]==255)
				{
					if(label>255){AfxMessageBox("���̫��!");return (FALSE);}
					pNewTemp[x]=label;//���赱ǰԪ���Ա�Ǻ�
					int cnt=1;
					xStart=x;/////////////
					yStart=nHeight-1-y;/////////////
					cx=xStart,cy=yStart;
					NumPixels=1;
					while(cnt)
					{
						cnt=0;//ѭ�����
						for(j=0;j<nHeight;j++)
						{
							pNewTemp=pNewBits;
							for(k=0;k<nWidth;k++)
							{
								if(pNewTemp[k+j*nWidthBytes]==label)//����ҵ�һ����ͬ�ļǺ�
								{
									for(i=0;i<8;i++)
									{
										lx=k+direct[i][0];
										ly=j+direct[i][1];
										//�ж��Ƿ񳬳�ͼ��߽�
										if(lx>=0 && lx<nWidth && ly>=0 && ly<nHeight)
											//ѭ����ǰ�����
											if(*(pNewTemp+lx+ly*nWidthBytes)==255){
												*(pNewTemp+lx+ly*nWidthBytes)=label;
												NumPixels++;
												cx+=lx;
												cy+=nHeight-1-ly;
												cnt++;}
									}
								}
							}
						}
					}
					if(NumPixels==0){cx=0;cy=0;}
					else
					{
						cx=cx/NumPixels;cy=cy/NumPixels;
					}
					m_LabelResult.nLabelNo=label;
					m_LabelResult.StartPoint.x=xStart;
					m_LabelResult.StartPoint.y=yStart;
					m_LabelResult.CenterPoint.x=cx;
					m_LabelResult.CenterPoint.y=cy;
					m_LabelResult.LabelArea=NumPixels;
					LabelArray.Add(m_LabelResult);
					label=label+1;
				}
			}
		}
		break;
	}
	if(label-100==0){::GlobalUnlock(hNewDib);::GlobalFree(hNewDib);return (FALSE);}
	//���ڴ�����ͽ�����ʹ�õ��ڴ��ͷ�
	::GlobalUnlock( m_pDibObject->GetDib());
	::GlobalFree(m_pDibObject->GetDib());
	::GlobalUnlock(hNewDib);
	m_pDibObject->SetDib(hNewDib);
	return( TRUE );
}

////////////////////////////////////////////////////////
//�������ƣ�GetCentroid()
//��    �ܣ�����������LabelArea()�м������������
//��    ������
//��    �أ�POINT
////////////////////////////////////////////////////////
POINT CImgAnalyse::GetCentroid()
{
	//����һ��LabelResult���͵ı���
	LabelResult m_LabelResult;
	
	POINT m_Center;
	if(LabelArray.GetSize() > 0)
	{
		//��ģ������LabelArray�ĵ�һ��Ԫ�ص����ݸ������m_LabelResult
		m_LabelResult = LabelArray.GetAt(0);
		m_Center = m_LabelResult.CenterPoint;
	}
		return (m_Center);
}
