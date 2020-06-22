// LookCD.cpp : 实现文件
//

#include "stdafx.h"
#include "CDStore.h"
#include "LookCD.h"
#include "afxdialogex.h"


// CLookCD 对话框

IMPLEMENT_DYNAMIC(CLookCD, CDialogEx)

CLookCD::CLookCD(_ConnectionPtr p, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOOKCD, pParent)
{
	m_pConnection = p;
}

CLookCD::~CLookCD()
{
}

void CLookCD::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CDNAME, m_cdname);
	DDX_Control(pDX, IDC_ARTIST, m_artistname);
	DDX_Control(pDX, IDC_GENRE, m_genre);
	DDX_Control(pDX, IDC_RESULTLIST, m_resultlist);
}


BEGIN_MESSAGE_MAP(CLookCD, CDialogEx)
	ON_BN_CLICKED(IDC_SEARCH, &CLookCD::OnBnClickedSearch)
END_MESSAGE_MAP()


// CLookCD 消息处理程序


void CLookCD::OnBnClickedSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	m_resultlist.ResetContent(); //清空上次查询的结果
	CString cdname, artist, genre, genre2;
	m_cdname.GetWindowTextW(cdname);
	m_artistname.GetWindowTextW(artist);
	int judge;
	m_genre.GetWindowTextW(genre);
	judge = m_genre.FindString(0, genre);
	if (judge >= 0)
	{
		m_genre.GetLBText(judge, genre2);
	}
	if (!genre.IsEmpty()&&genre!=genre2)
	{
		MessageBox(L"没有这个CD类别！", L"查看空闲CD", MB_OK | MB_ICONWARNING);
		return;
	}
	m_pRecordset.CreateInstance(__uuidof(Recordset)); //创建实例
	CString sqlHasRecord;
	if (!cdname.IsEmpty() && !artist.IsEmpty() && !genre.IsEmpty())
	{
		if (judge > 0)
		{
			sqlHasRecord.Format(L"select * from CusCDlist where [CD名]='%s' and [作者名]='%s' and [类别]='%s'",
				cdname, artist, genre);
		}
		else
		{
			sqlHasRecord.Format(L"select * from CusCDlist where [CD名]='%s' and [作者名]='%s' and [类别] "
				"is null", cdname, artist);
		}
	}
	else if (!cdname.IsEmpty() && !artist.IsEmpty() && genre.IsEmpty())
	{
		sqlHasRecord.Format(L"select * from CusCDlist where [CD名]='%s' and [作者名]='%s'", cdname, artist);
	}
	else if (!cdname.IsEmpty() && artist.IsEmpty() && !genre.IsEmpty())
	{
		if (judge > 0)
		{
			sqlHasRecord.Format(L"select * from CusCDlist where [CD名]='%s' and [类别]='%s'", cdname, genre);
		}
		else
		{
			sqlHasRecord.Format(L"select * from CusCDlist where [CD名]='%s' and [类别] is null", cdname);
		}
	}
	else if (!cdname.IsEmpty() && artist.IsEmpty() && genre.IsEmpty())
	{
		sqlHasRecord.Format(L"select * from CusCDlist where [CD名]='%s'", cdname);
	}
	else if (cdname.IsEmpty() && !artist.IsEmpty() && !genre.IsEmpty())
	{
		if (judge > 0)
		{
			sqlHasRecord.Format(L"select * from CusCDlist where [作者名]='%s' and [类别]='%s'", artist, genre);
		}
		else
		{
			sqlHasRecord.Format(L"select * from CusCDlist where [作者名]='%s' and [类别] is null", artist);
		}
	}
	else if (cdname.IsEmpty() && !artist.IsEmpty() && genre.IsEmpty())
	{
		sqlHasRecord.Format(L"select * from CusCDlist where [作者名]='%s'", artist);
	}
	else if (cdname.IsEmpty() && artist.IsEmpty() && !genre.IsEmpty())
	{
		if (judge > 0)
		{
			sqlHasRecord.Format(L"select * from CusCDlist where [类别]='%s'", genre);
		}
		else
		{
			sqlHasRecord.Format(L"select * from CusCDlist where [类别] is null");
		}
	}
	else if (cdname.IsEmpty() && artist.IsEmpty() && genre.IsEmpty())
	{
		sqlHasRecord.Format(L"select * from CusCDlist");
	}
	sqlHasRecord += L" order by [入库时间]";
	try
	{
		HRESULT hr = m_pRecordset->Open((_variant_t)sqlHasRecord, 
			_variant_t((IDispatch*)m_pConnection, true), 
			adOpenDynamic, adLockPessimistic, adCmdText);
		if (SUCCEEDED(hr))
		{
			if (m_pRecordset->rsEOF && m_pRecordset->BOF) //记录集为空，没有与任何记录匹配上
			{
				MessageBox(L"库存没有相应的空闲CD！", L"查看空闲CD", MB_OK | MB_ICONWARNING);
			}
			else
			{
				while (!m_pRecordset->rsEOF)//遍历返回的每一条记录
				{
					CString str, all;
					str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("CD号"));
					all = all + str + L"、";
					_variant_t vValue = m_pRecordset->GetCollect("CD名");
					if (vValue.vt != VT_NULL)
					{
						str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("CD名"));
					}
					else
					{
						str = L"未知";
					}
					all = all + str + L"、";
					vValue = m_pRecordset->GetCollect("作者名");
					if (vValue.vt != VT_NULL)
					{
						str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("作者名"));
					}
					else
					{
						str = L"匿名";
					}
					all = all + str + L"、";
					vValue = m_pRecordset->GetCollect("类别");
					if (vValue.vt != VT_NULL)
					{
						str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("类别"));
					}
					else
					{
						str = L"未知类别";
					}
					all = all + str + L"、";
					str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("售价"));
					all = all + str + L"、";
					str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("租用价格"));
					all = all + str + L"、";
					str = (LPCSTR)_bstr_t(m_pRecordset->GetCollect("租用次数"));
					all = all + str;
					m_resultlist.InsertString(m_resultlist.GetCount(), all);
					m_pRecordset->MoveNext();
				}
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


BOOL CLookCD::OnInitDialog()
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
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CLookCD::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	OnBnClickedSearch();
	//CDialogEx::OnOK();
}

