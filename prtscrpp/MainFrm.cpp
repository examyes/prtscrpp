/*
MainFrm.cpp : implementation of the CMainFrame class
*/

#include "stdafx.h"
#include "prtscrpp.h"
#include "MainFrm.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
#endif
#define MYWM_NOTIFYICON (WM_USER+2)

// CMainFrame
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_CLOSE()
    ON_WM_SYSCOMMAND()
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
    // TODO: Add settings loader here?
}

CMainFrame::~CMainFrame() {
    // TODO: Destroy settings loader here?
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

    // Initialize our tray icon here.
    TrayMessage(NIM_ADD);

    return 0;
}

void CMainFrame::OnClose() {
    // Destroy the tray icon here :)
    TrayMessage(NIM_DELETE);
    CFrameWnd::OnClose();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) {
    // Size the window accordingly and center it.
    cs.cy = ::GetSystemMetrics(SM_CYSCREEN) / 2;
    cs.cx = ::GetSystemMetrics(SM_CXSCREEN) / 2;
    cs.y = ((cs.cy * 2) - cs.cy) / 2;
    cs.x = ((cs.cx * 2) - cs.cx) / 2;

    if(!CFrameWnd::PreCreateWindow(cs)) return FALSE;

    return TRUE;
}

BOOL CMainFrame::TrayMessage(DWORD dwMessage) {
    CString sTip(_T("prtscrpp")); // Tooltip

    NOTIFYICONDATA tnd; // Shell32 struct which contains all the needed info to display the trayicon.
    tnd.cbSize = sizeof(NOTIFYICONDATA);
    tnd.hWnd = m_hWnd;
    tnd.uID = IDR_MAINFRAME; // The bitmap of the trayicon.
    tnd.uFlags = NIF_MESSAGE | NIF_ICON;
    tnd.uCallbackMessage = MYWM_NOTIFYICON;
    tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    VERIFY(tnd.hIcon = LoadIcon(AfxGetInstanceHandle(),
        MAKEINTRESOURCE(IDR_MAINFRAME)));
    lstrcpyn(tnd.szTip, (LPCTSTR)sTip, sizeof(tnd.szTip));

    return Shell_NotifyIcon(dwMessage, &tnd);
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

// Catch any events that happen in the MainFrame
LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
    // We are only interested in the NOTIFYICON event.
    if(message != MYWM_NOTIFYICON) return CFrameWnd::WindowProc(message, wParam, lParam);

    // Try to figure out what kind of a mouse click was performed
    switch(lParam) {
        case WM_LBUTTONDBLCLK: // Double left click
            if(wParam == IDR_MAINFRAME) {
                ShowWindow(SW_NORMAL);
                SetForegroundWindow();
                SetFocus();
                return TRUE;
            }
        break;
        case WM_RBUTTONDBLCLK: // Double right click
            this->OnClose();
            return TRUE;
        break;
    }

    return CFrameWnd::WindowProc(message, wParam, lParam);
}

// Hook any system events (Minimize, maximize, move, etc...)
void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam) {
    CFrameWnd::OnSysCommand(nID, lParam);

    if(nID == SC_MINIMIZE) {
        // If we've caught minimization, then try to grab the App's mainWnd and hide it.
        AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
    }
}