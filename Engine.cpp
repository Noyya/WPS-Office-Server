#include "Engine.h"
#include "Log.h"
#include "DriverMem/Mem.h"

extern Mem* mem;

namespace Engine {

	uintptr_t BaseNetWorkableIl2cppClass = NULL;
	uintptr_t GOM = NULL;

	uintptr_t EnumTaggedNodes(std::string objName) {

		GameObjectManager Gom = mem->ReadMemory<GameObjectManager>(GOM);
		FERRYLOG(FerryLog::Level::WARN, "Hash Map Enumerating...");
		UINT64 Slots = Gom.TaggedNodes.Slots;
		size_t count = 0;
		for (int i = 0;; i++) {
			Slot slot{};
			if (count >= Gom.TaggedNodes.size)
				break;
			//g_Process.memory().Read((blackbone::ptr_t)Slots + i * 0x18, sizeof(slot), &slot);
			slot = mem->ReadMemory<Slot>(Slots + i * 0x18);
			if (slot.hashValue < -1) {
				ListNode node{};
				UINT64 pNode;
				GameObject data{};
				pNode = mem->ReadMemory<UINT64>(slot.taggedNode + 8);
				node = mem->ReadMemory<ListNode>(pNode);
				data = mem->ReadMemory<GameObject>(node.m_Data);
				std::string name = mem->ReadStringA((uintptr_t)data.name);
				FERRYLOG(FerryLog::Level::ACCESS, "Node %llx Name %s", slot.taggedNode, name.c_str());
				count++;
				if (name.find(objName) != std::string::npos) {
					FERRYLOG(FerryLog::Level::WARN, "Find GameObject %s Success! address:%p", objName.c_str(), node.m_Data);
					return node.m_Data;
				}
			}
		}
		FERRYLOG(FerryLog::Level::ERR, "Find GameObject %s Failed!", objName.c_str());
		return NULL;
	}
	uintptr_t EnumActiveNodes(std::string objName) {
		FERRYLOG(FerryLog::Level::WARN, "ListNode Enumerating...");
		UINT64 startNode{};
		ListNode curNode{};

		// Read(pGameObjectManager, sizeof(GameObjectManager), &GOM);
		GameObjectManager Gom = mem->ReadMemory<GameObjectManager>(GOM);
		curNode = mem->ReadMemory<ListNode>(Gom.ActivedNodes.m_Root.m_Next);
		//g_Process.memory().Read((blackbone::ptr_t)GOM.ActivedNodes.m_Root.m_Next, sizeof(curNode), &curNode);

		startNode = Gom.ActivedNodes.m_Root.m_Next;
		FERRYLOG(FerryLog::Level::WARN, "start Node:%p", startNode);
		do {
			GameObject data{};
			//g_Process.memory().Read((blackbone::ptr_t)curNode.m_Data, sizeof(data), &data);
			data = mem->ReadMemory<GameObject>(curNode.m_Data);

			//g_Process.memory().Read((blackbone::ptr_t)data.name, sizeof(buf), buf);
			std::string name = mem->ReadStringA((uintptr_t)data.name);

			FERRYLOG(FerryLog::Level::ACCESS, "%p %s", data.name, name.c_str());

			if (name.find(objName) != std::string::npos) {
				FERRYLOG(FerryLog::Level::WARN, "Find GameObject %s Success! address:%p", objName.c_str(), curNode.m_Data);
				return curNode.m_Data;
			}
			//g_Process.memory().Read((blackbone::ptr_t)curNode.m_Next, sizeof(curNode), &curNode);
			curNode = mem->ReadMemory<ListNode>(curNode.m_Next);
			if (!curNode.m_Next)
				break;
		} while (((UINT64)curNode.m_Next) != startNode);
		FERRYLOG(FerryLog::Level::ERR, "Find GameObject %s Failed!", objName.c_str());
		return NULL;
	}
	bool InitEngine(char* windowName) {

		char moduleName2[] = "GameAssembly.dll";
		bool bret = mem->LoadDriver(windowName, moduleName2);
		if (!bret) {
			FERRYLOG(FerryLog::Level::ERR, "Load Driver Failed!");
			system("pause");
			return false;
		}
		uintptr_t pTypeInfo = mem->moduleBase + OFFSET;
		BaseNetWorkableIl2cppClass = mem->ReadMemory<uintptr_t>(pTypeInfo);

		char moduleName[] = "UnityPlayer.dll";
		UINT64 module = mem->GetModulebaseEx(moduleName) + GOM_OFFSET;
		GOM = mem->ReadMemory<UINT64>(module);
		if (!bret) {
			FERRYLOG(FerryLog::Level::ERR, "GetModulebaseEx Failed!");
			system("pause");
			return false;
		}
		return true;
	}
	uintptr_t GetBaseNetWorkableIl2cppClass() {
		return BaseNetWorkableIl2cppClass ? BaseNetWorkableIl2cppClass : NULL;
	}


}