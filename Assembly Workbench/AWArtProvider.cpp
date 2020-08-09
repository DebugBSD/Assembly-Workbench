#include "stdafx.h"
#include <wx/menu.h>
#include <wx/dc.h>
#include <wx/renderer.h>
#include <wx/dcclient.h>
#include <wx/aui/auibook.h>
#include <wx/aui/framemanager.h>
#include <wx/aui/dockart.h>
#include <wx/msw/uxtheme.h>
#include <wx/msw/private.h>
#include "AWArtProvider.h"

inline float wxAuiGetSRGB(float r) {
    return r <= 0.03928 ? r / 12.92 : pow((r + 0.055) / 1.055, 2.4);
}

float wxAuiGetRelativeLuminance(const wxColour& c)
{
    // based on https://www.w3.org/TR/2008/REC-WCAG20-20081211/#relativeluminancedef
    return 0.2126 * wxAuiGetSRGB(c.Red() / 255.0)
        + 0.7152 * wxAuiGetSRGB(c.Green() / 255.0)
        + 0.0722 * wxAuiGetSRGB(c.Blue() / 255.0);
}

float wxAuiGetColourContrast(const wxColour& c1, const wxColour& c2)
{
    // based on https://www.w3.org/TR/UNDERSTANDING-WCAG20/visual-audio-contrast7.html
    float L1 = wxAuiGetRelativeLuminance(c1);
    float L2 = wxAuiGetRelativeLuminance(c2);
    return L1 > L2 ? (L1 + 0.05) / (L2 + 0.05) : (L2 + 0.05) / (L1 + 0.05);
}

wxString wxAuiChopText(wxDC& dc, const wxString& text, int max_size)
{
    wxCoord x, y;

    // first check if the text fits with no problems
    dc.GetTextExtent(text, &x, &y);
    if (x <= max_size)
        return text;

    size_t i, len = text.Length();
    size_t last_good_length = 0;
    for (i = 0; i < len; ++i)
    {
        wxString s = text.Left(i);
        s += wxT("...");

        dc.GetTextExtent(s, &x, &y);
        if (x > max_size)
            break;

        last_good_length = i;
    }

    wxString ret = text.Left(last_good_length);
    ret += wxT("...");
    return ret;
}

// Check if the color has sufficient contrast ratio (4.5 recommended)
// (based on https://www.w3.org/TR/UNDERSTANDING-WCAG20/visual-audio-contrast7.html)
static bool wxAuiHasSufficientContrast(const wxColour& c1, const wxColour& c2)
{
    return wxAuiGetColourContrast(c1, c2) >= 4.5;
}

// Pick a color that provides better contrast against the background
static wxColour wxAuiGetBetterContrastColour(const wxColour& back_color,
    const wxColour& c1, const wxColour& c2)
{
    return wxAuiGetColourContrast(back_color, c1)
          > wxAuiGetColourContrast(back_color, c2) ? c1 : c2;
}


static void DrawButtons(wxDC& dc,
    const wxSize& offset,
    const wxRect& _rect,
    const wxBitmap& bmp,
    const wxColour& bkcolour,
    int button_state)
{
    wxRect rect = _rect;

    if (button_state == wxAUI_BUTTON_STATE_PRESSED)
    {
        rect.x += offset.x;
        rect.y += offset.y;
    }

    if (button_state == wxAUI_BUTTON_STATE_HOVER ||
        button_state == wxAUI_BUTTON_STATE_PRESSED)
    {
        dc.SetBrush(wxBrush(bkcolour.ChangeLightness(120)));
        dc.SetPen(wxPen(bkcolour.ChangeLightness(75)));

        // draw the background behind the button
        dc.DrawRectangle(rect.x, rect.y, bmp.GetScaledWidth() - offset.x, bmp.GetScaledHeight() - offset.y);
    }

    // draw the button itself
    dc.DrawBitmap(bmp, rect.x, rect.y, true);
}

static void IndentPressedBitmap(const wxSize& offset, wxRect* rect, int button_state)
{
    if (button_state == wxAUI_BUTTON_STATE_PRESSED)
    {
        rect->x += offset.x;
        rect->y += offset.y;
    }
}


// -- bitmaps --
// TODO: Provide x1.5 and x2.0 versions or migrate to SVG.

#if defined( __WXMAC__ )
static const unsigned char close_bits[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xFE, 0x03, 0xF8, 0x01, 0xF0, 0x19, 0xF3,
    0xB8, 0xE3, 0xF0, 0xE1, 0xE0, 0xE0, 0xF0, 0xE1, 0xB8, 0xE3, 0x19, 0xF3,
    0x01, 0xF0, 0x03, 0xF8, 0x0F, 0xFE, 0xFF, 0xFF };
#elif defined( __WXGTK__)
static const unsigned char close_bits[] = {
    0xff, 0xff, 0xff, 0xff, 0x07, 0xf0, 0xfb, 0xef, 0xdb, 0xed, 0x8b, 0xe8,
    0x1b, 0xec, 0x3b, 0xee, 0x1b, 0xec, 0x8b, 0xe8, 0xdb, 0xed, 0xfb, 0xef,
    0x07, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
#else
static const unsigned char close_bits[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xf3, 0xcf, 0xf9,
    0x9f, 0xfc, 0x3f, 0xfe, 0x3f, 0xfe, 0x9f, 0xfc, 0xcf, 0xf9, 0xe7, 0xf3,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
#endif

static const unsigned char left_bits[] = {
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xfe, 0x3f, 0xfe,
   0x1f, 0xfe, 0x0f, 0xfe, 0x1f, 0xfe, 0x3f, 0xfe, 0x7f, 0xfe, 0xff, 0xfe,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

static const unsigned char right_bits[] = {
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0x9f, 0xff, 0x1f, 0xff,
   0x1f, 0xfe, 0x1f, 0xfc, 0x1f, 0xfe, 0x1f, 0xff, 0x9f, 0xff, 0xdf, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

static const unsigned char list_bits[] = {
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0x0f, 0xf8, 0xff, 0xff, 0x0f, 0xf8, 0x1f, 0xfc, 0x3f, 0xfe, 0x7f, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

// wxAuiBitmapFromBits() is a utility function that creates a
// masked bitmap from raw bits (XBM format)
wxBitmap wxAuiBitmapFromBits(const unsigned char bits[], int w, int h,
    const wxColour& color)
{
    wxImage img = wxBitmap((const char*)bits, w, h).ConvertToImage();
    img.Replace(0, 0, 0, 123, 123, 123);
    img.Replace(255, 255, 255, color.Red(), color.Green(), color.Blue());
    img.SetMaskColour(123, 123, 123);
    return wxBitmap(img);
}

// A utility function to scales a bitmap in place for use at the given scale
// factor.
void wxAuiScaleBitmap(wxBitmap& bmp, double scale)
{
    // scale to a close round number to improve quality
    scale = floor(scale + 0.25);
    if (scale > 1.0 && !(bmp.GetScaleFactor() > 1.0))
    {
        wxImage img = bmp.ConvertToImage();
        img.Rescale(bmp.GetWidth() * scale, bmp.GetHeight() * scale,
            wxIMAGE_QUALITY_BOX_AVERAGE);
        bmp = wxBitmap(img);
    }
}

AWArtProvider::AWArtProvider()
    : m_normalFont(*wxNORMAL_FONT)
    , m_selectedFont(m_normalFont)
    , m_activeCloseBmp(wxAuiBitmapFromBits(close_bits, 16, 16, *wxBLACK))
    , m_disabledCloseBmp(wxAuiBitmapFromBits(close_bits, 16, 16, wxColour(128, 128, 128)))
    , m_activeLeftBmp(wxAuiBitmapFromBits(left_bits, 16, 16, *wxBLACK))
    , m_disabledLeftBmp(wxAuiBitmapFromBits(left_bits, 16, 16, wxColour(128, 128, 128)))
    , m_activeRightBmp(wxAuiBitmapFromBits(right_bits, 16, 16, *wxBLACK))
    , m_disabledRightBmp(wxAuiBitmapFromBits(right_bits, 16, 16, wxColour(128, 128, 128)))
    , m_activeWindowListBmp(wxAuiBitmapFromBits(list_bits, 16, 16, *wxBLACK))
    , m_disabledWindowListBmp(wxAuiBitmapFromBits(list_bits, 16, 16, wxColour(128, 128, 128)))
{
    m_selectedFont.SetWeight(wxFONTWEIGHT_BOLD);
    m_measuringFont = m_selectedFont;

    m_flags = 0;
    m_fixedTabWidth = wxWindow::FromDIP(100, NULL);

    wxColour baseColour = wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE);

    wxColour backgroundColour = baseColour;
    wxColour normaltabColour = baseColour;
    wxColour selectedtabColour = *wxWHITE;

    m_bkBrush = wxBrush(backgroundColour);
    m_normalBkBrush = wxBrush(normaltabColour);
    m_normalBkPen = wxPen(normaltabColour);
    m_selectedBkBrush = wxBrush(selectedtabColour);
    m_selectedBkPen = wxPen(selectedtabColour);

}

AWArtProvider::~AWArtProvider()
{
}

wxAuiTabArt* AWArtProvider::Clone()
{
    return new AWArtProvider(*this);
}

void AWArtProvider::SetFlags(unsigned int flags)
{
    m_flags = flags;
}


void AWArtProvider::SetSizingInfo(const wxSize& tab_ctrl_size,
    size_t tab_count)
{
    m_fixedTabWidth = wxWindow::FromDIP(100, NULL);

    int tot_width = (int)tab_ctrl_size.x - GetIndentSize() - wxWindow::FromDIP(4, NULL);

    if (m_flags & wxAUI_NB_CLOSE_BUTTON)
        tot_width -= m_activeCloseBmp.GetScaledWidth();
    if (m_flags & wxAUI_NB_WINDOWLIST_BUTTON)
        tot_width -= m_activeWindowListBmp.GetScaledWidth();

    if (tab_count > 0)
    {
        m_fixedTabWidth = tot_width / (int)tab_count;
    }


    m_fixedTabWidth = wxMax(m_fixedTabWidth, wxWindow::FromDIP(100, NULL));

    if (m_fixedTabWidth > tot_width / 2)
        m_fixedTabWidth = tot_width / 2;

    m_fixedTabWidth = wxMin(m_fixedTabWidth, wxWindow::FromDIP(220, NULL));
}

void AWArtProvider::SetColour(const wxColour& colour)
{
    m_bkBrush = wxBrush(colour);
    m_normalBkBrush = wxBrush(colour);
    m_normalBkPen = wxPen(colour);
}

void AWArtProvider::SetActiveColour(const wxColour& colour)
{
    m_selectedBkBrush = wxBrush(colour);
    m_selectedBkPen = wxPen(colour);
}

void AWArtProvider::DrawBorder(wxDC& dc, wxWindow* wnd, const wxRect& rect)
{
    int i, border_width = GetBorderWidth(wnd);

    wxRect theRect(rect);
    for (i = 0; i < border_width; ++i)
    {
        dc.DrawRectangle(theRect.x, theRect.y, theRect.width, theRect.height);
        theRect.Deflate(1);
    }
}

void AWArtProvider::DrawBackground(wxDC& dc,
    wxWindow* WXUNUSED(wnd),
    const wxRect& rect)
{
    // draw background
    dc.SetBrush(m_bkBrush);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawRectangle(-1, -1, rect.GetWidth() + 2, rect.GetHeight() + 2);

    // draw base line
    dc.SetPen(*wxGREY_PEN);
    dc.DrawLine(0, rect.GetHeight() - 1, rect.GetWidth(), rect.GetHeight() - 1);
}


// DrawTab() draws an individual tab.
//
// dc       - output dc
// in_rect  - rectangle the tab should be confined to
// caption  - tab's caption
// active   - whether or not the tab is active
// out_rect - actual output rectangle
// x_extent - the advance x; where the next tab should start

void AWArtProvider::DrawTab(wxDC& dc,
    wxWindow* wnd,
    const wxAuiNotebookPage& page,
    const wxRect& in_rect,
    int close_button_state,
    wxRect* out_tab_rect,
    wxRect* out_button_rect,
    int* x_extent)
{
    wxCoord normal_textx, normal_texty;
    wxCoord selected_textx, selected_texty;
    wxCoord textx, texty;

    // if the caption is empty, measure some temporary text
    wxString caption = page.caption;
    if (caption.empty())
        caption = wxT("Xj");

    dc.SetFont(m_normalFont);
    dc.GetTextExtent(caption, &normal_textx, &normal_texty);

    dc.SetFont(m_selectedFont);
    dc.GetTextExtent(caption, &selected_textx, &selected_texty);

    if (!m_closeBtnSize.IsFullySpecified())
        InitSizes(wnd, dc);

    // figure out the size of the tab
    wxSize tabSize = GetTabSize(dc,
        wnd,
        page.caption,
        page.bitmap,
        page.active,
        close_button_state,
        x_extent);

    wxCoord tabHeight = tabSize.y;
    wxCoord tabWidth = tabSize.x;
    wxCoord tabX = in_rect.x;
    wxCoord tabY = 0;

    if (page.active)
    {
        dc.SetPen(m_selectedBkPen);
        dc.SetBrush(m_selectedBkBrush);
        dc.SetFont(m_selectedFont);
        textx = selected_textx;
        texty = selected_texty;
    }
    else
    {
        dc.SetPen(m_normalBkPen);
        dc.SetBrush(m_normalBkBrush);
        dc.SetFont(m_normalFont);
        textx = normal_textx;
        texty = normal_texty;
        tabHeight += 2;
    }

    int clipWidth = tabWidth;
    if (tabX + clipWidth > in_rect.x + in_rect.width)
        clipWidth = (in_rect.x + in_rect.width) - tabX;
    dc.SetClippingRegion(tabX - wnd->FromDIP(2), tabY, clipWidth + wnd->FromDIP(4), tabHeight);

    int text_offset;

    int close_button_width = 0;

    // draw tab
    wxRect tabRect(tabX, tabY, tabWidth, tabHeight);

    int tabState;
    if (page.active)
        tabState = TIS_SELECTED;
    else if (page.hover)
        tabState = TIS_HOT;
    else
        tabState = TIS_NORMAL;

    wxUxThemeHandle hTabTheme(wnd, L"Tab");
    RECT tabR;
    wxCopyRectToRECT(tabRect, tabR);
    /*::DrawThemeBackground(hTabTheme, GetHdcOf(dc.GetTempHDC()), TABP_TABITEM,
        tabState,
        &tabR, NULL);*/

        // Apparently, in at least some Windows 10 installations the call above
        // does not draw the left edge of the first tab and it needs to be drawn
        // separately, or it wouldn't be drawn at all.
        /*if (tabX == GetIndentSize())
        {
            ::DrawThemeBackground
            (
                hTabTheme,
                GetHdcOf(dc.GetTempHDC()),
                TABP_TABITEMLEFTEDGE,
                tabState,
                &tabR,
                NULL
            );
        }*/

    text_offset = tabX + (tabHeight / 2) + ((tabWidth - close_button_width) / 2) - (textx / 2);
    wxRect textRect = tabRect;
    if (!page.active)
        textRect.Offset(0, wnd->FromDIP(1));
    if (close_button_state != wxAUI_BUTTON_STATE_HIDDEN)
        textRect.width -= m_closeBtnSize.x + wnd->FromDIP(3);


    // draw tab text
    wxColor back_color = dc.GetBrush().GetColour();
    wxColor sys_color = wxSystemSettings::GetColour(
        page.active ? wxSYS_COLOUR_CAPTIONTEXT : wxSYS_COLOUR_INACTIVECAPTIONTEXT);
    wxColor font_color = wxAuiHasSufficientContrast(back_color, sys_color) ? sys_color
        : wxAuiGetBetterContrastColour(back_color, *wxWHITE, *wxBLACK);
    dc.SetTextForeground(font_color);
    dc.SetFont(wnd->GetFont());
    dc.DrawLabel(page.caption, page.bitmap, textRect, wxALIGN_CENTRE);

    // draw focus rectangle
    if (page.active && (wnd->FindFocus() == wnd))
    {
        wxRect focusRect(text_offset, ((tabY + tabHeight) / 2 - (texty / 2) + 1),
            selected_textx, selected_texty);

        focusRect.Inflate(2, 2);
        /*wxRect focusRect = tabRect;
        focusRect.Deflate(wnd->FromDIP(2));*/

        wxRendererNative::Get().DrawFocusRect(wnd, dc, focusRect, 0);
    }

    // draw close button if necessary
    if (close_button_state != wxAUI_BUTTON_STATE_HIDDEN)
    {
        wxBitmap bmp;
        if (page.active)
            bmp = m_activeCloseBmp;
        else
            bmp = m_disabledCloseBmp;

        wxAuiScaleBitmap(bmp, wnd->GetContentScaleFactor());

        wxRect rect(tabX + tabWidth - bmp.GetScaledWidth() - 1,
            tabY + (tabHeight / 2) - (bmp.GetScaledHeight() / 2) + 1,
            bmp.GetScaledWidth(),
            tabHeight - 1);
        DrawButtons(dc, wnd->FromDIP(wxSize(1, 1)), rect, bmp, *wxWHITE, close_button_state);

        *out_button_rect = rect;
        close_button_width = bmp.GetScaledWidth();
    }

    *out_tab_rect = wxRect(tabX, tabY, tabWidth, tabHeight);

    dc.DestroyClippingRegion();
}

int AWArtProvider::GetIndentSize()
{
    return 0;
}

int AWArtProvider::GetBorderWidth(wxWindow* wnd)
{
    wxAuiManager* mgr = wxAuiManager::GetManager(wnd);
    if (mgr)
    {
        wxAuiDockArt* art = mgr->GetArtProvider();
        if (art)
            return art->GetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE);
    }
    return 1;
}

int AWArtProvider::GetAdditionalBorderSpace(wxWindow* WXUNUSED(wnd))
{
    return 0;
}

wxSize AWArtProvider::GetTabSize(wxDC& dc,
    wxWindow* wnd,
    const wxString& caption,
    const wxBitmap& WXUNUSED(bitmap),
    bool WXUNUSED(active),
    int close_button_state,
    int* x_extent)
{
    wxCoord measured_textx, measured_texty;

    dc.SetFont(m_measuringFont);
    dc.GetTextExtent(caption, &measured_textx, &measured_texty);

    wxCoord tab_height = measured_texty + wnd->FromDIP(4);
    wxCoord tab_width = measured_textx + tab_height + wnd->FromDIP(5);

    if (close_button_state != wxAUI_BUTTON_STATE_HIDDEN)
    {
        // increase by button size plus the padding
        tab_width += m_activeCloseBmp.GetScaledWidth() + wnd->FromDIP(3);
    }

    if (m_flags & wxAUI_NB_TAB_FIXED_WIDTH)
    {
        tab_width = m_fixedTabWidth;
    }

    *x_extent = tab_width - (tab_height / 2) - 1;

    return wxSize(tab_width, tab_height);
}


void AWArtProvider::DrawButton(wxDC& dc,
    wxWindow* wnd,
    const wxRect& in_rect,
    int bitmap_id,
    int button_state,
    int orientation,
    wxRect* out_rect)
{
    wxBitmap bmp;
    wxRect rect;

    switch (bitmap_id)
    {
    case wxAUI_BUTTON_CLOSE:
        if (button_state & wxAUI_BUTTON_STATE_DISABLED)
            bmp = m_disabledCloseBmp;
        else
            bmp = m_activeCloseBmp;
        break;
    case wxAUI_BUTTON_LEFT:
        if (button_state & wxAUI_BUTTON_STATE_DISABLED)
            bmp = m_disabledLeftBmp;
        else
            bmp = m_activeLeftBmp;
        break;
    case wxAUI_BUTTON_RIGHT:
        if (button_state & wxAUI_BUTTON_STATE_DISABLED)
            bmp = m_disabledRightBmp;
        else
            bmp = m_activeRightBmp;
        break;
    case wxAUI_BUTTON_WINDOWLIST:
        if (button_state & wxAUI_BUTTON_STATE_DISABLED)
            bmp = m_disabledWindowListBmp;
        else
            bmp = m_activeWindowListBmp;
        break;
    }

    if (!bmp.IsOk())
        return;

    wxAuiScaleBitmap(bmp, wnd->GetContentScaleFactor());

    rect = in_rect;

    if (orientation == wxLEFT)
    {
        rect.SetX(in_rect.x);
        rect.SetY(((in_rect.y + in_rect.height) / 2) - (bmp.GetScaledHeight() / 2));
        rect.SetWidth(bmp.GetScaledWidth());
        rect.SetHeight(bmp.GetScaledHeight());
    }
    else
    {
        rect = wxRect(in_rect.x + in_rect.width - bmp.GetScaledWidth(),
            ((in_rect.y + in_rect.height) / 2) - (bmp.GetScaledHeight() / 2),
            bmp.GetScaledWidth(), bmp.GetScaledHeight());
    }


    DrawButtons(dc, wnd->FromDIP(wxSize(1, 1)), rect, bmp, *wxWHITE, button_state);

    *out_rect = rect;
}

int AWArtProvider::ShowDropDown(wxWindow* wnd,
    const wxAuiNotebookPageArray& pages,
    int active_idx)
{
    /*
    wxMenu menuPopup;

    size_t i, count = pages.GetCount();
    for (i = 0; i < count; ++i)
    {
        const wxAuiNotebookPage& page = pages.Item(i);
        menuPopup.AppendCheckItem(1000 + i, page.caption);
    }

    if (active_idx != -1)
    {
        menuPopup.Check(1000 + active_idx, true);
    }

    // find out where to put the popup menu of window
    // items.  Subtract 100 for now to center the menu
    // a bit, until a better mechanism can be implemented
    int offset = wnd->FromDIP(100);
    wxPoint pt = ::wxGetMousePosition();
    pt = wnd->ScreenToClient(pt);
    if (pt.x < offset)
        pt.x = 0;
    else
        pt.x -= offset;

    // find out the screen coordinate at the bottom of the tab ctrl
    wxRect cli_rect = wnd->GetClientRect();
    pt.y = cli_rect.y + cli_rect.height;

    wxAuiCommandCapture* cc = new wxAuiCommandCapture;
    wnd->PushEventHandler(cc);
    wnd->PopupMenu(&menuPopup, pt);
    int command = cc->GetCommandId();
    wnd->PopEventHandler(true);

    if (command >= 1000)
        return command - 1000;
    */
    return -1;
}

int AWArtProvider::GetBestTabCtrlSize(wxWindow* wnd,
    const wxAuiNotebookPageArray& WXUNUSED(pages),
    const wxSize& WXUNUSED(requiredBmp_size))
{
    wxClientDC dc(wnd);
    dc.SetFont(m_measuringFont);
    int x_ext = 0;
    wxSize s = GetTabSize(dc,
        wnd,
        wxT("ABCDEFGHIj"),
        wxNullBitmap,
        true,
        wxAUI_BUTTON_STATE_HIDDEN,
        &x_ext);
    return s.y + 3;
}

void AWArtProvider::SetNormalFont(const wxFont& font)
{
    m_normalFont = font;
}

void AWArtProvider::SetSelectedFont(const wxFont& font)
{
    m_selectedFont = font;
}

void AWArtProvider::SetMeasuringFont(const wxFont& font)
{
    m_measuringFont = font;
}

void AWArtProvider::InitSizes(wxWindow* wnd, wxDC& dc)
{
    SIZE uxSize;

    // Borrow close button from tooltip (best fit on various backgrounds)
    wxUxThemeHandle hTooltipTheme(wnd, L"Tooltip");

    ::GetThemePartSize(hTooltipTheme, GetHdcOf(dc.GetTempHDC()),
        TTP_CLOSE, 0, NULL, TS_TRUE, &uxSize);
    m_closeBtnSize.Set(uxSize.cx, uxSize.cy);

    wxUxThemeHandle hTabTheme(wnd, L"Tab");
    ::GetThemePartSize(hTabTheme, GetHdcOf(dc.GetTempHDC()),
        TABP_TABITEM, 0, NULL, TS_TRUE, &uxSize);
    m_tabSize.Set(uxSize.cx, uxSize.cy);
}

bool AWArtProvider::IsThemed() const
{
    return
        m_themed &&
        !(m_flags & wxAUI_NB_BOTTOM); // Native theme does not support bottom tabs
}