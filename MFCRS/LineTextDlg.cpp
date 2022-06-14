// LineTextDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCRS.h"
#include "LineTextDlg.h"
#include "afxdialogex.h"
#include"MyFunc.h"

// CLineTextDlg �Ի���

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


// CLineTextDlg ��Ϣ�������

BOOL CLineTextDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	lpointnum.Format("%d",TablPointNum);
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(lpointnum);
	return TRUE;  // return TRUE unless you set the focus to a control
}
void CLineTextDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(lpointnum);
	CDialogEx::OnOK();
}
