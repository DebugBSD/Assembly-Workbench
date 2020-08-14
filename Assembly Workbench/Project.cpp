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
    std::ifstream file(fileName.GetFullPath().ToStdString());
    if (!file.is_open())
    {
        // Handle error
        return -1;
    }

    std::stringstream fileStream;
    fileStream << file.rdbuf();
    std::string contents = fileStream.str();
    rapidjson::StringStream jsonStr(contents.c_str());
    rapidjson::Document doc;
    doc.ParseStream(jsonStr);

    if (!doc.IsObject())
    {
        // Handle error
        return -2;
    }

    m_ProjectFile = fileName;
    const rapidjson::Value& fileObjects = doc["Configuration"];
    if (!fileObjects.IsArray() || fileObjects.Size() < 1)
    {
        // Handle error
        return -3;
    }

    for (rapidjson::SizeType i = 0; i < fileObjects.Size(); i++)
    {
        std::string fileName, type, assembler;
        JsonHelper::GetString(fileObjects[i], "File", fileName);
        JsonHelper::GetString(fileObjects[i], "Type", type);
        JsonHelper::GetString(fileObjects[i], "Type", assembler);
        File* pFile = new File(m_ProjectFile.GetPath() + wxFileName::GetPathSeparator() + fileName, m_pMainFrame->GetAssembler(), m_pMainFrame->GetLinker(), m_pMainFrame->GetCompiler(), m_pMainFrame->GetFileSettings(), this);
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
    rapidjson::Document doc;
    doc.SetObject();
    JsonHelper::AddString(doc.GetAllocator(), doc, "Root", m_ProjectFile.GetName().ToStdString());

    rapidjson::Value files(rapidjson::kArrayType);
    for (File* pFile : m_ProjectFiles)
    {
        rapidjson::Value fileObject(rapidjson::kObjectType);
        wxString relPath = pFile->GetFile().GetFullPath();
        wxString r = relPath.SubString(m_ProjectFile.GetPath().size() + 1, relPath.size());
        JsonHelper::AddString(doc.GetAllocator(), fileObject, "File", r.ToStdString());
        JsonHelper::AddString(doc.GetAllocator(), fileObject, "Assembler", "MASM64");
        JsonHelper::AddString(doc.GetAllocator(), fileObject, "Type", "SRC");
        files.PushBack(fileObject, doc.GetAllocator());
    }
    doc.AddMember("Configuration", files, doc.GetAllocator());
    
    // Save JSON to string buffer
    rapidjson::StringBuffer buffer;
    // Use PrettyWriter for pretty output (otherwise use Writer)
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    const char* output = buffer.GetString();

    // Write output to file
    std::string fileName = std::string(m_ProjectFile.GetFullPath());
    std::ofstream outFile(fileName);
    if (outFile.is_open())
    {
        outFile << output;
    }
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