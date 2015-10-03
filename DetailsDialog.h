#if !defined(AFX_DETAILSDIALOG_H__E4B6916D_92AF_4441_879A_596AD6C6F6D1__INCLUDED_)
#define AFX_DETAILSDIALOG_H__E4B6916D_92AF_4441_879A_596AD6C6F6D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetailsDialog.h : header file
//

#include "Note.h"

/////////////////////////////////////////////////////////////////////////////
// CDetailsDialog dialog

class CDetailsDialog : public CDialog
{
// Construction
public:
	void SetFieldsFromNote(Note& n, BOOL setAll);
	void ClearModified();
	BOOL IsTextModified(UINT ID);
	virtual void  OnCancel( );
	CDetailsDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDetailsDialog)
	enum { IDD = IDD_FORMVIEW };
	CEdit	m_TitleC;
	CEdit	m_CommentC;
	CEdit	m_BodyC;
	CString	m_body;
	CString	m_comment;
	CString	m_created;
	CString	m_modified;
	CString m_title;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDetailsDialog)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDetailsDialog)
	afx_msg void OnChangeBody();
	afx_msg void OnChangeTitle();
	afx_msg void OnChangeComment();
	afx_msg void OnUpdateTitle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void SetModified();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETAILSDIALOG_H__E4B6916D_92AF_4441_879A_596AD6C6F6D1__INCLUDED_)
