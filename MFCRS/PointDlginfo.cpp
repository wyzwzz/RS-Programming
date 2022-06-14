// PointDlginfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCRS.h"
#include "PointDlginfo.h"
#include "afxdialogex.h"


// PointDlginfo �Ի���

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


// PointDlginfo ��Ϣ�������

void PointDlginfo::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	// �쳣: OCX ����ҳӦ���� FALSE
}
