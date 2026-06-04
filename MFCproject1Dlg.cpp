
// MFCproject1Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCproject1.h"
#include "MFCproject1Dlg.h"
#include "afxdialogex.h"
// ★ 추가 시작 ★
#include <cmath>
#include <ctime>
// ★ 추가 끝 ★

#ifdef _DEBUG
#define new DEBUG_NEW
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


// CMFCproject1Dlg 대화 상자



CMFCproject1Dlg::CMFCproject1Dlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MFCPROJECT1_DIALOG, pParent)
    // ★ 추가 시작 (변수들의 초기값을 설정합니다) ★
    , m_editRadius(10)       // 에디트박스 초기 반지름 값
    , m_editThickness(2)    // 에디트박스 초기 선 두께 값
    // ★ 추가 끝 ★
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    // ★ 추가 시작 ★
    m_nDragIndex = -1;
    m_bIsThreadRunning = false;
    // ★ 추가 끝 ★
}

void CMFCproject1Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    // ★ 추가 시작 (에디트 박스 ID와 변수 매핑을 하기위해 쓰이는 것) ★
    DDX_Text(pDX, IDC_EDIT_RADIUS, m_editRadius);
    DDX_Text(pDX, IDC_EDIT_THICKNESS, m_editThickness);
    // ★ 추가 끝 ★
}

BEGIN_MESSAGE_MAP(CMFCproject1Dlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    // ★ 추가 시작 (이벤트 매핑 코드를 넣었습니다) ★
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_BN_CLICKED(IDC_BTN_RESET, &CMFCproject1Dlg::OnBnClickedBtnReset)
    ON_BN_CLICKED(IDC_BTN_RANDOM, &CMFCproject1Dlg::OnBnClickedBtnRandom)
    // ★ 추가 끝 ★
END_MESSAGE_MAP()


// CMFCproject1Dlg 메시지 처리기

BOOL CMFCproject1Dlg::OnInitDialog()
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
    // ★ 추가 시작 ★
    // 640x480 크기, 8비트 그레이스케일 이미지 생성
    m_image.Create(640, 480, 8);

    // 흑백 색상 테이블 설정
    RGBQUAD rgbTable[256];
    for (int i = 0; i < 256; i++) {
        rgbTable[i].rgbRed = rgbTable[i].rgbGreen = rgbTable[i].rgbBlue = i;
        rgbTable[i].rgbReserved = 0;
    }
    m_image.SetColorTable(0, 256, rgbTable);

    // 배경을 흰색으로 깨끗이 밀기
    ClearImage();
    // ★ 추가 끝 ★

    return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCproject1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        // ★ 추가 시작 (기본 CDialogEx::OnPaint() 앞에 화면 갱신 함수 호출하기 위해 쓰입니다) ★
        UpdateDisplay();
        // ★ 추가 끝 ★
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCproject1Dlg::OnPaint()
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
HCURSOR CMFCproject1Dlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

// ★ 추가 시작 ★
void CMFCproject1Dlg::ClearImage()
{
    if (m_image.IsNull()) return;

    BYTE* pBits = (BYTE*)m_image.GetBits();
    int nWidth = m_image.GetWidth();
    int nHeight = m_image.GetHeight();
    int nPitch = m_image.GetPitch();

    for (int j = 0; j < nHeight; j++) {
        for (int i = 0; i < nWidth; i++) {
            pBits[j * nPitch + i] = 255; // 흰색으로 채움
        }
    }
}

void CMFCproject1Dlg::UpdateDisplay()
{
    CClientDC dc(this);
    if (!m_image.IsNull()) {
        m_image.Draw(dc, 0, 0);
    }
}

bool CMFCproject1Dlg::ValidImagePosition(int x, int y)
{
    CRect rect(0, 0, m_image.GetWidth(), m_image.GetHeight());
    return rect.PtInRect(CPoint(x, y)) == TRUE;
}

// 클릭 지점 원(점) 그리기
void CMFCproject1Dlg::DrawDot(int cx, int cy, int r, BYTE color)
{
    BYTE* pBits = (BYTE*)m_image.GetBits();
    int nPitch = m_image.GetPitch();

    for (int j = cy - r; j <= cy + r; j++) {
        for (int i = cx - r; i <= cx + r; i++) {
            if (ValidImagePosition(i, j)) {
                if ((i - cx) * (i - cx) + (j - cy) * (j - cy) <= r * r) {
                    pBits[j * nPitch + i] = color;
                }
            }
        }
    }
}

// 세 점의 외심 구하기 수학 공식
bool CMFCproject1Dlg::GetCircleCenter(CPoint p1, CPoint p2, CPoint p3, double& cpX, double& cpY, double& r)
{
    double dA = p1.x * (p2.y - p3.y) - p1.y * (p2.x - p3.x) + p2.x * p3.y - p3.x * p2.y;
    if (abs(dA) < 0.000001) return false;

    double dB = (p1.x * p1.x + p1.y * p1.y) * (p3.y - p2.y) + (p2.x * p2.x + p2.y * p2.y) * (p1.y - p3.y) + (p3.x * p3.x + p3.y * p3.y) * (p2.y - p1.y);
    double dC = (p1.x * p1.x + p1.y * p1.y) * (p2.x - p3.x) + (p2.x * p2.x + p2.y * p2.y) * (p3.x - p1.x) + (p3.x * p3.x + p3.y * p3.y) * (p1.x - p2.x);

    cpX = -dB / (2 * dA);
    cpY = -dC / (2 * dA);
    r = sqrt((p1.x - cpX) * (p1.x - cpX) + (p1.y - cpY) * (p1.y - cpY));
    return true;
}

// 세 점을 지나는 큰 테두리 원 그리기 
void CMFCproject1Dlg::DrawBigCircle(CPoint p1, CPoint p2, CPoint p3, int thickness, BYTE color)
{
    double cpX, cpY, r;
    if (!GetCircleCenter(p1, p2, p3, cpX, cpY, r)) return;

    BYTE* pBits = (BYTE*)m_image.GetBits();
    int nPitch = m_image.GetPitch();

    int innerR = (int)r - thickness;
    int outerR = (int)r + thickness;

    for (int j = (int)(cpY - outerR); j <= (int)(cpY + outerR); j++) {
        for (int i = (int)(cpX - outerR); i <= (int)(cpX + outerR); i++) {
            if (ValidImagePosition(i, j)) {
                double distSq = (i - cpX) * (i - cpX) + (j - cpY) * (j - cpY);
                if (distSq >= innerR * innerR && distSq <= outerR * outerR) {
                    pBits[j * nPitch + i] = color;
                }
            }
        }
    }
}

// 마우스 왼쪽 버튼 클릭 처리
void CMFCproject1Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    UpdateData(TRUE); // 에디트박스 입력값 최신화

    if (m_vPoints.size() == 3) {
        for (int i = 0; i < 3; i++) {
            double dist = sqrt((m_vPoints[i].x - point.x) * (m_vPoints[i].x - point.x) + (m_vPoints[i].y - point.y) * (m_vPoints[i].y - point.y));
            if (dist <= m_editRadius) {
                m_nDragIndex = i; // 드래그 시작 선택
                break;
            }
        }
    }
    else if (m_vPoints.size() < 3) {
        m_vPoints.push_back(point); // 3개 미만일 때만 좌표 등록

        // 콘솔 및 디버그 창에 좌표 출력 요건 만족
        CString strLog;
        strLog.Format(_T("점 추가 -> X: %d, Y: %d\n"), point.x, point.y);
        OutputDebugString(strLog);
    }

    OnMouseMove(nFlags, point);
    CDialogEx::OnLButtonDown(nFlags, point);
}

// 마우스가 움직일 때 실시간 원 그리기 처리
void CMFCproject1Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
    if ((nFlags & MK_LBUTTON) && m_nDragIndex != -1) {
        m_vPoints[m_nDragIndex] = point; // 드래그 중인 점 좌표 갱신
    }

    if (nFlags & MK_LBUTTON || m_vPoints.size() <= 3) {
        ClearImage();

        // 찍혀있는 점들 복원
        for (size_t i = 0; i < m_vPoints.size(); i++) {
            DrawDot(m_vPoints[i].x, m_vPoints[i].y, m_editRadius, 0);
        }

        // 3개가 찍혔다면 통과하는 정원 그리기
        if (m_vPoints.size() == 3) {
            DrawBigCircle(m_vPoints[0], m_vPoints[1], m_vPoints[2], m_editThickness, 0);
        }
        UpdateDisplay();
    }
    CDialogEx::OnMouseMove(nFlags, point);
}

void CMFCproject1Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_nDragIndex = -1; // 드래그 해제
    CDialogEx::OnLButtonUp(nFlags, point);
}

// 초기화 버튼 이벤트
void CMFCproject1Dlg::OnBnClickedBtnReset()
{
    m_vPoints.clear();
    m_nDragIndex = -1;
    ClearImage();
    UpdateDisplay();
}

// 랜덤 이동 버튼 이벤트
void CMFCproject1Dlg::OnBnClickedBtnRandom()
{
    if (m_vPoints.size() < 3) {
        AfxMessageBox(_T("3개의 점이 찍혀 원이 그려진 상태여야 합니다."));
        return;
    }
    if (m_bIsThreadRunning) return;

    m_bIsThreadRunning = true;
    AfxBeginThread(ThreadRandomMove, this); // UI 프리징 방지 멀티스레드 가동
}

// 멀티스레드 실행부 (초당 2회 무작위 변동 총 10회 자동 반복)
UINT CMFCproject1Dlg::ThreadRandomMove(LPVOID pParam)
{
    CMFCproject1Dlg* pDlg = (CMFCproject1Dlg*)pParam;
    srand((unsigned int)time(NULL));

    int nWidth = pDlg->m_image.GetWidth();
    int nHeight = pDlg->m_image.GetHeight();

    for (int step = 0; step < 10; step++)
    {
        for (int i = 0; i < 3; i++) {
            pDlg->m_vPoints[i].x = rand() % nWidth;
            pDlg->m_vPoints[i].y = rand() % nHeight;
        }

        pDlg->ClearImage();
        for (int i = 0; i < 3; i++) {
            pDlg->DrawDot(pDlg->m_vPoints[i].x, pDlg->m_vPoints[i].y, pDlg->m_editRadius, 0);
        }
        pDlg->DrawBigCircle(pDlg->m_vPoints[0], pDlg->m_vPoints[1], pDlg->m_vPoints[2], pDlg->m_editThickness, 0);
        pDlg->UpdateDisplay();

        Sleep(500); // 0.5초 대기 (초당 2회 속도)
    }

    pDlg->m_bIsThreadRunning = false;
    return 0;
}
// ★ 추가 끝 ★