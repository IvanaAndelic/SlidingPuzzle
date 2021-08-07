
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
	int y_offset=0;
	


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
	void init_image(int bmp_width, int bmp_height);
	void init_test(int bmp_width, int bmp_height);
	void on_victory();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

