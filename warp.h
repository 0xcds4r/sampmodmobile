#pragma once

class CTeleportWindow
{
public:
	CTeleportWindow();
	~CTeleportWindow();

	void Render();
	void Clear();
	void Show(bool bShow);
	
public:
	bool				m_bIsActive;
};