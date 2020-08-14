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
    
    CreateAssemblerProperties();
    CreateCompilerProperties();
    CreateLinkerProperties();

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

void FileSettings::GetAssemblerEnvironmentSettings(wxEnvVariableHashMap &env)
{
    std::unordered_map<wxString, wxAny> settings;

    GetSettings(FileSettings::EProperty::Assembler_Environment, settings);

    for (const auto& e : settings)
    {
        env[e.first] = e.second.As<wxString>();
    }
}

void FileSettings::GetLinkerEnvironmentSettings(wxEnvVariableHashMap& env)
{
    std::unordered_map<wxString, wxAny> settings;

    GetSettings(FileSettings::EProperty::Linker_Environment, settings);

    for (const auto& e : settings)
    {
        env[e.first] = e.second.As<wxString>();
    }
}


void FileSettings::CreateAssemblerProperties(void)
{
    std::unordered_map<wxString, wxAny> assemblerProperties;
    assemblerProperties["COMMAND"] = wxString("\"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.27.29110/bin/Hostx64/x64/ml64.exe\"");
    m_settings[Assembler] = assemblerProperties;
    m_settings[Assembler_General] = assemblerProperties;

    std::unordered_map<wxString, wxAny> assemblerEnvironment;
    wxEnvVariableHashMap envMap;
    wxGetEnvMap(&envMap);
    std::string includePath =  "C:\\Program Files(x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.27.29110\\ATLMFC\\include;C:\\Program Files(x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.27.29110\\include;C:\\Program Files(x86)\\Windows Kits\\10\\include\\10.0.18362.0\\ucrt;C:\\Program Files(x86)\\Windows Kits\\10\\include\\10.0.18362.0\\shared;C:\\Program Files(x86)\\Windows Kits\\10\\include\\10.0.18362.0\\um;C:\\Program Files(x86)\\Windows Kits\\10\\include\\10.0.18362.0\\winrt;C:\\Program Files(x86)\\Windows Kits\\10\\include\\10.0.18362.0\\cppwinrt";
    envMap["INCLUDE"] = includePath;

    std::string envPath = "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.27.29110\\bin\\HostX64\\x64;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\VC\\VCPackages;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\TestWindow;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\TeamFoundation\\Team Explorer;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\MSBuild\\Current\\bin\\Roslyn;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Team Tools\\Performance Tools\\x64;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Team Tools\\Performance Tools;C:\\Program Files (x86)\\Microsoft Visual Studio\\Shared\\Common\\VSPerfCollectionTools\\vs2019\\\\x64;C:\\Program Files (x86)\\Microsoft Visual Studio\\Shared\\Common\\VSPerfCollectionTools\\vs2019\\;C:\\Program Files (x86)\\Microsoft SDKs\\Windows\\v10.0A\\bin\\NETFX 4.8 Tools\\x64\\;C:\\Program Files (x86)\\HTML Help Workshop;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\FSharp\\;C:\\Program Files (x86)\\Windows Kits\\10\\bin\\10.0.18362.0\\x64;C:\\Program Files (x86)\\Windows Kits\\10\\bin\\x64;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\\\MSBuild\\Current\\Bin;C:\\Windows\\Microsoft.NET\\Framework64\\v4.0.30319;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\Tools\\;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\CMake\\CMake\\bin;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\CMake\\Ninja;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\VC\\Linux\\bin\\ConnectionManagerExe;";

    std::string tempPath{ envMap["Path"] };
    envMap["Path"] = envPath + tempPath;

    for (const auto& e : envMap)
    {
        assemblerEnvironment[e.first] = e.second;
    }

    m_settings[Assembler_Environment] = assemblerEnvironment;

    std::unordered_map<wxString, wxAny> assemblerOptions;
    assemblerOptions["OPTIONS"] = wxString("/nologo /Zi /W3");
    m_settings[Assembler_Options] = assemblerOptions;
}

void FileSettings::CreateCompilerProperties(void)
{
}

void FileSettings::CreateLinkerProperties(void)
{
    std::unordered_map<wxString, wxAny> assemblerProperties;
    assemblerProperties["COMMAND"] = wxString("\"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.27.29110/bin/Hostx64/x64/link.exe\"");
    std::unordered_map<wxString, wxAny> assemblerEnvironment;
    wxEnvVariableHashMap envMap;
    wxGetEnvMap(&envMap);
    wxString lib = wxString("C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.27.29110\\ATLMFC\\lib\\x64;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.27.29110\\lib\\x64;C:\\Program Files (x86)\\Windows Kits\\NETFXSDK\\4.8\\lib\\um\\x64;C:\\Program Files (x86)\\Windows Kits\\10\\lib\\10.0.18362.0\\ucrt\\x64;C:\\Program Files (x86)\\Windows Kits\\10\\lib\\10.0.18362.0\\um\\x64;");
    envMap["Lib"] = lib;

    wxString envPath = wxString("C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.27.29110\\bin\\HostX64\\x64;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\VC\\VCPackages;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\TestWindow;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\TeamFoundation\\Team Explorer;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\MSBuild\\Current\\bin\\Roslyn;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Team Tools\\Performance Tools\\x64;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Team Tools\\Performance Tools;C:\\Program Files (x86)\\Microsoft Visual Studio\\Shared\\Common\\VSPerfCollectionTools\\vs2019\\\\x64;C:\\Program Files (x86)\\Microsoft Visual Studio\\Shared\\Common\\VSPerfCollectionTools\\vs2019\\;C:\\Program Files (x86)\\Microsoft SDKs\\Windows\\v10.0A\\bin\\NETFX 4.8 Tools\\x64\\;C:\\Program Files (x86)\\HTML Help Workshop;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\FSharp\\;C:\\Program Files (x86)\\Windows Kits\\10\\bin\\10.0.18362.0\\x64;C:\\Program Files (x86)\\Windows Kits\\10\\bin\\x64;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\\\MSBuild\\Current\\Bin;C:\\Windows\\Microsoft.NET\\Framework64\\v4.0.30319;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\Tools\\;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\CMake\\CMake\\bin;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\CMake\\Ninja;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\VC\\Linux\\bin\\ConnectionManagerExe;");

    wxString tempPath{ envMap["Path"] };
    envMap["Path"] = envPath + tempPath;

    for (const auto& e : envMap)
    {
        assemblerEnvironment[e.first] = e.second;
    }
    m_settings[Linker] = assemblerProperties;
    m_settings[Linker_General] = assemblerProperties;
    m_settings[Linker_Environment] = assemblerEnvironment;

    std::unordered_map<wxString, wxAny> assemblerOptions;
    assemblerOptions["OPTIONS"] = wxString("/DEBUG /subsystem:console /entry:main /largeaddressaware:no /nologo ");
    m_settings[Linker_Options] = assemblerOptions;
}
