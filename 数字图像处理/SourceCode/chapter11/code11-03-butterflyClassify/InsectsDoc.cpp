// InsectsDoc.cpp : implementation of the CInsectsDoc class
//

#include "stdafx.h"
#include "Insects.h"

#include "InsectsDoc.h"
#include "InsectsView.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "DlgInfo.h"
#include "DibObject.h"
#include "Pretreatment.h"  //Ԥ������
#include "DlgLine.h"	   //ͼ�󼸺α任�Ի�����
#include "DlgMedFilter.h"  //��ֵ�˲��Ի�����
#include "DlgTempSmooth.h" //ģ������Ի�����
#include "Identify.h"	   //ͼ��ʶ����	
#include "DlgThreshold.h"  //��ֵ�Ի�����
#include "DlgMorth.h"	   //ͼ����̬ѧ�Ի�����
#include "MorphProc.h"	   //ͼ����̬ѧ��
#include "DlgGenZoom.h"	   //ͼ�����ŶԻ�����
#include "DlgGenTran.h"    //ͼ���ƶ��Ի����� 
#include "GeoTrans.h"      //ͼ�󼸺α仯��  
#include "DlgGenRota.h"    //ͼ����ת�Ի����� 
#include "DlgCharacter.h"  //ͼ�������Ի�����
#include "DlgResult.h"     //����Ի�����
#include "LapDlg.h"        //ͼ������  


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char szFilter[];
extern float antenna_factor[100];
extern float belly_factor[100];
extern int antenna_num;
extern int belly_num;
//define MAX_PATH=100;


/////////////////////////////////////////////////////////////////////////////
// CInsectsDoc

IMPLEMENT_DYNCREATE(CInsectsDoc, CDocument)

BEGIN_MESSAGE_MAP(CInsectsDoc, CDocument)
	//{{AFX_MSG_MAP(CInsectsDoc)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_CONVERT, OnConvert)
	ON_COMMAND(ID_AUTO_LIGHT, OnAutoLight)
	ON_COMMAND(ID_BELLY_SEP, OnBellySep)
	ON_COMMAND(ID_CHARACTER, OnCharacter)
	ON_COMMAND(ID_IDENTIFY, OnIdentify)
	ON_COMMAND(ID_LAPULASI_SHARP, OnLapulasiSharp)
	ON_COMMAND(ID_LINE_LIGHT, OnLineLight)
	ON_COMMAND(ID_MIDIAN_TEMP, OnMidianTemp)
	ON_COMMAND(ID_TIDU_SHARP, OnTiduSharp)
	ON_COMMAND(ID_WING_SEP, OnWingSep)
	ON_COMMAND(ID_ANTENNA_SEP, OnAntennaSep)
	ON_COMMAND(ID_TEMP_SMOOTH, OnTempSmooth)
	ON_COMMAND(ID_THRESHOLD_SEP, OnThresholdSep)
	ON_COMMAND(ID_DELATE, OnDelate)
	ON_COMMAND(ID_CANCLE_ERSION, OnCancleErsion)
	ON_COMMAND(ID_MORPH_OPEN, OnMorphOpen)
	ON_COMMAND(ID_MORPH_CLOSE, OnMorphClose)
	ON_COMMAND(ID_GEOM_ZOOM, OnGeomZoom)
	ON_COMMAND(ID_GEOM_MIRH, OnGeomMirh)
	ON_COMMAND(ID_GEOM_TRAN, OnGeomTran)
	ON_COMMAND(ID_GEOM_MIRV, OnGeomMirv)
	ON_COMMAND(ID_GEOM_TRPO, OnGeomTrpo)
	ON_COMMAND(ID_GEOM_ROTA, OnGeomRota)
	ON_COMMAND(ID_INFO, OnInfo)
	ON_COMMAND(ID_CONTOUR_TRACE, OnContourTrace)
	ON_COMMAND(ID_EDGE_TRACE, OnEdgeTrace)
	ON_UPDATE_COMMAND_UI(ID_ANTENNA_SEP, OnUpdateAntennaSep)
	ON_UPDATE_COMMAND_UI(ID_BELLY_SEP, OnUpdateBellySep)
	ON_UPDATE_COMMAND_UI(ID_CANCLE_ERSION, OnUpdateCancleErsion)
	ON_UPDATE_COMMAND_UI(ID_CHARACTER, OnUpdateCharacter)
	ON_UPDATE_COMMAND_UI(ID_CONTOUR_TRACE, OnUpdateContourTrace)
	ON_UPDATE_COMMAND_UI(ID_DELATE, OnUpdateDelate)
	ON_UPDATE_COMMAND_UI(ID_EDGE_TRACE, OnUpdateEdgeTrace)
	ON_UPDATE_COMMAND_UI(ID_IDENTIFY, OnUpdateIdentify)
	ON_UPDATE_COMMAND_UI(ID_LAPULASI_SHARP, OnUpdateLapulasiSharp)
	ON_UPDATE_COMMAND_UI(ID_TIDU_SHARP, OnUpdateTiduSharp)
	ON_UPDATE_COMMAND_UI(ID_THRESHOLD_SEP, OnUpdateThresholdSep)
	ON_UPDATE_COMMAND_UI(ID_TEMP_SMOOTH, OnUpdateTempSmooth)
	ON_UPDATE_COMMAND_UI(ID_MIDIAN_TEMP, OnUpdateMidianTemp)
	ON_UPDATE_COMMAND_UI(ID_MORPH_CLOSE, OnUpdateMorphClose)
	ON_UPDATE_COMMAND_UI(ID_WING_SEP, OnUpdateWingSep)
	ON_UPDATE_COMMAND_UI(ID_MORPH_OPEN, OnUpdateMorphOpen)
	ON_COMMAND(ID_HELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsectsDoc construction/destruction

CInsectsDoc::CInsectsDoc()
{
	// TODO: add one-time construction code here
	m_pDibObject=NULL;
	m_bImageLoaded=FALSE;
	depart_factor=3.0;

}

CInsectsDoc::~CInsectsDoc()
{
	if(m_pDibObject!=NULL)
	{
		delete m_pDibObject;
		m_pDibObject=NULL;
	}
}

BOOL CInsectsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CInsectsDoc serialization

void CInsectsDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CInsectsDoc diagnostics

#ifdef _DEBUG
void CInsectsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CInsectsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInsectsDoc commands


void CInsectsDoc::OnFileNew() 
{
	// TODO: Add your command handler code here
	
}

void CInsectsDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here
	static int nIndex = 1;
	
	CFileDialog FileDlg( TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter );
	FileDlg.m_ofn.nFilterIndex = (DWORD) nIndex;
	
	if( FileDlg.DoModal() == IDOK )
	{
		CString strPathName = FileDlg.GetPathName();
		AfxGetApp()->OpenDocumentFile( strPathName );
		nIndex = (int) FileDlg.m_ofn.nFilterIndex;
		if( !ReadImgToDoc() )
		{
			AfxMessageBox("�޷�����ͼ���ļ���");
			return;
		}
	}
}

BOOL CInsectsDoc::ReadImgToDoc()
{
	CString strPathName = GetPathName();
	
	//���õȴ����
	BeginWaitCursor();
	m_pDibObject = new CDibObject( strPathName.GetBuffer(3) );
	//ȡ���ȴ����
	EndWaitCursor();
	
	//����ͼ���ļ�ʧ��
	if( m_pDibObject == NULL )
	{	
		AfxMessageBox("�޷�����ͼ�������");
		//����FALSE
		return(FALSE);
	}
	
	//����ͼ���ļ��ɹ���������Ӧ����
	m_bImageLoaded = TRUE;
	
	//����TRUE
	return(TRUE);
}

void CInsectsDoc::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	static int nIndex = 1;

	CFileDialog DialogSaveAs( FALSE, NULL, m_pDibObject->GetImageName(),
		                      OFN_HIDEREADONLY, szFilter );

	DialogSaveAs.m_ofn.nFilterIndex = (DWORD) nIndex;

	if( DialogSaveAs.DoModal() == IDOK )
	{
		CMainFrame *pMainFrame = ( CMainFrame *)AfxGetMainWnd();
		CChildFrame *pChildFrame = ( CChildFrame *)pMainFrame->MDIGetActive();
		CInsectsView *pImgProView = ( CInsectsView * )pChildFrame->GetActiveView();
		
		nIndex = (int) DialogSaveAs.m_ofn.nFilterIndex;

		//��JPEG��ʽͼ���ж����Ƿ�Ϊ24λ���ɫͼ��
		if( nIndex == 5 )
		{
			if( m_pDibObject->GetNumBits() != 24 )
			{
				AfxMessageBox("ֻ��24λ���ɫͼ����ܴ�ΪJPEG��ʽ��");
				return;
			}
		}
		
		//CDibObject������ͼ����ܱ���
		if( m_pDibObject != NULL )
		{
			//��ȡָ�����ļ�������·������
			CString strPathName = DialogSaveAs.GetPathName();
			int nFindIndex = strPathName.Find(".");

			if( nFindIndex != -1)
				strPathName = strPathName.Left( nFindIndex );

			//����ָ������չ���Թ��������ļ���
			strPathName += CDibObject::szExtensions[ nIndex - 1 ];
			//����CDibObject���Ա����Save()����ͼ��
			m_pDibObject->Save( strPathName );
			
			//���ô��ڱ���
			CString strFileName = DialogSaveAs.GetFileName();
			nFindIndex = strFileName.Find(".");

			if ( nFindIndex != -1 )
				strFileName = strFileName.Left( nFindIndex );

			strFileName += CDibObject::szExtensions[ nIndex - 1 ];
			pChildFrame->SetWindowText( strFileName );
			
			SetPathName( strPathName );

			//������ΪJPEGͼ�����½�ͼ�����벢������ͼ����ʾ
			if( nIndex == 5 )
			{
				m_pDibObject->Load( strPathName );
				pImgProView->InvalidateRect( NULL, FALSE );
				pImgProView->UpdateWindow();
			}
		}
	}
}

/*===========����ɫͼ��ת����256ɫ�ĻҶ�ͼ��========*/
void CInsectsDoc::OnConvert() 
{
	//��ȡӦ�ó�����ָ��
	CInsectsApp *pApp = (CInsectsApp *)AfxGetApp();
	//��ȡ����ܴ���ָ��
	CMainFrame *pFrame =(CMainFrame *) AfxGetMainWnd();

	CDibObject *mixed = new CDibObject(m_pDibObject);
	
	//����Ԥ����CPretreatment�����
	CPretreatment Pre(m_pDibObject);
	
	// ���Ĺ����״
	BeginWaitCursor();
		
	if (Pre.MakeGray(mixed))
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CInsectsDoc* pDocument = (CInsectsDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = mixed;
		pDocument->m_bImageLoaded = TRUE;
		pDocument->SetTitle("ת����ĻҶ�ͼ��");

		//��ȡ�ӿ�ܴ���ָ��
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 50, 
			pChild->m_nHeight + 50, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox("ת������");
	
	// �ָ����
	EndWaitCursor();	
}

/*========�Զ�����ͼ�������=========*/
void CInsectsDoc::OnAutoLight() 
{
	//�жϵ�ǰ�Ƿ���ͼ�����
	if( m_pDibObject == NULL ) return;
	
	//�ڵ㴦��CPointPro���д�����������ֱ��ͼ������
	CPretreatment Pre( m_pDibObject );

	int *pHistogram = Pre.GetHistogram();
	
	int i = 0;
	int nGrayMax = 0, nGrayMin = 0;

	//������С�Ҷ�ֵ
	while(nGrayMin == 0 && i <= 255)
	{
		if( pHistogram[i] >= 100 )
		{
			nGrayMin = i;
		}
		i++;
	}

	//�������Ҷ�ֵ
	i = 255;
	while(nGrayMax == 0 && i >= 0)
	{
		if( pHistogram[i] >= 100 )
		{
			nGrayMax = i;
		}
		i--;
	}

	delete [] pHistogram;

	// ���Ĺ����״
	BeginWaitCursor();
	// ����AutoLight()�����������Ա任
	Pre.AutoLight(nGrayMin,0,nGrayMax,255);

	// ������ͼ
	UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();
	
}

/*===============��ͼ�󸹲����зָ�=================*/
void CInsectsDoc::OnBellySep() 
{
	//��ȡӦ�ó�����ָ��
	CInsectsApp *pApp = (CInsectsApp *)AfxGetApp();
	//��ȡ����ܴ���ָ��
	CMainFrame *pFrame =(CMainFrame *) AfxGetMainWnd();

	CDibObject *belly = new CDibObject(m_pDibObject);
	
	//����Ԥ����CIdentify�����
	CIdentify Ide(belly);
	
	// ���Ĺ����״
	BeginWaitCursor();
		
	if (Ide.BellySep())
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CInsectsDoc* pDocument = (CInsectsDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = belly;
		pDocument->m_bImageLoaded = TRUE;
		pDocument->SetTitle("�����ָ�");

		//��ȡ�ӿ�ܴ���ָ��
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 50, 
			pChild->m_nHeight + 50, SWP_NOZORDER  );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox("�ָ���ִ���");
	
	// �ָ����
	EndWaitCursor();
}

/*=============��ȡͼ�����Ҫ��Ϣ�����ǣ������ͳ��================*/
void CInsectsDoc::OnCharacter() 
{
	//�����Ի���
	CDlgCharacter dlgPara;

	CDibObject *pDib0 = new CDibObject(m_pDibObject);
	CDibObject *pDib1 = new CDibObject(m_pDibObject);
	CDibObject *pDib2 = new CDibObject(m_pDibObject);	
	CDibObject *pDib3 = new CDibObject(m_pDibObject);
	CDibObject *pDib4 = new CDibObject(m_pDibObject);
	CDibObject *pDib5 = new CDibObject(m_pDibObject);
	CDibObject *pDib6 = new CDibObject(m_pDibObject);
	CDibObject *pDib7 = new CDibObject(m_pDibObject);
	//����Ԥ����CIdentify�����
	CIdentify Ide0(pDib0);
	CIdentify Ide1(pDib1);
	CIdentify Ide2(pDib2);
	CIdentify Ide3(pDib3);
	CIdentify Ide4(pDib4);
	CIdentify Ide5(pDib5);
	CIdentify Ide6(pDib6);
	CIdentify Ide7(pDib7);

	//������Ӧ�ķָ��
	Ide0.AntennaSep();
	Ide1.BellySep();
	Ide2.SwingSep();
	Ide5.EdgeTrace();

	//��ȡ�䴥����״����
	dlgPara.m_nAntenna=Ide0.GetAntennaPara();
	//��ȡ�丹����ϸ����
	dlgPara.m_nBelly=Ide1.GetBellyPara();
	//��ȡ�������
	dlgPara.m_nSwing=Ide2.GetSwingPara();
	//��ȡ��ƫ����
	dlgPara.m_nEccentricity=Ide3.GetEccentricity();
	//��ȡͼ����
	dlgPara.m_Widths=m_pDibObject->GetHeight()>m_pDibObject->GetWidth()?m_pDibObject->GetHeight():m_pDibObject->GetWidth();
	//��ȡͼ��̳�
	dlgPara.m_Heights=m_pDibObject->GetHeight()>m_pDibObject->GetWidth()?m_pDibObject->GetWidth():m_pDibObject->GetHeight();
	//��ȡ�����
	dlgPara.m_nArea=Ide4.GetArea();
	//��ȡ���ܳ�
	dlgPara.m_nPerimeter=(float)Ide5.GetPeriByChainCode();
	//��ȡͼ����ζ�
	dlgPara.m_Square=Ide4.GetArea()/(m_pDibObject->GetWidth()*m_pDibObject->GetHeight());
	//��ȡ��Բ�ζ�
	dlgPara.m_nCircularity=(float)(4*Ide4.GetArea()/(3.1416*m_pDibObject->GetWidth()*m_pDibObject->GetWidth()));
	//��ȡ����״����
	dlgPara.m_nFormFactor=(float)((Ide5.GetPeriByChainCode()*Ide5.GetPeriByChainCode())/(4*3.1416*Ide4.GetArea()));	
	//��ȡͼ������ܶ�
	dlgPara.m_Indense=(float)((Ide5.GetPeriByChainCode()*Ide5.GetPeriByChainCode())/Ide4.GetArea());
	//��ȡͼ������
	dlgPara.m_Center_x=(Ide6.GetCentroid()).x;
	dlgPara.m_Center_y=(Ide6.GetCentroid()).y;
	//��ȡͼ��������ľ�
	dlgPara.m_Center_Square=Ide7.GetSquare();
	//��ʾ�Ի���
	if(dlgPara.DoModal() != IDOK)
	{
		return;
	}
	
}

/*==============��ͼ�����ʶ��======================*/
void CInsectsDoc::OnIdentify() 
{
	//�����Ի���
	CDlgResult dlgPara;
	double weight=0.0;

	CDibObject *pDib1 = new CDibObject(m_pDibObject);
	CDibObject *pDib2 = new CDibObject(m_pDibObject);	
	CDibObject *pDib3 = new CDibObject(m_pDibObject);
	CDibObject *pDib4 = new CDibObject(m_pDibObject);
	CDibObject *pDib5 = new CDibObject(m_pDibObject);
	CDibObject *pDib6 = new CDibObject(m_pDibObject);

	//����Ԥ����CIdentify�����
	CIdentify Ide1(pDib1);
	CIdentify Ide2(pDib2);
	CIdentify Ide3(pDib3);
	CIdentify Ide4(pDib4);
	CIdentify Ide5(pDib5);
	CIdentify Ide6(pDib6);

	Ide1.AntennaSep();
	Ide2.BellySep();
	Ide3.SwingSep();
	Ide6.EdgeTrace();

	dlgPara.m_nAntenna=Ide1.GetAntennaPara();
	dlgPara.m_nBelly=Ide2.GetBellyPara();
	dlgPara.m_nSwing=Ide3.GetSwingPara();
	dlgPara.m_nEccentricity=Ide4.GetEccentricity();
	dlgPara.m_nArea=Ide5.GetArea();
	dlgPara.m_nSquare=(float)(Ide5.GetArea()/(m_pDibObject->GetWidth()*m_pDibObject->GetHeight()));
	
	if(Ide1.GetAntennaPara()>0.5)
		weight=weight+0.7;

	if(Ide2.GetBellyPara()<0.057)
		weight=weight+0.1;

	if(Ide5.GetArea()>88866)
		weight=weight+0.1;

	if(dlgPara.m_nSquare>=0.47)
		weight=weight+0.05;

	if(dlgPara.m_nEccentricity>=3.3)
		weight=weight+0.05;
	CString str;
	str.Format("%f",weight);
	dlgPara.m_nWeight=str;
    
	if(weight>=0.705)
		dlgPara.m_Result="��������������ȡ�Ͳ���������W>=0.705,\n\n���Կ����ж����۳�Ŀ����Ϊ����(Butterfly)";
	else
		dlgPara.m_Result="��������������ȡ�Ͳ���������W<0.705,\n\n���Կ����ж����۳�Ŀ����Ϊ���ӣ�Moth��";
	//��ʾ�Ի���
	if(dlgPara.DoModal() != IDOK)
	{
		return;
	}
	 	
}

/*============��ͼ�����������˹��=================*/
void CInsectsDoc::OnLapulasiSharp() 
{
// ģ��߶�
	int	nTempH;
	
	// ģ����
	int nTempW;
	
	// ģ��ϵ��
	float fTempC;
	
	// ģ������Ԫ��X����
	int nTempXc;
	
	// ģ������Ԫ��Y����
	int nTempYc;
	
	// ģ��Ԫ������
	float aValue[25];

	// �����Ի���
	CLapDlg dlgPara;
	
	// ��ģ�����鸳��ֵ��Ϊƽ��ģ�壩
	aValue[0]  = -1.0;
	aValue[1]  = -1.0;
	aValue[2]  = -1.0;
	aValue[3]  =  0.0;
	aValue[4]  =  0.0;
	aValue[5]  = -1.0;
	aValue[6]  =  9.0;
	aValue[7]  = -1.0;
	aValue[8]  =  0.0;
	aValue[9]  =  0.0;
	aValue[10] = -1.0;
	aValue[11] = -1.0;
	aValue[12] = -1.0;
	aValue[13] =  0.0;
	aValue[14] =  0.0;
	aValue[15] =  0.0;
	aValue[16] =  0.0;
	aValue[17] =  0.0;
	aValue[18] =  0.0;
	aValue[19] =  0.0;
	aValue[20] =  0.0;
	aValue[21] =  0.0;
	aValue[22] =  0.0;
	aValue[23] =  0.0;
	aValue[24] =  0.0;
	
	// ��ʼ���Ի������ֵ
	dlgPara.m_intType = 0;
	dlgPara.m_iTempH  = 3;
	dlgPara.m_iTempW  = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_fTempC  = 1.0;
	dlgPara.m_fpArray = aValue;
	
	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨��ƽ����
	nTempH  = dlgPara.m_iTempH;
	nTempW  = dlgPara.m_iTempW;
	nTempXc = dlgPara.m_iTempMX;
	nTempYc = dlgPara.m_iTempMY;
	fTempC  = dlgPara.m_fTempC;
	
	// ɾ���Ի���
	delete dlgPara;	
	
	//�����㴦��CPointPro�����
	CPretreatment Pre( m_pDibObject );
	// ���Ĺ����״
	BeginWaitCursor();

	Pre.LapSharping(aValue, fTempC, nTempW, nTempH, nTempXc, nTempYc);
	
	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);	
}

/*==========��ͼ������Ƚ������Ա仯==================*/
void CInsectsDoc::OnLineLight() 
{
	// �����Ի���
	CDlgLine dlg;
	
	// ���Ա任��б��
	float f1;	
	// ���Ա任�Ľؾ�
	float f2;

	// ��ʼ������ֵ
	dlg.m_f1= 1.0;
	dlg.m_f2 = 0.0;
	
	if (dlg.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨��ƽ����
	f1 = dlg.m_f1;
	f2 = dlg.m_f2;

	// ɾ���Ի���
	delete dlg;	
	
	//�����㴦��CPretreatment�����
	CPretreatment Pre( m_pDibObject );
	
	// ���Ĺ����״
	BeginWaitCursor();
	// ����LinerTrans()�����������Ա任
	Pre.LinerTran(f1, f2);

	// ������ͼ
	UpdateAllViews(NULL);

	// �ָ����
	EndWaitCursor();
	
}

/*========��ֵ�˲���������=============*/
void CInsectsDoc::OnMidianTemp() 
{	// ���ڸ߶�
	int	nTempH;
	
	// ���ڿ��
	int nTempW;
	
	// ��������Ԫ��X����
	int nTempXc;
	
	// ��������Ԫ��Y����
	int nTempYc;

	// ����ʽ
	int nSortType;

	
	// �����Ի���
	CDlgMedFilter dlg;
	
	// ��ʼ���Ի������ֵ
	dlg.m_nWindowSize = 0;
	dlg.m_iTempH  = 3;
	dlg.m_iTempW  = 3;
	dlg.m_iTempMX = 1;
	dlg.m_iTempMY = 1;
	dlg.m_nSortType = 0;
	
	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlg.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨��ƽ����
	nTempH  = dlg.m_iTempH;
	nTempW  = dlg.m_iTempW;
	nTempXc = dlg.m_iTempMX;
	nTempYc = dlg.m_iTempMY;
	nSortType  = dlg.m_nSortType;

	
	// ɾ���Ի���
	delete dlg;	
	
	//�����㴦��CPretreatment�����
	CPretreatment Pre( m_pDibObject );
	// ���Ĺ����״
	BeginWaitCursor();

	Pre.MedianFilter(nSortType, nTempW, nTempH, nTempXc, nTempYc);

	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);	

}

/*========�ݶ���=============*/
void CInsectsDoc::OnTiduSharp() 
{
	//�����㴦��CPointPro�����
	CPretreatment Pre( m_pDibObject );
	// ���Ĺ����״
	BeginWaitCursor();

	Pre.GradSharping(10);

	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);	
}

/*========��ͼ��ĳ����зָ�=============*/
void CInsectsDoc::OnWingSep() 
{
	//��ȡӦ�ó�����ָ��
	CInsectsApp *pApp = (CInsectsApp *)AfxGetApp();
	//��ȡ����ܴ���ָ��
	CMainFrame *pFrame =(CMainFrame *) AfxGetMainWnd();

	CDibObject *swing = new CDibObject(m_pDibObject);
	
	//����Ԥ����CIdentify�����
	CIdentify Ide(swing);
	
	// ���Ĺ����״
	BeginWaitCursor();
		
	if (Ide.SwingSep())
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CInsectsDoc* pDocument = (CInsectsDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = swing;
		pDocument->m_bImageLoaded = TRUE;
		pDocument->SetTitle("���ָ�");

		//��ȡ�ӿ�ܴ���ָ��
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 50, 
			pChild->m_nHeight + 50, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox("�ָ���ִ���");
	
	// �ָ����
	EndWaitCursor();		
}

void CInsectsDoc::OnAntennaSep() 
{
	//��ȡӦ�ó�����ָ��
	CInsectsApp *pApp = (CInsectsApp *)AfxGetApp();
	//��ȡ����ܴ���ָ��
	CMainFrame *pFrame =(CMainFrame *) AfxGetMainWnd();

	CDibObject *antenna = new CDibObject(m_pDibObject);
	
	//����Ԥ����CIdentify�����
	CIdentify Ide(antenna);
	
	// ���Ĺ����״
	BeginWaitCursor();
		
	if (Ide.AntennaSep())
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CInsectsDoc* pDocument = (CInsectsDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = antenna;
		pDocument->m_bImageLoaded = TRUE;
		pDocument->SetTitle("���Ƿָ�");

		//��ȡ�ӿ�ܴ���ָ��
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 12, 
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox("�ָ���ִ���");
	
	// �ָ����
	EndWaitCursor();
}

void CInsectsDoc::OnTempSmooth() 
{
	// ģ��߶�
	int	nTempH;
	
	// ģ����
	int nTempW;
	
	// ģ��ϵ��
	float fTempC;
	
	// ģ������Ԫ��X����
	int nTempXc;
	
	// ģ������Ԫ��Y����
	int nTempYc;
	
	// ģ��Ԫ������
	float nTemp[25];

	// �����Ի���
	CDlgTempSmooth dlg;
	
	// ��ģ�����鸳��ֵ��Ϊƽ��ģ�壩
	nTemp[0] = 1.0;
	nTemp[1] = 1.0;
	nTemp[2] = 1.0;
	nTemp[3] = 1.0;
	nTemp[4] = 1.0;
	nTemp[5] = 1.0;
	nTemp[6] = 1.0;
	nTemp[7] = 1.0;
	nTemp[8] = 1.0;
	nTemp[9] = 1.0;
	nTemp[10] = 1.0;
	nTemp[11] = 1.0;
	nTemp[12] = 1.0;
	nTemp[13] = 1.0;
	nTemp[14] = 1.0;
	nTemp[15] = 1.0;
	nTemp[16] = 1.0;
	nTemp[17] = 1.0;
	nTemp[18] = 1.0;
	nTemp[19] = 1.0;
	nTemp[20] = 1.0;
	nTemp[21] = 1.0;
	nTemp[22] = 1.0;
	nTemp[23] = 1.0;
	nTemp[24] = 1.0;
	
	// ��ʼ���Ի������ֵ
	dlg.m_intType = 0;
	dlg.m_iTempH  = 3;
	dlg.m_iTempW  = 3;
	dlg.m_iTempMX = 1;
	dlg.m_iTempMY = 1;
	dlg.m_fTempC  = 9.0;
	dlg.m_fpArray = nTemp;
	
	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlg.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨��ƽ����
	nTempH  = dlg.m_iTempH;
	nTempW  = dlg.m_iTempW;
	nTempXc = dlg.m_iTempMX;
	nTempYc = dlg.m_iTempMY;
	fTempC  = dlg.m_fTempC;
	
	// ɾ���Ի���
	delete dlg;	
	
	//�����㴦��CPretreatment�����
	CPretreatment Pre( m_pDibObject );
	// ���Ĺ����״
	BeginWaitCursor();

	Pre.TemplateSmooth(nTemp, fTempC, nTempW, nTempH, nTempXc, nTempYc);

	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);
	
}


void CInsectsDoc::OnThresholdSep() 
{
	//�жϵ�ǰ�Ƿ���ͼ�����
	if( m_pDibObject == NULL ) return;
	
	//�ڵ㴦��CPretreatment���д�����������ֱ��ͼ������
	CPretreatment Pre( m_pDibObject );
	
	CDlgThreshold Dlg;
	
	// ���Ĺ����״
	BeginWaitCursor();

	Dlg.m_nThresholdJustAnalyze = Pre.ThresholdJustAnlyze();
	Dlg.m_ThresholdAuto = Pre.ThresholdAuto();
	Dlg.m_nCurrentThreshold = Dlg.m_ThresholdAuto;
	
	//��ʾ�Ի���
	if (Dlg.DoModal() != IDOK)
		return;


	// ����Threshold()����������ֵ������
	  Pre.Threshold(Dlg.m_nCurrentThreshold, Dlg.m_nThresholdType, TRUE);

	m_bBinary = TRUE;

	// ������ͼ
	UpdateAllViews(NULL);	
	// �ָ����
	EndWaitCursor();	
}

void CInsectsDoc::OnDelate() 
{
	//�ṹԪ������ָ�루�洢���Ǹ��ṹԪ������ڽṹ���ĵ����꣩
	int *nMask;

	//�ṹԪ�صĴ�С���ԽṹԪ���еĵ���Ϊ��λ��
	int nMaskLen = 0;


	// �����Ի���
	CDlgMorth dlgPara;
	
	// ��ʾ�Ի�����ʾ�û��趨��ʴ����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}

	nMask = dlgPara.m_npMask;
	nMaskLen = dlgPara.m_nMaskLen;

	// ɾ���Ի���
	delete dlgPara;	

	//������̬ѧ����CMorphPro�����
	CMorphPro MorphProcesses( m_pDibObject );

	// ���Ĺ����״
	BeginWaitCursor();

	MorphProcesses.Dilation(nMask, nMaskLen);
	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);		
}

void CInsectsDoc::OnCancleErsion() 
{
		//�ṹԪ������ָ�루�洢���Ǹ��ṹԪ������ڽṹ���ĵ����꣩
	int *nMask;

	//�ṹԪ�صĴ�С���ԽṹԪ���еĵ���Ϊ��λ��
	int nMaskLen = 0;


	// �����Ի���
	CDlgMorth dlgPara;
	
	// ��ʾ�Ի�����ʾ�û��趨��ʴ����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}

	nMask = dlgPara.m_npMask;
	nMaskLen = dlgPara.m_nMaskLen;

	// ɾ���Ի���
	delete dlgPara;	

	//������̬ѧ����CMorphPro�����
	CMorphPro MorphProcesses( m_pDibObject );

	// ���Ĺ����״
	BeginWaitCursor();

	MorphProcesses.Erosion(nMask, nMaskLen);
	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);
}

void CInsectsDoc::OnMorphOpen() 
{
//�ṹԪ������ָ�루�洢���Ǹ��ṹԪ������ڽṹ���ĵ����꣩
	int *nMask;

	//�ṹԪ�صĴ�С���ԽṹԪ���еĵ���Ϊ��λ��
	int nMaskLen = 0;


	// �����Ի���
	CDlgMorth dlgPara;
	
	// ��ʾ�Ի�����ʾ�û��趨��ʴ����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}

	nMask = dlgPara.m_npMask;
	nMaskLen = dlgPara.m_nMaskLen;

	// ɾ���Ի���
	delete dlgPara;	

	//������̬ѧ����CMorphPro�����
	CMorphPro MorphProcesses( m_pDibObject );

	// ���Ĺ����״
	BeginWaitCursor();

	MorphProcesses.Opening(nMask, nMaskLen);
	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);		
}

void CInsectsDoc::OnMorphClose() 
{
    
	//�ṹԪ������ָ�루�洢���Ǹ��ṹԪ������ڽṹ���ĵ����꣩
	int *nMask;

	//�ṹԪ�صĴ�С���ԽṹԪ���еĵ���Ϊ��λ��
	int nMaskLen = 0;


	// �����Ի���
	CDlgMorth dlgPara;
	
	// ��ʾ�Ի�����ʾ�û��趨��ʴ����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}

	nMask = dlgPara.m_npMask;
	nMaskLen = dlgPara.m_nMaskLen;

	// ɾ���Ի���
	delete dlgPara;	

	//������̬ѧ����CMorphPro�����
	CMorphPro MorphProcesses( m_pDibObject );

	// ���Ĺ����״
	BeginWaitCursor();

	MorphProcesses.Closing(nMask, nMaskLen);
	// �ָ����
	EndWaitCursor();
	
	// ������ͼ
	UpdateAllViews(NULL);		
}

void CInsectsDoc::OnGeomZoom() 
{
    // ���ű���
	float fXZoomRatio;
	float fYZoomRatio;
	
	// �����Ի���
	CDlgGenZoom dlgPara;
	
	// ��ʼ������ֵ
	dlgPara.m_XZoom = 1.0;
	dlgPara.m_YZoom = 1.0;
	
	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨��ƽ����
	fXZoomRatio = dlgPara.m_XZoom;
	fYZoomRatio = dlgPara.m_YZoom;
	
	// ɾ���Ի���
	delete dlgPara;	
	
	//�����㴦��CImageGeoTrans�����
	CGeoTrans GeoTrans( m_pDibObject );

	// ���Ĺ����״
	BeginWaitCursor();
	
	//����ZoomImage()��������ͼ������
	GeoTrans.ZoomImage(fXZoomRatio, fYZoomRatio);
	
	// ������ͼ
	UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();	
}

void CInsectsDoc::OnGeomMirh() 
{
	//�����㴦��CImageGeoTrans�����
	CGeoTrans GeoTrans( m_pDibObject );

	// ���Ĺ����״
	BeginWaitCursor();

	//����Mirror()�������д�ֱ����
	GeoTrans.Mirror(TRUE);

	// ��������
	//	SetModifiedFlag(TRUE);

	// ������ͼ
	UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();	
}

void CInsectsDoc::OnGeomTran() 
{
// �����Ի���
	CDlgGenTran dlgPara;
	
	// ��ʼ������ֵ
	dlgPara.m_XOffset = 0;
	dlgPara.m_YOffset = 0;
	
	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨��ƽ����
	LONG lXOffset = dlgPara.m_XOffset;
	LONG lYOffset = dlgPara.m_YOffset;
	// ɾ���Ի���
	delete dlgPara;	
	
	//�����㴦��CImageGeoTrans�����
	CGeoTrans GeoTrans( m_pDibObject );

	// ���Ĺ����״
	BeginWaitCursor();

	GeoTrans.TranslationPixel(lXOffset, lYOffset);

	// ������ͼ
	UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();	
}

void CInsectsDoc::OnGeomMirv() 
{
//�����㴦��CImageGeoTrans�����
	CGeoTrans GeoTrans( m_pDibObject );

	// ���Ĺ����״
	BeginWaitCursor();

	//����Mirror()��������ˮƽ����
	GeoTrans.Mirror(FALSE);

	// ��������
	//	SetModifiedFlag(TRUE);

	// ������ͼ
	UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();	
}

void CInsectsDoc::OnGeomTrpo() 
{
//�����㴦��CImageGeoTrans�����
	CGeoTrans GeoTrans( m_pDibObject );

	// ���Ĺ����״
	BeginWaitCursor();

	//����Mirror()�������д�ֱ����
	GeoTrans.Transpose();

	// ��������
	//	SetModifiedFlag(TRUE);

	// ������ͼ
	UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();	
}

void CInsectsDoc::OnGeomRota() 
{
	//��ת�Ƕ�
	int nRotateAngle;
	
	//�����Ի���
	CDlgGenRota dlgPara;
	
	//��ʼ������ֵ
	dlgPara.m_iRotateAngle = 90;
	
	//��ʾ�Ի�����ʾ�û��趨��ת�Ƕ�
	if(dlgPara.DoModal() != IDOK)
	{
		//����
		return;
	}
	
	//��ȡ�û��趨�ĽǶ�
	nRotateAngle = dlgPara.m_iRotateAngle;
	
	//ɾ���Ի���
	delete dlgPara;	

	//�����㴦��CImageGeoTrans�����
	CGeoTrans GeoTrans( m_pDibObject );

	// ���Ĺ����״
	BeginWaitCursor();

	//����Rotate()��������ͼ����ת
	GeoTrans.Rotate(nRotateAngle);

	// ������ͼ
	UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();	
}

void CInsectsDoc::OnInfo() 
{
	CDlgInfo dlg;

	dlg.m_nHeight=m_pDibObject->GetHeight();
	dlg.m_nWidth=m_pDibObject->GetWidth();
	dlg.m_nSize=(m_pDibObject->GetWidth()*m_pDibObject->GetHeight())/1024;
	dlg.m_nBits=m_pDibObject->GetNumBits();

	//��ʾ�Ի�����ʾ�û��趨��ת�Ƕ�
	if(dlg.DoModal() != IDOK)
	{
		//����
		return;
	}
}

void CInsectsDoc::OnThin() 
{	
}

void CInsectsDoc::OnAnteenaFactor() 
{
	CDibObject *pDib = new CDibObject(m_pDibObject);
	//����Ԥ����CIdentify�����
	CIdentify Ide(pDib);

	//������Ӧ�ķָ��
	Ide.AntennaSep();
	CString str;
	str.Format("%f",Ide.GetAntennaFactor());
    AfxMessageBox(str);
	antenna_factor[antenna_num++]=Ide.GetAntennaFactor();
}

void CInsectsDoc::OnBellyFactor() 
{
	CDibObject *pDib = new CDibObject(m_pDibObject);
	//����Ԥ����CIdentify�����
	CIdentify Ide(pDib);

	//������Ӧ�ķָ��
	Ide.BellySep();
	CString str;
	str.Format("%f",Ide.GetBellyFactor());
    AfxMessageBox(str);
	belly_factor[belly_num++]=Ide.GetBellyFactor();
}

void CInsectsDoc::OnAnteenaAvgFactor() 
{
	float total=0.0,avg_factor=0.0;
	for(int i=0;i<antenna_num;i++)
		total+=antenna_factor[i];
	avg_factor=total/antenna_num;
	
	CString str;
	str.Format("%f",avg_factor);
	AfxMessageBox(str);
	
}

void CInsectsDoc::OnBellyAvgFactor() 
{
	float total=0.0;
	for(int i=0;i<belly_num;i++)
		total+=belly_factor[i];
	depart_factor=total/belly_num;
	
	CString str;
	str.Format("%f",depart_factor);
	AfxMessageBox(str);
	
}

void CInsectsDoc::OnContourTrace() 
{


	//��ȡӦ�ó�����ָ��
	CInsectsApp *pApp = (CInsectsApp *)AfxGetApp();
	//��ȡ����ܴ���ָ��
	CMainFrame *pFrame =(CMainFrame *) AfxGetMainWnd();

	CDibObject *counter_trace = new CDibObject(m_pDibObject);
	//����Ԥ����CIdentify�����
	CIdentify Ide(counter_trace);

	// ���Ĺ����״
	BeginWaitCursor();
		
	if (Ide.ContourTrace())
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CInsectsDoc* pDocument = (CInsectsDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = counter_trace;
		pDocument->m_bImageLoaded = TRUE;
		pDocument->SetTitle("������ȡ");

		//��ȡ�ӿ�ܴ���ָ��
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 12, 
			pChild->m_nHeight + 38, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox("������ȡ����");
	
	// �ָ����
	EndWaitCursor();

}

void CInsectsDoc::OnEdgeTrace() 
{

	//��ȡӦ�ó�����ָ��
	CInsectsApp *pApp = (CInsectsApp *)AfxGetApp();
	//��ȡ����ܴ���ָ��
	CMainFrame *pFrame =(CMainFrame *) AfxGetMainWnd();

	CDibObject *edge_trace = new CDibObject(m_pDibObject);
	//����Ԥ����CIdentify�����
	CIdentify Ide(edge_trace);

	// ���Ĺ����״
	BeginWaitCursor();
		
	if (Ide.EdgeTrace())
	{
		POSITION posTemplate = pApp->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = pApp->GetNextDocTemplate(posTemplate);
		
		CInsectsDoc* pDocument = (CInsectsDoc*) pDocTemplate->OpenDocumentFile(NULL);
		pDocument->m_pDibObject = edge_trace;
		pDocument->m_bImageLoaded = TRUE;
		pDocument->SetTitle("�߽����");

		//��ȡ�ӿ�ܴ���ָ��
		CChildFrame *pChild = (CChildFrame *) pFrame->MDIGetActive();
		pChild->m_nWidth = m_pDibObject->GetWidth(); 
		pChild->m_nHeight = m_pDibObject->GetHeight();
		pChild->SetWindowPos( NULL, 0, 0, pChild->m_nWidth + 50, 
			pChild->m_nHeight + 50, SWP_NOZORDER | SWP_NOMOVE );

		pDocument->UpdateAllViews(NULL);
		
	}
	else
		AfxMessageBox("�߽���ٴ���");
	
	// �ָ����
	EndWaitCursor();
	
}

void CInsectsDoc::OnUpdateAntennaSep(CCmdUI* pCmdUI) 
{
  
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());
	
}

void CInsectsDoc::OnUpdateBellySep(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());
	
}

void CInsectsDoc::OnUpdateCancleErsion(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());	
}

void CInsectsDoc::OnUpdateCharacter(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());	
}

void CInsectsDoc::OnUpdateContourTrace(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());	
}

void CInsectsDoc::OnUpdateDelate(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());	
}

void CInsectsDoc::OnUpdateEdgeTrace(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());	
}

void CInsectsDoc::OnUpdateIdentify(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());	
}

void CInsectsDoc::OnUpdateLapulasiSharp(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsGrayImage());	
}

void CInsectsDoc::OnUpdateTiduSharp(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsGrayImage());		
}

void CInsectsDoc::OnUpdateThresholdSep(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsGrayImage());		
}

void CInsectsDoc::OnUpdateTempSmooth(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsGrayImage());		
}

void CInsectsDoc::OnUpdateMidianTemp(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsGrayImage());		
}

void CInsectsDoc::OnUpdateMorphClose(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());	
	
}

void CInsectsDoc::OnUpdateWingSep(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());	
}

void CInsectsDoc::OnUpdateMorphOpen(CCmdUI* pCmdUI) 
{
	CPretreatment pre( m_pDibObject );	
	pCmdUI->Enable(pre.IsBlackandWhite256());	
}



void CInsectsDoc::OnHelp() 
{
	//�����ļ�
	::ShellExecute(0,"open",".\\help.chm","","",SW_SHOWNORMAL);	
}
