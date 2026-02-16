#include "sdk.h"

void CGame::update() {
	getMap();
	//local
	getLocalPlayer();
	getLocalPawn();
	getLocalTeam();
	getLocalPos();
	getLocalName();

	getEntityList();
	//Entities
	getPlayers();
	getName();
	getTeam();
	getHealth();
	getColor();
	getAngles();
	getPosition();
}

//Game
void CGame::getMap() {
	uintptr_t mBase = DMADevice::getModuleBase(_strdup("matchmaking.dll"));
	uintptr_t mapPtr = DMADevice::MemReadPtr<uint64_t>(mBase + matchmaking_dll::dwGameTypes + matchmaking_dll::dwGameTypes_mapName);
	DMADevice::MemRead(mapPtr + 0x4, &map, sizeof(map));
}
void CGame::getEntityList() {
	entityList = DMADevice::MemReadPtr<uint64_t>(DMADevice::moduleBase + client_dll::dwEntityList);
}

//Local
void CGame::getLocalPlayer() {
	localPlayer.controller = DMADevice::MemReadPtr<uint64_t>(DMADevice::moduleBase + client_dll::dwLocalPlayerController);
}
void CGame::getLocalPawn() {
	localPlayer.pawn = DMADevice::MemReadPtr<uint64_t>(DMADevice::moduleBase + client_dll::dwLocalPlayerPawn);
}
void CGame::getLocalTeam() {
	localPlayer.teamID = DMADevice::MemReadPtr<uint8_t>(localPlayer.controller + client_dll::m_iTeamNum);
}
void CGame::getLocalPos() {
	DMADevice::MemRead(localPlayer.pawn + client_dll::m_vOldOrigin, &localPlayer.position, sizeof(Vector3));
}
void::CGame::getLocalName() {
	localPlayer.nameAddr = DMADevice::MemReadPtr<uint64_t>(localPlayer.controller + client_dll::dwSanitizedName);
	DMADevice::MemRead(localPlayer.nameAddr, &localPlayer.name, sizeof(char[36]));
}

//Players
void CGame::getPlayers() {
	//listEntry
	for (int i = 1; i <= 64; i++) {
		DMADevice::PrepareEX(DMADevice::hScatter, entityList + (0x8 * (i >> 9) + 16), &players[i - 1].listEntry, sizeof(uint64_t));
	}
	DMADevice::ExecuteRead(DMADevice::hScatter);
	DMADevice::Clear(DMADevice::hScatter);

	//Controller
	for (int i = 1; i <= 64; i++) {
		DMADevice::PrepareEX(DMADevice::hScatter, players[i - 1].listEntry + 0x70 * (i & 0x1FF), &players[i - 1].controller, sizeof(uint64_t));
	}
	DMADevice::ExecuteRead(DMADevice::hScatter);
	DMADevice::Clear(DMADevice::hScatter);

	//PawnAddr
	for (int i = 1; i <= 64; i++) {
		DMADevice::PrepareEX(DMADevice::hScatter, players[i - 1].controller + client_dll::m_hPlayerPawn, &players[i - 1].pawnAddr, sizeof(uint32_t));
	}
	DMADevice::ExecuteRead(DMADevice::hScatter);
	DMADevice::Clear(DMADevice::hScatter);

	//listEntry2
	for (int i = 1; i <= 64; i++) {
		DMADevice::PrepareEX(DMADevice::hScatter, entityList + 0x8 * ((players[i - 1].pawnAddr & 0x7FFF) >> 9) + 16, &players[i - 1].listEntry2, sizeof(uint64_t));
	}
	DMADevice::ExecuteRead(DMADevice::hScatter);
	DMADevice::Clear(DMADevice::hScatter);

	//Pawn
	for (int i = 1; i <= 64; i++) {
		DMADevice::PrepareEX(DMADevice::hScatter, players[i - 1].listEntry2 + 0x70 * (players[i - 1].pawnAddr & 0x1FF), &players[i - 1].pawn, sizeof(uint64_t));
	}
	DMADevice::ExecuteRead(DMADevice::hScatter);
	DMADevice::Clear(DMADevice::hScatter);
}
void CGame::getName() {
	for (int i = 1; i <= 64; i++) {
		DMADevice::PrepareEX(DMADevice::hScatter, players[i - 1].controller + client_dll::dwSanitizedName, &players[i - 1].nameAddr, sizeof(uint64_t));
	}
	DMADevice::ExecuteRead(DMADevice::hScatter);
	DMADevice::Clear(DMADevice::hScatter);

	for (int i = 1; i <= 64; i++) {
		DMADevice::PrepareEX(DMADevice::hScatter, players[i - 1].nameAddr, &players[i - 1].name, sizeof(char[36]));
	}
	DMADevice::ExecuteRead(DMADevice::hScatter);
	DMADevice::Clear(DMADevice::hScatter);
}
void CGame::getTeam() {
	for (int i = 1; i <= 64; i++) {
		DMADevice::PrepareEX(DMADevice::hScatter, players[i - 1].controller + client_dll::m_iTeamNum, &players[i - 1].teamID, sizeof(uint64_t));
	}
	DMADevice::ExecuteRead(DMADevice::hScatter);
	DMADevice::Clear(DMADevice::hScatter);
}
void CGame::getHealth() {
	for (int i = 1; i <= 64; i++) {
		DMADevice::PrepareEX(DMADevice::hScatter, players[i - 1].pawn + client_dll::m_iHealth, &players[i - 1].health, sizeof(uint32_t));
	}
	DMADevice::ExecuteRead(DMADevice::hScatter);
	DMADevice::Clear(DMADevice::hScatter);
}
void CGame::getColor() {
	for (int i = 1; i <= 64; i++) {
		DMADevice::PrepareEX(DMADevice::hScatter, players[i - 1].controller + client_dll::m_iCompTeammateColor, &players[i - 1].color, sizeof(uint64_t));
	}
	DMADevice::ExecuteRead(DMADevice::hScatter);
	DMADevice::Clear(DMADevice::hScatter);
}
void CGame::getAngles() {
	for (int i = 1; i <= 64; i++) {
		DMADevice::PrepareEX(DMADevice::hScatter, players[i - 1].pawn + client_dll::m_angEyeAngles, &players[i - 1].eyeAngles, sizeof(Vector2));
	}
	DMADevice::ExecuteRead(DMADevice::hScatter);
	DMADevice::Clear(DMADevice::hScatter);
}
void CGame::getPosition() {
	for (int i = 1; i <= 64; i++) {
		DMADevice::PrepareEX(DMADevice::hScatter, players[i - 1].pawn + client_dll::m_vOldOrigin, &players[i - 1].position, sizeof(Vector3));
	}
	DMADevice::ExecuteRead(DMADevice::hScatter);
	DMADevice::Clear(DMADevice::hScatter);
}