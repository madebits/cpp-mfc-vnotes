// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "VNotes.h"

#include "MainFrm.h"
#include "VNotesDoc.h"
#include "FindNoteDialog.h"
#include "TextVisitor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_FIND_NOTE, OnFindNote)
	ON_COMMAND(ID_EXPORT_TEXT, OnExportText)
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("TRACE: Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("TRACE: Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	VERIFY(m_wndSplitter.CreateStatic(this, 1, 2));
	VERIFY(m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(BrowseTree), CSize(200, 100), pContext));
    VERIFY(m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CVNotesView), CSize(100, 100), pContext));
    return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnContextMenu(CWnd* hWnd, CPoint point){
 	// since OnContextMenu is for frames, hWnd is of CSplitWindow
	// we need it on the top level, thus
	TRACE(_T("\nHWND: %x"), hWnd->m_hWnd);
	CWnd* paneLeft = m_wndSplitter.GetPane(0, 0);
	CWnd* paneRight = m_wndSplitter.GetPane(0, 1);

	// check to see if it is in treeview
	CRect rectClient;
	paneLeft->GetClientRect(rectClient);
	CPoint local = point;
	paneLeft->ScreenToClient(&local);
	if(rectClient.PtInRect(local)){ 	
		CTreeCtrl& pTree = ((CTreeView*)paneLeft)->GetTreeCtrl();
		CPoint local = point;
 		pTree.ScreenToClient(&local);
 		UINT pFlags = 0;
 		HTREEITEM item = pTree.HitTest(local, &pFlags);
 		if(item != NULL){
 			TRACE(_T("\nTRACE: ITEM HIT: %s"), pTree.GetItemText(item));
			BrowseTree* b = (BrowseTree*)paneLeft;
			b->SetContextItem(item);
			// we choose to select the item here
			pTree.SelectItem(item);
			CMenu menu;
		    menu.LoadMenu(IDR_TREEITEM_POP_MENU);
			menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
				point.x, point.y, this);
		} else {
			CMenu menu;
		    menu.LoadMenu(IDR_TREEITEM_POP_MENU);
			menu.GetSubMenu(1)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
				point.x, point.y, this);
		}
	} else { // check to see if in formview
		CPoint local = point;
		paneRight->ScreenToClient(&local);
		CRect rectClient;
		paneRight->GetClientRect(rectClient);
		if(rectClient.PtInRect(local)){
			// if we ever need this
		} else { // outside known world
			CFrameWnd::OnContextMenu(hWnd, point);
		}
	}
}

// shortcut methods to access the views

CVNotesView* CMainFrame::GetDetailsView()
{
	return (CVNotesView*)m_wndSplitter.GetPane(0, 1);
}

BrowseTree* CMainFrame::GetLeftTreeView()
{
	return (BrowseTree*)m_wndSplitter.GetPane(0, 0);
}

void CMainFrame::OnFindNote() 
{
	static CFindNoteDialog fnd;
	fnd.DoModal();
}

void CMainFrame::OnExportText() 
{
	BrowseTree* btv = GetLeftTreeView();
	if(btv != NULL) btv->SaveIfModified();
	CFileDialog fd(FALSE, _T("*.txt"), _T("vn") + XMLDoc::GetTimeString() + _T(".txt"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"), this);
	if(fd.DoModal() == IDCANCEL) return;
	TextVisitor visitor(fd.GetPathName());
	if(visitor.IsInitialized()){
		CVNotesDoc* doc = (CVNotesDoc*)GetActiveDocument();
		if((doc != NULL) && (doc->m_xmlDoc != NULL))
			doc->m_xmlDoc->VisitNodes(visitor);	
	}
}
