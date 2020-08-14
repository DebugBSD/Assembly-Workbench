#pragma once
#include <wx\frame.h>
class CustomFrame :
    public wxFrame
{
public:
	CustomFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE, const wxString& name = wxFrameNameStr);
	~CustomFrame();

protected:

	enum class ClickedBar {
		TopBar = 0,
		BottomBar,
		LeftBar,
		RightBar,
		TopLeftBar,
		TopRightBar,
		BottomLeftBar,
		BottomRightBar
	};
	// Just for mouse
	bool m_IsResizing;
	wxRect m_WindowPosAndSize;
	ClickedBar m_ClickedBar;

	bool m_IsDragging;
	int m_X, m_Y;

private:


	// Mouse events
	void OnLeftButtonDown(wxMouseEvent& event);
	void OnLeftButtonUp(wxMouseEvent& event);
	void OnMove(wxMouseEvent& event);

	wxDECLARE_EVENT_TABLE();
};

