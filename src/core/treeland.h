// Copyright (C) 2023 Dingyuan Zhang <lxz@mkacg.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include "proxyinterface.h"

#include <WServer>
#include <wsocket.h>

#include <QDBusContext>
#include <QDBusUnixFileDescriptor>

#include <memory>

class QmlEngine;
class QLocalSocket;
class Helper;
class PluginInterface;

namespace Treeland {

class Treeland
    : public QObject
    , protected QDBusContext
    , TreelandProxyInterface
{
    Q_OBJECT

public:
    explicit Treeland();
    ~Treeland();

    Q_INVOKABLE void retranslate() noexcept;

    bool testMode() const;

    bool debugMode() const;

    QmlEngine *qmlEngine() const override;
    Workspace *workspace() const override;
    RootSurfaceContainer *rootSurfaceContainer() const override;

    void blockActivateSurface(bool block) override;
    bool isBlockActivateSurface() const override;

Q_SIGNALS:
    void socketDisconnected();

public Q_SLOTS:
    bool ActivateWayland(QDBusUnixFileDescriptor fd);
    QString XWaylandName();

private Q_SLOTS:
    void loadPlugin(const QString &path);
    void connected();
    void disconnected();
    void readyRead();
    void error();

private:
    QLocalSocket *m_socket{ nullptr };
    QLocalSocket *m_helperSocket{ nullptr };
    Helper *m_helper{ nullptr };
    std::unique_ptr<QmlEngine> m_qmlEngine;
    QMap<QString, std::shared_ptr<WAYLIB_SERVER_NAMESPACE::WSocket>> m_userWaylandSocket;
    QMap<QString, std::shared_ptr<QDBusUnixFileDescriptor>> m_userDisplayFds;
    std::vector<PluginInterface *> m_plugins;
    std::vector<QAction *> m_shortcuts;
};
} // namespace Treeland