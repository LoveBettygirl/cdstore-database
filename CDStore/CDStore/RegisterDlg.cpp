// RegisterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CDStore.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"


// CRegisterDlg �Ի���

IMPLEMENT_DYNAMIC(CRegisterDlg, CDialogEx)

CRegisterDlg::CRegisterDlg(_ConnectionPtr p,CWnd* pParent)
	: CDialogEx(IDD_REGISTER, pParent)
	, m_balancedata(0)
	, m_passwordstr(_T(""))
{
	m_pConnection = p;
}

CRegisterDlg::~CRegisterDlg()
{
}

void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BALANCE, m_balance);
	DDX_Control(pDX, IDC_PASSWORD1, m_password1);
	DDX_Control(pDX, IDC_PASSWORD2, m_password2);
	DDX_Text(pDX, IDC_BALANCE, m_balancedata);
	DDX_Text(pDX, IDC_PASSWORD1, m_passwordstr);
}


BEGIN_MESSAGE_MAP(CRegisterDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRegisterDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRegisterDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CRegisterDlg ��Ϣ�������


void CRegisterDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString pwd1, pwd2, bal;
	m_password1.GetWindowTextW(pwd1);
	m_password2.GetWindowTextW(pwd2);
	m_balance.GetWindowTextW(bal);
	if (pwd1.IsEmpty())
	{
		MessageBox(L"���벻��Ϊ�գ�", L"Register", MB_OK | MB_ICONWARNING);
	}
	else if (pwd2.IsEmpty())
	{
		MessageBox(L"ȷ�����벻��Ϊ�գ�", L"Register", MB_OK | MB_ICONWARNING);
	}
	else if (bal.IsEmpty())
	{
		MessageBox(L"��ֵ����Ϊ�գ�", L"Register", MB_OK | MB_ICONWARNING);
	}
	else
	{
		if (pwd1 != pwd2)
		{
			MessageBox(L"�����������벻һ�£����������룡", L"Register", MB_OK | MB_ICONWARNING);
		}
		else if (pwd1.GetLength() < 6 || pwd1.GetLength() > 30)
		{
			MessageBox(L"������벻����6���ַ��Ҳ�����30���ַ���", L"Register", MB_OK | MB_ICONWARNING);
		}
		else
		{
			UpdateData(true);
			CString strTime, cusid, strSql, sqlHasRecord, temp1;
			CTime tm;
			tm = CTime::GetCurrentTime();
			strTime = tm.Format("%Y%m%d");
			m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
			sqlHasRecord.Format(L"select * from Customer where cus_id like '%s", strTime);
			temp1 = L"%' order by register_date desc";
			sqlHasRecord += temp1;
			HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, 
				_variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
			if (SUCCEEDED(hr))
			{
				//�����¹˿ͻ�ԱID
				if (m_pRecordset->rsEOF && m_pRecordset->BOF) //��¼��Ϊ��
				{
					cusid.Format(L"%s_%d", strTime, 1);
				}
				else
				{
					CString temp2;
					temp2 = m_pRecordset->GetCollect(L"cus_id");
					int down = temp2.Find('_');
					temp2 = temp2.Mid(down + 1);
					cusid.Format(L"%s_%d", strTime, _ttoi(temp2) + 1);
				}
			}
			else
			{
				AfxMessageBox(L"Create instance of recordset failed!");
				return;
			}
			m_pRecordset->Close();

			//ִ�в������
			strSql.Format(L"insert into Customer values('%s','%s',%lf,getdate())", 
				cusid, m_passwordstr, m_balancedata);
			try
			{
				m_pConnection->Execute(_bstr_t(strSql), 0, adCmdText);
			}
			catch (_com_error e)
			{
				MessageBox(e.Description(), L"Register", MB_OK | MB_ICONWARNING);
				return;
			}
			CString success;
			success.Format(L"ע��ɹ���\r\n�ʻ�ID��%s\r\n���μ�����ʻ�ID�������Ա��´�ʹ�ã�", cusid);
			MessageBox(success, L"Register", MB_OK | MB_ICONINFORMATION);
			CDialogEx::OnOK();
		}
	}
	//CDialogEx::OnOK();
}


void CRegisterDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CRegisterDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	OnBnClickedOk();
	//CDialogEx::OnOK();
}


BOOL CRegisterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_balance.SetWindowTextW(L"");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
