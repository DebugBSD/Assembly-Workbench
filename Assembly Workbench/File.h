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
#include <wx/filename.h>
#include <string>

enum class FileType
{
    FT_NONE = -1,
    FT_ASSEMBLER,
    FT_C,
    FT_CPP
};

class File
{
public:
    
	File(const wxString& file, class AssemblerBase*pAssemblerFile = nullptr, class LinkerBase *pLinkerFile = nullptr, class CompilerBase *pCompiler = nullptr, class FileSettings* pFileSettings = nullptr, class Project *pProject = nullptr);
    File(const wxString& fileName, const wxString&filePath, class AssemblerBase* pAssemblerFile = nullptr, class LinkerBase* pLinkerFile = nullptr, class CompilerBase* pCompiler = nullptr, class FileSettings* pFileSettings = nullptr, class Project* pProject = nullptr);
    ~File();

    void Clean();

    void Assemble(); // Just for assembly files.

	void Compile(); // For C/C++ files.

	void Link(); // For object files. (ASM objects and C/C++ objects)

    void SetFileName(const wxString file) { m_FileName = file; }
    const wxString& GetFileName() const { return m_FileName; }
    const wxString GetAbsoluteFileName() const { return m_FilePath + wxFileName::GetPathSeparator() + m_FileName; }

    void SetFile(const wxString file) { m_FilePath = file; }
    const wxString& GetFile() const { return m_FilePath; }
    void SetAssembler(class AssemblerBase* pAssembler) { m_pAssembler = pAssembler; }
    void SetLinker(class LinkerBase* pLinker) { m_pLinker = pLinker; }
    void SetCompiler(class CompilerBase* pCompiler) { m_pCompiler = pCompiler; }

    class FileSettings* GetFileSettings() { return m_pFileSettings; }

    class Project* GetProject() { return m_pProject; }
private:

    // Use wxFileName instead of wxString
    wxString m_FileName;
    wxString m_FilePath; // Absolute or relative path to file without name of file.

    FileType m_FileType;

    class FileSettings* m_pFileSettings;
    class AssemblerBase* m_pAssembler;
    class LinkerBase* m_pLinker;
    class CompilerBase* m_pCompiler;
    class Project* m_pProject;
};

