// TestWxWidgets.cpp : Define el punto de entrada de la aplicación.
//

#include "stdafx.h"
#include "CodeEditor.h"
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

    //ShowFullScreen(true); // Show the window maximized but, without any close, minimize or maximize button.
    Maximize(true); // Show the window maximized
    
    CreateMenubar();

    CreateToolBar();

    // Layout


    //m_pmgr = new wxAuiManager(this);

    // Look at the page 63 - Mini-Frames of the book of WxWidgets to see how to use wxMiniFrame or wxMDIParentFrame.
    // Look at samples/mdi

    //wxPanel* pPanel = new wxPanel(this, wxID_ANY, { 0,0 }, GetSize() / 2);
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
    

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
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

void MainFrame::CreateMenubar()
{
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
        "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
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
    wxLogMessage("Hello world from wxWidgets!");
}