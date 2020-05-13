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

#include <wx/hashmap.h>
#include <wx/treebase.h>
#include <wx/any.h>

#include <unordered_map>

class FileSettings
{
public:
    enum EProperty
    {
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
    };

    FileSettings();
    ~FileSettings();

    void AddSettings(const EProperty &id, const std::unordered_map<wxString, wxAny>&settings);
    void GetSettings(const EProperty &id, std::unordered_map<wxString, wxAny>& settings);
    void GetAssemblerEnvironmentSettings(wxEnvVariableHashMap& env);

    void GetLinkerEnvironmentSettings(wxEnvVariableHashMap& env);

private:
    void CreateAssemblerProperties(void);
    void CreateCompilerProperties(void);
    void CreateLinkerProperties(void);

private:
    std::unordered_map<EProperty, std::unordered_map<wxString, wxAny>> m_settings;

};