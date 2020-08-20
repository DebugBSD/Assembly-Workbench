#pragma once
#include <wx/dialog.h>
class AWMenuProject :
    public wxDialog
{
public:
    AWMenuProject(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name);
    ~AWMenuProject();

    WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wparam, WXLPARAM lparam);


private:
    class wxBoxSizer* m_pSizer;

    class AWButton* m_pMenuAssemblerBtn;
    class AWButton* m_pMenuCompilerBtn;
    class AWButton* m_pMenuLinkerBtn;
    class AWButton* m_pMenuPreferencesBtn;

    long lastX;
    long lastY;

private:

    void OnMenuAssembler(wxCommandEvent& event);
    void OnMenuCompiler(wxCommandEvent& event);

    void OnMenuLiner(wxCommandEvent& event);

    void OnMenuPreferences(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

enum
{
    ID_MENU_ASSEMBLER_BTN,
    ID_MENU_COMPILER_BTN,
    ID_MENU_LINKER_BTN,
    ID_MENU_PREFERENCES_BTN,
};
