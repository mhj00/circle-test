
// GLIMDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "GLIM.h"
#include "GLIMDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <atlimage.h> // 이미지 저장을 위한 헤더 추가
#include <opencv2/opencv.hpp>
#include <atlimage.h>
#include <fstream>

using namespace cv;
using namespace std;


CImage m_image;
#include <vector>

struct CircleInfo {
	int radius;
	int centerX;
	int centerY;
	int x1;
	int y1;
	int x2;
	int y2;
};

std::vector<CircleInfo> gCircleData; // 전역 데이터 저장용

#ifdef _DEBUG
#define new DEBUG_NEW
#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")  // <-항상 콘솔 창 뜨게하는 코드 Debug일때 중요*
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CGLIMDlg 대화 상자



CGLIMDlg::CGLIMDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GLIM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGLIMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, ID_VIEW_LIST, m_View);
}

BEGIN_MESSAGE_MAP(CGLIMDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CGLIMDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CGLIMDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CGLIMDlg 메시지 처리기

BOOL CGLIMDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.
	 
	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CGLIMDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CGLIMDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CGLIMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//영처
void FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin)
{
	assert(bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));
	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);
	memset(bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = width;
	bmih->biHeight = origin ? abs(height) : -abs(height);
	bmih->biPlanes = 1;
	bmih->biBitCount = (unsigned short)bpp;
	bmih->biCompression = BI_RGB;
	if (bpp == 8) {
		RGBQUAD* palette = bmi->bmiColors;
		for (int i = 0; i < 256; i++) {
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}
}
//화면 추출해주는 코드 3
void DisplayImage(CDC* pDC, CRect rect, Mat& srcimg)
{
	if (!srcimg.data) return;
	Mat img;
	int step = ((int)(rect.Width() / 4)) * 4; // 4byte 단위조정해야 영상이 기울어지지 않는다.
	uchar buffer[sizeof(BITMAPINFOHEADER) * 1024];

	resize(srcimg, img, Size(step, rect.Height()));
	int bmp_w = img.cols;
	int bmp_h = img.rows;
	int depth = img.depth();
	int channels = img.channels();
	int bpp = 8 * channels;

	BITMAPINFO* bmi = (BITMAPINFO*)buffer;
	FillBitmapInfo(bmi, bmp_w, bmp_h, bpp, 0);
	int from_x = MIN(0, bmp_w - 1);
	int from_y = MIN(0, bmp_h - 1);
	int sw = MAX(MIN(bmp_w - from_x, rect.Width()), 0);
	int sh = MAX(MIN(bmp_h - from_y, rect.Height()), 0);

	SetDIBitsToDevice(pDC->m_hDC, rect.left, rect.top, sw, sh, from_x, from_y, 0, sh, img.data + from_y * img.step, bmi, 0);
	img.release();

}


void CGLIMDlg::OnBnClickedButton1()
{
	//완
	CClientDC dc(this); // 다이얼로그의 DC 가져오기

	// 라인에딧 값 가져오기 = 그릴 원의 개수 가져오기
	CString mText;
	m_edit1.GetWindowText(mText);
	int numCircles = _wtoi(mText.GetBuffer()); // 문자열 -> 정수 그릴 원의 개수
	mText.ReleaseBuffer();

	cout << numCircles << endl; // 출력 확인용

	// 다이얼로그의 클라이언트 영역 크기
	CRect rect;
	GetClientRect(&rect);

	int startX = 0;
	int startY = rect.Height()/10; // 공통 시작 Y 좌표
	int outputWidth = rect.Width();
	int outputHeight = rect.Height();
	//클라이언트 영역 선언
	CRect drawRect(0, startY, rect.Width(), rect.Height());

	// 랜덤 시드 설정
	srand((unsigned)time(0));
	std::ofstream outFile("circle_data.txt"); 

	if (!outFile.is_open()) {
		std::cout << "Failed to open circle_data.txt for writing." << std::endl;
		return;
	}

	CString fileName;

	for (int i = 0; i < numCircles; ++i) {
		// 클라이언트 영역 화면 초기화
		dc.FillSolidRect(&drawRect, RGB(0, 0, 0));

		// 랜덤 반지름 설정 (최소 10, 최대 우하단 영역의 크기에 맞게 조정)
		int maxRadius = min(outputWidth, outputHeight) / 4; // 영역 크기에 따른 최대 반지름
		//화면길이 설정에 반지름 길이 구함 사진 크기가 527 /4 =131 값이 됨 (int 정수)
		int radius = rand() % (maxRadius - 10) + 10;
		// %를 사용하므로 랜덤에 범위를 설정해주고 +10을 해주므로서 반지름이 안생기는걸 방지
		//실수로 하려면 double radius = (rand() % (maxRadius - 10) + 10) + (rand() / (double)RAND_MAX);

		// 랜덤 중심 좌표 설정
		int centerX = rand() % (outputWidth - 2 * radius) + radius;
		int centerY = rand() % (outputHeight - 2 * radius) + startY + radius;

		// 원의 좌표 계산
		int x1 = centerX - radius;
		int y1 = centerY - radius;
		int x2 = centerX + radius;
		int y2 = centerY + radius;

		// 좌표 출력
		std::cout << "Circle " << i + 1 << " :" << "  (" << x1 << ", " << y1 << "), (" << x2 << ", " << y2 << ")\n";
		// 반지름
		std::cout << "반지름(radius): " << radius << std::endl;
		// 중심좌표
		std::cout << "중심좌표 (centerX, centerY)" << "(" << centerX << ", " << centerY << ")" << std::endl;

		

		// 다이얼로그 화면에 그리기
		CBrush randomBrush(RGB(255, 255, 255));
		CBrush* oldBrush = dc.SelectObject(&randomBrush);
		dc.Ellipse(x1, y1, x2, y2);
		//dc.Ellipse(centerX - 3, centerY - 3, centerX + 3, centerY + 3);
		dc.SelectObject(oldBrush);
		
		// 우하단 영역을 이미지로 저장
		CImage image;
		image.Create(outputWidth, outputHeight, 24); // 출력 이미지 크기
		CDC* memDC = CDC::FromHandle(image.GetDC()); //HDC로 변경해서  BitBit  그림그리기 가능 FromHandle이 다시 CDC로 변경

		// BitBlt로 우하단 영역만 복사
		memDC->BitBlt(0, 0, outputWidth, outputHeight, &dc, 0, startY, SRCCOPY); //SRCCOPY 화면에 그대로 복사
		image.ReleaseDC(); //HDC 해제

		// **파일 이름을 먼저 설정**
		fileName.Format(L"circle_%d.bmp", i + 1);
		image.Save(fileName); // BMP 형식으로 저장

		// 데이터를 저장 (시작 좌표(x1, y1)와 종료 좌표(x2, y2) 추가)
		outFile << CT2A(fileName) << " "// 파일 이름 저장
			<< radius << " "		// 반지름
			<< centerX << " "		// 중심 X
			<< centerY << " "		// 중심 Y
			<< x1 << " "			//시작 좌표(x1, y1)
			<< y1 << " "
			<< x2 << " "			//종료 좌표(x2, y2) 
			<< y2 << std::endl;

		// 지연 추가
		this_thread::sleep_for(chrono::seconds(1));
	}
	outFile.close(); // 파일 닫기
	
	


}

	//openCV 나만의 준비중 
	//std::string filePath = CT2A(fileName);
	//Mat srcImg = imread(filePath, IMREAD_COLOR);
	//CDC* pDC = m_View.GetDC();  // 출력한 부분의 DC 얻기
	// 
	//m_View.GetClientRect(rect);  // 출력할 영역 얻기
	//DisplayImage(pDC, rect, srcImg);
	//ReleaseDC(pDC);

void CGLIMDlg::OnBnClickedButton2()
{	
	// 다이얼로그의 DC 가져오기
	CClientDC dc(this);

	// 다이얼로그의 클라이언트 영역 크기
	CRect rect;
	GetClientRect(&rect);

	int startY = rect.Height()/10; // 공통 시작 Y 좌표
	int outputWidth = rect.Width();
	int outputHeight = rect.Height(); // 공통 출력 높이
	CRect drawRect(0, startY, rect.Width(), rect.Height());

	// 기존 이미지 파일 로드
	CFileDialog fileDlg(TRUE, L"bmp", NULL, OFN_FILEMUSTEXIST, L"Image Files (*.bmp;*.jpg;*.jpeg)|*.bmp;*.jpg;*.jpeg|All Files (*.*)|*.*||");
	if (fileDlg.DoModal() == IDOK) {
		CString filePath = fileDlg.GetPathName();
		CString fileName = fileDlg.GetFileName();

		std::ifstream dataFile("circle_data.txt");
		if (!dataFile.is_open()) {
			std::cout << "Could not open circle_data.txt file." << std::endl;
			return;
		}

		std::string line, name;
		int radius, centerX, centerY, x1, y1, x2, y2;
		bool found = false;

		
		CImage image;
		if (image.Load(filePath) != S_OK) {
			std::cout << "Failed to load image." << std::endl;
			return;
		}

		HDC hdcImage = image.GetDC();
		if (hdcImage == nullptr) {
			std::cout << "Failed to get DC from image." << std::endl;
			return;
		}

		while (std::getline(dataFile, line)) {
			std::istringstream iss(line);
			iss >> name >> radius >> centerX >> centerY >> x1 >> y1 >> x2 >> y2;
			// **Y 좌표 보정 (startY 적용)**
			// 좌표 보정
			
			centerY -= startY;
			y1 -= startY;
			y2 -= startY;

			// 선택한 파일 이름과 일치할 때만 출력 및 작업 수행
			if (fileName == CString(name.c_str())) {
				// 좌표와 데이터 출력
				std::cout << "File: " << name
					<< ", Center: (" << centerX << ", " << centerY << ")"
					<< ", Radius: " << radius
					<< ", Start: (" << x1 << ", " << y1 << ")"
					<< ", End: (" << x2 << ", " << y2 << ")" << std::endl;

				CString centerText;
				centerText.Format(L"Center: (%d, %d)", centerX, centerY); // 중심 좌표 문자열로 변환
				m_edit2.SetWindowText(centerText); // m_edit2에 표시

				dc.BitBlt(0, startY, outputWidth, outputHeight, CDC::FromHandle(hdcImage), 0, 0, SRCCOPY);

				int adjustedCenterY = centerY + startY; // 중심 좌표 출력 보정
				int adjustedY1 = y1 + startY;          // 시작 좌표 보정
				int adjustedY2 = y2 + startY;          // 끝 좌표 보정


				dc.SetBkMode(TRANSPARENT);
				CBrush blueBrush(RGB(0, 0, 255));
				CBrush* pOldBrush = dc.SelectObject(&blueBrush);
				dc.Ellipse(centerX - 3, adjustedCenterY - 3, centerX + 3, adjustedCenterY + 3);
				dc.SelectObject(pOldBrush);

				CPen redPen(PS_SOLID, 1, RGB(255, 0, 0));
				CPen* pOldPen = dc.SelectObject(&redPen);
				dc.MoveTo(centerX, adjustedCenterY);
				dc.LineTo(centerX + radius, adjustedCenterY); // 반지름 길이만큼 선 그리기
				dc.SelectObject(pOldPen);

				// **점선 그리기** (위쪽으로, 왼쪽으로)
				CPen dashPen(PS_DOT, 1, RGB(0, 255, 0)); // 점선 (녹색)
				CPen* pOldDashPen = dc.SelectObject(&dashPen);

				// 위쪽으로 점선
				dc.MoveTo(centerX, adjustedCenterY);       // 시작점 (중심)
				dc.LineTo(centerX, adjustedCenterY - rect.Height()); // 위쪽으로 50만큼 점선 그리기

				// 왼쪽으로 점선
				dc.MoveTo(centerX, adjustedCenterY);       // 시작점 (중심)
				dc.LineTo(centerX - rect.Width(), adjustedCenterY); // 왼쪽으로 50만큼 점선 그리기

				dc.SelectObject(pOldDashPen); // 이전 펜 복원

				// **반지름을 빨간색 글씨로 출력**
				CString radiusText;
				radiusText.Format(L"Radius: %d", radius); // 반지름 값 문자열로 변환

				// 빨간색 텍스트로 반지름 출력
				dc.SetBkMode(TRANSPARENT); // 배경을 투명으로 설정
				dc.SetTextColor(RGB(255, 0, 0)); // 텍스트 색상을 빨간색으로 설정
				dc.TextOut(centerX + radius + 10, adjustedCenterY - 10, radiusText); // 반지름 값 출력

				found = true;
				break; // 데이터가 매칭되면 루프 종료
			}
		}
		dataFile.close();
		image.ReleaseDC();

		if (!found) {
			std::cout << "No data found for the selected file." << std::endl;
		}
	}


}
