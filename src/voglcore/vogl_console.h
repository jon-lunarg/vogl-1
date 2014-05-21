/**************************************************************************
 *
 * Copyright 2013-2014 RAD Game Tools and Valve Software
 * Copyright 2010-2014 Rich Geldreich and Tenacious Software LLC
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 **************************************************************************/

// File: vogl_console.h
#pragma once

#include "vogl_core.h"
#include "vogl_dynamic_string.h"

//----------------------------------------------------------------------------------------------------------------------
// Console output
//----------------------------------------------------------------------------------------------------------------------
#define vogl_printf(...)  vogl::console::printf(VOGL_FUNCTION_INFO_CSTR, cMsgPrint, __VA_ARGS__)
#define vogl_progress_printf(...) vogl::console::printf(VOGL_FUNCTION_INFO_CSTR, cMsgProgress, __VA_ARGS__)
#define vogl_header_printf(...) vogl::console::printf(VOGL_FUNCTION_INFO_CSTR, cMsgHeader, __VA_ARGS__)

#define vogl_error_printf(...) vogl::console::printf(VOGL_FUNCTION_INFO_CSTR, cMsgError, __VA_ARGS__)
#define vogl_warning_printf(...) vogl::console::printf(VOGL_FUNCTION_INFO_CSTR, cMsgWarning, __VA_ARGS__)
#define vogl_message_printf(...) vogl::console::printf(VOGL_FUNCTION_INFO_CSTR, cMsgVerbose, __VA_ARGS__)
#define vogl_debug_printf(...) vogl::console::printf(VOGL_FUNCTION_INFO_CSTR, cMsgDebug, __VA_ARGS__)

#define vogl_warning_printf_once(...)         \
    {                                         \
        static bool __printed_msg##__LINE__;  \
        if (!__printed_msg##__LINE__)         \
        {                                     \
            __printed_msg##__LINE__ = true;   \
            vogl_warning_printf(__VA_ARGS__); \
        }                                     \
    }

namespace vogl
{

class data_stream;

enum eConsoleMessageType
{
    cMsgPrint,    // ordinary printf messages
    cMsgProgress, // progress messages (not logged).
    cMsgHeader,   // header message

    cMsgError,    // high priority errors messages
    cMsgWarning,  // warning messages
    cMsgVerbose,  // verbose messages
    cMsgDebug,    // debugging messages

    cMsgTotal
};

typedef bool (*console_output_func)(eConsoleMessageType type, const char *pMsg, void *pData);

class console
{
public:
    static void init();
    static void deinit();

    static const char *get_message_type_str(eConsoleMessageType type);
    static eConsoleMessageType get_message_type_from_str(const char *str);

    static void add_console_output_func(console_output_func pFunc, void *pData);
    static void remove_console_output_func(console_output_func pFunc);

    static void printf(const char *caller_info, eConsoleMessageType type, const char *p, ...) VOGL_ATTRIBUTE_PRINTF(3, 4);
    static void vprintf(const char *caller_info, eConsoleMessageType type, const char *p, va_list args);

    static void set_tool_prefix(const char *pPrefix)
    {
        strncpy(m_tool_prefix, pPrefix, VOGL_ARRAY_SIZE(m_tool_prefix));
        m_tool_prefix[VOGL_ARRAY_SIZE(m_tool_prefix) - 1] = 0;
    }
    static const char *get_tool_prefix()
    {
        return m_tool_prefix;
    }

    // Set max message level: cMsgError, cMsgWarning, CMsgVerbose, cMsgDebug. Normally:
    //   default: display <= cMsgWarning
    //   --quiet: Only display <= cMsgError
    //   --verbose: Only display <= cMsgVerbose
    //   --debug: Display all messages
    static void set_output_level(eConsoleMessageType level)
    {
        if (level < cMsgError)
            level = cMsgError;
        else if (level >= cMsgDebug)
            level = cMsgDebug;

        m_output_level = level;
    }
    static eConsoleMessageType get_output_level()
    {
        return m_output_level;
    }
    
    static void set_log_stream(data_stream *pStream)
    {
        m_pLog_stream = pStream;
    }
    static data_stream *get_log_stream()
    {
        return m_pLog_stream;
    }

    static uint32_t get_total_messages(eConsoleMessageType type)
    {
        return m_num_messages[type];
    }

private:
    static uint32_t m_num_output_funcs;

    static bool m_prefixes;
    static eConsoleMessageType m_output_level;
    static data_stream *m_pLog_stream;
    static uint32_t m_num_messages[cMsgTotal];
    static bool m_at_beginning_of_line;
    static char m_tool_prefix[256];
};

int vogl_getch();
int vogl_kbhit();

} // namespace vogl
