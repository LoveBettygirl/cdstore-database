// LookCus.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CDStore.h"
#include "LookCus.h"
#include "afxdialogex.h"


// CLookCus �Ի���

IMPLEMENT_DYNAMIC(CLookCus, CDialogEx)

CLookCus::CLookCus(_ConnectionPtr p, int id, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOOKCUS, pParent)
{
	m_pConnection = p;
	adminid = id;
}

CLookCus::~CLookCus()
{
}

void CLookCus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOOKCUSID, m_cusid);
	DDX_Control(pDX, IDC_CUSLENT, m_cuslent);
	DDX_Control(pDX, IDC_CUSBUY, m_cusbuy);
	DDX_Control(pDX, IDC_CUSBAL, m_cusbal);
	DDX_Control(pDX, IDC_CUSTIME, m_custime);
}


BEGIN_MESSAGE_MAP(CLookCus, CDialogEx)
	ON_BN_CLICKED(IDC_SEARCHCUS, &CLookCus::OnBnClickedSearchcus)
END_MESSAGE_MAP()


// CLookCus ��Ϣ�������


void CLookCus::OnBnClickedSearchcus()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString cusid;
	m_cusid.GetWindowTextW(cusid);
	((CStatic*)GetDlgItem(IDC_CUSBAL))->SetWindowTextW(L"��");
	((CStatic*)GetDlgItem(IDC_CUSTIME))->SetWindowTextW(L"ע��ʱ�䣺");
	m_cuslent.ResetContent();
	m_cusbuy.ResetContent();
	if (cusid.IsEmpty())
	{
		MessageBox(L"�˿��ʻ�ID����Ϊ�գ�", L"�鿴�˿��ʻ�", MB_OK | MB_ICONWARNING);
		return;
	}
	else
	{
		m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
		CString sqlHasRecord;
		sqlHasRecord.Format(L"select * from Customer where cus_id='%s'", cusid);
		try
		{
			HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
			if (SUCCEEDED(hr))
			{
				if (m_pRecordset->rsEOF && m_pRecordset->BOF) //��¼��Ϊ��
				{
					MessageBox(L"�����ڴ˹˿ͣ�", L"�鿴�˿��ʻ�", MB_OK | MB_ICONWARNING);
					m_pRecordset->Close();
					return;
				}
				else
				{
					CString ttime, str1, str2;
					double bal;
					bal = _ttof(_bstr_t(m_pRecordset->GetCollect("balance")));
					ttime = m_pRecordset->GetCollect("register_date");
					((CStatic*)GetDlgItem(IDC_CUSBAL))->GetWindowTextW(str1);
					str2.Format(L"%lf", bal);
					str2 = str2.Left(str2.Find('.') + 3);
					str1 += str2;
					((CStatic*)GetDlgItem(IDC_CUSBAL))->SetWindowTextW(str1);
					((CStatic*)GetDlgItem(IDC_CUSTIME))->GetWindowTextW(str1);
					str2.Format(L"%s", ttime);
					str1 += str2;
					((CStatic*)GetDlgItem(IDC_CUSTIME))->SetWindowTextW(str1);
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

		m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
		sqlHasRecord.Format(L"select * from Trade where cus_id='%s'", cusid);
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
					if (_ttoi(type) == 1)
					{
						m_cusbuy.InsertString(m_cusbuy.GetCount(), cdid);
					}
					else
					{
						m_cuslent.InsertString(m_cuslent.GetCount(), cdid);
					}
					m_pRecordset->MoveNext();
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
	}
}


void CLookCus::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	OnBnClickedSearchcus();
	//CDialogEx::OnOK();
}
