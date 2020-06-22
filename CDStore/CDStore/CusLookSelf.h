#pragma once
#include "afxwin.h"


// CCusLookSelf 对话框

class CCusLookSelf : public CDialogEx
{
	DECLARE_DYNAMIC(CCusLookSelf)

public:
	CCusLookSelf(_ConnectionPtr p, CString cusid, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCusLookSelf();
	CString id;
	double balance;
	CString regtime;
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUSLOOKSELF };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_selflent;
	CListBox m_selfbuy;
	afx_msg void OnBnClickedOk();
};
