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
#include "AssemblerBase.h"
#include "LinkerBase.h"
#include "CompilerBase.h"
#include "FileSettings.h"
#include "File.h"
#include "Project.h"

File::File(const wxFileName& file, AssemblerBase* pAssemblerFile, LinkerBase* pLinkerFile, CompilerBase* pCompiler, FileSettings* pFileSettings, Project* pProject):
    m_FileName{file},
    m_pAssembler{pAssemblerFile},
    m_pLinker{pLinkerFile},
    m_pCompiler{pCompiler},
    m_pProject{pProject},
    m_pFileSettings{pFileSettings}
{
    if (m_pProject) m_pProject->AddFile(this);

    wxString extension = m_FileName.GetExt();
    if (extension == "asm")
    {
        m_FileType = FileType::FT_ASSEMBLER_SOURCE;
    }
    else if (extension == "inc")
    {
        m_FileType = FileType::FT_ASSEMBLER_INCLUDE;
    }
    else if (extension == "cpp")
    {
        m_FileType = FileType::FT_CPP_SOURCE;
    }
    else if (extension == "h")
    {
        m_FileType = FileType::FT_CCPP_INCLUDE;
    }
    else if (extension == "c")
    {
        m_FileType = FileType::FT_C_SOURCE;
    } 
    else
    {
        m_FileType = FileType::FT_NONE;
    }
}


File::~File()
{
}

void File::Assemble()
{
    if (m_pAssembler && m_FileName.GetFullPath() != "" && IsSourceCode())
    {
        m_pAssembler->AssembleFile(GetAbsoluteFileName(), m_pFileSettings);
    }
}

void File::Compile()
{
    if (m_pCompiler && m_FileName.GetFullPath() != "" && IsSourceCode())
    {
        m_pCompiler->Compile(GetAbsoluteFileName());
    }
}

void File::Link()
{
    if (m_pLinker && m_FileName.GetFullPath() != "" && IsSourceCode())
    {
        m_pLinker->Link(GetAbsoluteFileName(), m_pFileSettings);
    }
}
