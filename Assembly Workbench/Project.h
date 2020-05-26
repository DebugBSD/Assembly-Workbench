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

#include <vector>
#include <wx/filename.h>

/*
 * - Directory structure
 * -----------------------
 *
 * ProjectName/
 *   |-> ProjectName.awp (Assembly Workbench Project)
 *   |-> .cache
 *   |-> Config/
 *   |    |-> [Different Project Configurations (like Assembler, Linker, Compiler, Debugger, Paths, and Environment variables)] 
 *   |    |-> [Example:] Windows.cfg - This file sets the Visual Studio environment (masm and linker with paths, includes, and everything)
 *   |    |-> [Example:] Layout.cfg - Layout of the interface. If there is not layout, we load a default one.
 *   |-> Build - Directory where resides all binaries built with the IDE.
 *   |-> [.git] - Git directory
 *   |-> [src/include/]file[s].asm/inc/c/cpp/h - Source code.
 *   |-> [.gitignore] - Git ignore file
 *
 * - NOTES
 * ---------
 *
 * Before loading a project, we need to do some checks before:
 * - Check that the file exists and if not exists, show a warning to the user
 * - Check that configuration files exists and ensure we can load and use them. If not, show a warning to the user
 * - Think more about it
 * After checking that everything is okay, we proceed to load the project and the configuration.
 * 
 */


class Project
{
public:
	Project(class wxWindow *parent);
    ~Project();

    int Load(const wxString& fileName);
    void Close();
    int Create(const wxString&projectDirectory, const wxString&fileName);

	bool Build();

	void Clean();

	void ReBuild() { Clean(); Build(); }

    void AddFile(class File* pFile);
    const std::vector<class File*>& GetFiles() const { return m_Files; }

    wxString GetName() const { return wxFileName(m_ProjectFile).GetName(); }
    wxString GetProjectDirectory() const {return m_ProjectDirectory; }
    void Save();

    const wxString GetRelativePathToFile(const wxString& absoultePathToFile);
    void SetModified(bool modified) { m_IsModified = modified; }
    bool IsModified() { return m_IsModified; }

private:
    class MainFrame* m_pMainFrame;

    wxString m_ProjectFile;
    wxString m_ProjectDirectory;

	std::vector<class File*> m_Files;

    class AssemblerBase* m_pAssembler;
    class LinkerBase* m_pLinker;
    class CompilerBase* m_pCompiler;

    bool m_IsModified;
};

