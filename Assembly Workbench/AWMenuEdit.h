#pragma once
#include <wx/dialog.h>

class AWMenuEdit :
    public wxDialog
{
public:
    AWMenuEdit(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name);
    ~AWMenuEdit();

    WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wparam, WXLPARAM lparam);

private:

    class wxBoxSizer* m_pSizer;

    class AWButton* m_pApplicationSettingsBtn;

    long lastX;
    long lastY;

private:

    void OnApplicationSettings(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();

};

enum
{
    ID_MENU_EDIT_APPLICATION_SETTINGS
};

