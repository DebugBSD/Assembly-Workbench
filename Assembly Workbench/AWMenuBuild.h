#pragma once
#include <wx/dialog.h>
class AWMenuBuild :
    public wxDialog
{
public:
    AWMenuBuild(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name);
    ~AWMenuBuild();

    WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wparam, WXLPARAM lparam);


private:
    class wxBoxSizer* m_pSizer;

    class AWButton* m_pBuildSolutionBtn;
    class AWButton* m_pRebuildSolutionBtn;
    class AWButton* m_pCleanSolutionBtn;

    long lastX;
    long lastY;

private:

    void OnBuildSolution(wxCommandEvent& event);

    void OnRebuildSolution(wxCommandEvent& event);

    void OnCleanSolution(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();

};

enum
{
    ID_BUILD_SOLUTION_BTN,
    ID_REBUILD_SOLUTION_BTN,
    ID_CLEAN_SOLUTION_BTN,
};
