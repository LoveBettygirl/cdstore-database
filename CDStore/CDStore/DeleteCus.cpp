// DeleteCus.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CDStore.h"
#include "DeleteCus.h"
#include "afxdialogex.h"


// CDeleteCus �Ի���

IMPLEMENT_DYNAMIC(CDeleteCus, CDialogEx)

CDeleteCus::CDeleteCus(_ConnectionPtr p, CString cusid, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CANCEL, pParent)
{
	m_pConnection = p;
	id = cusid;
}

CDeleteCus::~CDeleteCus()
{
}

void CDeleteCus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CALPWD, m_calpwd);
}


BEGIN_MESSAGE_MAP(CDeleteCus, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDeleteCus::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDeleteCus::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDeleteCus ��Ϣ�������


void CDeleteCus::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
	CString pwd;
	m_calpwd.GetWindowTextW(pwd);
	if (pwd.IsEmpty())
	{
		MessageBox(L"���벻��Ϊ�գ�", L"�˿�ע���ʻ�", MB_OK | MB_ICONWARNING);
		return;
	}
	else
	{
		m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
		CString sqlHasRecord;
		sqlHasRecord.Format(L"select * from Customer where cus_id='%s' and cus_password='%s'", id, pwd);
		try
		{
			HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true),
				adOpenDynamic, adLockPessimistic, adCmdText);
			if (SUCCEEDED(hr))
			{
				if (m_pRecordset->rsEOF && m_pRecordset->BOF) //��¼��Ϊ��
				{
					MessageBox(L"�������", L"�˿�ע���ʻ�", MB_OK | MB_ICONWARNING);
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
			AfxMessageBox(temp1 + L"\r\n" + temp2);	 ///��ʾ������Ϣ
			return;
		}

		try
		{
			HRESULT hr = m_pCommand.CreateInstance(__uuidof(Command));
			if (FAILED(hr))
			{
				AfxMessageBox(L"Create instance of command failed!!");
				return;
			}
			m_pCommand->Parameters->Refresh();
			m_pCommand->Parameters->Append(m_pCommand->CreateParameter(_bstr_t("Return"),
				adInteger, adParamReturnValue, sizeof(int)));
			m_pCommand->Parameters->Append(m_pCommand->CreateParameter(_bstr_t("cus_id"),
				adVarChar, adParamInput, 30, _variant_t(id)));
			m_pCommand->Parameters->Append(m_pCommand->CreateParameter(_bstr_t("tips"),
				adVarChar, adParamOutput, 100));
			m_pCommand->CommandText = _bstr_t("sp_deletecus");
			m_pCommand->ActiveConnection = m_pConnection;
			m_pCommand->CommandType = adCmdStoredProc;
			m_pConnection->BeginTrans();
			m_pCommand->Execute(NULL, NULL, adCmdStoredProc); //ִ�д洢����
			int nRet = m_pCommand->Parameters->GetItem("Return")->GetValue();
			CString tips = m_pCommand->Parameters->GetItem("tips")->GetValue();
			if (nRet == 0)	 //������ɾ������
			{
				m_pConnection->RollbackTrans();
				MessageBox(tips, L"�˿�ע���ʻ�", MB_OK | MB_ICONWARNING);
				return;
			}
			else if (nRet == 1)	 //����ɾ������
			{
				m_pConnection->CommitTrans();
				MessageBox(tips, L"�˿�ע���ʻ�", MB_OK | MB_ICONINFORMATION);
				CDialogEx::OnOK();
			}
		}
		catch (_com_error e)
		{
			m_pConnection->RollbackTrans();
			CString temp1 = e.Description(), temp2 = e.ErrorMessage();
			AfxMessageBox(temp1 + L"\r\n" + temp2);	///��ʾ������Ϣ
			return;
		}
	}
}


void CDeleteCus::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (MessageBox(L"ȡ��ע���ʻ���ע�������󽫻ᱻȡ����", L"�˿�ע���ʻ�", MB_YESNO | MB_ICONINFORMATION)==IDYES)
	{
		CDialogEx::OnCancel();
	}
}


void CDeleteCus::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	OnBnClickedOk();
	//CDialogEx::OnOK();
}
