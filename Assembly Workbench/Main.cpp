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
#include "resource.h"
#include "Main.h"
#include "File.h"
#include "Project.h"
// Toolchain of Microsoft
// NOTE: Right now is hardcoded. In the future, will be autodetected.
#include "MASM.h"
#include "MLINKER.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

// Size of icons into toolbars and wxWidgets.
constexpr int ICON_SIZE = 16;

wxIMPLEMENT_APP(MyApp);

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
    EVT_MENU(wxID_OPEN, MainFrame::OnOpen)
    EVT_MENU(wxID_CUT, MainFrame::OnEdit)
    EVT_MENU(wxID_COPY, MainFrame::OnEdit)
    EVT_MENU(wxID_PASTE, MainFrame::OnEdit)

    EVT_MENU(ID_Open_Project, MainFrame::OnOpenProject)
    EVT_MENU(wxID_SAVE, MainFrame::OnSave)
    EVT_MENU(wxID_NEW, MainFrame::OnNew)
    EVT_MENU(ID_New_File, MainFrame::OnNewFile)
    EVT_MENU(ID_New_Project, MainFrame::OnNewProject)
    EVT_MENU(wxID_CLOSE, MainFrame::OnClose)
    EVT_MENU(ID_Tools_Command_Line, MainFrame::OnCMDTool)

    // View
    EVT_MENU(ID_View_LineNumber, MainFrame::OnModifySettings)
    EVT_MENU(ID_View_LongLine, MainFrame::OnModifySettings)
    EVT_MENU(ID_View_CaretLine, MainFrame::OnModifySettings)

    // Project
    EVT_MENU(ID_Project_Preferences, MainFrame::OnProjectPreferences)

    // Build
    EVT_MENU(ID_Build_Build_Solution, MainFrame::OnBuildSolution)
    EVT_MENU(ID_Build_Rebuild_Solution, MainFrame::OnRebuildSolution)
    EVT_MENU(ID_Build_Clean_Solution, MainFrame::OnCleanSolution)

    EVT_MENU(ID_Debug_LaunchWinDbg, MainFrame::OnLaunchDebugger)

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
    wxFrame(NULL, wxID_ANY, "Assembly Workbench", { 0,0 }, { 1280, 1000 }),
    m_pAssemblerBase{nullptr},
    m_pLinkerBase{nullptr},
    m_pCompilerBase{nullptr},
    m_pGlobalFileSettings{nullptr},
    m_pWindowManager{new WindowManager()}
{
    InitToolChain();

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

    // Layout
    m_pWindowManager->SetManagedWindow(this);

    // Set Icon
    // SetIcon();

    m_notebook_style = wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER;

    //SetBackgroundColour(wxColour(0x12, 0x12, 0x12));
    //SetForegroundColour(wxColour(0xCC, 0x99, 0xFF));
    //ShowFullScreen(true); // Show the window maximized but, without any close, minimize or maximize button.
    Maximize(true); // Show the window maximized
    
    CreateMenubar();

    m_pStatusBar = CreateStatusBar(2);
    m_pStatusBar->SetStatusText("Welcome to Assembly Workbench!",0);

    // Add toolbars
    wxAuiToolBar* tb1 = CreateMainToolBar();

    // Add panels
    m_pWindowManager->AddPane(new ProjectsWindow(this), wxAuiPaneInfo().
        Name("ProjectsWindow").Caption("Projects").
        Left().Layer(0).Row(0).Position(0).
        CloseButton(true).MaximizeButton(true));

    // Add panels
    m_pWindowManager->AddPane(new FindAndReplaceWindow(this), wxAuiPaneInfo().
        Name("FindAndReplaceWindow").Caption("Find And Replace").
        Right().Layer(0).Row(0).Position(0).
        CloseButton(true).MaximizeButton(true));

    // Create bottom panel
    wxTextCtrl* wnd10 = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
        wxPoint(0, 0), FromDIP(wxSize(150, 90)),
        wxNO_BORDER | wxTE_MULTILINE);

    m_pWindowManager->AddPane(wnd10, wxAuiPaneInfo().
        Name("ConsoleLog").Caption("Console Output").
        Bottom().Layer(1).Position(1).
        CloseButton(true).MaximizeButton(true));

    //EditorsWindow* pEditor = new EditorsWindow(this);
    // create center panels
    m_pWindowManager->AddPane(new EditorsWindow(this), wxAuiPaneInfo().Name("notebook_content").
        CenterPane().PaneBorder(false));

    m_pWindowManager->AddPane(tb1, wxAuiPaneInfo().Name("tb1").Caption("File Toolbar").ToolbarPane().Top());

    /*Bind(wxEVT_MENU, &MainFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_SIZE, &MainFrame::OnResize, this);*/

    // "commit" all changes made to wxAuiManager
    m_pWindowManager->Update();
}

MainFrame::~MainFrame()
{
    UnInitToolChain();
    m_pWindowManager->UnInit();

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
void MainFrame::OnSave(wxCommandEvent& event)
{
    wxFileDialog
        saveFileDialog(this, _("Save Assembly file"), "", "",
            "ASM files (*.asm)|*.asm", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    
    wxAuiNotebook* ctrl = static_cast<wxAuiNotebook*>(m_pWindowManager->GetPane("notebook_content").window);
    if (ctrl)
    {
        CodeEditor* pCodeEditor = static_cast<CodeEditor*>(ctrl->GetCurrentPage());
        if (pCodeEditor && pCodeEditor->IsModified())
        {
            File* pFile = pCodeEditor->GetFile();

            if (pFile && pFile->GetFile() == "")
            {
                if (saveFileDialog.ShowModal() == wxID_CANCEL)
                    return;     // the user changed idea...
                
                std::filesystem::path tempFile{ static_cast<std::string>(saveFileDialog.GetPath()) };
                pFile->SetFile(tempFile.parent_path().string());
                pFile->SetFileName(tempFile.filename().string());

                if (!pCodeEditor->SaveFile(static_cast<wxString>(tempFile)))
                {
                    wxLogError("Cannot save current contents in file '%s'.", saveFileDialog.GetPath());
                    return;
                }
                int pageNum = ctrl->GetPageIndex(ctrl->GetCurrentPage());
                ctrl->Freeze();
                ctrl->SetPageText(pageNum, pFile->GetFileName());
                ctrl->Thaw();
                m_pWindowManager->Update();
            }
            else
            {
                if (!pCodeEditor->SaveFile(pFile->GetFile()+ wxFileName::GetPathSeparator() + pFile->GetFileName()))
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

void MainFrame::OnOpenProject(wxCommandEvent &event)
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
        ProjectsWindow* ctrl = static_cast<ProjectsWindow*>(m_pWindowManager->GetPane("ProjectsWindow").window);
        // Add the project and files and everything to the main tree.
        if (ctrl) ctrl->AddProject(pProject);

        // Add the project to the vector of projects.
        m_Projects.push_back(pProject);
    }
}

void MainFrame::OnOpen(wxCommandEvent& event)
{
    wxFileDialog
        openFileDialog(this, _("Open Assembly file"), "", "",
            "ASM and Include files (*.asm;*.inc)|*.asm;*.inc", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    std::filesystem::path tempFile{ static_cast<std::string>(openFileDialog.GetPath()) };

    File* pFile = new File(tempFile.filename().string(),tempFile.parent_path().string(), m_pAssemblerBase, m_pLinkerBase, m_pCompilerBase, m_pGlobalFileSettings);
    CodeEditor* pCodeEditor = new CodeEditor(this, pFile);
    if (!pCodeEditor->LoadFile(openFileDialog.GetPath()))
    {
        wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
        return;
    }
    wxAuiNotebook* ctrl = static_cast<wxAuiNotebook*>(m_pWindowManager->GetPane("notebook_content").window);

    m_Files.insert({ pFile,pCodeEditor });

    ctrl->AddPage(pCodeEditor, tempFile.filename().string());
}

void MainFrame::OnNew(wxCommandEvent& event)
{
    wxAuiNotebook* ctrl = static_cast<wxAuiNotebook*>(m_pWindowManager->GetPane("notebook_content").window);

    File* pFile = new File("New File", m_pAssemblerBase, m_pLinkerBase, m_pCompilerBase, m_pGlobalFileSettings);
    CodeEditor* pCodeEditor = new CodeEditor(ctrl, pFile);
    
    ctrl->Freeze();

    m_Files.insert({ pFile,pCodeEditor });

    ctrl->AddPage(pCodeEditor, "New File");
    
    ctrl->Thaw();

    m_pWindowManager->Update();
}

void MainFrame::OnNewProject(wxCommandEvent& event)
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

        int res = pProject->Create(pNewProjectDlg->GetDirectory()+ wxFileName::GetPathSeparator() +pNewProjectDlg->GetProjectName(), pNewProjectDlg->GetFileName());

        if (res == 0)
        {
            // TODO: WindowManager has the control over this window, so, pass to WindowManager instead of accessing this control in this way.
            ProjectsWindow* ctrl = static_cast<ProjectsWindow*>(m_pWindowManager->GetPane("ProjectsWindow").window);
            // Add the project and files and everything to the main tree.
            if(ctrl) ctrl->AddProject(pProject);

            // Add the project to the vector of projects.
            m_Projects.push_back(pProject);
        }
    }

    pNewProjectDlg->Destroy();
}

void MainFrame::OnNewFile(wxCommandEvent& event)
{
    
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

void MainFrame::OnCMDTool(wxCommandEvent& event)
{
    // Note: X64
    wxExecute("cmd.exe /k \"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build/vcvars64.bat\"");
    // Note: X32 - wxExecute("cmd.exe /k \"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/Tools/VsDevCmd.bat\"");
    //wxExecute("cmd.exe /k \"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/Tools/VsDevCmd.bat\"&&dir");
}

void MainFrame::OnEdit(wxCommandEvent& event)
{
    wxAuiNotebook* ctrl = static_cast<wxAuiNotebook*>(m_pWindowManager->GetPane("notebook_content").window);
    if (ctrl)
    {
        CodeEditor* pCodeEditor = static_cast<CodeEditor*>(ctrl->GetCurrentPage());
        if (pCodeEditor)
        {
            pCodeEditor->GetEventHandler()->ProcessEvent(event);
        }
    }
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
}

void MainFrame::OnBuildSolution(wxCommandEvent& event)
{
    // Compilamos el proyecto actual unica y exclusivamente cuando solo tenemos un proyecto abierto 
    // y ninguna de las tabs abiertas pertenecen a ficheros de fuera.
    if (m_Projects.size() == 1)
    {
        wxAuiNotebook* ctrl = static_cast<wxAuiNotebook*>(m_pWindowManager->GetPane("notebook_content").window);
        if (ctrl)
        {
            bool buildProject = true;
            for (int i = 0; i < ctrl->GetPageCount(); i++)
            {
                CodeEditor* pCodeEditor = static_cast<CodeEditor*>(ctrl->GetPage(i));
                if (pCodeEditor->GetFile() != nullptr && pCodeEditor->GetFile()->GetProject() != m_Projects[0])
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
                        file.second->SaveFile(pFile->GetFile() + wxFileName::GetPathSeparator().operator char() + pFile->GetFileName());
                    }
                }
                pProject->Build();
                return;
            }
        }
    }

    wxAuiNotebook* ctrl = static_cast<wxAuiNotebook*>(m_pWindowManager->GetPane("notebook_content").window);
    if (ctrl)
    {
        CodeEditor* pCodeEditor = static_cast<CodeEditor*>(ctrl->GetCurrentPage());
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
                        file.second->SaveFile(pFile->GetFile() + wxFileName::GetPathSeparator().operator char() + pFile->GetFileName());
                    }
                }
                pProject->Build();
            }
        }
    }
}

void MainFrame::OnRebuildSolution(wxCommandEvent& event)
{
    int stop = 1;
}

void MainFrame::OnCleanSolution(wxCommandEvent& event)
{
    int stop = 1;
}

void MainFrame::OnLaunchDebugger(wxCommandEvent& event)
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

    m_pStatusBar->SetStatusText(statusText,1);

}

void MainFrame::Log(wxArrayString* pArrayLog)
{

    wxTextCtrl* pTextControl = static_cast<wxTextCtrl*>(m_pWindowManager->GetPane("ConsoleLog").window);
    for (wxString str : *pArrayLog)
    {
        pTextControl->AppendText(str + "\n");
    }
}

void MainFrame::Log(wxString* pError)
{
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
void MainFrame::CreateMenubar()
{
    wxMenu* menuFile = new wxMenu;
    wxMenu* pSubMenuFile = new wxMenu;
    /*menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
        "Help string shown in status bar for this menu item");*/
    pSubMenuFile->Append(wxID_NEW);                                     // Open a new tab with an empty buffer to start editing.
    pSubMenuFile->Append(ID_New_File, "New File");                      // Open a new window 
    pSubMenuFile->Append(ID_New_Project, "New Project");                // Open a new Window 
    menuFile->AppendSubMenu(pSubMenuFile, "New", "New File/Project");
    // TODO: Submenu 

    wxMenu* pSubMenuOpen = new wxMenu;
    pSubMenuOpen->Append(wxID_OPEN);
    pSubMenuOpen->Append(ID_Open_Project, "Open Project", "Open an existing project");
    menuFile->AppendSubMenu(pSubMenuOpen, "Open", "Open a File or Project");
    menuFile->Append(ID_Clone, "Clone a repository", "Clone a repository");

    menuFile->AppendSeparator();
    menuFile->Append(wxID_CLOSE);
    menuFile->Append(ID_Close_Project, "Close Project", "Close current project");

    menuFile->AppendSeparator();
    menuFile->Append(wxID_SAVE);
    menuFile->Append(ID_Save_Project, "Save Project", "Save current project");
    
    menuFile->AppendSeparator();
    menuFile->Append(ID_Recent_Files, "Recent Files", "Most recently used files");
    menuFile->Append(ID_Recent_Projects, "Recent Projects", "Most recently used projects");
    
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuEdit = new wxMenu;

    menuEdit->Append(ID_Search_Replace, "Search and Replace", "Search and replace in current project or file");

    menuEdit->AppendSeparator();
    menuEdit->Append(wxID_UNDO);
    menuEdit->Append(wxID_REDO);

    menuEdit->AppendSeparator();
    menuEdit->Append(wxID_CUT);
    menuEdit->Append(wxID_COPY);
    menuEdit->Append(wxID_PASTE);

    menuEdit->AppendSeparator();
    menuEdit->Append(wxID_PREFERENCES);

    wxMenu* menuView = new wxMenu;

    menuView->Append(ID_View_Files, "Files", "View current files");
    menuView->Append(ID_View_Functions, "Functions", "View current functions");
    menuView->Append(ID_View_Variables, "Variables", "View current variables");
    menuView->Append(ID_View_Opcodes, "Opcodes", "View current opcodes");
    menuView->Append(ID_View_LineNumber, "View Line Numbers");
    menuView->Append(ID_View_LongLine, "View End Line");
    menuView->Append(ID_View_CaretLine, "Highlight Current Line");
    
    

    wxMenu* menuProject = new wxMenu;
    menuProject->Append(ID_Project_Assembler, "Assembler", "Assembler to use when building the project");
    // TODO: Add assemblers.

    menuProject->AppendSeparator();
    menuProject->Append(ID_Project_Linker, "Linker", "Linker to use when building the project");
    // TODO: Add linkers.

    menuProject->AppendSeparator();
    menuProject->Append(ID_Project_Preferences, "Settings", "Edit project settings");

    wxMenu* menuBuild = new wxMenu;
    menuBuild->Append(ID_Build_Build_Solution, "Build solution", "Build current solution");
    menuBuild->Append(ID_Build_Rebuild_Solution, "Rebuild solution", "Rebuild current solution");
    menuBuild->Append(ID_Build_Clean_Solution, "Clean solution", "Clean current solution");

    wxMenu* menuDebug = new wxMenu;
    menuDebug->Append(ID_Debug_LaunchWinDbg, "Launch WindDbg", "Launch the debugger.");

    wxMenu* menuTools = new wxMenu;
    menuTools->Append(ID_Tools_Command_Line, "Command Line Tool", "Open a command line tool");
    menuTools->Append(ID_Tools_Hex_Editor, "HexEditor", "Open an Hexadecimal Editor");
    menuTools->Append(ID_Tools_CVS, "CVS", "Open Control Version System window");
    menuTools->Append(ID_Tool_Graph, "Graph", "Open the Graph tool");


    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuEdit, "&Edit");
    menuBar->Append(menuView, "&View");
    menuBar->Append(menuProject, "&Project");
    menuBar->Append(menuBuild, "&Build");
    menuBar->Append(menuDebug, "&Debug");
    menuBar->Append(menuTools, "&Tools");
    menuBar->Append(menuHelp, "&Help");


    SetMenuBar(menuBar);
}

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

