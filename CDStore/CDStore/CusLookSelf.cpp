// CusLookSelf.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CDStore.h"
#include "CusLookSelf.h"
#include "afxdialogex.h"


// CCusLookSelf �Ի���

IMPLEMENT_DYNAMIC(CCusLookSelf, CDialogEx)

CCusLookSelf::CCusLookSelf(_ConnectionPtr p, CString cusid, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CUSLOOKSELF, pParent)
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
			regtime = m_pRecordset->GetCollect("register_date");
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

CCusLookSelf::~CCusLookSelf()
{
}

void CCusLookSelf::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SELFLENT, m_selflent);
	DDX_Control(pDX, IDC_SELFBUY, m_selfbuy);
}


BEGIN_MESSAGE_MAP(CCusLookSelf, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCusLookSelf::OnBnClickedOk)
END_MESSAGE_MAP()


// CCusLookSelf ��Ϣ�������


BOOL CCusLookSelf::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString str1, str2;
	((CStatic*)GetDlgItem(IDC_MYID))->GetWindowTextW(str1);
	str2.Format(L"%s", id);
	str1 += str2;
	((CStatic*)GetDlgItem(IDC_MYID))->SetWindowTextW(str1);
	((CStatic*)GetDlgItem(IDC_MYBAL))->GetWindowTextW(str1);
	str2.Format(L"%lf", balance);
	str2 = str2.Left(str2.Find('.') + 3);
	str1 += str2;
	((CStatic*)GetDlgItem(IDC_MYBAL))->SetWindowTextW(str1);
	((CStatic*)GetDlgItem(IDC_MYTIME))->GetWindowTextW(str1);
	str2.Format(L"%s", regtime);
	str1 += str2;
	((CStatic*)GetDlgItem(IDC_MYTIME))->SetWindowTextW(str1);

	m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
	CString sqlHasRecord;
	sqlHasRecord.Format(L"select * from Trade where cus_id='%s'", id);
	try
	{
		HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
		if (SUCCEEDED(hr))
		{
			while (!m_pRecordset->rsEOF)//�������ص�ÿһ����¼
			{
				CString cdid, type;
				cdid = m_pRecordset->GetCollect("stock_id");
				type = m_pRecordset->GetCollect("trade_type");
				if (_ttoi(type)!=0)
				{
					m_selfbuy.InsertString(m_selfbuy.GetCount(), cdid);
				}
				else
				{
					m_selflent.InsertString(m_selflent.GetCount(), cdid);
				}
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
		AfxMessageBox(temp1 + L"\r\n" + temp2);	///��ʾ������Ϣ
		return FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CCusLookSelf::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}
