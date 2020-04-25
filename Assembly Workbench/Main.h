#pragma once

// wxWidgets "Hello World" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/aui/framemanager.h>
#include <wx/wxprec.h>
#include <wx/utils.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


class MyApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();
};

class MainFrame : public wxFrame
{
public:
    MainFrame();
    ~MainFrame();
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnResize(wxSizeEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnNew(wxCommandEvent& event);
    void OnClose(wxCommandEvent& event);
    void OnExitProgram(wxCloseEvent& event);
public:
#pragma region Public attributes

#pragma endregion

#pragma region Public Methods
    void SetStatusBar(const wxString& text) { m_pStatusBar->SetStatusText(text); }
    void SetStatusBar(size_t totalChars = 0, size_t totalLines = 0, size_t currentColumn = 0, size_t currentLine = 0);
#pragma endregion

private:
#pragma region Private attributes
    wxAuiManager* m_pmgr;
    wxStatusBar* m_pStatusBar;
    class CodeEditor* m_pCodeEditor;
#pragma endregion

#pragma region Private Methods
    void CreateMenubar();
    void CreateMainToolBar();
#pragma endregion

    wxDECLARE_EVENT_TABLE();
};
enum
{
    ID_Hello = wxID_HIGHEST+1,
    ID_Size,
    ID_Clone,
    ID_Close_Project,
    ID_Save_Project,
    ID_Recent_Files,
    ID_Recent_Projects,
    ID_Search_Replace,
    ID_View_Files,
    ID_View_Functions,
    ID_View_Variables,
    ID_View_Opcodes,
    ID_Project_Assembler,
    ID_Project_Linker,
    ID_Project_Custom_Assembler,
    ID_Project_Custom_Linker,
    ID_Project_Preferences,
    ID_Build_Build_Solution,
    ID_Build_Rebuild_Solution,
    ID_Build_Clean_Solution,
    ID_Tools_Hex_Editor,
    ID_Tools_CVS,
    ID_Tool_Graph
};