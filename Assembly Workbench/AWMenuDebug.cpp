#include "stdafx.h"
#include "AWMenuDebug.h"

#include <Windows.h>
#include <WinUser.h>

#include "Project.h"
#include "AWButton.h"
#include "Main.h"

#include <wx/window.h>
#include <wx/sizer.h>

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC USHORT(0x01)
#endif

#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE USHORT(0x02)
#endif

wxBEGIN_EVENT_TABLE(AWMenuDebug, wxDialog)
EVT_BUTTON(ID_MENU_DEBUG_LAUNCH_DEBUGGER, AWMenuDebug::OnMenuLaunchDebugger)
EVT_BUTTON(ID_MENU_DEBUG_WINDOWS, AWMenuDebug::OnMenuWindows)
wxEND_EVENT_TABLE()

AWMenuDebug::AWMenuDebug(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) :
	wxDialog(parent, id, wxEmptyString, pos, size, style)
{
	MainFrame* pMainFrame{ static_cast<MainFrame*>(wxTheApp->GetTopWindow()) };

	RAWINPUTDEVICE rid[1];
	rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	rid[0].dwFlags = RIDEV_INPUTSINK;
	rid[0].hwndTarget = GetHWND();
	RegisterRawInputDevices(rid, 1, sizeof(rid[0]));

	SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);

	m_pSizer = new wxBoxSizer(wxVERTICAL);

	struct menuInfo {
		AWButton** pButton;
		wxWindowID windowId;
		wxString text;
		wxPoint pos;
		wxSize size;
		long style;
		wxBitmap icon;
	} menuButtons[] =
	{
		{&m_pLaunchDebuggerBtn, ID_MENU_DEBUG_LAUNCH_DEBUGGER, wxT(" Launch Windows Debugger"), wxDefaultPosition, pMainFrame->GetAppSettings()->m_menuSize, wxBORDER_NONE | wxBU_LEFT, wxBitmap("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/1x/baseline_create_new_folder_white_18dp.png", wxBITMAP_TYPE_ANY)},
		{&m_pWindowsBtn, ID_MENU_DEBUG_WINDOWS, wxT(" Windows"), wxDefaultPosition, pMainFrame->GetAppSettings()->m_menuSize, wxBORDER_NONE | wxBU_LEFT, wxBitmap("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/1x/baseline_create_new_folder_white_18dp.png", wxBITMAP_TYPE_ANY)},
		{NULL, -1, wxEmptyString, wxPoint(-1, -1), wxSize(-1, -1), 0}
	};


	for (int i = 0; menuButtons[i].pButton != NULL; i++)
	{
		wxBoxSizer* pHoriz = new wxBoxSizer(wxHORIZONTAL);

		(*menuButtons[i].pButton) = new AWButton(this, menuButtons[i].windowId, menuButtons[i].text, menuButtons[i].pos, menuButtons[i].size, menuButtons[i].style, wxDefaultValidator, wxButtonNameStr, menuButtons[i].icon);
		pHoriz->Add((*menuButtons[i].pButton), 1, wxALL | wxEXPAND, 2);

		m_pSizer->Add(pHoriz, 1, wxALL | wxEXPAND, 2);
	}

	SetSizerAndFit(m_pSizer);
}

AWMenuDebug::~AWMenuDebug()
{
}

WXLRESULT AWMenuDebug::MSWWindowProc(WXUINT message, WXWPARAM wparam, WXLPARAM lparam)
{
	if (message == WM_INPUT) {
		unsigned size = sizeof(RAWINPUT);
		static RAWINPUT raw[sizeof(RAWINPUT)];
		GetRawInputData((HRAWINPUT)lparam, RID_INPUT, raw, &size, sizeof(RAWINPUTHEADER));

		if (raw->header.dwType == RIM_TYPEMOUSE) {
			lastX = raw->data.mouse.lLastX;
			lastY = raw->data.mouse.lLastY;

			if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
			{
				int x, y;
				wxGetMousePosition(&x, &y);
				if (!GetRect().Contains(x, y))
				{
					if (IsModal()) // Is not visible cause is not created
						EndModal(-1);
				}
			}
			else if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
			{
				/* Do nothing. */
			}
		}

		return 0;
	}

#if defined(__WXMSW__)
	return wxTopLevelWindow::MSWWindowProc(message, wparam, lparam);
#else
#error "Window procedure unsupported on other platforms!!"
#endif
}

void AWMenuDebug::OnMenuLaunchDebugger(wxCommandEvent& event)
{
	EndModal(ID_MENU_DEBUG_LAUNCH_DEBUGGER);
}

void AWMenuDebug::OnMenuWindows(wxCommandEvent& event)
{
	EndModal(ID_MENU_DEBUG_WINDOWS);
}
