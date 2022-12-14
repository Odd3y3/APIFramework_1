#include "CCore.h"
#include "Scene/SceneManager.h"
#include "Core/Timer.h"
#include "Core/PathManager.h"
#include "Resource/ResourceManager.h"

CCore* CCore::m_pInst = NULL;
bool CCore::m_bLoop = true;

CCore::CCore()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(282);
}

CCore::~CCore()
{
    DESTROY_SINGLE(CSceneManager);
    DESTROY_SINGLE(CResourceManager);
    DESTROY_SINGLE(CPathManager);
    DESTROY_SINGLE(CTimer);

    ReleaseDC(m_hWnd, m_hDC);
}

bool CCore::Init(HINSTANCE hInst)
{
	m_hInst = hInst;

    MyRegisterClass();

    // 해상도 설정
    m_tRS.iW = 1280;
    m_tRS.iH = 720;

    Create();

    //화면 DC를 만들어준다
    m_hDC = GetDC(m_hWnd);

    //타이머 초기화
    if (!GET_SINGLE(CTimer)->Init())
        return false;

    //경로관리자 초기화
    if (!GET_SINGLE(CPathManager)->Init())
        return false;

    //리소스 관리자 초기화
    if (!GET_SINGLE(CResourceManager)->Init(hInst, m_hDC))
        return false;

    //장면관리자 초기화
    if (!GET_SINGLE(CSceneManager)->Init())
        return false;

	return true;
}

int CCore::Run()
{
    MSG msg;

    // 기본 메시지 루프입니다:
    while (m_bLoop)
    {
        //PeekMessage는 메세지가 메세지큐에 없어도 바로 빠져나가고
        //메세지가 있으면 true, 없으면 false
        //메세지가 없는경우가 윈도우의 데드타임
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        //윈도우 데드타임 일경우.
        else
        {
            Logic();
        }
    }

    return (int)msg.wParam;
}

void CCore::Logic()
{
    //타이머 갱신
    GET_SINGLE(CTimer)->Update();

    float   fDeltaTime = GET_SINGLE(CTimer)->GetDeltaTime();

    Input(fDeltaTime);
    Update(fDeltaTime);
    LateUpdate(fDeltaTime);
    Collision(fDeltaTime);
    Render(fDeltaTime);
}

void CCore::Input(float fDeltaTime)
{
    GET_SINGLE(CSceneManager)->Input(fDeltaTime);
}

int CCore::Update(float fDeltaTime)
{
    GET_SINGLE(CSceneManager)->Update(fDeltaTime);
    return 0;
}

int CCore::LateUpdate(float fDeltaTime)
{
    GET_SINGLE(CSceneManager)->LateUpdate(fDeltaTime);
    return 0;
}

void CCore::Collision(float fDeltaTime)
{
    GET_SINGLE(CSceneManager)->Collision(fDeltaTime);
}

void CCore::Render(float fDeltaTime)
{
    GET_SINGLE(CSceneManager)->Render(m_hDC, fDeltaTime);
}

ATOM CCore::MyRegisterClass()
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = CCore::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInst;
    wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ICON2));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_WINAPI2);
    wcex.lpszClassName = TEXT("APIFramework_1");
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON2));

    return RegisterClassExW(&wcex);
}

BOOL CCore::Create()
{
    m_hWnd = CreateWindowW(TEXT("APIFramework_1"), TEXT("LOST ARK (64-bit, DX11) v.2어쩌구 저쩌구"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, nullptr);

    if (!m_hWnd)
    {
        return FALSE;
    }

    //실제 윈도우 타이틀바나 메뉴를 포함한 윈도우의 크기를 구해준다.
    RECT rc = { 0,0,m_tRS.iW,m_tRS.iH };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    //위에서 구해준 크기로 윈도우 클라이언트 영역의 크기를 원하는 크기로 맞춰줘야함.
    SetWindowPos(m_hWnd, HWND_TOPMOST, 100, 100, rc.right - rc.left,
        rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);

    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);

    return TRUE;
}

LRESULT CCore::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
    }
    break;
    // 윈도우 종료 시킬 때 들어오는 메세지
    case WM_DESTROY:
        m_bLoop = false;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}