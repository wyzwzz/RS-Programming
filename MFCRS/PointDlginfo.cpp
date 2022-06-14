// PointDlginfo.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCRS.h"
#include "PointDlginfo.h"
#include "afxdialogex.h"


// PointDlginfo 对话框

IMPLEMENT_DYNAMIC(PointDlginfo, CDialogEx)

PointDlginfo::PointDlginfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(PointDlginfo::IDD, pParent)
{
	inflag = FALSE;
	instr = "";
}

PointDlginfo::~PointDlginfo()
{
}

void PointDlginfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(PointDlginfo, CDialogEx)
	ON_BN_CLICKED(IDOK, &PointDlginfo::OnBnClickedOk)

END_MESSAGE_MAP()


// PointDlginfo 消息处理程序

void PointDlginfo::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT1)->GetWindowText(instr);
	if(instr != "") inflag = TRUE;
	CDialogEx::OnOK();
}

BOOL PointDlginfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString str;
	str.Format("%d",point_x);
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(str);
	str.Format("%d",point_y);
	GetDlgItem(IDC_EDIT3)->SetWindowTextA(str);
	return TRUE;  // return TRUE unless you set the focus to a control

	// 异常: OCX 属性页应返回 FALSE
}
