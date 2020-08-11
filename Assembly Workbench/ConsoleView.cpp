#include "stdafx.h"
#include "ConsoleView.h"
#include "Main.h"

ConsoleView::ConsoleView()
{
    Init();
}

ConsoleView::ConsoleView(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

bool ConsoleView::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
    wxPanel::Create(parent, id, pos, size, style);

    CreateControls();
    //SetMinSize(size);
    //Centre();

    return true;
}

ConsoleView::~ConsoleView()
{
}

void ConsoleView::Init()
{
}

void ConsoleView::CreateControls()
{
	MainFrame* pMainFrame{ static_cast<MainFrame*>(wxTheApp->GetTopWindow()) };
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxVERTICAL);
	m_pTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
	m_pTextCtrl->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
	m_pTextCtrl->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
	m_pLogTextCtrl = new wxLogTextCtrl(m_pTextCtrl);
	wxLog::SetActiveTarget(m_pLogTextCtrl);
	bSizer4->Add(m_pTextCtrl, 1, wxALL | wxEXPAND, 5);

	bSizer2->Add(bSizer4, 1, wxEXPAND, 2);


	bSizer1->Add(bSizer2, 1, wxEXPAND, 2);


	SetSizer(bSizer1);
	Layout();
}

void ConsoleView::Clear()
{
	m_pTextCtrl->Clear();
}
