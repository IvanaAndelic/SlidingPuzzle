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
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), nullptr);

	return TRUE;
}

//funkcija divide vraca najveci broj plocica na koje se bitmapa moze podijeliti s obzirom na dimenziju (visina ili sirina)
int CChildView::divide(int dimension, int max) {

	if (max > dimension / 2) {
		max = dimension / 2;

	}

	std::vector<int> divisors;
	for (int i = 1; i <= max; ++i) {
		if (dimension % i == 0)
			divisors.push_back(i);
	}

	return divisors[divisors.size() - 1];

}

void CChildView::init_test(int bmp_width, int bmp_height) {

	nrows = divide(bmp_height, 8);
	ncols = divide(bmp_width, 8);

	piece_height = bmp_height / nrows;
	piece_width = bmp_width / ncols;

	number_of_tiles = nrows * ncols;

	x_offset = x_default;
	y_offset = y_default;

	positions.resize(number_of_tiles);
	std::iota(positions.begin(), positions.end(), 0);
	empty = positions.size() - 3;//prazna ploèica je zadnja
	//std::shuffle(positions.end()-3, positions.end()-1, std::mt19937{ std::random_device{}() });
	std::swap(positions[empty + 1], positions[empty]);


	//empty = std::distance(positions.begin(), std::find(positions.begin(), positions.end(), number_of_tiles - 1));

	painted = true;

}


void CChildView::init_image(int bmp_width, int bmp_height, int x0, int y0, bool shuffle1 = true)
{

	nrows = divide(bmp_height, 8);
	ncols = divide(bmp_width, 8);

	piece_height = bmp_height / nrows;
	piece_width = bmp_width / ncols;

	x_offset = x_default + x0;
	y_offset = y_default + y0;

	number_of_tiles = nrows * ncols;

	positions.resize(number_of_tiles);
	std::iota(positions.begin(), positions.end(), 0);
	empty = positions.size() - 1;//prazna ploèica je zadnja
	if (shuffle1)
	{
		std::shuffle(positions.begin(), positions.end(), std::mt19937{ std::random_device{}() });

		empty = std::distance(positions.begin(), std::find(positions.begin(), positions.end(), number_of_tiles - 1));
		painted = true;
	}



}

void CChildView::draw_on_the_begining(CPaintDC& dc, CDC& memdc, BITMAP& bmp, int poc_x)
{
	int bitmap_height = bmp.bmHeight;
	int bitmap_width = bmp.bmWidth;
	// CRTANJE
	init_image(bitmap_width, bitmap_height, poc_x, min_y, false);
	for (int i = 0; i < number_of_tiles; ++i) {

		int row_src = positions[i] / ncols;
		int col_src = positions[i] % ncols;
		int row_dest = i / ncols;
		int col_dest = i % ncols;

		int x_src = col_src * piece_width;
		int y_src = row_src * piece_height;
		int x_dest = col_dest * piece_width;
		int y_dest = row_dest * piece_height;

		dc.BitBlt(x_dest + x_offset, y_dest + y_offset, piece_width, piece_height, &memdc, x_src, y_src, SRCCOPY);

	}
}

void CChildView::initial_choice(CPaintDC& dc, CDC& memdc, CBitmap& b1, CBitmap& b2, CBitmap& b3)
{
	// nacrtati 1. bitmapu, skaliranu, tako da
	// pocne od koordinata (x_first,min_y)
	auto prev = memdc.SelectObject(&b1);
	BITMAP bmp1; b1.GetBitmap(&bmp1);
	draw_on_the_begining(dc, memdc, bmp1, x_first);

	// nacrtati 2. bitmapu, skaliranu, tako da
	// pocne od koordinata (x_second,min_y)
	prev = memdc.SelectObject(&b2);
	BITMAP bmp2; b2.GetBitmap(&bmp2);
	draw_on_the_begining(dc, memdc, bmp2, x_second);

	// nacrtati 3. bitmapu, skaliranu, tako da pocne od koordinata (x_third,y_min)
	prev = memdc.SelectObject(&b3);
	BITMAP bmp3; b1.GetBitmap(&bmp3);
	// CRTANJE
	draw_on_the_begining(dc, memdc, bmp3, x_third);
	memdc.SelectObject(prev);
}


void CChildView::paint_bitmap(CPaintDC& dc, CDC& memdc, CBitmap& b, bool paint_first,
	int x_bmp_offset, int y_bmp_offset)
{
	auto prev = memdc.SelectObject(&b);
	BITMAP bmp; b.GetBitmap(&bmp);
	int bitmap_height = bmp.bmHeight;
	int bitmap_width = bmp.bmWidth;

	if (paint_first) {
		init_image(bitmap_width, bitmap_height, x_bmp_offset, y_bmp_offset);
	}

	for (int i = 0; i < number_of_tiles; ++i) {

		int row_src = positions[i] / ncols;
		int col_src = positions[i] % ncols;
		int row_dest = i / ncols;
		int col_dest = i % ncols;

		int x_src = col_src * piece_width;
		int y_src = row_src * piece_height;
		int x_dest = col_dest * piece_width;
		int y_dest = row_dest * piece_height;




		if (i == empty && !victory) {
			dc.Rectangle(x_dest + x_offset, y_dest + y_offset, x_dest + piece_width + x_offset, y_dest + piece_height + y_offset);
		}
		else {
			dc.BitBlt(x_dest + x_offset, y_dest + y_offset, piece_width, piece_height, &memdc, x_src, y_src, SRCCOPY);
		}


		// dc.BitBlt(x_dest, y_dest, piece_width, piece_height, &memdc, x_src, y_src, SRCCOPY);
		// dc.SelectObject(prev);
	}


	memdc.SelectObject(prev);

}




void CChildView::OnPaint()
{
	CPaintDC dc(this);
	CBitmap b; b.LoadBitmap(IDB_BITMAP1);
	CBitmap b2, b3;
	b2.LoadBitmap(IDB_BITMAP2);
	b3.LoadBitmap(IDB_BITMAP3);

	CDC memdc; memdc.CreateCompatibleDC(&dc);

	// odabir bitmape



	//choice = 0;
	switch (choice)
	{
	case 0:
		initial_choice(dc, memdc, b, b2, b3);
		break;
	case 1:
		paint_bitmap(dc, memdc, b, !painted, 0, 0);
		break;
	case 2:
		paint_bitmap(dc, memdc, b2, !painted, 0, 0);
		break;
	case 3:
		paint_bitmap(dc, memdc, b3, !painted, 0, 0);
	}
}



void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call def



	CWnd::OnLButtonDown(nFlags, point);

	//koordinate gdje je korisnik kliknuo
	// ukoliko je choice = 0, azurirati choice u ovisnosti o koordinatama (tocnije u ovisnosti o 
	// x koordinati)
	// postoje tri mogucnosti, ako je korisnik kliknuo na dobar y:
	if (choice == 0)
	{
		int x1 = x_first + x_default;
		int x2 = x_second + x_default;
		int x3 = x_third + x_default;
		if (point.y >= min_y + y_default && point.y <= max_y + y_default)
		{
			// 1. kliknuo je na 1. slagalicu
			if (point.x >= x1 && point.x <= x1 + x_width)
				choice = 1;
			// 2. kliknuo je na drugu slagalicu
			if (point.x >= x2 && point.x <= x2 + x_width)
				choice = 2;
			// 3. kliknuo je na trecu slagalicu
			if (point.x >= x3 && point.x <= x3 + x_width)
				choice = 3;
		}
		Invalidate();
		CWnd::UpdateWindow();
		return;
	}




	int row = (point.y - y_offset) / piece_height;
	int col = (point.x - x_offset) / piece_width;

	// ako 

	int empty_row = empty / ncols;
	int empty_col = empty % ncols;

	bool slide = false;



	if (abs(row - empty_row) == 0) {
		if (abs(col - empty_col) == 1) {
			slide = true;
		}
	}
	else if (abs(row - empty_row) == 1) {
		if (abs(col - empty_col) == 0) {
			slide = true;
		}
	}







	if (!victory && slide) {

		int old_index = row * ncols + col;
		std::swap(positions[old_index], positions[empty]);
		empty = old_index;

	

		
		Invalidate();


		CWnd::UpdateWindow();


	}


	








}





