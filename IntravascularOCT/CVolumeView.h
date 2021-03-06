#pragma once


// CVolumeView 대화 상자

class CVolumeView : public CDialogEx
{
	DECLARE_DYNAMIC(CVolumeView)

public:
	CVolumeView(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CVolumeView();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VOLUME_VIEW };
#endif

protected:
	int				nViewType_;
	CScrollBar		ctrlScrollBar_;


public:
	int get_view_type() const { return nViewType_; }
	void set_view_type(int type) { nViewType_ = type; }

	void update_scroll_bar();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
};
