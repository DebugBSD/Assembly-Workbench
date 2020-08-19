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

#include <wx/string.h>
#include <wx/filename.h>
#include <wx/imaglist.h>
#include <wx/frame.h>
#include <wx/combobox.h>

#include "Main.h"
#include "Project.h"
#include "File.h"
#include "CodeEditor.h"
#include "SearchModelNode.h"
#include "SearchModel.h"
#include "FindAndReplaceWindow.h"


/*
 * ProjectWindow event table definition
 */

wxBEGIN_EVENT_TABLE(FindAndReplaceWindow, wxPanel)
EVT_BUTTON(ID_FIND_ALL, FindAndReplaceWindow::OnFindAllBtnClicked)
wxEND_EVENT_TABLE()


    /*
     * FindAndReplaceWindow constructors
     */

FindAndReplaceWindow::FindAndReplaceWindow()
{
    Init();
}

FindAndReplaceWindow::FindAndReplaceWindow(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style):
    m_pMainFrame{ static_cast<MainFrame*>(wxTheApp->GetTopWindow()) }
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * FindAndReplaceWindow creator
 */

bool FindAndReplaceWindow::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
    wxPanel::Create(parent, id, pos, size, style);

    CreateControls();
    Centre();
    return true;
}


/*
 * FindAndReplaceWindow destructor
 */

FindAndReplaceWindow::~FindAndReplaceWindow()
{
}


/*
 * Member initialisation
 */

void FindAndReplaceWindow::Init()
{
    m_FindComboBox = NULL;
    m_ReplaceComboBox = NULL;
    m_FindInDirectory = NULL;
    m_pSearchResultTreeCtrl = NULL;
}


/*
 * Control creation for FindAndReplaceWindow
 */

void FindAndReplaceWindow::CreateControls()
{

    MainFrame* pMainFrame{ static_cast<MainFrame*>(wxTheApp->GetTopWindow()) };
    FindAndReplaceWindow* itemFrame1 = this;

    wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer1);

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer1->Add(itemBoxSizer2, 1, wxGROW | wxALL, 5);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW | wxALL, 2);

    wxArrayString m_FindComboBoxStrings;
    m_FindComboBox = new wxComboBox(itemFrame1, ID_FIND_COMBOBOX, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_FindComboBoxStrings, wxCB_DROPDOWN);
    m_FindComboBox->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
    m_FindComboBox->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
    m_FindComboBox->SetHelpText(_("Find"));
    if (FindAndReplaceWindow::ShowToolTips())
        m_FindComboBox->SetToolTip(_("Find"));
    itemBoxSizer3->Add(m_FindComboBox, 0, wxGROW | wxALL, 0);

    wxArrayString m_ReplaceComboBoxStrings;
    m_ReplaceComboBox = new wxComboBox(itemFrame1, ID_REPLACE_COMBOBOX, wxEmptyString, wxDefaultPosition, wxDefaultSize, m_ReplaceComboBoxStrings, wxCB_DROPDOWN);
    m_ReplaceComboBox->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
    m_ReplaceComboBox->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
    m_ReplaceComboBox->SetHelpText(_("Replace"));
    if (FindAndReplaceWindow::ShowToolTips())
        m_ReplaceComboBox->SetToolTip(_("Replace"));
    itemBoxSizer3->Add(m_ReplaceComboBox, 0, wxGROW | wxALL, 0);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer6, 0, wxGROW | wxALL, 3);

    wxCheckBox* itemCheckBox7 = new wxCheckBox(itemFrame1, ID_CASE_SENSITIVE_CBOX, _("Case Sensitive"), wxDefaultPosition, wxDefaultSize, 0);
    itemCheckBox7->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
    itemCheckBox7->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
    itemCheckBox7->SetValue(false);
    itemBoxSizer6->Add(itemCheckBox7, 0, wxGROW | wxALL, 3);

    wxCheckBox* itemCheckBox8 = new wxCheckBox(itemFrame1, ID_COMPLETE_WORDS_CBOX, _("Complete Words"), wxDefaultPosition, wxDefaultSize, 0);
    itemCheckBox8->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
    itemCheckBox8->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
    itemCheckBox8->SetValue(false);
    itemBoxSizer6->Add(itemCheckBox8, 0, wxGROW | wxALL, 3);

    wxCheckBox* itemCheckBox9 = new wxCheckBox(itemFrame1, ID_REG_EX_CBOX, _("Use regular expresions"), wxDefaultPosition, wxDefaultSize, 0);
    itemCheckBox9->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
    itemCheckBox9->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
    itemCheckBox9->SetValue(false);
    itemBoxSizer6->Add(itemCheckBox9, 0, wxGROW | wxALL, 3);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer4, 0, wxGROW | wxALL, 3);

    wxStaticText* itemStaticText5 = new wxStaticText(itemFrame1, wxID_STATIC, _("Find In"), wxDefaultPosition, wxDefaultSize, 0);
    itemStaticText5->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
    itemStaticText5->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
    itemBoxSizer4->Add(itemStaticText5, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    wxArrayString itemComboBox6Strings;
    itemComboBox6Strings.Add(_("All Projects"));
    itemComboBox6Strings.Add(_("All Projects and Files"));
    itemComboBox6Strings.Add(_("All Files"));
    itemComboBox6Strings.Add(_("Current Project"));
    itemComboBox6Strings.Add(_("Current File"));
    itemComboBox6Strings.Add(_("Current Project Directory"));
    itemComboBox6Strings.Add(_("Current File Directory"));
    m_WhereToFind = new wxComboBox(itemFrame1, ID_COMBOBOX2, _("All Projects"), wxDefaultPosition, wxDefaultSize, itemComboBox6Strings, wxCB_DROPDOWN);
    m_WhereToFind->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
    m_WhereToFind->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
    m_WhereToFind->SetStringSelection(_("All Projects"));
    itemBoxSizer4->Add(m_WhereToFind, 1, wxGROW | wxALL, 3);

    wxCheckBox* itemCheckBox1 = new wxCheckBox(itemFrame1, ID_CHECKBOX, _("Include subfolders"), wxDefaultPosition, wxDefaultSize, 0);
    itemCheckBox1->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
    itemCheckBox1->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
    itemCheckBox1->SetValue(false);
    itemBoxSizer4->Add(itemCheckBox1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    m_FindInDirectory = new wxButton(itemFrame1, ID_FIND_IN_DIRECTORY_BTN, _("..."), wxDefaultPosition, wxSize(30, -1), 0);
    m_FindInDirectory->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
    m_FindInDirectory->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
    itemBoxSizer4->Add(m_FindInDirectory, 0, wxGROW | wxALL, 3);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer8, 0, wxGROW | wxALL, 5);

    wxStaticText* itemStaticText9 = new wxStaticText(itemFrame1, wxID_STATIC, _("File Types"), wxDefaultPosition, wxDefaultSize, 0);
    itemStaticText9->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
    itemStaticText9->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
    itemBoxSizer8->Add(itemStaticText9, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    wxArrayString itemComboBox10Strings;
    wxComboBox* itemComboBox10 = new wxComboBox(itemFrame1, ID_COMBOBOX3, wxEmptyString, wxDefaultPosition, wxDefaultSize, itemComboBox10Strings, wxCB_DROPDOWN);
    itemComboBox10->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
    itemComboBox10->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
    itemBoxSizer8->Add(itemComboBox10, 1, wxGROW | wxALL, 3);

    wxGridSizer* itemGridSizer11 = new wxGridSizer(2, 3, 0, 0);
    itemBoxSizer2->Add(itemGridSizer11, 0, wxALIGN_RIGHT | wxALL, 2);

    wxButton* itemButton12 = new wxButton(itemFrame1, ID_FIND_BEFORE_BTN, _("Find Before"), wxDefaultPosition, wxDefaultSize, 0);
    itemButton12->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
    itemButton12->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
    itemGridSizer11->Add(itemButton12, 0, wxGROW | wxALL, 2);

    wxButton* itemButton13 = new wxButton(itemFrame1, ID_FIND_NEXT_BTN, _("Find Next"), wxDefaultPosition, wxDefaultSize, 0);
    itemButton13->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
    itemButton13->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
    itemGridSizer11->Add(itemButton13, 0, wxGROW | wxALL, 2);

    wxButton* itemButton14 = new wxButton(itemFrame1, ID_OMIT_FILE_BTN, _("Omit File"), wxDefaultPosition, wxDefaultSize, 0);
    itemButton14->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
    itemButton14->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
    itemGridSizer11->Add(itemButton14, 0, wxGROW | wxALL, 2);

    wxButton* itemButton15 = new wxButton(itemFrame1, ID_REPLACE_NEXT_BTN, _("Replace Next"), wxDefaultPosition, wxDefaultSize, 0);
    itemButton15->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
    itemButton15->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
    itemGridSizer11->Add(itemButton15, 0, wxGROW | wxALL, 2);

    wxButton* itemButton16 = new wxButton(itemFrame1, ID_REPLACE_ALL, _("Replace All"), wxDefaultPosition, wxDefaultSize, 0);
    itemButton16->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
    itemButton16->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
    itemGridSizer11->Add(itemButton16, 0, wxGROW | wxALL, 2);

    wxButton* itemButton17 = new wxButton(itemFrame1, ID_FIND_ALL, _("Find All"), wxDefaultPosition, wxDefaultSize, 0);
    itemButton17->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
    itemButton17->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);
    itemGridSizer11->Add(itemButton17, 0, wxGROW | wxALL, 2);

    m_pSearchResultTreeCtrl = new wxDataViewCtrl(itemFrame1, ID_FIND_AND_REPLACE_RESULTS_TREECTRL, wxDefaultPosition, wxSize(100, 100), wxDV_SINGLE);
    itemBoxSizer2->Add(m_pSearchResultTreeCtrl, 1, wxGROW | wxALL, 5);
    m_searchModel = new SearchModel();
    m_pSearchResultTreeCtrl->AssociateModel(m_searchModel.get());
    m_pSearchResultTreeCtrl->SetBackgroundColour(pMainFrame->GetAppSettings()->m_backgroundColor);
    m_pSearchResultTreeCtrl->SetForegroundColour(pMainFrame->GetAppSettings()->m_foregroundColor);

    // Append columns
    wxDataViewTextRenderer* tr0 =
        new wxDataViewTextRenderer("string", wxDATAVIEW_CELL_INERT);
    wxDataViewColumn* column0 =
        new wxDataViewColumn("Code", tr0, 0, FromDIP(200), wxALIGN_LEFT,
            wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE);
    m_pSearchResultTreeCtrl->AppendColumn(column0);

    wxDataViewTextRenderer* tr1 =
        new wxDataViewTextRenderer("string", wxDATAVIEW_CELL_INERT);
    wxDataViewColumn* column1 =
        new wxDataViewColumn("Filename", tr1, 1, FromDIP(80), wxALIGN_LEFT,
            wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE);
    m_pSearchResultTreeCtrl->AppendColumn(column1);

    wxDataViewTextRenderer* tr2 =
        new wxDataViewTextRenderer("long", wxDATAVIEW_CELL_INERT);
    wxDataViewColumn* column2 =
        new wxDataViewColumn("Line", tr2, 2, FromDIP(25), wxALIGN_LEFT,
            wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE);
    m_pSearchResultTreeCtrl->AppendColumn(column2);


    wxDataViewTextRenderer* tr3 =
        new wxDataViewTextRenderer("long", wxDATAVIEW_CELL_INERT);
    wxDataViewColumn* column3 =
        new wxDataViewColumn("Column", tr3, 2, FromDIP(25), wxALIGN_LEFT,
            wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE);
    m_pSearchResultTreeCtrl->AppendColumn(column3);
}


/*
 * Should we show tooltips?
 */

bool FindAndReplaceWindow::ShowToolTips()
{
    return true;
}

/* 
 * Funciones a usar para la busqueda de strings: LineFromPosition, GetColumn, FindText  
 */
void FindAndReplaceWindow::Find(const wxString& pattern, std::vector<wxStyledTextCtrl*>& inputBuffers, int flags, std::unordered_map<std::pair<int, int>, wxString, pair_hash>& elementsFound)
{
    for (wxStyledTextCtrl *iStream : inputBuffers)
    {

    }
}

void FindAndReplaceWindow::OnFindAllBtnClicked(wxCommandEvent& event)
{
    wxArrayString itemComboBox6Strings;
    itemComboBox6Strings.Add(_("All Projects"));
    itemComboBox6Strings.Add(_("All Projects and Files"));
    itemComboBox6Strings.Add(_("All Files"));
    itemComboBox6Strings.Add(_("Current Project"));
    itemComboBox6Strings.Add(_("Current File"));
    itemComboBox6Strings.Add(_("Current Project Directory"));
    itemComboBox6Strings.Add(_("Current File Directory"));
    // 1º Miramos donde hay que buscar consultando el combobox de buscar donde (m_WhereToFind)
    std::vector<wxStyledTextCtrl*> bufferedFiles;
    std::vector<CodeEditor*> openFiles;
    std::unordered_map < std::pair<int, int>, wxString, pair_hash> elementsFound;
#if 0
    switch (itemComboBox6Strings.Index(m_WhereToFind->GetStringSelection()))
    {
    case 0: // All Projects
        // Get all open files
        m_pMainFrame->GetFiles(openFiles);

        for (Project* pProject : m_pMainFrame->GetProjects())
        {
            for (File* pFile : pProject->GetFiles())
            {
                bool fileFound = false;
                for (CodeEditor* pOpenFile : openFiles)
                {
                    if (pOpenFile->GetFile() == pFile)
                    {
                        fileFound = true;
                        bufferedFiles.push_back(static_cast<wxStyledTextCtrl*>(pOpenFile));
                    }
                }
                if (!fileFound)
                {
                    wxStyledTextCtrl* pBufferFile = new wxStyledTextCtrl();
                    if (pBufferFile->LoadFile(pFile->GetAbsoluteFileName()))
                    {
                        bufferedFiles.push_back(pBufferFile);
                    }
                }
            }
        }

        Find("pattern", bufferedFiles, 0, elementsFound);
        break;
    case 1: // All Projects and Files

        // Get all open files
        m_pMainFrame->GetFiles(openFiles);

        // Get all files from all projects
        for (Project* pProject : m_pMainFrame->GetProjects())
        {
            for (File* pFile : pProject->GetFiles())
            {
                bool fileFound = false;
                for (CodeEditor* pOpenFile : openFiles)
                {
                    if (pOpenFile->GetFile() == pFile)
                    {
                        fileFound = true;
                    }
                }
                /*if(!fileFound)
                    arrayOfFiles.Add(pFile->GetAbsoluteFileName());*/
            }
        }
        Find("pattern", bufferedFiles, 0, elementsFound);
        break;
    case 2: // All Files
        break;
    case 3: // Current Project
        break;
    case 4: // Current File
        break;
    case 5: // Current Project Directory
        break;
    case 6: // Current File Directory
        break;
    default:

        break;
    }
#endif
    // 2º Open and search the pattern into the files or buffers (CodeEDitor*)
    m_searchModel->Init();

    // 3º search for a pattern

}

/*
 * Get bitmap resources
 */

wxBitmap FindAndReplaceWindow::GetBitmapResource(const wxString& name)
{
    wxUnusedVar(name);
    return wxNullBitmap;
}

/*
 * Get icon resources
 */

wxIcon FindAndReplaceWindow::GetIconResource(const wxString& name)
{
    wxUnusedVar(name);
    return wxNullIcon;
}
