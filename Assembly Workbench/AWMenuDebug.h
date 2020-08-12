#pragma once
#include <wx/dialog.h>
class AWMenuDebug :
    public wxDialog
{
public:
    AWMenuDebug(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name);
    ~AWMenuDebug();

    WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wparam, WXLPARAM lparam);

private:

    class wxBoxSizer* m_pSizer;

    class AWButton* m_pLaunchDebuggerBtn;
    class AWButton* m_pWindowsBtn;

    long lastX;
    long lastY;

private:

    void OnMenuLaunchDebugger(wxCommandEvent& event);
    void OnMenuWindows(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();

};

enum
{
    ID_MENU_DEBUG_LAUNCH_DEBUGGER,
    ID_MENU_DEBUG_WINDOWS
};

