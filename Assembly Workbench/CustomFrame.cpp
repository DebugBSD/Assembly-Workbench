#include "stdafx.h"
#include "CustomFrame.h"

wxBEGIN_EVENT_TABLE(CustomFrame, wxFrame)
	// Mouse events
	EVT_LEFT_DOWN(CustomFrame::OnLeftButtonDown)
	EVT_LEFT_UP(CustomFrame::OnLeftButtonUp)
	EVT_MOTION(CustomFrame::OnMove)
wxEND_EVENT_TABLE()

CustomFrame::CustomFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name) :
	wxFrame(parent, id, title, pos, size, style, name),
	m_IsDragging{ false },
	m_IsResizing{ false }
{

	//MyFrame* pMainFrame{ static_cast<MyFrame*>(wxTheApp->GetTopWindow()) };
	//SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
	//SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);

}

CustomFrame::~CustomFrame()
{
}

void CustomFrame::OnLeftButtonDown(wxMouseEvent& event)
{
	// Move around the window
	if (event.GetY() >= 5 && event.GetY() < 10)
	{
		CaptureMouse();
		m_X = event.GetX();
		m_Y = event.GetY();
		m_IsDragging = true;
		SetTransparent(192);
	}

	// Resize the window if user clicked on the frame
	bool isClickedBar = false;

	if ((event.GetY() >= 0 && event.GetY() < 5) && (event.GetX() >= 0 && event.GetX() < 5))
	{
		isClickedBar = true;
		m_ClickedBar = ClickedBar::TopLeftBar;
	}
	else if ((event.GetY() >= 0 && event.GetY() < 5) && (event.GetX() >= GetScreenRect().width - 5 && event.GetX() < GetScreenRect().width))
	{
		isClickedBar = true;
		m_ClickedBar = ClickedBar::TopRightBar;
	}
	else if ((event.GetY() >= GetScreenRect().height - 5 && event.GetY() < GetScreenRect().height) && (event.GetX() >= 0 && event.GetX() < 5))
	{
		isClickedBar = true;
		m_ClickedBar = ClickedBar::BottomLeftBar;
	}
	else if ((event.GetY() >= GetScreenRect().height - 5 && event.GetY() < GetScreenRect().height) &&
		(event.GetX() >= GetScreenRect().width - 5 && event.GetX() < GetScreenRect().width))
	{
		isClickedBar = true;
		m_ClickedBar = ClickedBar::BottomRightBar;
	}
	else if ((event.GetY() >= 0 && event.GetY() < 5))
	{
		isClickedBar = true;
		m_ClickedBar = ClickedBar::TopBar;

		SetCursor(wxCURSOR_SIZENS);
	}
	else if (event.GetY() >= GetScreenRect().height - 5 && event.GetY() < GetScreenRect().height)
	{
		isClickedBar = true;
		m_ClickedBar = ClickedBar::BottomBar;
	}
	else if (event.GetX() >= 0 && event.GetX() < 5)
	{
		isClickedBar = true;
		m_ClickedBar = ClickedBar::LeftBar;
	}
	else if (event.GetX() >= GetScreenRect().width - 5 && event.GetX() < GetScreenRect().width)
	{
		isClickedBar = true;
		m_ClickedBar = ClickedBar::RightBar;
	}

	if (isClickedBar)
	{
		CaptureMouse();
		m_WindowPosAndSize = GetScreenRect();
		m_IsResizing = true;
		SetTransparent(192);
	}

	event.Skip();
}

void CustomFrame::OnLeftButtonUp(wxMouseEvent& event)
{
	if (m_IsDragging || m_IsResizing)
	{
		ReleaseMouse();
		m_IsDragging = false;
		m_IsResizing = false;
		SetTransparent(255);
	}
}

void CustomFrame::OnMove(wxMouseEvent& event)
{
	SetCursor(wxCURSOR_ARROW);
	if ((event.GetY() >= 0 && event.GetY() < 5) && (event.GetX() >= 0 && event.GetX() < 5))
	{
		SetCursor(wxCURSOR_SIZENWSE);
	}
	else if ((event.GetY() >= 0 && event.GetY() < 5) && (event.GetX() >= GetScreenRect().width - 5 && event.GetX() < GetScreenRect().width))
	{
		SetCursor(wxCURSOR_SIZENESW);
	}
	else if ((event.GetY() >= GetScreenRect().height - 5 && event.GetY() < GetScreenRect().height) && (event.GetX() >= 0 && event.GetX() < 5))
	{
		SetCursor(wxCURSOR_SIZENESW);
	}
	else if ((event.GetY() >= GetScreenRect().height - 5 && event.GetY() < GetScreenRect().height) &&
		(event.GetX() >= GetScreenRect().width - 5 && event.GetX() < GetScreenRect().width))
	{
		SetCursor(wxCURSOR_SIZENWSE);
	}
	else if ((event.GetY() >= 0 && event.GetY() < 5))
	{
		SetCursor(wxCURSOR_SIZENS);
	}
	else if (event.GetY() >= GetScreenRect().height - 5 && event.GetY() < GetScreenRect().height)
	{
		SetCursor(wxCURSOR_SIZENS);
	}
	else if (event.GetX() >= 0 && event.GetX() < 5)
	{
		SetCursor(wxCURSOR_SIZEWE);
	}
	else if (event.GetX() >= GetScreenRect().width - 5 && event.GetX() < GetScreenRect().width)
	{
		SetCursor(wxCURSOR_SIZEWE);
	}

	if (m_IsDragging)
	{
		wxPoint mouseOnScreen = wxGetMousePosition();
		int newx = mouseOnScreen.x - m_X;
		int newy = mouseOnScreen.y - m_Y;
		Move(wxPoint(newx, newy));
	}
	if (m_IsResizing)
	{

		wxPoint mouseOnScreen = wxGetMousePosition();
		wxRect newRect = m_WindowPosAndSize;
		int x = 0, y = 0;

		switch (m_ClickedBar)
		{
		case ClickedBar::TopBar:
			y = mouseOnScreen.y - m_WindowPosAndSize.y;
			newRect.y = mouseOnScreen.y;
			newRect.height = m_WindowPosAndSize.height - y;
			break;
		case ClickedBar::BottomBar:
			newRect.height = mouseOnScreen.y - m_WindowPosAndSize.y;
			break;
		case ClickedBar::LeftBar:
			x = mouseOnScreen.x - m_WindowPosAndSize.x;
			newRect.x = mouseOnScreen.x;
			newRect.width = m_WindowPosAndSize.width - x;
			break;
		case ClickedBar::RightBar:
			newRect.width = mouseOnScreen.x - m_WindowPosAndSize.x;
			break;
		case ClickedBar::TopLeftBar:
			y = mouseOnScreen.y - m_WindowPosAndSize.y;
			newRect.y = mouseOnScreen.y;
			newRect.height = m_WindowPosAndSize.height - y;
			x = mouseOnScreen.x - m_WindowPosAndSize.x;
			newRect.x = mouseOnScreen.x;
			newRect.width = m_WindowPosAndSize.width - x;
			break;
		case ClickedBar::TopRightBar:
			y = mouseOnScreen.y - m_WindowPosAndSize.y;
			newRect.y = mouseOnScreen.y;
			newRect.height = m_WindowPosAndSize.height - y;
			newRect.width = mouseOnScreen.x - m_WindowPosAndSize.x;
			break;
		case ClickedBar::BottomLeftBar:
			x = mouseOnScreen.x - m_WindowPosAndSize.x;
			newRect.x = mouseOnScreen.x;
			newRect.width = m_WindowPosAndSize.width - x;
			newRect.height = mouseOnScreen.y - m_WindowPosAndSize.y;
			break;
		case ClickedBar::BottomRightBar:
			newRect.height = mouseOnScreen.y - m_WindowPosAndSize.y;
			newRect.width = mouseOnScreen.x - m_WindowPosAndSize.x;
			break;
		}

		SetSize(newRect);
	}
}
