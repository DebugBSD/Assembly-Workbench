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

#include <wx/aui/framemanager.h>
#include <wx/wxprec.h>
#include <wx/utils.h>
#include <wx/panel.h>
#include <wx/stc/stc.h>
#include <wx/sizer.h>


enum
{
	ID_TextChanged = 1,
	ID_KeyEnter
};

enum class EventType
{
	EVENT_NONE = -1,
	EVENT_LEFT,
	EVENT_RIGHT,
	EVENT_UP,
	EVENT_DOWN,
	EVENT_LMOUSE
};

class CodeEditor :
	public wxTextCtrl
{
public:
	CodeEditor(wxWindow* parent, class File *pFile);
	~CodeEditor();

    const class File* GetFile() const { return m_pFile; }
    class File* GetFile() { return m_pFile; }
    void SetFile(class File* pFile) { m_pFile = pFile; }

private:
	class MainFrame* m_pMainFrame;

    class File* m_pFile;
private:

	void GetCursorPosition(size_t &lnPos, size_t &colPos);
	void SetCursorPosition(const EventType &evtType = EventType::EVENT_NONE);

	void TextChanged(wxCommandEvent& event);
	void OnKeyEnter(wxCommandEvent &event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnMouseDown(wxMouseEvent& event);
	void OnMouseUp(wxMouseEvent& event);

	wxDECLARE_EVENT_TABLE();
};
