#pragma once
#include "afxwin.h"


// CModPassword �Ի���

class CModPassword : public CDialogEx
{
	DECLARE_DYNAMIC(CModPassword)

public:
	CModPassword(_ConnectionPtr p, CString cusid, CWnd* pParent = NULL);   // ��׼���캯��
	CModPassword(_ConnectionPtr p, int adminid, CWnd* pParent = NULL);
	virtual ~CModPassword();
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;
	int m_adminid;
	CString m_cusid;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODPASSWORD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CEdit m_oldpassword;
	CEdit m_newpassword;
	CEdit m_cerpassword;
	CString m_oldpasswordstr;
	CString m_newpasswordstr;
	CString m_cerpasswordstr;
	virtual void OnOK();
};
