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

#include <wx/xml/xml.h>

#include "Project.h"

Project::Project():
    m_pAssembler{nullptr},
    m_pCompiler{nullptr},
    m_pLinker{nullptr}
{
}

Project::~Project()
{
}

int Project::Load(const wxString& fileName)
{
    return 0;
}

void Project::Close()
{
}

int Project::Create(const wxString& projectDirectory, const wxString& fileName)
{
    // Here we need to create the Project Settings too based on the configuration file.
    int retCode = 0; // 0 -> OK
    m_ProjectDirectory = projectDirectory;
    m_ProjectFile = fileName;
    wxString path = projectDirectory;
    if (wxMkdir(path))
    {
        wxMkdir(path + "/Config");
        wxMkdir(path + "/.cache");
        wxMkdir(path + "/Build");
        
        // We should create a basic file.
        wxFile projectFile;
        if (!projectFile.Create( m_ProjectDirectory + '/' + m_ProjectFile, true ))
        {
            // TODO: Handle error
            int stop = 0;
        }
        
    }
    else
    {

        retCode = 1;
    }
    return retCode;
}

bool Project::Build()
{
	return false;
}

void Project::Clean()
{
}

void Project::Save()
{
    // Create a document and add the root node.
    wxXmlDocument xmlDoc;

    wxXmlNode* root = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, "Root");
    xmlDoc.SetRoot(root);

    // Add some XML.
    wxXmlNode* library = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Configuration");
    library->AddAttribute("type", "Windows");

    wxXmlNode* name = new wxXmlNode(library, wxXML_ELEMENT_NODE, "Name");
    name->AddAttribute("Assembler","MASM64");
    name->AddAttribute("File", "SRC");
    name->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "", "Main.asm"));

    // Write the output to a wxString.
    xmlDoc.Save(m_ProjectDirectory+'/'+m_ProjectFile);
}

