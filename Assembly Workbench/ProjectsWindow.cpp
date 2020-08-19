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
#include "wx/wxprec.h"
#include <wx/dir.h>

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
    EVT_TREE_ITEM_EXPANDED(ID_TreeCtrl_Projects_View, ProjectsWindow::OnItemExpanded)
    EVT_TREE_ITEM_EXPANDING(ID_TreeCtrl_Projects_View, ProjectsWindow::OnItemBeingExpanded)
    EVT_TREE_BEGIN_LABEL_EDIT(ID_TreeCtrl_Projects_View, ProjectsWindow::OnBeginEditLabel)
    EVT_TREE_END_LABEL_EDIT(ID_TreeCtrl_Projects_View, ProjectsWindow::OnEndEditLabel)
    EVT_MENU(ID_Project_View_Add_New_File, ProjectsWindow::OnPopupNewFile)
    EVT_MENU(ID_Project_View_Add_New_Folder, ProjectsWindow::OnPopupNewFolder)
    EVT_MENU(ID_Project_View_Delete_File, ProjectsWindow::OnPopupDeleteFile)
    EVT_MENU(ID_Project_View_Delete_Folder, ProjectsWindow::OnPopupDeleteFolder)

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
    m_NewFolder = false;
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

    m_pTreeCtrl = new wxTreeCtrl(itemFrame1, ID_TreeCtrl_Projects_View, wxDefaultPosition, wxSize(100, 100), wxTR_DEFAULT_STYLE | wxNO_BORDER | wxTR_EDIT_LABELS);
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

    wxTreeItemId root = m_pTreeCtrl->GetRootItem();
    wxTreeItemId rootProject = m_pTreeCtrl->AppendItem(root, pProject->GetName());
    TElement* pRoot = pProject->GetProjectFiles();
    pRoot->m_id = rootProject;

    for (TElement* pElem : static_cast<TFolder*>(pRoot)->m_Elements)
    {
        if (pElem->m_Type == TElementFile) // File
        {
            wxTreeItemId nodeTid = m_pTreeCtrl->AppendItem(rootProject, static_cast<TFile*>(pElem)->m_pFile->GetFileName());
            pElem->m_id = nodeTid;
        }
        else if (pElem->m_Type == TElementFolder) // Folder
        {
            wxTreeItemId nodeTid = m_pTreeCtrl->AppendItem(rootProject, static_cast<TFolder*>(pElem)->m_Name);
            m_pTreeCtrl->SetItemHasChildren(nodeTid);
            pElem->m_id = nodeTid;
        }
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

    Project* pProject{ m_pMainFrame->GetProjects()[0] };
    TElement* pRoot = m_pMainFrame->GetProjects()[0]->GetProjectFiles();

    TElement* pCurrentElement = GetElement(pRoot, id);
    if (pCurrentElement == nullptr && pProject->GetProjectFiles()->m_id == id)
        pCurrentElement = pProject->GetProjectFiles();
    if (pCurrentElement->m_Type == TElementFile)
    {
        File* pFile = static_cast<TFile*>(pCurrentElement)->m_pFile;

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
        static_cast<TFile*>(pCurrentElement)->m_pCodeEditor = pCodeEditor;
        m_pMainFrame->AddFile(pFile, pCodeEditor);

        dockWindows->Freeze();

        dockWindows->AddPage(pCodeEditor, pFile->GetFileName());

        dockWindows->Thaw();

        int stop = 1;
    }
}

void ProjectsWindow::OnRightClickOverTreeCtrl(wxTreeEvent& event)
{
    
    // We need to find the project based on the file.
    wxTreeItemId tid = event.GetItem();
    wxString text = m_pTreeCtrl->GetItemText(tid);
    Project* pProject{ GetProject(tid) };
    
    if (pProject)
    {
        TElement* pCurrentElement = GetElement(pProject->GetProjectFiles(),tid);
        if (pCurrentElement == nullptr && pProject->GetProjectFiles()->m_id == tid)
            pCurrentElement = pProject->GetProjectFiles();

        wxMenu menuPopUp = wxMenu();
        menuPopUp.Append(ID_Project_View_Add_New_File, "Add New File");
        if (pCurrentElement && pCurrentElement->m_Type == TElementFolder)
        {
            menuPopUp.Append(ID_Project_View_Add_New_Folder, "Add New Folder");
            menuPopUp.Append(ID_Project_View_Delete_Folder, "Delete Folder");
        }
        else if(pCurrentElement && pCurrentElement->m_Type == TElementFile)
        {
            menuPopUp.Append(ID_Project_View_Delete_File, "Delete File");
        }

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

    wxTreeItemId nodeTid = m_pTreeCtrl->AppendItem(m_pSelectedProjectTid, file);
    TElement* pCurrentElement = GetElement(m_pSelectedProject->GetProjectFiles(), m_pSelectedProjectTid);
    if (pCurrentElement == nullptr && m_pSelectedProject->GetProjectFiles()->m_id == m_pSelectedProjectTid)
        pCurrentElement = m_pSelectedProject->GetProjectFiles();
    if (pCurrentElement != nullptr)
    {
        TFile* pTFile = new TFile(pFile, nodeTid, pCurrentElement->m_pRoot, pCurrentElement);
        static_cast<TFolder*>(pCurrentElement)->m_Elements.push_front(pTFile);
        pTFile->m_pCodeEditor = pCodeEditor;
        pTFile->m_pFile = pFile;
    }

}

void ProjectsWindow::OnPopupNewFolder(wxCommandEvent& event)
{
    wxTreeItemId newFolderId = m_pTreeCtrl->AppendItem(m_pSelectedProjectTid, "New Folder");
    TElement* pElement = GetElement(m_pSelectedProject->GetProjectFiles(), m_pSelectedProjectTid);
    if (pElement == nullptr && m_pSelectedProject->GetProjectFiles()->m_id == m_pSelectedProjectTid)
        pElement = m_pSelectedProject->GetProjectFiles();
    if (pElement != nullptr)
    {
        static_cast<TFolder*>(pElement)->m_Elements.push_back(new TFolder("New Folder", newFolderId, pElement->m_pRoot, pElement));
        m_pTreeCtrl->EditLabel(newFolderId);
        m_NewFolder = true;
    }
}

void ProjectsWindow::OnPopupDeleteFolder(wxCommandEvent& event)
{
    TElement* pElement = GetElement(m_pSelectedProject->GetProjectFiles(), m_pSelectedProjectTid);
    if (pElement != nullptr && pElement->m_Type == TElementFolder)
    {
        // Rename folder
        wxString folderName = pElement->m_Name;

        // Get Path ...
        TElement* pParent = pElement->m_pParent;
        wxString relPath;
        while (pParent != nullptr && pParent->m_Name != "Root")
        {
            relPath = pParent->m_Name + wxFileName::GetPathSeparator() + relPath;
            pParent = pParent->m_pParent;
        }

        wxString folder = m_pMainFrame->GetProjects()[0]->GetProjectDirectory() + wxFileName::GetPathSeparator() + relPath + wxFileName::GetPathSeparator() + folderName;

        wxFileName fName = folder;

        fName.Normalize();

        folder = fName.GetFullPath();
        wxArrayString files;
        if (wxDir::GetAllFiles(folder, &files))
        {
            // We should ask about deleting contents from directory before removing the directory.
            int res = wxMessageBox(_("This forlder is not empty. Do you want to continue?"), _("Please confirm"), wxYES_NO, this);
            if (res == wxYES) // We save the file before closing it
            {
                if (wxDir::Remove(folder, wxPATH_RMDIR_RECURSIVE))
                {
                    // We have to delete the contents of the directory.
                    m_pTreeCtrl->Delete(pElement->m_id);
                    delete pElement;
                }
            }
        }
        else
        {
            if (wxDir::Remove(folder))
            {
                m_pTreeCtrl->Delete(pElement->m_id);
                delete pElement;
            }
        }
    }
}

void ProjectsWindow::OnPopupDeleteFile(wxCommandEvent& event)
{
    TElement* pElement = GetElement(m_pSelectedProject->GetProjectFiles(), m_pSelectedProjectTid);
    if (pElement != nullptr && pElement->m_Type == TElementFile)
    {
        File *pFile = static_cast<TFile*>(pElement)->m_pFile;
        // We should check if file is open before removing it from directory
        if (wxRemoveFile(pFile->GetAbsoluteFileName()))
        {
            m_pTreeCtrl->Delete(pElement->m_id);
            delete pElement;
        }
    }
}

TElement* ProjectsWindow::GetElement(TElement *pRootElement, const wxTreeItemId& elem)
{
    for (TElement* pElem : static_cast<TFolder*>(pRootElement)->m_Elements)
    {
        if (pElem->m_id == elem)
        {
            return pElem;
        }
        else if (pElem->m_Type == TElementFolder)
        {
            TElement* pChildren = GetElement(pElem, elem);
            if (pChildren == nullptr)
                continue;
            else
                return pChildren;
        }
        else if (pElem->m_Type == TElementFile)
        {
            continue;
        }
    }

    return nullptr;
}

void ProjectsWindow::AddItems(const wxTreeItemId& elem, TElement* pITem)
{
    for (TElement* pElem : static_cast<TFolder*>(pITem)->m_Elements)
    {
        if (pElem->m_Type == TElementFile) // File
        {
            wxTreeItemId nodeTid = m_pTreeCtrl->AppendItem(elem, static_cast<TFile*>(pElem)->m_pFile->GetFileName());
            pElem->m_id = nodeTid;
        }
        else if (pElem->m_Type == TElementFolder) // Folder
        {
            wxTreeItemId nodeTid = m_pTreeCtrl->AppendItem(elem, static_cast<TFolder*>(pElem)->m_Name);
            m_pTreeCtrl->SetItemHasChildren(nodeTid);
            pElem->m_id = nodeTid;
        }
    }
}

Project* ProjectsWindow::GetProject(const wxTreeItemId& elem)
{
    for (Project* pProject : m_pMainFrame->GetProjects())
    {
        if (pProject->GetProjectFiles()->m_id == elem)
        {
            return pProject;
        }
        else if (GetElement(pProject->GetProjectFiles(),elem))
        {
            return pProject;
        }
    }

    return nullptr;
}


void ProjectsWindow::OnItemExpanded(wxTreeEvent& event)
{
    int stop = 1;
}

void ProjectsWindow::OnItemBeingExpanded(wxTreeEvent& event)
{
    wxTreeItemId currentItem = event.GetItem();

    if (m_pTreeCtrl->GetChildrenCount(currentItem) > 0) return;

    TElement *pRoot = m_pMainFrame->GetProjects()[0]->GetProjectFiles();

    TElement* pCurrentElement = GetElement(pRoot,currentItem);

    AddItems(currentItem, pCurrentElement);
}

void ProjectsWindow::OnEndEditLabel(wxTreeEvent& event)
{
    wxTreeItemId currentItem = event.GetItem();

    TElement* pRoot = m_pMainFrame->GetProjects()[0]->GetProjectFiles();
    TElement* pElement = GetElement(pRoot,currentItem);
    if(pElement != nullptr)
    {
        if (pElement->m_Type == TElementFolder)
        {
            // Rename folder
            wxString folderName = pElement->m_Name;
            wxString destName = event.GetLabel();
            if (destName == "")
                destName = folderName;

            // Get Path ...
            TElement* pParent = pElement->m_pParent;
            wxString relPath;
            while (pParent != nullptr && pParent->m_Name != "Root")
            {
                relPath = pParent->m_Name + wxFileName::GetPathSeparator() + relPath;
                pParent = pParent->m_pParent;
            }

            wxString sourceDirectory = m_pMainFrame->GetProjects()[0]->GetProjectDirectory() + wxFileName::GetPathSeparator() + relPath + wxFileName::GetPathSeparator() + folderName;
            wxString destinationDirectory = m_pMainFrame->GetProjects()[0]->GetProjectDirectory() + wxFileName::GetPathSeparator() + relPath + wxFileName::GetPathSeparator() + destName;

            if (pElement->m_Name == "New Folder" && !wxFileName::DirExists(sourceDirectory) && !wxFileName::DirExists(destinationDirectory))
            {
                // Its new directory
                bool res = wxFileName::Mkdir(destinationDirectory);
                if (res)
                {
                    pElement->m_Name = destName;
                }
            }
            else
            {
                bool res = wxRenameFile(sourceDirectory, destinationDirectory);
                if (res)
                {
                    pElement->m_Name = destName;
                }
            }

            // Now, we should rename all files which are under this directory.
        }
        else if (pElement->m_Type == TElementFile)
        {
            // Rename file
            wxString fileName = pElement->m_Name;
            wxString destFileName = event.GetLabel();
            if (destFileName == "")
                destFileName = fileName;

            if (destFileName == fileName) return;

            File* pFile = static_cast<TFile*>(pElement)->m_pFile;

            bool res = wxRenameFile(pFile->GetFile().GetFullPath(), pFile->GetFile().GetPath() + wxFileName::GetPathSeparator() + destFileName);
            if (res)
            {
                wxString pathToFile = pFile->GetFile().GetPath();
                pFile->SetFile(pathToFile + wxFileName::GetPathSeparator() + destFileName);
                pElement->m_Name = destFileName;
            }

            int stop = 1;
        }
        else
        {

        }
    }

}

void ProjectsWindow::OnBeginEditLabel(wxTreeEvent& event)
{
    wxTreeItemId currentItem = event.GetItem();
    if (currentItem == m_pTreeCtrl->GetRootItem())
    {
        event.Veto();
    }
    else
    {
        for (Project* pProject : m_pMainFrame->GetProjects())
        {
            if (pProject->GetProjectFiles()->m_id == currentItem)
            {
                event.Veto();
            }
        }
    }
}
