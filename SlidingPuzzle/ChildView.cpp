
// ChildView.cpp : implementation of the CChildView class
//

#include "pch.h"
#include "framework.h"
#include "SlidingPuzzle.h"
#include "ChildView.h"
#include "resource.h"
#include <vector>
#include <cstdlib>
#include <numeric>
#include <random>
#include <iostream>


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

//funkcija divide vraca najveci broj plocica na koje se bitmapa moze podijeliti s obzirom na dimenziju (visina ili sirina)
int CChildView::divide(int dimension, int max) {

	if (max > dimension/2) {
		max = dimension / 2;

	}

	std::vector<int> divisors;
	for (int i = 1; i <= max; ++i) {
		if (dimension % i == 0)
			divisors.push_back(i);
	}

	return divisors[divisors.size() - 1];

}







void CChildView::OnPaint() 
{
	CPaintDC dc(this);
	CBitmap b; b.LoadBitmap(IDB_BITMAP1);
	CDC memdc; memdc.CreateCompatibleDC(&dc);
	auto prev = memdc.SelectObject(&b);
	BITMAP bmp; b.GetBitmap(&bmp);

	int bitmap_height = bmp.bmHeight;
	int bitmap_width = bmp.bmWidth;

	 nrows = divide(bitmap_height, 8);
	 ncols = divide(bitmap_width, 8);

	 piece_height = bitmap_height / nrows;
	 piece_width = bitmap_width / ncols;

	int number_of_tiles = nrows * ncols;

	positions.resize(number_of_tiles);
	std::iota(positions.begin(), positions.end(), 0);
	empty = positions.size() - 1;//prazna ploèica je zadnja
	std::shuffle(positions.begin(), positions.end(), std::mt19937{ std::random_device{}() });

	

	for (int i = 0; i < positions.size()-1; ++i) {

		int row_dest = positions[i] / ncols;
		int col_dest = positions[i] % ncols;
		int row_src = i / ncols;
		int col_src = i % ncols;

		int x_src = col_src * piece_width;
		int y_src = row_src * piece_height;
		int x_dest = col_dest * piece_width;
		int y_dest = row_dest * piece_height;



         dc.BitBlt(x_dest, y_dest, piece_width, piece_height, &memdc, x_src, y_src, SRCCOPY);
	     dc.SelectObject(prev);
	}
	
	
	
	
}



void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call def



	CWnd::OnLButtonDown(nFlags, point);

	//koordinate gdje je korisnik kliknuo

	int row = point.y / piece_height;
	int col = point.x / piece_width;

	int empty_row = empty / ncols;
	int empty_col = empty % ncols;

	bool slide = false;

	switch (abs(row - empty_row)) {
	case 1:
		if (abs(col==empty_col))
			slide = true;
		break;
	case 0:
		if (abs(col-empty_col==1))
			slide = true;
		break;
	}

	if (slide) {
		int old_index = row * ncols + col;
		positions[old_index] = positions[empty];
		empty = old_index;
	}



	
	
}

