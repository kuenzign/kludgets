#include "kipc.h"
#include "config_win.h"

#include <QFile>
#include <QDir>
#include <QDesktopServices>

typedef struct _sendMsgParam
{
    int pid;
    long msg;
}
sendMsgParam;

BOOL CALLBACK EnumWindowProc_SendMessage(HWND hWnd, LPARAM lParam)
{
    sendMsgParam *p = (sendMsgParam*)lParam;
    DWORD pid = 0;
    GetWindowThreadProcessId(hWnd, &pid);

    if (p->pid == 0 || pid == p->pid)
    {
        if (GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_LAYERED)
        {
            SendMessage(hWnd, p->msg, 0, 0);
        }
    }
    return TRUE;
}

void KIPC::sendMessage(Message msg, int pId)
{
    sendMsgParam p;
    p.pid = pId;
    p.msg = msg;
    EnumWindows(EnumWindowProc_SendMessage, (LPARAM)&p);
}

void KIPC::sendAllMessage(Message msg)
{
    sendMsgParam p;
    p.pid = 0;
    p.msg = msg;
    EnumWindows(EnumWindowProc_SendMessage, (LPARAM)&p);
}

int KIPC::getProcessId(QString kludgetId)
{
    int pid = 0;
    QString pidfile = QDir(QDesktopServices::storageLocation(QDesktopServices::TempLocation)).absolutePath() + "/" + kludgetId + ".pid";
    if (QFile::exists(pidfile))
    {
        QFile file;
        file.setFileName(pidfile);
        if (file.open(QIODevice::ReadOnly))
        {
            QString text = file.readAll();
            file.close();
            pid = text.toInt();
        }
    }

    if (!checkProcess(pid))
    {
        return 0;
    }

    return pid;
}


void KIPC::closeProcess(int pid)
{
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
                                  PROCESS_ALL_ACCESS,
                                  FALSE, pid);
    if (hProcess == NULL)
        return ;
    if (!TerminateProcess(hProcess, 0))
        CloseHandle(hProcess);
}

bool KIPC::checkProcess(int pid)
{
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
                                  PROCESS_VM_READ,
                                  FALSE, pid);
    if (hProcess == NULL)
        return false;
    CloseHandle(hProcess);
    return true;
}