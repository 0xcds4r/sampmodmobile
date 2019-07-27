#pragma once

#define MAX_SETTINGS_STRING	0x7F

struct stSettings
{
	char szNickName[MAX_PLAYER_NAME+1];
	char szHost[MAX_SETTINGS_STRING+1];
	char szPassword[MAX_SETTINGS_STRING+1];
	char szFont[40];
	
	int iFontOutline;
	int iChatMaxMessages;
	int iPort;
	int fColor1;
	int fColor2;
	int fColor3;

	float fChatPosX;
	float fChatPosY;
	float fChatSizeX;
	float fChatSizeY;
	float fFontSize;
	float fSpawnScreenPosX;
	float fSpawnScreenPosY;
	float fSpawnScreenSizeX;
	float fSpawnScreenSizeY;
	float fHealthBarWidth;
	float fHealthBarHeight;
	float fCheatsBoxWidth;
	float fCheatsBoxHeight;
	float fDist;

	bool bDebug;
	bool bOnline;
	bool fCheatsBox;
	bool fCheatInv;
	bool fCheatWallHack;
	bool fCheatNoFall;
	bool fCheatInvis;
	bool fCheatFlash; 
	bool fCheatBehind;
	bool fRadar;
	bool fFixCrash;
	bool fObjects;
	bool fTextLabel;
	bool fExtOS;
	bool fClock;
	bool fFastFire;
};

class CSettings
{
public:
	CSettings();
	~CSettings();

	stSettings& Get() { return m_Settings; }
	
private:
	struct stSettings m_Settings;
};