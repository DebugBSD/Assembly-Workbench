#pragma once
#include <wx/colour.h>
#include <wx/gdicmn.h>

/*
 * Most common app settings
 *
 * - Colors, fonts and themes
 * - Default code editor (VStudio, VSCode, Notepad++, Assembly Workbench (in the future))
 * - Open last open project
 * - Internationalization (Spanish, English, ...)
 */

class AppSettings
{
public:
	AppSettings();
	~AppSettings();

	wxColor m_backgroundColor;
	wxColor m_foregroundColor;
	wxColor m_activeTabColor;
	wxColor m_primaryColor;
	wxColor m_alternativeColor;
	wxColor m_highlightedColor;
	int m_fontSize;

	wxSize m_menuSize;
	wxSize m_frameIconsSize;
	wxSize m_tbIconsSize;

};

