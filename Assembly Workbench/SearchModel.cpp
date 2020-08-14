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

#include <wx/log.h> 

#include "SearchModelNode.h"
#include "SearchModel.h"

SearchModel::SearchModel()
{
    m_Root = new SearchModelNode(NULL, "");
}

void SearchModel::Init()
{
    
    if(m_Root)delete m_Root;

    m_Root = new SearchModelNode(NULL, "Project Directoy");
    SearchModelNode* pFile1Node = new SearchModelNode(m_Root, "main.asm");
    pFile1Node->Append(
        new SearchModelNode(pFile1Node, "mov rax, 0", "main.asm", 0, 1));
    pFile1Node->Append(
        new SearchModelNode(pFile1Node, "mov rbx, 0", "Madonna", 10, 4));
    m_Root->Append(pFile1Node);
    Cleared();
}

int SearchModel::Compare(const wxDataViewItem& item1, const wxDataViewItem& item2, unsigned int column, bool ascending) const
{
    if (IsContainer(item1) && IsContainer(item2))
    {
        wxVariant value1, value2;
        GetValue(value1, item1, 0);
        GetValue(value2, item2, 0);

        wxString str1 = value1.GetString();
        wxString str2 = value2.GetString();
        int res = str1.Cmp(str2);
        if (res) return res;

        // items must be different
        wxUIntPtr litem1 = (wxUIntPtr)item1.GetID();
        wxUIntPtr litem2 = (wxUIntPtr)item2.GetID();

        return litem1 - litem2;
    }

    return wxDataViewModel::Compare(item1, item2, column, ascending);
}

void SearchModel::GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const
{
    wxASSERT(item.IsOk());

    SearchModelNode* node = (SearchModelNode*)item.GetID();
    switch (col)
    {
    case 0:
        variant = node->m_text;
        break;
    case 1:
        variant = node->m_fileName;
        break;
    case 2:
        variant = (long)node->m_Line;
        break;
    case 3:
        variant = (long)node->m_Column;
        break;
    default:
        wxLogError("SearchModelNode::GetValue: wrong column %d", col);
    }
}

bool SearchModel::SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned int col)
{
    wxASSERT(item.IsOk());

    SearchModelNode* node = (SearchModelNode*)item.GetID();
    switch (col)
    {
    case 0:
        node->m_text = variant.GetString();
        return true;
    case 1:
        node->m_fileName = variant.GetString();
        return true;
    case 2:
        node->m_Line = variant.GetLong();
        return true;
    case 3:
        node->m_Column = variant.GetLong();
        return true;

    default:
        wxLogError("SearchModelNode::SetValue: wrong column");
    }
    return false;
}

bool SearchModel::IsEnabled(const wxDataViewItem& item, unsigned int col) const 
{
    wxASSERT(item.IsOk());

    // SearchModelNode* node = (SearchModelNode*)item.GetID();

    return true;
}

wxDataViewItem SearchModel::GetParent(const wxDataViewItem& item) const 
{
    // the invisible root node has no parent
    if (!item.IsOk())
        return wxDataViewItem(0);

    SearchModelNode* node = (SearchModelNode*)item.GetID();

    // "MyMusic" also has no parent
    if (node == m_Root)
        return wxDataViewItem(0);

    return wxDataViewItem((void*)node->GetParent());
}

bool SearchModel::IsContainer(const wxDataViewItem& item) const 
{
    // the invisble root node can have children
    // (in our model always "MyMusic")
    if (!item.IsOk())
        return true;

    SearchModelNode* node = (SearchModelNode*)item.GetID();
    return node->IsContainer();
}

unsigned int SearchModel::GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const
{
    SearchModelNode* node = (SearchModelNode*)parent.GetID();
    if (!node)
    {
        array.Add(wxDataViewItem((void*)m_Root));
        return 1;
    }

    if (node->GetChildCount() == 0)
    {
        return 0;
    }

    unsigned int count = node->GetChildren().GetCount();
    for (unsigned int pos = 0; pos < count; pos++)
    {
        SearchModelNode* child = node->GetChildren().Item(pos);
        array.Add(wxDataViewItem((void*)child));
    }

    return count;
}
