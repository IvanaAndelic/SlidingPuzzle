
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
	int piece_wd;
	int piece_hg;
	int nrows;
	int ncol;
	bool painted = false;


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
	void shuffle(std::vector<int>& positions);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

