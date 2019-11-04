
// img_process(4)Dlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "img_process(4).h"
#include "img_process(4)Dlg.h"
#include "afxdialogex.h"
#include "ProjectionDialog.h"
#include "Projection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnPaint();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_HSCROLL()
//	ON_WM_PAINT()
END_MESSAGE_MAP()


// Cimg_process4Dlg 대화 상자




Cimg_process4Dlg::Cimg_process4Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cimg_process4Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cimg_process4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ORIGINAL_PICBOX, original_picbox);
	DDX_Control(pDX, IDC_CHANGE_PICBOX, change_picbox);
	DDX_Control(pDX, IDC_ORIGINAL_HISTOPICBOX, original_histopicbox);
	DDX_Control(pDX, IDC_CHANGE_HISTOPICBOX, change_histopicbox);
	DDX_Control(pDX, IDC_ORIGINAL_SLIDER, original_slider);
	DDX_Control(pDX, IDC_ORIGINAL_EDIT1, original_editbox);
	DDX_Control(pDX, IDC_FILE_SELECT, file_select_button);
	DDX_Control(pDX, IDC_COMBO1, change_combobox);
	DDX_Control(pDX, IDC_BUTTON1, projection_x);
	DDX_Control(pDX, IDC_BUTTON2, projection_y);
}

BEGIN_MESSAGE_MAP(Cimg_process4Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_FILE_SELECT, &Cimg_process4Dlg::OnBnClickedFileSelect)
	ON_BN_CLICKED(IDC_RED_RADIO, &Cimg_process4Dlg::OnBnClickedRedRadio)
	ON_BN_CLICKED(IDC_GREEN_RADIO, &Cimg_process4Dlg::OnBnClickedGreenRadio)
	ON_BN_CLICKED(IDC_BLUE_RADIO, &Cimg_process4Dlg::OnBnClickedBlueRadio)
	ON_BN_CLICKED(IDC_RED_RADIO2, &Cimg_process4Dlg::OnBnClickedRedRadio2)
	ON_BN_CLICKED(IDC_GREEN_RADIO2, &Cimg_process4Dlg::OnBnClickedGreenRadio2)
	ON_BN_CLICKED(IDC_BLUE_RADIO2, &Cimg_process4Dlg::OnBnClickedBlueRadio2)
ON_WM_HSCROLL()
ON_CBN_SELCHANGE(IDC_COMBO1, &Cimg_process4Dlg::OnCbnSelchangeCombo1)
ON_BN_CLICKED(IDC_GRAY_RADIO, &Cimg_process4Dlg::OnBnClickedGrayRadio)
ON_BN_CLICKED(IDC_GRAY_RADIO2, &Cimg_process4Dlg::OnBnClickedGrayRadio2)
ON_BN_CLICKED(IDC_BUTTON1, &Cimg_process4Dlg::OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON2, &Cimg_process4Dlg::OnBnClickedButton2)
ON_COMMAND(ID_SAVE, &Cimg_process4Dlg::OnSave)
ON_COMMAND(ID_SAVETOGRAY, &Cimg_process4Dlg::OnSavetogray)
END_MESSAGE_MAP()


// Cimg_process4Dlg 메시지 처리기

BOOL Cimg_process4Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	file_selected = false;
	histo_original_selected = false;
	histo_change_selected = false;
	change_img_selectd = false;

	original_histopicbox.SetWindowPos(NULL, 0, 0, 256, 50, SWP_NOMOVE | SWP_SHOWWINDOW);
	change_histopicbox.SetWindowPos(NULL, 0, 0, 256, 50, SWP_NOMOVE | SWP_SHOWWINDOW);

	original_slider.SetRange(0, 255);
	original_slider.SetTicFreq(1);                
	original_slider.SetLineSize(1);
		
	original_histopicbox.GetClientRect(&histogramrect);
	
	change_combobox.InsertString(0, L"GrayScale");
	change_combobox.InsertString(1, L"Thresholding");
	change_combobox.InsertString(2, L"Blurring");
	change_combobox.InsertString(3, L"Sharpening");
	change_combobox.InsertString(4, L"Histogram Stretching");
	change_combobox.InsertString(5, L"Histogram Equalization");
	change_combobox.InsertString(6, L"Prewitt Edge Detection");
	change_combobox.InsertString(7, L"Canny Filter");
	change_combobox.InsertString(8, L"Laplacian of Gaussian");

	change_picbox.GetWindowRect(&position);
	change_histopicbox.GetWindowRect(&position2);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void Cimg_process4Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void Cimg_process4Dlg::OnPaint()
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
		
		if(file_selected)
			Draw_Img_PictureControl(original_DC, &original_picbox, original_Iplimg, &original_cvimg, imgrect);
		if(histo_original_selected)
		{
			if(RGBG != 3)
			{
				bgr = (RGBHISTO*)calloc(256, sizeof(RGBHISTO));
				bgr = RGBHIsto(original_Iplimg, imgrect, histogramrect);
				Draw_Histo_pictureControl(original_histoDC, &original_histopicbox, original_histo_Iplimg, original_Iplimg, &original_histo_cvimg, bgr, histogramrect, RGBG);
				free(bgr);
			}else
			{
				Draw_Img_PictureControl(original_histoDC, &original_histopicbox, original_histo_Iplimg, &original_histo_cvimg, histogramrect);
			}
		}
		if(change_img_selectd)
			Draw_Img_PictureControl(change_DC, &change_picbox, change_Iplimg, &change_cvimg, imgrect);
		if(histo_change_selected)
		{
			if(cRGBG != 3)
			{
				bgr = (RGBHISTO*)calloc(256, sizeof(RGBHISTO));
				bgr = RGBHIsto(change_Iplimg, imgrect, histogramrect);
				Draw_Histo_pictureControl(change_histoDC, &change_histopicbox, change_histo_Iplimg, change_Iplimg, &change_histo_cvimg, bgr, histogramrect, cRGBG);
				free(bgr);
			}else
			{
				Draw_Img_PictureControl(change_histoDC, &change_histopicbox, change_histo_Iplimg, &change_histo_cvimg, histogramrect);
			}
		}
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR Cimg_process4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cimg_process4Dlg::OnBnClickedFileSelect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	LPSTR szFilter = "BMP Files (*.bmp)|*.bmp|All Files (*.*)|*.*|";
	CFileDialog dlgFile(true);
	dlgFile.m_ofn.lpstrTitle = L"원본 그림 선택";

	if(dlgFile.DoModal() == IDOK)
	{
		file_path = dlgFile.GetPathName();
		file_selected = true;

		histo_original_selected = false;
		DeleteObject(original_histo_Iplimg);
		DeleteObject(change_Iplimg);
		histo_change_selected = false;
		DeleteObject(change_histo_Iplimg);
		DeleteObject(gra);
		DeleteObject(chan);

		original_Iplimg = cvLoadImage(file_path, 1);

		/*
		for(int tempy=10; tempy< 20; tempy++)
		{
			for(int tempx=0; tempx < original_Iplimg->width; tempx++)
			{
				original_Iplimg->imageData[tempy*(original_Iplimg->widthStep) + (tempx*original_Iplimg->nChannels)] = 250;
				original_Iplimg->imageData[tempy*(original_Iplimg->widthStep) + (tempx*original_Iplimg->nChannels)+1] = 250;
				original_Iplimg->imageData[tempy*(original_Iplimg->widthStep) + (tempx*original_Iplimg->nChannels)+2] = 250;
			}
		}
		*/
		original_histo_Iplimg = cvCreateImage(cvSize(256,50), original_Iplimg->depth, original_Iplimg->nChannels);
		change_histo_Iplimg = cvCreateImage(cvSize(256,50), original_Iplimg->depth, original_Iplimg->nChannels);

		gra = cvCreateImage(cvGetSize(original_Iplimg), original_Iplimg->depth, 1);

		chan = cvCreateImage(cvGetSize(original_Iplimg), original_Iplimg->depth, 1);
		

		/*
		IplImage *a = cvCreateImage(cvGetSize(original_Iplimg), original_Iplimg->depth, 1);
		cvCvtColor(original_Iplimg, a, CV_RGB2GRAY);
		cvSaveImage("dark_img_gray_255_white.bmp", a, 0);
		*/
		InvalidateRect(NULL, true);
	}
}

void Cimg_process4Dlg::OnBnClickedRedRadio()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	cvSet(original_histo_Iplimg, CV_RGB(255, 255, 255), 0);
	histo_original_selected = true;
	RGBG = 2;
	InvalidateRect(NULL, true);
}

void Cimg_process4Dlg::OnBnClickedGreenRadio()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	cvSet(original_histo_Iplimg, CV_RGB(255, 255, 255), 0);
	histo_original_selected = true;
	RGBG = 1;
	InvalidateRect(NULL, true);
}

void Cimg_process4Dlg::OnBnClickedBlueRadio()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	cvSet(original_histo_Iplimg, CV_RGB(255, 255, 255), 0);
	histo_original_selected = true;
	RGBG = 0;
	InvalidateRect(NULL, true);
}

void Cimg_process4Dlg::OnBnClickedGrayRadio()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	cvSet(original_histo_Iplimg, CV_RGB(255, 255, 255), 0);

	histo_original_selected = true;
	RGBG = 3;
	if(original_histo_Iplimg->nChannels == 3)
		HistoGram(original_histo_Iplimg, original_Iplimg, imgrect, histogramrect);
	else
	{
		IplImage *grayhis = cvCreateImage(cvGetSize(original_Iplimg), original_Iplimg->depth, 1);
		cvCvtColor(original_Iplimg, gra, CV_RGB2GRAY);
		HistoGram(grayhis, original_Iplimg, imgrect, histogramrect);
	}
	InvalidateRect(NULL, true);
}

void Cimg_process4Dlg::OnBnClickedRedRadio2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	cvSet(change_histo_Iplimg, CV_RGB(255, 255, 255), 0);
	histo_change_selected = true;
	cRGBG = 2;
	InvalidateRect(NULL, true);
}

void Cimg_process4Dlg::OnBnClickedGreenRadio2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	cvSet(change_histo_Iplimg, CV_RGB(255, 255, 255), 0);
	histo_change_selected = true;
	cRGBG = 1;
	InvalidateRect(NULL, true);
}

void Cimg_process4Dlg::OnBnClickedBlueRadio2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	cvSet(change_histo_Iplimg, CV_RGB(255, 255, 255), 0);
	histo_change_selected = true;
	cRGBG = 0;
	InvalidateRect(NULL, true);
}

void Cimg_process4Dlg::OnBnClickedGrayRadio2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	cvSet(change_histo_Iplimg, CV_RGB(255, 255, 255), 0);
	histo_change_selected = true;
	cRGBG = 3;
	HistoGram(change_histo_Iplimg, change_Iplimg, imgrect, histogramrect);
	InvalidateRect(NULL, true);
}

void Cimg_process4Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(combo_selected == 1 || combo_selected == 7)
	{
		if(pScrollBar != NULL && pScrollBar->m_hWnd == original_slider.m_hWnd)
		{
			CString str;
			double border = (double)(original_slider.GetPos());
			// 슬라이더 컨트롤의 현재값은 GetPos() 함수를 사용하여 알아낼수있다.
			// 정수값형태의 현재값을 에디트 컨트롤에 표시하기 위해서 문자형으로 변환한다.
			str.Format(L"%d / 255", original_slider.GetPos());

			// 문자열 형식으로 변경된 현재값을 에디트 컨트롤에 출력한다.
			original_editbox.SetWindowText(str);

			/*CV_THRESH_BINARY		: threshold 값 초과는 255, 이하는 0
			CV_THRESH_BINARY_INV	: threshold 값 초과는 0, 이하는 255
			CV_THRESH_TRUNC		: threshold 값 초과는 threshold, 이하는 그대로
			CV_THRESH_TOZERO		: threshold 값 초과는 그대로, 이하는 0
			CV_THRESH_TOZERO_INV	: threshold 값 초과는 0, 이하는 그대로 */
			if(combo_selected == 1)
				cvThreshold(gra, change_Iplimg, border, 255.0, CV_THRESH_BINARY);
		}
		InvalidateRect(position, false);
		InvalidateRect(position2, false);
		CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}

void Cimg_process4Dlg::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	change_img_selectd = true;
	combo_selected = change_combobox.GetCurSel();

	switch(combo_selected)
	{
	// 그레이, 이진, 히스토그램 평활화, 히스토그램 균등화
	case 0: case 1: case 4: case 5:	case 6: case 7: case 8:
		change_Iplimg = cvCreateImage(cvGetSize(original_Iplimg), original_Iplimg->depth, 1);
		break;
	case 2: case 3:					// 블러링, 샤프닝
		change_Iplimg = cvCreateImage(cvGetSize(original_Iplimg), original_Iplimg->depth, original_Iplimg->nChannels);
		break;
	}

	switch(combo_selected)
	{
	case 0:	// 그레이
		cvCvtColor(original_Iplimg, change_Iplimg, CV_RGB2GRAY);
		break;
	case 1:
		cvCvtColor(original_Iplimg, gra, CV_RGB2GRAY);
		break;
	case 2:	// 블러링
		DibFilterMean(original_Iplimg, change_Iplimg, 0);
		break;
	case 3:	// 샤프닝
		filter(original_Iplimg, change_Iplimg);
		break;
	case 4:	// 히스토그램 평활화
		HistoGram_Gray_Strech(original_Iplimg, change_Iplimg);
		break;
	case 5:	// 히스토그램 균등화
		HistoGram_Gray_Equalization(original_Iplimg, change_Iplimg);
		break;
	case 6:	// 외곽선 검출(프리위트 필터)
		if(original_Iplimg->nChannels == 3)
			cvCvtColor(original_Iplimg, gra, CV_RGB2GRAY);
		Prewitt_filter(gra, change_Iplimg);
		break;
	case 7: // 캐니 필터
		if(original_Iplimg->nChannels == 3)
			cvCvtColor(original_Iplimg, gra, CV_RGB2GRAY);
		DibFilterMean(gra, change_Iplimg, 1);
		break;
	case 8: // LoG 라플라이안 오브 가우시안
		if(original_Iplimg->nChannels == 3)
		{
			cvCvtColor(original_Iplimg, gra, CV_RGB2GRAY);
			laplacian_of_gausian(gra, change_Iplimg);
		}
		else
			laplacian_of_gausian(original_Iplimg, change_Iplimg);
		break;
	}
	histo_change_selected = false;
	Invalidate(true);
}

void Cimg_process4Dlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	ProjectionDialog prodia;
	prodia.projection_Iplimg = cvCreateImage(cvGetSize(change_Iplimg), change_Iplimg->depth, change_Iplimg->nChannels);
	Process_Projection(change_Iplimg, prodia.projection_Iplimg, 0);
	//prodia.is_opend = true;

	prodia.DoModal();
	//DeleteObject(prodia.projection_Iplimg);
}

void Cimg_process4Dlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ProjectionDialog prodia;
	prodia.projection_Iplimg = cvCreateImage(cvGetSize(change_Iplimg), change_Iplimg->depth, change_Iplimg->nChannels);

	Process_Projection(change_Iplimg, prodia.projection_Iplimg, 1);
//	prodia.is_opend = true;

	//this->SetWindowPos(null, 200, 200, 300, 300, SWP_SHOWWINDOW);
//	prodia.SetWindowPos(, 200, 200, 300, 300, SWP_NOMOVE);
	prodia.DoModal();
	//DeleteObject(prodia.projection_Iplimg);
}



void Cimg_process4Dlg::OnSave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	cvSaveImage("G512_512_exception.bmp", change_Iplimg, 0);
}


void Cimg_process4Dlg::OnSavetogray()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}
