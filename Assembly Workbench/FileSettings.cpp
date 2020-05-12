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
#include "wx/wxprec.h"
#include "wx/imaglist.h"
#include "filesettingsdlg.h"
#include "FileSettings.h"

FileSettings::FileSettings()
{
    // We should read all of this information from Global Settings.
    wxEnvVariableHashMap envMap;
    wxGetEnvMap(&envMap);

    /*
        General,
        Advanced,
        Debugging,
        Directories,
        Environment,
        Compiler, 
        Compiler_General,
        Compiler_Environment,
        Compiler_Options,
        Assembler,
        Assembler_General,
        Assembler_Environment,
        Assembler_Options,
        Linker,
        Linker_General,
        Linker_Environment,
        Linker_Options,
        BuildOptions
     */
    std::unordered_map<wxString, wxAny> general;
    std::unordered_map<wxString, wxAny> environment;

    // Fill this property with the different types and values.
    for (const auto& e : envMap)
    {
        environment[e.first] = e.second;
    }

    wxArrayString directories;
    directories.Add("Uno");
    directories.Add("Dos");
    directories.Add("Tres");

    general["General"] = directories;
    m_settings[General] = general;
    m_settings[Advanced] = general;
    m_settings[Debugging] = general;
    
    m_settings[Directories] = general;
    m_settings[Environment] = environment;
    m_settings[Assembler] = general;
    m_settings[Assembler_General] = general;
    m_settings[Assembler_Environment] = environment;
    m_settings[Assembler_Options] = general;
    m_settings[Linker] = general;
    m_settings[Linker_General] = general;
    m_settings[Linker_Environment] = environment;
    m_settings[Linker_Options] = general;
    m_settings[BuildOptions] = general;

}

FileSettings::~FileSettings()
{
}

void FileSettings::AddSettings(const EProperty& id, const std::unordered_map<wxString, wxAny>& settings)
{
    // TODO: Handle error
    m_settings[id] = settings;
}

void FileSettings::GetSettings(const EProperty& id, std::unordered_map<wxString, wxAny>& settings)
{
    // TODO: Handle error
    settings = m_settings[id];
}
