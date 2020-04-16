#pragma once

#include <wx/aui/framemanager.h>
#include <wx/wxprec.h>
#include <wx/utils.h>
#include <wx/panel.h>
#include <wx/stc/stc.h>
#include <wx/sizer.h>

class CodeEditor :
	public wxStyledTextCtrl
{
public:
	CodeEditor(wxWindow* parent);
	~CodeEditor();

private:

	void TextChanged(wxCommandEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnKeyChar(wxKeyEvent& event);

	//wxDECLARE_EVENT_TABLE();
};

enum
{
	ID_TextChanged = 1
};