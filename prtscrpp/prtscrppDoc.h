/*
interface of the CprtscrppDoc class
*/
#pragma once

#include "Bitmap.h"
class CprtscrppDoc: public CDocument {
    protected:
        // create from serialization only
        CprtscrppDoc();
        DECLARE_DYNCREATE(CprtscrppDoc)
        DECLARE_MESSAGE_MAP()

    public:
        virtual BOOL OnNewDocument();
        virtual void Serialize(CArchive& ar);
        virtual ~CprtscrppDoc();
        Bitmap &getBitmap(); // Returns Bitmap reference
        afx_msg void OnCaptureArea();
        afx_msg void OnCaptureScreen();
        afx_msg void OnCaptureWindow();

        void SendTrayNotification(CString); // Sends a tray notification.

    private:
        Bitmap Bitmap;
};