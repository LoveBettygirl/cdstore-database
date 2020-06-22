#pragma once
#include "afxwin.h"


// CDeleteCus �Ի���

class CDeleteCus : public CDialogEx
{
	DECLARE_DYNAMIC(CDeleteCus)

public:
	CDeleteCus(_ConnectionPtr p, CString cusid, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDeleteCus();
	CString id;
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CANCEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_calpwd;
	virtual void OnOK();
};
