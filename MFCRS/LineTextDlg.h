#pragma once


// CLineTextDlg 对话框

class CLineTextDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLineTextDlg)

public:
	CLineTextDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLineTextDlg();
	CString lpointnum;
	int lpnum;

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
