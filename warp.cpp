#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "game/game.h"
#include "net/netgame.h"
#include "gui/gui.h"
#include "vendor/imgui/imgui_internal.h"
#include "keyboard.h"
#include "warp.h"
#include "chatwindow.h"
#include "modsa.h"
#include "vendor/inih/cpp/INIReader.h"

extern CGUI *pGUI;
extern CGame *pGame;
extern CNetGame *pNetGame;
extern CKeyBoard *pKeyBoard;
extern CChatWindow *pChatWindow;
extern CModSAWindow *pModSAWindow;

char szPlayerIDInputBuffer[100];
char utf8PlayerIDInputBuffer[100*3];

CTeleportWindow::CTeleportWindow()
{
	m_bIsActive = false;
}

CTeleportWindow::~CTeleportWindow()
{

}

void CTeleportWindow::Show(bool bShow)
{
	if(pGame) 
		pGame->FindPlayerPed()->TogglePlayerControllable(!bShow);

	m_bIsActive = bShow;
}

void CTeleportWindow::Clear()
{
	m_bIsActive = false;

	memset(szPlayerIDInputBuffer, 0, 100);
	memset(utf8PlayerIDInputBuffer, 0, 100*3);
}

void PlayerIDWindowInputHandler(const char* str)
{
	if(!str || *str == '\0') return;
	strcpy(szPlayerIDInputBuffer, str);
	cp1251_to_utf8(utf8PlayerIDInputBuffer, str);
}

void CTeleportWindow::Render()
{
	if(!m_bIsActive) return;

	ImGuiIO &io = ImGui::GetIO();

	ImGui::StyleColorsClassic();
	
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8,8));

	ImGui::GetStyle().ButtonTextAlign = ImVec2(0.5f,0.5f);

	ImGui::Begin("> Teleport", nullptr, 
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize);

		ImGui::Text("Enter the player ID");

		if( ImGui::Button(utf8PlayerIDInputBuffer, ImVec2(200, 45) ))
		{
			if(!pKeyBoard->IsOpen())
				pKeyBoard->Open(&PlayerIDWindowInputHandler);
		}

		ImGui::ItemSize( ImVec2(0, pGUI->GetFontSize()/2 + 5) );

		if(ImGui::Button("Teleport", ImVec2(200, 50)))
		{
			char *strid = szPlayerIDInputBuffer;
			PLAYERID id = atoi(strid);

			CRemotePlayer* pPlayer = pNetGame->GetPlayerPool()->GetAt(id);
			CPlayerPed* pPlayerPed = pPlayer->GetPlayerPed();

			MATRIX4X4 mat;
        	pPlayerPed->GetMatrix(&mat);

        	float x = mat.pos.X;
        	float y = mat.pos.Y;
        	float z = mat.pos.Z;

			if(!pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed()->IsInVehicle())
				pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed()->TeleportTo((x - 0.25), (y - 0.5), z);
					else ScriptCommand(&set_car_coordinates, pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed()->GetCurrentVehicleID(), (x - 0.25), (y - 0.5), z);

			//Show(false);
		}

		//ImGui::ItemSize( ImVec2(0, pGUI->GetFontSize()/2 + 5) );
		
		if(ImGui::Button("Cancel", ImVec2(200, 50)))
		{
			pNetGame->GetPlayerPool()->GetLocalPlayer()->ToggleSpectating(false);
			Show(false);
		}

	ImGui::SetWindowSize(ImVec2(-1, -1));
	ImVec2 size = ImGui::GetWindowSize();
	ImGui::SetWindowPos( ImVec2( ((io.DisplaySize.x - size.x + 435)/2), ((io.DisplaySize.y - size.y)/2)) );
	ImGui::End();

	ImGui::PopStyleVar();
}