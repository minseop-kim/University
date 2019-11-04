// ProjectionDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "img_process(4).h"
#include "ProjectionDialog.h"
#include "Histogram.h"
#include "afxdialogex.h"
#include "CvvImage.h"


// ProjectionDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(ProjectionDialog, CDialogEx)

ProjectionDialog::ProjectionDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(ProjectionDialog::IDD, pParent)
{
	is_opend = false;
}

ProjectionDialog::~ProjectionDialog()
{
}

void ProjectionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROJECTION, projection_picturecontrol);
}


BEGIN_MESSAGE_MAP(ProjectionDialog, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()




void ProjectionDialog::OnPaint()
{
	GetWindowRect(size);
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	
	//if(is_opend)
	{
		Draw_Img_PictureControl(projectionDC, &projection_picturecontrol, projection_Iplimg, &projection_cvimg, projection_picturecontrol_size);
		//is_opend = false;
	}
}
