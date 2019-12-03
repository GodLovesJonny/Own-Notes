// DlgHuffman.cpp : implementation file
//

#include "stdafx.h"
#include "dip.h"
#include "DlgHuffman.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHuffman dialog


CDlgHuffman::CDlgHuffman(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHuffman::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgHuffman)
	m_ChannelSel = 0;
	m_fCodeEfficiency = 0.0f;
	m_fEntropy = 0.0f;
	m_fAvCodeLen = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgHuffman::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHuffman)
	DDX_Control(pDX, IDC_LST_Table, m_lstTable);
	DDX_Radio(pDX, IDC_RADIO_GRAY, m_ChannelSel);
	DDX_Text(pDX, IDC_EDIT_CODEEF, m_fCodeEfficiency);
	DDX_Text(pDX, IDC_EDIT_ENTROPY, m_fEntropy);
	DDX_Text(pDX, IDC_EDIT_CODELEN, m_fAvCodeLen);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHuffman, CDialog)
	//{{AFX_MSG_MAP(CDlgHuffman)
	ON_BN_CLICKED(IDC_RADIO_GREEN, OnRadioGreen)
	ON_BN_CLICKED(IDC_RADIO_BLUE, OnRadioBlue)
	ON_BN_CLICKED(IDC_RADIO_RED, OnRadioRed)
	ON_BN_CLICKED(IDC_RADIO_GRAY, OnRadioGray)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHuffman message handlers

void CDlgHuffman::Huffman()
{
	CString str;
	int i,j,x1,x2,c,p;
	int m1,m2;
	LV_ITEM lvitem;				//ListCtrl��ITEM
	int iActualItem;			//�м����������ListCtrl����ӵ�ITEM���
	HuffmanNode * huffNode;	//������ɹ��������Ľ��
	CString * strCode;		//ָ����ַ��Ĺ���������ָ��
	int *iMap;					//λ��ӳ��ָ��
	int iNumNode = 0;			//Ƶ����Ϊ��Ľ����(��Ҷ���)

	//ͳ��ֱ��ͼ������Ƶ����Ϊ��ĻҶ���
	for(i=0;i<m_iLeafNum;i++)
	{
		if(m_fFreq[i+m_ChannelSel*m_iLeafNum] != 0) iNumNode++;
	}

	//�����ڴ棬���ھ���n��Ҷ���Ĺ����������ԣ�������2n-1�����
	huffNode = new HuffmanNode [iNumNode*2-1];
	strCode = new CString [iNumNode];
	iMap = new int[iNumNode];
	//��ʼ��huffNode�еĸ����
	for(i=0;i<iNumNode*2-1;i++)
	{
		huffNode[i].Weight = 0;
		huffNode[i].Parent = 0;
		huffNode[i].Flag = 0;
		huffNode[i].LChild = -1;
		huffNode[i].RChild = -1;
	}

	//����iNumNode��Ҷ����Ȩֵ����˵Ƶ��,iMap�д�ŵ���Ƶ����Ϊ��ĻҶ�ֵ
	j = 0;
	for(i=0;i<m_iLeafNum;i++)
	{
		if(m_fFreq[i+m_ChannelSel*m_iLeafNum] != 0)
		{
			huffNode[j].Weight = m_fFreq[i+m_ChannelSel*m_iLeafNum];
			iMap[j] = i;
			j++;
		}
	}

	//�����������,��iNumNode��Ҷ��㣬�蹹��iNumNode-1��
	for(i=0;i<iNumNode-1;i++)
	{
		m1=m2=m_iSizeImage;//�Ƚ�����������ѡ��������С�����ı���
		x1=x2=0;//����ȷ����С�����������ڽ�������е����
		for(j=0;j<iNumNode+i;j++)
		{
			//�Ա��FlagΪ0�Ľ��Ƚ�����x1��m1��Ӧ��С��������x2��m2��֮
			if(huffNode[j].Weight<m1 && huffNode[j].Flag ==0 )
			{
				m2=m1;
				x2=x1;
				m1=huffNode[j].Weight;
				x1=j;
			}
			else if(huffNode[j].Weight<m2 && huffNode[j].Flag==0)
			{
				m2=huffNode[j].Weight;
				x2=j;
			}
		}
	
		//���ҳ���������С�����ϲ�Ϊһ������
		huffNode[x1].Parent = iNumNode+i;
		huffNode[x2].Parent = iNumNode+i;
		//FlagΪ1��˵���ý���Ѿ���Ϊ���������ӽ��
		huffNode[x1].Flag = 1;
		huffNode[x2].Flag = 1;
		huffNode[iNumNode+i].Weight = huffNode[x1].Weight+huffNode[x2].Weight;
		//����ֵ��Ȩֵ��С��������Ϊ������
		huffNode[iNumNode+i].LChild = x2;
		huffNode[iNumNode+i].RChild = x1;
	}

	//��ÿ��Ҷ���Ĺ���������
	for(i=0;i<iNumNode;i++)
	{
		c=i;
		p=huffNode[c].Parent;
		//��Ҷ������ݵ�����,��ֱ����ǰ��ĸ�����Ǹ����
		while(p != 0)
		{
			//������������������ĸ�һλ���0���������1
			if(huffNode[p].LChild == c) strCode[i]="0"+strCode[i];
			else	strCode[i]="1"+strCode[i];
			c=p;
			p=huffNode[c].Parent;
		}
	}

	//����ƽ�����ֳ��Ⱥ�ͼ����
	int CodeLen=0;
	m_fEntropy = 0.0;
	for (i = 0; i < iNumNode; i ++)
	{
		 CodeLen+= huffNode[i].Weight * strCode[i].GetLength();
		// ����ͼ����
		m_fEntropy -= ((double)huffNode[i].Weight/m_iSizeImage) * log((double)huffNode[i].Weight/m_iSizeImage) / log(2.0);
	}
	//����ƽ�����ֳ���
	m_fAvCodeLen = (double)CodeLen/m_iSizeImage;
	// �������Ч��
	m_fCodeEfficiency = m_fEntropy / m_fAvCodeLen;
	//���¶Ի����ϵ��ı����е�����
	UpdateData(FALSE);
	//���������������
	lvitem.mask = LVIF_TEXT;	// ������ʽΪ�ı�
	for (i = 0; i < iNumNode; i ++)
	{
		if(huffNode[i].Weight > 0)
		{
			// ���һ��
			lvitem.iItem = m_lstTable.GetItemCount();
			str.Format("%u",iMap[i]);
			lvitem.iSubItem = 0;
			lvitem.pszText= (LPTSTR)(LPCTSTR)str;
			iActualItem = m_lstTable.InsertItem(&lvitem);
			
			// ���������
			lvitem.iItem = iActualItem;
			
			// ��ӻҶ�ֵ���ֵ�Ƶ��
			lvitem.iSubItem = 1;
			str.Format("%f",(float)huffNode[i].Weight/m_iSizeImage);
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_lstTable.SetItem(&lvitem);
			
			// ��ӹ���������
			lvitem.iSubItem = 2;
			lvitem.pszText = (LPTSTR)(LPCTSTR)strCode[i];
			m_lstTable.SetItem(&lvitem);
			
			// ������ֳ���
			lvitem.iSubItem = 3;
			str.Format("%u",strCode[i].GetLength());
			lvitem.pszText = (LPTSTR)(LPCTSTR)str;
			m_lstTable.SetItem(&lvitem);
		}
	}
	delete[] huffNode;
	delete[] strCode;
	delete[] iMap;
}

BOOL CDlgHuffman::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// ����List�ؼ���ʽ
	m_lstTable.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
	// ��List�ؼ����Header
	m_lstTable.InsertColumn(0, "�Ҷ�ֵ", LVCFMT_LEFT, 60, 0);
	m_lstTable.InsertColumn(1, "����Ƶ��", LVCFMT_LEFT, 78, 0);
	m_lstTable.InsertColumn(2, "����������", LVCFMT_LEFT, 110, 1);
	m_lstTable.InsertColumn(3, "���ֳ���", LVCFMT_LEFT, 78, 2);

	Huffman();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgHuffman::OnRadioGreen() 
{
	m_lstTable.DeleteAllItems();
	UpdateData();

	Huffman();	
}

void CDlgHuffman::OnRadioBlue() 
{
	m_lstTable.DeleteAllItems();
	UpdateData();

	Huffman();	
}

void CDlgHuffman::OnRadioRed() 
{
	m_lstTable.DeleteAllItems();
	UpdateData();

	Huffman();	
	
}

void CDlgHuffman::OnRadioGray() 
{
	m_lstTable.DeleteAllItems();
	UpdateData();

	Huffman();	
}

