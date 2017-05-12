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

#include "qmouseevent.h"

using namespace v8;

Nan::Persistent<FunctionTemplate> QMouseEventWrap::prototype;
Nan::Persistent<Function> QMouseEventWrap::constructor;

QMouseEventWrap::QMouseEventWrap() : q_(NULL) {
  // Standalone constructor not implemented
  // Use SetWrapped()
}

QMouseEventWrap::~QMouseEventWrap() {
  delete q_;
}

NAN_MODULE_INIT(QMouseEventWrap::Initialize) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("QMouseEvent").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  Nan::SetPrototypeMethod(tpl, "x", X);
  Nan::SetPrototypeMethod(tpl, "y", Y);
  Nan::SetPrototypeMethod(tpl, "button", Button);

  prototype.Reset(tpl);
  Local<Function> function = Nan::GetFunction(tpl).ToLocalChecked();
  constructor.Reset(function);
  Nan::Set(target, Nan::New("QMouseEvent").ToLocalChecked(), function);
}

NAN_METHOD(QMouseEventWrap::New) {
  QMouseEventWrap* w = new QMouseEventWrap();
  w->Wrap(info.This());
}

Handle<Value> QMouseEventWrap::NewInstance(QMouseEvent q) {
  Nan::EscapableHandleScope scope;
  
  Local<Object> instance = Nan::NewInstance(Nan::New(constructor), 0, NULL).ToLocalChecked();
  QMouseEventWrap* w = node::ObjectWrap::Unwrap<QMouseEventWrap>(instance);
  w->SetWrapped(q);

  return scope.Escape(instance);
}

NAN_METHOD(QMouseEventWrap::X) {
  QMouseEventWrap* w = node::ObjectWrap::Unwrap<QMouseEventWrap>(info.This());
  QMouseEvent* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->x()));
}

NAN_METHOD(QMouseEventWrap::Y) {
  QMouseEventWrap* w = node::ObjectWrap::Unwrap<QMouseEventWrap>(info.This());
  QMouseEvent* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->y()));
}

NAN_METHOD(QMouseEventWrap::Button) {
  QMouseEventWrap* w = node::ObjectWrap::Unwrap<QMouseEventWrap>(info.This());
  QMouseEvent* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->button()));
}
