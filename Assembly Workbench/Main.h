#pragma once

// wxWidgets "Hello World" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/aui/framemanager.h>
#include <wx/wxprec.h>
#include <wx/utils.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


/*****************************************************************************/
// Menus
//      File
//          New
//              File
//              Project
//          Close Project
//          Recent Files
//          Recent Solutions or Projects
//          Exit
//      Edit
//          Undo
//          Redo
//          Cut
//          Copy
//          Paste
//          Settings (wxDialog)
//              Editor
//              Build Preferences
//          Search & Replace (wxFindReplaceDialog)
//      View
//          Files 
//          Functions
//          Variables
//      Proyect
//      Build
//          Build Solution
//          Rebuild Solution
//          Clean Solution
//      Debug
//      Tools
//          Hexadecimal Editor
//          SCV
//              git
//          Graph Tool
/*****************************************************************************/

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
#pragma endregion

#pragma region Private Methods
    void CreateMenubar();
    void CreateToolBar();
#pragma endregion

    wxDECLARE_EVENT_TABLE();
};
enum
{
    ID_Hello = 1,
    ID_Size
};