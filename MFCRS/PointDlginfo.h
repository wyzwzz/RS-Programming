#pragma once


// PointDlginfo �Ի���

class PointDlginfo : public CDialogEx
{
	DECLARE_DYNAMIC(PointDlginfo)

public:
	PointDlginfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PointDlginfo();
	CString instr;
	BOOL inflag;
	int point_x;
	int point_y;

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdit3();
	virtual BOOL OnInitDialog();
};
