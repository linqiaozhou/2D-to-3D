#if !defined(AFX_BILDIALOG_H__93FA0D95_57F7_402A_B2F0_49655B6AD727__INCLUDED_)
#define AFX_BILDIALOG_H__93FA0D95_57F7_402A_B2F0_49655B6AD727__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BilDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBilDialog dialog

class CBilDialog : public CDialog
{
// Construction
public:
	CBilDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBilDialog)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
public:
	CComboBox	m_nCombo;//感觉没什么用
	int	m_nIndex;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBilDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBilDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BILDIALOG_H__93FA0D95_57F7_402A_B2F0_49655B6AD727__INCLUDED_)
