#pragma once
#include "afxwin.h"


// CNewSup �Ի���

class CNewSup : public CDialogEx
{
	DECLARE_DYNAMIC(CNewSup)

public:
	CNewSup(_ConnectionPtr p, int id, CWnd* pParent = NULL);   // ��׼���캯��
	CNewSup(_ConnectionPtr p, int id, CString supid, CWnd* pParent = NULL);
	virtual ~CNewSup();
	int m_adminid;
	int mode;
	CString m_supid;
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWSUP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CEdit m_supname;
	CEdit m_supaddr;
	CEdit m_supphone;
	afx_msg void OnBnClickedOk();
};
