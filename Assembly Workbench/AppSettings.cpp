#include "stdafx.h"
#include "AppSettings.h"
#include "Main.h"

AppSettings::AppSettings() :
	m_menuSize{ wxSize(-1, 24) },
	m_frameIconsSize{ wxSize(18, 18) },
	m_tbIconsSize{ wxSize(18, 18) },
	m_backgroundColor{ RGB(0x21, 0x21, 0x21) },
	m_foregroundColor{ RGB(0xff, 0xff, 0xff) },
	m_activeTabColor{ RGB(0x41, 0x41, 0x41) },
	m_primaryColor{ RGB(0xBB, 0x86, 0xFC) },
	m_alternativeColor{ RGB(0x37, 0x00, 0xB3) },
	m_highlightedColor{ RGB(0x41, 0x41, 0x41) },
	m_fontSize{ 10 }
{
}

AppSettings::~AppSettings()
{
}
