#pragma once

#include "Bitmap.h"
class AreaSelection : public CDialog {
	DECLARE_DYNAMIC(AreaSelection)

    public:
	    AreaSelection(CRect &);   // standard constructor
	    virtual ~AreaSelection();
        // Dialog Data
        #ifdef AFX_DESIGN_TIME
	        enum { IDD = ID_CAPTURE_AREA };
        #endif
        virtual BOOL OnInitDialog();
        //afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg void OnPaint();
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
        afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

    private:
        CRect &rectangle; // The area
        CRectTracker rectangleTracker; // Area tracker
        Bitmap Bitmap; // Image

    protected:
	    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
};
