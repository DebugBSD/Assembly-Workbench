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
#include <wx/wxprec.h>

#include "Languages.h"       
 //----------------------------------------------------------------------------
 //! language types
const CommonInfo g_CommonPrefs = {
    // editor functionality prefs
    true,  // syntaxEnable
    true,  // foldEnable
    true,  // indentEnable
    // display defaults prefs
    false, // overTypeInitial
    false, // readOnlyInitial
    false,  // wrapModeInitial
    false, // displayEOLEnable
    false, // IndentGuideEnable
    true,  // lineNumberEnable
    false, // longLineOnEnable
    false, // whiteSpaceEnable
};

//----------------------------------------------------------------------------
// keywordlists
// C++
const char* CppWordlist1 =
"asm auto bool break case catch char class const const_cast "
"continue default delete do double dynamic_cast else enum explicit "
"export extern false float for friend goto if inline int long "
"mutable namespace new operator private protected public register "
"reinterpret_cast return short signed sizeof static static_cast "
"struct switch template this throw true try typedef typeid "
"typename union unsigned using virtual void volatile wchar_t "
"while";
const char* CppWordlist2 =
"file";
const char* CppWordlist3 =
"a addindex addtogroup anchor arg attention author b brief bug c "
"class code date def defgroup deprecated dontinclude e em endcode "
"endhtmlonly endif endlatexonly endlink endverbatim enum example "
"exception f$ f[ f] file fn hideinitializer htmlinclude "
"htmlonly if image include ingroup internal invariant interface "
"latexonly li line link mainpage name namespace nosubgrouping note "
"overload p page par param post pre ref relates remarks return "
"retval sa section see showinitializer since skip skipline struct "
"subsection test throw todo typedef union until var verbatim "
"verbinclude version warning weakgroup $ @ \"\" & < > # { }";

// C++
const char* AsmWordlist1 =
"mov add mul sub inc dec shr shl rol ror lea call invoke ";
const char* AsmWordlist2 =
"al ah ax eax rax bl bh bx ebx rbx cl ch cx ecx rcx dl dh dx edx rdx "
"rdi edi di rsi esi si rbp ebp bp rsp esp sp rflags eflags ";
const char* AsmOperators =
"* + - . / [ ] MOD ! != ||  && < <= == > >= & CARRY? OVERFLOW? PARITY? SIGN? ZERO?"
"AND NOT OR SHL SHR XOR "
"EQ GE GT LE LT NE "
"OFFSET "
"LENGTHOF SIZEOF ";


// Python
const char* PythonWordlist1 =
"and assert break class continue def del elif else except exec "
"finally for from global if import in is lambda None not or pass "
"print raise return try while yield";
const char* PythonWordlist2 =
"ACCELERATORS ALT AUTO3STATE AUTOCHECKBOX AUTORADIOBUTTON BEGIN "
"BITMAP BLOCK BUTTON CAPTION CHARACTERISTICS CHECKBOX CLASS "
"COMBOBOX CONTROL CTEXT CURSOR DEFPUSHBUTTON DIALOG DIALOGEX "
"DISCARDABLE EDITTEXT END EXSTYLE FONT GROUPBOX ICON LANGUAGE "
"LISTBOX LTEXT MENU MENUEX MENUITEM MESSAGETABLE POPUP PUSHBUTTON "
"RADIOBUTTON RCDATA RTEXT SCROLLBAR SEPARATOR SHIFT STATE3 "
"STRINGTABLE STYLE TEXTINCLUDE VALUE VERSION VERSIONINFO VIRTKEY";


//----------------------------------------------------------------------------
//! languages
const LanguageInfo g_LanguagePrefs[] = {
    // Assembly
    {"ASM",
     "*.asm;*.inc",
     wxSTC_LEX_ASM,
     {{mySTC_TYPE_DEFAULT, NULL},
      {mySTC_TYPE_COMMENT, NULL},
      {mySTC_TYPE_COMMENT_LINE, NULL},
      {mySTC_TYPE_COMMENT_DOC, NULL},
      {mySTC_TYPE_NUMBER, NULL},
      {mySTC_TYPE_WORD1, AsmWordlist1}, // Instructions
      {mySTC_TYPE_STRING, NULL},
      {mySTC_TYPE_CHARACTER, NULL},
      {mySTC_TYPE_UUID, NULL},
      {mySTC_TYPE_PREPROCESSOR, NULL},
      {mySTC_TYPE_OPERATOR, AsmOperators},
      {mySTC_TYPE_IDENTIFIER, NULL},
      {mySTC_TYPE_STRING_EOL, NULL},
      {mySTC_TYPE_DEFAULT, NULL}, // VERBATIM
      {mySTC_TYPE_REGEX, NULL},
      {mySTC_TYPE_COMMENT_SPECIAL, NULL}, // DOXY
      {mySTC_TYPE_WORD2, AsmWordlist2}, // Registers
      {mySTC_TYPE_WORD3, NULL}, // DOXY KEYWORDS
      {mySTC_TYPE_ERROR, NULL}, // KEYWORDS ERROR
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL}},
     0}, //mySTC_FOLD_COMMENT | mySTC_FOLD_COMPACT | mySTC_FOLD_PREPROC},
     // C++
    {"C++",
     "*.c;*.cc;*.cpp;*.cxx;*.cs;*.h;*.hh;*.hpp;*.hxx;*.sma",
     wxSTC_LEX_CPP,
     {{mySTC_TYPE_DEFAULT, NULL},
      {mySTC_TYPE_COMMENT, NULL},
      {mySTC_TYPE_COMMENT_LINE, NULL},
      {mySTC_TYPE_COMMENT_DOC, NULL},
      {mySTC_TYPE_NUMBER, NULL},
      {mySTC_TYPE_WORD1, CppWordlist1}, // KEYWORDS
      {mySTC_TYPE_STRING, NULL},
      {mySTC_TYPE_CHARACTER, NULL},
      {mySTC_TYPE_UUID, NULL},
      {mySTC_TYPE_PREPROCESSOR, NULL},
      {mySTC_TYPE_OPERATOR, NULL},
      {mySTC_TYPE_IDENTIFIER, NULL},
      {mySTC_TYPE_STRING_EOL, NULL},
      {mySTC_TYPE_DEFAULT, NULL}, // VERBATIM
      {mySTC_TYPE_REGEX, NULL},
      {mySTC_TYPE_COMMENT_SPECIAL, NULL}, // DOXY
      {mySTC_TYPE_WORD2, CppWordlist2}, // EXTRA WORDS
      {mySTC_TYPE_WORD3, CppWordlist3}, // DOXY KEYWORDS
      {mySTC_TYPE_ERROR, NULL}, // KEYWORDS ERROR
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL}},
     mySTC_FOLD_COMMENT | mySTC_FOLD_COMPACT | mySTC_FOLD_PREPROC},
      // * (any)
      {wxTRANSLATE(DEFAULT_LANGUAGE),
       "*.*",
       wxSTC_LEX_PROPERTIES,
       {{mySTC_TYPE_DEFAULT, NULL},
        {mySTC_TYPE_DEFAULT, NULL},
        {mySTC_TYPE_DEFAULT, NULL},
        {mySTC_TYPE_DEFAULT, NULL},
        {mySTC_TYPE_DEFAULT, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL},
        {-1, NULL}},
       0},
};

const int g_LanguagePrefsSize = WXSIZEOF(g_LanguagePrefs);

//----------------------------------------------------------------------------
//! style types
const StyleInfo g_StylePrefs[] = {
    // mySTC_TYPE_DEFAULT
    {"Default",
    "BLACK", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_WORD1
    {"Keyword1",
    "BLUE", "WHITE",
    "", 10, mySTC_STYLE_BOLD, 0},

    // mySTC_TYPE_WORD2
    {"Keyword2",
    "MIDNIGHT BLUE", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_WORD3
    {"Keyword3",
    "CORNFLOWER BLUE", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_WORD4
    {"Keyword4",
    "CYAN", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_WORD5
    {"Keyword5",
    "DARK GREY", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_WORD6
    {"Keyword6",
    "GREY", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_COMMENT
    {"Comment",
    "FOREST GREEN", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_COMMENT_DOC
    {"Comment (Doc)",
    "FOREST GREEN", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_COMMENT_LINE
    {"Comment line",
    "FOREST GREEN", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_COMMENT_SPECIAL
    {"Special comment",
    "FOREST GREEN", "WHITE",
    "", 10, mySTC_STYLE_ITALIC, 0},

    // mySTC_TYPE_CHARACTER
    {"Character",
    "KHAKI", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_CHARACTER_EOL
    {"Character (EOL)",
    "KHAKI", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_STRING
    {"String",
    "BROWN", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_STRING_EOL
    {"String (EOL)",
    "BROWN", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_DELIMITER
    {"Delimiter",
    "ORANGE", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_PUNCTUATION
    {"Punctuation",
    "ORANGE", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_OPERATOR
    {"Operator",
    "BLACK", "WHITE",
    "", 10, mySTC_STYLE_BOLD, 0},

    // mySTC_TYPE_BRACE
    {"Label",
    "VIOLET", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_COMMAND
    {"Command",
    "BLUE", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_IDENTIFIER
    {"Identifier",
    "BLACK", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_LABEL
    {"Label",
    "VIOLET", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_NUMBER
    {"Number",
    "SIENNA", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_PARAMETER
    {"Parameter",
    "VIOLET", "WHITE",
    "", 10, mySTC_STYLE_ITALIC, 0},

    // mySTC_TYPE_REGEX
    {"Regular expression",
    "ORCHID", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_UUID
    {"UUID",
    "ORCHID", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_VALUE
    {"Value",
    "ORCHID", "WHITE",
    "", 10, mySTC_STYLE_ITALIC, 0},

    // mySTC_TYPE_PREPROCESSOR
    {"Preprocessor",
    "GREY", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_SCRIPT
    {"Script",
    "DARK GREY", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_ERROR
    {"Error",
    "RED", "WHITE",
    "", 10, 0, 0},

    // mySTC_TYPE_UNDEFINED
    {"Undefined",
    "ORANGE", "WHITE",
    "", 10, 0, 0}

};

const int g_StylePrefsSize = WXSIZEOF(g_StylePrefs);
