// XMLDoc.cpp: implementation of the XMLDoc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VNotes.h"
#include "XMLDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XMLDoc::XMLDoc()
{
	m_pDoc = NULL;
	m_xmlUnknownFormat = TRUE; // unknown
	HRESULT hr;
	hr = CoInitialize(NULL);
	if(SUCCEEDED(hr)){
		m_ComOK = true;
	} else {
		m_ComOK = false;
		AfxMessageBox(IDS_XML_COM_ERR);
	}
}

XMLDoc::~XMLDoc()
{
	if(m_ComOK){
		SAFERELEASE(m_pDoc);
		CoUninitialize();
		m_ComOK = false;
	}
	TRACE0(_T("\nTRACE: XMLDoc::~XMLDoc()"));
}

BOOL XMLDoc::IsFormatSupported()
{
	return !m_xmlUnknownFormat;
}

BOOL XMLDoc::InitFromURL(CArchive& ar)
{
	m_xmlUnknownFormat = FALSE;
    IXMLDocument *pDoc = NULL;
    IPersistStreamInit *pIPersistStreamInit = NULL;
    HRESULT hr;
	if(!ar.IsLoading()) return FALSE;
	SAFERELEASE(m_pDoc);
	if(!CreateDocument(&pDoc)) return FALSE;
	m_pDoc = pDoc;
	hr = pDoc->QueryInterface(IID_IPersistStreamInit, (void **)&pIPersistStreamInit);
	if(SUCCEEDED(hr)){
		CArchiveStream as(&ar);
		IStream* pIStream = (IStream*)&as;
		hr = pIPersistStreamInit->Load(pIStream);
		SAFERELEASE(pIPersistStreamInit);
		SAFERELEASE(pIStream);
		if(SUCCEEDED(hr)){
			return TRUE;
		} else ShowXMLError();
	}
	m_pDoc = NULL;
	return FALSE;
}

BOOL XMLDoc::CreateDocument(IXMLDocument** ppDoc)
{
	IXMLDocument* pDoc = NULL;
    HRESULT hr = CoCreateInstance(CLSID_XMLDocument, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDocument, (void**)&pDoc);	
	if(SUCCEEDED(hr) && (pDoc != NULL)){
		*ppDoc = pDoc;
		return TRUE;
	}
	AfxMessageBox(IDS_XML_DOM_ERR);
	return FALSE;
}

void XMLDoc::ShowXMLError()
{
	IXMLDocument* pDoc = m_pDoc;
	if(pDoc == NULL) return;
	IXMLError *pXMLError = NULL ;
    XML_ERROR xmle;
    HRESULT hr = pDoc->QueryInterface(IID_IXMLError, (void **)&pXMLError);
	if(!SUCCEEDED(hr)){
		AfxMessageBox(IDS_XML_ERR);
		return;
	}
    hr = pXMLError->GetErrorInfo(&xmle);
	if(!SUCCEEDED(hr)){
		SAFERELEASE(pXMLError);
		AfxMessageBox(IDS_XML_ERR);
		return;
	}
	TCHAR buf[4024];
	ZeroMemory(buf, 1024);
    wsprintf(buf, _T("Error on line %d char %d.\nFound %s while expecting %s"),
		xmle._nLine,
		xmle._ich,
        xmle._pszFound,
        xmle._pszExpected);
    SysFreeString(xmle._pszFound);
    SysFreeString(xmle._pszExpected);
    SysFreeString(xmle._pchBuf);
	SAFERELEASE(pXMLError);
	m_xmlUnknownFormat = TRUE;
	AfxMessageBox(buf);
}

BOOL XMLDoc::SaveToURL(CArchive& ar)
{
	IXMLDocument* pDoc;
    HRESULT hr;
	if(!ar.IsStoring()) return FALSE;
    if(m_pDoc != NULL) pDoc = m_pDoc;
	else return FALSE;
	IPersistStreamInit* pIPersistStreamInit;
	hr = pDoc->QueryInterface(IID_IPersistStreamInit, (void **)&pIPersistStreamInit);
	if(SUCCEEDED(hr)){
		CArchiveStream as(&ar);
		IStream* pIStream = (IStream*)&as;
		hr = pIPersistStreamInit->Save(pIStream, TRUE);
		SAFERELEASE(pIPersistStreamInit);
		SAFERELEASE(pIStream);
		if(SUCCEEDED(hr)) return TRUE;
	}
	return FALSE;	
}

BOOL XMLDoc::InitNewDocument()
{
	// dom-1 has no support to create and empty doc with a root node
	TRACE0(_T("\nTRACE: XMLDoc::InitNewDocument()"));
	m_xmlUnknownFormat = FALSE;
	BOOL result = FALSE;
	CString emptyXMLDoc;
	emptyXMLDoc.LoadString(IDS_EMPTY_XML_DOC);
	CMemFile mf;
	mf.SetFilePath(_T("c:\\temp\\~emptydoc.vxml")); // fake name
	mf.Write((const void*)(LPCTSTR)emptyXMLDoc, emptyXMLDoc.GetLength());
	mf.SeekToBegin();
	CArchive ar(&mf, CArchive::load);
	result = InitFromURL(ar);
	ar.Close();
	mf.Close();
	return result;
}

void XMLDoc::VisitNodes(XMLVisitor &visitor)
{
	if(m_pDoc == NULL) return;
	IXMLElement *pElem = NULL;
	m_pDoc->get_root(&pElem);
	if(pElem == NULL){
		m_xmlUnknownFormat = TRUE;
		AfxMessageBox(IDS_XML_FORMAT_ERR);
		return;
	}
	BSTR bstrString;
    pElem->get_tagName(&bstrString);
	_bstr_t bTemp(bstrString);
	SysFreeString(bstrString);
	if(lstrcmp((TCHAR*)bTemp, NoteTag::NOTES) != 0){
		m_xmlUnknownFormat = TRUE;
		AfxMessageBox(IDS_XML_FORMAT_ERR);
		return;
	}
	VisitNodes(pElem, visitor, 0);
	SAFERELEASE(pElem);
}

void XMLDoc::VisitNodes(IXMLElement *pElem, XMLVisitor &visitor, LPARAM p)
{
	HRESULT hr;
	IXMLElementCollection *pCollection = NULL;
	if(pElem == NULL) return;

	pElem->get_children(&pCollection);
	if(pCollection == NULL) return;
	long length;
	pCollection->get_length(&length);
	if(length <= 0) return;
	for(long i = 0; i < length; i++){
		IXMLElement *pElem = NULL; // local
		IDispatch *pDisp;
		_variant_t index(i, VT_I4);
		_variant_t empty;
		pCollection->item(index, empty, &pDisp);
		hr = pDisp->QueryInterface(IID_IXMLElement, (void **)&pElem);
		if(SUCCEEDED(hr)){
			// to keep the pElem pointer valid the visitor must AddRef it
			VisitNodes(pElem, visitor, visitor.VisitNode(pElem, p));
			SAFERELEASE(pElem);
		}
		SAFERELEASE(pDisp);
	}
	SAFERELEASE(pCollection);
}

// xml 'strange char' (unicode) support

CString XMLDoc::EncodeData(CString data)
{
	if(data.IsEmpty()) return data;
	CString nd;
	for(int i = 0; i < data.GetLength(); i++){
		nd += EncodeChar(data[i]);
	}
	return nd;
}

CString XMLDoc::DecodeData(CString data)
{
	if(data.IsEmpty()) return data;
	CString nd;
	int j = -1;
	int possible = -1;
	for(int i = 0; i < data.GetLength(); i++){
		TCHAR c = data[i];
		if(c == _T('#')){
			possible = i;
		} else if(c == _T(';') && (possible > -1)){
			TCHAR nc = DecodeChar(data.Mid(possible + 1, (i - possible - 1)));
			if((int)nc == 0) {
				// parse error, throw();
				return data;
			}
			nd.Insert(++j, nc);
			possible = -1;
		} else {
			if(possible < 0) // do not add in the middle
				nd.Insert(++j, c);
		}
	}	
	return nd;
}

CString XMLDoc::EncodeChar(TCHAR c)
{
	CString nc;
	int ic = (int)c; // can be negative
	if((ic < 32) || (ic > 127) ||
		(ic == (int)_T('<')) ||
		(ic == (int)_T('>')) ||
		(ic == (int)_T('#')) ||
		(ic == (int)_T(';'))
	){
		nc.Format(_T("#%d;"), ic);
	} else {
		nc.Insert(0, c);
	}
	return nc;
}

TCHAR XMLDoc::DecodeChar(CString c)
{
	return (TCHAR)_ttoi(c);
}

// editing support

CString XMLDoc::GetTimeString()
{
	CTime time = CTime::GetCurrentTime();
	return time.Format(_T("%Y-%m-%d-%H-%M-%S"));
}

IXMLElement* XMLDoc::AddNewElement(IXMLElement *pParentElem)
{
	if(!IsFormatSupported()) return NULL;
	if(m_pDoc == NULL) return NULL;
	HRESULT hr;
	IXMLElement* pParent = pParentElem;
	IXMLElement* pNewElem = NULL;
	_variant_t elementType((long)XMLELEMTYPE_ELEMENT, VT_I4);
	_variant_t vNote(NoteTag::NOTE);
	hr = m_pDoc->createElement(elementType, vNote, &pNewElem);
	if(SUCCEEDED(hr)){
		_bstr_t bTitle(NoteTag::TITLE);
		_variant_t vTitle(DecodeData(NoteTag::NEW_NOTE_NAME));
		pNewElem->setAttribute(bTitle, vTitle);
		_bstr_t bCreated(NoteTag::CREATED);
		_variant_t vCreated(GetTimeString());
		pNewElem->setAttribute(bCreated, vCreated);
		_bstr_t bModified(NoteTag::MODIFIED);
		pNewElem->setAttribute(bModified, vCreated);
		// create an empty body
		IXMLElement* pBody = NULL;
		_variant_t vBody(NoteTag::BODY);
		hr = m_pDoc->createElement(elementType, vBody, &pBody);
		if(SUCCEEDED(hr)){
			IXMLElement* pBodyText = NULL;
			_variant_t elementType((long)XMLELEMTYPE_TEXT, VT_I4);
			VARIANT vt;
			vt.vt = VT_EMPTY;
			hr = m_pDoc->createElement(elementType, vt, &pBodyText);
			if(SUCCEEDED(hr)){
				_bstr_t bText(_T(""));
				pBodyText->put_text(bText);
				pBody->addChild(pBodyText, -1, -1);
			}
			hr = pNewElem->addChild(pBody, -1, -1);
			if(SUCCEEDED(hr)){
				if(pParent == NULL){
					hr = m_pDoc->get_root(&pParent); // fails when empty
					if(!SUCCEEDED(hr)){
						// make the document; not defined in dom-1,but in dom-3
						// thus we fail here
						pNewElem->Release();
						return NULL;
					}
				}
				hr = pParent->addChild(pNewElem, -1, -1);
				if(SUCCEEDED(hr)){
					//caller owns pParent->Release();
					return pNewElem;
				}
			}
		}
	}
	return NULL; // failure
}

BOOL XMLDoc::DeleteElement(IXMLElement *pElem)
{
	HRESULT hr;
	if(pElem == NULL) return FALSE;
	IXMLElement *pParent = NULL;
	hr = pElem->get_parent(&pParent);
	if(SUCCEEDED(hr)){
		hr = pParent->removeChild(pElem);
		if(SUCCEEDED(hr)){
			// pElem is no more valid
			SAFERELEASE(pElem);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL XMLDoc::GetNoteData(IXMLElement *pElem, Note *n)
{	// pElem -> n
	if((pElem == NULL) || (n == NULL)) return FALSE;
 	BSTR bstrString;
 	pElem->get_tagName(&bstrString);
 	_bstr_t tagName(bstrString);
	SysFreeString(bstrString);
	if(lstrcmp((TCHAR*)tagName, NoteTag::NOTE) != 0) return FALSE;

	_bstr_t titleAttribute(NoteTag::TITLE);
	_variant_t title;
	pElem->getAttribute(titleAttribute, &title); //#
	_bstr_t bTitle(title);
	_bstr_t creationAttribute(NoteTag::CREATED);
	_variant_t creation;
	pElem->getAttribute(creationAttribute, &creation);
	_bstr_t bCreation(creation);
	_bstr_t modifiedAttribute(NoteTag::MODIFIED);
	_variant_t modified;
	pElem->getAttribute(modifiedAttribute, &modified);
	_bstr_t bModified(modified);
	_bstr_t commentAttribute(NoteTag::COMMENT);
	_variant_t comment;
	pElem->getAttribute(commentAttribute, &comment);
	_bstr_t bComment(comment);

	IXMLElement *pBody = XMLDoc::GetBodyTagForNote(pElem);
	if(pBody != NULL){
		BSTR bstrString;
		pBody->get_text(&bstrString);
		_bstr_t bBody(bstrString);
		SAFERELEASE(pBody);
		n->m_body = DecodeData((LPCTSTR)bBody);
	}

	n->m_title = DecodeData((LPCTSTR)bTitle);
	n->m_comment = DecodeData((LPCTSTR)bComment);
	n->m_created = ((LPCTSTR)bCreation);
	n->m_modified = ((LPCTSTR)bModified);

	return TRUE;
}

void XMLDoc::SetNoteData(IXMLElement *pElem, Note *n, BOOL updateBody)
{	// n -> pElem
	if((pElem == NULL) || (n == NULL)) return;
 	BSTR bstrString;
 	pElem->get_tagName(&bstrString);
 	_bstr_t tagName(bstrString);
	SysFreeString(bstrString);
	if(lstrcmp((TCHAR*)tagName, NoteTag::NOTE) != 0) return;

	n->m_title.TrimLeft();
	n->m_title.TrimRight();
	n->m_comment.TrimLeft();
	n->m_comment.TrimRight();

	_bstr_t bTitle(NoteTag::TITLE);
	_variant_t vTitle(XMLDoc::EncodeData(n->m_title));
	pElem->setAttribute(bTitle, vTitle);
	_bstr_t bComment(NoteTag::COMMENT);
	_variant_t vComment(XMLDoc::EncodeData(n->m_comment));
	pElem->setAttribute(bComment, vComment);
	_bstr_t bModified(NoteTag::MODIFIED);
	n->m_modified = XMLDoc::GetTimeString();
	_variant_t vModified(n->m_modified);
	pElem->setAttribute(bModified, vModified);

	if(updateBody){
		IXMLElement *pBody = XMLDoc::GetBodyTagForNote(pElem);
		if(pBody != NULL){
			SetBodyText(pBody, XMLDoc::EncodeData(n->m_body));
			SAFERELEASE(pBody);
		}
	}
}

IXMLElement* XMLDoc::GetBodyTagForNote(IXMLElement *pElem)
{
	if(pElem == NULL) return NULL;
	// get the body subtag, must be only one
	HRESULT hr;
	IXMLElement *pBody = NULL;
	IXMLElementCollection *pCollection = NULL;
	pElem->get_children(&pCollection);
	if(pCollection == NULL) return NULL;
	long length;
	pCollection->get_length(&length);
	if(length <= 0L) return NULL;
	for(long i = 0L; i < length; i++){
		IXMLElement *pElement = NULL;
		IDispatch *pDisp;
		_variant_t index(i, VT_I4);
		_variant_t empty;
		pCollection->item(index, empty, &pDisp);
		hr = pDisp->QueryInterface(IID_IXMLElement, (void **)&pElement);
		if(SUCCEEDED(hr)){
			BSTR bstrString;
 			pElement->get_tagName(&bstrString);
 			_bstr_t tagName(bstrString);
			SysFreeString(bstrString);
			if(lstrcmp((TCHAR*)tagName, NoteTag::BODY) == 0){
				// ok we found the body in pElement
				// caller must release it
				pBody = pElement;
				SAFERELEASE(pDisp);
				break;
			}
			SAFERELEASE(pElement);
		}
		SAFERELEASE(pDisp);
	}
	SAFERELEASE(pCollection);
	return pBody;
}

void XMLDoc::SetBodyText(IXMLElement *pBody, CString &text)
{
	if(pBody == NULL) return;
	HRESULT hr;
	IXMLElement *pBodyText = NULL;
	IXMLElementCollection *pCollection = NULL;
	_bstr_t bText(text);
	pBody->get_children(&pCollection);
	if(pCollection == NULL){
		// we must create a text element
		if(m_pDoc == NULL) return;
		IXMLElement* pBodyText = NULL;
		_variant_t elementType((long)XMLELEMTYPE_TEXT, VT_I4);
		VARIANT vt;
		vt.vt = VT_EMPTY;
		hr = m_pDoc->createElement(elementType, vt, &pBodyText);
		if(SUCCEEDED(hr)){
			pBodyText->put_text(bText);
			pBody->addChild(pBodyText, -1, -1);
			SAFERELEASE(pBodyText);
		}
	} else {
		// find the (first) text node and replace text
		long length;
		pCollection->get_length(&length);
		if(length <= 0L) return;
		for(long i = 0L; i < length; i++){
			IXMLElement *pElement = NULL;
			IDispatch *pDisp;
			_variant_t index(i, VT_I4);
			_variant_t empty;
			pCollection->item(index, empty, &pDisp);
			hr = pDisp->QueryInterface(IID_IXMLElement, (void **)&pElement);
			if(SUCCEEDED(hr)){
				long type;
				pElement->get_type(&type);
				if(type == XMLELEMTYPE_TEXT){
					pElement->put_text(bText);
					SAFERELEASE(pElement);
					SAFERELEASE(pDisp);
					break;
				}
				SAFERELEASE(pElement);
			}
			SAFERELEASE(pDisp);
		}
		SAFERELEASE(pCollection);
	}
}
