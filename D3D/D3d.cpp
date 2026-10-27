#include "D3d.h"

MARGINS					Margin;
LPDIRECT3D9              g_pD3D = NULL;
LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
D3DPRESENT_PARAMETERS    g_d3dpp = {};
ID3DXLine* pLine = 0;
ID3DXFont* Font;

HWND RenderHwnd, GameHwnd;			//辅助窗口句柄，游戏窗口句柄
RECT WndBox;						//窗口矩形
int WndHight, WndWidth;			//窗口高，窗口宽

//注册窗口需要用到的窗口类
WNDCLASSEX wClass;

//画矩形，文字之类的单独放在这个函数里

Draw Render;

LRESULT WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_PAINT:
		if (g_pd3dDevice) {
			PStart();
			//这里调用我们的画框画线之类的函数
			Render();
			PEnd();
		}
		break;

	case WM_CREATE:
		DwmExtendFrameIntoClientArea(hWnd, &Margin);
		break;

	case WM_DESTROY:
	{
		g_pD3D->Release();
		g_pd3dDevice->Release();
		exit(1);
		return 0;
	}
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

bool InitD3D()//初始化D3d
{
	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return false;

	// 创建D3D设备
	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
	g_d3dpp.Windowed = TRUE;
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, RenderHwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
		return false;

	if (pLine == NULL)
		D3DXCreateLine(g_pd3dDevice, &pLine);

	//创建D3D字体
	D3DXCreateFontW(g_pd3dDevice, 20, 0, FW_DONTCARE, D3DX_DEFAULT, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"微软雅黑", &Font);

	return true;
}

HWND  CreateRenderWnd(HWND _GameHWnd, Draw _RenDer)//创建透明窗口
{
	if (_RenDer == NULL || _GameHWnd == 0) return NULL;

	GameHwnd = _GameHWnd;
	Render = _RenDer;

	//初始化窗口类
	wClass.cbClsExtra = NULL;
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.cbWndExtra = NULL;
	wClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	wClass.hCursor = LoadCursor(0, IDC_ARROW);
	wClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
	wClass.hInstance = GetModuleHandle(NULL);
	wClass.lpfnWndProc = (WNDPROC)WinProc;
	wClass.lpszClassName = L" ";
	wClass.lpszMenuName = L" ";
	wClass.style = CS_VREDRAW | CS_HREDRAW;
	//CS_VREDRAW：一旦移动或尺寸调整使客户区的高度发生变化，就重新绘制窗口。
	//CS_HREDRAW：一旦移动或尺寸调整使客户区的宽度发生变化，就重新绘制窗口

	//注册窗口
	if (RegisterClassEx(&wClass) == 0)
	{
		MessageBox(NULL, L"创建窗口出错！", L"提示！", 0);
		exit(1);
	}

	//创建窗口
	GetWindowRect(GameHwnd, &WndBox);
	WndWidth = WndBox.right - WndBox.left;
	WndHight = WndBox.bottom - WndBox.top;
	RenderHwnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, wClass.lpszClassName, wClass.lpszMenuName, WS_POPUP, 1, 1, WndWidth, WndHight, 0, 0, 0, 0);

	//显示窗口
	SetLayeredWindowAttributes(RenderHwnd, 0, RGB(0, 0, 0), LWA_COLORKEY);
	ShowWindow(RenderHwnd, SW_SHOW);

	//设置窗口属性 使其在任务栏隐藏
	SetWindowLong(RenderHwnd, GWL_EXSTYLE, (GetWindowLong(RenderHwnd, GWL_EXSTYLE) & (~WS_EX_APPWINDOW)) | WS_EX_TOOLWINDOW);
	InitD3D();
	return RenderHwnd;
}

void WndLoop()//窗口循环
{
	while (1)
	{
		//使辅助窗口一直盖在游戏窗口上
		if (GameHwnd)
		{

			GetWindowRect(GameHwnd, &WndBox);
			WndWidth = WndBox.right - WndBox.left;
			WndHight = WndBox.bottom - WndBox.top;
			DWORD dwStyle = GetWindowLong(GameHwnd, GWL_STYLE);
			if (dwStyle & WS_BORDER)
			{
				WndBox.top += 23;
				WndHight -= 23;
			}
			MoveWindow(RenderHwnd, WndBox.left, WndBox.top, WndWidth, WndHight, true);


			//esp开始

		}

		//处理窗口消息
		MSG Message;
		ZeroMemory(&Message, sizeof(Message));
		if (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&Message);
			TranslateMessage(&Message);
		}

		Sleep(1);
	}


	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
	if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
	CloseWindow(RenderHwnd);

	UnregisterClass(wClass.lpszClassName, wClass.hInstance);
	//修改游戏窗口的属性 不让他覆盖辅助窗口
	RECT GameRwnd{};
	GetWindowRect(GameHwnd, &GameRwnd);
	SetWindowPos(GameHwnd, HWND_NOTOPMOST, GameRwnd.left, GameRwnd.top, (GameRwnd.right - GameRwnd.left), (GameRwnd.bottom - GameRwnd.top), SWP_SHOWWINDOW);

}

void PLine(D3DCOLOR Color, float X1, float Y1, float X2, float Y2, float Width)//画线
{
	D3DXVECTOR2 Vertex[2] = { {X1,Y1},{X2,Y2} };
	pLine->SetWidth(Width);
	pLine->Draw(Vertex, 2, Color);
}

void Ptext(float X, float Y, const char* Str, D3DCOLOR Color)//写字
{
	RECT Rect = { (LONG)X,(LONG)Y };
	Font->DrawTextA(NULL, Str, -1, &Rect, DT_CALCRECT, Color);
	Font->DrawTextA(NULL, Str, -1, &Rect, DT_LEFT, Color);
}

void PBox(float X, float Y, float W, float H, float Width, D3DCOLOR Color)//画框
{
	D3DXVECTOR2 Vertex[5] = { {X,Y},{X + W,Y},{X + W,Y + H},{X,Y + H},{X,Y} };
	pLine->SetWidth(Width);
	pLine->Draw(Vertex, 5, Color);
}

void PStart()//绘制开始
{
	g_pd3dDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
	g_pd3dDevice->BeginScene();
}

void PEnd()//绘制结束
{
	g_pd3dDevice->EndScene();
	g_pd3dDevice->Present(0, 0, 0, 0);
}

D3DCOLOR RED = D3DCOLOR_ARGB(255, 255, 0, 0);//红色
D3DCOLOR GREEN = D3DCOLOR_ARGB(255, 0, 255, 0);//绿色
D3DCOLOR BLUE = D3DCOLOR_ARGB(255, 0, 0, 255);//蓝色
