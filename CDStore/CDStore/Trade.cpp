// Trade.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CDStore.h"
#include "Trade.h"
#include "afxdialogex.h"


// CTrade �Ի���

IMPLEMENT_DYNAMIC(CTrade, CDialogEx)

CTrade::CTrade(_ConnectionPtr p, CString cusid, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TRADE, pParent)
	, m_cdidstr(_T(""))
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

CTrade::~CTrade()
{
}

void CTrade::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CDID, m_cdid);
	DDX_Text(pDX, IDC_CDID, m_cdidstr);
}


BEGIN_MESSAGE_MAP(CTrade, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTrade::OnBnClickedOk)
END_MESSAGE_MAP()


// CTrade ��Ϣ�������


BOOL CTrade::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	((CButton *)GetDlgItem(IDC_BUY))->SetCheck(TRUE);
	CString str1, str2;
	((CStatic*)GetDlgItem(IDC_DISPBALANCE))->GetWindowTextW(str1);
	str2.Format(L"%lf", balance);
	str2 = str2.Left(str2.Find('.') + 3);
	str1 += str2;
	((CStatic*)GetDlgItem(IDC_DISPBALANCE))->SetWindowTextW(str1);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CTrade::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString cdid;
	m_cdid.GetWindowTextW(cdid);
	if (cdid.IsEmpty())
	{
		MessageBox(L"CD�Ų���Ϊ�գ�", L"��������CD", MB_OK | MB_ICONWARNING);
		return;
	}
	else
	{
		UpdateData(true);
		if (((CButton*)GetDlgItem(IDC_BUY))->GetCheck())
		{
			tradetype = 1;
		}
		else
		{
			tradetype = 0;
		}
		CString type;
		type.Format(L"%d", tradetype);
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
			m_pCommand->Parameters->Append(m_pCommand->CreateParameter(_bstr_t("stock_id"), 
				adVarChar, adParamInput, 30, _variant_t(m_cdidstr)));
			m_pCommand->Parameters->Append(m_pCommand->CreateParameter(_bstr_t("trade_type"), 
				adBoolean, adParamInput, sizeof(BOOL), _variant_t(type)));
			m_pCommand->Parameters->Append(m_pCommand->CreateParameter(_bstr_t("trade_price"), 
				adDouble, adParamOutput, sizeof(double)));
			m_pCommand->Parameters->Append(m_pCommand->CreateParameter(_bstr_t("trade_balance"), 
				adDouble, adParamOutput, sizeof(double)));
			m_pCommand->CommandText = _bstr_t("sp_trade");
			m_pCommand->ActiveConnection = m_pConnection;
			m_pCommand->CommandType = adCmdStoredProc;
			m_pConnection->BeginTrans();
			m_pCommand->Execute(NULL, NULL, adCmdStoredProc); //ִ�д洢����
			int nRet = m_pCommand->Parameters->GetItem("Return")->GetValue();
			double price = m_pCommand->Parameters->GetItem("trade_price")->GetValue();
			double bal = m_pCommand->Parameters->GetItem("trade_balance")->GetValue();
			m_pConnection->CommitTrans();
			if (nRet == -1)	 //�������в����ڵ�CD��Υ���洢����
			{
				MessageBox(L"���û�д�CD��", L"��������CD", MB_OK | MB_ICONWARNING);
				return;
			}
			else if (nRet == 0)	 //����������û����CD��Υ���洢����
			{
				MessageBox(L"��CD�ѱ������˿͹���������ڱ����ã�", L"��������CD", MB_OK | MB_ICONWARNING);
				return;
			}
			else if (nRet == 1)	 //�����Լ��ѹ����CD��Υ���洢����
			{
				MessageBox(L"���ѹ����CD��", L"��������CD", MB_OK | MB_ICONWARNING);
				return;
			}
			else if (nRet == 2)	 //���������õ�CD�ɹ�
			{
				CString str;
				str.Format(L"����ɹ���\r\n�����%lf", bal);
				str = str.Left(str.Find('.') + 3);
				MessageBox(str, L"��������CD", MB_OK | MB_ICONINFORMATION);
				return;
			}
			else if (nRet == 3)	 //���������õ�CD���������㣬Υ���洢����
			{
				CString str1, str2;
				str1.Format(L"���㣬���ȳ�ֵ��\r\n����CD��%lf", price);
				str1 = str1.Left(str1.Find('.') + 3);
				str2.Format(L"\r\n�����%lf", bal);
				str2 = str2.Left(str2.Find('.') + 3);
				str1 += str2;
				MessageBox(str1, L"��������CD", MB_OK | MB_ICONWARNING);
				return;
			}
			else if (nRet == 4)	//�ظ������Լ��������õ�CD��Υ���洢����
			{
				MessageBox(L"�����������ô�CD�������ظ����ã�", L"��������CD", MB_OK | MB_ICONWARNING);
				return;
			}
			else if (nRet == 5)	 //���ÿ���CD�������㣬���³ɹ�
			{
				CString str;
				str.Format(L"���óɹ���\r\n�����%lf", bal);
				str = str.Left(str.Find('.') + 3);
				MessageBox(str, L"��������CD", MB_OK | MB_ICONINFORMATION);
			}
			else if (nRet == 6)	 //�������CD�������㣬���³ɹ�
			{
				CString str;
				str.Format(L"����ɹ���\r\n�����%lf", bal);
				str = str.Left(str.Find('.') + 3);
				MessageBox(str, L"��������CD", MB_OK | MB_ICONINFORMATION);
			}
			else if (nRet == 7)	 //���ÿ���CD�����㣬Υ���洢����
			{
				CString str1, str2;
				str1.Format(L"���㣬���ȳ�ֵ��\r\n���ñ�CD��%lf", price);
				str1 = str1.Left(str1.Find('.') + 3);
				str2.Format(L"\r\n�����%lf", bal);
				str2 = str2.Left(str2.Find('.') + 3);
				str1 += str2;
				MessageBox(str1, L"��������CD", MB_OK | MB_ICONWARNING);
				return;
			}
			else if (nRet == 8)	 //�������CD�����㣬Υ���洢����
			{
				CString str1, str2;
				str1.Format(L"���㣬���ȳ�ֵ��\r\n����CD��%lf", price);
				str1 = str1.Left(str1.Find('.') + 3);
				str2.Format(L"\r\n�����%lf", bal);
				str2 = str2.Left(str2.Find('.') + 3);
				str1 += str2;
				MessageBox(str1, L"��������CD", MB_OK | MB_ICONWARNING);
				return;
			}
		}
		catch (_com_error e)
		{
			m_pConnection->RollbackTrans();
			CString temp1 = e.Description(), temp2 = e.ErrorMessage();
			AfxMessageBox(temp1 + L"\r\n" + temp2);	///��ʾ������Ϣ
			return;
		}

		//����һ����¼��Trade����
		CString strTime, strSql, sqlHasRecord, temp1, tradeid;
		CTime tm;
		tm = CTime::GetCurrentTime();
		strTime = tm.Format("%Y%m%d");
		m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
		sqlHasRecord.Format(L"select * from Trade where trade_id like '%s", strTime);
		temp1 = L"%' order by trade_date desc";
		sqlHasRecord += temp1;
		try
		{
			HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord,
				_variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
			if (SUCCEEDED(hr))
			{
				if (m_pRecordset->rsEOF && m_pRecordset->BOF) //��¼��Ϊ��
				{
					tradeid.Format(L"%s_%d", strTime, 1);
				}
				else
				{
					CString temp2;
					temp2 = m_pRecordset->GetCollect(L"trade_id");
					int down = temp2.Find('_');
					temp2 = temp2.Mid(down + 1);
					tradeid.Format(L"%s_%d", strTime, _ttoi(temp2) + 1);
				}
			}
			else
			{
				AfxMessageBox(L"Create instance of recordset failed!");
				return;
			}
			m_pRecordset->Close();

			strSql.Format(L"insert into Trade values('%s','%s','%s',%d,getdate())",
				tradeid, id, cdid, tradetype);
			m_pConnection->Execute(_bstr_t(strSql), 0, adCmdText);
		}
		catch (_com_error e)
		{
			CString temp1 = e.Description(), temp2 = e.ErrorMessage();
			AfxMessageBox(temp1 + L"\r\n" + temp2);	///��ʾ������Ϣ
			return;
		}
		CDialogEx::OnOK();
	}
	//CDialogEx::OnOK();
}


void CTrade::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	OnBnClickedOk();
	//CDialogEx::OnOK();
}
