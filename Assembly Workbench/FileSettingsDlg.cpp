/*
 * BSD 3-Clause License
 * 
 * Copyright (c) 2020, DebugBSD
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "stdafx.h"
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/imaglist.h"

#include "filesettingsdlg.h"

/*
 * FileSettings event table definition
 */

BEGIN_EVENT_TABLE(FileSettingsDlg, wxDialog)
EVT_TREE_SEL_CHANGED(ID_TREECTRL, FileSettingsDlg::OnEndSelection)
EVT_TREE_SEL_CHANGING(ID_TREECTRL, FileSettingsDlg::OnBeginSelection)
END_EVENT_TABLE()


/*
 * FileSettings constructors
 */

FileSettingsDlg::FileSettingsDlg()
{
    Init();
}

FileSettingsDlg::FileSettingsDlg(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style, FileSettings* pFileSettings)
{
    Init();

    Create(parent, id, caption, pos, size, style, pFileSettings);
}


/*
 * FileSettings creator
 */

bool FileSettingsDlg::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style, FileSettings* pFileSettings)
{
    m_pFileSettings = pFileSettings;
    SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY | wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create(parent, id, caption, pos, size, style);

    CreateControls();
    SetMinSize(size);
    Centre();
    return true;
}


/*
 * FileSettings destructor
 */

FileSettingsDlg::~FileSettingsDlg()
{
}


/*
 * Member initialisation
 */

void FileSettingsDlg::Init()
{
    m_pProjectConfigCtrl = nullptr;
    m_pPlatformConfCtrl = nullptr;
    m_pSplitter = nullptr;
    m_pPropGridCtrl = nullptr;
}


/*
 * Control creation for FileSettings
 */

void FileSettingsDlg::CreateControls()
{

    FileSettingsDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer1, 1, wxGROW | wxALL, 5);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer1->Add(itemBoxSizer3, 0, wxGROW | wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText(itemDialog1, wxID_STATIC, _("Project Config"), wxDefaultPosition, wxDefaultSize, 0);
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxArrayString m_pProjectConfigCtrlStrings;
    m_pProjectConfigCtrlStrings.Add(_("Debug"));
    m_pProjectConfigCtrlStrings.Add(_("Release"));
    m_pProjectConfigCtrlStrings.Add(_("Add New"));
    m_pProjectConfigCtrl = new wxComboBox(itemDialog1, wxID_ANY, "Debug", wxDefaultPosition, wxDefaultSize, m_pProjectConfigCtrlStrings, wxALIGN_LEFT | wxCB_READONLY);
    m_pProjectConfigCtrl->SetStringSelection(_("Debug"));
    itemBoxSizer3->Add(m_pProjectConfigCtrl, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    itemBoxSizer3->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxStaticText* itemStaticText7 = new wxStaticText(itemDialog1, wxID_STATIC, _("Platform Config"), wxDefaultPosition, wxDefaultSize, 0);
    itemBoxSizer3->Add(itemStaticText7, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxArrayString m_pPlatformConfCtrlStrings;
    m_pPlatformConfCtrlStrings.Add(_("X64"));
    m_pPlatformConfCtrlStrings.Add(_("X86"));
    m_pPlatformConfCtrlStrings.Add(_("ARM"));
    m_pPlatformConfCtrlStrings.Add(_("ARM64"));
    m_pPlatformConfCtrlStrings.Add(_("Add New"));
    m_pPlatformConfCtrl = new wxComboBox(itemDialog1, wxID_ANY, "X64", wxDefaultPosition, wxDefaultSize, m_pPlatformConfCtrlStrings, wxALIGN_LEFT | wxCB_READONLY);
    m_pPlatformConfCtrl->SetStringSelection(_("X64"));
    itemBoxSizer3->Add(m_pPlatformConfCtrl, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    itemBoxSizer1->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

    m_pSplitter = new wxSplitterWindow(itemDialog1, ID_SPLITTERWINDOW, wxDefaultPosition, wxSize(100, 100), wxSP_3DBORDER | wxSP_3DSASH | wxNO_BORDER);
    m_pSplitter->SetMinimumPaneSize(0);

    // We set the images list
    /* TODO: Set images to the elementos of the tree.
    wxSize size = FromDIP(wxSize(16, 16));
    wxImageList* imglist = new wxImageList(size.x, size.y, true, 2);
    imglist->Add(wxArtProvider::GetBitmap(wxART_PLUS, wxART_OTHER, size));
    imglist->Add(wxArtProvider::GetBitmap(wxART_MINUS, wxART_OTHER, size));*/

    // We set the tree
    m_pTreeCtrl = SetTree();

    wxPanel* itemPanel6 = new wxPanel(m_pSplitter, ID_PANEL, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTAB_TRAVERSAL);
    itemPanel6->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxVERTICAL);
    itemPanel6->SetSizer(itemBoxSizer7);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer7->Add(itemBoxSizer8, 1, wxGROW | wxALL, 0);
    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer8->Add(itemBoxSizer4, 0, wxGROW | wxALL, 5);
    wxStaticText* itemStaticText5 = new wxStaticText(itemPanel6, wxID_STATIC, _("Property"), wxDefaultPosition, wxDefaultSize, 0);
    itemBoxSizer4->Add(itemStaticText5, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxTextCtrl* itemTextCtrl6 = new wxTextCtrl(itemPanel6, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    itemBoxSizer4->Add(itemTextCtrl6, 1, wxGROW | wxALL, 5);

    wxArrayString itemComboBox1Strings;
    itemComboBox1Strings.Add("String");
    itemComboBox1Strings.Add("Array of Strings");
    itemComboBox1Strings.Add("Integer");
    itemComboBox1Strings.Add("Boolean");
    wxComboBox* itemComboBox1 = new wxComboBox(itemPanel6, wxID_ANY, "String", wxDefaultPosition, wxDefaultSize, itemComboBox1Strings, wxCB_DROPDOWN | wxCB_READONLY);
    itemBoxSizer4->Add(itemComboBox1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);


    wxButton* itemButton7 = new wxButton(itemPanel6, wxID_ANY, _("Add"), wxDefaultPosition, wxDefaultSize, 0);
    itemBoxSizer4->Add(itemButton7, 0, wxGROW | wxALL, 5);

    itemBoxSizer8->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

    // Construct wxPropertyGrid control
    m_pPropGridCtrl = new wxPropertyGrid(
        itemPanel6, // parent
        wxID_ANY, // id
        wxDefaultPosition, // position
        wxDefaultSize, // size
        // Here are just some of the supported window styles
        wxPG_AUTO_SORT | // Automatic sorting after items added
        wxPG_SPLITTER_AUTO_CENTER | // Automatically center splitter until user manually adjusts it
        // Default style
        wxPG_DEFAULT_STYLE);
    itemBoxSizer8->Add(m_pPropGridCtrl, 1, wxGROW | wxALL, 0);

    // Window style flags are at premium, so some less often needed ones are
    // available as extra window styles (wxPG_EX_xxx) which must be set using
    // SetExtraStyle member function. wxPG_EX_HELP_AS_TOOLTIPS, for instance,
    // allows displaying help strings as tool tips.
    m_pPropGridCtrl->SetExtraStyle(wxPG_EX_HELP_AS_TOOLTIPS);

    // wxArrayStringProperty embeds a wxArrayString.
    wxArrayString arrayStr;
    arrayStr.Add("test1");
    arrayStr.Add("test2");
    arrayStr.Add("test3");
    m_pPropGridCtrl->Append(new wxArrayStringProperty("Label of ArrayStringProperty",
        "NameOfArrayStringProp", arrayStr));

    // A file selector property.
    m_pPropGridCtrl->Append(new wxFileProperty("FileProperty", wxPG_LABEL, wxEmptyString));

    // Extra: set wild card for file property (format same as in wxFileDialog).
    m_pPropGridCtrl->SetPropertyAttribute("FileProperty",
        wxPG_FILE_WILDCARD,
        "All files (*.*)|*.*");

    // Extra: We set the delimiter to the array of strings to ;
    m_pPropGridCtrl->SetPropertyAttribute("NameOfArrayStringProp",
        wxPG_ARRAY_DELIMITER,
        ";");

    m_pSplitter->SplitVertically(m_pTreeCtrl, itemPanel6, 150);
    itemBoxSizer1->Add(m_pSplitter, 1, wxGROW | wxALL, 5);

    wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer1->Add(itemBoxSizer14, 0, wxALIGN_RIGHT | wxALL, 5);

    itemBoxSizer14->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxButton* itemButton17 = new wxButton(itemDialog1, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0);
    itemBoxSizer14->Add(itemButton17, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    wxButton* itemButton18 = new wxButton(itemDialog1, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
    itemBoxSizer14->Add(itemButton18, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

}


/*
 * Should we show tooltips?
 */

bool FileSettingsDlg::ShowToolTips()
{
    return true;
}

wxTreeCtrl* FileSettingsDlg::SetTree()
{
    wxTreeItemId tmpId;
    // We set the tree
    m_pTreeCtrl = new wxTreeCtrl(m_pSplitter, ID_TREECTRL, wxDefaultPosition, wxSize(100, 100), wxTR_DEFAULT_STYLE);
    m_pTreeCtrl->AddRoot("Properties");
    wxTreeItemId root = m_pTreeCtrl->GetRootItem();
    tmpId = m_pTreeCtrl->AppendItem(root, "General");
    m_Settings[tmpId.GetID()] = FileSettings::EProperty::General;
    tmpId = m_pTreeCtrl->AppendItem(root, "Advanced");
    m_Settings[tmpId.GetID()] = FileSettings::EProperty::Advanced;
    tmpId = m_pTreeCtrl->AppendItem(root, "Debugging");
    m_Settings[tmpId.GetID()] = FileSettings::EProperty::Debugging;
    tmpId = m_pTreeCtrl->AppendItem(root, "Directories");
    m_Settings[tmpId.GetID()] = FileSettings::EProperty::Directories;
    tmpId = m_pTreeCtrl->AppendItem(root, "Environment");
    m_Settings[tmpId.GetID()] = FileSettings::EProperty::Environment;
    //m_pTreeCtrl->AppendItem(m_pTreeCtrl->GetRootItem(), "C/C++");

    wxTreeItemId assembler = m_pTreeCtrl->AppendItem(root, "Assembler");
    m_Settings[assembler.GetID()] = FileSettings::EProperty::Assembler;
    tmpId = m_pTreeCtrl->AppendItem(assembler, "General");
    m_Settings[tmpId.GetID()] = FileSettings::EProperty::Assembler_General;
    tmpId = m_pTreeCtrl->AppendItem(assembler, "Environment");
    m_Settings[tmpId.GetID()] = FileSettings::EProperty::Assembler_Environment;
    tmpId = m_pTreeCtrl->AppendItem(assembler, "Options");
    m_Settings[tmpId.GetID()] = FileSettings::EProperty::Assembler_Options;

    wxTreeItemId linker = m_pTreeCtrl->AppendItem(root, "Linker");
    m_Settings[linker.GetID()] = FileSettings::EProperty::Linker;
    tmpId = m_pTreeCtrl->AppendItem(linker, "General");
    m_Settings[tmpId.GetID()] = FileSettings::EProperty::Linker_General;
    tmpId = m_pTreeCtrl->AppendItem(linker, "Environment");
    m_Settings[tmpId.GetID()] = FileSettings::EProperty::Linker_Environment;
    tmpId = m_pTreeCtrl->AppendItem(linker, "Options");
    m_Settings[tmpId.GetID()] = FileSettings::EProperty::Linker_Options;

    tmpId = m_pTreeCtrl->AppendItem(root, "Build Steps");
    m_Settings[tmpId.GetID()] = FileSettings::EProperty::BuildOptions;
    m_pTreeCtrl->Expand(root);
    // m_pTreeCtrl->AssignImageList(imglist); // TODO: Set images to the properties of the tree.

    //m_pTreeCtrl->SetItemImage(root, 0);

    return m_pTreeCtrl;
}

void FileSettingsDlg::OnBeginSelection(wxTreeEvent& event)
{
    int stop = 0;
}

void FileSettingsDlg::OnEndSelection(wxTreeEvent& event)
{
    if (event.GetItem().IsOk() && m_pTreeCtrl && event.GetItem() != m_pTreeCtrl->GetRootItem())
    {
        wxString text = m_pTreeCtrl->GetItemText(event.GetItem());

        int stop = 1;
        std::unordered_map<wxString, wxAny> settings;
        void* pId = event.GetItem().GetID();
        m_pFileSettings->GetSettings(m_Settings[pId], settings);
        stop = 0;
        m_pPropGridCtrl->Clear();

        // Fill this property with the different types and values.
        for (const auto &e : settings)
        {
            wxString key = e.first;
            if (e.second.CheckType<wxArrayString>())
            {
                m_pPropGridCtrl->Append(new wxArrayStringProperty(e.first,e.first, e.second.As<wxArrayString>()));
            }
            else if (e.second.CheckType<wxString>())
            {
                m_pPropGridCtrl->Append(new wxStringProperty(e.first, e.first, e.second.As<wxString>()));
            }
            else
            {
                int stop = 1;
            }
        }
    }

}

/*
 * Get bitmap resources
 */

wxBitmap FileSettingsDlg::GetBitmapResource(const wxString& name)
{
    // Bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
}

/*
 * Get icon resources
 */

wxIcon FileSettingsDlg::GetIconResource(const wxString& name)
{
    // Icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
}
