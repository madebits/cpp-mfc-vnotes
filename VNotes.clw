; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=BrowseTree
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "VNotes.h"
LastPage=0

ClassCount=8
Class1=CVNotesApp
Class2=CVNotesDoc
Class3=CVNotesView
Class4=CMainFrame

ResourceCount=5
Resource1=IDD_FORMVIEW
Class5=CAboutDlg
Class6=BrowseTree
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class7=CDetailsDialog
Resource4=IDR_TREEITEM_POP_MENU
Class8=CFindNoteDialog
Resource5=IDD_DIALOG_FIND

[CLS:CVNotesApp]
Type=0
HeaderFile=VNotes.h
ImplementationFile=VNotes.cpp
Filter=N
LastObject=ID_FILE_NEW
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CVNotesDoc]
Type=0
HeaderFile=VNotesDoc.h
ImplementationFile=VNotesDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=ID_REFRESH

[CLS:CVNotesView]
Type=0
HeaderFile=VNotesView.h
ImplementationFile=VNotesView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=CVNotesView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=VNotes.cpp
ImplementationFile=VNotes.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_EMAIL

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_EMAIL,button,1342242816

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_EXPORT_TEXT
Command6=ID_FILE_MRU_FILE1
Command7=ID_APP_EXIT
Command8=ID_EDIT_NEWTOPNOTE
Command9=ID_EDIT_NEWSUBNOTE
Command10=ID_FIND_NOTE
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=ID_WINDOW_SPLIT
Command14=ID_APP_ABOUT
CommandCount=14

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_NEWSUBNOTE
Command2=ID_EDIT_COPY
Command3=ID_EXPORT_TEXT
Command4=ID_FIND_NOTE
Command5=ID_FILE_NEW
Command6=ID_FILE_OPEN
Command7=ID_FILE_SAVE
Command8=ID_EDIT_NEWTOPNOTE
Command9=ID_EDIT_PASTE
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
Command14=ID_EDIT_COPY
Command15=ID_EDIT_PASTE
Command16=ID_EDIT_CUT
Command17=ID_EDIT_UNDO
CommandCount=17

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_TREEITEM_INSERT
Command5=ID_TREEITEM_DELETE
Command6=ID_FIND_NOTE
Command7=ID_APP_ABOUT
CommandCount=7

[CLS:BrowseTree]
Type=0
HeaderFile=BrowseTree.h
ImplementationFile=BrowseTree.cpp
BaseClass=CTreeView
Filter=C
VirtualFilter=VWC
LastObject=BrowseTree

[DLG:IDD_FORMVIEW]
Type=1
Class=CDetailsDialog
ControlCount=10
Control1=IDC_STATIC,static,1342308352
Control2=IDC_TITLE,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_COMMENT,edit,1350631552
Control5=IDC_BODY,edit,1344344068
Control6=IDC_UPDATE_TITLE,button,1342242817
Control7=IDC_STATIC,static,1342308352
Control8=IDC_CREATED_TIME,edit,1350568066
Control9=IDC_STATIC,static,1342308352
Control10=IDC_MODIFIED_TIME,edit,1350568066

[MNU:IDR_TREEITEM_POP_MENU]
Type=1
Class=?
Command1=ID_TREEITEM_INSERT
Command2=ID_TREEITEM_DELETE
Command3=ID_VIEW_NOTE_TXT
Command4=ID_PO2_NEWSUBNOTE
Command5=ID_PO2_NEWSUBNOTE
Command6=ID_EDIT_UNDO
Command7=ID_EDIT_CUT
Command8=ID_EDIT_COPY
Command9=ID_EDIT_PASTE
CommandCount=9

[CLS:CDetailsDialog]
Type=0
HeaderFile=DetailsDialog.h
ImplementationFile=DetailsDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_TITLE

[DLG:IDD_DIALOG_FIND]
Type=1
Class=CFindNoteDialog
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_FIND,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_FIND_NEXT,button,1476460544
Control6=IDC_CHECK_CASE,button,1342242819

[CLS:CFindNoteDialog]
Type=0
HeaderFile=FindNoteDialog.h
ImplementationFile=FindNoteDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_VIEW_NOTE_TXT
VirtualFilter=dWC

