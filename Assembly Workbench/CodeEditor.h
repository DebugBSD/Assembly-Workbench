#pragma once

#include <wx/aui/framemanager.h>
#include <wx/wxprec.h>
#include <wx/utils.h>
#include <wx/panel.h>
#include <wx/stc/stc.h>
#include <wx/sizer.h>


enum
{
	ID_TextChanged = 1,
	ID_KeyEnter
};

enum class EventType
{
	EVENT_NONE = -1,
	EVENT_LEFT,
	EVENT_RIGHT,
	EVENT_UP,
	EVENT_DOWN,
	EVENT_LMOUSE
};

class CodeEditor :
	public wxTextCtrl
{
public:
	CodeEditor(wxWindow* parent);
	~CodeEditor();

private:
	class MainFrame* m_pMainFrame;

private:

	void GetCursorPosition(size_t &lnPos, size_t &colPos);
	void SetCursorPosition(const EventType &evtType = EventType::EVENT_NONE);

	void TextChanged(wxCommandEvent& event);
	void OnKeyEnter(wxCommandEvent &event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnMouseDown(wxMouseEvent& event);
	void OnMouseUp(wxMouseEvent& event);

	wxDECLARE_EVENT_TABLE();
};
