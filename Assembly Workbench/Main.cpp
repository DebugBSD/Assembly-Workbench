// TestWxWidgets.cpp : Define el punto de entrada de la aplicación.
//

#include "stdafx.h"
#include <sstream>
#include "CodeEditor.h"
#include "SettingsDialog.h"
#include "Main.h"

wxIMPLEMENT_APP(MyApp);

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
    EVT_MENU(ID_Hello, MainFrame::OnHello)
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

    CreateToolBar();

    // Layout


    //m_pmgr = new wxAuiManager(this);

    // Look at the page 63 - Mini-Frames of the book of WxWidgets to see how to use wxMiniFrame or wxMDIParentFrame.
    // Look at samples/mdi

    // Here is the TextBox
    CodeEditor* text = new CodeEditor(this);
    
    //m_pmgr->AddPane(text, wxALL,"Text Editor");
    //m_pmgr->Update();
    // Just for color format
    /*text->SetDefaultStyle(wxTextAttr(*wxRED));
    text->AppendText("Red text\n");
    text->SetDefaultStyle(wxTextAttr(wxNullColour, *wxLIGHT_GREY));
    text->AppendText("Red on grey text\n");
    text->SetDefaultStyle(wxTextAttr(*wxBLUE));
    text->AppendText("Blue on grey text\n");*/
    

    m_pStatusBar = CreateStatusBar();
    m_pStatusBar->SetStatusText("Welcome to wxWidgets!");
    /*Bind(wxEVT_MENU, &MainFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_SIZE, &MainFrame::OnResize, this);*/

}

MainFrame::~MainFrame()
{
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

    m_pStatusBar->SetStatusText(statusText);
}

/*****************************************************************************/
// Menus
//      File
//          New
//              File
//              Project
//          Open
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
//              Build Preferences
//      View
//          Files 
//          Functions
//          Variables
//          Opcodes
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
    // Todo

    wxMenu* menuBuild = new wxMenu;
    menuBuild->Append(ID_Build_Build_Solution, "Build solution", "Build current solution");
    menuBuild->Append(ID_Build_Rebuild_Solution, "Rebuild solution", "Rebuild current solution");
    menuBuild->Append(ID_Build_Clean_Solution, "Clean solution", "Clean current solution");

    wxMenu* menuDebug = new wxMenu;


    wxMenu* menuTools = new wxMenu;
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

void MainFrame::CreateToolBar()
{
    // Create a Toolbar
    // CreateToolBar()
    // SetToolBar()
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

    wxExecute("explorer.exe");
}