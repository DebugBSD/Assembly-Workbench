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
#include <string>
#include "MASM.h"
#include <wx/arrstr.h>
#include <wx/utils.h> 

 /*
  * MASM Options
  * ------------
  *
  * - /c - Assemble without linking
  * - /nologo - Supress copyright message
  * - /Fo<file> - Name object file
  * - /G<c|d|z> - Use Pascal, C, or Stdcall.
  * 
  * Environment variables used by ml64.
  * -----------------------------------
  *
  * - Include - INCLUDE=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.25.28610\ATLMFC\include;
  *                     C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.25.28610\include;
  *                     C:\Program Files (x86)\Windows Kits\NETFXSDK\4.8\include\um;
  *                     C:\Program Files (x86)\Windows Kits\10\include\10.0.18362.0\ucrt;
  *                     C:\Program Files (x86)\Windows Kits\10\include\10.0.18362.0\shared;
  *                     C:\Program Files (x86)\Windows Kits\10\include\10.0.18362.0\um;
  *                     C:\Program Files (x86)\Windows Kits\10\include\10.0.18362.0\winrt;
  *                     C:\Program Files (x86)\Windows Kits\10\include\10.0.18362.0\cppwinrt
  * - Tmp -     TMP=C:\Users\debugg\AppData\Local\Temp
  */

MASM::MASM()
{
    // Set Default Options
}

MASM::~MASM()
{
}

void MASM::AssembleFile(const std::string& file)
{
    // Check if everything is okay.
    if (file != "")
    {
        wxExecuteEnv environment;
        // Get the environment variables.
        SetEnvVariables(environment.env);

        // Assemble file.
        std::filesystem::path filepath{ file };
        std::string fileInput{ "/c " + filepath.parent_path().string() + "/" + filepath.filename().string() };
        std::string fileOutput{ "/Fo " + filepath.parent_path().string() + "/" + filepath.filename().stem().string() + ".obj" };
        // Options 
        std::string options{ "/nologo " };

        // See options of MASM
        std::string command{ m_PathToAssembler + " " + options + " " + fileOutput + " " + fileInput };

        // execute the wxExecute.
        wxArrayString output;
        wxArrayString errors;
        long res = wxExecute(command, output, errors, wxEXEC_SYNC,&environment);
        
        int stop = 1;
        
    }

}

void MASM::SetEnvVariables(wxEnvVariableHashMap& envMap)
{
    wxGetEnvMap(&envMap);
    std::string includePath;

    includePath =  "C:\\Program Files(x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.25.28610\\ATLMFC\\include;\
                    C:\\Program Files(x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.25.28610\\include;        \
                    C:\\Program Files(x86)\\Windows Kits\\10\\include\\10.0.18362.0\\ucrt;                                          \
                    C:\\Program Files(x86)\\Windows Kits\\10\\include\\10.0.18362.0\\shared;                                        \
                    C:\\Program Files(x86)\\Windows Kits\\10\\include\\10.0.18362.0\\um;                                            \
                    C:\\Program Files(x86)\\Windows Kits\\10\\include\\10.0.18362.0\\winrt;                                         \
                    C:\\Program Files(x86)\\Windows Kits\\10\\include\\10.0.18362.0\\cppwinrt";

    envMap["INCLUDE"] = includePath;

}
