#pragma once

// keyboard buttons
#define	ID_KEY_ALT 1024
#define	ID_KEY_TAB 1
#define	ID_KEY_C 2
#define	ID_KEY_LCTRL 4
#define	ID_KEY_SPACE 8
#define	ID_KEY_ENTER 16
#define	ID_KEY_LSHIFT 32
#define	ID_KEY_E 64
#define	ID_KEY_RMB 128
#define	ID_KEY_Q 256
#define	ID_KEY_TWO 320
#define	ID_KEY_TWOS 512
#define	ID_KEY_Y 65536
#define	ID_KEY_N 131072

class CExtraKeyBoard
{
public:
	CExtraKeyBoard();
	~CExtraKeyBoard();

	void Render();
	void Clear();
	void Show(bool bShow);
	void ToggleExtraKeyBoard();

	void SendKeyUsing(int keyId);

public:
	bool		m_bIsActive;
	bool 		m_bClose;
	uint16_t	key;
private:
	float	m_fPosX;
	float	m_fPosY;
	float 	m_fSizeX;
	float 	m_fSizeY;
	float 	m_fButWidth;
	float 	m_fButHeight;
};