// NewSup.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CDStore.h"
#include "NewSup.h"
#include "afxdialogex.h"


// CNewSup �Ի���

IMPLEMENT_DYNAMIC(CNewSup, CDialogEx)

CNewSup::CNewSup(_ConnectionPtr p, int id, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NEWSUP, pParent)
{
	m_pConnection = p;
	m_adminid = id;
	mode = 1;
	m_supid = L"";
}

CNewSup::CNewSup(_ConnectionPtr p, int id, CString supid, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NEWSUP, pParent)
{
	m_pConnection = p;
	m_adminid = id;
	mode = 0;
	m_supid = supid;
}

CNewSup::~CNewSup()
{
}

void CNewSup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUPNAME1, m_supname);
	DDX_Control(pDX, IDC_SUPADDR1, m_supaddr);
	DDX_Control(pDX, IDC_SUPPHONE1, m_supphone);
}


BEGIN_MESSAGE_MAP(CNewSup, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNewSup::OnBnClickedOk)
END_MESSAGE_MAP()


// CNewSup ��Ϣ�������


BOOL CNewSup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CNewSup::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	OnBnClickedOk();
	CDialogEx::OnOK();
}


void CNewSup::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (mode == 1)
	{
		CString name, addr, phone, strSql, sqlHasRecord;
		m_supname.GetWindowTextW(name);
		m_supaddr.GetWindowTextW(addr);
		m_supphone.GetWindowTextW(phone);
		int insid;
		if (name.IsEmpty())
		{
			MessageBox(L"��Ӧ�����Ʋ���Ϊ�գ�", L"�޸Ĺ�Ӧ����Ϣ", MB_OK | MB_ICONWARNING);
			return;
		}
		else
		{
			m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
			sqlHasRecord = L"select * from Supplier order by sup_id desc";
			try
			{
				HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord,
					_variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
				if (SUCCEEDED(hr))
				{
					if (m_pRecordset->rsEOF && m_pRecordset->BOF) //��¼��Ϊ��
					{
						insid = 1;
					}
					else
					{
						CString temp;
						temp = m_pRecordset->GetCollect(L"sup_id");
						insid = _ttoi(temp) + 1;
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
				AfxMessageBox(temp1 + L"\r\n" + temp2);	///��ʾ������Ϣ
				return;
			}

			if (!addr.IsEmpty() && !phone.IsEmpty())
			{
				strSql.Format(L"insert into Supplier values(%d,'%s','%s','%s')", insid, name, addr, phone);
			}
			else if (!addr.IsEmpty() && phone.IsEmpty())
			{
				strSql.Format(L"insert into Supplier values(%d,'%s','%s',null)", insid, name, addr);
			}
			else if (addr.IsEmpty() && !phone.IsEmpty())
			{
				strSql.Format(L"insert into Supplier values(%d,'%s',null,'%s')", insid, name, phone);
			}
			else
			{
				strSql.Format(L"insert into Supplier values(%d,'%s',null,null)", insid, name);
			}

			try
			{
				m_pConnection->Execute(_bstr_t(strSql), 0, adCmdText);
			}
			catch (_com_error e)
			{
				MessageBox(e.Description(), L"���ӹ�Ӧ��", MB_OK | MB_ICONWARNING);
				return;
			}
			MessageBox(L"��ӳɹ���", L"���ӹ�Ӧ��", MB_OK | MB_ICONINFORMATION);
			CDialogEx::OnOK();
		}
	}
	else
	{
		CString name, addr, phone, strSql;
		m_supname.GetWindowTextW(name);
		m_supaddr.GetWindowTextW(addr);
		m_supphone.GetWindowTextW(phone);
		if (name.IsEmpty() && addr.IsEmpty() && phone.IsEmpty())
		{
			MessageBox(L"������Ϣ����ͬʱΪ�գ�", L"���¹�Ӧ����Ϣ", MB_OK | MB_ICONWARNING);
			return;
		}
		else if (name.IsEmpty() && addr.IsEmpty() && !phone.IsEmpty())
		{
			strSql.Format(L"update Supplier set phone='%s' where sup_id=%s", phone, m_supid);
		}
		else if (name.IsEmpty() && !addr.IsEmpty() && phone.IsEmpty())
		{
			strSql.Format(L"update Supplier set addr='%s' where sup_id=%s", addr, m_supid);
		}
		else if (name.IsEmpty() && !addr.IsEmpty() && !phone.IsEmpty())
		{
			strSql.Format(L"update Supplier set addr='%s',phone='%s' where sup_id=%s", addr, phone, m_supid);
		}
		else if (!name.IsEmpty() && addr.IsEmpty() && phone.IsEmpty())
		{
			strSql.Format(L"update Supplier set sup_name='%s' where sup_id=%s", name, m_supid);
		}
		else if (!name.IsEmpty() && addr.IsEmpty() && !phone.IsEmpty())
		{
			strSql.Format(L"update Supplier set sup_name='%s',phone='%s' where sup_id=%s", name, phone, m_supid);
		}
		else if (!name.IsEmpty() && !addr.IsEmpty() && phone.IsEmpty())
		{
			strSql.Format(L"update Supplier set sup_name='%s',addr='%s' where sup_id=%s", name, addr, m_supid);
		}
		else if(!name.IsEmpty() && !addr.IsEmpty() && !phone.IsEmpty())
		{
			strSql.Format(L"update Supplier set sup_name='%s',addr='%s',phone='%s' where sup_id=%s", name, addr, phone, m_supid);
		}

		try
		{
			m_pConnection->BeginTrans();
			m_pConnection->Execute(_bstr_t(strSql), 0, adCmdText);
		}
		catch (_com_error e)
		{
			m_pConnection->RollbackTrans();
			MessageBox(e.Description(), L"���¹�Ӧ����Ϣ", MB_OK | MB_ICONWARNING);
			return;
		}
		m_pConnection->CommitTrans();
		MessageBox(L"���³ɹ���", L"���¹�Ӧ����Ϣ", MB_OK | MB_ICONINFORMATION);
		CDialogEx::OnOK();
	}
	//CDialogEx::OnOK();
}
