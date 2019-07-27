#include "main.h"
#include "game/game.h"
#include "net/netgame.h"
#include "gui/gui.h"
#include "dialog.h"
#include "vendor/imgui/imgui_internal.h"
#include "keyboard.h"
#include "modsa.h"
#include <stdlib.h>

extern CGUI *pGUI;
extern CGame *pGame;
extern CNetGame *pNetGame;
extern CKeyBoard *pKeyBoard;
extern CModSAWindow *pModSAWindow;

char szDialogInputBuffer[100];
char utf8DialogInputBuffer[100*3];

CDialogWindow::CDialogWindow()
{
	m_bIsActive = false;
	m_putf8Info = nullptr;
	m_pszInfo = nullptr;
	m_selectedItem = -1;

	passEntering = 0;
	fxdialog = 0;
}

CDialogWindow::~CDialogWindow()
{

}

void CDialogWindow::Show(bool bShow)
{
	ImGuiIO &io = ImGui::GetIO();
	if(pGame) 
		pGame->FindPlayerPed()->TogglePlayerControllable(!bShow);

	m_bIsActive = bShow;

	if(bShow != true)
	{
		passEntering = 0;
		fxdialog = 0;
		m_selectedItem = -1;
	}
}

void CDialogWindow::Clear()
{
	if(m_putf8Info)
	{
		free(m_putf8Info);
		m_putf8Info = nullptr;
	}

	if(m_pszInfo)
	{
		free(m_pszInfo);
		m_pszInfo = nullptr;
	}

	m_bIsActive = false;
	m_byteDialogStyle = 0;
	m_wDialogID = -1;
	m_utf8Title[0] = 0;
	m_utf8Button1[0] = 0;
	m_utf8Button2[0] = 0;

	passEntering = 0;
	fxdialog = 0;

	m_fSizeX = -1.0f;
	m_fSizeY = -1.0f;

	memset(szDialogInputBuffer, 0, 100);
	memset(utf8DialogInputBuffer, 0, 100*3);
}

void CDialogWindow::SetInfo(char* szInfo, int length)
{
	if(!szInfo || !length) return;

	if(m_putf8Info)
	{
		free(m_putf8Info);
		m_putf8Info = nullptr;
	}

	if(m_pszInfo)
	{
		free(m_pszInfo);
		m_pszInfo = nullptr;
	}

	m_putf8Info = (char*)malloc((length * 3) + 1);
	if(!m_putf8Info)
	{
		Log("CDialog::SetInfo: can't allocate memory");
		return;
	}

	m_pszInfo = (char*)malloc((length * 3) + 1);
	if(!m_pszInfo)
	{
		Log("CDialog::SetInfo: can't allocate memory");
		return;
	}

	cp1251_to_utf8(m_putf8Info, szInfo);

	// =========
	char szNonColorEmbeddedMsg[4200];
	int iNonColorEmbeddedMsgLen = 0;

	for (size_t pos = 0; pos < strlen(szInfo) && szInfo[pos] != '\0'; pos++)
	{
		if(pos+7 < strlen(szInfo))
		{
			if (szInfo[pos] == '{' && szInfo[pos+7] == '}')
			{
				pos += 7;
				continue;
			}
		}

		szNonColorEmbeddedMsg[iNonColorEmbeddedMsgLen] = szInfo[pos];
		iNonColorEmbeddedMsgLen++;
	}

	szNonColorEmbeddedMsg[iNonColorEmbeddedMsgLen] = 0;
	// ========

	cp1251_to_utf8(m_pszInfo, szNonColorEmbeddedMsg);
}

bool ProcessInlineHexColor(const char* start, const char* end, ImVec4& color);

void TextWithColors(const char* fmt, ...)
{
	char tempStr[4096];

	va_list argPtr;
	va_start(argPtr, fmt);
	vsnprintf(tempStr, sizeof(tempStr), fmt, argPtr);
	va_end(argPtr);
	tempStr[sizeof(tempStr) - 1] = '\0';

	bool pushedColorStyle = false;
	const char* textStart = tempStr;
	const char* textCur = tempStr;
	while(textCur < (tempStr + sizeof(tempStr)) && *textCur != '\0')
	{
		if (*textCur == '{')
		{
			// Print accumulated text
			if (textCur != textStart)
			{
				ImGui::TextUnformatted(textStart, textCur);
				ImGui::SameLine(0.0f, 0.0f);
			}

			// Process color code
			const char* colorStart = textCur + 1;
			do
			{
				++textCur;
			} while (*textCur != '\0' && *textCur != '}');

			// Change color
			if (pushedColorStyle)
			{
				ImGui::PopStyleColor();
				pushedColorStyle = false;
			}

			ImVec4 textColor;
			if (ProcessInlineHexColor(colorStart, textCur, textColor))
			{
				ImGui::PushStyleColor(ImGuiCol_Text, textColor);
				pushedColorStyle = true;
			}

			textStart = textCur + 1;
		}
		else if (*textCur == '\n')
		{
			// Print accumulated text an go to next line
			ImGui::TextUnformatted(textStart, textCur);
			textStart = textCur + 1;
		}

		++textCur;
	}

	if (textCur != textStart)
		ImGui::TextUnformatted(textStart, textCur);
	else
		ImGui::NewLine();

	if(pushedColorStyle)
		ImGui::PopStyleColor();
}

void DialogWindowInputHandler(const char* str)
{
	if(!str || *str == '\0') return;
	strcpy(szDialogInputBuffer, str);
	cp1251_to_utf8(utf8DialogInputBuffer, str);
}

void CDialogWindow::AddListItemsAsText()
{
	char bufString[4096];
    strcpy(bufString, m_putf8Info);
    char *str = bufString;
    char *pch;

    int i = -1;
    pch = strtok (str, "\n");

    while(pch != NULL)
    {
        i++;
        if(i != 1)
        {
        	char bufStr[16] = "%d. %s";
			if(i != 0) sprintf(bufStr, "%d. %s", (i - 1), pch); // items
				else sprintf(bufStr, "%s", pch); // header text

			TextWithColors(bufStr);
			pch = strtok(NULL, "\n");
        }
    }
}

void CDialogWindow::Render()
{
	if(!m_bIsActive || !m_putf8Info) return;

	ImGuiIO &io = ImGui::GetIO();
	ImVec2 buttsize;
	ImVec2 incurSize;
	buttsize.x = ImGui::CalcTextSize("QWERTYUIOPASQWERTYUIOPASQWERTY").x;
	incurSize.x = ImGui::CalcTextSize("QWERTYUIOPASDFGHJKLZXCVBN").x;

	ImVec2 inputSize;
	inputSize = ImVec2((ImGui::CalcTextSize("QWERTYUIOPAS").x) * 1.25f, pGUI->GetFontSize() * 2 + 3.0);
	int posInpX = (ImGui::GetWindowSize().x / 2) - (((inputSize.x*2) + pGUI->GetFontSize() + 0.25) / 2);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14, 8));

	ImGui::Begin("dialog", nullptr, 
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings);

	std::vector<std::string> lines;

	TextWithColors(m_utf8Title);
	
	ImGui::ItemSize( ImVec2(0, pGUI->GetFontSize()/2 + 2.75) );

	passEntering = 0;
	fxdialog = 0;
	ImGui::GetStyle().ButtonTextAlign = ImVec2(0.5f, 0.5f);

	switch(m_byteDialogStyle)
	{
		case DIALOG_STYLE_MSGBOX:
		TextWithColors(m_putf8Info);
		ImGui::ItemSize( ImVec2(0, pGUI->GetFontSize()/2) );
		break;

		case DIALOG_STYLE_INPUT:
		TextWithColors(m_putf8Info);
		ImGui::ItemSize( ImVec2(0, 50) );

		//ImGui::SetCursorPosX(ImGui::GetCursorPos().x + posInpX);
		if(ImGui::CalcTextSize(m_putf8Info).x < 280)
		{
			if( ImGui::Button(utf8DialogInputBuffer, ImVec2(incurSize.x * 2, 50)) )
			{
				pKeyBoard->Open(&DialogWindowInputHandler);
			}
		}else if(ImGui::CalcTextSize(m_putf8Info).x > 280){
			if( ImGui::Button(utf8DialogInputBuffer, ImVec2(ImGui::CalcTextSize(m_putf8Info).x, 50)) )
			{
				pKeyBoard->Open(&DialogWindowInputHandler);
			}
		}
		
		ImGui::ItemSize( ImVec2(0, pGUI->GetFontSize()/2) );
		break;

		case DIALOG_STYLE_PASSWORD:
		passEntering = 1;
		TextWithColors(m_putf8Info);
		ImGui::ItemSize( ImVec2(0, 50) );

		//ImGui::SetCursorPosX(ImGui::GetCursorPos().x + posInpX);
		if(strlen(utf8DialogInputBuffer) <= 0)
		{
			if(ImGui::CalcTextSize(m_putf8Info).x < 280)
			{
				if( ImGui::Button(" ", ImVec2(incurSize.x * 2, 50)) )
				{
					pKeyBoard->Open(&DialogWindowInputHandler);
				}
			}else if(ImGui::CalcTextSize(m_putf8Info).x > 280){
				if( ImGui::Button(" ", ImVec2(ImGui::CalcTextSize(m_putf8Info).x, 50)) )
				{
					pKeyBoard->Open(&DialogWindowInputHandler);
				}
			}
		}else{
			if(ImGui::CalcTextSize(m_putf8Info).x <= 180)
			{
				if( ImGui::Button("**********", ImVec2(incurSize.x * 2, 50)) )
				{
					pKeyBoard->Open(&DialogWindowInputHandler);
				}
			}else if(ImGui::CalcTextSize(m_putf8Info).x >= 280){
				if( ImGui::Button("**********", ImVec2(ImGui::CalcTextSize(m_putf8Info).x, 50)) )
				{
					pKeyBoard->Open(&DialogWindowInputHandler);
				}
			}
		}

		ImGui::ItemSize( ImVec2(0, pGUI->GetFontSize()/2) );
		break;

		case DIALOG_STYLE_TABLIST_HEADERS:
		AddListItemsAsText();

		ImGui::ItemSize( ImVec2(0, 50) );

		if(ImGui::CalcTextSize(m_putf8Info).x < 280)
		{
			if( ImGui::Button(utf8DialogInputBuffer, ImVec2(incurSize.x * 2, 50)) )
			{
				pKeyBoard->Open(&DialogWindowInputHandler);
			}
		}else if(ImGui::CalcTextSize(m_putf8Info).x > 280){
			if( ImGui::Button(utf8DialogInputBuffer, ImVec2(ImGui::CalcTextSize(m_putf8Info).x, 50)) )
			{
				pKeyBoard->Open(&DialogWindowInputHandler);
			}
		}

		ImGui::ItemSize( ImVec2(0, pGUI->GetFontSize()/2) );
		// допилить
		break;

		case DIALOG_STYLE_TABLIST:
		case DIALOG_STYLE_LIST:
		fxdialog = 1;
		ImGuiContext& g = *GImGui;

		ImGui::GetStyle().WindowPadding = ImVec2(8, 8);
	
		std::string line = m_putf8Info; 

		while (line.find('\n') != -1)
		{
			unsigned short int posV = line.find('\n');
			if(posV <= 0) break;

			std::string buffer;

			buffer.assign(line, 0, posV);
			line.erase(0, posV + 1);

			if(buffer[0] != '\0')
				lines.push_back(buffer);

			if(posV <= 0) break;
		}

		if(line[0] != '\0') lines.push_back(line);

		ImGui::GetStyle().ButtonTextAlign = ImVec2(0.1f, 0.5f); // left | center
		//ImGui::GetStyle().ItemSpacing = ImVec2(0.0f, pGUI->GetFontSize() / 2);
		unsigned int curSize = lines.size();

		for(int i = 0; i < curSize; i++)
		{	
			ImGuiStyle style;
			style.Colors[ImGuiCol_Button] = ImGui::GetStyle().Colors[ImGuiCol_Button];
			
			if(m_selectedItem == i) ImGui::GetStyle().Colors[ImGuiCol_Button] = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
			
			ImVec2 curpos = ImGui::GetCursorPos();
			std::string szName;

			for(int x = 0; x < i; x++) szName.insert(szName.begin(),' ');

			ImVec2 butSize;
			if((ImGui::CalcTextSize(m_pszInfo).x * 1.4f + pGUI->GetFontSize() * 3) > (buttsize.x) * 2)
				butSize.x = ImGui::CalcTextSize(m_pszInfo).x * 1.4f;
					else butSize.x = (buttsize.x)*2 + pGUI->GetFontSize();
			
			if(ImGui::Button((char*)szName.c_str(), ImVec2(butSize.x, pGUI->GetFontSize() * 2.0f + 3.0)) )
			{
				if(m_selectedItem == i)
				{
					ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(0.13f, 0.13f, 0.13f, 0.95f);
					Show(false);
					if(pNetGame){
						if(m_byteDialogStyle == DIALOG_STYLE_LIST || m_byteDialogStyle == DIALOG_STYLE_TABLIST)
							pNetGame->SendDialogResponse(m_wDialogID, 1, i, (char*)lines[i].c_str());
					}
					return;
				}

				strcpy(szDialogInputBuffer, lines[i].c_str());
				m_selectedItem = i;
			}

			ImVec2 newxtcurpos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(curpos.x+pGUI->GetFontSize(), curpos.y+pGUI->GetFontSize()/2));
			TextWithColors(lines[i].c_str());
			ImGui::SetCursorPos(newxtcurpos);
			
			ImGui::GetStyle().Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_Button];
		}

		//ImGui::GetStyle().ItemSpacing = ImVec2(0.0f, 1.0f);
		ImGui::GetStyle().ButtonTextAlign = ImVec2(0.5f,0.5f);

		ImGui::Dummy(ImVec2(1, pGUI->GetFontSize()));
		ImGui::GetStyle().WindowPadding = ImVec2(pGUI->GetFontSize(), pGUI->GetFontSize());
		break;
	}

	ImVec2 button1Size = ImVec2((ImGui::CalcTextSize("QWERTYUIOPAS").x) * 1.25f, pGUI->GetFontSize() * 2 + 3.0);
	ImVec2 button2Size = ImVec2((ImGui::CalcTextSize("QWERTYUIOPAS").x) * 1.25f, pGUI->GetFontSize() * 2 + 3.0);
	ImVec2 buttonSize = ImMax(button1Size, button2Size);

	int posx = ((ImGui::GetWindowSize().x / 2) - ((buttonSize.x + buttonSize.x + pGUI->GetFontSize() * 3) / 2) + 8.75);
	ImGui::GetStyle().ButtonTextAlign = ImVec2(0.5f, 0.5f);

	//ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 278 + ImGui::GetStyle().ItemSpacing.x) / 2);

	if(m_byteDialogStyle == DIALOG_STYLE_TABLIST_HEADERS && szDialogInputBuffer != NULL)
	{
		int inputBuffer = atoi(szDialogInputBuffer);
		m_selectedItem = (inputBuffer - 1);
	}

	if(m_utf8Button1[0] != 0) 
	{
		if(m_utf8Button2[0] != 0) ImGui::SetCursorPosX(ImGui::GetCursorPos().x + posx);

		if(ImGui::Button(m_utf8Button1, ImVec2(buttonSize.x, buttonSize.y + 2.5)))
		{
			if(pNetGame) 
				pNetGame->SendDialogResponse(m_wDialogID, 1, m_selectedItem, szDialogInputBuffer);

			Show(false);
		}
	}

	ImGui::SameLine(0, pGUI->GetFontSize());

	if(m_utf8Button2[0] != 0) 
	{
		if(ImGui::Button(m_utf8Button2, ImVec2(buttonSize.x, buttonSize.y + 2.5)))
		{
			if(pNetGame) 
				pNetGame->SendDialogResponse(m_wDialogID, 0, -1, szDialogInputBuffer);
			
			Show(false);
		}
	}

	ImGui::ItemSize( ImVec2(0, pGUI->GetFontSize()/2 + 1.75) );

	ImGui::SetWindowSize(ImVec2(-1, -1));
	ImVec2 size = ImGui::GetWindowSize();
	ImGui::SetWindowPos( ImVec2( ((io.DisplaySize.x - size.x)/2), ((io.DisplaySize.y - size.y)/2)) );
	ImGui::End();

	ImGui::PopStyleVar();
}