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
#include <wx/dataview.h>
class SearchModel :
	public wxDataViewModel
{
public:
	SearchModel();
    ~SearchModel() { delete m_Root; }

    void Init();

    // override sorting to always sort branches ascendingly

    int Compare(const wxDataViewItem& item1, const wxDataViewItem& item2,
        unsigned int column, bool ascending) const wxOVERRIDE;

    // implementation of base class virtuals to define model

    virtual unsigned int GetColumnCount() const wxOVERRIDE
    {
        return 4;
    }

    virtual wxString GetColumnType(unsigned int col) const wxOVERRIDE
    {
        if (col == 2 || col == 3)
            return "long";

        return "string";
    }

    virtual void GetValue(wxVariant& variant,
        const wxDataViewItem& item, unsigned int col) const wxOVERRIDE;
    virtual bool SetValue(const wxVariant& variant,
        const wxDataViewItem& item, unsigned int col) wxOVERRIDE;

    virtual bool IsEnabled(const wxDataViewItem& item,
        unsigned int col) const wxOVERRIDE;

    virtual wxDataViewItem GetParent(const wxDataViewItem& item) const wxOVERRIDE;
    virtual bool IsContainer(const wxDataViewItem& item) const wxOVERRIDE;
    virtual unsigned int GetChildren(const wxDataViewItem& parent,
        wxDataViewItemArray& array) const wxOVERRIDE;
private:

    class SearchModelNode* m_Root;

};

