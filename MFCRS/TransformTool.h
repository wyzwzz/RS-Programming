#pragma once


// CTransformTool 对话框

class CTransformTool : public CDialogEx
{
	DECLARE_DYNAMIC(CTransformTool)

public:
	CTransformTool(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTransformTool();
	CString filename,filename1,filename2;
// 对话框数据
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBtnPicture_1();
	afx_msg void OnBtnPicture_2();
	afx_msg void OnTwoPictureAdd();
	afx_msg void OnUpDownPicture();
	afx_msg void OnBtnBmpToImgFunc();
	afx_msg void OnBtnImgToBmpFunc();
	afx_msg void OnBtnRunLength();
	afx_msg void OnBnScreenPrintf();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnOnePicToTwoFunc();
	afx_msg void OnBnSaveBmp_Func();
	afx_msg void OnBnSaveImg_Func();
	afx_msg void OnBnSaveAsImgOtherPlace();
	afx_msg void OnBnSaveAsBmpOtherPlace();
	afx_msg void OnSaveRSDataAsJpg();
	afx_msg void OnSaveAsJpgOtherPlace();
};
