#pragma once
#include <wx/dialog.h>
class AWMenuFile :
    public wxDialog
{
public:
    AWMenuFile(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name);
    ~AWMenuFile();

    WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wparam, WXLPARAM lparam);

private:

    class wxBoxSizer* m_pSizer;

    class AWButton* m_pNewFileBtn;
    class AWButton* m_pNewProjectBtn;
    class AWButton* m_pOpenFileBtn; 
    class AWButton* m_pOpenProjectBtn;
    class AWButton* m_pSaveBtn;
    class AWButton* m_pCloseBtn;
    class AWButton* m_pExitAppBtn;

    long lastX;
    long lastY;

private:

    void OnMenuNewFile(wxCommandEvent& event);
    void OnMenuNewProject(wxCommandEvent& event);
    void OnMenuOpenFile(wxCommandEvent& event);
    void OnMenuOpenProject(wxCommandEvent& event);
    void OnMenuSave(wxCommandEvent& event);
    void OnMenuClose(wxCommandEvent& event);
    void OnMenuExit(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();

};

enum
{
    ID_MENU_FILE_NEW_FILE, 
    ID_MENU_FILE_NEW_PROJECT,
    ID_MENU_FILE_OPEN_FILE,
    ID_MENU_FILE_OPEN_PROJECT,
    ID_MENU_FILE_SAVE,
    ID_MENU_FILE_CLOSE,
    ID_MENU_FILE_EXIT
};
