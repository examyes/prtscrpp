/*
implementation of the CprtscrppView class
*/
#include "stdafx.h"

#ifndef SHARED_HANDLERS
    #include "prtscrpp.h"
#endif

#include <string>
#include <omp.h>
#include "prtscrppDoc.h"
#include "prtscrppView.h"
#include "Uploader.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CprtscrppView, CScrollView)
BEGIN_MESSAGE_MAP(CprtscrppView, CScrollView)
    ON_WM_HOTKEY()
    ON_COMMAND(ID_FILE_SAVE, &CprtscrppView::OnFileSave)
    ON_COMMAND(ID_FILE_OPEN, &CprtscrppView::OnFileOpen)
    ON_COMMAND(ID_EDIT_PASTE, &CprtscrppView::OnEditPaste)
    ON_COMMAND(ID_EDIT_COPY, &CprtscrppView::OnEditCopy)
    ON_COMMAND(ID_EDIT_CUT, &CprtscrppView::OnEditCut)
    ON_COMMAND(ID_CAPTURE_TOIMGUR, &CprtscrppView::handleUpload)
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
    RegisterHotKey(m_hWnd, 100, MOD_CONTROL, 0x2C); // 0x34
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

void CprtscrppView::updateClipboard(std::string link) {
    // Try to opent he clipboard
    if(!OpenClipboard()) {
        AfxMessageBox(_T("Cannot open the Clipboard."));
        return;
    }

    // Remove the current Clipboard contents
    if(!EmptyClipboard()) {
        AfxMessageBox(_T("Failed to empty the Clipboard."));
        return;
    }

    // Get the currently selected data
    HGLOBAL hGlob = GlobalAlloc(GMEM_FIXED, 64); // 64 should do it.
    strcpy_s((char*)hGlob, 64, link.c_str());

    // For the appropriate data formats...
    if(::SetClipboardData(CF_TEXT, hGlob) == NULL) {
        CString msg;
        msg.Format(_T("Unable to set Clipboard data, error: %d"), GetLastError());
        AfxMessageBox(msg);
        CloseClipboard();
        GlobalFree(hGlob);
        return;
    }

    // Close here.
    CloseClipboard();
}

// Hotkey handler.
void CprtscrppView::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2) {
    // Always hide before sending a notification!
    AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
    this->pDoc->OnCaptureArea(); // Capture!
    this->handleUpload(); // And just upload.

    return CScrollView::OnHotKey(nHotKeyId, nKey1, nKey2);
}

void CprtscrppView::handleUpload() {
    // Get the bitmap
    Bitmap &Bitmap = this->pDoc->getBitmap();
    if(Bitmap.IsNull()) {
        AfxMessageBox(_T("There is no image loaded."));
        return;
    }

    // Try to save it. TODO: Add user options to choose their format here, for now we'll force png.
    if(Bitmap.Save(_T("C:\\prtscrpp_temp.png")) != S_OK) {
        this->pDoc->SendTrayNotification(CString("Failed to save the image to the disk."));
        return;
    }

    // Benchmarking purposes
    double timeStart = omp_get_wtime();

    Uploader uploader; // Uploader instance
    std::string link; // Our direct link \:D/

    // Start the upload process!
    link = uploader.imgur(CString("C:\\prtscrpp_temp.png"));

    // Benchmarking...
    double delta = omp_get_wtime() - timeStart;

    // Check if our link is empty...
    if(link.empty()) {
        this->pDoc->SendTrayNotification(CString("Failed to upload. There was a problem with the API."));
        return;
    }

    // Otherwise :D
    CString notification;
    notification.Format(_T("Uploaded successfully in %f second(s)"), delta);
    this->pDoc->SendTrayNotification(notification);

    // And finally, update our clipboard.
    this->updateClipboard(link);
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

void CprtscrppView::OnEditCopy() {
    if(!OpenClipboard()) return; // Try to access the clipboard

    Bitmap &Bitmap = this->pDoc->getBitmap();
    if(Bitmap.IsNull()) { // There is no bitmap available or something is messed up.
        AfxMessageBox(_T("There is no Bitmap data loaded into the application or it has been corrupt."));
    }

    // If we can't empty it then it's no use.
    if(!EmptyClipboard()) {
        AfxMessageBox(_T("Failed to empty the contents of the clipboard."));
        return;
    }

    // Just set the data now?
    //SetClipboardData(CF_BITMAP, Bitmap); // This actually works lol -- it doesn't.
    HBITMAP m_hbitmap = (HBITMAP)CopyImage(
        Bitmap,
        IMAGE_BITMAP,
        0,
        0,
        LR_COPYRETURNORG); // copy the handle again...
    SetClipboardData(CF_BITMAP, m_hbitmap);
    CloseClipboard();
}

// Copy+new
void CprtscrppView::OnEditCut() {
    this->OnEditCopy();
    this->pDoc->OnNewDocument();
}