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
#include <filesystem>

#include <wx/filename.h>

#include "EditorsWindow.h"
#include "CodeEditor.h"
#include "File.h"

wxBEGIN_EVENT_TABLE(EditorsWindow, wxAuiNotebook)
    EVT_AUINOTEBOOK_PAGE_CLOSE(ID_Notebook, EditorsWindow::OnCloseTab)
    EVT_AUINOTEBOOK_PAGE_CLOSED(ID_Notebook, EditorsWindow::OnClosedTab)
    EVT_AUINOTEBOOK_PAGE_CHANGED(ID_Notebook, EditorsWindow::OnOpenTab)
wxEND_EVENT_TABLE()


void EditorsWindow::OnCloseTab(wxAuiNotebookEvent& event)
{
    int res = CloseFile();
    if (res == wxYES) // We save the file before closing it
    {
        event.Skip();
    }
    else if (res == wxNO) // We close without any saving.
    {
        event.Skip();
    }
    else // We return to the program.
    {
        event.Veto();
    }
}

EditorsWindow::EditorsWindow(wxWindow* pWindow) : wxAuiNotebook(pWindow, ID_Notebook),
    m_pMainFrame{ static_cast<MainFrame*>(wxTheApp->GetTopWindow()) }
{
}

int EditorsWindow::CloseFile()
{
    CodeEditor* pCodeEditor = static_cast<CodeEditor*>(GetCurrentPage());
    if (!pCodeEditor) return -1;
    // If File is Modified
    if (pCodeEditor->IsModified())
    {
        File* pFile = pCodeEditor->GetFile();
        if (pFile->GetFile() != "") // There is a file to save
        {
            int res = wxMessageBox(_("This file has been modifed. Do you want to save it before closing it?"), _("Please confirm"), wxYES_NO | wxCANCEL, this);
            if (res == wxYES) // We save the file before closing it
            {
                pCodeEditor->SaveFile(pFile->GetAbsoluteFileName());
                m_pMainFrame->RemoveFile(pFile);
            }
            else if (res == wxNO)
            {
                m_pMainFrame->RemoveFile(pFile);
            }
            return res;
        }
        else // There is no file to save to.
        {
            int res = wxMessageBox(_("This buffer has been modifed. All changes will be lost! Do you want to save it before saving it?"), _("Please confirm"), wxYES_NO | wxCANCEL, this);

            wxFileDialog
                saveFileDialog(this, _("Save Assembly file"), "", "",
                    "ASM files (*.asm)|*.asm", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

            if (res == wxYES)
            {
                if (saveFileDialog.ShowModal() == wxID_CANCEL)
                    return wxCANCEL;     // the user changed idea...

                std::filesystem::path tempFile{ static_cast<std::string>(saveFileDialog.GetPath()) };
                //pFile->SetFile(tempFile.parent_path().string());
                pFile->SetFileName(tempFile.filename().string());
                if (!pCodeEditor->SaveFile(static_cast<wxString>(tempFile)))
                {
                    wxLogError("Cannot save current contents in file '%s'.", saveFileDialog.GetPath());
                    return -1;
                }
            }
            else
            {
            }
            m_pMainFrame->RemoveFile(pFile);
            return res;
        }
    }
    else
    {
        m_pMainFrame->RemoveFile(pCodeEditor->GetFile());
        return wxYES;
    }
}

void EditorsWindow::OnOpenTab(wxAuiNotebookEvent& event)
{
    CodeEditor* pCodeEditor = static_cast<CodeEditor*>(GetCurrentPage());
    if (!pCodeEditor) return;

    pCodeEditor->UpdateStatusBar();
}

void EditorsWindow::OnClosedTab(wxAuiNotebookEvent& event)
{
}
