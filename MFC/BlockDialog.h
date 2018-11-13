#if !defined(AFX_BLOCKDIALOG_H__ED9B6B23_A565_43EC_BCDA_8912837CB896__INCLUDED_)
#define AFX_BLOCKDIALOG_H__ED9B6B23_A565_43EC_BCDA_8912837CB896__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BlockDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBlockDialog dialog

class CBlockDialog : public CDialog
{
// Construction
public:
	CBlockDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBlockDialog)
	enum { IDD = IDD_DIALOG2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

public:
	CComboBox	m_nCombo1;
	int	m_nIndex1;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlockDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBlockDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLOCKDIALOG_H__ED9B6B23_A565_43EC_BCDA_8912837CB896__INCLUDED_)
