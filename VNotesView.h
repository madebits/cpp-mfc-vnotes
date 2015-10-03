// VNotesView.h : interface of the CVNotesView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VNOTESVIEW_H__C5080C84_EB61_46C7_B3D5_4753082DF684__INCLUDED_)
#define AFX_VNOTESVIEW_H__C5080C84_EB61_46C7_B3D5_4753082DF684__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DetailsDialog.h"

class CVNotesView : public CView
{
protected: // create from serialization only
	CVNotesView();
	DECLARE_DYNCREATE(CVNotesView)

// Attributes
public:
	CVNotesDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVNotesView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void TransferFocus();
	void EnableDetails(BOOL enable);
	void ClearData();
	BOOL IsNoteModfied();
	CString GetNoteTitle();
	void UpdateDetails(IXMLElement* pElem, BOOL direction);
	virtual ~CVNotesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CVNotesView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDetailsDialog m_workPane;
};

#ifndef _DEBUG  // debug version in VNotesView.cpp
inline CVNotesDoc* CVNotesView::GetDocument()
   { return (CVNotesDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VNOTESVIEW_H__C5080C84_EB61_46C7_B3D5_4753082DF684__INCLUDED_)
