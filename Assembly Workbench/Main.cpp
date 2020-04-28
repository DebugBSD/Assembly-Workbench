// TestWxWidgets.cpp : Define el punto de entrada de la aplicación.
//

#include "stdafx.h"
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include <sstream>
#include "CodeEditor.h"
#include "SettingsDialog.h"
#include "resource.h"
#include "Main.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

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
    m_pmgr{nullptr}
{
    // Set Icon
    // SetIcon();
    SetBackgroundColour(wxColour(0x12, 0x12, 0x12));
    SetForegroundColour(wxColour(0xCC, 0x99, 0xFF));
    //ShowFullScreen(true); // Show the window maximized but, without any close, minimize or maximize button.
    Maximize(true); // Show the window maximized
    
    CreateMenubar();

    CreateMainToolBar();

    // Layout
    //m_pmgr = new wxAuiManager(this);

    // Look at the page 63 - Mini-Frames of the book of WxWidgets to see how to use wxMiniFrame or wxMDIParentFrame.
    // Look at samples/mdi

    // Here is the Editor
    m_pCodeEditor = new CodeEditor(this) ;

    m_pStatusBar = CreateStatusBar();
    m_pStatusBar->SetStatusText("Welcome to wxWidgets!");

    /*Bind(wxEVT_MENU, &MainFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_SIZE, &MainFrame::OnResize, this);*/

}

MainFrame::~MainFrame()
{
    delete m_pCodeEditor;
    if(m_pmgr) m_pmgr->UnInit();
    delete m_pmgr;

}
void MainFrame::OnExit(wxCommandEvent& event)
{
    
    Close(true);
}
void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
        "About Hello World", wxOK | wxICON_INFORMATION);
}
void MainFrame::OnResize(wxSizeEvent& event)
{
    int stop = 1;
    event.Skip();
}

void MainFrame::OnSave(wxCommandEvent& event)
{
    wxFileDialog
        saveFileDialog(this, _("Save Assembly file"), "", "",
            "ASM files (*.asm)|*.asm", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    // save the current contents in the file;
    if (!m_pCodeEditor->SaveFile(saveFileDialog.GetPath()))
    {
        wxLogError("Cannot save current contents in file '%s'.", saveFileDialog.GetPath());
        return;
    }
}

void MainFrame::OnOpen(wxCommandEvent& event)
{

    bool isModified = m_pCodeEditor->IsModified();
    if (isModified)
    {
        if (wxMessageBox(_("Current content has not been saved! Proceed?"), _("Please confirm"),
            wxICON_QUESTION | wxYES_NO, this) == wxNO)
            return;
        //else: proceed asking to the user the new file to open
    }

    wxFileDialog
        openFileDialog(this, _("Open Assembly file"), "", "",
            "ASM files (*.asm)|*.asm", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    m_pCodeEditor->Destroy();
    m_pCodeEditor = new CodeEditor(this, openFileDialog.GetPath());
    if (!m_pCodeEditor->LoadFile(openFileDialog.GetPath()))
    {
        wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
        return;
    }
}

void MainFrame::OnNew(wxCommandEvent& event)
{
    if (!m_pCodeEditor->IsModified())
    {
        m_pCodeEditor->Clear();
    }
    else
    {
        if (wxMessageBox(_("This file has been modifed. All changes will be lost!"), _("Please confirm"),
            wxICON_QUESTION | wxYES_NO, this) == wxNO)
            return;
        //else: proceed asking to the user the new file to open
        m_pCodeEditor->Clear();
    }

}

void MainFrame::OnClose(wxCommandEvent& event)
{
    if (!m_pCodeEditor->IsModified())
    {
        m_pCodeEditor->Clear();
    }
    else
    {
        if (wxMessageBox(_("This file has been modifed. All changes will be lost!"), _("Please confirm"),
            wxICON_QUESTION | wxYES_NO, this) == wxNO)
            return;
        //else: proceed asking to the user the new file to open
        m_pCodeEditor->Clear();
    }
}

void MainFrame::OnExitProgram(wxCloseEvent& event)
{
    if (event.CanVeto() && m_pCodeEditor->IsModified())
    {
        if (wxMessageBox(_("This file has been modifed. All changes will be lost!"), _("Please confirm"),
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
    wxExecute("cmd.exe /k \"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/Tools/VsDevCmd.bat\"");
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
    if(m_pStatusBar)
        m_pStatusBar->SetStatusText(statusText);
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
    menuProject->Append(ID_Project_Preferences, "Preferences", "Edit project preferences");

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

void MainFrame::CreateMainToolBar()
{
    // Create a Toolbar
    HANDLE hndOpen = LoadImageA(wxGetInstance(), MAKEINTRESOURCEA(IDI_ICON1), 1, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
    wxIcon iconOpen;
    iconOpen.CreateFromHICON((WXHICON)hndOpen);

    HANDLE hndSave = LoadImageA(wxGetInstance(), MAKEINTRESOURCEA(IDI_ICON2), 1, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
    wxIcon iconSave;
    iconSave.CreateFromHICON((WXHICON)hndSave);

    HANDLE hndSaveAll = LoadImageA(wxGetInstance(), MAKEINTRESOURCEA(IDI_ICON3), 1, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
    wxIcon iconSaveAll;
    iconSaveAll.CreateFromHICON((WXHICON)hndSaveAll);

    HANDLE hndNew = LoadImageA(wxGetInstance(), MAKEINTRESOURCEA(IDI_ICON4), 1, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE);
    wxIcon iconNew;
    iconNew.CreateFromHICON((WXHICON)hndNew);

    int stop = 1;
    //wxToolBar* pToolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize , wxTB_DEFAULT_STYLE);
    //pToolBar->AddTool(wxID_OPEN, "Open", icon, "Open");
    wxToolBar* pToolBar = CreateToolBar();
    pToolBar->AddTool(wxID_NEW, "New", iconNew, "New file");
    pToolBar->AddTool(wxID_OPEN, "Open", iconOpen, "Open");
    pToolBar->AddTool(wxID_SAVE, "Save", iconSave, "Save current");
    pToolBar->AddTool(ID_Save_Project, "Save Project", iconSaveAll, "Save current project");
    // CreateToolBar()
    // SetToolBar()
    pToolBar->Realize();
    SetToolBar(pToolBar);
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