
// ChildView.cpp : implementation of the CChildView class
//

#include "pch.h"
#include "framework.h"
#include "SlidingPuzzle.h"
#include "ChildView.h"
#include "resource.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

std::vector<HBITMAP> SplitBitmap(HBITMAP bmp, int columns, int rows)
{
	// get the bitmap dimensions
	BITMAP bm;
	GetObject(bmp, sizeof(BITMAP), &bm);
	int wd = bm.bmWidth, hgt = bm.bmHeight;
	int piece_wd = wd / columns;
	int piece_hgt = hgt / rows;

	// Select the given bitmap into a device context.
	auto hdcScreen = GetDC(NULL);
	auto hdcBitmap = CreateCompatibleDC(hdcScreen);
	auto hbmOldBmp = SelectObject(hdcBitmap, bmp);

	std::vector<HBITMAP> pieces(columns * rows);
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < columns; col++) {
			// create a device context for a piece and select an appropriately sized bitmap into it
			auto hdcPiece = CreateCompatibleDC(hdcScreen);
			auto hbmPiece = CreateCompatibleBitmap(hdcScreen, piece_wd, piece_hgt);
			auto hbmOldPiece = SelectObject(hdcPiece, hbmPiece);

			// paint a piece of the whole bitmap into the piece bitmap
			BitBlt(hdcPiece, 0, 0, piece_wd, piece_hgt, hdcBitmap, col * piece_wd, row * piece_hgt, SRCCOPY);

			// cleanup per piece resources we dont need.
			SelectObject(hdcPiece, hbmOldPiece);
			DeleteDC(hdcPiece);

			pieces[row * columns + col] = hbmPiece;
		}
	}

	SelectObject(hdcBitmap, hbmOldBmp);
	DeleteDC(hdcBitmap);
	ReleaseDC(NULL, hdcScreen);

	return pieces;

}

void CChildView::OnPaint() 
{
	auto hbm = (HBITMAP)LoadImage(NULL, L"res\\heart.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	auto pieces = SplitBitmap(hbm, 4, 4);
	
}










void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnLButtonDown(nFlags, point);
}
