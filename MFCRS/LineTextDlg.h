#pragma once


// CLineTextDlg �Ի���

class CLineTextDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLineTextDlg)

public:
	CLineTextDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLineTextDlg();
	CString lpointnum;
	int lpnum;

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
