#pragma once
#include "afxwin.h"
#include "DotEdit.h"


// CAddStock �Ի���

class CAddStock : public CDialogEx
{
	DECLARE_DYNAMIC(CAddStock)

public:
	CAddStock(_ConnectionPtr p, int id, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddStock();
	int m_adminid;
	_ConnectionPtr m_pConnection;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRecordset;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDSTOCK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CDotEdit m_purprice;
	afx_msg void OnBnClickedOk();
	CEdit m_supplierid;
	CEdit m_cdname;
	CEdit m_artist;
	CComboBox m_genre;
	CEdit m_purnum;
	double m_purpricenum;
	int m_purnumdata;
	CString m_artiststr;
	CString m_cdnamestr;
	int m_supplieriddata;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
