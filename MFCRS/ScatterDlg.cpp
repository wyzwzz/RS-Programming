// ScatterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCRS.h"
#include "ScatterDlg.h"
#include "afxdialogex.h"
#include"MyFunc.h"

// CScatterDlg �Ի���

IMPLEMENT_DYNAMIC(CScatterDlg, CDialogEx)

CScatterDlg::CScatterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScatterDlg::IDD, pParent)
{
	idc = NULL;
	checkbol = FALSE;
	irect = new CRect;
	nr0=10;
	isreadcolorfile = FALSE;
	SmooBol=FALSE;
}

CScatterDlg::~CScatterDlg()
{
}

void CScatterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_S_STATIC, S_Picture);
	DDX_Control(pDX, IDC_L_COMBO1, L_Combobox);
	DDX_Control(pDX, IDC_B_COMBO2, B_Combobox);
	DDX_Control(pDX, IDC_CHECK1, mcheck);
}


BEGIN_MESSAGE_MAP(CScatterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CScatterDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK1, &CScatterDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CScatterDlg::OnClickSmooBol)
END_MESSAGE_MAP()


// CScatterDlg ��Ϣ�������


//---------------------------------------------------------------------------------------
//-------------------------------------��άɢ��ͼ----------------------------------------
//---------------------------------------------------------------------------------------
void CScatterDlg::OnBnClickedButton1()//��ɢ��ͼ�ĺ���
{
	idc = S_Picture.GetDC();
	ipwnd = GetDlgItem(IDC_S_STATIC);
	ipwnd->GetClientRect(irect);
	ClearMyImgWindow(idc,irect);
	if(!mrsimg) return;
	UCHAR **Limg = NULL;
	UCHAR **Bimg = NULL;
	CString str;
	int LefMin,LefMax,BotMin,BotMax,nr0;
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(str); LefMin=atoi(str);
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(str); LefMax=atoi(str);
	GetDlgItem(IDC_EDIT3)->GetWindowTextA(str); BotMin=atoi(str);
	GetDlgItem(IDC_EDIT4)->GetWindowTextA(str); BotMax=atoi(str);
	GetDlgItem(IDC_EDIT5)->GetWindowTextA(str); nr0=atoi(str);
	int index = L_Combobox.GetCurSel(); if(index == 0) Limg = mrsimg->Rimg; else if(index == 1) Limg = mrsimg->Gimg; else if(index == 2) Limg = mrsimg->Bimg; else if(index == 3 && mrsimg->InMBol) Limg = mrsimg->Mimg;
	    index = B_Combobox.GetCurSel(); if(index == 0) Bimg = mrsimg->Rimg; else if(index == 1) Bimg = mrsimg->Gimg; else if(index == 2) Bimg = mrsimg->Bimg; else if(index == 3 && mrsimg->InMBol) Bimg = mrsimg->Mimg;
	if(checkbol) objrsimg.FillScatDiagramFunc(idc,irect->Width(),irect->Height(),mrsimg->Img_W0,mrsimg->Img_H0,mrsimg->Img_B0,Limg,Bimg,LefMin,LefMax,BotMin,BotMax,SmooBol,nr0);
	else objrsimg.PlotScatDiagramFunc(idc,irect->Width(),irect->Height(),mrsimg->Img_W0,mrsimg->Img_H0,mrsimg->Img_B0,Limg,Bimg,LefMin,LefMax,BotMin,BotMax);
	return;
}
//---------------------------------------------------------
void CScatterDlg::ClearMyImgWindow(CDC* dc,CRect* rect)//ÿ�λ�֮ǰ�����������Ϊ��ɫ
{
	dc->FillSolidRect(rect,RGB(255,255,255));
}
//---------------------------------------------------------
BOOL CScatterDlg::OnInitDialog()//�Ի���ĳ�ʼ�����������ú��������ѡ��
{
	CDialogEx::OnInitDialog();
	L_Combobox.InsertString(0,_T("R"));
	B_Combobox.InsertString(0,_T("R"));
	L_Combobox.InsertString(1,_T("G"));
	B_Combobox.InsertString(1,_T("G"));
	L_Combobox.InsertString(2,_T("B"));
	B_Combobox.InsertString(2,_T("B"));
	if(mrsimg->InMBol)
	{
		L_Combobox.InsertString(3,_T("M"));
		B_Combobox.InsertString(3,_T("M"));
	}
	L_Combobox.SetCurSel(0);
	B_Combobox.SetCurSel(1);
	GetDlgItem(IDC_EDIT1)->SetWindowTextA("0");
	GetDlgItem(IDC_EDIT2)->SetWindowTextA("255");
	GetDlgItem(IDC_EDIT3)->SetWindowTextA("0");
	GetDlgItem(IDC_EDIT4)->SetWindowTextA("255");
	GetDlgItem(IDC_EDIT5)->SetWindowTextA("10");
	return TRUE;
}
//---------------------------------------------------------
void CScatterDlg::OnBnClickedCheck1()//�����ɫ��ѡ����ɫ
{
	checkbol = !checkbol;
	mcheck.SetCheck(checkbol);
	if(!isreadcolorfile)
	{
		isreadcolorfile = ReadOneTextColorSpectrmFunc("D:\\Study\\ң�г������\\ң�г������ʵ�鲿��1\\ң�г������ʵ�鲿��\\�ʺ�2.lib");//Ĭ�϶�ȡ·��
		if(!isreadcolorfile)//�����ļ��ƶ��ˣ�û��ȡ�������Դ򿪶Ի����Լ�ѡ��λ�ú��ļ�
		{
			CFileDialog file( 1, _T("lib"), NULL, 0, _T("lib�ļ�(*.lib)|*.lib||"), this );
			if( IDOK == file.DoModal())
			{
				isreadcolorfile=ReadOneTextColorSpectrmFunc(file.GetPathName());
			}
		}
	}
	else
		return;
}
//---------------------------------------------------------
void CScatterDlg::OnClickSmooBol()//���ƽ���뾶�ĸ�ѡ��
{
	SmooBol=!SmooBol;
}
