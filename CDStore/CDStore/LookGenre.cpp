// LookGenre.cpp : 实现文件
//

#include "stdafx.h"
#include "CDStore.h"
#include "LookGenre.h"
#include "afxdialogex.h"


// CLookGenre 对话框

IMPLEMENT_DYNAMIC(CLookGenre, CDialogEx)

CLookGenre::CLookGenre(_ConnectionPtr p, int id, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOOKGENRE, pParent)
{
	m_pConnection = p;
	adminid = id;
}

CLookGenre::~CLookGenre()
{
}

void CLookGenre::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GENRELIST, m_genrelist);
	DDX_Control(pDX, IDC_ADDGENRE, m_addgenre);
}


BEGIN_MESSAGE_MAP(CLookGenre, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLookGenre::OnBnClickedOk)
END_MESSAGE_MAP()


// CLookGenre 消息处理程序


void CLookGenre::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	m_addgenre.GetWindowTextW(str);
	if (str.IsEmpty())
	{
		MessageBox(L"类别名不能为空！", L"增加CD类别", MB_OK | MB_ICONWARNING);
	}
	else if (str == L"未知类别")
	{
		MessageBox(L"不合法的分类名！", L"增加CD类别", MB_OK | MB_ICONWARNING);
	}
	else
	{
		CString sqlHasRecord,strSql;
		int genreid;
		m_pRecordset.CreateInstance(__uuidof(Recordset)); //创建实例
		sqlHasRecord = L"select * from Genre order by genre_id desc";
		try
		{
			HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord,
				_variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
			if (SUCCEEDED(hr))
			{
				if (m_pRecordset->rsEOF && m_pRecordset->BOF) //记录集为空
				{
					genreid = 1;
				}
				else
				{
					CString temp;
					temp = m_pRecordset->GetCollect(L"genre_id");
					genreid = _ttoi(temp) + 1;
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
			AfxMessageBox(temp1 + L"\r\n" + temp2);	///显示错误信息
			return;
		}

		strSql.Format(L"insert into Genre values(%d,'%s')", genreid, str);
		try
		{
			m_pConnection->Execute(_bstr_t(strSql), 0, adCmdText);
		}
		catch (_com_error e)
		{
			MessageBox(e.Description(), L"增加CD类别", MB_OK | MB_ICONWARNING);
			return;
		}
		MessageBox(L"添加成功！", L"增加CD类别", MB_OK | MB_ICONINFORMATION);
		m_pRecordset.CreateInstance(__uuidof(Recordset)); //创建实例
		sqlHasRecord.Format(L"select * from Genre where genre_id=%d", genreid);
		try
		{
			HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord,
				_variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
			if (SUCCEEDED(hr))
			{
				if (!m_pRecordset->rsEOF) 
				{
					str = m_pRecordset->GetCollect("genre_name");
					m_genrelist.InsertString(m_genrelist.GetCount(), str);
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
			AfxMessageBox(temp1 + L"\r\n" + temp2);	///显示错误信息
			return;
		}
	}
	//CDialogEx::OnOK();
}


void CLookGenre::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnBnClickedOk();
	//CDialogEx::OnOK();
}

BOOL CLookGenre::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pRecordset.CreateInstance(__uuidof(Recordset)); //创建实例
	CString sqlHasRecord;
	sqlHasRecord = L"select * from Genre";
	try
	{
		HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, _variant_t((IDispatch*)m_pConnection, true), adOpenDynamic, adLockPessimistic, adCmdText);
		if (SUCCEEDED(hr))
		{
			while (!m_pRecordset->rsEOF)//遍历返回的每一条记录
			{
				CString str;
				_variant_t vValue = m_pRecordset->GetCollect("genre_name");
				if (vValue.vt != VT_NULL)
				{
					str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("genre_name"));
				}
				else
				{
					str = L"未知类别";
				}
				m_genrelist.InsertString(m_genrelist.GetCount(), str);
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
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
