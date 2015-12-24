/*
prtscrppDoc.cpp : implementation of the CprtscrppDoc class
*/
#include "stdafx.h"
#ifndef SHARED_HANDLERS
    #include "prtscrpp.h"
#endif

#include "prtscrppDoc.h"
#include "AreaSelection.h"
#include <propkey.h>

#ifdef _DEBUG
    #define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CprtscrppDoc, CDocument)
BEGIN_MESSAGE_MAP(CprtscrppDoc, CDocument)
    ON_COMMAND(ID_CAPTURE_AREA, &CprtscrppDoc::OnCaptureArea)
    ON_COMMAND(ID_CAPTURE_SCREEN, &CprtscrppDoc::OnCaptureScreen)
    ON_COMMAND(ID_CAPTURE_WINDOW, &CprtscrppDoc::OnCaptureWindow)
END_MESSAGE_MAP()

CprtscrppDoc::CprtscrppDoc() {
    // TODO: add one-time construction code here
}

CprtscrppDoc::~CprtscrppDoc() {}

BOOL CprtscrppDoc::OnNewDocument() {
    if(!CDocument::OnNewDocument()) return FALSE;

    if(!Bitmap.IsNull()) {
        Bitmap.Detach();
        Bitmap.Destroy();
    }

    UpdateAllViews(NULL);
    //return TRUE;
}

// Save/Load bitmaps here
void CprtscrppDoc::Serialize(CArchive& ar) {
    if(ar.IsStoring()) {
        // TODO: add storing code here
    }else {
        // TODO: add loading code here
    }
}

// Getter for the main (View) class. Returns a reference. 2 hours of life down the drain all because of an &.
Bitmap &CprtscrppDoc::getBitmap() { return Bitmap; }

void CprtscrppDoc::OnCaptureArea() {
    // Spawn a rectangle
    CRect rectangle;
    
    // Construct the area DIALOG object
    AreaSelection areaObject(rectangle); // This will set the rectangle to be empty
    areaObject.DoModal();

    Bitmap.GrabArea(rectangle); // Bitmap (CImage) now contains image data.

    AfxGetMainWnd()->ShowWindow(SW_SHOW);
    UpdateAllViews(NULL);
}

void CprtscrppDoc::OnCaptureScreen() {
    CWnd *mainWnd = AfxGetMainWnd(); // Get prtscrpp win and hide it
    BOOL grabArea;

    mainWnd->ShowWindow(SW_HIDE);
    Sleep(100); // So it can hide itself fully.

    // Construct a rectangle of full screen width & height. Top, left = 0, right = resolution width, height = resolution height.
    CRect wholeScreen = CRect(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    
    // Since we are not in a dialog now, we can just call the GrabArea function without any rubberband interaction.
    grabArea = Bitmap.GrabArea(wholeScreen);

    mainWnd->ShowWindow(SW_SHOW); // Show again the main window.
    UpdateAllViews(NULL);
}

void CprtscrppDoc::OnCaptureWindow() {
    // TODO: Add your command handler code here
}