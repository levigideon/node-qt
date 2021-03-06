// Copyright (c) 2012, Artur Adib
// All rights reserved.
//
// Author(s): Artur Adib <aadib@mozilla.com>
//
// You may use this file under the terms of the New BSD license as follows:
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Artur Adib nor the
//       names of contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL ARTUR ADIB BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF 
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include <node.h>
#include <nan.h>
#include <QPainter>

class QPainterWrap : public node::ObjectWrap {
 public:
  static Nan::Persistent<v8::FunctionTemplate> prototype;
  static NAN_MODULE_INIT(Initialize);
  QPainter* GetWrapped() const { return q_; };

 private:
  static Nan::Persistent<v8::Function> constructor;
  QPainterWrap();
  ~QPainterWrap();
  static NAN_METHOD(New);

  //
  // Wrapped methods exposed to JS
  //

  static NAN_METHOD(Begin);
  static NAN_METHOD(End);
  static NAN_METHOD(IsActive);
  static NAN_METHOD(Save);
  static NAN_METHOD(Restore);

  // State
  static NAN_METHOD(SetPen);
  static NAN_METHOD(SetFont);
  static NAN_METHOD(SetMatrix);

  // Paint actions
  static NAN_METHOD(FillRect);
  static NAN_METHOD(DrawText);
  static NAN_METHOD(DrawPixmap);
  static NAN_METHOD(DrawImage);
  static NAN_METHOD(StrokePath);

  // Wrapped object
  QPainter* q_;
};
