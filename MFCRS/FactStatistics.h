#pragma once
#include "resource.h"
#include "afxwin.h"

// COpenBmp �Ի���
#define L_MOUSE_DOWN 0x00000001L
#define L_MOUSE_UP 0x00000002L
#define R_MOUSE_DOWN 0x00000004L
#define R_MOUSE_UP 0x00000008L
#define RGBM_ISSELECTED 0x00000010L
#define RGBM_LEFT 0x00000100L
#define RGBM_RIGHT 0x00000200L
class CFactStatistics : public CDialogEx
{
	DECLARE_DYNAMIC(CFactStatistics)

public:
	CFactStatistics(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFactStatistics();//��������
	CDC* rdc;//R���λ����dc
	CDC *gdc;
	CDC *bdc;
	CDC *mdc;
	CWnd *rpwnd;//�˱������ڽ���Ļ�������תΪ�ؼ����������
	CWnd *gpwnd;
	CWnd *bpwnd;
	CWnd *mpwnd;
	CRect *rrect;//��ȡ�ؼ��ľ���
	CRect *grect;
	CRect *brect;
	CRect *mrect;
	CPen *redpen;//��ɫ����
	CPen *greenpen;
	CPen *bluepen;
	CPen *graypen;
	CEdit R_Picture;//����Ŀؼ�����
	CEdit G_Picture;
	CEdit B_Picture;
	CEdit M_Picture;
	CPoint mpoint;//�������ʱ������
	int rbol,gbol,bbol,mbol;//ÿ���������ߵ�ѡ��״̬
	int flag;//�жϵ��ģʽ��ʱ����Сֵ�������ֵ
	BOOL modetwo;//�жϵ��ģʽ��
	BOOL isinited;//�ж��Ƿ���й���ʼ��
// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnStnClickedRStatic();
	afx_msg void OnStnClickedGStatic();
	afx_msg void OnStnClickedBStatic();
	afx_msg void OnStnClickedMStatic();
	afx_msg void OnBnClickedButtonChange();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3Strech();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnBnClickMode();
	afx_msg void OnChangeMaxOrMin();
	afx_msg void OnPaint();
	int ClickToLine(int x,int min,int max);
	int ClickToLineS(int x,int min,int max,int* bol);
	void ClearLineSelect(int* bol,CDC* dc);
	void PictureAction(CDC* dc,CRect* rec,int* bol,int x,int* min,int* max,int color);
	void PicActionModeTwo(CDC* dc,CRect* rec,int x,int *min,int* max,int color);
	void InitBKColor();
	void InitRGBMbol();
	void SetPenModeCopy();
	void InitFactLine();
	void ChangeFactLine();
	void LineBeSelected(int x,CRect *rect,CDC* dc,int color);
	void PlotFact(CDC* dc,long fact[],int fmin,int fmax,int color);
	void PlotLine(CDC* dc,int oldv,int newv,int color);
	void GetEditInfo();
	void ChangeEditInfo();
	void InitStrech();
	void PaintMyFact();
};
