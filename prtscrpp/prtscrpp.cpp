/*/
Defines the class behaviors for the application.
*/

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "prtscrpp.h"
#include "MainFrm.h"

#include "prtscrppDoc.h"
#include "prtscrppView.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CprtscrppApp, CWinApp)
    ON_COMMAND(ID_APP_ABOUT, &CprtscrppApp::OnAppAbout)

    // Standard file based document commands
    ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()

// Constructor
CprtscrppApp::CprtscrppApp() {
    
    // Add support for Restart Manager
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;

    // Set some app id.
    SetAppID(_T("prtscrpp.1"));
}

// The one and only CprtscrppApp object
CprtscrppApp theApp;

BOOL CprtscrppApp::InitInstance() {
    // InitCommonControlsEx() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC  = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

    // Initialize OLE libraries
    if(!AfxOleInit()) {
        AfxMessageBox(IDP_OLE_INIT_FAILED);
        return FALSE;
    }

    AfxEnableControlContainer();
    EnableTaskbarInteraction(FALSE);

    // Register document templates.
    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(CprtscrppDoc),
        RUNTIME_CLASS(CMainFrame),
        RUNTIME_CLASS(CprtscrppView));
    if(!pDocTemplate) return FALSE;
    AddDocTemplate(pDocTemplate);

    // Parse command line for standard shell commands, DDE, file open
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // Dispatch commands specified on the command line.  Will return FALSE if
    // app was launched with /RegServer, /Register, /Unregserver or /Unregister.
    if(!ProcessShellCommand(cmdInfo)) return FALSE;

    // The one and only window has been initialized, so show and update it
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();
    return TRUE;
}

int CprtscrppApp::ExitInstance() {
    //TODO: handle additional resources you may have added
    AfxOleTerm(FALSE);

    return CWinApp::ExitInstance();
}

/*
About dialog
*/
class CAboutDlg: public CDialogEx {
    public:
        CAboutDlg();

    // Dialog Data
    #ifdef AFX_DESIGN_TIME
        enum { IDD = IDD_ABOUTBOX };
    #endif

    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg(): CDialogEx(IDD_ABOUTBOX) {}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CprtscrppApp::OnAppAbout() {
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}