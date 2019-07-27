#include "main.h"
#include "settings.h"
#include "vendor/inih/cpp/INIReader.h"

CSettings::CSettings()
{
	//Log("Loading settings..");	

	char buff[0x7F];
	sprintf(buff, "%sSAMP/settings.ini", g_pszStorage);

	INIReader reader(buff);

	if(reader.ParseError() < 0)
	{
		Log("Error: can't load %s", buff);
		std::terminate();
		return;
	}

	// Client
	size_t length = 0;
	sprintf(buff, "QDS_User%d%d", rand() % 1000, rand() % 1000);
	length = reader.Get("client", "name", buff).copy(m_Settings.szNickName, MAX_PLAYER_NAME);
	m_Settings.szNickName[length] = '\0';

	length = reader.Get("client", "host", "127.0.0.1").copy(m_Settings.szHost, MAX_SETTINGS_STRING);
	m_Settings.szHost[length] = '\0';
	length = reader.Get("client", "password", "").copy(m_Settings.szPassword, MAX_SETTINGS_STRING);
	m_Settings.szPassword[length] = '\0';
	m_Settings.iPort = reader.GetInteger("client", "port", 7777);

	// Debug
	m_Settings.bDebug = reader.GetBoolean("debug", "debug", true);
	m_Settings.bOnline = reader.GetBoolean("debug", "online", false);

	// gui
	length = reader.Get("gui", "Font", "Arial.ttf").copy(m_Settings.szFont, 35);
	m_Settings.szFont[length] = '\0';
	m_Settings.fFontSize = reader.GetReal("gui", "FontSize", 40.0f);
	m_Settings.iFontOutline = reader.GetInteger("gui", "FontOutline", 1.9);

	// chat
	m_Settings.fChatPosX = reader.GetReal("gui", "ChatPosX", 325.0f);
	m_Settings.fChatPosY = reader.GetReal("gui", "ChatPosY", 25.0f);
	m_Settings.fChatSizeX = reader.GetReal("gui", "ChatSizeX", 1150.0f);
	m_Settings.fChatSizeY = reader.GetReal("gui", "ChatSizeY", 220.0f);
	m_Settings.iChatMaxMessages = reader.GetInteger("gui", "ChatMaxMessages", 10);

	// spawnscreen
	m_Settings.fSpawnScreenPosX = reader.GetReal("gui", "SpawnScreenPosX", 660.0f);
	m_Settings.fSpawnScreenPosY = reader.GetReal("gui", "SpawnScreenPosY", 950.0f);
	m_Settings.fSpawnScreenSizeX = reader.GetReal("gui", "SpawnScreenSizeX", 600.0f);
	m_Settings.fSpawnScreenSizeY = reader.GetReal("gui", "SpawnScreenSizeY", 100.0f);

	// nametags
	m_Settings.fHealthBarWidth = reader.GetReal("gui", "HealthBarWidth", 100.0f);
	m_Settings.fHealthBarHeight = reader.GetReal("gui", "HealthBarHeight", 10.0f);

	// ~

	char buff2[0x7F];
	sprintf(buff2, "%smod_sa/sets.ini", g_pszStorage);

	INIReader reader2(buff2);

	if(reader2.ParseError() < 0)
	{
		Log("Error: can't load %s", buff2);
		std::terminate();
		return;
	}
	size_t length2 = 0;

	// render sets
	m_Settings.fCheatsBoxWidth = reader2.GetReal("modsa", "CheatsBoxWidth", 55.0f);
	m_Settings.fCheatsBoxHeight = reader2.GetReal("modsa", "CheatsBoxHeight", 370.0f);
	m_Settings.fCheatsBox = reader2.GetBoolean("modsa", "CheatsBox", true);
	m_Settings.fColor1 = reader2.GetInteger("modsa", "color1", 250);
	m_Settings.fColor2 = reader2.GetInteger("modsa", "color2", 5);
	m_Settings.fColor3 = reader2.GetInteger("modsa", "color3", 5);

	// cheats
	m_Settings.fCheatInv = reader2.GetBoolean("modsa", "invincible", false);
	m_Settings.fCheatWallHack = reader2.GetBoolean("modsa", "wallhack", false);
	m_Settings.fCheatNoFall = reader2.GetBoolean("modsa", "nofall", false);
	m_Settings.fCheatInvis = reader2.GetBoolean("modsa", "invisible", false);
	m_Settings.fCheatFlash = reader2.GetBoolean("modsa", "flash", false);
	m_Settings.fCheatBehind = reader2.GetBoolean("modsa", "behind", false);

	// gta patches
	m_Settings.fRadar = reader2.GetBoolean("game", "radar", false);
	m_Settings.fClock = reader2.GetBoolean("game", "clock", false);
	m_Settings.fFastFire = reader2.GetBoolean("game", "fastfire", false);

	// custom server sets
	length = reader2.Get("customserver", "host", "127.0.0.1").copy(m_Settings.szHost, MAX_SETTINGS_STRING);
	m_Settings.szHost[length] = '\0';
	m_Settings.iPort = reader2.GetInteger("customserver", "port", 7777);

	m_Settings.fFixCrash = reader2.GetBoolean("customserver", "fixcrash", false);
	m_Settings.fObjects = reader2.GetBoolean("customserver", "objects", false);
	m_Settings.fTextLabel = reader2.GetBoolean("customserver", "textlabel", false);
	m_Settings.fExtOS = reader2.GetBoolean("customserver", "ext_os", false);

	// fly sets
	m_Settings.fDist = reader2.GetReal("flyhack", "speed", 0.7f);
}