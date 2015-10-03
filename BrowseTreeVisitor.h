// BrowseTreeVisitor.h: interface for the BrowseTreeVisitor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BROWSETREEVISITOR_H__7E36A6C0_9E96_4ACA_B3A5_C77E6258AEA7__INCLUDED_)
#define AFX_BROWSETREEVISITOR_H__7E36A6C0_9E96_4ACA_B3A5_C77E6258AEA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XMLDoc.h"

class BrowseTreeVisitor : public XMLVisitor  
{
public:
	virtual LPARAM VisitNode(IXMLElement* pElem, LPARAM p);
	CTreeCtrl *m_pTree;
	TV_INSERTSTRUCT tvinsert;
	BrowseTreeVisitor(CTreeCtrl *pTree);
	virtual ~BrowseTreeVisitor();
};

#endif // !defined(AFX_BROWSETREEVISITOR_H__7E36A6C0_9E96_4ACA_B3A5_C77E6258AEA7__INCLUDED_)
