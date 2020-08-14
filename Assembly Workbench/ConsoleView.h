#pragma once
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/log.h>

class ConsoleView :
    public wxPanel
{
public:
    /// Constructors
    ConsoleView();
    ConsoleView(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& caption = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL);

    bool Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& caption = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL);

    /// Destructor
    ~ConsoleView();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

    void Clear();
private:

    wxTextCtrl* m_pTextCtrl;
    wxLogTextCtrl* m_pLogTextCtrl;
};

