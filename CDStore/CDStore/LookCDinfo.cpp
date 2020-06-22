// LookCDinfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CDStore.h"
#include "LookCDinfo.h"	 
#include "AddCDinfo.h"
#include "afxdialogex.h"


// CLookCDinfo �Ի���

IMPLEMENT_DYNAMIC(CLookCDinfo, CDialogEx)

CLookCDinfo::CLookCDinfo(_ConnectionPtr p, int id, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOOKCDINFO, pParent)
{
	m_pConnection = p;
	adminid = id;
}

CLookCDinfo::~CLookCDinfo()
{
}

void CLookCDinfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CDINFOLIST, m_cdinfolist);
}


BEGIN_MESSAGE_MAP(CLookCDinfo, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLookCDinfo::OnBnClickedOk)
	ON_BN_CLICKED(IDC_MODIFYINFO, &CLookCDinfo::OnBnClickedModifyinfo)
END_MESSAGE_MAP()


// CLookCDinfo ��Ϣ�������


BOOL CLookCDinfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
	CString sqlHasRecord;
	sqlHasRecord = L"select * from LookCDinfo order by cd_id";
	try
	{
		HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
		if (SUCCEEDED(hr))
		{
			while (!m_pRecordset->rsEOF)//�������ص�ÿһ����¼
			{
				CString str, all;
				str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("cd_id"));
				all = all + str + L"��";
				str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("cd_name"));
				all = all + str + L"��";
				str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("artist_name"));
				all = all + str + L"��";
				_variant_t vValue = m_pRecordset->GetCollect("genre_name");
				if (vValue.vt != VT_NULL)
				{
					str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("genre_name"));
				}
				else
				{
					str = L"δ֪���";
				}
				all = all + str;
				m_cdinfolist.InsertString(m_cdinfolist.GetCount(), all);
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
	m_cdinfolist.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CLookCDinfo::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAddCDinfo dlg(m_pConnection, adminid);
	if (dlg.DoModal() == IDOK)
	{
		m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
		CString sqlHasRecord;
		sqlHasRecord = L"select * from LookCDinfo order by cd_id desc";
		try
		{
			HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
			if (SUCCEEDED(hr))
			{
				if (!m_pRecordset->rsEOF)
				{
					CString str, all;
					str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("cd_id"));
					all = all + str + L"��";
					str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("cd_name"));
					all = all + str + L"��";
					str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("artist_name"));
					all = all + str + L"��";
					_variant_t vValue = m_pRecordset->GetCollect("genre_name");
					if (vValue.vt != VT_NULL)
					{
						str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("genre_name"));
					}
					else
					{
						str = L"δ֪���";
					}
					all = all + str;
					m_cdinfolist.InsertString(m_cdinfolist.GetCount(), all);
				}
			}
			else
			{
				AfxMessageBox(L"Create instance of recordset failed!");
				return;
			}
			m_pRecordset->Close();
		}
		catch (_com_error e)
		{
			CString temp1 = e.Description(), temp2 = e.ErrorMessage();
			AfxMessageBox(temp1 + L"\r\n" + temp2);
			return;
		}
	}
	//CDialogEx::OnOK();
}


void CLookCDinfo::OnBnClickedModifyinfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	m_cdinfolist.GetText(m_cdinfolist.GetCurSel(), str);
	str = str.Left(str.Find(L"��"));
	int cdid = _ttoi(str);
	CAddCDinfo dlg(m_pConnection, adminid, cdid);
	if (dlg.DoModal() == IDOK)
	{
		m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
		CString sqlHasRecord;
		sqlHasRecord.Format(L"select * from LookCDinfo where cd_id=%d", cdid);
		try
		{
			HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
			if (SUCCEEDED(hr))
			{
				if (!m_pRecordset->rsEOF)
				{
					CString str, all;
					str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("cd_id"));
					all = all + str + L"��";
					str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("cd_name"));
					all = all + str + L"��";
					str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("artist_name"));
					all = all + str + L"��";
					_variant_t vValue = m_pRecordset->GetCollect("genre_name");
					if (vValue.vt != VT_NULL)
					{
						str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("genre_name"));
					}
					else
					{
						str = L"δ֪���";
					}
					all = all + str;
					int curr = m_cdinfolist.GetCurSel();
					m_cdinfolist.DeleteString(curr);
					m_cdinfolist.InsertString(curr, all);
					m_cdinfolist.SetCurSel(curr);
				}
			}
			else
			{
				AfxMessageBox(L"Create instance of recordset failed!");
				return;
			}
			m_pRecordset->Close();
		}
		catch (_com_error e)
		{
			CString temp1 = e.Description(), temp2 = e.ErrorMessage();
			AfxMessageBox(temp1 + L"\r\n" + temp2);
			return;
		}
	}
}
