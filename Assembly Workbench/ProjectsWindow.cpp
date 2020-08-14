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

#include "wx/imaglist.h"



/*
 * ProjectWindow event table definition
 */

wxBEGIN_EVENT_TABLE(ProjectsWindow, wxPanel)

    // Tree control events
    EVT_TREE_ITEM_RIGHT_CLICK(ID_TreeCtrl_Projects_View, ProjectsWindow::OnRightClickOverTreeCtrl)
    EVT_TREE_ITEM_ACTIVATED(ID_TreeCtrl_Projects_View, ProjectsWindow::SelectedElement)
    EVT_MENU(ID_Project_View_Add_New_File, ProjectsWindow::OnPopupNewFile)
    EVT_MENU(ID_Project_View_Add_New_Folder, ProjectsWindow::OnPopupNewFolder)

wxEND_EVENT_TABLE()

    /*
     * ProjectWindow constructors
     */

ProjectsWindow::ProjectsWindow()
{
    Init();
}

ProjectsWindow::ProjectsWindow(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style):
    m_pMainFrame{ static_cast<MainFrame*>(wxTheApp->GetTopWindow()) }
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * ProjectWindow creator
 */

bool ProjectsWindow::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
    wxPanel::Create(parent, id, pos, size, style);

    CreateControls();
    //SetMinSize(size);
    //Centre();

    return true;
}


/*
 * ProjectWindow destructor
 */

ProjectsWindow::~ProjectsWindow()
{
}


/*
 * Member initialisation
 */

void ProjectsWindow::Init()
{
}


/*
 * Control creation for ProjectWindow
 */

void ProjectsWindow::CreateControls()
{

    MainFrame* pMainFrame{ static_cast<MainFrame*>(wxTheApp->GetTopWindow()) };

    ProjectsWindow* itemFrame1 = this;

    wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer1);

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer1->Add(itemBoxSizer2, 1, wxGROW | wxALL, 0);

    m_pSearchCtrl = new wxSearchCtrl(itemFrame1, ID_PROJECTS_WINDOW_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    m_pSearchCtrl->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
    m_pSearchCtrl->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
    itemBoxSizer2->Add(m_pSearchCtrl, 0, wxGROW | wxALL, 0);

    m_pTreeCtrl = new wxTreeCtrl(itemFrame1, ID_TreeCtrl_Projects_View, wxDefaultPosition, wxSize(100, 100), wxTR_DEFAULT_STYLE | wxNO_BORDER);
    m_pTreeCtrl->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
    m_pTreeCtrl->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
    m_pTreeCtrl->AddRoot("Projects");
    itemBoxSizer2->Add(m_pTreeCtrl, 1, wxGROW | wxALL, 0);

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
     * TODO: Documentation says that is best practice to add elements to the tree when user
     * expands an item and delete them when user collapses the item, but,right now, we just
     * add them.
     * In the future, change this as documentation says.
     */

    wxTreeItemId root = m_pTreeCtrl->GetRootItem();
    wxTreeItemId rootProject = m_pTreeCtrl->AppendItem(root, pProject->GetName());
    m_TreeProjects.insert({pProject, new TFolder(pProject->GetName(), rootProject)});
    // Add the rest of the files
    for (File* pFile : pProject->GetFiles())
    {
        wxTreeItemId nodeTid = m_pTreeCtrl->AppendItem(rootProject, pFile->GetFileName());
        m_pTreeFiles.insert({ nodeTid.GetID(), pFile });
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
    wxUnusedVar(name);
    return wxNullBitmap;
}

/*
 * Get icon resources
 */

wxIcon ProjectsWindow::GetIconResource(const wxString& name)
{
    wxUnusedVar(name);
    return wxNullIcon;
}

void ProjectsWindow::SelectedElement(wxTreeEvent& event)
{
    wxTreeItemId id = event.GetItem();
    File* pFile = m_pTreeFiles[id.GetID()];
    if (pFile)
    {
        wxAuiNotebook* dockWindows = m_pMainFrame->GetWindow();
        
        for (size_t i = 0; i < dockWindows->GetPageCount(); i++)
        {
            CodeEditor* pWindow = m_pMainFrame->GetCodeEditor(pFile);
            if (dockWindows->GetPage(i) == pWindow)
            {
                return;
            }
        }

        CodeEditor* pCodeEditor = new CodeEditor(dockWindows, pFile);
        pCodeEditor->LoadFile(pFile->GetAbsoluteFileName());
        m_pMainFrame->AddFile(pFile, pCodeEditor);

        dockWindows->Freeze();

        dockWindows->AddPage(pCodeEditor, pFile->GetFileName());

        dockWindows->Thaw();
    }
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
        wxMenu menuPopUp = wxMenu();
        menuPopUp.Append(ID_Project_View_Add_New_File, "Add New File");
        menuPopUp.Append(ID_Project_View_Add_New_Folder, "Add New Folder");

        m_pSelectedProjectTid = tid;
        m_pSelectedProject = pProject;
        // Now we have detected the project so we can add files.
        wxPoint currentPos = event.GetPoint();
        m_pTreeCtrl->PopupMenu(&menuPopUp, currentPos);
    }
}

void ProjectsWindow::OnPopupNewFile(wxCommandEvent& event)
{
    wxString path, file;
    NewFileDlg* pNewFileDlg = new NewFileDlg(nullptr);
    pNewFileDlg->SetFileLocation(m_pSelectedProject->GetProjectDirectory());
    int retCode = pNewFileDlg->ShowModal();
    if (retCode == wxID_CANCEL)
    {
        pNewFileDlg->Destroy();
        return;
    }

    pNewFileDlg->GetFileName(path, file);
    pNewFileDlg->Destroy();


    wxAuiNotebook* dockWindows = m_pMainFrame->GetWindow();

    File* pFile = new File(path + wxFileName::GetPathSeparator() + file, m_pMainFrame->GetAssembler(), m_pMainFrame->GetLinker(), m_pMainFrame->GetCompiler(), m_pMainFrame->GetFileSettings(), m_pSelectedProject);

    CodeEditor* pCodeEditor = new CodeEditor(dockWindows, pFile);

    pCodeEditor->SaveFile(pFile->GetFile().GetFullPath());
    m_pMainFrame->AddFile(pFile, pCodeEditor);

    dockWindows->Freeze();

    dockWindows->AddPage(pCodeEditor, file);

    dockWindows->Thaw();

    //m_pWindowManager->Update();

    wxTreeItemId nodeTid = m_pTreeCtrl->AppendItem(m_pSelectedProjectTid, file);
    m_pTreeFiles.insert({ nodeTid.GetID(), pFile });

}

void ProjectsWindow::OnPopupNewFolder(wxCommandEvent& event)
{
    int stop = 1;
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