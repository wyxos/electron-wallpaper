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

#include <napi.h>
#include <windows.h>
namespace wallpaper {
    bool attach(Napi::Buffer<void *> handle, int x, int y);
    bool detach(Napi::Buffer<void *> handle);
    bool setPosition(Napi::Buffer<void *> handle, int x, int y);
    RECT getLocalCoordinates(HWND handle);

    Napi::Boolean AttachWrapped(const Napi::CallbackInfo& info);
    Napi::Boolean DetachWrapped(const Napi::CallbackInfo& info);
    Napi::Boolean SetPositionWrapped(const Napi::CallbackInfo& info);

    Napi::Object Init(Napi::Env env, Napi::Object exports);
}  // namespace wallpaper
