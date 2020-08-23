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
#pragma once
#include <wx/panel.h>
#include <wx/utils.h>
#include <wx/process.h>
#include <wx/txtstrm.h>
#include <wx/textctrl.h>
class DebuggerView :
    public wxPanel
{
public:
    /// Constructors
    DebuggerView();
    DebuggerView(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& caption = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL);

    bool Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& caption = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL);

    /// Destructor
    ~DebuggerView();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

    void Clear();

    void SetErrorStream(wxInputStream* errorStream) { m_pErrorStream = errorStream; }
    void SetInputStream(wxInputStream* inputStream) { m_pInputStream = inputStream; }
    void SetOututStream(wxOutputStream* outputStream) { m_pOutputStream = outputStream; }

    void InitDebugSession(const wxString &program);
    void ShutdownDebugSession();
private:

    wxTextCtrl* m_pOutTextCtrl;
    wxTextCtrl* m_pInTextCtrl;
    wxInputStream* m_pErrorStream;
    wxInputStream* m_pInputStream;
    wxOutputStream* m_pOutputStream;

    wxProcess* m_pProc;

private:

    void OnEnter(wxCommandEvent& event);
    void OnIdle(wxIdleEvent& evt);
    void OnTerminate(wxProcessEvent& event);

    wxDECLARE_EVENT_TABLE();

};

enum
{
    ID_InputTextControl,
    ID_OutputTextControl,
    ID_DebuggerProcess,
    ID_CDBProcess
};