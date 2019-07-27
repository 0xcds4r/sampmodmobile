#pragma once

class CModSAWindow
{
public:
	CModSAWindow();
	~CModSAWindow();

	void Process();

	void Render();
	void Clear();
	void Show(bool bShow);

	void UpdateData();
	void FlyPlayMove(int moveId);

	void ToggleGTAPatch(int gtapatchid);
	void ToggleCheat(int cheatid);
	void ToggleRPC(int rpcid);
	void ToggleTeleport(int posid);

	void StoreMarkerXYZ(float x, float y, float z);
	void ShowFlyMenu(bool bShow);

	void SetFlySets(float distance);
	float GetFlySets();

public:
	/** state */
	bool		m_bIsActive;

	/** step */
	int			m_bMenuStep;

	/** cheats */
	int 		m_bFlyHack;
	int			m_bGodMode;
	int			m_bWallHack;
	int			m_bCols;
	int 		m_bINVIS;
	int			m_bAirbreak;
	int 		m_bFLASH;
	int			m_bKrutilka;
	int 		m_bPulsatorHealth;
	int 		m_bPulsatorArmour;

	/** samp patches */
	// incoming rpc
	int			m_bVP;
	int			m_bVP2;
	int			m_bVV;
	int			m_bCO;
	int			m_bPH;
	int			m_bRPFV;
	int			m_bSD;
	int			m_bAPA;
	int			m_bRPW;
	int			m_bNF;
	int			m_bCP;
	int			m_bCP2;
	int			m_bCA;
	int			m_bSPP;
	int			m_bCTL;
	int			m_bCE;
	int			m_bSPN;
	int			m_bAGZ;
	int 		m_bTPC;
	int 		m_bSTD;
	int 		m_bHTD;
	int 		m_bSPI;
	int			m_bSM;
	int 		m_bHM;
	int 		m_bSG;
	int 		m_bSPS;
	int			m_bDGT;
	int 		m_bFSS;
	int 		m_bSCP;
	int 		m_bSCLA;
	int 		m_bSPFA;
	int 		m_bSFS;
	int 		m_bSSI;
	int 		m_bSPA;
	int 		m_bSPC;
	int 		m_bSPPFZ;
	int 		m_bSMI;
	int 		m_bDMI;
	int 		m_bGPW;
	int 		m_bSVH;
	int 		m_bSVP;
	int 		m_bPS;
	int 		m_bSPWL;
	int 		m_bWVA;
	int 		m_bSCBP;
	int 		m_bSPSA;
	int	 		m_bPPIV;
	int 		m_bSES;

	// outcoming RPC
	int			m_bEV;
	int			m_bGCW;
	int			m_bGPP;
	int 		m_bEXV;

	/** GTA Patches */
	int 		m_bClock;
	int			m_bFastfire;
	int			m_bCarvisible;
	int			m_bLockWeather;
	int 		m_bLockTime;
	int 		m_bRxyz;
	int 		m_bFlock;
	int 		m_bRadar;

	/** protection */
	int			protect;

	/** settings */
	int			m_bLockInp;
	int 		m_bDNames;
	int			m_bPlayers;

	/** positions */
	float 		sPos1X;
	float 		sPos1Y;
	float 		sPos1Z;

	float 		sPos2X;
	float 		sPos2Y;
	float 		sPos2Z;

	float 		sPos3X;
	float 		sPos3Y;
	float 		sPos3Z;

	float 		i_x;
	float 		i_y;
	float 		i_z;

	float		marker_X;
	float		marker_Y;
	float		marker_Z;

	float		sx;
	float		sy;
	float		sz;

	/** inv hack params */
	float 		cHealth;
	float		xHealth;

	/** fly hack params */
	float 		fDist;

	/** network params */
	char*		m_szAddress;
	int			m_szPort;

	/** other */
	int			m_bExtOS;
	int			m_bCurAnim;
};