#include "windows.h"
#include "Engine.h"
#include "ESP.h"
#include "DriverMem/Mem.h"
#include "Log.h"
#include "D3D/D3d.h"
#include "atlconv.h"
#include "atlstr.h"
#include "math.h"
Mem* mem = &Mem::GetInstance();
ESP esp;

void WPSRender() {
	//取本人坐标
	UINT64 posAddress = mem->ReadChain(esp.m_BasePlayer, 5, 0x10, 0x30, 0x30, 0x8, 0x38) + 0x90;
	Vector3 playerPos = mem->ReadMemory<Vector3>(posAddress);

	//esp
	std::vector<EntityInfo> res = esp.GetAllEntityInfo();
	Vector2 out{};
	D3DMATRIX matrix = esp._GetMatrix();
	char buf[128]{ 0 };
	for (auto& i : res) {
		if (!esp.WorldToScreen(matrix, i.pos, &out))
			continue;
		float dis = sqrt((playerPos.x - i.pos.x) * (playerPos.x - i.pos.x) + (playerPos.y - i.pos.y) * (playerPos.y - i.pos.y) + (playerPos.z - i.pos.z)
			* (playerPos.z - i.pos.z));
		//方框
		if (i.parent_name.find("Human") != std::string::npos) {
			Vector2 outHead{};
			esp.WorldToScreen(matrix, { i.pos.x,i.pos.y + 1.8f ,i.pos.z }, &outHead);
			float h = out.y - outHead.y;
			float w = h / 2;
			PBox(out.x - w / 2, outHead.y, w, h, 1, GREEN);
			PLine(RED, WndWidth / 2, 0, outHead.x, outHead.y, 1);
		}

		//物资
		if (dis <= 30) {
			sprintf_s(buf, sizeof(buf), "%s [%.0lf]m", i.name.c_str(), dis);
			Ptext(out.x, out.y, buf, RED);
		}

		//骨骼
		if ((i.name.find("BasePlayer") != std::string::npos)
			|| (i.parent_name.find("HumanNPC") != std::string::npos)) {
			//esp.GetBonesName(i.address);
			esp.DrawBones(matrix, i.address, RED);
		}

	}

}

int main() {
	char windowName[] = "Rust";
	HWND gameWnd = FindWindowA(NULL, windowName);
	HWND renderWnd = CreateRenderWnd(gameWnd, WPSRender);
	bool ret = Engine::InitEngine(windowName);
	esp.InitData();
	WndLoop();
	system("pause");
	return 0;
}