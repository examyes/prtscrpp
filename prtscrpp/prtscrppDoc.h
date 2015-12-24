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
        Bitmap &getBitmap();
        afx_msg void OnCaptureArea();
        afx_msg void OnCaptureScreen();
        afx_msg void OnCaptureWindow();
        CprtscrppDoc &getDoc();

    private:
        Bitmap Bitmap;
        LRESULT OnHotKey(WPARAM a, LPARAM b);
};