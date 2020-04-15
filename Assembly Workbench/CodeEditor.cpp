#include "stdafx.h"
#include "CodeEditor.h"


wxBEGIN_EVENT_TABLE(CodeEditor, wxTextCtrl)
	EVT_TEXT_ENTER(ID_TextChanged, CodeEditor::TextChanged)
wxEND_EVENT_TABLE()

CodeEditor::CodeEditor(wxWindow* parent):
    wxTextCtrl(parent, ID_TextChanged, wxEmptyString, { 0, 0 }, { 400, 400 }, wxTE_MULTILINE | wxTE_RICH | wxTE_RICH2 | wxTE_AUTO_URL | wxHSCROLL | wxTE_PROCESS_ENTER) // Base class
{
}

CodeEditor::~CodeEditor()
{
}

void CodeEditor::TextChanged(wxCommandEvent& event)
{
    int stop = 1;
}
