#pragma once
#include <wx/dialog.h>
class AWMenuView :
    public wxDialog
{
public:
    AWMenuView(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name);
    ~AWMenuView();

    WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wparam, WXLPARAM lparam);


private:
    class wxBoxSizer* m_pSizer;

    class AWButton* m_pMenuViewFilesBtn;
    class AWButton* m_pMenuViewFunctionsBtn;
    class AWButton* m_pMenuViewVariablesBtn;
    class AWButton* m_pMenuViewOpcodesBtn;

    long lastX;
    long lastY;

private:

    void OnViewFiles(wxCommandEvent& event);
    void OnViewFunctions(wxCommandEvent& event);
    void OnViewVariables(wxCommandEvent& event);
    void OnViewOpcodes(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

enum
{
    ID_MENU_VIEW_FILES_BTN,
    ID_MENU_VIEW_FUNCTIONS_BTN,
    ID_MENU_VIEW_VARIABLES_BTN,
    ID_MENU_VIEW_OPCODES_BTN,
};
