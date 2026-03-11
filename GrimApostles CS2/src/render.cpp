#include "gui.h"

void gui::gameLoop(CGame game) {
	std::string mapName = game.map;
	//checking bounds for vertigo/nuke
	if (mapName == "de_nuke" && game.localPlayer.position.z <= maps::nukeZBound) mapName = "de_nuke_lower";
	if (mapName == "de_vertigo" && game.localPlayer.position.z <= maps::vertigoZBound) mapName = "de_vertigo_lower";
	ID3D11ShaderResourceView* texture = maps::mapTextures[mapName];
	renderMap(texture);
	renderPlayers(game);
	ImGui::End();
}

void gui::renderMap(ID3D11ShaderResourceView* texture) {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SetNextWindowPos(ImVec2((ImGui::GetIO().DisplaySize.x / 2 - maps::radarSize / 2), (ImGui::GetIO().DisplaySize.y / 2 - maps::radarSize / 2)));
	ImGui::SetNextWindowSize(ImVec2(maps::radarSize, maps::radarSize));
	ImGui::Begin("MAP", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::Image((void*)texture, ImVec2(maps::radarSize, maps::radarSize));
	ImGui::PopStyleVar(2);
}

void gui::renderPlayers(CGame game) {
	for (int i = 1; i <= 64; i++) {
		CPlayer& p = game.players[i - 1];
		if (p.controller == NULL) continue;
		if (p.health == 0) continue;

		ImVec2 windowPos = ImGui::GetWindowPos();
		ImU32 dotColor;
		float localZ = game.localPlayer.position.z;
	
		float x = p.position.x;
		float y = p.position.y;
		float z = p.position.z;
		worldToRadar(x, y, game);

		float opacity = setOpacity(localZ, z, game);

		float aimLine = 40.f;
		float angle = p.eyeAngles.y;
		angle = angle * 3.14159265f / 180.0f;

		//aim lines
		ImVec2 endpoint = ImVec2(windowPos.x + x + aimLine * cos(angle) + 1.0f, windowPos.y + y + aimLine * sin(angle) * -1 + 1.0f);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2((windowPos.x + x), (windowPos.y + y)), endpoint, IM_COL32(0, 0, 0, opacity), 6.5f);
		ImGui::GetForegroundDrawList()->AddLine(ImVec2((windowPos.x + x), (windowPos.y + y)), endpoint, IM_COL32(255, 255, 255, opacity), 4.0f);

		//Players
		//local player dot white, set team color, enemies red
		if (p.controller == game.localPlayer.controller) dotColor = IM_COL32(255, 255, 255, 255);
		else if (p.teamID == game.localPlayer.teamID) dotColor = setColor(p.color, opacity);
		else (dotColor = IM_COL32(255, 9, 9, opacity));
		//Draw black outline
		ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2((windowPos.x + x), (windowPos.y + y)), 9.25f, IM_COL32(0, 0, 0, 255));
		ImGui::GetForegroundDrawList()->AddCircleFilled(ImVec2((windowPos.x + x), (windowPos.y + y)), 8.0f, dotColor);

		//Weapons - only drawing enemy
		int weaponID = p.activeWeapon.weaponID;
		bool hasC4 = false;
		//active weapon
		float iconW = (float)icons::iconWidths[weaponID] * icons::scale;
		float iconH = (float)icons::iconHeights[weaponID] * icons::scale;
		//C4
		float c4W = (float)icons::iconWidths[49] * icons::scale;
		float c4H = (float)icons::iconHeights[49] * icons::scale;

		ImVec2	iconPos;
		ImVec2 c4Pos;
		//Icon/C4 position
		if (angle <= 3.14159265f && angle >= 0) {
			// below the dot
			iconPos = ImVec2(windowPos.x + x - (iconW / 2), (windowPos.y + y) + 10.f);
			c4Pos = ImVec2(windowPos.x + x - (c4W / 2), iconPos.y + iconH + 2.f);
		}
		else {
			// above the dot
			iconPos = ImVec2(windowPos.x + x - (iconW / 2), (windowPos.y + y) - 10.f - iconH);
			c4Pos = ImVec2(windowPos.x + x - (c4W / 2), iconPos.y - c4H - 2.f);
		}

		//Drawing icons
		if (p.teamID != game.localPlayer.teamID) {

			//active weapon
			ImGui::GetForegroundDrawList()->AddImage(
				(ImTextureID)icons::iconTextures[p.activeWeapon.weaponID],
				iconPos,
				ImVec2(iconPos.x + iconW, iconPos.y + iconH),
				ImVec2(0, 0), ImVec2(1, 1),
				IM_COL32(255, 255, 255, 255)
			);

			//C4
			if (p.weaponCount < 0 || p.weaponCount > 64) return;
			for (int j = 0; j < p.weaponCount; j++) {
				if (p.weapons[j].weaponID == 49) hasC4 = true;
			}

			if (hasC4 && weaponID != 49) {
				ImGui::GetForegroundDrawList()->AddImage(
					(ImTextureID)icons::iconTextures[49],
					c4Pos,
					ImVec2(c4Pos.x + c4W, c4Pos.y + c4H),
					ImVec2(0, 0), ImVec2(1, 1),
					IM_COL32(255, 255, 255, 255)
				);
			}

		}

	}
}



void gui::worldToRadar(float& x, float& y, CGame game) {
	std::string mapName = game.map;
	mapData data = maps::mapBounds[mapName];
	x -= data.xBound;
	y -= data.yBound;
	x /= data.scale;
	y /= data.scale;
	//radar size conversion
	x *= maps::radarSize / 1024.f;
	y *= maps::radarSize / 1024.f;
	y *= -1;
}
ImU32 gui::setColor(DWORD color, float opacity) {
	switch (color) {
		//Grey
	case -1:
		return IM_COL32(142, 212, 210, opacity);
		//Blue
	case 0:
		return IM_COL32(0, 255, 251, opacity);
		//Green
	case 1:
		return IM_COL32(47, 255, 0, opacity);
		//Yellow
	case 2:
		return IM_COL32(255, 255, 0, opacity);
		//Orange
	case 3:
		return IM_COL32(250, 130, 2, opacity);
		//Purple
	case 4:
		return IM_COL32(250, 2, 182, opacity);
	default:
		return IM_COL32(133, 204, 148, opacity);
	}
}
float gui::setOpacity(float localZ, float entZ, CGame game) {
	std::string mapName = game.map;
	if (mapName == "de_nuke") {
		if (localZ < -495 && entZ >= -495) return 155;
		if (localZ >= -495 && entZ < -495) return 155;
	}
	if (mapName == "de_vertigo") {
		if (localZ < 11700 && entZ >= 11700) return 155;
		if (localZ >= 11700 && entZ < 11700) return 155;
	}
	return 255;
}