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

// TestWxWidgets.cpp : Define el punto de entrada de la aplicación.
//

#include "stdafx.h"

#include <filesystem>

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include "wx/treectrl.h"
#include "wx/artprov.h"
#include "wx/aui/aui.h"
#include "wx/stdpaths.h"
#include "wx/textfile.h"
#include <sstream>
#include "WindowManager.h"
#include "CodeEditor.h"
#include "SettingsDialog.h"
#include "FileSettingsDlg.h"
#include "NewProjectDlg.h"
#include "NewFileDlg.h"
#include "EditorsWindow.h"
#include "ProjectsWindow.h"
#include "FindAndReplaceWindow.h"
#include "ConsoleView.h"
#include "resource.h"
#include "Main.h"
#include "File.h"
#include "Project.h"

#include "AWMenuFile.h"
#include "AWMenuEdit.h"
#include "AWMenuBuild.h"
#include "AWMenuTools.h"
#include "AWMenuDebug.h"

// Toolchain of Microsoft
// NOTE: Right now is hardcoded. In the future, will be autodetected.
#include "MASM.h"
#include "MLINKER.h"
#include "AWButton.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

// Size of icons into toolbars and wxWidgets.
constexpr int ICON_SIZE = 16;

wxIMPLEMENT_APP(MyApp);

wxBEGIN_EVENT_TABLE(MainFrame, CustomFrame)
    // Minimize button
    EVT_BUTTON(ID_FRAME_BUTTON_MINIMIZE, MainFrame::OnMinimizeBtn)
    // Maximize button
    EVT_BUTTON(ID_FRAME_BUTTON_MAXIMIZE, MainFrame::OnMaximizeBtn)
    // Close button
    EVT_BUTTON(wxID_EXIT, MainFrame::OnExit)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)

    EVT_BUTTON(ID_Menu_File, MainFrame::OnMenuFile)
    EVT_BUTTON(ID_Menu_Edit, MainFrame::OnMenuEdit)
    EVT_BUTTON(ID_Menu_Build, MainFrame::OnMenuBuild)
    EVT_BUTTON(ID_Menu_Tools, MainFrame::OnMenuTools)
    EVT_BUTTON(ID_Menu_Debug, MainFrame::OnMenuDebug)
    EVT_AUINOTEBOOK_PAGE_CHANGED(ID_TAB_MANAGER, MainFrame::OnPageChanged)

    EVT_CLOSE(MainFrame::OnExitProgram)
wxEND_EVENT_TABLE()

bool MyApp::OnInit()
{
    MainFrame* frame = new MainFrame();
    frame->Show(true);
    return true;
}

int MyApp::OnExit()
{

    return EXIT_SUCCESS;
}

MainFrame::MainFrame(): 
    CustomFrame(NULL, wxID_ANY, "Assembly Workbench", wxDefaultPosition, wxSize(1280, 1000), wxNO_BORDER | wxCLIP_CHILDREN),
    m_pAppSettings{ new AppSettings() },
    m_pAssemblerBase{nullptr},
    m_pLinkerBase{nullptr},
    m_pCompilerBase{nullptr},
    m_pGlobalFileSettings{nullptr},
    m_Config{ new wxConfig("AssemblyWorkbench") },
    m_pArtProvider{ new AWArtProvider() }/*,
    m_pWindowManager{new WindowManager()}*/
{
    wxInitAllImageHandlers();
    InitToolChain();

    m_pArtProvider->SetColour(m_pAppSettings->m_backgroundColor);
    m_pArtProvider->SetActiveColour(m_pAppSettings->m_activeTabColor);

    SetBackgroundColour(m_pAppSettings->m_backgroundColor);
    // Load cache
    wxString userDir{ wxStandardPaths::Get().GetUserLocalDataDir() };
    if (!wxFileName::DirExists(userDir))
    {
        wxFileName::Mkdir(userDir);
    }

    if (wxFileName::FileExists(userDir + "/dirEntries"))
    {
        wxTextFile entriesFile{ userDir + "/dirEntries" };
        entriesFile.Open();

        for (size_t i = 0; i < entriesFile.GetLineCount(); i++)
        {
            m_ProjectDirectoryEntries.Add(entriesFile[i]);
        }

    }

    // Set Icon
    wxIcon logoIcon;
    wxBitmap bitmap(wxT("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/logo.bmp"), wxBITMAP_TYPE_BMP);
    logoIcon.CopyFromBitmap(bitmap);
    SetIcon(wxIcon(logoIcon));

    //ShowFullScreen(true); // Show the window maximized but, without any close, minimize or maximize button.
    //Maximize(true); // Show the window maximized
    wxBoxSizer* bSizer11;
    bSizer11 = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* bSizer12;
    bSizer12 = new wxBoxSizer(wxVERTICAL);

    /*
     * Init menus and frame buttons
     */
    bSizer12->Add(InitFrameButtons(), 0, wxEXPAND, 0);

    /*
     * Init the toolbar
     */
    bSizer12->Add(InitToolbar(), 0, wxEXPAND, 0);

    wxBoxSizer* bSizer14;
    bSizer14 = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* bSizer16;
    bSizer16 = new wxBoxSizer(wxVERTICAL);


    bSizer16->Add(InitViews(), 1, wxALL | wxEXPAND /*| wxALIGN_CENTER_VERTICAL*/, 5);


    bSizer14->Add(bSizer16, 1, wxEXPAND, 0);


    bSizer12->Add(bSizer14, 1, wxEXPAND, 0);

    wxBoxSizer* bSizer17;
    bSizer17 = new wxBoxSizer(wxVERTICAL);

    m_pStatusBar = new wxStaticText(this, wxID_ANY, wxT("Welcome to Assembly Workbench!"), wxDefaultPosition, wxDefaultSize, 0);
    m_pStatusBar->SetForegroundColour(m_pAppSettings->m_foregroundColor);
    m_pStatusBar->Wrap(-1);
    bSizer17->Add(m_pStatusBar, 0, wxALL, 5);


    bSizer12->Add(bSizer17, 0, 0, 5);


    bSizer11->Add(bSizer12, 1, wxEXPAND, 5);

    this->SetSizer(bSizer11);
    this->Layout();

    // We won't use this anymore until we can close projects ....
    /*wxString lastProjectOpen;
    m_Config->Read("LastProject", &lastProjectOpen);
    if (lastProjectOpen != "")
    {

        Project *pProject = new Project(this);
        
        if (pProject->Load(lastProjectOpen) == 0) {
            m_pProjectWindow->AddProject(pProject);
            m_Projects.push_back(pProject);
            wxLogMessage("Open project: ", lastProjectOpen);
        }
        else
        {
            delete pProject;
            wxLogError("Opening project: ", lastProjectOpen);
        }
    }*/
}

MainFrame::~MainFrame()
{
    UnInitToolChain();
    //m_pWindowManager->UnInit();
    delete m_Config;

}

void MainFrame::OnMenuFile(wxCommandEvent& event)
{
    wxString projectName;
    wxString projectDirectory;
    wxPoint pos = m_pFileBtn->GetScreenPosition();
    pos.y += m_pFileBtn->GetSize().y + 5;
    AWMenuFile menuFile = AWMenuFile(NULL, wxID_ANY, pos, wxDefaultSize, 0, wxEmptyString);
    int option = menuFile.ShowModal();
    if (option == ID_MENU_FILE_NEW_FILE)
    {
        NewFile();
    }
    else if (option == ID_MENU_FILE_NEW_PROJECT)
    {
        NewProject();
    }
    else if (option == ID_MENU_FILE_OPEN_FILE)
    {
        OpenFile();
    }
    else if (option == ID_MENU_FILE_OPEN_PROJECT)
    {
        OpenProject();
    }
    else if (option == ID_MENU_FILE_SAVE)
    {
        SaveCurrent();
    }
    else if (option == ID_MENU_FILE_CLOSE)
    {

    }
    else if (option == ID_MENU_FILE_EXIT)
    {

    }
}

void MainFrame::OnMenuEdit(wxCommandEvent& event)
{
}

void MainFrame::OnMenuBuild(wxCommandEvent& event)
{
    wxString projectName;
    wxString projectDirectory;
    wxPoint pos = m_pBuildMenuBtn->GetScreenPosition();
    pos.y += m_pBuildMenuBtn->GetSize().y + 5;
    AWMenuBuild menuFile = AWMenuBuild(NULL, wxID_ANY, pos, wxDefaultSize, 0, wxEmptyString);
    int option = menuFile.ShowModal();
    if (option == ID_BUILD_SOLUTION_BTN)
    {
        BuildSolution();
    }
    else if (option == ID_REBUILD_SOLUTION_BTN)
    {
        RebuildSolution();
    }
    else if (option == ID_CLEAN_SOLUTION_BTN)
    {
        CleanSolution();
    }
}

void MainFrame::OnMenuTools(wxCommandEvent& event)
{
    wxString projectName;
    wxString projectDirectory;
    wxPoint pos = m_pToolsBtn->GetScreenPosition();
    pos.y += m_pToolsBtn->GetSize().y + 5;
    AWMenuTools menuFile = AWMenuTools(NULL, wxID_ANY, pos, wxDefaultSize, 0, wxEmptyString);
    int option = menuFile.ShowModal();
    if (option == ID_MENU_TOOLS_CMD_LINE_TOOL)
    {
        CMDTool();
    }
    else if (option == ID_MENU_TOOLS_HEX_EDITOR)
    {
        int stop = 1;
    }
    else if (option == ID_MENU_TOOLS_GIT)
    {
        int stop = 1;
    }
    else if (option == ID_MENU_TOOLS_GRAPH)
    {
        int stop = 1;
    }
}

void MainFrame::OnMenuDebug(wxCommandEvent& event)
{
    wxString projectName;
    wxString projectDirectory;
    wxPoint pos = m_pDebugBtn->GetScreenPosition();
    pos.y += m_pDebugBtn->GetSize().y + 5;
    AWMenuDebug menuFile = AWMenuDebug(NULL, wxID_ANY, pos, wxDefaultSize, 0, wxEmptyString);
    int option = menuFile.ShowModal();
    if (option == ID_MENU_DEBUG_LAUNCH_DEBUGGER)
    {
        LaunchDebugger();
    }
    else if (option == ID_MENU_DEBUG_WINDOWS)
    {
        int stop = 1;
    }
}

void MainFrame::OnExit(wxCommandEvent& event)
{
    if(!m_Files.empty())
    {
        if (wxMessageBox(_("There are unsaved changes. All changes will be lost!"), _("Please confirm"),
            wxICON_QUESTION | wxYES_NO, this) == wxNO)
        {
            return;
        }

    }
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event)
{

}

void MainFrame::OnResize(wxSizeEvent& event)
{
    int stop = 1;
    event.Skip();
}

// It save the current file currently edited.
void MainFrame::SaveCurrent(void)
{
    wxFileDialog
        saveFileDialog(this, _("Save Assembly file"), "", "",
            "ASM files (*.asm)|*.asm", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    
    wxAuiNotebook* ctrl = m_auinotebook5;
    if (ctrl)
    {
        CodeEditor* pCodeEditor = GetCodeEditorFromWindow(ctrl->GetCurrentPage());
        if (pCodeEditor && pCodeEditor->IsModified())
        {
            File* pFile = pCodeEditor->GetFile();

            if (pFile && pFile->GetFile().GetPath() == "")
            {
                if (saveFileDialog.ShowModal() == wxID_CANCEL)
                    return;     // the user changed idea...
                
                pFile->SetFileName(saveFileDialog.GetPath());

                if (!pCodeEditor->SaveFile(saveFileDialog.GetPath()))
                {
                    wxLogError("Cannot save current contents in file '%s'.", saveFileDialog.GetPath());
                    return;
                }
                int pageNum = ctrl->GetPageIndex(ctrl->GetCurrentPage());
                ctrl->Freeze();
                ctrl->SetPageText(pageNum, pFile->GetFileName());
                ctrl->Thaw();
                m_auinotebook5->Update();
            }
            else
            {
                if (!pCodeEditor->SaveFile(pFile->GetAbsoluteFileName()))
                {
                    wxLogError("Cannot save current contents in file '%s'.", saveFileDialog.GetPath());
                    return;
                }
            }
        }
        else
        {
            // TODO: Handle errors.
        }
    }
    else
    {
        // TODO: Handle errors.
    }
}

void MainFrame::OpenProject(void)
{
    wxFileDialog
        openFileDialog(this, _("Open Project file"), "", "",
            "Project files (*.awp)|*.awp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    Project* pProject = new Project(this);
    if (pProject->Load(openFileDialog.GetPath()))
    {
        // TODO: Handle error
    }
    else
    {
        // TODO: WindowManager has the control over this window, so, pass to WindowManager instead of accessing this control in this way.
        ProjectsWindow* ctrl = m_pProjectWindow;
        // Add the project and files and everything to the main tree.
        if (ctrl) ctrl->AddProject(pProject);

        // Add the project to the vector of projects.
        m_Projects.push_back(pProject);
        m_Config->Write("LastProject", pProject->GetFileName().GetFullPath());
    }
}

void MainFrame::OpenFile(void)
{
    wxFileDialog
        openFileDialog(this, _("Open Assembly file"), "", "",
            "ASM and Include files (*.asm;*.inc)|*.asm;*.inc", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    File* pFile = new File(openFileDialog.GetPath(), m_pAssemblerBase, m_pLinkerBase, m_pCompilerBase, m_pGlobalFileSettings);
    CodeEditor* pCodeEditor = new CodeEditor(this, pFile);
    if (!pCodeEditor->LoadFile(openFileDialog.GetPath()))
    {
        wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
        return;
    }
    wxAuiNotebook* ctrl = m_auinotebook5;

    m_Files.insert({ pFile,pCodeEditor });

    ctrl->AddPage(pCodeEditor, wxFileName(openFileDialog.GetPath()).GetFullName());
}

void MainFrame::NewFile(void)
{
    wxAuiNotebook* ctrl = m_auinotebook5;
    File* pFile = new File(wxFileName("New File"), m_pAssemblerBase, m_pLinkerBase, m_pCompilerBase, m_pGlobalFileSettings);
    CodeEditor* pCodeEditor = new CodeEditor(ctrl, pFile);
    
    ctrl->Freeze();

    m_Files.insert({ pFile,pCodeEditor });

    ctrl->AddPage(pCodeEditor, "New File");
    
    ctrl->Thaw();
}

void MainFrame::NewProject(void)
{
    NewProjectDlg* pNewProjectDlg = new NewProjectDlg(nullptr, m_ProjectDirectoryEntries);

    if (pNewProjectDlg->ShowModal() == 0)
    {
        bool isFound = false;
        for (wxString projectDirStr : m_ProjectDirectoryEntries)
        {
            if (projectDirStr == pNewProjectDlg->GetDirectory())
            {
                isFound = true;
            }
        }
        if(!isFound) m_ProjectDirectoryEntries.Add(pNewProjectDlg->GetDirectory());

        Project* pProject = new Project(this);

        int res = pProject->Create(pNewProjectDlg->GetDirectory()+ wxFileName::GetPathSeparator() +pNewProjectDlg->GetProjectName() + wxFileName::GetPathSeparator() + pNewProjectDlg->GetFileName());

        if (res == 0)
        {
            // TODO: WindowManager has the control over this window, so, pass to WindowManager instead of accessing this control in this way.
            ProjectsWindow* ctrl = m_pProjectWindow;
            // Add the project and files and everything to the main tree.
            if(ctrl) ctrl->AddProject(pProject);

            // Add the project to the vector of projects.
            m_Projects.push_back(pProject);
        }
    }

    pNewProjectDlg->Destroy();
}

void MainFrame::OnClose(wxCommandEvent& event)
{
    //int res = CloseFile();
    event.Skip();
}

void MainFrame::OnExitProgram(wxCloseEvent& event)
{
    if (event.CanVeto() && !m_Files.empty())
    {
        if (wxMessageBox(_("There are unsaved changes. All changes will be lost!"), _("Please confirm"),
            wxICON_QUESTION | wxYES_NO, this) == wxNO)
        {
            event.Veto();
            return;
        }
    }

    // We save the project configuration
    for (Project* pProject : m_Projects)
    {
        if(pProject->IsModified()) pProject->Save();
    }

    // Save cache
    wxString userDir{ wxStandardPaths::Get().GetUserLocalDataDir() };
    wxTextFile entriesFile;
    bool fileOpen = false;
    if (wxFileName::FileExists(userDir + "/dirEntries"))
    {
        fileOpen = entriesFile.Open(userDir + "/dirEntries");
        entriesFile.Clear();
    }
    else
    {
        fileOpen = entriesFile.Create(userDir + "/dirEntries");
    }

    if (fileOpen)
    {
        for (wxString currentLine : m_ProjectDirectoryEntries)
        {
            entriesFile.AddLine(currentLine);
        }
        entriesFile.Write();
        entriesFile.Close();
    }

    Destroy();

}

void MainFrame::CMDTool(void)
{
    // Note: X64
    wxExecute("cmd.exe /k \"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build/vcvars64.bat\"");
    // Note: X32 - wxExecute("cmd.exe /k \"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/Tools/VsDevCmd.bat\"");
    //wxExecute("cmd.exe /k \"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/Tools/VsDevCmd.bat\"&&dir");
}

void MainFrame::OnEdit(wxCommandEvent& event)
{
#if 0
    wxAuiNotebook* ctrl = static_cast<wxAuiNotebook*>(m_pWindowManager->GetPane("notebook_content").window);
    if (ctrl)
    {
        CodeEditor* pCodeEditor = static_cast<CodeEditor*>(ctrl->GetCurrentPage());
        if (pCodeEditor)
        {
            pCodeEditor->GetEventHandler()->ProcessEvent(event);
        }
    }
#endif
}

void MainFrame::OnModifySettings(wxCommandEvent& event)
{
    if (event.GetId() == ID_View_LineNumber)
    {
        g_CommonPrefs.lineNumberEnable = !g_CommonPrefs.lineNumberEnable;
    }
    else if (event.GetId() == ID_View_LongLine)
    {
        g_CommonPrefs.longLineOnEnable = !g_CommonPrefs.longLineOnEnable;
    }
    else if (event.GetId() == ID_View_CaretLine)
    {
        g_CommonPrefs.caretLineEnable = !g_CommonPrefs.caretLineEnable;
    }

    for (std::pair<File*, CodeEditor *> pFile : m_Files)
    {
        pFile.second->GetEventHandler()->ProcessEvent(event);
    }
}


void MainFrame::OnProjectPreferences(wxCommandEvent& event)
{
#if 0
    wxAuiNotebook* ctrl = static_cast<wxAuiNotebook*>(m_pWindowManager->GetPane("notebook_content").window);
    if (ctrl)
    {
        CodeEditor* pCodeEditor = static_cast<CodeEditor*>(ctrl->GetCurrentPage());
        if (pCodeEditor && pCodeEditor->GetFile())
        {
            FileSettingsDlg* pSettings = new FileSettingsDlg(nullptr, wxID_ANY, "File Settings", wxDefaultPosition, wxSize(640, 480), SYMBOL_FILESETTINGS_STYLE, pCodeEditor->GetFile()->GetFileSettings());

            pSettings->ShowModal();

            pSettings->Destroy();

        }
    }
#endif
}

void MainFrame::BuildSolution(void)
{

    if(m_pConsoleView)
        m_pConsoleView->Clear();

    if (m_Projects.size() == 1)
    {
        wxAuiNotebook* ctrl = m_auinotebook5;
        if (ctrl)
        {
            bool buildProject = true;
            for (int i = 0; i < ctrl->GetPageCount(); i++)
            {
                CodeEditor* pCodeEditor = GetCodeEditorFromWindow(ctrl->GetPage(i));
                if (pCodeEditor != nullptr && pCodeEditor->GetFile() != nullptr && pCodeEditor->GetFile()->GetProject() != m_Projects[0])
                {
                    buildProject = false;
                    break;
                }
            }

            if (buildProject)
            {
                Project* pProject = m_Projects[0];
                // Save all files before build the project
                for (std::pair<File*, CodeEditor*> file : m_Files)
                {
                    if (file.first->GetProject() == pProject && file.second->IsModified())
                    {
                        File* pFile = file.first;
                        file.second->SaveFile(pFile->GetAbsoluteFileName());
                    }
                }
                pProject->Build();
                return;
            }
        }
    }

    wxAuiNotebook* ctrl = m_auinotebook5;
    if (ctrl)
    {
        SaveCurrent();

        CodeEditor* pCodeEditor = GetCodeEditorFromWindow(ctrl->GetCurrentPage());
        if (pCodeEditor && pCodeEditor->GetFile())
        {
            Project* pProject = pCodeEditor->GetFile()->GetProject();
            if (pProject == nullptr)
            {
                pCodeEditor->GetFile()->Assemble();
                pCodeEditor->GetFile()->Compile();
                pCodeEditor->GetFile()->Link();
                
            }
            else
            {
                // Save all files before build the project
                for (std::pair<File*, CodeEditor*> file : m_Files)
                {
                    if (file.first->GetProject() == pProject && file.second->IsModified())
                    {
                        File* pFile = file.first;
                        file.second->SaveFile(pFile->GetAbsoluteFileName());
                    }
                }
                pProject->Build();
            }
        }
    }
}

void MainFrame::RebuildSolution(void)
{
    int stop = 1;
}

void MainFrame::CleanSolution(void)
{
    int stop = 1;
}

void MainFrame::LaunchDebugger(void)
{
    // WinDbg accept an executable as argument so, you can pass the program you have built.
    wxExecute("\"C:\\Program Files (x86)\\Windows Kits\\10\\Debuggers\\x64\\WinDbg.exe\"");
}

void MainFrame::SetStatusBar(size_t totalChars, size_t totalLines, size_t currentColumn, size_t currentLine)
{
    wxString statusText;

    // Longitud total en caracteres
    statusText << " Characters: " << totalChars;

    // Longitud total en lineas
    statusText << " Lines: " << totalLines;

    // Linea actual del cursor
    statusText << " Ln: " << currentLine;

    // Columna actual del cursor
    statusText << " Col: " << currentColumn;

    m_pStatusBar->SetLabelText(statusText);

}

void MainFrame::Log(wxArrayString* pArrayLog)
{
    for (wxString str : *pArrayLog)
    {
        //wxLogWarning(str);
        wxLogMessage(str);
    }
}

void MainFrame::Log(wxString* pError)
{
    //wxLogWarning(*pError);
    wxLogMessage(*pError);
}

CodeEditor* MainFrame::GetCodeEditorFromWindow(wxWindow* window)
{
    CodeEditor* pCodeEditor = nullptr;
    for (const auto& e : m_Files)
    {
        if (e.second == window)
        {
            pCodeEditor = e.second;
        }
    }
    return pCodeEditor;
}

CodeEditor* MainFrame::GetCodeEditor(File* pFile)
{
    CodeEditor *pCodeEditor = nullptr;
    for (const auto& e : m_Files)
    {
        if (e.first == pFile)
        {
            
            pCodeEditor = e.second;
        }
    }
    return pCodeEditor;
}

void MainFrame::GetFiles(std::vector<class CodeEditor*>& outOpenFiles)
{
    for (std::pair<File* , CodeEditor*> openFile : m_Files)
    {
        outOpenFiles.push_back(openFile.second);
    }
}

wxSizer* MainFrame::InitFrameButtons()
{
    wxBoxSizer* bSizer13;

    struct menuInfo {
        AWButton** pButton;
        wxWindowID windowId;
        wxString text;
        wxPoint pos;
        wxSize size;
        long style;
    } menuButtons[] =
    {
        {&m_pFileBtn, ID_Menu_File, wxT("File"), wxDefaultPosition, m_pAppSettings->m_menuSize, wxBORDER_NONE | wxBU_EXACTFIT},
        {&m_pEditBtn, ID_Menu_Edit, wxT("Edit"), wxDefaultPosition, m_pAppSettings->m_menuSize, wxBORDER_NONE | wxBU_EXACTFIT},
        {&m_pViewBtn, wxID_ANY, wxT("View"), wxDefaultPosition, m_pAppSettings->m_menuSize, wxBORDER_NONE | wxBU_EXACTFIT},
        {&m_pProjectBtn, wxID_ANY, wxT("Project"), wxDefaultPosition, m_pAppSettings->m_menuSize, wxBORDER_NONE | wxBU_EXACTFIT},
        {&m_pBuildMenuBtn, ID_Menu_Build, wxT("Build"), wxDefaultPosition, m_pAppSettings->m_menuSize, wxBORDER_NONE | wxBU_EXACTFIT},
        {&m_pDebugBtn, ID_Menu_Debug, wxT("Debug"), wxDefaultPosition, m_pAppSettings->m_menuSize, wxBORDER_NONE | wxBU_EXACTFIT},
        {&m_pToolsBtn, ID_Menu_Tools, wxT("Tools"), wxDefaultPosition, m_pAppSettings->m_menuSize, wxBORDER_NONE | wxBU_EXACTFIT},
        {&m_pHelpBtn, wxID_ANY, wxT("Help"), wxDefaultPosition, m_pAppSettings->m_menuSize, wxBORDER_NONE | wxBU_EXACTFIT},
        {NULL, -1, wxEmptyString, wxPoint(-1, -1), wxSize(-1, -1), 0}
    };

    bSizer13 = new wxBoxSizer(wxHORIZONTAL);

    // Logo

    wxBitmap logo = wxBitmap(wxT("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/jollyroger.png"), wxBITMAP_TYPE_ANY);
    wxStaticBitmap* pBitmap = new wxStaticBitmap(this, wxID_ANY, logo, wxDefaultPosition, wxSize(32, 32), 0);
    bSizer13->Add(pBitmap, 0, 0, 0);

    // Menu buttons
    for (int i = 0; menuButtons[i].pButton != NULL; i++)
    {
        (*menuButtons[i].pButton) = new AWButton(this, menuButtons[i].windowId, menuButtons[i].text, menuButtons[i].pos, menuButtons[i].size, menuButtons[i].style);
        bSizer13->Add(*(menuButtons[i].pButton), 0, wxTOP, 5);
    }

    // Project name 
    m_staticTitle = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(100, 24), wxBORDER_SIMPLE | wxALIGN_CENTRE_HORIZONTAL);
    m_staticTitle->SetFont(wxFont(m_pAppSettings->m_fontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));
    m_staticTitle->SetBackgroundColour(m_pAppSettings->m_activeTabColor);
    m_staticTitle->SetForegroundColour(m_pAppSettings->m_foregroundColor);

    bSizer13->Add(m_staticTitle, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    // Finder
    pSearchCtrl = new wxSearchCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, m_pAppSettings->m_menuSize, wxBORDER_SIMPLE);
    pSearchCtrl->SetBackgroundColour(m_pAppSettings->m_backgroundColor);
    pSearchCtrl->SetForegroundColour(m_pAppSettings->m_foregroundColor);
    bSizer13->Add(pSearchCtrl, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    bSizer13->Add(0, 0, 1, wxEXPAND | wxRIGHT, 0);

    // Top left frame buttons
    m_pMinimizeBtn = new AWButton(this, ID_FRAME_BUTTON_MINIMIZE, wxT("MyButton"), wxDefaultPosition, m_pAppSettings->m_frameIconsSize, wxBORDER_NONE | wxBU_EXACTFIT | wxBU_NOTEXT, wxDefaultValidator, wxButtonNameStr, wxBitmap(wxT("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/Minimize.png"), wxBITMAP_TYPE_ANY));
    bSizer13->Add(m_pMinimizeBtn, 0, wxALL, 5);

    m_pMaximizeBtn = new AWButton(this, ID_FRAME_BUTTON_MAXIMIZE, wxT("MyButton"), wxDefaultPosition, m_pAppSettings->m_frameIconsSize, wxBORDER_NONE | wxBU_EXACTFIT | wxBU_NOTEXT, wxDefaultValidator, wxButtonNameStr, wxBitmap(wxT("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/Maximize.png"), wxBITMAP_TYPE_ANY));
    bSizer13->Add(m_pMaximizeBtn, 0, wxALL, 5);

    m_pCloseBtn = new AWButton(this, wxID_EXIT, wxT("MyButton"), wxDefaultPosition, m_pAppSettings->m_frameIconsSize, wxBORDER_NONE | wxBU_EXACTFIT | wxBU_NOTEXT, wxDefaultValidator, wxButtonNameStr, wxBitmap(wxT("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/Close.png"), wxBITMAP_TYPE_ANY));
    bSizer13->Add(m_pCloseBtn, 0, wxALL, 5);
    return bSizer13;
}

wxSizer* MainFrame::InitToolbar()
{
    struct menuInfo {
        AWButton** pButton;
        wxWindowID windowId;
        wxString text;
        wxPoint pos;
        wxSize size;
        long style;
        wxString tooltip;
    } menuButtons[] =
    {
        {&m_pGoBackBtn, wxID_ANY, wxT("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/1x/baseline_arrow_left_white_18dp.png"), wxDefaultPosition, m_pAppSettings->m_tbIconsSize, wxNO_BORDER | wxBU_EXACTFIT | wxBU_NOTEXT, "Go back"},
        {&m_pGoForwardBtn, wxID_ANY, wxT("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/1x/baseline_arrow_right_white_18dp.png"), wxDefaultPosition, m_pAppSettings->m_tbIconsSize, wxNO_BORDER | wxBU_EXACTFIT | wxBU_NOTEXT, "Go forward"},
        {&m_pNewBtn, wxID_ANY, wxT("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/1x/baseline_create_new_folder_white_18dp.png"), wxDefaultPosition, m_pAppSettings->m_tbIconsSize, wxNO_BORDER | wxBU_EXACTFIT | wxBU_NOTEXT, "New"},
        {&m_pOpenBtn, wxID_ANY, wxT("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/1x/baseline_folder_open_white_18dp.png"), wxDefaultPosition, m_pAppSettings->m_tbIconsSize, wxNO_BORDER | wxBU_EXACTFIT | wxBU_NOTEXT, "Open"},
        {&m_pSaveBtn, wxID_ANY, wxT("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/1x/baseline_save_white_18dp.png"), wxDefaultPosition, m_pAppSettings->m_tbIconsSize, wxNO_BORDER | wxBU_EXACTFIT | wxBU_NOTEXT, "Save"},
        {&m_pUndoBtn, wxID_ANY, wxT("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/1x/baseline_undo_white_18dp.png"), wxDefaultPosition, m_pAppSettings->m_tbIconsSize, wxNO_BORDER | wxBU_EXACTFIT | wxBU_NOTEXT, "Undo"},
        {&m_pRedoBtn, wxID_ANY, wxT("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/1x/baseline_redo_white_18dp.png"), wxDefaultPosition, m_pAppSettings->m_tbIconsSize, wxNO_BORDER | wxBU_EXACTFIT | wxBU_NOTEXT, "Redo"},
        {&m_pPlayBtn, wxID_ANY, wxT("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/1x/baseline_play_arrow_white_18dp.png"), wxDefaultPosition, m_pAppSettings->m_tbIconsSize, wxNO_BORDER | wxBU_EXACTFIT | wxBU_NOTEXT, "Play"},
        {&m_pBuildBtn, wxID_ANY, wxT("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/1x/baseline_build_white_18dp.png"), wxDefaultPosition, m_pAppSettings->m_tbIconsSize, wxNO_BORDER | wxBU_EXACTFIT | wxBU_NOTEXT, "Build"},
        {&m_pSettingsBtn, wxID_ANY, wxT("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/1x/baseline_settings_white_18dp.png"), wxDefaultPosition, m_pAppSettings->m_tbIconsSize, wxNO_BORDER | wxBU_EXACTFIT | wxBU_NOTEXT, "Settings"},
        {&m_pCodeEditorBtn, wxID_ANY, wxT("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/1x/baseline_code_white_18dp.png"), wxDefaultPosition, m_pAppSettings->m_tbIconsSize, wxNO_BORDER | wxBU_EXACTFIT | wxBU_NOTEXT, "Code editor"},
        {NULL, -1, wxEmptyString, wxPoint(-1, -1), wxSize(-1, -1), 0}
    };

    wxBoxSizer* bSizerToolBar;
    bSizerToolBar = new wxBoxSizer(wxHORIZONTAL);

    for (int i = 0; menuButtons[i].pButton != NULL; i++)
    {
        (*menuButtons[i].pButton) = new AWButton(this, menuButtons[i].windowId, wxT("MyButton"), menuButtons[i].pos, menuButtons[i].size, menuButtons[i].style, wxDefaultValidator, wxButtonNameStr, wxBitmap(menuButtons[i].text, wxBITMAP_TYPE_ANY), menuButtons[i].tooltip);
        bSizerToolBar->Add((*menuButtons[i].pButton), 0, wxALL, 4);
    }

    return bSizerToolBar;
}

wxSizer* MainFrame::InitViews()
{
    wxBoxSizer* bSizer2;
    bSizer2 = new wxBoxSizer(wxHORIZONTAL);

    m_auinotebook5 = new wxAuiNotebook(this, ID_TAB_MANAGER, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER);
    wxAuiManager& mgr = const_cast <wxAuiManager&> (m_auinotebook5->GetAuiManager());
    mgr.GetArtProvider()->SetMetric(wxAUI_DOCKART_SASH_SIZE, 2);
    mgr.GetArtProvider()->SetColour(wxAUI_DOCKART_SASH_COLOUR, m_pAppSettings->m_backgroundColor);
    m_auinotebook5->SetArtProvider(m_pArtProvider);

    // Projects view
    m_pProjectWindow = new ProjectsWindow(m_auinotebook5);
    m_auinotebook5->AddPage(m_pProjectWindow, wxT("Project"), false, wxBitmap("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/1x/baseline_clear_all_white_18dp.png", wxBITMAP_TYPE_ANY));

    // Console view
    m_pConsoleView = new ConsoleView(m_auinotebook5);
    m_auinotebook5->AddPage(m_pConsoleView, wxT("Console"), false, wxBitmap("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/1x/baseline_message_white_18dp.png", wxBITMAP_TYPE_ANY));

    // Find and Replace view
    //m_pFindAndReplaceView = new FindAndReplaceWindow(m_auinotebook5);
    //m_auinotebook5->AddPage(m_pFindAndReplaceView, wxT("Find And Replace"), false, wxBitmap("C:/Users/debugg/My Projects/LevelEditor/World Editor Interfaces/icons/1x/baseline_message_white_18dp.png", wxBITMAP_TYPE_ANY));

    // BUG: wxAuiManager from wxAuiNotebook doesn't works as expected or maybe I don't understand how is supposed to work
    /*wxString auiPerspective;
    if (wxConfig::Get()->Read(CFG_AUI_PERSPECTIVE, &auiPerspective)) {
        mgr.LoadPerspective(auiPerspective, false);
    }
    mgr.Update();*/

    bSizer2->Add(m_auinotebook5, 1, wxEXPAND | wxALL, 5);
    return bSizer2;
}

/*****************************************************************************/
// Menus
//      File
//          New
//              File
//              Project
//          Open
//              File
//              Project
//              Binary File (exe, dll, so, o, elf, ...)
//          Clone
//          Close Project
//          Recent Files
//          Recent Solutions or Projects
//          Exit
//      Edit
//          Search & Replace (wxFindReplaceDialog)
//          ---------------------------------------
//          Undo
//          Redo
//          ---------------------------------------
//          Cut
//          Copy
//          Paste
//          ---------------------------------------
//          Settings (wxDialog)
//              Editor
//      View
//          Files 
//          Functions
//          Variables
//          Opcodes
//      Project
//          Assembler
//              MASM (Only Windows)
//              GAS
//              NASM
//              Yasm
//              Fasm
//              Tasm
//              UASM
//              JWASM
//              GoAsm
//              -----------------------------------
//              Custom
//          Linker
//              ML
//              GoLink
//              -----------------------------------
//              Custom
//          ---------------------------------------
//          Preferences
//      Build
//          Build Solution
//          Rebuild Solution
//          Clean Solution
//      Debug - See Visual Studio
//          Launch Debugger
//          Windows
//              Memory
//              Registers
//              Call stack
//      Tools
//          Command Line Tool
//          Hexadecimal Editor
//          SCV
//              git
//          Graph Tool
/*****************************************************************************/

void MainFrame::InitToolChain()
{
    m_pAssemblerBase = new MASM(this);
    m_pLinkerBase = new MLINKER(this);
    m_pGlobalFileSettings = new FileSettings();
}

void MainFrame::UnInitToolChain()
{
    delete m_pLinkerBase;
    delete m_pAssemblerBase;
    delete m_pGlobalFileSettings;
}

wxAuiToolBar* MainFrame::CreateMainToolBar()
{
    int width = ICON_SIZE, height = ICON_SIZE;
    // Create a Toolbar
    HANDLE hndOpen = LoadImageA(wxGetInstance(), MAKEINTRESOURCEA(IDI_ICON1), 1, width, height, LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
    wxIcon iconOpen;
    iconOpen.CreateFromHICON((WXHICON)hndOpen);

    HANDLE hndSave = LoadImageA(wxGetInstance(), MAKEINTRESOURCEA(IDI_ICON2), 1, width, height, LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
    wxIcon iconSave;
    iconSave.CreateFromHICON((WXHICON)hndSave);

    HANDLE hndSaveAll = LoadImageA(wxGetInstance(), MAKEINTRESOURCEA(IDI_ICON3), 1, width, height, LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
    wxIcon iconSaveAll;
    iconSaveAll.CreateFromHICON((WXHICON)hndSaveAll);

    HANDLE hndNew = LoadImageA(wxGetInstance(), MAKEINTRESOURCEA(IDI_ICON4), 1, width, height, LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
    wxIcon iconNew;
    iconNew.CreateFromHICON((WXHICON)hndNew);

    wxAuiToolBar* tb1 = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxAUI_TB_DEFAULT_STYLE);
    tb1->SetToolBitmapSize(FromDIP(wxSize(width, height)));
    tb1->AddTool(wxID_NEW, "New", iconNew);
    tb1->AddTool(wxID_OPEN, "Open",iconOpen);
    tb1->AddTool(wxID_SAVE, "Save", iconSave);
    tb1->AddTool(ID_Save_Project, "Save Project", iconSaveAll);
    tb1->Realize();

    return tb1;
}

void MainFrame::OnMaximizeBtn(wxCommandEvent& event)
{
    Maximize(!IsMaximized());
}

void MainFrame::OnMinimizeBtn(wxCommandEvent& event)
{
    Iconize(!IsIconized());
}

void MainFrame::OnPageChanged(wxAuiNotebookEvent& event)
{
    CodeEditor* pCodeEditor = GetCodeEditorFromWindow(m_auinotebook5->GetCurrentPage());
    if (pCodeEditor)
    {
        File* pFile = pCodeEditor->GetFile();
        if (pFile != nullptr)
        {
            if (pFile->GetProject())
            {
                wxFileName fileName(pFile->GetProject()->GetName());
                m_staticTitle->SetLabelText(fileName.GetFullName());
            }
            else
            {
                wxFileName fileName(pFile->GetFileName());
                m_staticTitle->SetLabelText(fileName.GetFullName());
            }
        }
    }
}
