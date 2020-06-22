// LookStock.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CDStore.h"
#include "LookStock.h"
#include "afxdialogex.h"


// CLookStock �Ի���

IMPLEMENT_DYNAMIC(CLookStock, CDialogEx)

CLookStock::CLookStock(_ConnectionPtr p, int id, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOOKSTOCK, pParent)
{
	m_pConnection = p;
	adminid = id;
}

CLookStock::~CLookStock()
{
}

void CLookStock::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STOCKLIST, m_stocklist);
}


BEGIN_MESSAGE_MAP(CLookStock, CDialogEx)
END_MESSAGE_MAP()


// CLookStock ��Ϣ�������


BOOL CLookStock::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
	CString sqlHasRecord;
	sqlHasRecord = L"select * from Stock order by pur_date";
	try
	{
		HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
		if (SUCCEEDED(hr))
		{
			while (!m_pRecordset->rsEOF)//�������ص�ÿһ����¼
			{
				CString str, all;
				str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("stock_id"));
				all = all + str + L"��";
				str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("admin_id"));
				all = all + str + L"��";
				str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("sup_id"));
				all = all + str + L"��";
				str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("cd_id"));
				all = all + str + L"��";
				str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("stock_id"));
				all = all + str + L"��";
				str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("pur_price"));
				all = all + str + L"��";
				str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("lent_count"));
				all = all + str + L"��";
				str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("pur_date"));
				all = all + str;
				m_stocklist.InsertString(m_stocklist.GetCount(), all);
				m_pRecordset->MoveNext();
			}
		}
		else
		{
			AfxMessageBox(L"Create instance of recordset failed!");
			return FALSE;
		}
		m_pRecordset->Close();
	}
	catch (_com_error e)
	{
		CString temp1 = e.Description(), temp2 = e.ErrorMessage();
		AfxMessageBox(temp1 + L"\r\n" + temp2);
		return FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
