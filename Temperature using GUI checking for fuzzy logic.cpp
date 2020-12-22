
// BrakeSystemDlg.cpp : implementation file
//
#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "BrakeSystem.h"
#include "BrakeSystemDlg.h"
#include "afxdialogex.h"
#include "CSeries.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBrakeSystemDlg dialog



CBrakeSystemDlg::CBrakeSystemDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CBrakeSystemDlg::IDD, pParent)
, m_dist(0)
//, m_chart1(0)
//, m_chart2(0)
, m_speed(0)
//, m_chart4(0)
, m_min(_T(""))
, m_jarak(_T(""))
, m_kecepatan(_T(""))
//, m_chart3(0)
, m_max(_T(""))
, m_outdata()
, m_kondisi(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBrakeSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_dist);
	DDX_Control(pDX, IDC_TCHART1, m_chart1);
	DDX_Control(pDX, IDC_TCHART2, m_chart2);
	DDX_Text(pDX, IDC_EDIT3, m_speed);
	DDX_Control(pDX, IDC_TCHART4, m_chart4);
	DDX_Text(pDX, IDC_EDIT1, m_min);
	DDX_Text(pDX, IDC_EDIT4, m_jarak);
	DDX_Text(pDX, IDC_EDIT5, m_kecepatan);
	DDX_Control(pDX, IDC_TCHART3, m_chart3);
	DDX_Text(pDX, IDC_EDIT7, m_max);
	DDX_Text(pDX, IDC_EDIT8, m_outdata);
	DDX_Text(pDX, IDC_EDIT9, m_kondisi);
}

BEGIN_MESSAGE_MAP(CBrakeSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CBrakeSystemDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CBrakeSystemDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CBrakeSystemDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CBrakeSystemDlg message handlers

BOOL CBrakeSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	distance();
	speed();
	brake();

	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CBrakeSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBrakeSystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBrakeSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
double a = 20, b = 40, c = 60, d = 80, jarak = 20;
double nilai;
double dataout[152];
double maxfanon_1 = 0, maxfanof = 0, maxfanon_2 = 0;
void CBrakeSystemDlg::datarulecl()
{
	maxfanon_1 = 0; maxfanof = 0; maxfanon_2 = 0;
}
void CBrakeSystemDlg::ruletable()//menginputkan gambar tabel pada mfc
{
	CString pngPath = L"ruletablesuhu.png";
	CImage pngImage;
	CBitmap pngBmp;
	CDC bmDC;
	CBitmap *pOldbmp;
	BITMAP  bi;
	UINT xPos = 800, yPos = 20;
	//UINT bmWidth = 200, bmHeight = 200;

	CClientDC dc(this);

	pngImage.Load(pngPath);
	// new code

	pngBmp.Attach(pngImage.Detach());

	bmDC.CreateCompatibleDC(&dc);

	pOldbmp = bmDC.SelectObject(&pngBmp);
	pngBmp.GetBitmap(&bi);
	dc.BitBlt(xPos, yPos, bi.bmWidth, bi.bmHeight, &bmDC, 0, 0, SRCCOPY);
	bmDC.SelectObject(pOldbmp);
}
double CBrakeSystemDlg::Member1(double x, int kondisi)//Series 1 untuk tiap grafik teechart
{
	double fuz = 0;
	if (kondisi == 1)//suhu(dingin)
	{
		if (x <= 25)
		{
			fuz = 1;
		}
		else if (x > 25 && x <= 30)
		{
			fuz = 1 - ((x - 25) / 5);
		}
		else fuz = 0;

	}
	else if (kondisi == 2)//kelembaban(kering)
	{
		if (x <= 30)
		{
			fuz = 1;
		}
		else if (x > 30 && x <= 45)
		{
			fuz = 1 - ((x - 30) / 15);
		}
		else
			fuz = 0;
	}
	else if (kondisi == 3)//kondisi kipas mati
	{
		if (x <= 30)
		{
			fuz = 1;
		}
		else if (x > 30 && x <= 50)
		{
			fuz = 1 - ((x - 30) / 20);
		}
		else
		{
			fuz = 0;
		}
	}
	return fuz;
}
double CBrakeSystemDlg::Member2(double x, int kondisi)//series2 untuk tiap grafik teechart
{
	double fuz = 0;
	if (kondisi == 1){//suhu hangat
		if (x <= 25){
			fuz = 0;
		}
		else if (x > 25 && x <= 30){
			fuz = 1 - ((30 - x) / 5);
		}
		else if (x > 30 && x <= 35){
			fuz = 1 - ((x - 30) / 5);
		}
		else fuz = 0;
	}
	else if (kondisi == 2)//kelembaban(hangat)
	{
		if (x <= 30){
			fuz = 0;
		}
		else if (x > 30 && x <= 45){
			fuz = 1 - ((45 - x) / 15);
		}
		else if (x > 45 && x <= 70){
			fuz = 1 - ((x - 45) / 25);
		}
		else fuz = 0;
	}
	else if (kondisi == 3)//1 kipas nyala
	{
		if (x <= 45){
			fuz = 0;
		}
		else if (x > 45 && x <= 65){
			fuz = 1 - ((65 - x) / 20);
		}
		else if (x > 65 && x <= 80){
			fuz = 1 - ((x - 65) / 15);
		}
		else fuz = 0;
	}
	return fuz;

}
double CBrakeSystemDlg::Member3(double x, int kondisi)//series3 untuk tiap grafik teechart
{
	double fuz = 0;
	if (kondisi == 1){//suhu(panas)
		if (x <= 30){
			fuz = 0;
		}
		else if (x > 30 && x <= 35){
			fuz = 1 - ((35 - x) / 5);
		}
		else fuz = 1;
	}
	else if (kondisi == 2)//kelembaban(basah)
	{
		if (x <= 45){
			fuz = 0;
		}
		else if (x > 45 && x <= 70){
			fuz = 1 - ((70 - x) / 25);
		}
		else fuz = 1;
	}
	else if (kondisi == 3)//2 kipas nyala
	{
		if (x <= 70){
			fuz = 0;
		}
		else if (x > 70 && x <= 90){
			fuz = 1 - ((90 - x) / 20);
		}
		else fuz = 1;
	}
	return fuz;
}
void CBrakeSystemDlg::Clearpoint()//cleardata dotpoint
{
	CSeries(m_chart1.get_Series(3)).Clear();
	CSeries(m_chart1.get_Series(4)).Clear();
	CSeries(m_chart1.get_Series(5)).Clear();

	CSeries(m_chart2.get_Series(3)).Clear();
	CSeries(m_chart2.get_Series(4)).Clear();
	CSeries(m_chart2.get_Series(5)).Clear();
}
double fanoff, fan1on, brakes;
void CBrakeSystemDlg::brake()//menampilkan trapezoid out brake
{
	for (int x = 0; x <= 120; x++)
	{
		fanoff = Member1(x, 3);
		fan1on = Member2(x, 3);
		brakes = Member3(x, 3);
		((CSeries)(m_chart3.get_Series(0))).AddXY(x, fanoff, NULL, NULL);
		((CSeries)(m_chart3.get_Series(1))).AddXY(x, fan1on, NULL, NULL);
		((CSeries)(m_chart3.get_Series(2))).AddXY(x, brakes, NULL, NULL);
	}
}
double dingin, hangat, panas;
void CBrakeSystemDlg::distance()//menampilkan trapezoid suhu
{
	for (int x = 0; x <= 100; x++)
	{
		dingin = Member1(x, 1);
		hangat = Member2(x, 1);
		panas = Member3(x, 1);
		((CSeries)(m_chart1.get_Series(0))).AddXY(x, dingin, NULL, NULL);
		((CSeries)(m_chart1.get_Series(1))).AddXY(x, hangat, NULL, NULL);
		((CSeries)(m_chart1.get_Series(2))).AddXY(x, panas, NULL, NULL);
	}
}
double kering, sedang, basah;
void CBrakeSystemDlg::speed()//menampilkan trapezoid kelembapan
{
	for (int x = 0; x <= 120; x++)
	{
		kering = Member1(x, 2);
		sedang = Member2(x, 2);
		basah = Member3(x, 2);
		((CSeries)(m_chart2.get_Series(0))).AddXY(x, kering, NULL, NULL);
		((CSeries)(m_chart2.get_Series(1))).AddXY(x, sedang, NULL, NULL);
		((CSeries)(m_chart2.get_Series(2))).AddXY(x, basah, NULL, NULL);
	}
}
double minnilai[4][4];
double temper[3], kelembapan[3];
double fanon_1[3], fanof[3], fanon_2[4];

void CBrakeSystemDlg::OnBnClickedButton2()//RULE TABLE
{
	CSeries(m_chart4.get_Series(0)).Clear();
	datarulecl();
	UpdateData(TRUE);
	

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			minnilai[i][j] = min(kelembapan[i], temper[j]);
		}
	}
	char str[400], str1[100];
	sprintf_s(str, "[1] kering-dingin = %1f\r\n [4] kering-hangat = %1f\r\n [7] kering-panas = %1f\r\n [2] sedang-dingin = %1f\r\n [5] sedang-hangat = %1f\r\n [8] sedang-panas = %1f\r\n [3] basah-dingin = %1f\r\n [6] basah-hangat = %1f\r\n [9] basah-panas= %1f\r\n", minnilai[0][0], minnilai[0][1], minnilai[0][2], minnilai[1][0], minnilai[1][1], minnilai[1][2], minnilai[2][0], minnilai[2][1], minnilai[2][2]);
	m_min = str;
	fanof[0] = minnilai[0][0]; fanon_1[0] = minnilai[0][1]; fanon_2[0] = minnilai[0][2];
	fanof[1] = minnilai[1][0]; fanon_1[1] = minnilai[1][1]; fanon_2[1] = minnilai[1][2];
	fanof[2] = minnilai[2][0]; fanon_2[3] = minnilai[2][1]; fanon_2[2] = minnilai[2][2];
	for (int a = 0; a < 3; a++)
	{
		maxfanof = max(fanof[a], maxfanof);
		maxfanon_1 = max(fanon_1[a], maxfanon_1);
	}
	for (int a = 0; a < 4; a++)
	{
		maxfanon_2 = max(fanon_2[a], maxfanon_2);
	}
	sprintf_s(str1, "kipas mati= %1f\r\n 1_kipas_menyala = %1f\r\n 2_kipas_menyala=%1f\r\n", maxfanof, maxfanon_1, maxfanon_2);
	m_max = str1;

	UpdateData(FALSE);
}


void CBrakeSystemDlg::OnBnClickedButton1()//FUZZIFIKASI
{
	// TODO: Add your control notification handler code here
	Clearpoint();
	ruletable();
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	temper[0] = Member1(m_dist, 1);
	temper[1] = Member2(m_dist, 1);
	temper[2] = Member3(m_dist, 1);
	kelembapan[0] = Member1(m_speed, 2);
	kelembapan[1] = Member2(m_speed, 2);
	kelembapan[2] = Member3(m_speed, 2);
	((CSeries)(m_chart1.get_Series(3))).AddXY(m_dist, temper[0], NULL, NULL);
	((CSeries)(m_chart1.get_Series(4))).AddXY(m_dist, temper[1], NULL, NULL);
	((CSeries)(m_chart1.get_Series(5))).AddXY(m_dist, temper[2], NULL, NULL);

	((CSeries)(m_chart2.get_Series(3))).AddXY(m_speed, kelembapan[0], NULL, NULL);
	((CSeries)(m_chart2.get_Series(4))).AddXY(m_speed, kelembapan[1], NULL, NULL);
	((CSeries)(m_chart2.get_Series(5))).AddXY(m_speed, kelembapan[2], NULL, NULL);
	char datalembab[100], datasuhu[100];
	sprintf_s(datalembab, "Kering= %1f\r\n sedang=%1f\r\n Basah= %1f\r\n", kelembapan[0], kelembapan[1], kelembapan[2]);
	sprintf_s(datasuhu, "Dingin= %1f\r\n Hangat=%1f\r\n Panas= %1f\r\n", temper[0], temper[1], temper[2]);
	m_kecepatan = datalembab;
	m_jarak = datasuhu;
	UpdateData(FALSE);
}


void CBrakeSystemDlg::OnBnClickedButton3()//defuzzifikasi
{
	// TODO: Add your control notification handler code here
	CSeries(m_chart3.get_Series(3)).Clear();
	CSeries(m_chart3.get_Series(4)).Clear();
	CSeries(m_chart3.get_Series(5)).Clear();
	double nout1 = 0, nout2 = 0, nout = 0, ttk1, ttk2, ttk3, tt1, tt2, tt3;
	UpdateData(TRUE);
	char dataku[100];
	double data1, data2, data3;
	for (int y = 0; y <= 120; y++)
	{
		if (Member1(y, 3) > maxfanof)
		{
			data1 = maxfanof;
		}
		else
			data1 = Member1(y, 3);
		if (Member2(y, 3) > maxfanon_1)
		{
			data2 = maxfanon_1;
		}
		else
			data2 = Member2(y, 3);
		if (Member3(y, 3) > maxfanon_2)
		{
			data3 = maxfanon_2;
		}
		else
			data3 = Member3(y, 3);
		dataout[y] = max(data1, max(data2, data3));
		CSeries(m_chart4.get_Series(0)).AddXY(y, dataout[y], NULL, NULL);
	}
	for (int x = 0; x <= 120; x++)
	{
		nout1 += (dataout[x] * x);
		nout2 += (dataout[x]);
	}
	nout = nout1 / nout2;
	m_outdata = nout;
	tt1 = Member1(m_outdata, 3);
	tt2 = Member2(m_outdata, 3);
	tt3 = Member3(m_outdata, 3);
	((CSeries)(m_chart3.get_Series(3))).AddXY(nout, tt1, NULL, NULL);
	((CSeries)(m_chart3.get_Series(4))).AddXY(nout, tt2, NULL, NULL);
	((CSeries)(m_chart3.get_Series(5))).AddXY(nout, tt3, NULL, NULL);
	ttk1 = Member1(nout, 3);
	ttk2 = Member2(nout, 3);
	ttk3 = Member3(nout, 3);

	if (ttk1>ttk2 && ttk1>ttk3) m_kondisi = "Kipas Mati";
	else if (ttk2>ttk1 && ttk2>ttk3) m_kondisi = "1 kipas menyala";
	else if (ttk3>ttk1 && ttk3>ttk2) m_kondisi = "2 kipas menyala";

	UpdateData(FALSE);

}
