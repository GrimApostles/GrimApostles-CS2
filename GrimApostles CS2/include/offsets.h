#pragma once
#include <iostream>

namespace client_dll { // client.dll
	//These need changed every update
    constexpr std::ptrdiff_t dwCSGOInput = 0x2317040;
    constexpr std::ptrdiff_t dwEntityList = 0x21C90D0;
    constexpr std::ptrdiff_t dwGameEntitySystem = 0x24AC2A8;
    constexpr std::ptrdiff_t dwGameEntitySystem_highestEntityIndex = 0x20A0;
    constexpr std::ptrdiff_t dwGameRules = 0x230AF60;
    constexpr std::ptrdiff_t dwGlobalVars = 0x205B5C0;
    constexpr std::ptrdiff_t dwGlowManager = 0x2307D68;
    constexpr std::ptrdiff_t dwLocalPlayerController = 0x22F1208;
    constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x2066B60;
    constexpr std::ptrdiff_t dwPlantedC4 = 0x23147C0;
    constexpr std::ptrdiff_t dwPrediction = 0x2066A70;
    constexpr std::ptrdiff_t dwSensitivity = 0x2308878;
    constexpr std::ptrdiff_t dwSensitivity_sensitivity = 0x58;
    constexpr std::ptrdiff_t dwViewAngles = 0x23176C8;
    constexpr std::ptrdiff_t dwViewMatrix = 0x230CFA0;
    constexpr std::ptrdiff_t dwViewRender = 0x230D3B8;
    constexpr std::ptrdiff_t dwWeaponC4 = 0x2296410;

	//Player Value offsets - updated rarely. Search for these values in client.dll dump if update breaks
	constexpr std::ptrdiff_t m_vOldOrigin = 0x1588;
	constexpr std::ptrdiff_t m_iTeamNum = 0x3F3;
	constexpr std::ptrdiff_t m_iHealth = 0x354;
	constexpr std::ptrdiff_t m_hPlayerPawn = 0x90C;
	constexpr std::ptrdiff_t dwSanitizedName = 0x860;
	constexpr std::ptrdiff_t m_angEyeAngles = 0x3DD0;
	constexpr std::ptrdiff_t m_iCompTeammateColor = 0x848;
    constexpr std::ptrdiff_t m_pClippingWeapon = 0x3DC0;
    constexpr std::ptrdiff_t m_pWeaponServices = 0x13D8;

    //weapon services
    constexpr std::ptrdiff_t m_hMyWeapons = 0x48; // C_NetworkUtlVectorBase<CHandle<C_BasePlayerWeapon>>

    constexpr std::ptrdiff_t m_AttributeManager = 0x1378;
    constexpr std::ptrdiff_t m_Item = 0x50;
    constexpr std::ptrdiff_t m_iItemDefinitionIndex = 0x1BA;

};

//These change rarely
namespace matchmaking_dll {
	constexpr std::ptrdiff_t dwGameTypes = 0x1B8000;
	constexpr std::ptrdiff_t dwGameTypes_mapName = 0x120;
};