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
#include "qscrollbar.h"

using namespace v8;

Persistent<Function> QScrollBarWrap::constructor;

QScrollBarWrap::QScrollBarWrap(const FunctionCallbackInfo<Value>& args) : q_(NULL) {
}

QScrollBarWrap::~QScrollBarWrap() {
  // Since q_ is never new'd (it's always a pointer to an existing scrollbar), 
  // don't delete it! It'll segfault.
}

void QScrollBarWrap::Initialize(Handle<Object> target) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(Nan::New("QScrollBar").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Prototype
  tpl->PrototypeTemplate()->Set(Nan::New("value").ToLocalChecked(),
      FunctionTemplate::New(Value)->GetFunction());
  tpl->PrototypeTemplate()->Set(Nan::New("setValue").ToLocalChecked(),
      FunctionTemplate::New(SetValue)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());
  target->Set(Nan::New("QScrollBar").ToLocalChecked(), constructor);
}

Handle<Value> QScrollBarWrap::New(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollBarWrap* w = new QScrollBarWrap(args);
  w->Wrap(args.This());

  return args.This();
}

Handle<Value> QScrollBarWrap::NewInstance(QScrollBar *q) {
  HandleScope scope;
  
  Local<Object> instance = constructor->NewInstance(0, NULL);
  QScrollBarWrap* w = node::ObjectWrap::Unwrap<QScrollBarWrap>(instance);
  w->SetWrapped(q);

  return scope.Close(instance);
}

Handle<Value> QScrollBarWrap::Value(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollBarWrap* w = ObjectWrap::Unwrap<QScrollBarWrap>(args.This());
  QScrollBar* q = w->GetWrapped();

  return scope.Close(Integer::New(q->value()));
}

Handle<Value> QScrollBarWrap::SetValue(const FunctionCallbackInfo<Value>& args) {
  HandleScope scope;

  QScrollBarWrap* w = ObjectWrap::Unwrap<QScrollBarWrap>(args.This());
  QScrollBar* q = w->GetWrapped();

  q->setValue(args[0]->IntegerValue());

  return scope.Close(Undefined());
}
