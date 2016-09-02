
// playerDlg.h : ͷ�ļ�
//

#pragma once
#include "Enconn.h"
#include "Decoderav.h"
#define MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio
// CplayerDlg �Ի���
class CplayerDlg : public CDialogEx
{
// ����
public:
	CplayerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��	
// ʵ��
protected:
	HICON m_hIcon;
	Enconn *conn;
	Decoderav* m_Dec;
	
	//���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_msg;
	afx_msg void OnBnClickedButton4();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CRichEditCtrl m_RichEdit;
	int m_audiochan;
	afx_msg void OnBnClickedRadio1();
//	afx_msg void OnBnDropDownRadio2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadio6();
//	afx_msg void OnBnDropDownRadio7(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnBnDropDownRadio12(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnBnDropDownRadio13(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnBnDropDownRadio14(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnBnDropDownRadio17(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnBnDropDownRadio6(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnBnDropDownRadio1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio13();
	afx_msg void OnBnClickedRadio14();
	afx_msg void OnBnClickedRadio17();
	afx_msg void OnBnClickedRadio12();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
