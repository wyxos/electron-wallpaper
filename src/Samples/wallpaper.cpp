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

#include "wallpaper.h"
#include <napi.h>
#include <windows.h>
#include <winuser.h>
#include <iostream>

HWND workerW = NULL;

BOOL CALLBACK FindWorkerW(HWND hwnd, LPARAM param)
{
    HWND shelldll = FindWindowEx(hwnd, NULL, "SHELLDLL_DefView", NULL);

    if (shelldll)
    {
        workerW = FindWindowEx(NULL, hwnd, "WorkerW", NULL);
        return FALSE;
    }
    return TRUE;
}

bool wallpaper::attach(HWND handle, int x, int y)
{
    HWND progman = FindWindowA("Progman", NULL);

    LRESULT result = SendMessageTimeoutA(
        progman,
        0x052C,
        NULL,
        NULL,
        SMTO_NORMAL,
        1000,
        NULL);

    EnumWindows(&FindWorkerW, reinterpret_cast<LPARAM>(&workerW));

    SetWindowPos(
        handle,
        workerW,
        x,
        y,
        NULL,
        NULL,
        SWP_NOSIZE);

    SetParent(handle, workerW);

    return wallpaper::setPosition(handle, x, y);
}

bool wallpaper::detach(HWND handle)
{
    SetParent(handle, NULL);

    return true;
}

bool wallpaper::setPosition(HWND handle, int x, int y)
{
    if (!workerW)
    {
        EnumWindows(&FindWorkerW, reinterpret_cast<LPARAM>(&workerW));
    }

    POINT pt = {};

    pt.x = x;

    pt.y = y;

    ScreenToClient(workerW, &pt);

    SetWindowPos(handle, HWND_TOP, pt.x, pt.y, NULL, NULL, SWP_NOSIZE);

    return true;
}

POINT wallpaper::getCoordinate(int x, int y)
{
    if (!workerW)
    {
        EnumWindows(&FindWorkerW, reinterpret_cast<LPARAM>(&workerW));
    }

    POINT pt = {};

    pt.x = x;

    pt.y = y;

    ScreenToClient(workerW, &pt);

    return pt;
}

Napi::Boolean wallpaper::AttachWrapped(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 3)
    {
        Napi::TypeError::New(env, "Argument missing.").ThrowAsJavaScriptException();
    }

    if (!info[1].IsNumber() || !info[2].IsNumber())
    {
        Napi::TypeError::New(env, "Argument 2 and 3 should to be integers.").ThrowAsJavaScriptException();
    }

    Napi::Number x = info[1].As<Napi::Number>();

    Napi::Number y = info[2].As<Napi::Number>();

    Napi::Buffer<void *> handle = info[0].As<Napi::Buffer<void *>>();

    HWND target = static_cast<HWND>(*reinterpret_cast<void **>(handle.Data()));

    bool value = wallpaper::attach(target, x, y);

    return Napi::Boolean::New(env, value);
}

Napi::Boolean wallpaper::DetachWrapped(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1)
    {
        Napi::TypeError::New(env, "Argument missing.").ThrowAsJavaScriptException();
    }

    Napi::Buffer<void *> handle = info[0].As<Napi::Buffer<void *>>();

    HWND target = static_cast<HWND>(*reinterpret_cast<void **>(handle.Data()));

    bool value = wallpaper::detach(target);

    return Napi::Boolean::New(env, value);
}

Napi::Boolean wallpaper::SetPositionWrapped(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 3)
    {
        Napi::TypeError::New(env, "Argument missing.").ThrowAsJavaScriptException();
    }

    if (!info[1].IsNumber() || !info[2].IsNumber())
    {
        Napi::TypeError::New(env, "Argument 2 and 3 should to be integers.").ThrowAsJavaScriptException();
    }

    Napi::Number x = info[1].As<Napi::Number>();

    Napi::Number y = info[2].As<Napi::Number>();

    Napi::Buffer<void *> handle = info[0].As<Napi::Buffer<void *>>();

    HWND target = static_cast<HWND>(*reinterpret_cast<void **>(handle.Data()));

    bool value = wallpaper::setPosition(target, x, y);

    return Napi::Boolean::New(env, value);
}

Napi::Object wallpaper::GetCoordinateWrapped(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 2)
    {
        Napi::TypeError::New(env, "Argument missing.").ThrowAsJavaScriptException();
    }

    if (!info[0].IsNumber() || !info[1].IsNumber())
    {
        Napi::TypeError::New(env, "Argument 1 and 2 should to be integers.").ThrowAsJavaScriptException();
    }

    Napi::Number x = info[0].As<Napi::Number>();

    Napi::Number y = info[1].As<Napi::Number>();

    POINT value = wallpaper::getCoordinate(x, y);

    Napi::Object object = Napi::Object::New(env);

    object.Set("x", value.x);

    object.Set("y", value.y);

    return object;
}

Napi::Object wallpaper::Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("attach", Napi::Function::New(env, wallpaper::AttachWrapped));
    exports.Set("detach", Napi::Function::New(env, wallpaper::DetachWrapped));
    exports.Set("setPosition", Napi::Function::New(env, wallpaper::SetPositionWrapped));
    exports.Set("getCoordinate", Napi::Function::New(env, wallpaper::GetCoordinateWrapped));

    return exports;
}
