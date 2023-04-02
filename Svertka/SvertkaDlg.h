
// SvertkaDlg.h: файл заголовка
//

#pragma once
#include "Drawer.h"
#include <vector>
#include "Signal.h"

// Диалоговое окно CSvertkaDlg
class CSvertkaDlg : public CDialogEx
{
// Создание
public:
	CSvertkaDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SVERTKA_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	double N;
	double fd;
	double Ah;
	double Gh;
	double A1;
	double A2;
	double A3;
	double G1;
	double G2;
	double G3;
	double niu1;
	double niu2;
	double niu3;

	MSG mPause;
	Signal curSignal;
	Drawer
		GR1,
		GR2,
		GR3;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOk2();
	afx_msg void OnBnClickedOk3();
	afx_msg void OnBnClickedOk4();
	afx_msg void OnEnChangeEdit14();
	float TAU;
	float h;
	afx_msg void OnBnClickedOk5();
	float e;
	CButton* btnCount;
	CButton* btnPaint;
	CButton* btnPause;
	CButton* btnContinue;

	CEdit* edtFirstName1;
	CEdit* edtFirstName2;
	CEdit* edtFirstName3;
	CEdit* edtFirstName4;
	CEdit* edtFirstName5;
	CEdit* edtFirstName6;
	CEdit* edtFirstName7;
	CEdit* edtFirstName8;
	CEdit* edtFirstName9;
	CEdit* edtFirstName10;
	CEdit* edtFirstName11;
	CEdit* edtFirstName12;
	CEdit* edtFirstName13;
	CEdit* edtFirstName14;
	CEdit* edtFirstName15;
	CEdit* edtFirstName16;
	CEdit* edtFirstName17;
	void SetEditFormat(BOOL);
	bool ready;
	double noise;
};
