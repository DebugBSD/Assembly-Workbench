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
#include <sstream>
#include "CodeEditor.h"
#include "SettingsDialog.h"
#include "resource.h"
#include "Main.h"
#include "File.h"
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
    EVT_MENU(wxID_SAVE, MainFrame::OnSave)
    EVT_MENU(wxID_NEW, MainFrame::OnNew)
    EVT_MENU(wxID_CLOSE, MainFrame::OnClose)
    EVT_MENU(wxID_PREFERENCES, MainFrame::OnHello)
    EVT_MENU(ID_Tools_Command_Line, MainFrame::OnCMDTool)


    EVT_MENU(ID_Build_Build_Solution, MainFrame::OnBuildSolution)
    EVT_MENU(ID_Build_Rebuild_Solution, MainFrame::OnRebuildSolution)
    EVT_MENU(ID_Build_Clean_Solution, MainFrame::OnCleanSolution)

    EVT_AUINOTEBOOK_PAGE_CLOSE(ID_Notebook, MainFrame::OnCloseTab)

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
    m_pCompilerBase{nullptr}
{
    InitToolChain();

    // Layout
    m_mgr.SetManagedWindow(this);

    // Set Icon
    // SetIcon();

    m_notebook_style = wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER;

    //SetBackgroundColour(wxColour(0x12, 0x12, 0x12));
    //SetForegroundColour(wxColour(0xCC, 0x99, 0xFF));
    //ShowFullScreen(true); // Show the window maximized but, without any close, minimize or maximize button.
    Maximize(true); // Show the window maximized
    
    CreateMenubar();

    CreateStatusBar();
    GetStatusBar()->SetStatusText("Welcome to wxWidgets!");

    // Add toolbars
    wxAuiToolBar* tb1 = CreateMainToolBar();

    // Add panels
    m_mgr.AddPane(CreateTreeCtrl(), wxAuiPaneInfo().
        Name("TreeControl").Caption("Projects").
        Left().Layer(1).Position(1).
        CloseButton(true).MaximizeButton(true));

    // Create bottom panel
    wxTextCtrl* wnd10 = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
        wxPoint(0, 0), FromDIP(wxSize(150, 90)),
        wxNO_BORDER | wxTE_MULTILINE);

    m_mgr.AddPane(wnd10, wxAuiPaneInfo().
        Name("ConsoleLog").Caption("Console Output").
        Bottom().Layer(1).Position(1));

    // create center panels
    m_mgr.AddPane(CreateNotebook(), wxAuiPaneInfo().Name("notebook_content").
        CenterPane().PaneBorder(false));

    // add toolbars to the manager
    m_mgr.GetPane("TreeControl").Show().Left().Layer(0).Row(0).Position(0);
    m_mgr.GetPane("ConsoleLog").Show().Bottom().Layer(0).Row(0).Position(0);
    m_mgr.GetPane("notebook_content").Show();
    m_mgr.AddPane(tb1, wxAuiPaneInfo().Name("tb1").Caption("File Toolbar").ToolbarPane().Top());

    /*Bind(wxEVT_MENU, &MainFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_SIZE, &MainFrame::OnResize, this);*/

    // "commit" all changes made to wxAuiManager
    m_mgr.Update();
}

wxTreeCtrl* MainFrame::CreateTreeCtrl()
{
    wxTreeCtrl* tree = new wxTreeCtrl(this, wxID_ANY,
        wxPoint(0, 0),
        FromDIP(wxSize(160, 250)),
        wxTR_DEFAULT_STYLE | wxNO_BORDER);

    /*wxSize size = FromDIP(wxSize(16, 16));
    wxImageList* imglist = new wxImageList(size.x, size.y, true, 2);
    imglist->Add(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER, size));
    imglist->Add(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, size));
    tree->AssignImageList(imglist);

    wxTreeItemId root = tree->AddRoot("Carone Engine", 0);
    wxArrayTreeItemIds items;


    items.Add(tree->AppendItem(root, "include", 0));
    items.Add(tree->AppendItem(root, "src", 0));


    int i, count;
    for (i = 0, count = items.Count(); i < count; ++i)
    {
        wxTreeItemId id = items.Item(i);
        tree->AppendItem(id, "Main", 1);
        tree->AppendItem(id, "Render", 1);
        tree->AppendItem(id, "OpenGL", 1);
        tree->AppendItem(id, "Vulkan", 1);
        tree->AppendItem(id, "DirectX", 1);
    }


    tree->Expand(root);*/

    return tree;
}

wxAuiNotebook* MainFrame::CreateNotebook()
{
    // create the notebook off-window to avoid flicker
    wxSize client_size = GetClientSize();

    wxAuiNotebook* ctrl = new wxAuiNotebook(this, ID_Notebook,
        wxPoint(client_size.x, client_size.y),
        FromDIP(wxSize(430, 200)),
        m_notebook_style);
    ctrl->Freeze();

    /*wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, FromDIP(wxSize(16, 16)));

    ctrl->AddPage(new CodeEditor(ctrl, wxEmptyString), "main.asm");

    ctrl->AddPage(new CodeEditor(ctrl, wxEmptyString), "Render.asm");

    ctrl->AddPage(new CodeEditor(ctrl, wxEmptyString), "OpenGL.asm");*/


    /*
    ctrl->AddPage(new wxTextCtrl(ctrl, wxID_ANY, "Some more text",
        wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxNO_BORDER), "wxTextCtrl 1");

    ctrl->AddPage(new wxTextCtrl(ctrl, wxID_ANY, "Some more text",
        wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxNO_BORDER), "wxTextCtrl 2");

    ctrl->AddPage(new wxTextCtrl(ctrl, wxID_ANY, "Some more text",
        wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxNO_BORDER), "wxTextCtrl 3");
    */
    ctrl->Thaw();
    return ctrl;
}

MainFrame::~MainFrame()
{
    UnInitToolChain();
    m_mgr.UnInit();

}

void MainFrame::OnExit(wxCommandEvent& event)
{
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
    
    wxAuiNotebook* ctrl = static_cast<wxAuiNotebook*>(m_mgr.GetPane("notebook_content").window);
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
                m_mgr.Update();
            }
            else
            {
                if (!pCodeEditor->SaveFile(pFile->GetFile()+'/'+ pFile->GetFileName()))
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

void MainFrame::OnOpen(wxCommandEvent& event)
{
    wxFileDialog
        openFileDialog(this, _("Open Assembly file"), "", "",
            "ASM files (*.asm)|*.asm", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    std::filesystem::path tempFile{ static_cast<std::string>(openFileDialog.GetPath()) };

    File* pFile = new File(tempFile.filename().string(),tempFile.parent_path().string(), m_pAssemblerBase, m_pLinkerBase, m_pCompilerBase);
    CodeEditor* pCodeEditor = new CodeEditor(this, pFile);
    if (!pCodeEditor->LoadFile(openFileDialog.GetPath()))
    {
        wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
        return;
    }
    wxAuiNotebook* ctrl = static_cast<wxAuiNotebook*>(m_mgr.GetPane("notebook_content").window);

    m_Files.insert({ pFile,pCodeEditor });

    ctrl->AddPage(pCodeEditor, tempFile.filename().string());
}

void MainFrame::OnNew(wxCommandEvent& event)
{
    wxAuiNotebook* ctrl = static_cast<wxAuiNotebook*>(m_mgr.GetPane("notebook_content").window);

    File* pFile = new File("New File", m_pAssemblerBase, m_pLinkerBase, m_pCompilerBase);
    CodeEditor* pCodeEditor = new CodeEditor(ctrl, pFile);
    
    ctrl->Freeze();

    m_Files.insert({ pFile,pCodeEditor });

    ctrl->AddPage(pCodeEditor, "New File");
    
    ctrl->Thaw();

    m_mgr.Update();
}

void MainFrame::OnClose(wxCommandEvent& event)
{
    int res = CloseFile();
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

    Destroy();

}

void MainFrame::OnCMDTool(wxCommandEvent& event)
{
    // Note: X64
    wxExecute("cmd.exe /k \"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build/vcvars64.bat\"");
    // Note: X32 - wxExecute("cmd.exe /k \"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/Tools/VsDevCmd.bat\"");
    //wxExecute("cmd.exe /k \"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/Tools/VsDevCmd.bat\"&&dir");
}

void MainFrame::OnBuildSolution(wxCommandEvent& event)
{
    wxAuiNotebook* ctrl = static_cast<wxAuiNotebook*>(m_mgr.GetPane("notebook_content").window);
    if (ctrl)
    {
        CodeEditor* pCodeEditor = static_cast<CodeEditor*>(ctrl->GetCurrentPage());
        if (pCodeEditor && pCodeEditor->GetFile())
        {
            pCodeEditor->GetFile()->Assemble();
            pCodeEditor->GetFile()->Compile();
            pCodeEditor->GetFile()->Link();
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

void MainFrame::OnCloseTab(wxAuiNotebookEvent& event)
{
    int res = CloseFile();
    if (res == wxYES) // We save the file before closing it
    {
        event.Skip();
    }
    else if (res == wxNO) // We close without any saving.
    {
        event.Skip();
    }
    else // We return to the program.
    {
        event.Veto();
    }
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

    // Caracter actual (sin tab)
    // TODO
    //if(GetStatusBar())
    //    GetStatusBar()->SetStatusText(statusText);
}

void MainFrame::Log(wxArrayString* pArrayLog)
{

    wxTextCtrl* pTextControl = static_cast<wxTextCtrl*>(m_mgr.GetPane("ConsoleLog").window);
    for (wxString str : *pArrayLog)
    {
        pTextControl->AppendText(str + "\n");
    }
}

void MainFrame::Log(wxString* pError)
{
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
//              -----------------------------------
//              Custom
//          Linker
//              ML
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
    /*menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
        "Help string shown in status bar for this menu item");*/
    menuFile->Append(wxID_NEW);
    // TODO: Submenu 

    menuFile->Append(wxID_OPEN);
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
}

void MainFrame::UnInitToolChain()
{
    delete m_pLinkerBase;
    delete m_pAssemblerBase;
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

int MainFrame::CloseFile()
{
    wxAuiNotebook* ctrl = static_cast<wxAuiNotebook*>(m_mgr.GetPane("notebook_content").window);
    if (ctrl)
    {
        CodeEditor* pCodeEditor = static_cast<CodeEditor*>(ctrl->GetCurrentPage());
        // If File is Modified
        if (pCodeEditor && pCodeEditor->IsModified())
        {
            File* pFile = pCodeEditor->GetFile();
            if (pFile->GetFile() != "") // There is a file to save
            {
                int res = wxMessageBox(_("This file has been modifed. Do you want to save it before closing it?"), _("Please confirm"), wxYES_NO | wxCANCEL, this);
                if (res == wxYES) // We save the file before closing it
                {
                    pCodeEditor->SaveFile(pFile->GetFile() + '/' + pFile->GetFileName());
                    // Delete this elemento from m_Files.
                    m_Files.erase(pCodeEditor->GetFile());
                    
                }
                return res;
            }
            else // There is no file to save to.
            {
                int res = wxMessageBox(_("This buffer has been modifed. All changes will be lost! Do you want to save it before saving it?"), _("Please confirm"), wxYES_NO | wxCANCEL, this);

                wxFileDialog
                    saveFileDialog(this, _("Save Assembly file"), "", "",
                        "ASM files (*.asm)|*.asm", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

                if (res == wxYES)
                {
                    if (saveFileDialog.ShowModal() == wxID_CANCEL)
                        return wxCANCEL;     // the user changed idea...

                    std::filesystem::path tempFile{ static_cast<std::string>(saveFileDialog.GetPath()) };
                    pFile->SetFile(tempFile.parent_path().string());
                    pFile->SetFileName(tempFile.filename().string());
                    m_Files.erase(pCodeEditor->GetFile());
                    if (!pCodeEditor->SaveFile(static_cast<wxString>(tempFile)))
                    {
                        wxLogError("Cannot save current contents in file '%s'.", saveFileDialog.GetPath());
                        return -1;
                    }
                }
                else
                {
                    m_Files.erase(pCodeEditor->GetFile());
                }
                return res;
            }
        }
        else
        {
            return wxYES;
        }
    }
    else
    {

    }
    return -1;
}

void MainFrame::OnHello(wxCommandEvent& event)
{
    SettingsDialog* pSettings = new SettingsDialog(nullptr);
    
    pSettings->ShowModal();

    pSettings->Destroy();

    std::stringstream ss;
    ss << "PATH=" << getenv("PATH");
    //ss << ";" << plugin_path;
    //ss << ";" << lib_path;
    ss << '\0';
    std::string env = ss.str();

}
