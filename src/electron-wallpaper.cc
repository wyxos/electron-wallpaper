//ElectronWallpaper.cc
#include <node.h>
#include <node_buffer.h>
#include <node_version.h>
#include <node_object_wrap.h>
#include <iostream>
#include <windows.h>
#include <winuser.h>
#include <iostream>
#include <v8.h>

using namespace std;

HWND workerw = NULL;

BOOL CALLBACK
FindWorkerW(HWND
hwnd,
LPARAM param
) {
HWND shelldll = FindWindowEx(hwnd, NULL, "SHELLDLL_DefView", NULL);

if (shelldll) {
workerw = FindWindowEx(NULL, hwnd, "WorkerW", NULL);
return
FALSE;
}

return
TRUE;
}

namespace ElectronWallpaper {
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Number;
    using v8::Object;
    using v8::Value;
    using v8::String;
    using v8::Context;

    void Attach(const FunctionCallbackInfo <Value> &args) {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();

        Local <Object> bufferObj = args[0].As<Object>();

        unsigned char *bufferData = (unsigned char *)node::Buffer::Data(bufferObj);

        unsigned long handle = *reinterpret_cast<unsigned long *>(bufferData);

        HWND target = (HWND)handle;

        HWND progman = FindWindowA("Progman", NULL);

        LRESULT result = SendMessageTimeoutA(
                progman,
                0x052C,
                NULL,
                NULL,
                SMTO_NORMAL,
                1000,
                NULL
        );

        EnumWindows(&FindWorkerW, reinterpret_cast<LPARAM>(&workerw));

        SetWindowLongPtrA(target, GWL_EXSTYLE, WS_EX_LAYERED);

        SetParent(target, workerw);

        args.GetReturnValue().Set(args[0]);
    }

    void Detach(const FunctionCallbackInfo <Value> &args) {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();

        Local <Object> bufferObj = args[0].As<Object>();

        unsigned char *bufferData = (unsigned char *)node::Buffer::Data(bufferObj);

        unsigned long handle = *reinterpret_cast<unsigned long *>(bufferData);

        HWND target = (HWND)handle;

        HWND previousParent = SetParent(target, NULL);

        args.GetReturnValue().Set(args[0]);
    }

    void Initialize(Local <Object> exports) {
        NODE_SET_METHOD(exports, "attach", Attach);
        NODE_SET_METHOD(exports, "detach", Detach);
    }

    NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize
    );
}
