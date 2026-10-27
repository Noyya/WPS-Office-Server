#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#include <iostream>
#include <Tlhelp32.h>
#include <tchar.h>
#include <time.h>
#include <d3d9helper.h>


extern MARGINS					Margin;
extern LPDIRECT3D9              g_pD3D;
extern LPDIRECT3DDEVICE9        g_pd3dDevice;
extern D3DPRESENT_PARAMETERS    g_d3dpp;
extern ID3DXLine* pLine;
extern ID3DXFont* Font;

extern HWND RenderHwnd, GameHwnd;			//辅助窗口句柄，游戏窗口句柄
extern RECT WndBox;						//窗口矩形
extern int WndHight, WndWidth;			//窗口高，窗口宽

//注册窗口需要用到的窗口类
extern WNDCLASSEX wClass;


//画矩形，文字之类的单独放在这个函数里
typedef void(*Draw)();
extern Draw Render;


//窗口消息处理函数
LRESULT WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

bool InitD3D();//初始化D3d

HWND CreateRenderWnd(HWND GameHwnd, Draw 绘制函数);//创建透明窗口

void WndLoop();//窗口循环

void PLine(//划线
	D3DCOLOR	Color,		//颜色
	float		X1,			//开始X轴
	float		Y1,			//开始Y轴
	float		X2,			//结束X轴
	float		Y2,			//结束Y轴
	float		Width		//线粗
);

void Ptext(//写字
	float		X,			//坐标X
	float		Y,			//坐标Y
	const char* Str,		//文字
	D3DCOLOR	Color		//颜色
);

void PBox(//画框
	float		X,			//坐标X
	float		Y,			//坐标Y
	float		W,			//框宽
	float		H,			//框高
	float		Width,		//线粗
	D3DCOLOR	Color		//颜色
);

void PStart();//绘制开始

void PEnd();//绘制结束

extern D3DCOLOR RED;//红色
extern D3DCOLOR GREEN;//绿色
extern D3DCOLOR BLUE;//蓝色