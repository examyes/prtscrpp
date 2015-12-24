/*
main header file for the prtscrpp application
*/

#pragma once
#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h" // main symbols

class CprtscrppApp: public CWinApp {
    public:
	    CprtscrppApp();
        virtual BOOL InitInstance();
        virtual int ExitInstance();

        afx_msg void OnAppAbout();
        DECLARE_MESSAGE_MAP()
};

extern CprtscrppApp theApp;