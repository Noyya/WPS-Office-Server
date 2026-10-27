#pragma once
#include "stdint.h"
#include "windows.h"
#include <string>
#include <vector>
#include "D3D/D3d.h"

#define HEAD  47
#define NECK  46

#define LARM 24
#define LHAND 25
#define LHAND1  26

#define RARM 55
#define RHAND 56
#define RHAND1  57

#define HIP  0

#define LFOOT  3
#define LKNEE 2

#define RFOOT  15
#define RKNEE 14

struct Vector3 {

	Vector3() :x(0), y(0), z(0) {}
	Vector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	float x;
	float y;
	float z;
};
struct EntityInfo {
	Vector3 pos;
	std::string name;
	std::string parent_name;
	UINT64 address;
	EntityInfo(Vector3 _pos, std::string _name, std::string _parent_name, UINT64 _address) :pos(_pos), name(_name), parent_name(_parent_name), address(_address) {}
};
struct Vector2 {
	Vector2(float x, float y) {
		this->x = x;
		this->y = y;
	}
	Vector2() :x(0), y(0) {}
	float x;
	float y;

};

typedef struct TRect
{
	int x;
	int y;
	int w;
	int h;
}MRect;

class ESP {

public:
	ESP() : m_EntityList(0), m_Camera(0), m_pEntityListCount(0), m_BasePlayer(0) {}
	UINT64 m_EntityList;
	UINT64 m_Camera;
	UINT64 m_pEntityListCount;
	UINT64 m_Matrix;
	UINT64 m_LocalPlayer;	//游戏对象
	UINT64 m_BasePlayer;	//玩家的脚本对象
private:

public:
	void InitData();
	std::vector<EntityInfo> GetAllEntityInfo();
	const Vector3 __fastcall CalculateGlobalPosition(UINT64 transform);
	D3DMATRIX _GetMatrix();
	bool WorldToScreen(const D3DMATRIX& viewWorld, const Vector3& TargerPos, Vector2* result);
	void DrawBones(const D3DMATRIX& matrix, UINT64 player, D3DCOLOR color);
	void GetBonesName(UINT64 player);
	Vector3 GetBonesPosition(UINT64 player, uint32_t i);
};