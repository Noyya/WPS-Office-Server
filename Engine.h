#pragma once
#include "iostream"
#include "Windows.h"

#define OFFSET 0x33C4620;
#define GOM_OFFSET 0x01AD8580;

class ListElement {
public:
	UINT64 m_Prev;
	UINT64 m_Next;
};

class ListNode : public ListElement {
public:
	UINT64 m_Data;
};

class List {
public:
	ListElement m_Root;
};

class GameObject {
public:
	char pad_0000[0x30];
	UINT64 componentList;				//0x30 
	char pad_0001[0x8];					//0x38
	int size;							//0x40 
	char pad_0002[0x1C];				//0x44
	char* name;							//0x60 
};
class Slot {
public:
	DWORD hashValue;									//0x00
	DWORD unknown1;										//0x04
	DWORD tagValue;										//0x08
	DWORD unknown2;										//0xC
	UINT64 taggedNode;									//0x10
};
#pragma pack(push,4)
class HashMap {
public:
	UINT64 Slots;									//0x00
	DWORD mask;										//0x08
	DWORD size;										//0x0c
	char pad_0000[0x8];								//0x10
};
#pragma pack(pop)
class GameObjectManager {
public:
	HashMap TaggedNodes;		//0x00	
	char pad_0000[0x8];			//0x18
	List ActivedNodes;			//0x20  
};

namespace Engine {
	bool InitEngine(char* windowName);
	uintptr_t GetBaseNetWorkableIl2cppClass();
	uintptr_t EnumTaggedNodes(std::string objName);
	uintptr_t EnumActiveNodes(std::string objName);
}

