// BrowseTree.cpp : implementation file
//

#include "stdafx.h"
#include "VNotes.h"
#include "BrowseTree.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// BrowseTree

IMPLEMENT_DYNCREATE(BrowseTree, CTreeView)

BrowseTree::BrowseTree()
{
	m_previousItem = NULL;
	m_contextItem = NULL;
	b_onUpdate = 0;
}

BrowseTree::~BrowseTree()
{

}


BEGIN_MESSAGE_MAP(BrowseTree, CTreeView)
	//{{AFX_MSG_MAP(BrowseTree)
	ON_WM_CREATE()
	ON_COMMAND(ID_TREEITEM_INSERT, OnTreeitemInsert)
	ON_COMMAND(ID_TREEITEM_DELETE, OnTreeitemDelete)
	ON_COMMAND(ID_EDIT_NEWSUBNOTE, OnEditNewSubnote)
	ON_COMMAND(ID_PO2_NEWSUBNOTE, OnPo2Newsubnote)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	ON_COMMAND(ID_EDIT_NEWTOPNOTE, OnEditNewtopnote)
	ON_COMMAND(ID_VIEW_NOTE_TXT, OnViewNoteTxt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// BrowseTree drawing

/////////////////////////////////////////////////////////////////////////////
// BrowseTree diagnostics

#ifdef _DEBUG
void BrowseTree::AssertValid() const
{
	CTreeView::AssertValid();
}

void BrowseTree::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// BrowseTree message handlers

int BrowseTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	CTreeView::ModifyStyle(TVS_EDITLABELS, TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_NOTOOLTIPS); 
	const int IMAGE_NUM = 2;
	HICON hIcon[IMAGE_NUM];
	int n;
	m_imageList.Create(16, 16, 0, IMAGE_NUM, IMAGE_NUM); // 32, 32 for large icons
	hIcon[0] = AfxGetApp()->LoadIcon(IDI_ICON1);
	hIcon[1] = AfxGetApp()->LoadIcon(IDI_ICON2);
	for (n = 0; n < IMAGE_NUM; n++) {
		m_imageList.Add(hIcon[n]);
	}
	CTreeCtrl& pTree = GetTreeCtrl();
	pTree.SetImageList(&m_imageList, TVSIL_NORMAL);
	return 0;
}

CVNotesDoc* BrowseTree::GetDocument()
{
	// we overide GetDocument here
	CMainFrame* mFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	return (CVNotesDoc*)mFrame->GetActiveDocument();
}

CVNotesView* BrowseTree::GetNoteDetailsView()
{
	return ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetDetailsView();
}

void BrowseTree::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	b_onUpdate++; // we use it as a semaphore
	CVNotesDoc* pDoc = BrowseTree::GetDocument();
	if((pDoc == NULL) || (pDoc->m_xmlDoc == NULL)){
		b_onUpdate--;
		return;
	}
	CTreeCtrl& pTree = GetTreeCtrl();
	ClearTree();
	BrowseTreeVisitor visitor(&pTree);
	pDoc->m_xmlDoc->VisitNodes(visitor);
	b_onUpdate--;
}

void BrowseTree::ClearTree()
{
	b_onUpdate++;
	CTreeCtrl& pTree = GetTreeCtrl();
	m_previousItem = NULL;
	m_contextItem = NULL;
	HTREEITEM next = SafeDeleteNoteItem(pTree, pTree.GetRootItem());
	while(next != NULL){
		next = SafeDeleteNoteItem(pTree, next);
	}
	pTree.DeleteAllItems();
	b_onUpdate--;
}

void BrowseTree::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(b_onUpdate != 0){
		*pResult = 0;
		return;
	}
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	CVNotesView* cDetails = GetNoteDetailsView();
	cDetails->EnableDetails(TRUE);
	// save changes to previously selected note
	SaveIfModified();
	// show details for the new selected note
	CTreeCtrl& pTree = GetTreeCtrl();
	if(cDetails != NULL) cDetails->ClearData();
	HTREEITEM hSelected = pNMTreeView->itemNew.hItem;
	if(hSelected != NULL){
		IXMLElement *pElem = (IXMLElement*)pTree.GetItemData(hSelected);
		if(pElem != NULL){
			TRACE(_T("\nTRACE: Update %s %h %h"), pTree.GetItemText(hSelected), hSelected);
			cDetails->UpdateDetails(pElem, FALSE);
		}
	}
	m_previousItem = hSelected; // remeber who was selected to save changes
	SetContextItem(hSelected);
	*pResult = 0;
}

void BrowseTree::SaveIfModified()
{
	CTreeCtrl& pTree = GetTreeCtrl();
	CVNotesView* cDetails = GetNoteDetailsView();
	if(m_previousItem != NULL){
		IXMLElement *pElem = (IXMLElement*)pTree.GetItemData(m_previousItem);
		if(pElem == NULL) return;
		cDetails->UpdateDetails(pElem, TRUE);
		TRACE(_T("\nTRACE: SaveIfModified() %s"), cDetails->GetNoteTitle());
		pTree.SetItemText(m_previousItem, cDetails->GetNoteTitle());
	}
}

void BrowseTree::InitEmptyItem(TV_INSERTSTRUCT *tvinsert, HTREEITEM parent)
{
		if(tvinsert == NULL) return;	
		tvinsert->hParent = parent;
		tvinsert->hInsertAfter = TVI_LAST;
		tvinsert->item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
		tvinsert->item.hItem = NULL; 
		tvinsert->item.state = 0;
		tvinsert->item.stateMask = 0;
		tvinsert->item.cchTextMax = 6;
		tvinsert->item.iSelectedImage = 1;
		tvinsert->item.cChildren = 0;
		tvinsert->item.lParam = 0;
		tvinsert->item.iImage = 0;
}

void BrowseTree::OnTreeitemInsert() 
{
	CTreeCtrl& pTree = GetTreeCtrl();

	IXMLElement* pElem = NULL;
	if(m_contextItem != NULL){
		// no need to release pElem
		pElem = (IXMLElement*)pTree.GetItemData(m_contextItem);
	}
	IXMLElement* pNewElem = XMLAddNewSubNode(pElem);

	// this is simpler but removes focus from new note
	// GetDocument()->UpdateAllViews(NULL);
	// thus we work a turnaround here
	if(pNewElem != NULL){
		TV_INSERTSTRUCT tvinsert;
		BrowseTree::InitEmptyItem(&tvinsert, m_contextItem);
		tvinsert.item.pszText = NoteTag::NEW_NOTE_NAME;
		
		HTREEITEM newItem = pTree.InsertItem(&tvinsert);
		if(newItem != NULL){
			pNewElem->AddRef();
			pTree.SetItemData(newItem, (DWORD)pNewElem);
			pTree.SelectItem(newItem);
			GetDocument()->SetModifiedFlag();
			return; // all ok
		} else {
			XMLDeleteNode(pNewElem); // delete the new note
		}
	}
	AfxMessageBox(IDS_NOTE_CREATE_ERR);
}

void BrowseTree::OnTreeitemDelete() 
{
	if(m_contextItem == NULL){
		AfxMessageBox(IDS_NO_NOTE_SELECTED_ERR);
		return;
	}
	CTreeCtrl& m_pTree = GetTreeCtrl();
	// selectitem is necessary here, otherwise onselectchange
	// will cause violation during tree delete
	m_pTree.SelectItem(m_contextItem);

	CString prompt;
	prompt.LoadString(IDS_NOTE_DELETE_CONFIRM);
	prompt += m_pTree.GetItemText(m_contextItem);
	if(AfxMessageBox(prompt, MB_YESNO | MB_ICONEXCLAMATION) == IDYES){
		IXMLElement *pElem = (IXMLElement*)m_pTree.GetItemData(m_contextItem);
		if(XMLDeleteNode(pElem)){
			// pElem is no more valid
			m_pTree.SetItemData(m_contextItem, (DWORD)NULL);
			HTREEITEM item = m_contextItem;
			m_contextItem = NULL;
			m_previousItem = NULL;
			CVNotesView* cDetails = GetNoteDetailsView();
			cDetails->ClearData();
			if(m_lastFoundItem == item) m_lastFoundItem = NULL;
			SafeDeleteNoteItem(m_pTree, item);
			m_pTree.DeleteItem(item);
			GetDocument()->SetModifiedFlag();
			if(m_pTree.GetCount() <= 0){
				cDetails->EnableDetails(FALSE);
			}
		} else AfxMessageBox(IDS_NOTE_DELETE_ERR, MB_OK);
	}
}

// delegate action calls to document

IXMLElement* BrowseTree::XMLAddNewSubNode(IXMLElement *pElem)
{
	return (GetDocument()->m_xmlDoc)->AddNewElement(pElem);
}

BOOL BrowseTree::XMLDeleteNode(IXMLElement *pElem)
{
	return (GetDocument()->m_xmlDoc)->DeleteElement(pElem);
}

// menu event handlers

void BrowseTree::OnEditNewSubnote() 
{
	SetContextItem(GetTreeCtrl().GetSelectedItem());
	OnTreeitemInsert();
}

void BrowseTree::OnEditNewtopnote() 
{
	OnPo2Newsubnote();
}

void BrowseTree::OnPo2Newsubnote() 
{
	// free pop menu New Note adds it at root level
	SetContextItem((HTREEITEM)NULL);
	OnTreeitemInsert();
}

void BrowseTree::SetContextItem(HTREEITEM item)
{
	m_contextItem = item;
}

void BrowseTree::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	switch(pTVKeyDown->wVKey){
		case VK_DELETE:
			OnTreeitemDelete();
			break;
		case VK_INSERT:
			OnTreeitemInsert();
			break;
		case VK_F1:
			OnViewNoteTxt();
			break;
		case VK_SPACE:
			OnPo2Newsubnote();
			break;
		case VK_TAB:
			GetNoteDetailsView()->TransferFocus();
			break;
	}
	*pResult = 0;
}

// safe release before delete, releases 
// com IXMLElemente pointers that were AddRef by BrowseTreeVisitor

HTREEITEM BrowseTree::SafeDeleteNoteItem(CTreeCtrl& pTree, HTREEITEM item){
	if(item == NULL) return NULL;
	HTREEITEM next = pTree.GetNextSiblingItem(item);
	if((item == m_previousItem) || (next == m_previousItem))
		m_previousItem = NULL;

	if(pTree.ItemHasChildren(item)){
		HTREEITEM ci1 = pTree.GetChildItem(item);
		while(ci1 != NULL){
			ci1 = SafeDeleteNoteItem(pTree, ci1);
		}
	}
	TRACE(_T("\nTRACE: About to Release: %s %h"), pTree.GetItemText(item), item);
	IXMLElement *pElem = (IXMLElement*)pTree.GetItemData(item);
	if(pElem != NULL){
		pElem->Release();
		pElem = NULL;
	}
	pTree.SetItemData(item, (DWORD)NULL);
	return next;
}

// search support

HTREEITEM BrowseTree::SearchTree(CTreeCtrl &pTree, HTREEITEM item, HTREEITEM *next, CString &itemTitle, BOOL caseSensitive)
{
	if(item == NULL) return NULL;
	*next = pTree.GetNextSiblingItem(item);
	CString title = pTree.GetItemText(item);
	if(!caseSensitive) title.MakeUpper();
	if(title.Find(itemTitle) != -1){
		if(ShouldReportFoundNote(item)) return item;	
	}
	if(pTree.ItemHasChildren(item)){
		HTREEITEM ci1 = pTree.GetChildItem(item);
		HTREEITEM nextC;
		do{
			nextC = NULL;
			HTREEITEM found = SearchTree(pTree, ci1, &nextC, itemTitle, caseSensitive);
			if(ShouldReportFoundNote(found)) return found;
			ci1 = nextC;
		} while(nextC != NULL);
	}
	return NULL;
}

BOOL BrowseTree::ShouldReportFoundNote(HTREEITEM found)
{
	if(found != NULL){
		if(m_lastFoundItem != NULL){
			if(found != m_lastFoundItem){
				if(m_lastFoundSkipped)
					return TRUE;
			} else {
					m_lastFoundSkipped = TRUE;
			}
		} else {
			if(m_lastFoundItem != NULL){
				if(m_lastFoundSkipped) return TRUE;
			} else return TRUE;
		}
	}
	return FALSE; // do not report
}

BOOL BrowseTree::FindNote(CString title, BOOL findNext, BOOL caseSensitive)
{
	CTreeCtrl& pTree = GetTreeCtrl();
	// reset state
	if((!findNext) || (m_lastFoundItem == NULL)){
		m_lastFoundItem = NULL;
		m_lastFoundRoot = pTree.GetRootItem();
	}
	m_lastFoundSkipped = FALSE;
	HTREEITEM item = m_lastFoundRoot;
	HTREEITEM nextRoot;
	do{
		nextRoot = NULL;
		m_lastFoundRoot = item;
		HTREEITEM found = SearchTree(pTree, item, &nextRoot, title, caseSensitive);
		if(found != NULL){
			m_lastFoundItem = found;
			pTree.SelectItem(found);
			return TRUE;
		}
		item = nextRoot;
	} while(nextRoot != NULL);
	AfxMessageBox(IDS_NO_MORE_NOTES_FIND);
	return FALSE;
}

void BrowseTree::OnViewNoteTxt() 
{
	SaveIfModified();
	Note n;
	CTreeCtrl& pTree = GetTreeCtrl();
	HTREEITEM item = m_contextItem; //pTree.GetSelectedItem();
	if(item == NULL) return;
	IXMLElement *pElem = (IXMLElement*)pTree.GetItemData(item);
	if(pElem != NULL){
		if(!XMLDoc::GetNoteData(pElem, &n)) return;
		CString note = Note::Note2Text(n);
		TCHAR bPath[MAX_PATH], bFile[MAX_PATH];
		ZeroMemory((void *)bPath, MAX_PATH);
		ZeroMemory((void *)bFile, MAX_PATH);
		if(::GetTempPath(MAX_PATH, bPath) == 0) return;
		if(::GetTempFileName(bPath, _T("vno"), 0, bFile) == 0) return;
		try {
			UINT VNTDelete( LPVOID);
			CString fName(bFile);
			fName += _T(".txt");
			CFile f(fName, CFile::modeCreate | CFile::modeWrite);
			f.Write((LPCTSTR)note, note.GetLength());
			f.Close();
			::ShellExecute(NULL, _T("Open"), fName, NULL, NULL, SW_SHOWNORMAL);
			// ->this is simple and mostly unnoticed in gui;
			/*
			::Sleep(700);
			::DeleteFile(fName);
			::DeleteFile(bFile); // see GetTempFileName in msdn
			*/
			// ->but this is better, thought a bit more involved
			CString s = fName + CString(_T('|')) + bFile;
			// thread stack memory is not in this local function stack
			// so we need to copy data in heap, this memory should be
			// freed in thread function
			LPCTSTR cc = (LPCTSTR)s;
			int bufLen = (lstrlen(cc)  + 1) / sizeof(char); // if ever sizeof char != 1
			char* buf = new char[bufLen];
			ZeroMemory((void*)buf, bufLen);
			memcpy((void*)buf, (const void*)cc, bufLen);
			AfxBeginThread(VNTDelete, (LPVOID)buf);
		} catch(...){
			return;
		}
		
	}
}

UINT VNTDelete(LPVOID pParam){
	if(pParam == 0) return 0;
	PTCHAR c = (PTCHAR)pParam;
	CString s(c);
	TRACE(_T("\nTRACE: VNTDelete %s"), s);
	int i = -1, lastI = 0;
	::Sleep(700);
	do{
		int start, count;
		i = s.Find(_T('|'), lastI);
		if(i != -1){
			start = lastI;
			count = i - lastI;
		} else {
			start = lastI;
			count = s.GetLength() - lastI;
		}
		lastI = i + 1;
		CString f = s.Mid(start, count);
		TRACE(_T("\nTRACE: Deleting temp file: %s\n"), f);
		::DeleteFile(f);
	}while(i != -1);
	// we need to free global memory here
	delete (char*)c;
	return 0;
}


