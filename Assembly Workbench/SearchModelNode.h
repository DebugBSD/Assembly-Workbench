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

#include "wx/dataview.h"
#include "wx/hashmap.h"
#include "wx/vector.h"

class SearchModelNode;
WX_DEFINE_ARRAY_PTR(SearchModelNode*, SearchModelNodePtrArray);

class SearchModelNode
{
public:
	SearchModelNode(SearchModelNode *parent, const wxString &text, const wxString &fileName, int line, int column);

	SearchModelNode(SearchModelNode* parent, const wxString& text);

	~SearchModelNode();
    bool IsContainer() const { return m_Container; }
    SearchModelNode* GetParent() { return m_parent; }
    SearchModelNodePtrArray &GetChildren() { return m_children; }
    SearchModelNode* GetNthChild(unsigned int n) { return m_children.Item(n); }
    void Insert(SearchModelNode* child, unsigned int n) { m_children.Insert(child, n); }
    void Append(SearchModelNode* child) { m_children.Add(child); }
    unsigned int GetChildCount() const { return m_children.GetCount(); }

public:
#pragma region Public Members
	wxString m_text;
	wxString m_fileName;
	int m_Line;
	int m_Column;

	bool m_Container;
#pragma endregion 

private:
	SearchModelNode* m_parent;
	SearchModelNodePtrArray m_children;
};

