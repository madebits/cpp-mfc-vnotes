// VNotesView.cpp : implementation of the CVNotesView class
//

#include "stdafx.h"
#include "VNotes.h"

#include "VNotesDoc.h"
#include "VNotesView.h"
#include "Note.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVNotesView

IMPLEMENT_DYNCREATE(CVNotesView, CView)

BEGIN_MESSAGE_MAP(CVNotesView, CView)
	//{{AFX_MSG_MAP(CVNotesView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVNotesView construction/destruction

CVNotesView::CVNotesView()
{
	// TODO: add construction code here

}

CVNotesView::~CVNotesView()
{
}

BOOL CVNotesView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CVNotesView drawing

void CVNotesView::OnDraw(CDC* pDC)
{
	CVNotesDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

/////////////////////////////////////////////////////////////////////////////
// CVNotesView diagnostics

#ifdef _DEBUG
void CVNotesView::AssertValid() const
{
	CView::AssertValid();
}

void CVNotesView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVNotesDoc* CVNotesView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVNotesDoc)));
	return (CVNotesDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVNotesView message handlers

int CVNotesView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_workPane.Create(IDD_FORMVIEW, this);
	EnableDetails(FALSE);
	return 0;
}

void CVNotesView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	CRect rectClient;
    GetClientRect(rectClient);
	m_workPane.MoveWindow(0, 0, rectClient.right - rectClient.left, rectClient.bottom - rectClient.top, true);
	CEdit* body = (CEdit*)m_workPane.GetDlgItem(IDC_BODY);
	CRect rect;
	body->GetWindowRect(rect);
	m_workPane.ScreenToClient(rect);
	body->MoveWindow(0, rect.top, rectClient.right - rectClient.left, rectClient.bottom - rect.top, true);
}

CString CVNotesView::GetNoteTitle()
{
	return m_workPane.m_title;
}

void CVNotesView::UpdateDetails(IXMLElement *pElem, BOOL direction)
{
	if(pElem == NULL) return;
	if(direction){ // save data to doc
		m_workPane.UpdateData(TRUE);
		if(IsNoteModfied()){
			Note n;
			n.m_title = m_workPane.m_title;
			n.m_comment = m_workPane.m_comment;
			n.m_body = m_workPane.m_body;
			BOOL updateBody = m_workPane.IsTextModified(IDC_BODY);
			GetDocument()->m_xmlDoc->SetNoteData(pElem, &n, updateBody);
			GetDocument()->SetModifiedFlag();
			m_workPane.SetFieldsFromNote(n, FALSE);
			m_workPane.UpdateData(FALSE);
			m_workPane.ClearModified();
		}
	} else { // read data from doc
		Note n;
		if(!XMLDoc::GetNoteData(pElem, &n)) return;
		m_workPane.SetFieldsFromNote(n, TRUE);
		m_workPane.UpdateData(FALSE);
	}
}

BOOL CVNotesView::IsNoteModfied()
{
	if(
		m_workPane.IsTextModified(IDC_TITLE) ||
		m_workPane.IsTextModified(IDC_COMMENT) ||
		m_workPane.IsTextModified(IDC_BODY)
	) return TRUE;
	return FALSE;
}

void CVNotesView::ClearData()
{
	m_workPane.m_title = _T("");
	m_workPane.m_comment = _T("");
	m_workPane.m_created = _T("");
	m_workPane.m_modified = _T("");
	m_workPane.m_body = _T("");
	m_workPane.UpdateData(FALSE);
}

void CVNotesView::EnableDetails(BOOL enable)
{
	m_workPane.EnableWindow(enable);
}

void CVNotesView::TransferFocus()
{
	m_workPane.m_TitleC.SetFocus();
}
