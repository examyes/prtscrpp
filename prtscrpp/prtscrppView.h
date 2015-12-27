/*
prtscrppView.h : interface of the CprtscrppView class
*/
#pragma once

class CprtscrppView: public CScrollView {
    protected:
	    DECLARE_DYNCREATE(CprtscrppView)
        virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
        virtual void OnInitialUpdate(); // called first time after construct
        virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
        DECLARE_MESSAGE_MAP()

    public:
	    CprtscrppDoc* GetDocument() const;
        CprtscrppView();
        virtual ~CprtscrppView();
        virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
        void OnDraw(CDC * pDC); // overridden to draw this view
        void drawText(CDC * pDC); // draws text when there is no bitmap

        afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
        afx_msg void OnFileSave();

    private:
        // Store the Doc ptr here for future use!
        CprtscrppDoc *pDoc;
public:
    afx_msg void OnFileOpen();
    afx_msg void OnEditPaste();
};