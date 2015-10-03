// XMLVisitor.h: interface for the XMLVisitor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLVISITOR_H__E31FDBD5_2F57_4278_9E9D_9F2D8D6B30A3__INCLUDED_)
#define AFX_XMLVISITOR_H__E31FDBD5_2F57_4278_9E9D_9F2D8D6B30A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class XMLVisitor  
{
public:
	virtual LPARAM VisitNode(IXMLElement* pElem, LPARAM p) = 0;
	XMLVisitor();
	virtual ~XMLVisitor();

};

#endif // !defined(AFX_XMLVISITOR_H__E31FDBD5_2F57_4278_9E9D_9F2D8D6B30A3__INCLUDED_)
