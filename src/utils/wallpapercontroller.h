// Copyright (C) 2024 Dingyuan Zhang <zhangdingyuan@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include "helper.h"

#include <woutput.h>

#include <QQmlEngine>
#include <QQuickItem>

class WallpaperController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Waylib::Server::WOutput* output READ output WRITE setOutput NOTIFY outputChanged)
    Q_PROPERTY(Helper::WallpaperType type READ type WRITE setType NOTIFY typeChanged)

    QML_ELEMENT

public:
    explicit WallpaperController(QObject *parent = nullptr);

Q_SIGNALS:
    void typeChanged();
    void outputChanged();

public:
    void setType(Helper::WallpaperType type);

    Helper::WallpaperType type() const { return m_type; }

    void setOutput(Waylib::Server::WOutput *output);

    inline Waylib::Server::WOutput *output() const { return m_output; }

private:
    void updateState();

private:
    Waylib::Server::WOutput *m_output;
    Helper::WallpaperType m_type{ Helper::WallpaperType::Normal };
};