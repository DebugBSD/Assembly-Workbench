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
#include <wx/filename.h>

#include "MLINKER.h"
#include "File.h"
#include "Project.h"
#include "Main.h"

Project::Project(wxWindow* parent):
    m_pMainFrame{ static_cast<MainFrame*>(wxTheApp->GetTopWindow()) },
    m_pAssembler{nullptr},
    m_pCompiler{nullptr},
    m_pLinker{nullptr},
    m_IsModified{false}
{
}

Project::~Project()
{
}

int Project::Load(const wxFileName& fileName)
{
    wxXmlDocument doc;
    if (!doc.Load(fileName.GetFullPath()))
        return -1;

    // Start processing the XML file.
    if (doc.GetRoot()->GetName() != "Root")
        return -2;

    //wxString path, fName, extension;
    //wxFileName::SplitPath(fileName, &path, &fName, &extension);
    m_ProjectFile = fileName;
    //m_ProjectDirectory = path;

    wxXmlNode* child = doc.GetRoot()->GetChildren();
    while (child)
    {
        if (child->GetName() == "Settings")
        {
            // We parse the settings
            wxString content = child->GetNodeContent();

            // Process attributes of tag1.
            wxString type =
                child->GetAttribute("type", "Parent");

        }
        else if (child->GetName() == "Configuration")
        {
            // We parse the files
            wxString type =
                child->GetAttribute("type", "Windows");

            wxXmlNode* pFilesNode = child->GetChildren();
            while (pFilesNode)
            {
                if (pFilesNode->GetName() == "Name") {
                    wxString assembler = pFilesNode->GetAttribute("Assembler", "MASM64");
                    wxString fileType = pFilesNode->GetAttribute("File", "SRC");
                    wxString sourceFile;

                    wxXmlNode* pContent = pFilesNode->GetChildren();
                    while (pContent)
                    {
                        sourceFile = pContent->GetContent();
                        pContent = pContent->GetNext();
                    }

                    File* pFile = new File(m_ProjectFile.GetPath() + wxFileName::GetPathSeparator() + sourceFile, m_pMainFrame->GetAssembler(), m_pMainFrame->GetLinker(), m_pMainFrame->GetCompiler(), m_pMainFrame->GetFileSettings(), this);
                }
                pFilesNode = pFilesNode->GetNext();
            }

        }

        child = child->GetNext();
    }

    m_IsModified = false;

    return 0;
}

void Project::Close()
{
}

int Project::Create(const wxFileName& fileName)
{
    // Here we need to create the Project Settings too based on the configuration file.
    int retCode = 0; // 0 -> OK
    m_IsModified = true;
    m_ProjectFile = fileName;
    wxString path = m_ProjectFile.GetPath();
    if (wxMkdir(path))
    {
        wxMkdir(path + "/Config");
        wxMkdir(path + "/.cache");
        wxMkdir(path + "/Build");
        
        // We should create a basic file.
        Save();
        
    }
    else
    {

        retCode = 1;
    }
    return retCode;
}

bool Project::Build()
{
    wxArrayString objects;

    for (File* pFile:m_Files)
    {
        if (pFile->IsSourceCode())
        {
            wxString path, fileName, extension;
            wxString obj{ pFile->GetAbsoluteFileName() };
            wxFileName::SplitPath(obj, &path, &fileName, &extension);
            objects.Add("\"" + path + wxFileName::GetPathSeparator() + fileName + ".obj\"");
            pFile->Assemble();
            pFile->Compile();
        }
    }

    // TODO: Add support tyo multiple linkers.
    // Link time
    MLINKER* pLinker = static_cast<MLINKER*>(m_pMainFrame->GetLinker());
    pLinker->Link(m_ProjectFile.GetPath(), objects, m_pMainFrame->GetFileSettings(), m_ProjectFile .GetName()+".exe");

	return false;
}

void Project::Clean()
{
}

void Project::AddFile(File* pFile)
{
    m_IsModified = true;
    m_Files.push_back(pFile);
}

void Project::Save()
{
    // Create a document and add the root node.
    wxXmlDocument xmlDoc;

    wxXmlNode* root = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, "Root");
    xmlDoc.SetRoot(root);

    // Add Settings
    wxXmlNode* fileNode = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Settings");
    fileNode->AddAttribute("type", "Parent");

    // Add files to the same Configuration
    wxXmlNode* fileNodeConf = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Configuration");
    fileNodeConf->AddAttribute("type", "Windows");
    for (File* pFile : m_Files)
    {
        wxXmlNode* name = new wxXmlNode(fileNodeConf, wxXML_ELEMENT_NODE, "Name");
        name->AddAttribute("Assembler", "MASM64");
        name->AddAttribute("File", "SRC");
        wxString srcFile{ GetRelativePathToFile(pFile->GetAbsoluteFileName()) };
        name->AddChild(new wxXmlNode(wxXML_TEXT_NODE, "", srcFile));
    }
    
    xmlDoc.Save(m_ProjectFile.GetFullPath());
}

const wxString Project::GetRelativePathToFile(const wxString& absoultePathToFile)
{
    wxString pathToFile, fileName, extension, projectName;

    wxFileName::SplitPath(absoultePathToFile, &pathToFile, &fileName, &extension);
    int pos = pathToFile.Find(m_ProjectFile.GetPath());
    if (pos != wxNOT_FOUND)
    {
        pathToFile.erase(pos, m_ProjectFile.GetPath().Length());
    }

    wxString relPath;
    if (pathToFile.IsEmpty())
        relPath = fileName + '.' + extension;
    else
    {
        pathToFile.StartsWith(wxFileName::GetPathSeparator(), &pathToFile);
        relPath = pathToFile + wxFileName::GetPathSeparator() + fileName + '.' + extension;
    }
    return relPath;
}