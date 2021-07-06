
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

int CChildView::divide(int dimension, int max) {

	std::vector<int> divisors;
	if (max > dimension / 2)
		max = dimension / 2;

	for (int i = 2; i <= max; ++i) {
		if (dimension % i == 0) {
			divisors.push_back(i);
		}
	}
	int n = divisors.size();
	if (n == 0) return 1;

	int index = (int)(((rand() * 1.0) / (RAND_MAX + 1)) * n);
	return divisors[index];

}

void CChildView::shuffle(std::vector<int>& positions) {

	int n = positions.size();

	for (int i = 0; i != n; ++i) {
		int x = (int)(((rand() * 1.0) / (RAND_MAX + 1)) * (n-i));

		int tmp = positions[x];
		positions[x] = positions[n - 1];
		positions[n - 1] = tmp;
	}
}




void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting


	CBitmap b; b.LoadBitmap(IDB_BITMAP1);
	CDC memdc; memdc.CreateCompatibleDC(&dc);
	auto prev = memdc.SelectObject(&b);
	BITMAP bmp; b.GetBitmap(&bmp);

	int height = bmp.bmHeight;
	int width = bmp.bmWidth;

	int nrows = divide(height, 8);
	int ncol = divide(width, 8);

	int piece_wd = bmp.bmWidth / ncol;
	int piece_hg = bmp.bmHeight / nrows;

	int total_size = nrows * ncol;

	std::vector<int> positions(total_size);

	std::iota(positions.begin(), positions.end(), 0);

	shuffle(positions);

	for(int i=0;i!=positions.size();++i){
		
		int row_dest = positions[i] / ncol;
		int col_dest = positions[i] % ncol;
		int row_src = i / ncol;
		int col_src = i % ncol;

		int x_src = col_src * piece_wd;
		int y_src = row_src * piece_hg;
		int x_dest = col_dest * piece_wd;
		int y_dest = row_dest * piece_hg;
		 

          dc.BitBlt(x_dest, y_dest, piece_wd, piece_hg, &memdc, x_src, y_src, SRCCOPY);
		  dc.SelectObject(prev);
		 
		
	}

	
	
	
}












void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnLButtonDown(nFlags, point);
}
