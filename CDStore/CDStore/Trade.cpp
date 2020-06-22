// Trade.cpp : 实现文件
//

#include "stdafx.h"
#include "CDStore.h"
#include "Trade.h"
#include "afxdialogex.h"


// CTrade 对话框

IMPLEMENT_DYNAMIC(CTrade, CDialogEx)

CTrade::CTrade(_ConnectionPtr p, CString cusid, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TRADE, pParent)
	, m_cdidstr(_T(""))
{
	m_pConnection = p;
	id = cusid;
	m_pRecordset.CreateInstance(__uuidof(Recordset)); //创建实例
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
		AfxMessageBox(temp1 + L"\r\n" + temp2);	///显示错误信息
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


// CTrade 消息处理程序


BOOL CTrade::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	((CButton *)GetDlgItem(IDC_BUY))->SetCheck(TRUE);
	CString str1, str2;
	((CStatic*)GetDlgItem(IDC_DISPBALANCE))->GetWindowTextW(str1);
	str2.Format(L"%lf", balance);
	str2 = str2.Left(str2.Find('.') + 3);
	str1 += str2;
	((CStatic*)GetDlgItem(IDC_DISPBALANCE))->SetWindowTextW(str1);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CTrade::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cdid;
	m_cdid.GetWindowTextW(cdid);
	if (cdid.IsEmpty())
	{
		MessageBox(L"CD号不能为空！", L"购买、租用CD", MB_OK | MB_ICONWARNING);
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
			m_pCommand->Execute(NULL, NULL, adCmdStoredProc); //执行存储过程
			int nRet = m_pCommand->Parameters->GetItem("Return")->GetValue();
			double price = m_pCommand->Parameters->GetItem("trade_price")->GetValue();
			double bal = m_pCommand->Parameters->GetItem("trade_balance")->GetValue();
			m_pConnection->CommitTrans();
			if (nRet == -1)	 //输入库存中不存在的CD，违背存储过程
			{
				MessageBox(L"库存没有此CD！", L"购买、租用CD", MB_OK | MB_ICONWARNING);
				return;
			}
			else if (nRet == 0)	 //输入别人租用或购买的CD，违背存储过程
			{
				MessageBox(L"此CD已被其他顾客购买或者正在被租用！", L"购买、租用CD", MB_OK | MB_ICONWARNING);
				return;
			}
			else if (nRet == 1)	 //输入自己已购买的CD，违背存储过程
			{
				MessageBox(L"你已购买此CD！", L"购买、租用CD", MB_OK | MB_ICONWARNING);
				return;
			}
			else if (nRet == 2)	 //购买已租用的CD成功
			{
				CString str;
				str.Format(L"购买成功！\r\n你的余额：%lf", bal);
				str = str.Left(str.Find('.') + 3);
				MessageBox(str, L"购买、租用CD", MB_OK | MB_ICONINFORMATION);
				return;
			}
			else if (nRet == 3)	 //购买已租用的CD，但是余额不足，违背存储过程
			{
				CString str1, str2;
				str1.Format(L"余额不足，请先充值！\r\n购买本CD金额：%lf", price);
				str1 = str1.Left(str1.Find('.') + 3);
				str2.Format(L"\r\n你的余额：%lf", bal);
				str2 = str2.Left(str2.Find('.') + 3);
				str1 += str2;
				MessageBox(str1, L"购买、租用CD", MB_OK | MB_ICONWARNING);
				return;
			}
			else if (nRet == 4)	//重复租用自己正在租用的CD，违背存储过程
			{
				MessageBox(L"你已正在租用此CD！不能重复租用！", L"购买、租用CD", MB_OK | MB_ICONWARNING);
				return;
			}
			else if (nRet == 5)	 //租用空闲CD，余额充足，更新成功
			{
				CString str;
				str.Format(L"租用成功！\r\n你的余额：%lf", bal);
				str = str.Left(str.Find('.') + 3);
				MessageBox(str, L"购买、租用CD", MB_OK | MB_ICONINFORMATION);
			}
			else if (nRet == 6)	 //购买空闲CD，余额充足，更新成功
			{
				CString str;
				str.Format(L"购买成功！\r\n你的余额：%lf", bal);
				str = str.Left(str.Find('.') + 3);
				MessageBox(str, L"购买、租用CD", MB_OK | MB_ICONINFORMATION);
			}
			else if (nRet == 7)	 //租用空闲CD，余额不足，违背存储过程
			{
				CString str1, str2;
				str1.Format(L"余额不足，请先充值！\r\n租用本CD金额：%lf", price);
				str1 = str1.Left(str1.Find('.') + 3);
				str2.Format(L"\r\n你的余额：%lf", bal);
				str2 = str2.Left(str2.Find('.') + 3);
				str1 += str2;
				MessageBox(str1, L"购买、租用CD", MB_OK | MB_ICONWARNING);
				return;
			}
			else if (nRet == 8)	 //购买空闲CD，余额不足，违背存储过程
			{
				CString str1, str2;
				str1.Format(L"余额不足，请先充值！\r\n购买本CD金额：%lf", price);
				str1 = str1.Left(str1.Find('.') + 3);
				str2.Format(L"\r\n你的余额：%lf", bal);
				str2 = str2.Left(str2.Find('.') + 3);
				str1 += str2;
				MessageBox(str1, L"购买、租用CD", MB_OK | MB_ICONWARNING);
				return;
			}
		}
		catch (_com_error e)
		{
			m_pConnection->RollbackTrans();
			CString temp1 = e.Description(), temp2 = e.ErrorMessage();
			AfxMessageBox(temp1 + L"\r\n" + temp2);	///显示错误信息
			return;
		}

		//插入一条记录到Trade表中
		CString strTime, strSql, sqlHasRecord, temp1, tradeid;
		CTime tm;
		tm = CTime::GetCurrentTime();
		strTime = tm.Format("%Y%m%d");
		m_pRecordset.CreateInstance(__uuidof(Recordset)); //创建实例
		sqlHasRecord.Format(L"select * from Trade where trade_id like '%s", strTime);
		temp1 = L"%' order by trade_date desc";
		sqlHasRecord += temp1;
		try
		{
			HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord,
				_variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
			if (SUCCEEDED(hr))
			{
				if (m_pRecordset->rsEOF && m_pRecordset->BOF) //记录集为空
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
			AfxMessageBox(temp1 + L"\r\n" + temp2);	///显示错误信息
			return;
		}
		CDialogEx::OnOK();
	}
	//CDialogEx::OnOK();
}


void CTrade::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnBnClickedOk();
	//CDialogEx::OnOK();
}
