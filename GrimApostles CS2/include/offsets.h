#pragma once
#include <iostream>

namespace client_dll { // client.dll
	//These need changed every update
    constexpr std::ptrdiff_t dwCSGOInput = 0x2314910;
    constexpr std::ptrdiff_t dwEntityList = 0x21C6FF0;
    constexpr std::ptrdiff_t dwGameEntitySystem = 0x24AA0D8;
    constexpr std::ptrdiff_t dwGameEntitySystem_highestEntityIndex = 0x20A0;
    constexpr std::ptrdiff_t dwGameRules = 0x2308DA0;
    constexpr std::ptrdiff_t dwGlobalVars = 0x20595D0;
    constexpr std::ptrdiff_t dwGlowManager = 0x2305BA0;
    constexpr std::ptrdiff_t dwLocalPlayerController = 0x22EF0B8;
    constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x2064AE0;
    constexpr std::ptrdiff_t dwPlantedC4 = 0x23120B0;
    constexpr std::ptrdiff_t dwPrediction = 0x20649F0;
    constexpr std::ptrdiff_t dwSensitivity = 0x23066B8;
    constexpr std::ptrdiff_t dwSensitivity_sensitivity = 0x58;
    constexpr std::ptrdiff_t dwViewAngles = 0x2314F98;
    constexpr std::ptrdiff_t dwViewMatrix = 0x230ADE0;
    constexpr std::ptrdiff_t dwViewRender = 0x230B1E8;
    constexpr std::ptrdiff_t dwWeaponC4 = 0x2293E30;

	//Player Value offsets - updated rarely. Search for these values in client.dll dump if update breaks
	constexpr std::ptrdiff_t m_vOldOrigin = 0x1588;
	constexpr std::ptrdiff_t m_iTeamNum = 0x3F3;
	constexpr std::ptrdiff_t m_iHealth = 0x354;
	constexpr std::ptrdiff_t m_hPlayerPawn = 0x90C;
	constexpr std::ptrdiff_t dwSanitizedName = 0x860;
	constexpr std::ptrdiff_t m_angEyeAngles = 0x3DD0;
	constexpr std::ptrdiff_t m_iCompTeammateColor = 0x848;
};

//These change rarely
namespace matchmaking_dll {
	constexpr std::ptrdiff_t dwGameTypes = 0x1B8000;
	constexpr std::ptrdiff_t dwGameTypes_mapName = 0x120;
};