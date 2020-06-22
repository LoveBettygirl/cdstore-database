#pragma once
#include "afxwin.h"


// CLookCus �Ի���

class CLookCus : public CDialogEx
{
	DECLARE_DYNAMIC(CLookCus)

public:
	CLookCus(_ConnectionPtr p, int id, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLookCus();
	int adminid;
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOOKCUS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSearchcus();
	CEdit m_cusid;
	CListBox m_cuslent;
	CListBox m_cusbuy;
	CStatic m_cusbal;
	CStatic m_custime;
	virtual void OnOK();
};
