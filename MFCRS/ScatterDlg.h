#pragma once
#include "afxwin.h"


// CScatterDlg 对话框

class CScatterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScatterDlg)

public:
	CScatterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScatterDlg();
	CDC* idc;
	CWnd* ipwnd;
	CRect* irect;
	int nr0;//平滑半径
	BOOL checkbol;
	BOOL isreadcolorfile;
	BOOL SmooBol;
// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit S_Picture;
	afx_msg void OnBnClickedButton1();
	


	void ClearMyImgWindow(CDC* dc,CRect* rect);
	CComboBox L_Combobox;
	virtual BOOL OnInitDialog();
//	afx_msg void OnCbnSelchangeLCombo1();
//	afx_msg void OnCbnDropdownLCombo1();
	afx_msg void OnCbnSelchangeLCombo1();
	CComboBox B_Combobox;
	afx_msg void OnBnClickedCheck1();
	CButton mcheck;
	afx_msg void OnClickSmooBol();
};
