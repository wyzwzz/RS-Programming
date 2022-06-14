#pragma once


// PointDlginfo 对话框

class PointDlginfo : public CDialogEx
{
	DECLARE_DYNAMIC(PointDlginfo)

public:
	PointDlginfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PointDlginfo();
	CString instr;
	BOOL inflag;
	int point_x;
	int point_y;

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdit3();
	virtual BOOL OnInitDialog();
};
