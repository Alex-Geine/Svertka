
// SvertkaDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Svertka.h"
#include "SvertkaDlg.h"
#include "afxdialogex.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CSvertkaDlg



CSvertkaDlg::CSvertkaDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SVERTKA_DIALOG, pParent)
	, N(50)
	, fd(1)
	, Ah(1)
	, Gh(3)
	, A1(3)
	, A2(4)
	, A3(5)
	, G1(4)
	, G2(2)
	, G3(3)
	, niu1(5)
	, niu2(30)
	, niu3(40)
	, TAU(1.e-6f)
	, h(0.0001)
	, e(0)
	, noise(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSvertkaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, N);
	DDX_Text(pDX, IDC_EDIT2, fd);
	DDX_Text(pDX, IDC_EDIT3, Ah);
	DDX_Text(pDX, IDC_EDIT4, Gh);
	DDX_Text(pDX, IDC_EDIT5, A1);
	DDX_Text(pDX, IDC_EDIT8, A2);
	DDX_Text(pDX, IDC_EDIT11, A3);
	DDX_Text(pDX, IDC_EDIT6, G1);
	DDX_Text(pDX, IDC_EDIT9, G2);
	DDX_Text(pDX, IDC_EDIT12, G3);
	DDX_Text(pDX, IDC_EDIT7, niu1);
	DDX_Text(pDX, IDC_EDIT10, niu2);
	DDX_Text(pDX, IDC_EDIT13, niu3);
	DDX_Text(pDX, IDC_EDIT14, TAU);
	DDX_Text(pDX, IDC_EDIT15, h);
	DDX_Text(pDX, IDC_EDIT16, e);
	DDX_Text(pDX, IDC_EDIT17, noise);
}

BEGIN_MESSAGE_MAP(CSvertkaDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_QUERYDRAGICON()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_MOUSEMOVE()
	ON_WM_MBUTTONDOWN()
	ON_BN_CLICKED(IDOK, &CSvertkaDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDOK2, &CSvertkaDlg::OnBnClickedOk2)
	ON_BN_CLICKED(IDOK3, &CSvertkaDlg::OnBnClickedOk3)
	ON_BN_CLICKED(IDOK4, &CSvertkaDlg::OnBnClickedOk4)
	ON_EN_CHANGE(IDC_EDIT14, &CSvertkaDlg::OnEnChangeEdit14)
	ON_BN_CLICKED(IDOK5, &CSvertkaDlg::OnBnClickedOk5)
END_MESSAGE_MAP()


// Обработчики сообщений CSvertkaDlg

BOOL CSvertkaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	GR1.Create(GetDlgItem(IDC_Graph1)->GetSafeHwnd());
	GR2.Create(GetDlgItem(IDC_Graph2)->GetSafeHwnd());
	GR3.Create(GetDlgItem(IDC_Graph3)->GetSafeHwnd());
	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	//кнопки
	btnCount = reinterpret_cast<CButton*>(GetDlgItem(IDOK));
	btnPaint = reinterpret_cast<CButton*>(GetDlgItem(IDOK2));
	btnPause = reinterpret_cast<CButton*>(GetDlgItem(IDOK4));
	btnContinue = reinterpret_cast<CButton*>(GetDlgItem(IDOK5));
	
	
	

	btnCount->EnableWindow(FALSE);
	btnPause->EnableWindow(FALSE);
	btnContinue->EnableWindow(FALSE);
	
	//поля
	edtFirstName1 = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT1));
	edtFirstName2 = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT2));
	edtFirstName3 = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT3));
	edtFirstName4 = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT4));
	edtFirstName5 = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT5));
	edtFirstName6 = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT6));
	edtFirstName7 = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT7));
	edtFirstName8 = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT8));
	edtFirstName9 = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT9));
	edtFirstName10 = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT10));
	edtFirstName11 = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT11));
	edtFirstName12 = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT12));
	edtFirstName13 = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT13));
	edtFirstName14 = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT14));
	edtFirstName15 = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT15));
	edtFirstName16 = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT16));
	edtFirstName17 = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT17));
	
	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CSvertkaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CSvertkaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{		
		CDialogEx::OnPaint();
		if (!curSignal.keys.empty()) {
			GR1.ReDraw();
			GR2.ReDraw();
			GR3.ReDraw();
		}
		
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CSvertkaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//меняет ыидимость полей
void CSvertkaDlg::SetEditFormat(BOOL form) {
	
	edtFirstName1->SetReadOnly(form);
	edtFirstName2->SetReadOnly(form);
	edtFirstName3->SetReadOnly(form);
	edtFirstName4->SetReadOnly(form);
	edtFirstName5->SetReadOnly(form);
	edtFirstName6->SetReadOnly(form);
	edtFirstName7->SetReadOnly(form);
	edtFirstName8->SetReadOnly(form);
	edtFirstName9->SetReadOnly(form);
	edtFirstName10->SetReadOnly(form);
	edtFirstName11->SetReadOnly(form);
	edtFirstName12->SetReadOnly(form);
	edtFirstName13->SetReadOnly(form);
	edtFirstName14->SetReadOnly(form);
	edtFirstName15->SetReadOnly(form);
	edtFirstName16->SetReadOnly(form);
	edtFirstName17->SetReadOnly(form);
}

//посчитать
void CSvertkaDlg::OnBnClickedOk(){
		SetEditFormat(TRUE);
		btnCount->EnableWindow(FALSE);
		btnPaint->EnableWindow(FALSE);
		btnPause->EnableWindow(TRUE);
		
		curSignal.MHJ(curSignal.N, curSignal.liambda, mPause, TAU, h, e);

		while (PeekMessage(&mPause, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&mPause);
			DispatchMessage(&mPause);
			if (mPause.lParam == LPARAM("COMPLETE"))
			{
				btnCount->EnableWindow(FALSE);
				btnPause->EnableWindow(FALSE);
				btnContinue->EnableWindow(FALSE);
				btnPaint->EnableWindow(TRUE);
				ready = true;
				SetEditFormat(FALSE);
				MessageBoxA(NULL,(LPCSTR)"Операция выполнена!", (LPCSTR)"Complete", MB_OK);
				
			}
		}
		if(ready == false)
			e = curSignal.GetE();
		UpdateData(FALSE);		
}


//Отрисовать
void CSvertkaDlg::OnBnClickedOk2()
{
	btnCount->EnableWindow(TRUE);
	btnPause->EnableWindow(FALSE);
	btnContinue->EnableWindow(FALSE);
	ready = false;
	
	UpdateData(TRUE);

	e = 0.;

	curSignal.ClearSignal();
	curSignal.FillSignal(N, fd, Ah, Gh,noise, A1, A2, A3, G1, G2, G3, niu1, niu2, niu3, &GR1);

	GR1.DrawW();
	GR2.DrawW();
	GR3.DrawW();
		
	//исходный сигнал
	GR1.DrawOne(curSignal.X, *min_element(curSignal.keys.begin(), curSignal.keys.end()), *max_element(curSignal.keys.begin(), curSignal.keys.end()),
		*min_element(curSignal.X.begin(), curSignal.X.end()), *max_element(curSignal.X.begin(), curSignal.X.end()), 't', 'A', curSignal.keys);
	//импульсная характеристика
	GR2.DrawOne(curSignal.H, *min_element(curSignal.keys.begin(), curSignal.keys.end()), *max_element(curSignal.keys.begin(), curSignal.keys.end()),
		*min_element(curSignal.H.begin(), curSignal.H.end()), *max_element(curSignal.H.begin(), curSignal.H.end()), 't', 'A', curSignal.keys);
	//На выходе
	GR3.DrawOne(curSignal.Y, *min_element(curSignal.keys.begin(), curSignal.keys.end()), *max_element(curSignal.keys.begin(), curSignal.keys.end()),
		*min_element(curSignal.Y.begin(), curSignal.Y.end()), *max_element(curSignal.Y.begin(), curSignal.Y.end()), 't', 'A', curSignal.keys);
	
	UpdateData(FALSE);
}

//закртытие 
void CSvertkaDlg::OnBnClickedOk3()
{
	exit(0);
	
}


//пауза
void CSvertkaDlg::OnBnClickedOk4()
{
	SetEditFormat(FALSE);
	btnContinue->EnableWindow(TRUE);
	btnPaint->EnableWindow(TRUE);
	PostThreadMessageA(GetCurrentThreadId(), 12345, WPARAM("PAUSE"), LPARAM("PAUSE"));
}


void CSvertkaDlg::OnEnChangeEdit14()
{
	
}

//продолжить
void CSvertkaDlg::OnBnClickedOk5()
{
	OnBnClickedOk();
}
