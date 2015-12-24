#pragma once

#include "atlimage.h" // Need this because of CImage

class Bitmap: public CImage {
    public:
        BOOL GrabArea(const CRect &);
        BOOL GrabWindow(HWND hWnd);
};