#include "stdafx.h"
#include "AWMenuView.h"

#include <Windows.h>
#include <WinUser.h>

#include <wx/window.h>
#include <wx/sizer.h>

#include "AWMenuEdit.h"
#include "AWButton.h"
#include "Main.h"

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC USHORT(0x01)
#endif

#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE USHORT(0x02)
#endif

wxBEGIN_EVENT_TABLE(AWMenuView, wxDialog)
EVT_BUTTON(ID_MENU_VIEW_FILES_BTN, AWMenuView::OnViewFiles)
EVT_BUTTON(ID_MENU_VIEW_FUNCTIONS_BTN, AWMenuView::OnViewFunctions)
EVT_BUTTON(ID_MENU_VIEW_VARIABLES_BTN, AWMenuView::OnViewVariables)
EVT_BUTTON(ID_MENU_VIEW_OPCODES_BTN, AWMenuView::OnViewOpcodes)
wxEND_EVENT_TABLE()

AWMenuView::AWMenuView(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) : wxDialog(parent, id, wxEmptyString, pos, size, style)
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
		{&m_pMenuViewFilesBtn, ID_MENU_VIEW_FILES_BTN, wxT(" Files"), wxDefaultPosition, pMainFrame->GetAppSettings()->m_menuSize, wxBORDER_NONE | wxBU_LEFT, wxBitmap("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/1x/baseline_create_new_folder_white_18dp.png", wxBITMAP_TYPE_ANY)},
		{&m_pMenuViewFunctionsBtn, ID_MENU_VIEW_FUNCTIONS_BTN, wxT(" Functions"), wxDefaultPosition, pMainFrame->GetAppSettings()->m_menuSize, wxBORDER_NONE | wxBU_LEFT, wxBitmap("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/1x/baseline_create_new_folder_white_18dp.png", wxBITMAP_TYPE_ANY)},
		{&m_pMenuViewVariablesBtn, ID_MENU_VIEW_VARIABLES_BTN, wxT(" Variables"), wxDefaultPosition, pMainFrame->GetAppSettings()->m_menuSize, wxBORDER_NONE | wxBU_LEFT, wxBitmap("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/1x/baseline_folder_open_white_18dp.png", wxBITMAP_TYPE_ANY)},
		{&m_pMenuViewOpcodesBtn, ID_MENU_VIEW_OPCODES_BTN, wxT(" Opcodes"), wxDefaultPosition, pMainFrame->GetAppSettings()->m_menuSize, wxBORDER_NONE | wxBU_LEFT, wxBitmap("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/1x/baseline_folder_open_white_18dp.png", wxBITMAP_TYPE_ANY)},
		{NULL, -1, wxEmptyString, wxPoint(-1, -1), wxSize(-1, -1), 0}
	};

	for (int i = 0; menuButtons[i].pButton != NULL; i++)
	{
		wxBoxSizer* pHoriz = new wxBoxSizer(wxHORIZONTAL);

		(*menuButtons[i].pButton) = new AWButton(this, menuButtons[i].windowId, menuButtons[i].text, menuButtons[i].pos, menuButtons[i].size, menuButtons[i].style, wxDefaultValidator, wxButtonNameStr, menuButtons[i].icon);
		pHoriz->Add((*menuButtons[i].pButton), 1, wxALL | wxEXPAND, 2);

		m_pSizer->Add(pHoriz, 1, wxALL | wxEXPAND, 2);
	}

	// Chech menu elements and enable if possible.
	/*if (pMainFrame->GetProject() != nullptr)
		m_pProjectSettingsBtn->Enable(true);
	else
		m_pProjectSettingsBtn->Enable(false);*/

	SetSizerAndFit(m_pSizer);
}

AWMenuView::~AWMenuView()
{
}

WXLRESULT AWMenuView::MSWWindowProc(WXUINT message, WXWPARAM wparam, WXLPARAM lparam)
{
	if (message == WM_INPUT) {
		unsigned size = sizeof(RAWINPUT);
		static RAWINPUT raw[sizeof(RAWINPUT)];
		GetRawInputData((HRAWINPUT)lparam, RID_INPUT, raw, &size, sizeof(RAWINPUTHEADER));

		// Revisar esto, no me cuadra.... Declaras un array de raw y luego coges solo 1???
		// Buscar la fuente de esto y mirar como lo hiciste originalmente.
		if (raw->header.dwType == RIM_TYPEMOUSE) {
			lastX = raw->data.mouse.lLastX;
			lastY = raw->data.mouse.lLastY;

			if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
			{
				int x, y;
				wxGetMousePosition(&x, &y);
				if (!GetRect().Contains(x, y))
				{
					//if (m_pProjectSettingsView == nullptr && m_pAppSettingsView == nullptr)
					//{
					if (IsModal()) // Is not visible cause is not created
						EndModal(-1);
					//}
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

void AWMenuView::OnViewFiles(wxCommandEvent& event)
{
	EndModal(ID_MENU_VIEW_FILES_BTN);
}

void AWMenuView::OnViewFunctions(wxCommandEvent& event)
{
	EndModal(ID_MENU_VIEW_FUNCTIONS_BTN);
}

void AWMenuView::OnViewVariables(wxCommandEvent& event)
{
	EndModal(ID_MENU_VIEW_VARIABLES_BTN);
}

void AWMenuView::OnViewOpcodes(wxCommandEvent& event)
{
	EndModal(ID_MENU_VIEW_OPCODES_BTN);
}
