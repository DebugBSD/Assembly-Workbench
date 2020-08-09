#include "stdafx.h"
#include "AWButton.h"
#include "Main.h"

wxBEGIN_EVENT_TABLE(AWButton, wxButton)
EVT_ENTER_WINDOW(AWButton::OnMouseEnter)
EVT_LEAVE_WINDOW(AWButton::OnMouseLeave)
wxEND_EVENT_TABLE()

AWButton::AWButton(wxWindow* parent, wxWindowID id, const wxString& label, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name, const wxBitmap& bitmap, const wxString& tooltip) :
	wxButton(parent, id, label, pos, size, style, validator, name),
	m_Bitmap{ bitmap },
	m_Tooltip{ tooltip },
	m_pMainFrame{ static_cast<MainFrame*>(wxTheApp->GetTopWindow()) },
	m_BackgroundColor{ m_pMainFrame->GetAppSettings()->m_backgroundColor },
	m_ForegroundColor{ m_pMainFrame->GetAppSettings()->m_foregroundColor },
	m_HighlightedColor{ m_pMainFrame->GetAppSettings()->m_highlightedColor }
{
	SetFont(wxFont(m_pMainFrame->GetAppSettings()->m_fontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
	SetBitmap(m_Bitmap);
	SetToolTip(m_Tooltip);
	SetBackgroundColour(m_BackgroundColor);
	SetForegroundColour(m_ForegroundColor);
}

AWButton::~AWButton()
{
}

void AWButton::OnMouseEnter(wxMouseEvent& event)
{
	SetBackgroundColour(m_HighlightedColor);
}

void AWButton::OnMouseLeave(wxMouseEvent& event)
{
	SetBackgroundColour(m_BackgroundColor);
}

void AWButton::SetBackgroundColor(const wxColor& c)
{
	m_BackgroundColor = c;
	SetBackgroundColour(m_BackgroundColor);
	Update();
}

void AWButton::SetForegroundColor(const wxColor& c)
{
	m_ForegroundColor = c;
	SetForegroundColour(m_ForegroundColor);
	Update();
}

void AWButton::SetHighlightedColor(const wxColor& c)
{
	m_ForegroundColor = c;
}
