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

public:
#pragma region Public attributes

#pragma endregion

#pragma region Public Methods

#pragma endregion

private:
#pragma region Private attributes
    wxAuiManager* m_pmgr;
#pragma endregion

#pragma region Private Methods

#pragma endregion


};
enum
{
    ID_Hello = 1
};