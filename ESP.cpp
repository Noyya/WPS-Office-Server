#include "ESP.h"
#include "Engine.h"
#include "DriverMem/Mem.h"
#include "emmintrin.h"
#include "Log.h"

extern Mem* mem;
extern int WndHight;
extern int WndWidth;

D3DMATRIX ESP::_GetMatrix()
{
	D3DMATRIX matrix = mem->ReadMemory<D3DMATRIX>(m_Matrix);
	return matrix;
}

bool ESP::WorldToScreen(const D3DMATRIX& viewWorld, const Vector3& TargerPos, Vector2* result)
{
	int sightX = WndWidth / 2;
	int sightY = WndHight / 2;
	if (sightX <= 0 || sightY <= 0) {
		FERRYLOG(FerryLog::ERR, "SightX or SightY less than Zero! SightX[%d],SightY[%d]", sightX, sightY);
		return false;
	}
	float fViewW = viewWorld._14 * TargerPos.x + viewWorld._24 * TargerPos.y + viewWorld._34 * TargerPos.z + viewWorld._44;
	if (fViewW < 0.01f) { return false; }
	fViewW = 1 / fViewW;
	float fBoxX = sightX + (viewWorld._11 * TargerPos.x + viewWorld._21 * TargerPos.y + viewWorld._31 * TargerPos.z + viewWorld._41) * fViewW * sightX;
	float fBoxY = sightY - (viewWorld._12 * TargerPos.x + viewWorld._22 * TargerPos.y + viewWorld._32 * TargerPos.z + viewWorld._42) * fViewW * sightY;
	result->x = fBoxX;
	result->y = fBoxY;
	return true;
}
Vector3 ESP::GetBonesPosition(UINT64 boneItems, uint32_t i) {
	Vector3 res{ 0,0,0 };
	UINT64 Transform = mem->ReadChain(boneItems + 0x20 + i * 8, 2, 0, 0x10);
	if (Transform > 0x100000000) {
		res = CalculateGlobalPosition(Transform);
	}
	return res;
}
void ESP::DrawBones(const D3DMATRIX& matrix, UINT64 player, D3DCOLOR color)
{
	UINT64 boneNameItems = mem->ReadChain(player, 2, 0xB8, 0x48);
	Vector3 头部_3D = GetBonesPosition(boneNameItems, HEAD);
	Vector3 颈部_3D = GetBonesPosition(boneNameItems, NECK);
	Vector3 左肩膀_3D = GetBonesPosition(boneNameItems, LARM);
	Vector3 左手肘_3D = GetBonesPosition(boneNameItems, LHAND);
	Vector3 左手_3D = GetBonesPosition(boneNameItems, LHAND1);
	Vector3 右肩膀_3D = GetBonesPosition(boneNameItems, RARM);
	Vector3 右手肘_3D = GetBonesPosition(boneNameItems, RHAND);
	Vector3 右手_3D = GetBonesPosition(boneNameItems, RHAND1);
	Vector3 髋部_3D = GetBonesPosition(boneNameItems, HIP);
	Vector3 左脚_3D = GetBonesPosition(boneNameItems, LFOOT);
	Vector3 左膝_3D = GetBonesPosition(boneNameItems, LKNEE);
	Vector3 右脚_3D = GetBonesPosition(boneNameItems, RFOOT);
	Vector3 右膝_3D = GetBonesPosition(boneNameItems, RKNEE);

	Vector2 头_2D{};
	Vector2 颈_2D{};
	Vector2 左肩膀_2D{};
	Vector2 左手肘_2D{};
	Vector2 左手_2D{};
	Vector2 右肩膀_2D{};
	Vector2 右手肘_2D{};
	Vector2 右手_2D{};
	Vector2 髋部_2D{};
	Vector2 左膝_2D{};
	Vector2 左脚_2D{};
	Vector2 右膝_2D{};
	Vector2 右脚_2D{};

	if (WorldToScreen(matrix, 头部_3D, &头_2D)) {
		if (WorldToScreen(matrix, 颈部_3D, &颈_2D)) {
			if (WorldToScreen(matrix, 左肩膀_3D, &左肩膀_2D)) {
				if (WorldToScreen(matrix, 左手肘_3D, &左手肘_2D)) {
					if (WorldToScreen(matrix, 左手_3D, &左手_2D)) {
						if (WorldToScreen(matrix, 右肩膀_3D, &右肩膀_2D)) {
							if (WorldToScreen(matrix, 右手肘_3D, &右手肘_2D)) {
								if (WorldToScreen(matrix, 右手_3D, &右手_2D)) {
									if (WorldToScreen(matrix, 髋部_3D, &髋部_2D)) {
										if (WorldToScreen(matrix, 左脚_3D, &左脚_2D)) {
											if (WorldToScreen(matrix, 左膝_3D, &左膝_2D)) {
												if (WorldToScreen(matrix, 右脚_3D, &右脚_2D)) {
													if (WorldToScreen(matrix, 右膝_3D, &右膝_2D)) {

														PBox(头_2D.x, 头_2D.y, 头_2D.y - 颈_2D.y, (头_2D.y - 颈_2D.y), 1, RED);
														PLine(color, 头_2D.x, 头_2D.y, 颈_2D.x, 颈_2D.y, 1);

														PLine(color, 颈_2D.x, 颈_2D.y, 左肩膀_2D.x, 左肩膀_2D.y, 1);
														PLine(color, 左肩膀_2D.x, 左肩膀_2D.y, 左手肘_2D.x, 左手肘_2D.y, 1);
														PLine(color, 左手肘_2D.x, 左手肘_2D.y, 左手_2D.x, 左手_2D.y, 1);

														PLine(color, 颈_2D.x, 颈_2D.y, 右肩膀_2D.x, 右肩膀_2D.y, 1);
														PLine(color, 右肩膀_2D.x, 右肩膀_2D.y, 右手肘_2D.x, 右手肘_2D.y, 1);
														PLine(color, 右手肘_2D.x, 右手肘_2D.y, 右手_2D.x, 右手_2D.y, 1);

														PLine(color, 颈_2D.x, 颈_2D.y, 髋部_2D.x, 髋部_2D.y, 1);
														PLine(color, 髋部_2D.x, 髋部_2D.y, 左膝_2D.x, 左膝_2D.y, 1);
														PLine(color, 髋部_2D.x, 髋部_2D.y, 右膝_2D.x, 右膝_2D.y, 1);

														PLine(color, 左膝_2D.x, 左膝_2D.y, 左脚_2D.x, 左脚_2D.y, 1);
														PLine(color, 右膝_2D.x, 右膝_2D.y, 右脚_2D.x, 右脚_2D.y, 1);
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

}
void ESP::GetBonesName(UINT64 player) {
	UINT64 boneNameItems = mem->ReadChain(player, 2, 0xb8, 0x50);//_keys
	UINT boneNameCount = mem->ReadMemory<UINT32>(boneNameItems + 0x18);
	for (int i = 0; i < boneNameCount; i++) {
		UINT64 pStr = mem->ReadChain(boneNameItems + i * 8 + 0x20, 1, 0x0) + 0x14;
		std::wstring name = mem->ReadStringW(pStr);
		printf("BoneName:%ws index:%d\n", name.c_str(), i);
	}
}

void ESP::InitData() {
	//TODO
	m_EntityList = mem->ReadChain(Engine::GetBaseNetWorkableIl2cppClass(), 5, 0xB8, 0, 0x10, 0x28, 0x18) + 0x20;//第一个元素地址
	uintptr_t tmp = Engine::GetBaseNetWorkableIl2cppClass();
	m_pEntityListCount = mem->ReadChain(tmp, 4, 0xB8, 0, 0x10, 0x28) + 0x10;
	m_Camera = Engine::EnumTaggedNodes("Main Camera");
	m_Matrix = (uintptr_t)mem->ReadChain(m_Camera, 2, 0x30, 0x18) + 0xDC;
	m_LocalPlayer = Engine::EnumTaggedNodes("LocalPlayer");
	m_BasePlayer = mem->ReadChain(m_LocalPlayer, 3, 0x30, 0x18, 0x28);
}

const Vector3 __fastcall ESP::CalculateGlobalPosition(
	UINT64 transform)
{
	uint64_t index; // r8
	UINT64 transformData1; // r11
	UINT64 transformData2; // rdx
	int transformIndex; // eax
	__m128 v10; // xmm10
	__m128 v11; // xmm4
	__m128 v12; // xmm2
	__m128 v13; // xmm8
	__m128 v14; // xmm6
	__m128 v15; // xmm3
	__m128 v16; // xmm1
	__m128 res;//arg1

	index = mem->ReadMemory <uint64_t>(transform + 0x40);
	UINT64 pTransformData = mem->ReadMemory <uint64_t>(transform + 0x38);
	if (pTransformData) {
		transformData1 = mem->ReadMemory<uint64_t>(pTransformData + 0x18);//transformAccess->hierarchy->localTransforms;
		transformData2 = mem->ReadMemory<uint64_t>(pTransformData + 0x20);//transformAccess->hierarchy->parentIndices;
		if (transformData1 && transformData2) {
			struct Matrix34 { BYTE vec0[16]; BYTE vec1[16]; BYTE vec2[16]; };
			const __m128 mulVec0 = { -2.0f,2.0f,-2.0f,0.0f };
			const __m128 mulVec1 = { -2.0f,-2.0f,2.0f,0.0f };
			const __m128 mulVec2 = { 2.0f,-2.0f,-2.0f,0.0f };
			transformIndex = mem->ReadMemory<int>(transformData2 + index * 4);
			res = mem->ReadMemory<__m128>(transformData1 + index * 0x30);//*result = transformData1[index].t
			while (transformIndex >= 0) {
				//v7 = transformIndex;
				Matrix34 matrix = mem->ReadMemory<Matrix34>(transformData1 + transformIndex * 0x30);//(__m128i)transformData1[v8].q;
				v10 = _mm_mul_ps(*(__m128*) & matrix.vec2, res);
				v11 = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*) & matrix.vec1, 85));
				v12 = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*) & matrix.vec1, 0));
				v13 = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*) & matrix.vec1, 170));
				v14 = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*) & matrix.vec1, 142));
				v15 = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*) & matrix.vec1, 219));
				v16 = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*) & matrix.vec1, 113));
				res = _mm_add_ps(
					_mm_add_ps(
						_mm_add_ps(
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps(_mm_mul_ps(v13, (__m128)mulVec1), v15),
									_mm_mul_ps(_mm_mul_ps(v12, (__m128)mulVec0), v16)),
								(__m128)_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v10), 85))),
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps(_mm_mul_ps(v12, (__m128)mulVec2), v14),
									_mm_mul_ps(_mm_mul_ps(v11, (__m128)mulVec1), v15)),
								(__m128)_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v10), 170)))),
						_mm_add_ps(
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps(_mm_mul_ps(v11, (__m128)mulVec0), v16),
									_mm_mul_ps(_mm_mul_ps(v13, (__m128)mulVec2), v14)),
								(__m128)_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v10), 0))),
							(__m128)v10)),
					*(__m128*) & matrix.vec0);
				transformIndex = mem->ReadMemory<int>(transformData2 + transformIndex * 4);
			}
			return Vector3(res.m128_f32[0], res.m128_f32[1], res.m128_f32[2]);
		}
	}

	return Vector3(0.0f, 0.0f, 0.0f);
}

//内置 - RVO优化
std::vector<EntityInfo> ESP::GetAllEntityInfo()
{
	uintptr_t count = mem->ReadMemory<uintptr_t>(m_pEntityListCount);
	std::vector<EntityInfo> ret;
	for (int i = 0; i < count; i++) {
		uintptr_t entity = mem->ReadMemory<uintptr_t>(m_EntityList + i * 8);
		if (m_BasePlayer == entity)
			continue;
		uintptr_t pClassName = mem->ReadChain(entity, 2, 0, 0x10);
		std::string ClassName = mem->ReadStringA(pClassName);
		if (ClassName.empty())
			continue;
		uintptr_t pParentClassName = mem->ReadChain(entity, 3, 0, 0x58, 0x10);
		std::string ParentClassName = mem->ReadStringA(pParentClassName);
		/*printf("%s\n", ClassName.c_str());*/
		UINT64 posAddress = mem->ReadChain(entity, 5, 0x10, 0x30, 0x30, 0x8, 0x38) + 0x90;
		Vector3 pos = mem->ReadMemory<Vector3>(posAddress);
		ret.emplace_back(pos, ClassName, ParentClassName, entity);
	}
	return ret;
}

