// BlockDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Graduation Design.h"
#include "BlockDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlockDialog dialog


CBlockDialog::CBlockDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CBlockDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBlockDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBlockDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlockDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBlockDialog, CDialog)
	//{{AFX_MSG_MAP(CBlockDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlockDialog message handlers

void CBlockDialog::OnOK() 
{
	// TODO: Add extra validation here
	m_nIndex1=((CComboBox *)GetDlgItem(IDC_COMBO2))->GetCurSel();
	CDialog::OnOK();

}
BOOL CBlockDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
((CComboBox *)GetDlgItem(IDC_COMBO2))->AddString("2*2");
((CComboBox *)GetDlgItem(IDC_COMBO2))->AddString("4*4");
((CComboBox *)GetDlgItem(IDC_COMBO2))->AddString("8*8");
((CComboBox *)GetDlgItem(IDC_COMBO2))->SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

