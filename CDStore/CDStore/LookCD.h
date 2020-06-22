#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CLookCD 对话框

class CLookCD : public CDialogEx
{
	DECLARE_DYNAMIC(CLookCD)

public:
	CLookCD(_ConnectionPtr p, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLookCD();
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOOKCD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSearch();
	CEdit m_cdname;
	CEdit m_artistname;
	CComboBox m_genre;
	virtual BOOL OnInitDialog();
	CListBox m_resultlist;
	virtual void OnOK();
	CListCtrl m_resultable;
};
