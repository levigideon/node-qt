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

#include "../qt_v8.h"
#include "../QtCore/qsize.h"
#include "qwidget.h"

using namespace v8;

//
// QWidgetImpl()
// Extends QWidget to implement virtual methods from QWidget
//
class QWidgetImpl : public QWidget {
 public:
  QWidgetImpl(QWidget* parent, QWidgetWrap* wrapper) : QWidget(parent) {
    this->wrapper = wrapper;
  }

 protected:
  QWidgetWrap* wrapper;
  virtual void paintEvent(QPaintEvent* e) {
    QWidget::paintEvent(e);
    wrapper->paintEvent(e);
  };
  virtual void mousePressEvent(QMouseEvent* e) {
    QWidget::mousePressEvent(e);
    wrapper->mousePressEvent(e);
  };
  virtual void mouseReleaseEvent(QMouseEvent* e) {
    QWidget::mouseReleaseEvent(e);
    wrapper->mouseReleaseEvent(e);
  };
  virtual void mouseMoveEvent(QMouseEvent* e) {
    QWidget::mouseMoveEvent(e);
    wrapper->mouseMoveEvent(e);
  };
  virtual void keyPressEvent(QKeyEvent* e) {
    QWidget::keyPressEvent(e);
    wrapper->keyPressEvent(e);
  };
  virtual void keyReleaseEvent(QKeyEvent* e) {
    QWidget::keyReleaseEvent(e);
    wrapper->keyReleaseEvent(e);
  };
};

Nan::Persistent<FunctionTemplate> QWidgetWrap::prototype;
Nan::Persistent<Function> QWidgetWrap::constructor;

//
// QWidgetWrap()
//
QWidgetWrap::QWidgetWrap(QWidget* parent) {
  q_ = new QWidgetImpl(parent, this);
}

QWidgetWrap::~QWidgetWrap() {
  delete q_;
}

NAN_MODULE_INIT(QWidgetWrap::Initialize) {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("QWidget").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);  

  // Wrapped methods
  Nan::SetPrototypeMethod(tpl, "resize", Resize);
  Nan::SetPrototypeMethod(tpl, "show", Show);
  Nan::SetPrototypeMethod(tpl, "close", Close);
  Nan::SetPrototypeMethod(tpl, "size", Size);
  Nan::SetPrototypeMethod(tpl, "width", Width);
  Nan::SetPrototypeMethod(tpl, "height", Height);
  Nan::SetPrototypeMethod(tpl, "parent", Parent);
  Nan::SetPrototypeMethod(tpl, "objectName", ObjectName);
  Nan::SetPrototypeMethod(tpl, "setObjectName", SetObjectName);
  Nan::SetPrototypeMethod(tpl, "update", Update);
  Nan::SetPrototypeMethod(tpl, "hasMouseTracking", HasMouseTracking);
  Nan::SetPrototypeMethod(tpl, "setMouseTracking", SetMouseTracking);
  Nan::SetPrototypeMethod(tpl, "setFocusPolicy", SetFocusPolicy);
  Nan::SetPrototypeMethod(tpl, "move", Move);
  Nan::SetPrototypeMethod(tpl, "x", X);
  Nan::SetPrototypeMethod(tpl, "y", Y);
  Nan::SetPrototypeMethod(tpl, "sizeHint", SizeHint);
  Nan::SetPrototypeMethod(tpl, "setContentsMargins", SetContentsMargins);
  Nan::SetPrototypeMethod(tpl, "showFullScreen", ShowFullScreen);

  // Events
  QWidgetWrapBase::Inherit(tpl);

  prototype.Reset(tpl);
  Local<Function> function = Nan::GetFunction(tpl).ToLocalChecked();
  constructor.Reset(function);
  Nan::Set(target, Nan::New("QWidget").ToLocalChecked(), function);
}

NAN_METHOD(QWidgetWrap::New) {
  QWidget* q_parent = 0;

  if (info.Length() > 0) {
    QWidgetWrap* w_parent = node::ObjectWrap::Unwrap<QWidgetWrap>(info[0]->ToObject());
    q_parent = w_parent->GetWrapped();
  }

  QWidgetWrap* w = new QWidgetWrap(q_parent);
  w->Wrap(info.This());
}

// Supported implementations:
//    resize (int width, int height)
//    resize (QSize size)
NAN_METHOD(QWidgetWrap::Resize) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidget* q = w->GetWrapped();

  if (info.Length() == 2 && info[0]->IsNumber() && info[1]->IsNumber()) {
    q->resize(info[0]->NumberValue(), info[1]->NumberValue());
  }
  else if(info.Length() == 1 && info[0]->IsObject()) {
    QString objectConstructor = 
        qt_v8::ToQString(info[0]->ToObject()->GetConstructorName());
    
    if (objectConstructor != "QSize") {
      Nan::ThrowError(Exception::TypeError(
        Nan::New("QWidget::Resize: bad argument").ToLocalChecked()));
    }
    
    QSizeWrap* widgetWrapper = ObjectWrap::Unwrap<QSizeWrap>(
        info[0]->ToObject());
    QSize* size = widgetWrapper->GetWrapped();
    
    q->resize( *size );
  }
  else {
    Nan::ThrowError(Exception::TypeError(
      Nan::New("QWidget::Resize: bad argument").ToLocalChecked()));
  }

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QWidgetWrap::Show) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidget* q = w->GetWrapped();

  q->show();

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QWidgetWrap::Close) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidget* q = w->GetWrapped();

  q->close();

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QWidgetWrap::Size) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidget* q = w->GetWrapped();

  info.GetReturnValue().Set(QSizeWrap::NewInstance(q->size()));
}

NAN_METHOD(QWidgetWrap::Width) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidget* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->width()));
}

NAN_METHOD(QWidgetWrap::Height) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidget* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->height()));
}

NAN_METHOD(QWidgetWrap::ObjectName) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidget* q = w->GetWrapped();

  info.GetReturnValue().Set(qt_v8::FromQString(q->objectName()));
}

NAN_METHOD(QWidgetWrap::SetObjectName) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidget* q = w->GetWrapped();

  q->setObjectName(qt_v8::ToQString(info[0]->ToString()));

  info.GetReturnValue().Set(Nan::Undefined());
}

//
// QUIRK:
// Here: Parent() returns the parent's name
// Qt: Parent() returns QObject
// Intended mostly for sanity checks
//
NAN_METHOD(QWidgetWrap::Parent) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidget* q = w->GetWrapped();

  info.GetReturnValue().Set(qt_v8::FromQString(q->parent()->objectName()));
}

NAN_METHOD(QWidgetWrap::Update) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidget* q = w->GetWrapped();

  q->update();

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QWidgetWrap::HasMouseTracking) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidget* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->hasMouseTracking()));
}

NAN_METHOD(QWidgetWrap::SetMouseTracking) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidget* q = w->GetWrapped();

  q->setMouseTracking(info[0]->BooleanValue());

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QWidgetWrap::SetFocusPolicy) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidget* q = w->GetWrapped();

  q->setFocusPolicy((Qt::FocusPolicy)(info[0]->IntegerValue()));

  info.GetReturnValue().Set(Nan::Undefined());
}

// Supported implementations:
//    move (int x, int y)
NAN_METHOD(QWidgetWrap::Move) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidget* q = w->GetWrapped();

  q->move(info[0]->IntegerValue(), info[1]->IntegerValue());

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(QWidgetWrap::X) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidget* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->x()));
}

NAN_METHOD(QWidgetWrap::Y) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidget* q = w->GetWrapped();

  info.GetReturnValue().Set(Nan::New(q->y()));
}

NAN_METHOD(QWidgetWrap::SizeHint) {
  QWidgetWrap* w = ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidget* q = w->GetWrapped();

  info.GetReturnValue().Set(QSizeWrap::NewInstance(q->sizeHint()));
}

// Supported implementations:
//    setContentsMargins (int left, int top, int right, int bottom)
NAN_METHOD(QWidgetWrap::SetContentsMargins) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidget* q = w->GetWrapped();

  q->setContentsMargins(info[0]->NumberValue(), info[1]->NumberValue(), info[2]->NumberValue(), info[3]->NumberValue());

  info.GetReturnValue().Set(Nan::Undefined());
}


NAN_METHOD(QWidgetWrap::ShowFullScreen) {
  QWidgetWrap* w = node::ObjectWrap::Unwrap<QWidgetWrap>(info.This());
  QWidget* q = w->GetWrapped();

  q->showFullScreen();

  info.GetReturnValue().Set(Nan::Undefined());
}