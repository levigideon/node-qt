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

#include <node.h>
#include <nan.h>
#include "../QtCore/qpointf.h"
#include "qpainterpath.h"
#include "../qt_v8.h"

using namespace v8;

Persistent<Function> QPainterPathWrap::constructor;

// Supported implementations:
//   QPainterPath ( ??? )
QPainterPathWrap::QPainterPathWrap(const FunctionCallbackInfo<Value>& args) {
  q_ = new QPainterPath();
}

QPainterPathWrap::~QPainterPathWrap() {
  delete q_;
}

void QPainterPathWrap::Initialize(Handle<Object> target) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(Nan::New("QPainterPath").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  tpl->PrototypeTemplate()->Set(Nan::New("moveTo").ToLocalChecked(),
      FunctionTemplate::New(MoveTo)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("lineTo").ToLocalChecked(),
      FunctionTemplate::New(LineTo)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("currentPosition").ToLocalChecked(),
      FunctionTemplate::New(CurrentPosition)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("closeSubpath").ToLocalChecked(),
      FunctionTemplate::New(CloseSubpath)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());
  target->Set(Nan::New("QPainterPath").ToLocalChecked(), constructor);
}

Handle<Value> QPainterPathWrap::New(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QPainterPathWrap* w = new QPainterPathWrap(args);
  w->Wrap(args.This());

  return args.This();
}

// Supported versions:
//   moveTo( QPointF() )
Handle<Value> QPainterPathWrap::MoveTo(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QPainterPathWrap* w = ObjectWrap::Unwrap<QPainterPathWrap>(args.This());
  QPainterPath* q = w->GetWrapped();

  QString arg0_constructor;
  if (args[0]->IsObject()) {
    arg0_constructor = 
        qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());
  }
  
  if (arg0_constructor != "QPointF")
    return ThrowException(Exception::TypeError(
      String::New("QPainterPathWrap::MoveTo: argument not recognized")));

  // moveTo( QPointF point )
  QPointFWrap* pointf_wrap = ObjectWrap::Unwrap<QPointFWrap>(
      args[0]->ToObject());
  QPointF* pointf = pointf_wrap->GetWrapped();

  q->moveTo(*pointf);

  return scope.Close(Undefined());
}

Handle<Value> QPainterPathWrap::CurrentPosition(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QPainterPathWrap* w = ObjectWrap::Unwrap<QPainterPathWrap>(args.This());
  QPainterPath* q = w->GetWrapped();

  return scope.Close(QPointFWrap::NewInstance(q->currentPosition()));
}

// Supported versions:
//   lineTo( QPointF() )
Handle<Value> QPainterPathWrap::LineTo(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QPainterPathWrap* w = ObjectWrap::Unwrap<QPainterPathWrap>(args.This());
  QPainterPath* q = w->GetWrapped();

  QString arg0_constructor;
  if (args[0]->IsObject()) {
    arg0_constructor = 
        qt_v8::ToQString(args[0]->ToObject()->GetConstructorName());
  }
  
  if (arg0_constructor != "QPointF")
    return ThrowException(Exception::TypeError(
      String::New("QPainterPathWrap::MoveTo: argument not recognized")));

  // lineTo( QPointF point )
  QPointFWrap* pointf_wrap = ObjectWrap::Unwrap<QPointFWrap>(
      args[0]->ToObject());
  QPointF* pointf = pointf_wrap->GetWrapped();

  q->lineTo(*pointf);

  return scope.Close(Undefined());
}

Handle<Value> QPainterPathWrap::CloseSubpath(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QPainterPathWrap* w = ObjectWrap::Unwrap<QPainterPathWrap>(args.This());
  QPainterPath* q = w->GetWrapped();

  q->closeSubpath();
  
  return scope.Close(Undefined());
}
