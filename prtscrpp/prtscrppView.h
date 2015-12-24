/*
prtscrppView.h : interface of the CprtscrppView class
*/
#pragma once

class CprtscrppView: public CScrollView {
    protected:
        //afx_msg void OnHotKey(UINT a, UINT b, UINT c);
        // create from serialization only
	    CprtscrppView();
	    DECLARE_DYNCREATE(CprtscrppView)
        virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
        virtual void OnInitialUpdate(); // called first time after construct
        virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
        DECLARE_MESSAGE_MAP()

    public:
	    CprtscrppDoc* GetDocument() const;
        void OnDraw(CDC * pDC); // overridden to draw this view
        void drawText(CDC * pDC); // draws text when there is no bitmap
        virtual BOOL PreCreateWindow(CREATESTRUCT& cs);    
        virtual ~CprtscrppView();
        afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
};