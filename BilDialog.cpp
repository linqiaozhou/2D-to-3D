// BilDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Graduation Design.h"
#include "BilDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBilDialog dialog


CBilDialog::CBilDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CBilDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBilDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBilDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBilDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CBilDialog, CDialog)
	//{{AFX_MSG_MAP(CBilDialog)
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBilDialog message handlers

void CBilDialog::OnOK() 
{
	// TODO: Add extra validation here
	m_nIndex=((CComboBox *)GetDlgItem(IDC_COMBO1))->GetCurSel();
	CDialog::OnOK();

}

BOOL CBilDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
((CComboBox *)GetDlgItem(IDC_COMBO1))->AddString("3*3");
((CComboBox *)GetDlgItem(IDC_COMBO1))->AddString("5*5");
((CComboBox *)GetDlgItem(IDC_COMBO1))->AddString("7*7");
((CComboBox *)GetDlgItem(IDC_COMBO1))->SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
