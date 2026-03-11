#pragma once
#include <iostream>

namespace client_dll { // client.dll
	//These need changed every update
    constexpr std::ptrdiff_t dwEntityList = 0x21CA050;
    constexpr std::ptrdiff_t dwLocalPlayerController = 0x22F2178;
    constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x2067B60;

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
    constexpr std::ptrdiff_t m_hMyWeapons = 0x48;

    constexpr std::ptrdiff_t m_AttributeManager = 0x1378;
    constexpr std::ptrdiff_t m_Item = 0x50;
    constexpr std::ptrdiff_t m_iItemDefinitionIndex = 0x1BA;

};

//These change rarely
namespace matchmaking_dll {
	constexpr std::ptrdiff_t dwGameTypes = 0x1B8000;
	constexpr std::ptrdiff_t dwGameTypes_mapName = 0x120;
};