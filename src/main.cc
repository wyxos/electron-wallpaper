/*
 * Copyright 2018 Joey J <maverick.slayer@live.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <v8.h>
#include <node.h>
#include <node_buffer.h>
#include <node_version.h>
#include <node_object_wrap.h>
#include <napi.h>
#include <windows.h>
#include <winuser.h>
#include <iostream>

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
    using v8::Integer;

    void Attach(const FunctionCallbackInfo <Value> &args) {
        v8::Isolate *isolate = v8::Isolate::GetCurrent();

        Local <Object> bufferObj = args[0].As<Object>();

        unsigned char *bufferData = (unsigned char *) node::Buffer::Data(bufferObj);

        int64_t handle = *reinterpret_cast<int64_t *>(bufferData);

        HWND target = (HWND) handle;

        HWND progman = FindWindowA("Progman", NULL);

        LRESULT result = SendMessageTimeoutA(
                progman,
                0x052C,
                NULL,
                NULL,
                SMTO_NORMAL,
                1000,
                NULL);

        EnumWindows(&FindWorkerW, reinterpret_cast<LPARAM>(&workerw));

//        SetWindowLongPtrA(target, GWL_EXSTYLE, WS_EX_LAYERED);

//        Local <Integer> originalX = args[1].As<Integer>();

//        Local <Integer> originalY = args[2].As<Integer>();

//        int x = originalX;

//        int y = originalY;

//        SetWindowPos(
//                target,
//                HWND_TOP,
//                x,
//                y,
//                NULL,
//                NULL,
//                NULL
//        );

        SetParent(target, workerw);

        args.GetReturnValue().Set(args[0]);
    }

    void Detach(const FunctionCallbackInfo <Value> &args) {
        v8::Isolate *isolate = v8::Isolate::GetCurrent();

        Local <Object> bufferObj = args[0].As<Object>();

        unsigned char *bufferData = (unsigned char *) node::Buffer::Data(bufferObj);

        int64_t handle = *reinterpret_cast<int64_t *>(bufferData);

        HWND
        target = (HWND)
        handle;

        HWND previousParent = SetParent(target, NULL);

        args.GetReturnValue().Set(args[0]);
    }

    void Initialize(Local <Object> exports) {
        NODE_SET_METHOD(exports, "attach", Attach);
        NODE_SET_METHOD(exports, "detach", Detach);
    }

    NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize);
}  //  namespace ElectronWallpaper
