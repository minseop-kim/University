// ProjectionDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "img_process(4).h"
#include "ProjectionDialog.h"
#include "Histogram.h"
#include "afxdialogex.h"
#include "CvvImage.h"


// ProjectionDialog ��ȭ �����Դϴ�.

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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
	
	//if(is_opend)
	{
		Draw_Img_PictureControl(projectionDC, &projection_picturecontrol, projection_Iplimg, &projection_cvimg, projection_picturecontrol_size);
		//is_opend = false;
	}
}
