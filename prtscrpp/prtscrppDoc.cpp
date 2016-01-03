/*
prtscrppDoc.cpp : implementation of the CprtscrppDoc class
*/
#include "stdafx.h"

#ifndef SHARED_HANDLERS
    #include "prtscrpp.h"
#endif

#include <string>
#include "prtscrppDoc.h"
#include "prtscrppView.h"
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

/// <summary>
/// Gets called whenever "new" has been pressed on the toolbar/from the menu
/// </summary>
/// <returns></returns>
BOOL CprtscrppDoc::OnNewDocument() {
    if(!CDocument::OnNewDocument()) return FALSE;

    if(!Bitmap.IsNull()) {
        //Bitmap.Detach();
        Bitmap.Destroy();
    }

    UpdateAllViews(NULL);
    //return TRUE;
}

/// <summary>
/// Handles serialization.
/// </summary>
/// <param name="ar"></param>
void CprtscrppDoc::Serialize(CArchive& ar) {
    if(ar.IsStoring()) {
        // TODO: add storing code here
    }else {
        // TODO: add loading code here
    }
}

// Getter for the main (View) class.
Bitmap &CprtscrppDoc::getBitmap() { return Bitmap; }

/// <summary>
/// Captures an area of the screen.
/// </summary>
void CprtscrppDoc::OnCaptureArea() {
    // Spawn a rectangle
    CRect rectangle;
    INT_PTR nRet = -1; // This'll grab the response from the dialog

    // Construct the area DIALOG object
    AreaSelection areaObject(rectangle); // This will set the rectangle to be empty
    nRet = areaObject.DoModal();

    if(nRet == IDOK) { // If left button was hit, it means all went well and we captured something
        Bitmap.GrabArea(rectangle); // Bitmap (CImage) now contains image data.
    }else { // Otherwise, right button was pressed, which means it was an accident.
        if(!Bitmap.IsNull()) { // Perhaps the right button was pressed on the first screenshot capture, when there was not a Bitmap at all.
            //Bitmap.Detach();
            Bitmap.Destroy();
        }
    }

    // Show after taking the pic
    AfxGetMainWnd()->ShowWindow(SW_SHOW);
    UpdateAllViews(NULL);
}

/// <summary>
/// Captures the whole screen.
/// </summary>
void CprtscrppDoc::OnCaptureScreen() {
    CWnd *mainWnd = AfxGetMainWnd(); // Get prtscrpp win and hide it
    BOOL grabArea;

    mainWnd->ShowWindow(SW_HIDE);
    Sleep(200); // So it can hide itself fully.

    // Construct a rectangle of full screen width & height. Top, left = 0, right = resolution width, height = resolution height.
    CRect wholeScreen = CRect(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    
    // Since we are not in a dialog now, we can just call the GrabArea function without any rubberband interaction.
    grabArea = Bitmap.GrabArea(wholeScreen);

    mainWnd->ShowWindow(SW_SHOW); // Show again the main window.
    UpdateAllViews(NULL);
}

/// <summary>
/// Captures the first foreground window.
/// </summary>
void CprtscrppDoc::OnCaptureWindow() {
    CWnd *mainWnd = AfxGetMainWnd(); // Retrieve main window
    mainWnd->ShowWindow(SW_HIDE); // Hide it
    Sleep(200); // Sleep some, to hide.

    HWND hWnd = GetForegroundWindow(); // Now grab any foreground window
    Bitmap.GrabWindow(hWnd);

    mainWnd->ShowWindow(SW_SHOW); // Show our app!
    UpdateAllViews(NULL);
}

/// <summary>
/// Sends a notification to the tray
/// </summary>
/// <param name="body">The message to be sent</param>
void CprtscrppDoc::SendTrayNotification(CString body) {
    // Get the main window
    CWnd *mainWnd = AfxGetMainWnd();
    bool wasHidden = false;

    // Find out whether it was minimized (iconic/hidden) or not.
    if(mainWnd->IsIconic()) wasHidden = true;

    // Show the main window -- This is needed because Windows for some reason doesn't process background notifications in some sort of a queue.
    AfxGetMainWnd()->ShowWindow(SW_SHOW);
    ShowWindow(GetActiveWindow(), SW_SHOW);
    SetFocus(GetActiveWindow());

    NOTIFYICONDATA data; // Construct the new Shell32 struct with notification data in it
    data.hWnd = GetActiveWindow(); // Retrieve the active Window (now, the main window)
    data.cbSize = sizeof(NOTIFYICONDATA);
    //data.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_INFO));
    data.uID = IDR_MAINFRAME;
    data.uTimeout = 5000; // in miliseconds
    data.uFlags = NIF_INFO;
    data.dwInfoFlags = NIIF_INFO | NIF_MESSAGE;
    _tcscpy_s(data.szInfoTitle, _T("prtscrpp"));
    _tcscpy_s(data.szInfo, body);
    Shell_NotifyIcon(NIM_MODIFY, &data);

    // Small hack, 
    // Hide the main window now if it was originally hidden, user can't notice this anyway since it happens instantly.
    if(wasHidden) AfxGetMainWnd()->ShowWindow(SW_HIDE);
}