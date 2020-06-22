// AddBal.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CDStore.h"
#include "AddBal.h"
#include "afxdialogex.h"


// CAddBal �Ի���

IMPLEMENT_DYNAMIC(CAddBal, CDialogEx)

CAddBal::CAddBal(_ConnectionPtr p, CString cusid, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ADDBAL, pParent)
	, m_pwdstr(_T(""))
	, m_balstr(_T(""))
{
	m_pConnection = p;
	id = cusid;
	m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
	CString sqlHasRecord;
	sqlHasRecord.Format(L"select * from Customer where cus_id='%s'", id);
	try
	{
		HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
		if (SUCCEEDED(hr))
		{
			balance = _ttof(_bstr_t(m_pRecordset->GetCollect("balance")));
			//balance = m_pRecordset->GetCollect("balance");
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
}

CAddBal::~CAddBal()
{
}

void CAddBal::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BALBAL, m_balance);
	DDX_Control(pDX, IDC_BALDISP, m_baldisp);
	DDX_Control(pDX, IDC_PWDBAL, m_pwdbal);
	DDX_Text(pDX, IDC_PWDBAL, m_pwdstr);
	DDX_Text(pDX, IDC_BALBAL, m_balstr);
}


BEGIN_MESSAGE_MAP(CAddBal, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddBal::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddBal ��Ϣ�������


void CAddBal::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString pwd, bal;
	m_pwdbal.GetWindowTextW(pwd);
	m_balance.GetWindowTextW(bal);
	if (pwd.IsEmpty())
	{
		MessageBox(L"���벻��Ϊ�գ�", L"��ֵ���", MB_OK | MB_ICONWARNING);
		return;
	}
	else if (bal.IsEmpty())
	{
		MessageBox(L"����Ϊ�գ�", L"��ֵ���", MB_OK | MB_ICONWARNING);
		return;
	}
	else
	{
		UpdateData(true);
		CString strSql, sqlHasRecord;
		sqlHasRecord.Format(L"select * from Customer where cus_id='%s' and cus_password='%s'", id, m_pwdstr);
		try
		{
			HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
			if (SUCCEEDED(hr))
			{
				if (m_pRecordset->rsEOF && m_pRecordset->BOF) //��¼��Ϊ�գ���������ʻ�ID���������û�����κμ�¼ƥ����
				{
					MessageBox(L"�������", L"��ֵ���", MB_OK | MB_ICONWARNING);
					m_pRecordset->Close();
					return;
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
			AfxMessageBox(temp1 + L"\r\n" + temp2);	  ///��ʾ������Ϣ
			return;
		}
		if (_ttof(m_balstr) <= 0)
		{
			MessageBox(L"����Ϊ0��", L"��ֵ���", MB_OK | MB_ICONWARNING);
			return;
		}
		strSql.Format(L"update Customer set balance=balance+%s where cus_id='%s'",
			m_balstr, id);
		try
		{
			m_pConnection->BeginTrans();
			m_pConnection->Execute(_bstr_t(strSql), 0, adCmdText);
			m_pConnection->CommitTrans();
		}
		catch (_com_error e)
		{
			m_pConnection->RollbackTrans();
			CString temp1 = e.Description(), temp2 = e.ErrorMessage();
			AfxMessageBox(temp1 + L"\r\n" + temp2);	  ///��ʾ������Ϣ
			return; 
		}
		CString success;
		balance += _ttof(m_balstr);
		success.Format(L"��ֵ���ɹ���\n�����ڵ���%lf", balance);
		success = success.Left(success.Find('.') + 3);
		MessageBox(success, L"��ֵ���", MB_OK | MB_ICONINFORMATION);
		CDialogEx::OnOK();
	}
	//CDialogEx::OnOK();
}



void CAddBal::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	OnBnClickedOk();
	//CDialogEx::OnOK();
}


BOOL CAddBal::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString str1, str2;
	((CStatic*)GetDlgItem(IDC_BALDISP))->GetWindowTextW(str1);
	str2.Format(L"%lf", balance);
	str2 = str2.Left(str2.Find('.') + 3);
	str1 += str2;
	((CStatic*)GetDlgItem(IDC_BALDISP))->SetWindowTextW(str1);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
