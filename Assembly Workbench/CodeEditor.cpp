#include "stdafx.h"
#include "CodeEditor.h"
#include "Main.h"


wxBEGIN_EVENT_TABLE(CodeEditor, wxTextCtrl)
	EVT_TEXT(wxID_ANY, CodeEditor::TextChanged)
	EVT_TEXT_ENTER(wxID_ANY, CodeEditor::OnKeyEnter)
	EVT_KEY_DOWN(CodeEditor::OnKeyDown)
	EVT_KEY_UP(CodeEditor::OnKeyUp)
	EVT_LEFT_DOWN(CodeEditor::OnMouseDown)
	EVT_LEFT_UP(CodeEditor::OnMouseUp)
wxEND_EVENT_TABLE()

CodeEditor::CodeEditor(wxWindow* parent, const wxString& fileName):
	wxTextCtrl(parent, wxID_ANY, wxEmptyString, { 0, 0 }, parent->GetClientSize(), wxTE_MULTILINE | wxHSCROLL | wxVSCROLL | wxTE_RICH | wxTE_PROCESS_ENTER), // Base class
	m_pMainFrame{ static_cast<MainFrame*>(parent) },
	m_FileName{ fileName }
{
    wxFont f(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Consolas");
	SetFont(f);
	SetBackgroundColour(wxColour(0x12, 0x12, 0x12));
	SetForegroundColour(wxColour(0xCC, 0x99, 0xFF));
    
}

CodeEditor::~CodeEditor()
{
}

void CodeEditor::GetCursorPosition(size_t& lnPos, size_t& colPos)
{

}

void CodeEditor::SetCursorPosition(const EventType& evtType)
{
	switch (evtType)
	{
	case EventType::EVENT_LEFT:

		break;
	case EventType::EVENT_RIGHT:

		break;
	case EventType::EVENT_UP:

		break;
	case EventType::EVENT_DOWN:

		break;
	case EventType::EVENT_LMOUSE:

		break;
	}
}

void CodeEditor::TextChanged(wxCommandEvent& event)
{
	wxString text{ event.GetString() };
	
	int insertPosition = GetInsertionPoint();
	int numberOfLines{ GetNumberOfLines() };
	size_t colPos{ 0 }, lnPos{ 0 };

	GetCursorPosition(lnPos, colPos);

	//SetModified(true);

	// We update the status bar
	m_pMainFrame->SetStatusBar(GetLastPosition(), numberOfLines, colPos, lnPos);
}

void CodeEditor::OnKeyEnter(wxCommandEvent& event)
{
	int stop = 1;
}

void CodeEditor::OnKeyDown(wxKeyEvent& event)
{
	event.Skip();
}

void CodeEditor::OnKeyUp(wxKeyEvent& event)
{
	switch (event.GetKeyCode())
	{
	case WXK_LEFT:
		SetCursorPosition(EventType::EVENT_LEFT);
		break;
	case WXK_RIGHT:
		SetCursorPosition(EventType::EVENT_RIGHT);
		break;
	case WXK_UP:
		SetCursorPosition(EventType::EVENT_UP);
		break;
	case WXK_DOWN:
		SetCursorPosition(EventType::EVENT_DOWN);
		break;
	}

}

void CodeEditor::OnMouseDown(wxMouseEvent& event)
{
	int stop = 1;
	int pos = GetInsertionPoint();
	event.Skip();
}

void CodeEditor::OnMouseUp(wxMouseEvent& event)
{
	int stop = 1;
	int pos = GetInsertionPoint();
	event.Skip();
}
