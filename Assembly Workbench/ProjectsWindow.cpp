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
#include "stdafx.h"
#include "Main.h"
#include "ProjectsWindow.h"
#include "File.h"
#include "CodeEditor.h"
#include "NewFileDlg.h"
#include "Project.h"
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
#include "wx/imaglist.h"
////@end includes

////@begin XPM images

////@end XPM images



/*
 * ProjectWindow event table definition
 */

wxBEGIN_EVENT_TABLE(ProjectsWindow, wxPanel)

    ////@begin ProjectWindow event table entries
    // Tree control events
    EVT_TREE_ITEM_RIGHT_CLICK(ID_TreeCtrl_Projects_View, ProjectsWindow::OnRightClickOverTreeCtrl)
    ////@end ProjectWindow event table entries

wxEND_EVENT_TABLE()

    /*
     * ProjectWindow constructors
     */

ProjectsWindow::ProjectsWindow()
{
    Init();
}

ProjectsWindow::ProjectsWindow(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style):
    m_pMainFrame{ static_cast<MainFrame*>(parent) }
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * ProjectWindow creator
 */

bool ProjectsWindow::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
    ////@begin ProjectWindow creation
    wxPanel::Create(parent, id, pos, size, style);

    CreateControls();
    //SetMinSize(size);
    //Centre();
    ////@end ProjectWindow creation
    return true;
}


/*
 * ProjectWindow destructor
 */

ProjectsWindow::~ProjectsWindow()
{
    ////@begin ProjectWindow destruction
    ////@end ProjectWindow destruction
}


/*
 * Member initialisation
 */

void ProjectsWindow::Init()
{
    ////@begin ProjectWindow member initialisation
    ////@end ProjectWindow member initialisation
}


/*
 * Control creation for ProjectWindow
 */

void ProjectsWindow::CreateControls()
{
    ////@begin ProjectWindow content construction
    m_MenuPopUp = new wxMenu;
    m_MenuPopUp->Append(ID_Project_View_Add_New_File, "Add New File");                      // Open a new window 
    ProjectsWindow* itemFrame1 = this;

    wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer1);

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer1->Add(itemBoxSizer2, 1, wxGROW | wxALL, 0);

    m_pSearchCtrl = new wxSearchCtrl(itemFrame1, ID_PROJECTS_WINDOW_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    itemBoxSizer2->Add(m_pSearchCtrl, 0, wxGROW | wxALL, 0);

    m_pTreeCtrl = new wxTreeCtrl(itemFrame1, ID_TreeCtrl_Projects_View, wxDefaultPosition, wxSize(100, 100), wxTR_DEFAULT_STYLE | wxNO_BORDER);
    m_pTreeCtrl->AddRoot("Projects");
    itemBoxSizer2->Add(m_pTreeCtrl, 1, wxGROW | wxALL, 0);

    ////@end ProjectWindow content construction
}


/*
 * Should we show tooltips?
 */

bool ProjectsWindow::ShowToolTips()
{
    return true;
}

void ProjectsWindow::AddProject(Project* pProject)
{
    /*
     * TODO: Documentation says that is best practice to add elementos to the tree when user
     * expands an item and delete them when user collapses the item, but,right now, we just
     * add them.
     * In the future, change this as documentation says.
     */

    wxTreeItemId root = m_pTreeCtrl->GetRootItem();
    wxTreeItemId rootProject = m_pTreeCtrl->AppendItem(root, pProject->GetName());

    // Add the rest of the files
    for (File* pFile : pProject->GetFiles())
    {
        m_pTreeCtrl->AppendItem(rootProject, pFile->GetFileName());
    }

    m_pTreeCtrl->Expand(root);

}

void ProjectsWindow::RemoveProject(Project* pProject)
{

}

void ProjectsWindow::RemoveProject(const wxString& projectName)
{
}

/*
 * Get bitmap resources
 */

wxBitmap ProjectsWindow::GetBitmapResource(const wxString& name)
{
    // Bitmap retrieval
////@begin ProjectWindow bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
    ////@end ProjectWindow bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon ProjectsWindow::GetIconResource(const wxString& name)
{
    // Icon retrieval
////@begin ProjectWindow icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
    ////@end ProjectWindow icon retrieval
}

void ProjectsWindow::OnRightClickOverTreeCtrl(wxTreeEvent& event)
{
    
    // We need to find the project based on the file.
    wxTreeItemId tid = event.GetItem();
    wxString text = m_pTreeCtrl->GetItemText(tid);
    Project* pProject{ GetProject(text) };
    while (!pProject && tid.IsOk())
    {
        tid = m_pTreeCtrl->GetItemParent(tid);
        if (tid.IsOk())
        {
            text = m_pTreeCtrl->GetItemText(tid);
            pProject = GetProject(text);
        }
    }

    if (pProject)
    {
        wxString path, file;
        // Now we have detected the project so we can add files.
        wxPoint currentPos = event.GetPoint();
        m_pTreeCtrl->PopupMenu(m_MenuPopUp, currentPos);
        NewFileDlg* pNewFileDlg = new NewFileDlg(nullptr);
        pNewFileDlg->SetFileLocation(pProject->GetProjectDirectory());
        int retCode = pNewFileDlg->ShowModal();
        if (retCode == wxID_CANCEL)
        {
            pNewFileDlg->Destroy();
            return;
        }

        pNewFileDlg->GetFileName(path, file);
        pNewFileDlg->Destroy();
        

        wxAuiNotebook* dockWindows = static_cast<wxAuiNotebook*>(m_pMainFrame->GetWindow("notebook_content"));

        File* pFile = new File(file, path, m_pMainFrame->GetAssembler(), m_pMainFrame->GetLinker(), m_pMainFrame->GetCompiler(), m_pMainFrame->GetFileSettings(), pProject);
        
        CodeEditor* pCodeEditor = new CodeEditor(dockWindows, pFile);

        pCodeEditor->SaveFile(pFile->GetFile() + wxFileName::GetPathSeparator() + pFile->GetFileName());
        m_pMainFrame->AddFile(pFile,pCodeEditor);

        dockWindows->Freeze();

        dockWindows->AddPage(pCodeEditor, file);

        dockWindows->Thaw();

        //m_pWindowManager->Update();

        m_pTreeCtrl->AppendItem(tid, file);
    }
}


Project* ProjectsWindow::GetProject(const wxString& text)
{
    for (Project* pProject : m_pMainFrame->GetProjects())
    {
        if (pProject->GetName() == text)
        {
            return pProject;
        }
    }

    return nullptr;
}