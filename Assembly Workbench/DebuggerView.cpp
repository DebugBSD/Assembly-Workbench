#include "stdafx.h"
#include "DebuggerView.h"

#include "Main.h"

wxBEGIN_EVENT_TABLE(DebuggerView, wxPanel)
	EVT_TEXT_ENTER(ID_OutputTextControl, DebuggerView::OnEnter)
	EVT_IDLE(DebuggerView::OnIdle)
wxEND_EVENT_TABLE()

DebuggerView::DebuggerView()
{
	Init();
}

DebuggerView::DebuggerView(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
	Init();
	Create(parent, id, caption, pos, size, style);
}

bool DebuggerView::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
	wxPanel::Create(parent, id, pos, size, style);

	CreateControls();
	//SetMinSize(size);
	//Centre();

	return true;
}

DebuggerView::~DebuggerView()
{
}

void DebuggerView::Init()
{
}

void DebuggerView::CreateControls()
{
	MainFrame* pMainFrame{ static_cast<MainFrame*>(wxTheApp->GetTopWindow()) };
	wxBoxSizer* pSizer = new wxBoxSizer(wxVERTICAL);

	m_pOutTextCtrl = new wxTextCtrl(this, ID_InputTextControl, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxNO_BORDER | wxTE_READONLY);
	m_pOutTextCtrl->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
	m_pOutTextCtrl->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
	m_pOutTextCtrl->SetFocus();
	m_pOutTextCtrl->ShowNativeCaret(false);
	pSizer->Add(m_pOutTextCtrl, 1, wxEXPAND, 0);

	m_pInTextCtrl = new wxTextCtrl(this, ID_OutputTextControl, wxEmptyString, wxDefaultPosition, wxSize(-1, 16), wxTE_PROCESS_ENTER | wxNO_BORDER | wxTE_NO_VSCROLL);
	m_pInTextCtrl->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
	m_pInTextCtrl->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
	pSizer->Add(m_pInTextCtrl, 0, wxEXPAND, 0);

	SetSizer(pSizer);
	Layout();
}

void DebuggerView::Clear()
{
}

void DebuggerView::InitDebugSession(const wxString& program)
{
	if (m_pProc) {
		ShutdownDebugSession();
	}

	m_pProc = new wxProcess(this, ID_CDBProcess);

	// We have to call to wincdb.exe which is located into - "\"C:\\Program Files (x86)\\Windows Kits\\10\\Debuggers\\x64\\cdb.exe""
	wxString pathToDebugger = "\"C:\\Program Files (x86)\\Windows Kits\\10\\Debuggers\\x64\\windbg.exe\"";
	wxString p = pathToDebugger + " " + program;

	long pid = wxExecute(p, wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE, m_pProc);
	if (pid == 0) // Some error happened
	{
		delete m_pProc;
		m_pProc = nullptr;
		wxLogError("ERROR running the program");
	}
	else
	{
		/*m_pErrorStream = m_pProc->GetErrorStream();
		m_pInputStream = m_pProc->GetInputStream();
		m_pOutputStream = m_pProc->GetOutputStream();*/
	}
}

void DebuggerView::ShutdownDebugSession()
{
	// TODO: Maybe, we should kill the child process if it's not terminated yet.
	delete m_pProc;
	m_pProc = nullptr;
}

void DebuggerView::OnEnter(wxCommandEvent& event)
{
	if (m_pOutputStream)
	{
		wxTextOutputStream tStream(*m_pOutputStream);
		tStream << m_pInTextCtrl->GetValue() + "\n";
	}
	m_pInTextCtrl->Clear();
}

void DebuggerView::OnIdle(wxIdleEvent& evt)
{
	if (m_pProc)
	{
		wxTextInputStream tStream(*m_pErrorStream);
		m_pOutTextCtrl->AppendText(tStream.ReadLine());
	}

	if (m_pProc)
	{
		wxTextInputStream tStream(*m_pInputStream);
		m_pOutTextCtrl->AppendText(tStream.ReadLine() + "\n");
	}
}

void DebuggerView::OnTerminate(wxProcessEvent& event)
{
	delete m_pProc;
	m_pProc = nullptr;
}