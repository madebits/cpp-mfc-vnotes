#if !defined(AFX_FINDNOTEDIALOG_H__0CD1EDD2_82B9_4E8D_90DD_57684C48E866__INCLUDED_)
#define AFX_FINDNOTEDIALOG_H__0CD1EDD2_82B9_4E8D_90DD_57684C48E866__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindNoteDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFindNoteDialog dialog

class CFindNoteDialog : public CDialog
{
// Construction
public:
	CFindNoteDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFindNoteDialog)
	enum { IDD = IDD_DIALOG_FIND };
	CButton	m_findNextButton;
	CString	m_FindText;
	BOOL	m_caseSensitive;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindNoteDialog)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFindNoteDialog)
	afx_msg void OnFind();
	afx_msg void OnFindNext();
	virtual void OnCancel();
	afx_msg void OnCheckCase();
	afx_msg void OnChangeEditFind();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL Find(BOOL next);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDNOTEDIALOG_H__0CD1EDD2_82B9_4E8D_90DD_57684C48E866__INCLUDED_)
