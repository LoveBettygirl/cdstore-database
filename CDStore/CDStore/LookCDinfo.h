#pragma once
#include "afxwin.h"


// CLookCDinfo �Ի���

class CLookCDinfo : public CDialogEx
{
	DECLARE_DYNAMIC(CLookCDinfo)

public:
	CLookCDinfo(_ConnectionPtr p, int id, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLookCDinfo();
	int adminid;
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOOKCDINFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_cdinfolist;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedModifyinfo();
};
