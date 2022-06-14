// LineTextDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCRS.h"
#include "LineTextDlg.h"
#include "afxdialogex.h"
#include"MyFunc.h"

// CLineTextDlg 对话框

IMPLEMENT_DYNAMIC(CLineTextDlg, CDialogEx)

CLineTextDlg::CLineTextDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLineTextDlg::IDD, pParent)
	, lpnum(0)
{
	
}

CLineTextDlg::~CLineTextDlg()
{
}

void CLineTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLineTextDlg, CDialogEx)
	
	ON_BN_CLICKED(IDOK, &CLineTextDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLineTextDlg 消息处理程序

BOOL CLineTextDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	lpointnum.Format("%d",TablPointNum);
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(lpointnum);
	return TRUE;  // return TRUE unless you set the focus to a control
}
void CLineTextDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(lpointnum);
	CDialogEx::OnOK();
}
