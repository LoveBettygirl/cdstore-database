
// CDStoreDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "CustomerDlg.h"


// CCDStoreDlg �Ի���
class CCDStoreDlg : public CDialogEx
{
// ����
public:
	CCDStoreDlg(_ConnectionPtr p,CWnd* pParent = NULL);	// ��׼���캯��
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDSTORE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnFinalRelease();
	afx_msg void OnClose();
	afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedRegister();
	CEdit m_id;
	CEdit m_password;
	CString m_idstr;
	CString m_passwordstr;
	virtual void OnOK();
	afx_msg void OnBnClickedAbout();
};
