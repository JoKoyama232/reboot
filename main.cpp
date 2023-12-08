//=============================================================================
// メイン処理 [main.cpp]
// Author : 小山　城　小井詰　礼　岩田　輝
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "model.h"
#include "WorldSpaceClass.h"
#include "fade.h"
#include "title.h"
#include "result.h"
#include "sound.h"
#include "clock.h"
#include "game.h"
#include "Player.h"
#include "debris.h"
#include "antenna.h"
#include "Intro.h"
#include "light.h"
//シーン



// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// ウインドウのクラス名
#define WINDOW_NAME		"Flight"		// ウインドウのキャプション名

// カメラ位置定数群
#define CAMERA_OFFSET_BACK	 (100.0f)	// プレイヤーからの後ろへのカメラのオフセット
#define CAMERA_OFFSET_RIGHT	 (30.0f)	// プレイヤーからの右へのカメラのオフセット
#define CAMERA_OFFSET_HEIGHT (15.0f)	// プレイヤーからの上へのカメラのオフセット
#define CAMERA_PITCH_RADIUSZ (30.0f)	// プレイヤーからのオフセット位置を軸としたx軸の楕円の横距離
#define CAMERA_PITCH_RADIUSY (100.0f)	// プレイヤーからのオフセット位置を軸としたy軸の楕円の横距離

// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);


// グローバル変数:
//*****************************************************************************
long g_MouseX = 0;
long g_MouseY = 0;
bool firstLoad = true;
tagRECT window;


#ifdef _DEBUG
int		g_CountFPS;							// FPSカウンタ
char	g_DebugStr[2048] = WINDOW_NAME;		// デバッグ文字表示用

#endif

int	g_Mode = MODE_INTRO;					// 起動時の画面を設定


// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	// 時間計測用
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX	wcex = {
 		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND		hWnd;
	MSG			msg;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,																		// ウィンドウの左座標
		CW_USEDEFAULT,																		// ウィンドウの上座標
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,									// ウィンドウ横幅
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// ウィンドウ縦幅
		NULL,
		NULL,
		hInstance,
		NULL);

	// 初期化処理(ウィンドウを作成してから行う)
	if (FAILED(Init(hInstance, hWnd, true)))
	{
		return -1;
	}

	// フレームカウント初期化
	timeBeginPeriod(1);	// 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1秒ごとに実行
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
				dwFrameCount = 0;							// カウントをクリア
				AddClock(1);
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;	// 処理した時刻を保存

#ifdef _DEBUG	// デバッグ版の時だけFPSを表示する
				wsprintf(g_DebugStr, WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
#endif
				RECT window;
				GetWindowRect(hWnd, &window);
				SetWindowInfo(window);
				Update();			// 更新処理
				Draw();				// 描画処理

#ifdef _DEBUG	// デバッグ版の時だけ表示する
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " MX:%d MY:%d", GetMousePosX(), GetMousePosY());
				SetWindowText(hWnd, g_DebugStr);
#endif

				dwFrameCount++;
			}
		}
	}

	timeEndPeriod(1);				// 分解能を戻す

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	return (int)msg.wParam;
}

// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_MOUSEMOVE:
		g_MouseX = LOWORD(lParam);
		g_MouseY = HIWORD(lParam);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}


// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	InitRenderer(hInstance, hWnd, bWindow);

	InitLight();
	
	InitCamera();

	// 入力処理の初期化
	InitInput(hInstance, hWnd);
	//-----
	InitFade();

	//サウンドの初期化処理
	InitSound(hWnd);

	// ライトを有効化
	SetLightEnable(true);

	// 背面ポリゴンをカリング
	SetCullingMode(CULL_MODE_BACK);
	SetMode(g_Mode);

	return S_OK;
}

// 終了処理
//=============================================================================
void Uninit(void)
{
	
	UninitFade();
	// カメラの終了処理
	UninitCamera();

	// サウンドの終了処理
	UninitSound();

	//入力の終了処理
	UninitInput();

	// レンダラーの終了処理
	UninitRenderer();
}

// 更新処理
//=============================================================================
void Update(void)
{
	UpdateLight();

	// 入力の更新処理
	UpdateInput();

	// カメラ更新
	UpdateCamera();

	switch (g_Mode) {
	case MODE_INTRO:
		UpdateIntro();
		break;
	case MODE_TITLE:
		UpdateTitle();
		break;
	case MODE_GAME:
		UpdateGame();
		break;
	case MODE_RESULT:
		UpdateResult();
		break;

	}

	UpdateFade();


}

// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファクリア
	Clear();
	SetViewPort(TYPE_FULL_SCREEN);
	switch (g_Mode) {
	case MODE_INTRO:
		SetCamera();
		DrawIntro();
		break;
	case MODE_TITLE:
		SetCamera();
		DrawTitle();
		break;
	case MODE_GAME:
		// プレイヤー視点
		XMFLOAT3 pos = GetPlayer()->object.GetPositionFloat();
		XMFLOAT3 rot = GetCamera()->rot;
		pos.x += -sinf(rot.y) * (CAMERA_OFFSET_BACK + sinf(rot.x) * CAMERA_PITCH_RADIUSZ) + cosf(rot.y) * CAMERA_OFFSET_RIGHT;
		pos.z += -cosf(rot.y) * (CAMERA_OFFSET_BACK + sinf(rot.x) * CAMERA_PITCH_RADIUSZ) - sinf(rot.y) * CAMERA_OFFSET_RIGHT;
		pos.y += -sinf(rot.x) * CAMERA_PITCH_RADIUSY + CAMERA_OFFSET_HEIGHT;

		SetCameraPos(pos);
		SetCamera();
		DrawGame();
		break;
	case MODE_RESULT:
		SetCamera();
		DrawResult();
		break;

	}
	//SetViewPort(TYPE_RIGHT_HALF_SCREEN);
	DrawFade();

#ifdef _DEBUG
	// デバッグ表示
	DrawDebugProc();
#endif

	// バックバッファ、フロントバッファ入れ替え
	Present();
}


long GetMousePosX(void)
{
	return g_MouseX;
}


long GetMousePosY(void)
{
	return g_MouseY;
}


#ifdef _DEBUG
char* GetDebugStr(void)
{
	return g_DebugStr;
}
#endif





void SetMode(int mode) {
	g_Mode = mode;
	UninitIntro();

	// タイトル画面の終了処理
	UninitTitle();

	//ゲームシーンの終了処理
	UninitGame();

	//リザルト処理の終了処理
	UninitResult();

	switch (g_Mode) {
	case MODE_INTRO:
		InitIntro();
		break;
	case MODE_TITLE:
		InitTitle();
		PlaySound(SOUND_LABEL_BGM_title);
		break;
	case MODE_GAME:
		InitGame();
		PlaySound(SOUND_LABEL_BGM_stage1);
		break;
	case MODE_RESULT:
		InitResult();
		
		break;
	}
}

int	GetMode(void) {
	return g_Mode;
}

bool IsFirstLoad(void) {
	return firstLoad;
}

void setLoad(bool update) {
	firstLoad = update;
}

