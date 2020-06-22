// AddStock.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CDStore.h"
#include "AddStock.h"
#include "afxdialogex.h"


// CAddStock �Ի���

IMPLEMENT_DYNAMIC(CAddStock, CDialogEx)

CAddStock::CAddStock(_ConnectionPtr p, int id, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ADDSTOCK, pParent)
	, m_purpricenum(0)
	, m_purnumdata(0)
	, m_artiststr(_T(""))
	, m_cdnamestr(_T(""))
	, m_supplieriddata(0)
{
	m_pConnection = p;
	m_adminid = id;
}

CAddStock::~CAddStock()
{
}

void CAddStock::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PURPRICE, m_purprice);
	DDX_Control(pDX, IDC_SUPPLIERID, m_supplierid);
	DDX_Control(pDX, IDC_PURCDNAME, m_cdname);
	DDX_Control(pDX, IDC_PURARTIST, m_artist);
	DDX_Control(pDX, IDC_PURGENRELIST, m_genre);
	DDX_Control(pDX, IDC_STOCKNUM, m_purnum);
	DDX_Text(pDX, IDC_PURPRICE, m_purpricenum);
	DDX_Text(pDX, IDC_STOCKNUM, m_purnumdata);
	DDX_Text(pDX, IDC_PURARTIST, m_artiststr);
	DDX_Text(pDX, IDC_PURCDNAME, m_cdnamestr);
	DDX_Text(pDX, IDC_SUPPLIERID, m_supplieriddata);
}


BEGIN_MESSAGE_MAP(CAddStock, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddStock::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddStock ��Ϣ�������


void CAddStock::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString supplierid, cdname, artist, genre, purnum, purprice;
	m_supplierid.GetWindowTextW(supplierid);
	m_cdname.GetWindowTextW(cdname);
	m_artist.GetWindowTextW(artist);
	m_genre.GetWindowTextW(genre);
	m_purnum.GetWindowTextW(purnum);
	m_purprice.GetWindowTextW(purprice);
	if (supplierid.IsEmpty())
	{
		MessageBox(L"��Ӧ��ID����Ϊ�գ�", L"����", MB_OK | MB_ICONWARNING);
	}
	else if (purnum.IsEmpty())
	{
		MessageBox(L"������Ŀ����Ϊ�գ�", L"����", MB_OK | MB_ICONWARNING);
	}
	else if (purprice.IsEmpty())
	{
		MessageBox(L"���۲���Ϊ�գ�", L"����", MB_OK | MB_ICONWARNING);
	}
	else if (_ttoi(purnum) <= 0)
	{
		MessageBox(L"������Ŀ�������0��", L"����", MB_OK | MB_ICONWARNING);
	}
	else
	{
		UpdateData(true);
		CString strTime, stockid, strSql, sqlHasRecord, temp1;
		CString unknown = L"δ֪���";
		CTime tm;
		int offset;
		tm = CTime::GetCurrentTime();
		strTime = tm.Format("%Y%m%d");
		m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
		sqlHasRecord.Format(L"select * from Stock where stock_id like '%s", strTime);
		temp1 = L"%' order by pur_date desc";
		sqlHasRecord += temp1;
		HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord,
			_variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
		if (SUCCEEDED(hr))
		{
			//����stock_id
			if (m_pRecordset->rsEOF && m_pRecordset->BOF) //��¼��Ϊ��
			{
				offset = 1;
			}
			else
			{
				CString temp2;
				temp2 = m_pRecordset->GetCollect(L"stock_id");
				int down = temp2.Find('_');
				temp2 = temp2.Mid(down + 1);
				offset = _ttoi(temp2) + 1;
			}
		}
		else
		{
			AfxMessageBox(L"Create instance of recordset failed!");
			return;
		}
		m_pRecordset->Close();
		for (int i = offset; i < offset + m_purnumdata; i++)
		{
			stockid.Format(L"%s_%d", strTime, i);
			//ִ�д洢����
			if (!m_cdnamestr.IsEmpty() && !m_artiststr.IsEmpty() && !(genre==unknown)) 
			{
				strSql.Format(L"exec sp_AddStock %d,%d,'%s',%lf,'%s','%s','%s'", m_adminid, 
					m_supplieriddata, stockid, m_purpricenum, genre, m_cdnamestr, m_artiststr);
			}
			else if (!m_cdnamestr.IsEmpty() && !m_artiststr.IsEmpty() && genre==unknown)
			{
				strSql.Format(L"exec sp_AddStock %d,%d,'%s',%lf,null,'%s','%s'", m_adminid,
					m_supplieriddata, stockid, m_purpricenum, m_cdnamestr, m_artiststr);
			}
			else if (!m_cdnamestr.IsEmpty() && m_artiststr.IsEmpty() && !(genre==unknown))
			{
				strSql.Format(L"exec sp_AddStock %d,%d,'%s',%lf,'%s','%s',null", m_adminid,
					m_supplieriddata, stockid, m_purpricenum, genre, m_cdnamestr);
			}
			else if (!m_cdnamestr.IsEmpty() && m_artiststr.IsEmpty() && genre==unknown)
			{
				strSql.Format(L"exec sp_AddStock %d,%d,'%s',%lf,null,'%s',null", m_adminid,
					m_supplieriddata, stockid, m_purpricenum, m_cdnamestr);
			}
			else if (m_cdnamestr.IsEmpty() && !m_artiststr.IsEmpty() && !(genre==unknown))
			{
				strSql.Format(L"exec sp_AddStock %d,%d,'%s',%lf,'%s',null,'%s'", m_adminid,
					m_supplieriddata, stockid, m_purpricenum, genre, m_artiststr);
			}
			else if (m_cdnamestr.IsEmpty() && !m_artiststr.IsEmpty() && genre==unknown)
			{
				strSql.Format(L"exec sp_AddStock %d,%d,'%s',%lf,null,null,'%s'", m_adminid,
					m_supplieriddata, stockid, m_purpricenum, m_artiststr);
			}
			else if (m_cdnamestr.IsEmpty() && m_artiststr.IsEmpty() && !(genre == unknown))
			{
				strSql.Format(L"exec sp_AddStock %d,%d,'%s',%lf,'%s',null,null", m_adminid,
					m_supplieriddata, stockid, m_purpricenum, genre);
			}
			else if (m_cdnamestr.IsEmpty() && m_artiststr.IsEmpty() && genre == unknown)
			{
				strSql.Format(L"exec sp_AddStock %d,%d,'%s',%lf,null,null,null", m_adminid,
					m_supplieriddata, stockid, m_purpricenum);
			}
			try
			{
				m_pConnection->Execute(_bstr_t(strSql), 0, adCmdText);
			}
			catch (_com_error e)
			{
				MessageBox(e.Description(), L"����", MB_OK | MB_ICONWARNING);
				return;
			}
		}
		MessageBox(L"�����ɹ���", L"����", MB_OK | MB_ICONINFORMATION);
		CDialogEx::OnOK();
	}
	//CDialogEx::OnOK();
}


BOOL CAddStock::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_supplierid.SetWindowTextW(L"");
	m_purnum.SetWindowTextW(L"");
	m_purprice.SetWindowTextW(L"");

	m_pRecordset.CreateInstance(__uuidof(Recordset)); //����ʵ��
	CString sqlHasRecord;
	sqlHasRecord = L"select * from Genre";
	try
	{
		HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
		if (SUCCEEDED(hr))
		{
			while (!m_pRecordset->rsEOF)//�������ص�ÿһ����¼
			{
				CString str;
				_variant_t vValue = m_pRecordset->GetCollect("genre_name");
				if (vValue.vt != VT_NULL)
				{
					str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("genre_name"));
				}
				else
				{
					str = L"δ֪���";
				}
				m_genre.InsertString(m_genre.GetCount(), str);
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
		AfxMessageBox(temp1 + L"\r\n" + temp2);
		return FALSE;
	}
	m_genre.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CAddStock::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	OnBnClickedOk();
	//CDialogEx::OnOK();
}
