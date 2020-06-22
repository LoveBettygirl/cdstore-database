#pragma once
#include "afxwin.h"
#include "DotEdit.h"


// CRegisterDlg �Ի���

class CRegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegisterDlg)

public:
	CRegisterDlg(_ConnectionPtr p,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegisterDlg();
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REGISTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CDotEdit m_balance;
	virtual void OnOK();
	CEdit m_password1;
	CEdit m_password2;
	virtual BOOL OnInitDialog();
	double m_balancedata;
	CString m_passwordstr;
};
