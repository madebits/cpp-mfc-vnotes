#if !defined(AFX_BROWSETREE_H__44B9323C_77DB_43C2_96AE_828FEB8361BD__INCLUDED_)
#define AFX_BROWSETREE_H__44B9323C_77DB_43C2_96AE_828FEB8361BD__INCLUDED_


#include "VNotesDoc.h"
#include "BrowseTreeVisitor.h"
#include "VNotesView.h"

//#include "VNotesView.h"

extern CVNotesApp theApp;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BrowseTree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// BrowseTree view

class BrowseTree : public CTreeView
{
protected:
	BrowseTree();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(BrowseTree)

// Attributes
public:

// Operations
public:
	static void InitEmptyItem(TV_INSERTSTRUCT* tvinsert, HTREEITEM parent);
	BOOL FindNote(CString title, BOOL findNext, BOOL caseSensitive);
	void SaveIfModified();
	CVNotesDoc* GetDocument();
	void SetContextItem(HTREEITEM item);
	void ClearTree();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(BrowseTree)
	public:
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~BrowseTree();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(BrowseTree)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTreeitemInsert();
	afx_msg void OnTreeitemDelete();
	afx_msg void OnEditNewSubnote();
	afx_msg void OnPo2Newsubnote();
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditNewtopnote();
	afx_msg void OnViewNoteTxt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL ShouldReportFoundNote(HTREEITEM found);
	BOOL m_lastFoundSkipped;
	HTREEITEM m_lastFoundRoot;
	HTREEITEM m_lastFoundItem;
	HTREEITEM SearchTree(CTreeCtrl& pTree, HTREEITEM item, HTREEITEM* next, CString& itemTitle, BOOL caseSensitive);
	HTREEITEM SafeDeleteNoteItem(CTreeCtrl& pTree, HTREEITEM item);
	CVNotesView* GetNoteDetailsView();
	// b_onUpdate is used not to process SelectChange(s) 
	// events during OnUpdate, when b_onUpdate > 0;
	int b_onUpdate;
	BOOL XMLDeleteNode(IXMLElement* pElem);
	IXMLElement* XMLAddNewSubNode(IXMLElement *pElem);
	HTREEITEM m_previousItem;
	HTREEITEM m_contextItem; // current selected item, can be NULL
	CImageList m_imageList;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BROWSETREE_H__44B9323C_77DB_43C2_96AE_828FEB8361BD__INCLUDED_)
