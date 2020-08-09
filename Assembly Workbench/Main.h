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

// wxWidgets "Hello World" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/aui/framemanager.h>
#include <wx/aui/aui.h>
#include <wx/srchctrl.h>
#include <wx/wxprec.h>
#include <wx/utils.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>

#include "AppSettings.h"
#include "AWArtProvider.h"
//#include "WindowManager.h"
#include "CustomFrame.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <unordered_map>


class MyApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();
};

class MainFrame : public CustomFrame
{
public:
    MainFrame();
    ~MainFrame();

private:

    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnResize(wxSizeEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnOpenProject(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnNew(wxCommandEvent& event);
    void OnNewProject(wxCommandEvent& event);
    void OnNewFile(wxCommandEvent& event);
    void OnClose(wxCommandEvent& event);
    void OnExitProgram(wxCloseEvent& event);
    void OnCMDTool(wxCommandEvent& event);
    void OnEdit(wxCommandEvent& event);
    void OnModifySettings(wxCommandEvent &event);

    // Project Menu
    void OnProjectPreferences(wxCommandEvent& event);

    // Build Menu
    void OnBuildSolution(wxCommandEvent& event);
    void OnRebuildSolution(wxCommandEvent& event);
    void OnCleanSolution(wxCommandEvent& event);

    void OnLaunchDebugger(wxCommandEvent& event);



public:
#pragma region Public attributes

#pragma endregion

#pragma region Public Methods
    class AppSettings* GetAppSettings() { return m_pAppSettings; }
    void SetStatusBar(const wxString& text) { m_pStatusBar->SetStatusText(text); }
    void SetStatusBar(size_t totalChars = 0, size_t totalLines = 0, size_t currentColumn = 0, size_t currentLine = 0);
    class wxStatusBar* GetStatusBar() { return m_pStatusBar; }

    void Log(class wxArrayString *pArrayLog);
    void Log(class wxString* pError);

    // Access to Projects
    const std::vector<class Project*>& GetProjects() const { return m_Projects; }

    // Access to windows from WindowManager
    class wxWindow* GetWindow(const wxString& windowName) { /*if (m_pWindowManager) return m_pWindowManager->GetWindow(windowName); else*/ return nullptr; }

    class AssemblerBase* GetAssembler() { return m_pAssemblerBase; }
    class LinkerBase* GetLinker() { return m_pLinkerBase; }
    class CompilerBase* GetCompiler() { return m_pCompilerBase; }
    class FileSettings* GetFileSettings() { return m_pGlobalFileSettings; }

    // Access to the files of the projects or the editor.
    void RemoveFile(class File* pFile) { if (pFile) m_Files.erase(pFile); }
    void AddFile(class File* pFile, class CodeEditor* pCodeEditor) 
    { 
        if (pFile && pCodeEditor) 
            m_Files.insert({pFile,pCodeEditor}); 
    }
    class CodeEditor* GetCodeEditor(class File* pFile);

    // Get/Set project directory entries
    wxArrayString& GetProjectDirectoryEntries() { return m_ProjectDirectoryEntries; }
    void SetProjectDirectoryEntries(wxArrayString& projectDirectoryEntries) { m_ProjectDirectoryEntries = projectDirectoryEntries; }

    // Get files (those wich are not binded to a project)
    void GetFiles(std::vector<class CodeEditor*>& outOpenFiles);
#pragma endregion

private:
#pragma region Private attributes
    //class WindowManager* m_pWindowManager;
    class AppSettings* m_pAppSettings;
    class AWArtProvider* m_pArtProvider;
    long m_notebook_style;
    class AssemblerBase* m_pAssemblerBase;
    class LinkerBase* m_pLinkerBase;
    class CompilerBase* m_pCompilerBase;
    class FileSettings *m_pGlobalFileSettings;

    class wxStatusBar* m_pStatusBar;

    // Create a map with assemblers, compilers and linkers.
    // Then, set a default.
    // On a new file created, set the default based on the file type.

    // Map with File Editor so, we can know which file goes into a editor.
    std::unordered_map<class File*, class CodeEditor*> m_Files;
    std::vector<class Project*> m_Projects;

    wxArrayString m_ProjectDirectoryEntries;

    // From Caronte Editor...

    class AWButton* m_pFileBtn;
    class AWButton* m_pEditBtn;
    class AWButton* m_pCreateBtn;
    class AWButton* m_pToolsBtn;
    class AWButton* m_pWindowsBtn;
    class AWButton* m_pRenderingBtn;
    class AWButton* m_pHelpBtn;
    class AWButton* m_pMinimizeBtn;
    class AWButton* m_pMaximizeBtn;
    class AWButton* m_pCloseBtn;
    wxPanel* m_panel9;
    wxStaticText* m_staticText2;
    wxStaticText* m_staticTitle;
    wxAuiNotebook* m_auinotebook5;
    class CodeEditor* m_pSceneView;
    class EntitySystemView* m_pEntitySystemView;
    wxPanel* m_panel19;
    class AssetBrowserView* m_pAssetBrowserViewPanel;
    class ConsoleView* m_pConsoleView;
    wxPanel* m_panel22;
    class ComponentSystemView* m_pComponentSystemView;
    wxPanel* m_panel24;
    wxSearchCtrl* pSearchCtrl;


    class AWButton* m_pGoBackBtn;
    class AWButton* m_pGoForwardBtn;
    class AWButton* m_pNewBtn;
    class AWButton* m_pOpenBtn;
    class AWButton* m_pSaveBtn;
    class AWButton* m_pUndoBtn;
    class AWButton* m_pRedoBtn;
    class AWButton* m_pPlayBtn;
    class AWButton* m_pBuildBtn;
    class AWButton* m_pSettingsBtn;
    class AWButton* m_pCodeEditorBtn;

    // Popup menus
    class CEMenuFile* m_pMenuFile;
    class CEMenuEdit* m_pMenuEdit;

#pragma endregion

#pragma region Private Methods

    class wxSizer* InitFrameButtons();
    class wxSizer* InitToolbar();
    class wxSizer* InitViews();
    void CreateMenubar();
    void InitToolChain();
    void UnInitToolChain();
    class wxAuiToolBar * CreateMainToolBar();
#pragma endregion

    wxDECLARE_EVENT_TABLE();
};
enum
{
    ID_Hello = wxID_HIGHEST+1,
    ID_Size,
    ID_New_File,
    ID_New_Project,
    ID_Clone,
    ID_Open_Project,
    ID_Close_Project,
    ID_Save_Project,
    ID_Recent_Files,
    ID_Recent_Projects,
    ID_Search_Replace,
    ID_View_Files,
    ID_View_Functions,
    ID_View_Variables,
    ID_View_Opcodes,
    ID_View_LineNumber,
    ID_View_LongLine,
    ID_View_CaretLine,
    ID_Project_Assembler,
    ID_Project_Linker,
    ID_Project_Custom_Assembler,
    ID_Project_Custom_Linker,
    ID_Project_Preferences,
    ID_Build_Build_Solution,
    ID_Build_Rebuild_Solution,
    ID_Build_Clean_Solution,
    ID_Debug_LaunchWinDbg,
    ID_Tools_Command_Line,
    ID_Tools_Hex_Editor,
    ID_Tools_CVS,
    ID_Tool_Graph,

};