#include "stdafx.h"
#include "Bitmap.h"

// Grab the selected area
/// <summary>
/// Screenshots the selected area
/// </summary>
/// <param name="rectangle">Area to capture</param>
/// <returns>0 on success, nonzero on failure</returns>
BOOL Bitmap::GrabArea(const CRect &rectangle) {
    // Check if a previous bitmap exists, if so, remove it.
    if(!CImage::IsNull()) CImage::Destroy();

    // Create a screen & memory context
    HDC hdcScreen = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
    HDC hdcMem    = CreateCompatibleDC(hdcScreen);

    // Create a bitmap
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, rectangle.Width(), rectangle.Height());
    HBITMAP hBitmapOld = (HBITMAP) SelectObject(hdcMem, hBitmap);

    // Bit-blt from screen to memory device context
    DWORD dwRop = SRCCOPY | CAPTUREBLT;
    // Call BitBlt from the global namespace.
    BOOL bRet   = ::BitBlt(hdcMem, 0, 0, rectangle.Width(), rectangle.Height(), hdcScreen, rectangle.left, rectangle.top, dwRop);

    // Attach the bitmap handle.
    Attach(hBitmap);

    // Free
    SelectObject(hdcMem, hBitmapOld);
    DeleteDC(hdcMem);
    DeleteDC(hdcScreen);

    // Return the bit to block color transfer bool now.
    return bRet;
}

// Capture a window. Relies on FindWindow()????
/// <summary>
/// Captures a window.
/// </summary>
/// <param name="hWnd">Window handle</param>
/// <returns>0 on success, nonzero on failure</returns>
BOOL Bitmap::GrabWindow(HWND hWnd){
    if(!IsWindow(hWnd)) return false;
    CRect rectangle;

    GetWindowRect(hWnd, rectangle);
    return GrabArea(rectangle);
}