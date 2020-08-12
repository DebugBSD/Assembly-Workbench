#pragma once
#include <wx/dialog.h>
class AWMenuTools :
    public wxDialog
{
public:
    AWMenuTools(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name);
    ~AWMenuTools();

    WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wparam, WXLPARAM lparam);

private:

    class wxBoxSizer* m_pSizer;

    class AWButton* m_pCmdLineToolBtn;
    class AWButton* m_pHexEditorBtn;
    class AWButton* m_pGitBtn;
    class AWButton* m_pGraphBtn;

    long lastX;
    long lastY;

private:

    void OnMenuCommandLineTool(wxCommandEvent& event);
    void OnMenuHexEditor(wxCommandEvent& event);
    void OnMenuGit(wxCommandEvent& event);
    void OnMenuGraph(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();

};

enum
{
    ID_MENU_TOOLS_CMD_LINE_TOOL,
    ID_MENU_TOOLS_HEX_EDITOR,
    ID_MENU_TOOLS_GIT,
    ID_MENU_TOOLS_GRAPH
};