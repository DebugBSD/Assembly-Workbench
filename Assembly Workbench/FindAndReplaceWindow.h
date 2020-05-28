#pragma once
#include <wx/panel.h>
#include <wx/dataview.h>

/*!
 * Control identifiers
 */

#define ID_FINDANDREPLACEWINDOW 10000
#define ID_FIND_COMBOBOX 10001
#define ID_REPLACE_COMBOBOX 10002
#define ID_CASE_SENSITIVE_CBOX 10003
#define ID_COMPLETE_WORDS_CBOX 10004
#define ID_REG_EX_CBOX 10005
#define ID_COMBOBOX2 10006
#define ID_CHECKBOX 10016
#define ID_FIND_IN_DIRECTORY_BTN 10007
#define ID_COMBOBOX3 10008
#define ID_FIND_BEFORE_BTN 10009
#define ID_FIND_NEXT_BTN 10010
#define ID_OMIT_FILE_BTN 10011
#define ID_REPLACE_NEXT_BTN 10012
#define ID_REPLACE_ALL 10013
#define ID_FIND_ALL 10014
#define ID_FIND_AND_REPLACE_RESULTS_TREECTRL 10015
#define SYMBOL_FINDANDREPLACEWINDOW_STYLE wxDV_SINGLE | wxNO_BORDER
#define SYMBOL_FINDANDREPLACEWINDOW_TITLE _("Find and Replace")
#define SYMBOL_FINDANDREPLACEWINDOW_IDNAME ID_FINDANDREPLACEWINDOW
#define SYMBOL_FINDANDREPLACEWINDOW_SIZE wxSize(640, 480)
#define SYMBOL_FINDANDREPLACEWINDOW_POSITION wxDefaultPosition


/*!
 * FindAndReplaceWindow class declaration
 */
class FindAndReplaceWindow :
	public wxPanel
{
public:
    /// Constructors
    FindAndReplaceWindow();
    FindAndReplaceWindow(wxWindow* parent, wxWindowID id = SYMBOL_FINDANDREPLACEWINDOW_IDNAME, const wxString& caption = SYMBOL_FINDANDREPLACEWINDOW_TITLE, const wxPoint& pos = SYMBOL_FINDANDREPLACEWINDOW_POSITION, const wxSize& size = SYMBOL_FINDANDREPLACEWINDOW_SIZE, long style = SYMBOL_FINDANDREPLACEWINDOW_STYLE);

    bool Create(wxWindow* parent, wxWindowID id = SYMBOL_FINDANDREPLACEWINDOW_IDNAME, const wxString& caption = SYMBOL_FINDANDREPLACEWINDOW_TITLE, const wxPoint& pos = SYMBOL_FINDANDREPLACEWINDOW_POSITION, const wxSize& size = SYMBOL_FINDANDREPLACEWINDOW_SIZE, long style = SYMBOL_FINDANDREPLACEWINDOW_STYLE);

    /// Destructor
    ~FindAndReplaceWindow();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

        /// Retrieves bitmap resources
    wxBitmap GetBitmapResource(const wxString& name);

    /// Retrieves icon resources
    wxIcon GetIconResource(const wxString& name);

        /// Should we show tooltips?
    static bool ShowToolTips();

private:
    class wxComboBox* m_FindComboBox;
    class wxComboBox* m_ReplaceComboBox;
    class wxButton* m_FindInDirectory;
    class wxDataViewCtrl* m_pSearchResultTreeCtrl;
    wxObjectDataPtr<class SearchModel> m_searchModel;
private:
    // Event table methods
    void OnFindAllBtnClicked(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};

