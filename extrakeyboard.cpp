#include "main.h"
#include "game/game.h"
#include "net/netgame.h"
#include "game/common.h"
#include "gui/gui.h"
#include "extrakeyboard.h"
#include "vendor/imgui/imgui_internal.h"
#include "keyboard.h"
#include "chatwindow.h"
#include "net/vehiclepool.h"
#include "settings.h"
#include "modsa.h"
#include "dialog.h"
#include "customserver.h"
#include "net/playerpool.h"
#include "net/localplayer.h"
#include "playerslist.h"
#include "spawnscreen.h"
#include "menu.h"
#include "skinchanger.h"

#include <stdlib.h>
#include <string.h>

extern CGUI *pGUI;
extern CGame *pGame;
extern CNetGame *pNetGame;
extern CKeyBoard *pKeyBoard;
extern CChatWindow *pChatWindow;
extern CSettings *pSettings;
extern CModSAWindow *pModSAWindow;
extern CPlayersList *pPlayersList;
extern CDialogWindow *pDialogWindow;
extern CSpawnScreen *pSpawnScreen;
extern CMenu *pMenu;
extern CSkinChanger *pSkinChanger;

CExtraKeyBoard::CExtraKeyBoard()
{
	m_bIsActive = false;
    m_bClose = false;
}

CExtraKeyBoard::~CExtraKeyBoard()
{

}

void CExtraKeyBoard::Show(bool bShow)
{
    m_fPosX = pGUI->ScaleX( pSettings->Get().fSpawnScreenPosX );
    m_fPosY = pGUI->ScaleY( pSettings->Get().fSpawnScreenPosY );
    m_fSizeX = pGUI->ScaleX( pSettings->Get().fSpawnScreenSizeX );
    m_fSizeY = pGUI->ScaleY( pSettings->Get().fSpawnScreenSizeY );
    m_fButWidth = m_fSizeX / 3;
    m_fButHeight = m_fSizeY * 0.9;
	m_bIsActive = bShow;
}

void CExtraKeyBoard::ToggleExtraKeyBoard()
{
    if(m_bClose) m_bClose = false;
        else m_bClose = true;
}

void CExtraKeyBoard::Clear()
{
	m_bIsActive = false;
}

void CExtraKeyBoard::SendKeyUsing(int keyId)
{   
    CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
    CLocalPlayer *pLocalPlayer = 0;
    if(pPlayerPool) pLocalPlayer = pPlayerPool->GetLocalPlayer();

    switch(keyId){
        case 0:
            key = ID_KEY_ENTER;
            pLocalPlayer->SendFakeOnFootFullSyncData();
        break;

        case 1:
            key = ID_KEY_ALT;
            pLocalPlayer->SendFakeOnFootFullSyncData();
        break;
    }
    
}

void CExtraKeyBoard::Render()
{
	if(!m_bIsActive or m_bClose or pSpawnScreen->m_bEnabled or pDialogWindow->m_bIsActive or 
        pPlayersList->m_bIsActive or pSkinChanger->m_bIsActive or pMenu->m_bEnabled or pModSAWindow->m_bIsActive) return;

	ImGuiIO &io = ImGui::GetIO();

    ImGui::GetStyle().ButtonTextAlign = ImVec2(0.5f, 0.5f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));

    ImGui::Begin("Neckeys", nullptr, 
    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | 
    ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings);

    CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
    CLocalPlayer *pLocalPlayer = 0;
    if(pPlayerPool) pLocalPlayer = pPlayerPool->GetLocalPlayer();

    m_fButWidth = ImGui::CalcTextSize("QWERTY").x;
    m_fButHeight = 40;

    if(!pGame->FindPlayerPed()->IsInVehicle())
    {
        if (ImGui::Button("ALT", ImVec2(m_fButWidth, m_fButHeight)))
        {
            SendKeyUsing(1);
        }
    
        ImGui::SameLine(0, 5);
    
        if (ImGui::Button("TAB", ImVec2(m_fButWidth, m_fButHeight)))
        {
            pPlayersList->Show(true);
        }
    
        ImGui::SameLine(0, 5);
        
        if (ImGui::Button("ENTER", ImVec2(m_fButWidth, m_fButHeight)))
        {
            SendKeyUsing(0);
        }
    
        ImGui::SameLine(0, 5);
    
        if (ImGui::Button("G", ImVec2(m_fButWidth, m_fButHeight)))
        {
            pLocalPlayer->HandlePassengerEntryByCommand();
        }
    } else {
        if (ImGui::Button("TAB", ImVec2(m_fButWidth, m_fButHeight)))
        {
            pPlayersList->Show(true);
        }
    
        ImGui::SameLine(0, 5);
        
        if (ImGui::Button("ENTER", ImVec2(m_fButWidth, m_fButHeight)))
        {
            SendKeyUsing(0);
    
            pLocalPlayer->GetPlayerPed()->ExitCurrentVehicle();
        }
    }
    
    ImGui::SetWindowSize(ImVec2(-1, -1));
    ImVec2 size = ImGui::GetWindowSize();
    
    ImGui::SetWindowPos(ImVec2(((io.DisplaySize.x - size.x)/2) + 110, ((io.DisplaySize.y - size.y) - 35)));
    ImGui::End();
}