


// ChildView.h : interface of the CChildView class
//

#include<vector>


#pragma once


// CChildView window

class CChildView : public CWnd
{
	// Construction
public:
	CChildView();

	// Attributes
public:
	std::vector<int> positions;
	int empty;
	int piece_width;
	int piece_height;
	int nrows;
	int ncols;
	bool painted = false;
	int number_of_tiles;
	int number_of_moves = 0;
	bool victory = false;
	int x_offset = 0;
	int y_offset = 0;
	int choice = 0;
	// koordinate za pocetni izbor izmedju tri slagalice (sve su slagalice izmedju min_y i max_y)
	int min_y = 75;
	int max_y;
	int x_prvi = 0;
	int x_drugi = 300;
	int x_treci = 600;
	int x_sirina;

	// Operations
public:

	// Overrides
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	int divide(int dimension, int max);
	bool test_victory();
	void init_image(int bmp_width, int bmp_height, int x0, int y0, bool shuffle1);
	void init_test(int bmp_width, int bmp_height);
	void on_victory();
	void paint_bitmap(CPaintDC& dc, CDC& memdc, CBitmap& b, bool paint_first,
		int x_bmp_offset, int y_bmp_offset);
	void initial_choice(CPaintDC& dc, CDC& memdc, CBitmap& b1, CBitmap& b2, CBitmap& b3);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};