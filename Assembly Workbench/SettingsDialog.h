#pragma once
#include <wx/dialog.h>

class SettingsDialog:
	public wxDialog
{
public:
	SettingsDialog(class wxWindow *parent);

private:


private:

	void OnCloseButton(wxCommandEvent& event);
	wxDECLARE_EVENT_TABLE();


};

