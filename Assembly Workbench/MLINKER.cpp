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
#include <filesystem>
#include <wx/filename.h>
#include "Main.h"
#include "FileSettings.h"
#include "MLINKER.h"

/* 
 * Microsoft Linkers requires some environment variables, which are optional but needed:
 * - LIB 
 * - PATH 
 * 
 * Command Line:
 * LINK [options] [files] [@command_files]
 *
 * Example:
 * link /DEBUG /subsystem:console /entry:main /largeaddressaware:no /nologo /out:main.exe basic.obj kernel32.lib user32.lib msvcrt.lib libucrt.lib
 */

MLINKER::MLINKER(MainFrame* pFrame):
    m_pFrame{ pFrame }
{
}

MLINKER::~MLINKER()
{
}

void MLINKER::Clean(const wxString& file, FileSettings* pFileSettings)
{
}

void MLINKER::Link(const wxString& file, FileSettings* pFileSettings)
{
    wxExecuteEnv environment;
    // Get the environment variables.
    pFileSettings->GetLinkerEnvironmentSettings(environment.env);

    std::unordered_map<wxString, wxAny> linkerCommand;
    std::unordered_map<wxString, wxAny> linkerOptions;
    pFileSettings->GetSettings(FileSettings::EProperty::Linker, linkerCommand);

    // Assemble file.
    // TODO: Move to settings.
    std::filesystem::path filepath{ file.ToStdString() };
    std::string fileInput{ filepath.filename().stem().string() + ".obj" };
    std::string fileOutput{ "/out:" + filepath.filename().stem().string() + ".exe" };

    environment.cwd = filepath.parent_path().string();

    // Options 
    pFileSettings->GetSettings(FileSettings::EProperty::Linker_Options, linkerOptions);

    // Dependencies
    // TODO: Move to settings.
    std::string dependencies{"kernel32.lib user32.lib msvcrt.lib libucrt.lib"};

    // See options of MASM
    std::string command{ linkerCommand["COMMAND"].As<wxString>() + " " + linkerOptions["OPTIONS"].As<wxString>() + " " + fileOutput + " " + fileInput + " " + dependencies};

    // execute the wxExecute.
    wxArrayString output;
    wxArrayString errors;
    long res = wxExecute(command, output, errors, wxEXEC_SYNC, &environment);

    m_pFrame->Log(&output);
    m_pFrame->Log(&errors);

    if (res == 0)
    {
        wxString output = "Ok building " + fileOutput;
        m_pFrame->Log(&output);
    }
}

void MLINKER::Link(const wxString& cwd, const wxArrayString& objList, FileSettings* pFileSettings, const wxString &outFile)
{
    wxExecuteEnv environment;
    // Get the environment variables.
    pFileSettings->GetLinkerEnvironmentSettings(environment.env);

    std::unordered_map<wxString, wxAny> linkerCommand;
    std::unordered_map<wxString, wxAny> linkerOptions;
    pFileSettings->GetSettings(FileSettings::EProperty::Linker, linkerCommand);

    environment.cwd = cwd;

    wxString objects;
    for (wxString f : objList)
    {
        objects += f + " ";
    }

    // Options 
    pFileSettings->GetSettings(FileSettings::EProperty::Linker_Options, linkerOptions);

    // Dependencies
    // TODO: Move to settings.
    std::string dependencies{ "kernel32.lib user32.lib msvcrt.lib libucrt.lib" };

    // See options of MASM
    std::string command{ linkerCommand["COMMAND"].As<wxString>() + " " + linkerOptions["OPTIONS"].As<wxString>() + " /out:\"" + outFile + "\" " + objects + dependencies };

    // execute the wxExecute.
    wxArrayString output;
    wxArrayString errors;
    long res = wxExecute(command, output, errors, wxEXEC_SYNC, &environment);

    m_pFrame->Log(&output);
    m_pFrame->Log(&errors);

    if (res == 0)
    {
        wxString output = "Ok building " + outFile;
        m_pFrame->Log(&output);
    }
}

void MLINKER::SetEnvVariables(wxEnvVariableHashMap& envMap)
{
    wxGetEnvMap(&envMap);
    std::string lib;

    // OK - lib =  "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.27.29110/ATLMFC/lib/x64;C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/lib/x64;C:/Program Files (x86)/Windows Kits/NETFXSDK/4.8/lib/um/x64;C:/Program Files (x86)/Windows Kits/10/lib/10.0.18362.0/ucrt/x64;C:/Program Files (x86)/Windows Kits/10/lib/10.0.18362.0/um/x64;";

    lib = "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.27.29110\\ATLMFC\\lib\\x64;C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.25.28610\\lib\\x64;C:\\Program Files (x86)\\Windows Kits\\NETFXSDK\\4.8\\lib\\um\\x64;C:\\Program Files (x86)\\Windows Kits\\10\\lib\\10.0.18362.0\\ucrt\\x64;C:\\Program Files (x86)\\Windows Kits\\10\\lib\\10.0.18362.0\\um\\x64;";

    
    /*lib =  "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.27.29110\\ATLMFC\\lib\\x64;\
            C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.27.29110\\lib\\x64;\
            C:\\Program Files (x86)\\Windows Kits\\NETFXSDK\\4.8\\lib\\um\\x64;\
            C:\\Program Files (x86)\\Windows Kits\\10\\lib\\10.0.18362.0\\ucrt\\x64;\
            C:\\Program Files (x86)\\Windows Kits\\10\\lib\\10.0.18362.0\\um\\x64;";*/
    


    envMap["Lib"] = lib;

    std::string envPath =  "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.27.29110\\bin\\HostX64\\x64;                         \
                            C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\VC\\VCPackages;                                            \
                            C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\TestWindow;                   \
                            C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\TeamFoundation\\Team Explorer;\
                            C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\MSBuild\\Current\\bin\\Roslyn;                                           \
                            C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Team Tools\\Performance Tools\\x64;                                      \
                            C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Team Tools\\Performance Tools;                                           \
                            C:\\Program Files (x86)\\Microsoft Visual Studio\\Shared\\Common\\VSPerfCollectionTools\\vs2019\\\\x64;                                     \
                            C:\\Program Files (x86)\\Microsoft Visual Studio\\Shared\\Common\\VSPerfCollectionTools\\vs2019\\;                                          \
                            C:\\Program Files (x86)\\Microsoft SDKs\\Windows\\v10.0A\\bin\\NETFX 4.8 Tools\\x64\\;                                                      \
                            C:\\Program Files (x86)\\HTML Help Workshop;                                                                                                \
                            C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\FSharp\\;                     \
                            C:\\Program Files (x86)\\Windows Kits\\10\\bin\\10.0.18362.0\\x64;                                                                          \
                            C:\\Program Files (x86)\\Windows Kits\\10\\bin\\x64;                                                                                        \
                            C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\\\MSBuild\\Current\\Bin;                                                 \
                            C:\\Windows\\Microsoft.NET\\Framework64\\v4.0.30319;                                                                                        \
                            C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\;                                                          \
                            C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\Tools\\;                                                        \
                            C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\CMake\\CMake\\bin;            \
                            C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\CMake\\Ninja;                 \
                            C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\IDE\\VC\\Linux\\bin\\ConnectionManagerExe;";

    std::string tempPath{envMap["Path"]};
    envMap["Path"] = envPath + tempPath;


}