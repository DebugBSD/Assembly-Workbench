#include "stdafx.h"
#include <wx/button.h>
#include "SettingsDialog.h"

wxBEGIN_EVENT_TABLE(SettingsDialog, wxDialog)
	EVT_BUTTON(wxID_OK, SettingsDialog::OnCloseButton)
wxEND_EVENT_TABLE()

SettingsDialog::SettingsDialog(wxWindow* parent):
	wxDialog(parent, wxID_ANY, "Settings", { 0,0 }, { 600, 400 })
{
	SetBackgroundColour(wxColour(0x12, 0x12, 0x12));
	SetForegroundColour(wxColour(0xCC, 0x99, 0xFF));
	new wxButton(this, wxID_OK, "OK", { 0,0 }, { 100, 25 });
	new wxButton(this, wxID_CANCEL, "Cancel", { 100,0 }, { 100, 25 });
}


void SettingsDialog::OnCloseButton(wxCommandEvent& event)
{
	int stop = 1;

	EndModal(wxID_OK);
}