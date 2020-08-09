#pragma once
#include <wx/button.h>
class AWButton :
    public wxButton
{
public:
    AWButton(wxWindow* parent,
        wxWindowID id,
        const wxString& label = wxEmptyString,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxButtonNameStr,
        const wxBitmap& bitmap = wxNullBitmap,
        const wxString& tooltip = wxEmptyString);

    ~AWButton();

    void SetBackgroundColor(const wxColor& c);
    void SetForegroundColor(const wxColor& c);
    void SetHighlightedColor(const wxColor& c);
private:

    class MainFrame* m_pMainFrame;

    wxBitmap m_Bitmap;
    wxString m_Tooltip;

    wxColor m_BackgroundColor;
    wxColor m_ForegroundColor;
    wxColor m_HighlightedColor;

private:

    void OnMouseEnter(wxMouseEvent& event);
    void OnMouseLeave(wxMouseEvent& event);
    wxDECLARE_EVENT_TABLE();
};

