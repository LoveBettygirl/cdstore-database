#pragma once
#include "afxwin.h"


// CCusLookSelf �Ի���

class CCusLookSelf : public CDialogEx
{
	DECLARE_DYNAMIC(CCusLookSelf)

public:
	CCusLookSelf(_ConnectionPtr p, CString cusid, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCusLookSelf();
	CString id;
	double balance;
	CString regtime;
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUSLOOKSELF };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_selflent;
	CListBox m_selfbuy;
	afx_msg void OnBnClickedOk();
};
