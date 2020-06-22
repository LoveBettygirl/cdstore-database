// CustomerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CDStore.h"
#include "CustomerDlg.h"
#include "ModPassword.h"
#include "LookCD.h"
#include "Trade.h"
#include "GiveBack.h"
#include "AddBal.h"
#include "CusLookSelf.h"
#include "DeleteCus.h"
#include "afxdialogex.h"


// CCustomerDlg �Ի���

IMPLEMENT_DYNAMIC(CCustomerDlg, CDialogEx)

CCustomerDlg::CCustomerDlg(_ConnectionPtr p,CString cusid,CWnd* pParent /*= NULL*/)
	: CDialogEx(IDD_CUSTOMERDLG, pParent)
	, id(cusid)
{
	m_pConnection = p;
}

CCustomerDlg::~CCustomerDlg()
{
}

void CCustomerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GREETING, m_greeting);
}


BEGIN_MESSAGE_MAP(CCustomerDlg, CDialogEx)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, &CCustomerDlg::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CCustomerDlg ��Ϣ�������


BOOL CCustomerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString temp1;
	temp1.Format(L"��ã�%s����ѡ��һ�����ܣ�", id);
	m_greeting.SetWindowTextW(temp1); 
	((CButton *)GetDlgItem(IDC_LOOKCD))->SetCheck(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


int CCustomerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	return 0;
}


void CCustomerDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (((CButton *)GetDlgItem(IDC_MODPASSWORD))->GetCheck())
	{
		CModPassword dlg(m_pConnection, id);
		dlg.DoModal();
	}
	else if (((CButton *)GetDlgItem(IDC_LOOKCD))->GetCheck())
	{
		CLookCD	dlg(m_pConnection);
		dlg.DoModal();
	}
	else if (((CButton *)GetDlgItem(IDC_LOGOUT))->GetCheck())
	{
		if (MessageBox(L"ȷ���˳���¼��", L"�˿�ѡ����", MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			CDialogEx::OnOK();
		}
	}
	else if (((CButton *)GetDlgItem(IDC_TRADE))->GetCheck())
	{
		CTrade dlg(m_pConnection, id);
		dlg.DoModal();
	}
	else if (((CButton *)GetDlgItem(IDC_GIVEBACK))->GetCheck())
	{
		CGiveBack dlg(m_pConnection, id);
		dlg.DoModal();
	}
	else if (((CButton *)GetDlgItem(IDC_ADDBAL))->GetCheck())
	{
		CAddBal dlg(m_pConnection, id);
		dlg.DoModal();
	}
	else if (((CButton *)GetDlgItem(IDC_CUSLOOKSELF))->GetCheck())
	{
		CCusLookSelf dlg(m_pConnection, id);
		dlg.DoModal();
	}
	else if (((CButton *)GetDlgItem(IDC_DELETECUS))->GetCheck())
	{
		if (MessageBox(L"ȷ��ע���ʻ���ע���ʻ�֮ǰ���ȹ黹����CD���ұ�֤�ʻ����С��20Ԫ��"
			"\nע���ʻ���ϵͳ���ջ��������ѹ����CD��ʹ��Ȩ��Ҳ�����˻���\n"
			"ȷ�ϼ�����", L"�˿�ѡ����", 
			MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			/*try
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
				m_pCommand->Execute(NULL, NULL, adCmdStoredProc); //ִ�д洢����
				int nRet = m_pCommand->Parameters->GetItem("Return")->GetValue();
				CString tips = m_pCommand->Parameters->GetItem("typs")->GetValue();
				if (nRet == 0)	 //������ɾ������
				{
					MessageBox(tips, L"ע���˿��ʻ�", MB_OK | MB_ICONWARNING);
					return;
				}
				else if (nRet == 1)	 //����ɾ������
				{
					MessageBox(tips, L"ע���˿��ʻ�", MB_OK | MB_ICONINFORMATION);
					CDialogEx::OnOK();
				}
			}
			catch (_com_error e)
			{
				m_pConnection->RollbackTrans();
				CString temp1 = e.Description(), temp2 = e.ErrorMessage();
				AfxMessageBox(temp1 + L"\r\n" + temp2);	///��ʾ������Ϣ
				return;
			} */
			CDeleteCus dlg(m_pConnection, id);
			if (dlg.DoModal() == IDOK)
			{
				CDialogEx::OnOK();
			}
		}
	}
	//CDialogEx::OnOK();
}


void CCustomerDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (MessageBox(L"ȷ���˳���¼��", L"�˿�ѡ����", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		CDialogEx::OnClose();
	}
}
