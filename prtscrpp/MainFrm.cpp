/*
MainFrm.cpp : implementation of the CMainFrame class
*/

#include "stdafx.h"
#include "prtscrpp.h"
#include "MainFrm.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
#endif

// CMainFrame
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_CREATE()
END_MESSAGE_MAP()

// Status line indicators
static UINT indicators[] =
{
    ID_SEPARATOR,
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};


CMainFrame::CMainFrame() {
    // TODO: Add keyboard hook and settings loader here.
}

CMainFrame::~CMainFrame() {
    // TODO: Remove RegisterHotKey and destruct everything?
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if(CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Try to create the toolbar
    if(!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP |  CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) || !m_wndToolBar.LoadToolBar(IDR_MAINFRAME)) {
        TRACE0("Failed to create toolbar\n");
        return -1;
    }

    // Try to create the status bar
    if(!m_wndStatusBar.Create(this)) {
        TRACE0("Failed to create status bar\n");
        return -1;
    }

    // Set statusbar indicators
    m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

    // TODO: Delete these three lines if you don't want the toolbar to be dockable
    /*m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndToolBar);*/

    return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) {
    // Size the window and center it -- msdn
    cs.cy = ::GetSystemMetrics(SM_CYSCREEN) / 2;
    cs.cx = ::GetSystemMetrics(SM_CXSCREEN) / 2;
    cs.y = ((cs.cy * 2) - cs.cy) / 2;
    cs.x = ((cs.cx * 2) - cs.cx) / 2;

    if(!CFrameWnd::PreCreateWindow(cs)) return FALSE;

    return TRUE;
}

// CMainFrame diagnostics
#ifdef _DEBUG
void CMainFrame::AssertValid() const {
    CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const {
    CFrameWnd::Dump(dc);
}
#endif //_DEBUG