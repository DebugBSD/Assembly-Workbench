/*
 * BSD 3-Clause License
 * 
 * Copyright (c) 2020, DebugBSD
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once
#include <wx/panel.h>
#include <wx/treectrl.h>
#include <wx/srchctrl.h>
#include <wx/string.h>

#include <vector>

#include "File.h"
/*!
 * Forward declarations
 */


 /*!
  * Control identifiers
  */

#define ID_PROJECTWINDOW 10000
#define ID_PROJECTS_WINDOW_TEXTCTRL 10001
#define ID_TREECTRL 10002
#define SYMBOL_PROJECTWINDOW_STYLE wxTR_DEFAULT_STYLE | wxNO_BORDER
#define SYMBOL_PROJECTWINDOW_TITLE _("ProjectWindow")
#define SYMBOL_PROJECTWINDOW_IDNAME ID_PROJECTWINDOW
#define SYMBOL_PROJECTWINDOW_SIZE wxSize(200, 300)
#define SYMBOL_PROJECTWINDOW_POSITION wxDefaultPosition

enum {
    // Tree Control IDs
    ID_TreeCtrl_Projects_View = wxID_HIGHEST+1,
    // Add new file to project. We Show a window to create the file.
    ID_Project_View_Add_New_File,
    ID_Project_View_Add_New_Folder
};
/*!
 * ProjectWindow class declaration
 */

class TElement {
public:
    TElement(const wxString &name, wxTreeItemId Id, int type):
        m_Name{ name }, m_id{ Id }, m_Type{ type } {}

    ~TElement(){}

private:
    wxTreeItemId m_id;
    wxString m_Name;
    int m_Type;
};

class TFile : public TElement{
public:
    TFile(class File* pFile, wxTreeItemId Id) :
        TElement{ pFile->GetFileName(), Id, 0 },
        m_pFile{pFile}
    {}

    ~TFile() {}
private:
    class File* m_pFile;
};

class TFolder : public TElement {
public:
    TFolder(const wxString &name, wxTreeItemId Id) :
        TElement{ name, Id, 1 }
    {}

    ~TFolder() {}
private:
    std::vector<TElement*> m_Elements;
};

class ProjectsWindow : public wxPanel
{

public:
    /// Constructors
    ProjectsWindow();
    ProjectsWindow(wxWindow* parent, wxWindowID id = ID_TreeCtrl_Projects_View, const wxString& caption = SYMBOL_PROJECTWINDOW_TITLE, const wxPoint& pos = SYMBOL_PROJECTWINDOW_POSITION, const wxSize& size = SYMBOL_PROJECTWINDOW_SIZE, long style = SYMBOL_PROJECTWINDOW_STYLE);

    bool Create(wxWindow* parent, wxWindowID id = ID_TreeCtrl_Projects_View, const wxString& caption = SYMBOL_PROJECTWINDOW_TITLE, const wxPoint& pos = SYMBOL_PROJECTWINDOW_POSITION, const wxSize& size = SYMBOL_PROJECTWINDOW_SIZE, long style = SYMBOL_PROJECTWINDOW_STYLE);

    /// Destructor
    ~ProjectsWindow();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

        /// Retrieves bitmap resources
    wxBitmap GetBitmapResource(const wxString& name);

    /// Retrieves icon resources
    wxIcon GetIconResource(const wxString& name);

        /// Should we show tooltips?
    static bool ShowToolTips();

    void AddProject(class Project* pProject);
    void RemoveProject(class Project* pProject);
    void RemoveProject(const wxString& projectName);

private:
    class Project* m_pSelectedProject;
    wxTreeItemId m_pSelectedProjectTid;

    class MainFrame* m_pMainFrame;

    class wxSearchCtrl* m_pSearchCtrl;
    class wxTreeCtrl* m_pTreeCtrl;

    // Files container
    std::unordered_map<void*, class File *> m_pTreeFiles;

    std::unordered_map<Project*, TElement*> m_TreeProjects;
private:

    class Project* GetProject(const wxString& text);

    // Tree Events
    void SelectedElement(wxTreeEvent& event);
    void OnRightClickOverTreeCtrl(wxTreeEvent& event);
    void OnPopupNewFile(wxCommandEvent& event);

    void OnPopupNewFolder(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

// _PROJECTWINDOW_H_
