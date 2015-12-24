/*
implementation of the CprtscrppView class
*/
#include "stdafx.h"

#ifndef SHARED_HANDLERS
    #include "prtscrpp.h"
#endif

#include "prtscrppDoc.h"
#include "prtscrppView.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CprtscrppView, CScrollView)
BEGIN_MESSAGE_MAP(CprtscrppView, CScrollView)
    ON_WM_HOTKEY()
END_MESSAGE_MAP()

CprtscrppView::CprtscrppView() {
    // TODO: add construction code here
}
CprtscrppView::~CprtscrppView() {}

void CprtscrppView::DoDataExchange(CDataExchange* pDX) {
    CScrollView::DoDataExchange(pDX);
}

BOOL CprtscrppView::PreCreateWindow(CREATESTRUCT& cs) {
    return CScrollView::PreCreateWindow(cs);
}

// https://msdn.microsoft.com/en-us/library/3ew6s3ez.aspx
void CprtscrppView::OnInitialUpdate() {
    RegisterHotKey(m_hWnd, 100, MOD_CONTROL, 0x34);
    CSize size(100, 100);
    SetScrollSizes(MM_TEXT, size);
}

CprtscrppDoc* CprtscrppView::GetDocument() const {
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CprtscrppDoc)));
    return (CprtscrppDoc*)m_pDocument;
}

// View area onDraw. Display the image if the bitmap exists.
void CprtscrppView::OnDraw(CDC* pDC) {
    CprtscrppDoc *pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if(!pDoc) return;

    Bitmap &Bitmap = pDoc->getBitmap();
    if(Bitmap.IsNull()) {
        // There has been nothing captured or "new" button was pushed.
        this->drawText(pDC);
    }else {
        CBrush brush(::GetSysColor(COLOR_APPWORKSPACE));
        Bitmap.Draw(pDC->GetSafeHdc(), 0, 0);
        FillOutsideRect(pDC, &brush);
    }
}

void CprtscrppView::drawText(CDC *pDC) {
    CRect rect;
    CBrush brush(GetSysColor(COLOR_APPWORKSPACE)); // Load the brush color
    CString s("There is no image loaded. Start by capturing a screenshot.");

    GetClientRect(&rect); // Load the rectangle into rect
    pDC->SetTextAlign(TA_BASELINE | TA_CENTER); // Align to center
    pDC->TextOut(rect.right / 2, rect.bottom / 2, s, s.GetLength()); // And echo
}

// Whenever an event occurs in the main area (image gets updated, window gets resized)
void CprtscrppView::OnUpdate(CView*, LPARAM , CObject*) {
    CprtscrppDoc *pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if(!pDoc) return;

    Bitmap &Bitmap = pDoc->getBitmap();

    if(!Bitmap.IsNull()) { // If there is an image loaded then resize the app to fit the image.
        CSize size(Bitmap.GetWidth(), Bitmap.GetHeight());
        SetScrollSizes(MM_TEXT, size);
        ResizeParentToFit();
    }else { // Otherwise just draw the standard text.
        this->drawText(GetDC());
    }
}

void CprtscrppView::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
    // TODO: Add your message handler code here and/or call default
    TRACE("HOKTEY ACTIVATEDDDDDDDDDDD");
    //CScrollView::OnHotKey(nHotKeyId, nKey1, nKey2);
}
