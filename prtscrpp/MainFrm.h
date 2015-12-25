/*
MainFrm.h : interface of the CMainFrame class
*/

#pragma once
class CMainFrame : public CFrameWnd {
    protected: // create from serialization only
        CMainFrame();
        DECLARE_DYNCREATE(CMainFrame)
        CToolBar   m_wndToolBar;
        CStatusBar m_wndStatusBar;
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        DECLARE_MESSAGE_MAP()

    public:
        virtual BOOL PreCreateWindow(CREATESTRUCT& cs); // Base Window creation
        BOOL TrayMessage(DWORD dwMessage); // Handle the tray icon
        virtual ~CMainFrame();
        #ifdef _DEBUG
            virtual void AssertValid() const;
            virtual void Dump(CDumpContext& dc) const;
        #endif
        afx_msg void OnClose(); // Handle the closing of the application
        virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam); // Hook any window events
        void OnSysCommand(UINT nID, LPARAM lParam); // Hook any system events
};