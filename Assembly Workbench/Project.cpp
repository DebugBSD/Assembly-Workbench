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

#include <fstream>
#include <sstream>
#include <wx/filename.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include "JsonHelper.h"

#include "MLINKER.h"
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
    if (!doc.Load(fileName.GetFullPath().ToStdString()))
        return -2;
    // Start processing the XML file.
    if (doc.GetRoot()->GetName() != "Project")
        return -3;

    m_ProjectFile = fileName;

    wxXmlNode* child = doc.GetRoot()->GetChildren();
    while (child)
    {
        if (child->GetName() == "Configuration")
        {
            wxXmlNode* configuration = child->GetChildren();
            m_pElements = new TFolder("Root",0, nullptr, nullptr);
            ProcessConfigurationRecursive(configuration, "", m_pElements);

            int stop = 1;
        }
        child = child->GetNext();
    }

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

    for (File* pFile: m_ProjectFiles)
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
    m_ProjectFiles.push_back(pFile);
}

void Project::Save()
{
    // Create a document and add the root node.
    wxXmlDocument xmlDoc;
    wxXmlNode* pProjectNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, "Project");
    pProjectNode->AddAttribute("Name", "TestFGNE");
    xmlDoc.SetRoot(pProjectNode);
    // Add some XML.
    wxXmlNode* pConfigurationNode = new wxXmlNode(pProjectNode, wxXML_ELEMENT_NODE, "Configuration");
    TFolder* pFolder = static_cast<TFolder*>(m_pElements);
    for (TElement* pElement : pFolder->m_Elements)
    {
        SaveConfigurationRecursive(pConfigurationNode, pElement);
    }

    // Write the output to a wxString.
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

void Project::ProcessConfigurationRecursive(wxXmlNode* pNode, const wxString& directoryName, TElement* pRootElement)
{
    while (pNode)
    {
        if (pNode->GetName() == "Directory")
        {
            wxXmlNode* configuration = pNode->GetChildren();
            wxString dir;
            pNode->GetAttribute("Name", &dir);
            TFolder* pFolder = new TFolder(dir,0, pRootElement->m_pRoot, pRootElement);
            TFolder* pCurrentFolder = static_cast<TFolder*>(pRootElement);
            pCurrentFolder->m_Elements.push_front(pFolder);
            ProcessConfigurationRecursive(configuration, directoryName + "\\" + dir, pFolder);

        }
        else if (pNode->GetName() == "File")
        {
            wxString fileName;
            wxString assemblerType;

            TFolder* pCurrentFolder = static_cast<TFolder*>(pRootElement);

            pNode->GetAttribute("Name", &fileName);
            pNode->GetAttribute("AssemblerType", &assemblerType);

            // Check that file exists!!
            File* pFile = new File(m_ProjectFile.GetPath() + wxFileName::GetPathSeparator() + directoryName + wxFileName::GetPathSeparator() + fileName, m_pMainFrame->GetAssembler(), m_pMainFrame->GetLinker(), m_pMainFrame->GetCompiler(), m_pMainFrame->GetFileSettings(), this);

            TFile* pTFile = new TFile(pFile, 0, pRootElement->m_pRoot, pRootElement);
            pCurrentFolder->m_Elements.push_front(pTFile);

            int stop = 1;
        }

        pNode = pNode->GetNext();
    }
}

void Project::SaveConfigurationRecursive(wxXmlNode* pNode, TElement* pRootElement)
{
    if (pRootElement->m_Type == 1) // TFolder
    {
        wxXmlNode* pDirectory = new wxXmlNode(pNode, wxXML_ELEMENT_NODE, "Directory");
        pDirectory->AddAttribute("Name", pRootElement->m_Name);
        TFolder* pFolder = static_cast<TFolder*>(pRootElement);
        for (TElement* pElement : pFolder->m_Elements)
        {
            SaveConfigurationRecursive(pDirectory, pElement);
        }
    }
    else if (pRootElement->m_Type == 0) // TFile
    {
        wxXmlNode* pFileNode = new wxXmlNode(pNode, wxXML_ELEMENT_NODE, "File");
        pFileNode->AddAttribute("AssemblerType", "MASM64");
        pFileNode->AddAttribute("Name", pRootElement->m_Name);
    }
}
