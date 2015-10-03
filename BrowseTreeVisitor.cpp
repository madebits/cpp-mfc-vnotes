// BrowseTreeVisitor.cpp: implementation of the BrowseTreeVisitor class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "VNotes.h"
#include "BrowseTreeVisitor.h"
#include "BrowseTree.h"
#include "Note.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BrowseTreeVisitor::BrowseTreeVisitor(CTreeCtrl *pTree)
{
	m_pTree = pTree;
	BrowseTree::InitEmptyItem(&tvinsert, NULL);
}

BrowseTreeVisitor::~BrowseTreeVisitor()
{

}

LPARAM BrowseTreeVisitor::VisitNode(IXMLElement *pElem, LPARAM p)
{
 	if(pElem == NULL) return (LPARAM)NULL;
 	BSTR bstrString;
 	pElem->get_tagName(&bstrString);
 	_bstr_t tagName(bstrString);
	SysFreeString(bstrString);
 	TRACE0(tagName);
 	if(lstrcmp((TCHAR*)tagName, NoteTag::NOTE) == 0){
		_bstr_t titleAttribute(NoteTag::TITLE);
 		_variant_t title;
 		pElem->getAttribute(titleAttribute, &title);
		_bstr_t bRawTitle(title);
		_bstr_t bTitle(XMLDoc::DecodeData((LPCTSTR)bRawTitle));
		// get parent
		tvinsert.hParent = (HTREEITEM)p;
		tvinsert.item.pszText = bTitle;
		HTREEITEM thisItem = m_pTree->InsertItem(&tvinsert);
		if(thisItem != NULL){
			m_pTree->SetItemData(thisItem, (DWORD)pElem);
			pElem->AddRef(); // keeps it from destruction
			// must call Release when done
		}
 		return (LPARAM)thisItem;
 	}
	return (LPARAM)NULL;
}