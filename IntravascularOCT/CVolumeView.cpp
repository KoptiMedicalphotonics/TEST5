// CVolumeView.cpp: 구현 파일
//

#include "stdafx.h"
#include "IntravascularOCT.h"
#include "CVolumeView.h"
#include "afxdialogex.h"

#include "ImageViewerManger.h"


// CVolumeView 대화 상자

IMPLEMENT_DYNAMIC(CVolumeView, CDialogEx)

CVolumeView::CVolumeView(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VOLUME_VIEW, pParent)
{

}

CVolumeView::~CVolumeView()
{
}

void CVolumeView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVolumeView, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CVolumeView 메시지 처리기


BOOL CVolumeView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return FALSE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


void CVolumeView::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (!::IsWindow(GetSafeHwnd()))
		return;

	CRect rect;
	GetClientRect(rect);
	// VTK 창크기
	CRect vtkRect = rect;
	if (nViewType_ == ImageViewerManager::VIEW_AXIAL ||
		nViewType_ == ImageViewerManager::VIEW_CORONAL ||
		nViewType_ == ImageViewerManager::VIEW_SAGITTAL) {
		// 높이 15 픽셀 스크롤바 설정
		CRect scrollRect = rect;
		scrollRect.top = rect.top + rect.Height() - 15;
		if (::IsWindow(ctrlScrollBar_.GetSafeHwnd()))
			ctrlScrollBar_.MoveWindow(scrollRect);

		// 스크롤바를 뺀 나머지 창 크기 계산
		vtkRect.bottom = rect.bottom - 15;
	}

	// VtkRender Window 크기 설정
	ImageViewerManager::Mgr()->ResizeVtkWindow(nViewType_, vtkRect.Width(), vtkRect.Height());
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CVolumeView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int scrollPos = pScrollBar->GetScrollPos();
	switch (nSBCode) {
	case SB_LINELEFT:
		scrollPos -= 1;
		break;
	case SB_LINERIGHT:
		scrollPos += 1;
		break;
	case SB_PAGELEFT:
		scrollPos -= 10;
		break;
	case SB_PAGERIGHT:
		scrollPos += 10;
		break;
	case SB_THUMBTRACK:
		scrollPos = (int)nPos;
		break;
	}

	if (pScrollBar == &ctrlScrollBar_) {
		pScrollBar->SetScrollPos(scrollPos);
		ImageViewerManager::Mgr()->ScrollSliceIndex(nViewType_, scrollPos);
	}


	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CVolumeView::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	//CDialogEx::OnCancel();
}


void CVolumeView::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	//CDialogEx::OnOK();
}


BOOL CVolumeView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// 2D의 경우 Scrollbar 생성
	if (nViewType_ == ImageViewerManager::VIEW_AXIAL ||
		nViewType_ == ImageViewerManager::VIEW_CORONAL ||
		nViewType_ == ImageViewerManager::VIEW_SAGITTAL) {
		ctrlScrollBar_.Create(SBS_HORZ | SBS_BOTTOMALIGN | WS_CHILD, CRect(0, 0, 1000, 100), this, 1);
		ctrlScrollBar_.ShowScrollBar();
	}

	// VTK Window 생성
	ImageViewerManager::Mgr()->InitVtkWindow(nViewType_, GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CVolumeView::update_scroll_bar() {
	// 스크롤바 생성 여부 검사
	if (ctrlScrollBar_.GetSafeHwnd() == NULL)
		return;

	// 현재 로드된 Volume Data
	vtkSmartPointer<VolumeData> volumeData = ImageViewerManager::Mgr()->GetImageLoader()->GetVolumeData();
	if (volumeData == NULL)
		return;

	// Volume 이미지 인덱스 범위
	int ext[6];
	volumeData->GetImgeData()->GetExtent(ext);

	// Slice Type에 다른 스크롤바 범위 설정
	switch (nViewType_) {
	case ImageViewerManager::VIEW_AXIAL:
		ctrlScrollBar_.SetScrollRange(ext[4], ext[5]);
		break;
	case ImageViewerManager::VIEW_CORONAL:
		ctrlScrollBar_.SetScrollRange(ext[2], ext[3]);
		break;
	case ImageViewerManager::VIEW_SAGITTAL:
		ctrlScrollBar_.SetScrollRange(ext[0], ext[1]);
		break;
	}

	// 스크롤바 위치 설정
	ctrlScrollBar_.SetScrollPos(volumeData->GetSliceIndex(nViewType_));
}