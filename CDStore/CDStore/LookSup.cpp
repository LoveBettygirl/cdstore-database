// LookSup.cpp : 实现文件
//

#include "stdafx.h"
#include "CDStore.h"
#include "LookSup.h"
#include "NewSup.h"
#include "afxdialogex.h"


// CLookSup 对话框

IMPLEMENT_DYNAMIC(CLookSup, CDialogEx)

CLookSup::CLookSup(_ConnectionPtr p, int id, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOOKSUPPLIER, pParent)
{
	m_pConnection = p;
	m_adminid = id;
}

CLookSup::~CLookSup()
{
}

void CLookSup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUPIDLIST, m_supidlist);
}


BEGIN_MESSAGE_MAP(CLookSup, CDialogEx)
	ON_LBN_SELCHANGE(IDC_SUPIDLIST, &CLookSup::OnLbnSelchangeSupidlist)
	ON_BN_CLICKED(IDC_MODIFY, &CLookSup::OnBnClickedModify)
	ON_BN_CLICKED(IDC_ADDSUP, &CLookSup::OnBnClickedAddsup)
END_MESSAGE_MAP()


// CLookSup 消息处理程序


BOOL CLookSup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pRecordset.CreateInstance(__uuidof(Recordset)); //创建实例
	CString sqlHasRecord, name, addr, phone;
	sqlHasRecord = L"select * from Supplier";
	try
	{
		HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
		if (SUCCEEDED(hr))
		{
			int i = 0;
			while (!m_pRecordset->rsEOF)//遍历返回的每一条记录
			{
				CString str;
				str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("sup_id"));
				if (i == 0)
				{
					name = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("sup_name"));
					_variant_t vValue = m_pRecordset->GetCollect("addr");
					if (vValue.vt != VT_NULL)
					{
						addr = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("addr"));
					}
					else
					{
						addr = L"未知";
					}
					vValue = m_pRecordset->GetCollect("phone");
					if (vValue.vt != VT_NULL)
					{
						phone = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("phone"));
					}
					else
					{
						phone = L"未知";
					}
					CString str1 = L"名称：", str2 = L"地址：", str3 = L"电话：";
					str1 += name;
					str2 += addr;
					str3 += phone;
					((CStatic*)GetDlgItem(IDC_SUPNAME))->SetWindowTextW(str1);
					((CStatic*)GetDlgItem(IDC_SUPADDR))->SetWindowTextW(str2);
					((CStatic*)GetDlgItem(IDC_SUPPHONE))->SetWindowTextW(str3);
				}
				m_supidlist.InsertString(m_supidlist.GetCount(), str);
				m_pRecordset->MoveNext();
				i++;
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

	m_supidlist.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CLookSup::OnLbnSelchangeSupidlist()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strText;
	int nCurSel;
	nCurSel = m_supidlist.GetCurSel();    
	m_supidlist.GetText(nCurSel, strText);  
	m_pRecordset.CreateInstance(__uuidof(Recordset)); //创建实例
	CString sqlHasRecord;
	sqlHasRecord.Format(L"select * from Supplier where sup_id=%s", strText);
	try
	{
		HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
		if (SUCCEEDED(hr))
		{
			CString name, addr, phone;
			name = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("sup_name"));
			_variant_t vValue = m_pRecordset->GetCollect("addr");
			if (vValue.vt != VT_NULL)
			{
				addr = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("addr"));
			}
			else
			{
				addr = L"未知";
			}
			vValue = m_pRecordset->GetCollect("phone");
			if (vValue.vt != VT_NULL)
			{
				phone = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("phone"));
			}
			else
			{
				phone = L"未知";
			}
			CString str1 = L"名称：", str2 = L"地址：", str3 = L"电话：";
			str1 += name;
			str2 += addr;
			str3 += phone;
			((CStatic*)GetDlgItem(IDC_SUPNAME))->SetWindowTextW(str1);
			((CStatic*)GetDlgItem(IDC_SUPADDR))->SetWindowTextW(str2);
			((CStatic*)GetDlgItem(IDC_SUPPHONE))->SetWindowTextW(str3);
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

void CLookSup::OnBnClickedModify()
{
	// TODO: 在此添加控件通知处理程序代码
	CString supid;
	m_supidlist.GetText(m_supidlist.GetCurSel(), supid);
	CNewSup dlg(m_pConnection, m_adminid, supid);
	if (dlg.DoModal() == IDOK)
	{
		CString str1 = L"名称：", str2 = L"地址：", str3 = L"电话：";
		CString strText;
		CString name, addr, phone;
		int nCurSel;
		nCurSel = m_supidlist.GetCurSel();
		m_supidlist.GetText(nCurSel, strText);
		CString sqlHasRecord;
		sqlHasRecord.Format(L"select * from Supplier where sup_id=%s", strText);
		try
		{
			HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
			if (SUCCEEDED(hr))
			{
				name = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("sup_name"));
				_variant_t vValue = m_pRecordset->GetCollect("addr");
				if (vValue.vt != VT_NULL)
				{
					addr = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("addr"));
				}
				else
				{
					addr = L"未知";
				}
				vValue = m_pRecordset->GetCollect("phone");
				if (vValue.vt != VT_NULL)
				{
					phone = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("phone"));
				}
				else
				{
					phone = L"未知";
				}
				CString str1 = L"名称：", str2 = L"地址：", str3 = L"电话：";
				str1 += name;
				str2 += addr;
				str3 += phone;
				((CStatic*)GetDlgItem(IDC_SUPNAME))->SetWindowTextW(str1);
				((CStatic*)GetDlgItem(IDC_SUPADDR))->SetWindowTextW(str2);
				((CStatic*)GetDlgItem(IDC_SUPPHONE))->SetWindowTextW(str3);
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


void CLookSup::OnBnClickedAddsup()
{
	// TODO: 在此添加控件通知处理程序代码
	CNewSup dlg(m_pConnection, m_adminid);
	if (dlg.DoModal() == IDOK)
	{
		m_pRecordset.CreateInstance(__uuidof(Recordset)); //创建实例
		CString sqlHasRecord;
		sqlHasRecord = L"select * from Supplier order by sup_id desc";
		int add;
		try
		{
			HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
			if (SUCCEEDED(hr))
			{
				if (m_pRecordset->rsEOF && m_pRecordset->BOF) //记录集为空
				{
					add = 1;
					m_pRecordset->Close();
					return;
				}
				else
				{
					CString adds;
					adds = m_pRecordset->GetCollect("sup_id");
					add = _ttoi(adds);
				}
				CString str;
				str.Format(L"%d", add);
				m_supidlist.InsertString(m_supidlist.GetCount(), str);
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