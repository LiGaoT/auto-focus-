#pragma once
#include "afxdialogex.h"
#include "MFCHalconDlg.h"


// OPTIONS 对话框

class OPTIONS : public CDialogEx
{
	DECLARE_DYNAMIC(OPTIONS)

public:
	OPTIONS(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~OPTIONS();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPTIONS_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnEnChangeEdit1();
	CEdit m_Edit1;
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit5();
};
