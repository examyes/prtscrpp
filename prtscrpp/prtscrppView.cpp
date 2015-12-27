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
    ON_COMMAND(ID_FILE_SAVE, &CprtscrppView::OnFileSave)
    ON_COMMAND(ID_FILE_OPEN, &CprtscrppView::OnFileOpen)
    ON_COMMAND(ID_EDIT_PASTE, &CprtscrppView::OnEditPaste)
END_MESSAGE_MAP()

CprtscrppView::CprtscrppView() {}
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
    /*CprtscrppDoc *pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if(!pDoc) return;*/

    Bitmap &Bitmap = this->pDoc->getBitmap();
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
    this->pDoc = GetDocument();
    ASSERT_VALID(this->pDoc);
    if(!this->pDoc) return;

    // Get the bitmap object
    Bitmap &Bitmap = pDoc->getBitmap();

    if(!Bitmap.IsNull()) { // If there is an image loaded then resize the app to fit the image.
        CSize size(Bitmap.GetWidth(), Bitmap.GetHeight());
        SetScrollSizes(MM_TEXT, size);
        ResizeParentToFit();
    }else { // Otherwise just draw the standard text.
        this->drawText(GetDC());
    }
}

// Hotkey handler.
void CprtscrppView::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2) {
    // TODO: Add your message handler code here and/or call default
    TRACE("HOKTEY ACTIVATEDDDDDDDDDDD");
    return CScrollView::OnHotKey(nHotKeyId, nKey1, nKey2);
}

// Called when saving a file
void CprtscrppView::OnFileSave() {
    // Try to fetch the bitmap first
    Bitmap &Bitmap = this->pDoc->getBitmap();
    if(Bitmap.IsNull()) { // The bitmap is NULL, nothing has been saved, just return.
        AfxMessageBox(_T("There is either no bitmap loaded or it has been corrupted."));
        return;
    }

    // Filter for files types that can be saved. We can support GIF,PNG,JPG,BMP (uncompressed)
    TCHAR szFilters[] = _T("24-bit Uncompressed Bitmap (*.bmp)|*.bmp|Portable Network Graphics (*.png)|*.png|Joint Photographic Experts Group (*.jpg)|*.jpg|Graphics Interchange Format (*.gif)|*.gif||");

    // Create a save dialog; the default file name extension is .png
    CFileDialog fileDlg(FALSE, _T("png"), _T("*.png"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

    // Display the file dialog. When user clicks OK, fileDlg.DoModal() returns IDOK
    if(fileDlg.DoModal() == IDOK) {
        CString pathName = fileDlg.GetPathName(); // Grab the name
        CString pathExt  = fileDlg.GetFileExt(); // And the extension

        // Get the file's title.
        CString fileName = fileDlg.GetFileTitle();

        // Just save, CImage automatically figures out the Gdiplus constant based on the extension ;)
        if(Bitmap.Save(pathName) == S_OK) {
            // Set the window title accordingly.
            AfxGetMainWnd()->SetWindowText(fileName + "." + pathExt + CString(" - prtscrpp"));
        }else {
            AfxMessageBox(_T("There has been a problem saving your file."));
        }
    }
}

// Called when opening a file
void CprtscrppView::OnFileOpen() {
    // Try to fetch the bitmap first
    Bitmap &Bitmap = this->pDoc->getBitmap();
    if(!Bitmap.IsNull()) { // There is already a bitmap!
        AfxMessageBox(_T("There is a bitmap currently loaded, consider saving?"));
    }

    // Filter for files types that can be saved. We can support GIF,PNG,JPG,BMP (uncompressed)
    TCHAR szFilters[] = _T("Data Files (*.png;*.jpg;*.bmp;*.gif)|*.png; *.jpg; *.bmp; *.gif||");

    // Create an Open dialog; the default file name extension is .png
    CFileDialog fileDlg(TRUE, _T(""), _T(""), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

    // Display the file dialog. When user clicks OK, fileDlg.DoModal() returns IDOK
    if(fileDlg.DoModal() == IDOK) {
        CString pathName = fileDlg.GetPathName(); // Grab the name
        CString pathExt  = fileDlg.GetFileExt(); // And the extension

        // Get the file title
        CString fileName = fileDlg.GetFileTitle();

        // First detach and destroy the old bitmap if it exists.
        if(!Bitmap.IsNull()) {
            Bitmap.Detach();
            Bitmap.Destroy();
        }

        // Hack, scroll to (0,0), this will -hopefully- cause it all to redraw properly.
        POINT corner;
        corner.x = 0;
        corner.y = 0;
        ScrollToPosition(corner);

        // Now load the new one.
        if(Bitmap.Load(pathName) == S_OK) { // We good
            // Change the window's title to the opened file's title.
            AfxGetMainWnd()->SetWindowText(fileName + "." + pathExt + CString(" - prtscrpp"));
        }else {
            AfxMessageBox(_T("There has been a problem loading the file."));
        }
    }

    // Update.
    this->OnDraw(GetDC());
    AfxGetMainWnd()->ShowWindow(SW_SHOW);
    this->pDoc->UpdateAllViews(NULL);
}

void CprtscrppView::OnEditPaste() {
    if(!OpenClipboard()) return;

    // Fetch the bitmap
    Bitmap &Bitmap = this->pDoc->getBitmap();

    // If an existing bitmap exists, destroy it
    if(!Bitmap.IsNull()) {
        Bitmap.Detach();
        Bitmap.Destroy();
    }

    //Get the clipboard data
    HBITMAP handle = (HBITMAP)GetClipboardData(CF_BITMAP);
    if(!handle) {
        CloseClipboard();
        return;
    }
    
    // If an existing bitmap exists, detach from it.
    if(!Bitmap.IsNull()) Bitmap.Detach();
    
    // Try to copy the bitmap into an HBITMAP object, FML direct handling doesn't work 
    HBITMAP m_hbitmap = (HBITMAP)CopyImage(
        (HBITMAP)GetClipboardData(CF_BITMAP),
        IMAGE_BITMAP,
        0,
        0,
        LR_COPYRETURNORG); // copy the handle to make atlimage.h shut up
    CloseClipboard(); // Finally close the clipboard

    // Hack the rescroll so it redraws properly.
    POINT corner;
    corner.x = 0;
    corner.y = 0;
    ScrollToPosition(corner);

    Bitmap.Attach(m_hbitmap); // And attach our new image
    
    // Update all the windows here
    this->OnDraw(GetDC());
    AfxGetMainWnd()->ShowWindow(SW_SHOW);
    this->pDoc->UpdateAllViews(NULL);
}