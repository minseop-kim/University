
// img_process(4)Dlg.cpp : ���� ����
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


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// Cimg_process4Dlg ��ȭ ����




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


// Cimg_process4Dlg �޽��� ó����

BOOL Cimg_process4Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void Cimg_process4Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
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

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR Cimg_process4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cimg_process4Dlg::OnBnClickedFileSelect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	LPSTR szFilter = "BMP Files (*.bmp)|*.bmp|All Files (*.*)|*.*|";
	CFileDialog dlgFile(true);
	dlgFile.m_ofn.lpstrTitle = L"���� �׸� ����";

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	cvSet(original_histo_Iplimg, CV_RGB(255, 255, 255), 0);
	histo_original_selected = true;
	RGBG = 2;
	InvalidateRect(NULL, true);
}

void Cimg_process4Dlg::OnBnClickedGreenRadio()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	cvSet(original_histo_Iplimg, CV_RGB(255, 255, 255), 0);
	histo_original_selected = true;
	RGBG = 1;
	InvalidateRect(NULL, true);
}

void Cimg_process4Dlg::OnBnClickedBlueRadio()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	cvSet(original_histo_Iplimg, CV_RGB(255, 255, 255), 0);
	histo_original_selected = true;
	RGBG = 0;
	InvalidateRect(NULL, true);
}

void Cimg_process4Dlg::OnBnClickedGrayRadio()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	cvSet(change_histo_Iplimg, CV_RGB(255, 255, 255), 0);
	histo_change_selected = true;
	cRGBG = 2;
	InvalidateRect(NULL, true);
}

void Cimg_process4Dlg::OnBnClickedGreenRadio2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	cvSet(change_histo_Iplimg, CV_RGB(255, 255, 255), 0);
	histo_change_selected = true;
	cRGBG = 1;
	InvalidateRect(NULL, true);
}

void Cimg_process4Dlg::OnBnClickedBlueRadio2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	cvSet(change_histo_Iplimg, CV_RGB(255, 255, 255), 0);
	histo_change_selected = true;
	cRGBG = 0;
	InvalidateRect(NULL, true);
}

void Cimg_process4Dlg::OnBnClickedGrayRadio2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	cvSet(change_histo_Iplimg, CV_RGB(255, 255, 255), 0);
	histo_change_selected = true;
	cRGBG = 3;
	HistoGram(change_histo_Iplimg, change_Iplimg, imgrect, histogramrect);
	InvalidateRect(NULL, true);
}

void Cimg_process4Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if(combo_selected == 1 || combo_selected == 7)
	{
		if(pScrollBar != NULL && pScrollBar->m_hWnd == original_slider.m_hWnd)
		{
			CString str;
			double border = (double)(original_slider.GetPos());
			// �����̴� ��Ʈ���� ���簪�� GetPos() �Լ��� ����Ͽ� �˾Ƴ����ִ�.
			// ������������ ���簪�� ����Ʈ ��Ʈ�ѿ� ǥ���ϱ� ���ؼ� ���������� ��ȯ�Ѵ�.
			str.Format(L"%d / 255", original_slider.GetPos());

			// ���ڿ� �������� ����� ���簪�� ����Ʈ ��Ʈ�ѿ� ����Ѵ�.
			original_editbox.SetWindowText(str);

			/*CV_THRESH_BINARY		: threshold �� �ʰ��� 255, ���ϴ� 0
			CV_THRESH_BINARY_INV	: threshold �� �ʰ��� 0, ���ϴ� 255
			CV_THRESH_TRUNC		: threshold �� �ʰ��� threshold, ���ϴ� �״��
			CV_THRESH_TOZERO		: threshold �� �ʰ��� �״��, ���ϴ� 0
			CV_THRESH_TOZERO_INV	: threshold �� �ʰ��� 0, ���ϴ� �״�� */
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	change_img_selectd = true;
	combo_selected = change_combobox.GetCurSel();

	switch(combo_selected)
	{
	// �׷���, ����, ������׷� ��Ȱȭ, ������׷� �յ�ȭ
	case 0: case 1: case 4: case 5:	case 6: case 7: case 8:
		change_Iplimg = cvCreateImage(cvGetSize(original_Iplimg), original_Iplimg->depth, 1);
		break;
	case 2: case 3:					// ����, ������
		change_Iplimg = cvCreateImage(cvGetSize(original_Iplimg), original_Iplimg->depth, original_Iplimg->nChannels);
		break;
	}

	switch(combo_selected)
	{
	case 0:	// �׷���
		cvCvtColor(original_Iplimg, change_Iplimg, CV_RGB2GRAY);
		break;
	case 1:
		cvCvtColor(original_Iplimg, gra, CV_RGB2GRAY);
		break;
	case 2:	// ����
		DibFilterMean(original_Iplimg, change_Iplimg, 0);
		break;
	case 3:	// ������
		filter(original_Iplimg, change_Iplimg);
		break;
	case 4:	// ������׷� ��Ȱȭ
		HistoGram_Gray_Strech(original_Iplimg, change_Iplimg);
		break;
	case 5:	// ������׷� �յ�ȭ
		HistoGram_Gray_Equalization(original_Iplimg, change_Iplimg);
		break;
	case 6:	// �ܰ��� ����(������Ʈ ����)
		if(original_Iplimg->nChannels == 3)
			cvCvtColor(original_Iplimg, gra, CV_RGB2GRAY);
		Prewitt_filter(gra, change_Iplimg);
		break;
	case 7: // ĳ�� ����
		if(original_Iplimg->nChannels == 3)
			cvCvtColor(original_Iplimg, gra, CV_RGB2GRAY);
		DibFilterMean(gra, change_Iplimg, 1);
		break;
	case 8: // LoG ���ö��̾� ���� ����þ�
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	ProjectionDialog prodia;
	prodia.projection_Iplimg = cvCreateImage(cvGetSize(change_Iplimg), change_Iplimg->depth, change_Iplimg->nChannels);
	Process_Projection(change_Iplimg, prodia.projection_Iplimg, 0);
	//prodia.is_opend = true;

	prodia.DoModal();
	//DeleteObject(prodia.projection_Iplimg);
}

void Cimg_process4Dlg::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	cvSaveImage("G512_512_exception.bmp", change_Iplimg, 0);
}


void Cimg_process4Dlg::OnSavetogray()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}
