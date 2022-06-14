#pragma once
#include "afxwin.h"
#include"MyFunc.h"
// NoiseDlg 对话框

class CPicTool : public CDialogEx
{
	DECLARE_DYNAMIC(CPicTool)

public:
	CPicTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPicTool();
	BOOL HaarInvTF;
// 对话框数据
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSaltPepper();
	afx_msg void OnBnClickedGussianNoise();
	CButton HaarInverseTransform;
	afx_msg void OnBnClickedButtonHaarTF();
	afx_msg void OnBnClickedCheckHaarInv();
	void OneBandHarrWaveFunc(RSImg* mrsimg,UCHAR** Rimg,int HarrLevel,BOOL InvBol);
	void Smoothing_ConvolutionFilterFunc(BOOL smooBol);
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnImgSmooBtnClick();
	afx_msg void OnConvolutionBtnClick();
	afx_msg void OnGausValueRadClick();
	afx_msg void OnSharpeningRadClick();
	afx_msg void OnLaplacianRadClick();
	afx_msg void OnHoriPrewittBtnClick();
	afx_msg void OnVertPrewittBtnClick();
	afx_msg void OnHoriSobelBtnClick();
	afx_msg void OnVertSobelBtnClick();
	afx_msg void OnX_MirorBtnFun();
	afx_msg void OnY_MirorBtnFunc();
	afx_msg void OnBmp_Rotate90DFunc();
	afx_msg void OnBmpRotate270DFunc();
	afx_msg void OnBnMediumConv();
	afx_msg void OnBnKTtransform();
	afx_msg void OnBn0_MirrorFunc();
};
