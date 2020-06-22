#pragma once
#include "afxwin.h"


// CLookStock �Ի���

class CLookStock : public CDialogEx
{
	DECLARE_DYNAMIC(CLookStock)

public:
	CLookStock(_ConnectionPtr p, int id, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLookStock();
	int adminid;
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOOKSTOCK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_stocklist;
	virtual BOOL OnInitDialog();
};
