
// MFCproject1Dlg.h: 헤더 파일
//

#pragma once
// ★ 추가 시작 ★
#include <vector>
// ★ 추가 끝 ★


// CMFCproject1Dlg 대화 상자
class CMFCproject1Dlg : public CDialogEx
{
	// 생성입니다.
public:
	CMFCproject1Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	// ★ 추가 시작 (변수와 함수들을 선언합니다) ★
public:
	CImage m_image;                        // 도화지 역할을 할 CImage 객체
	std::vector<CPoint> m_vPoints;         // 클릭한 세 점의 좌표를 저장할 배열
	int m_nDragIndex;                      // 현재 드래그 중인 점의 번호 (-1이면 드래그 아님)
	bool m_bIsThreadRunning;               // 랜덤 이동 스레드가 작동 중인지 체크하는 플래그

	// 이미지 제어 및 화면 갱신 함수
	void UpdateDisplay();                  // CImage를 화면에 출력하는 함수
	void ClearImage();                     // 이미지를 흰색 배경으로 초기화하는 함수
	bool ValidImagePosition(int x, int y); // 픽셀 접근 전 좌표 유효성 검사

	// 순수 포인터 연산 그리기 함수 
	void DrawDot(int cx, int cy, int r, BYTE color);
	void DrawBigCircle(CPoint p1, CPoint p2, CPoint p3, int thickness, BYTE color);
	bool GetCircleCenter(CPoint p1, CPoint p2, CPoint p3, double& cpX, double& cpY, double& r);

	// 워커 스레드 함수 
	static UINT ThreadRandomMove(LPVOID pParam);

	// 컨트롤과 연결될 변수 
	int m_editRadius;    // 클릭 지점 원의 반지름 (사용자 입력)
	int m_editThickness; // 정원의 가장자리 두께 (사용자 입력)
	// ★ 추가 끝 ★

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCPROJECT1_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


	// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	// ★ 추가 시작 (마우스 이벤트 및 버튼 클릭 이벤트를 추가합니다) ★
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnRandom();
	// ★ 추가 끝 ★
};