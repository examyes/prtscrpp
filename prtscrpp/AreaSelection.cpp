/*
AreaSelection.cpp : implementation file
*/
#include "stdafx.h"
#include "prtscrpp.h"
#include "AreaSelection.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(AreaSelection, CDialog)

AreaSelection::AreaSelection(CRect &rect): CDialog(ID_CAPTURE_AREA), rectangle(rect) {
    // Set the rectangle to be empty at first.
    rect.SetRectEmpty();
}

AreaSelection::~AreaSelection() {}

void AreaSelection::DoDataExchange(CDataExchange* pDX){CDialog::DoDataExchange(pDX);}

BEGIN_MESSAGE_MAP(AreaSelection, CDialog)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
    ON_WM_SETCURSOR()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


/* MESSAGE HANDLERS */
BOOL AreaSelection::OnInitDialog() {
    CDialog::OnInitDialog(); // Construct parent
    BOOL grabArea;

    // Construct a rectangle of full screen width & height. Top, left = 0, right = resolution width, height = resolution height.
    CRect wholeScreen = CRect(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

    AfxGetMainWnd()->ShowWindow(SW_HIDE); // Hide prtscrpp
    Sleep(100); // Let it hide.

    // Grab an area of the whole screen. Control this via the rubberBand (alla QT/QRubberBand).
    grabArea = Bitmap.GrabArea(wholeScreen);

    // Capturing of area was not successful, abort.
    if(!grabArea) EndDialog(IDABORT);

    // Topmost window gets buggy so this fixes it ?
    const int cx = GetSystemMetrics(SM_CXSCREEN);
    const int cy = GetSystemMetrics(SM_CYSCREEN);
    SetWindowPos(&CWnd::wndTopMost, 0, 0, cx, cy, SWP_SHOWWINDOW);

    return 0;
}

/*BOOL AreaSelection::OnEraseBkgnd(CDC* pDC) {
    return true;
}*/

void AreaSelection::OnPaint() {
    CPaintDC dc(this);
    Bitmap.Draw(dc.GetSafeHdc(), 0, 0);
    rectangleTracker.Draw(&dc);
}

void AreaSelection::OnLButtonDown(UINT nFlags, CPoint point)  {
    // Track the area which we would like to screenshot
    rectangleTracker.TrackRubberBand(this, point);
    rectangleTracker.GetTrueRect(rectangle);
    EndDialog(IDOK); // All good.
}

void AreaSelection::OnRButtonDown(UINT nFlags, CPoint point) {
    EndDialog(IDCANCEL);
}

BOOL AreaSelection::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
    // give the tracker for the selection a chance.
    // TODO: Is this really needed lol???
    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
    return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void AreaSelection::OnMouseMove(UINT nFlags, CPoint point) {
    // Change the cursor to a cross so we know we're going to take a screenie
    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
    CDialog::OnMouseMove(nFlags, point);
}
