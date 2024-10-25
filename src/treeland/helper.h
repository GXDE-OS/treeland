// Copyright (C) 2024 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#pragma once

#include "foreigntoplevelmanagerv1.h"
#include "qmlengine.h"
#include "togglablegesture.h"
#include "virtualoutputmanager.h"
#include "windowmanagement.h"
#include "multitaskview.h"

#include <wglobal.h>
#include <wqmlcreator.h>
#include <wseat.h>
#include <wxdgdecorationmanager.h>

Q_MOC_INCLUDE(<wtoplevelsurface.h>)
Q_MOC_INCLUDE(<wxdgsurface.h>)
Q_MOC_INCLUDE(<qwgammacontorlv1.h>)
Q_MOC_INCLUDE(<qwoutputmanagementv1.h>)
Q_MOC_INCLUDE("surfacewrapper.h")
Q_MOC_INCLUDE("workspace.h")
Q_MOC_INCLUDE("rootsurfacecontainer.h")

QT_BEGIN_NAMESPACE
class QQuickItem;
QT_END_NAMESPACE

WAYLIB_SERVER_BEGIN_NAMESPACE
class WServer;
class WOutputRenderWindow;
class WOutputLayout;
class WCursor;
class WBackend;
class WOutputItem;
class WOutputViewport;
class WOutputLayer;
class WOutput;
class WXWayland;
class WXdgDecorationManager;
class WSocket;
class WSurface;
class WToplevelSurface;
class WSurfaceItem;
class WForeignToplevel;
class WOutputManagerV1;
WAYLIB_SERVER_END_NAMESPACE

QW_BEGIN_NAMESPACE
class qw_renderer;
class qw_allocator;
class qw_compositor;
class qw_output_configuration_v1;
QW_END_NAMESPACE

WAYLIB_SERVER_USE_NAMESPACE
QW_USE_NAMESPACE

class Output;
class SurfaceWrapper;
class SurfaceContainer;
class RootSurfaceContainer;
class ForeignToplevelV1;
class LockScreen;
class ShortcutV1;
class PersonalizationV1;
class WallpaperColorV1;
class WindowManagementV1;
class Multitaskview;
class DDEShellManagerV1;
class VirtualOutputV1;
class ShellHandler;

class Helper : public WSeatEventFilter
{
    friend class RootSurfaceContainer;
    Q_OBJECT
    Q_PROPERTY(bool socketEnabled READ socketEnabled WRITE setSocketEnabled NOTIFY socketEnabledChanged FINAL)
    Q_PROPERTY(RootSurfaceContainer* rootContainer READ rootContainer CONSTANT FINAL)
    Q_PROPERTY(int currentUserId READ currentUserId WRITE setCurrentUserId NOTIFY currentUserIdChanged FINAL)
    Q_PROPERTY(float animationSpeed READ animationSpeed WRITE setAnimationSpeed NOTIFY animationSpeedChanged FINAL)
    Q_PROPERTY(OutputMode outputMode READ outputMode WRITE setOutputMode NOTIFY outputModeChanged FINAL)
    Q_PROPERTY(QString cursorTheme READ cursorTheme NOTIFY cursorThemeChanged FINAL)
    Q_PROPERTY(QSize cursorSize READ cursorSize NOTIFY cursorSizeChanged FINAL)
    Q_PROPERTY(TogglableGesture* multiTaskViewGesture READ multiTaskViewGesture CONSTANT)
    Q_PROPERTY(TogglableGesture* windowGesture READ windowGesture CONSTANT)
    Q_PROPERTY(SurfaceWrapper* activatedSurface READ activatedSurface NOTIFY activatedSurfaceChanged FINAL)
    Q_PROPERTY(Workspace* workspace READ workspace CONSTANT FINAL)
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit Helper(QObject *parent = nullptr);
    ~Helper();

    enum class OutputMode
    {
        Copy,
        Extension
    };
    Q_ENUM(OutputMode)

    enum class CurrentMode
    {
        Normal,
        LockScreen,
        WindowSwitch,
        Multitaskview
    };
    Q_ENUM(CurrentMode)

    static Helper *instance();

    QmlEngine *qmlEngine() const;
    WOutputRenderWindow *window() const;
    ShellHandler *shellHandler() const;
    Workspace *workspace() const;
    void init();

    TogglableGesture *multiTaskViewGesture() const
    {
        return m_multiTaskViewGesture;
    }

    TogglableGesture *windowGesture() const
    {
        return m_windowGesture;
    }

    bool socketEnabled() const;
    void setSocketEnabled(bool newSocketEnabled);

    RootSurfaceContainer *rootContainer() const;
    Output *getOutput(WOutput *output) const;

    int currentUserId() const;
    void setCurrentUserId(int uid);

    float animationSpeed() const;
    void setAnimationSpeed(float newAnimationSpeed);

    OutputMode outputMode() const;
    void setOutputMode(OutputMode mode);
    Q_INVOKABLE void addOutput();

    void addSocket(WSocket *socket);
    WXWayland *createXWayland();
    void removeXWayland(WXWayland *xwayland);

    WSocket *defaultWaylandSocket() const;
    WXWayland *defaultXWaylandSocket() const;

    PersonalizationV1 *personalization() const;
    void seatSendStartDrag(WSeat *seat);

    WSeat *seat() const;

    void toggleOutputMenuBar(bool show);

    QString cursorTheme() const;
    QSize cursorSize() const;

    Q_INVOKABLE bool isLaunchpad(WLayerSurface *surface) const;

public Q_SLOTS:
    void activateSurface(SurfaceWrapper *wrapper, Qt::FocusReason reason = Qt::OtherFocusReason);
    void forceActivateSurface(SurfaceWrapper *wrapper,
                              Qt::FocusReason reason = Qt::OtherFocusReason);
    void fakePressSurfaceBottomRightToReszie(SurfaceWrapper *surface);

Q_SIGNALS:
    void socketEnabledChanged();
    void primaryOutputChanged();
    void currentUserIdChanged();
    void activatedSurfaceChanged();

    void animationSpeedChanged();
    void socketFileChanged();
    void outputModeChanged();
    void cursorThemeChanged();
    void cursorSizeChanged();

private Q_SLOTS:
    void onOutputAdded(WOutput *output);
    void onOutputRemoved(WOutput *output);
    void onSurfaceModeChanged(WSurface *surface, WXdgDecorationManager::DecorationMode mode);
    void setGamma(struct wlr_gamma_control_manager_v1_set_gamma_event *event);
    void onOutputTestOrApply(qw_output_configuration_v1 *config, bool onlyTest);
    void onDockPreview(std::vector<SurfaceWrapper *> surfaces,
                       WSurface *target,
                       QPoint pos,
                       ForeignToplevelV1::PreviewDirection direction);
    void onDockPreviewTooltip(QString tooltip,
                              WSurface *target,
                              QPoint pos,
                              ForeignToplevelV1::PreviewDirection direction);
    void onShowDesktop();
    void onSetCopyOutput(treeland_virtual_output_v1 *virtual_output);
    void onRestoreCopyOutput(treeland_virtual_output_v1 *virtual_output);
    void onSurfaceWrapperAdded(SurfaceWrapper *wrapper);
    void onSurfaceWrapperAboutToRemove(SurfaceWrapper *wrapper);

private:
    void allowNonDrmOutputAutoChangeMode(WOutput *output);
    void enableOutput(WOutput *output);

    int indexOfOutput(WOutput *output) const;

    void setOutputProxy(Output *output);

    SurfaceWrapper *keyboardFocusSurface() const;
    void reuqestKeyboardFocusForSurface(SurfaceWrapper *newActivateSurface, Qt::FocusReason reason);
    SurfaceWrapper *activatedSurface() const;
    void setActivatedSurface(SurfaceWrapper *newActivateSurface);

    void setCursorPosition(const QPointF &position);

    bool beforeDisposeEvent(WSeat *seat, QWindow *watched, QInputEvent *event) override;
    bool afterHandleEvent([[maybe_unused]] WSeat *seat,
                          WSurface *watched,
                          QObject *surfaceItem,
                          QObject *,
                          QInputEvent *event) override;
    bool unacceptedEvent(WSeat *, QWindow *, QInputEvent *event) override;
    void toggleMultitaskview(Multitaskview::ActiveReason reason);
    void handleLeftButtonStateChanged(const QInputEvent *event);
    void handleWhellValueChanged(const QInputEvent *event);
    bool doGesture(QInputEvent *event);
    Output *createNormalOutput(WOutput *output);
    Output *createCopyOutput(WOutput *output, Output *proxy);

    static Helper *m_instance;

    CurrentMode m_currentMode{ CurrentMode::Normal };

    // qtquick helper
    WOutputRenderWindow *m_renderWindow = nullptr;
    QQuickItem *m_dockPreview = nullptr;

    // gesture
    TogglableGesture *m_multiTaskViewGesture = nullptr;
    TogglableGesture *m_windowGesture = nullptr;

    // wayland helper
    WServer *m_server = nullptr;
    WSocket *m_socket = nullptr;
    WSeat *m_seat = nullptr;
    WBackend *m_backend = nullptr;
    qw_renderer *m_renderer = nullptr;
    qw_allocator *m_allocator = nullptr;

    // protocols
    qw_compositor *m_compositor = nullptr;
    ShellHandler *m_shellHandler = nullptr;
    WXWayland *m_defaultXWayland = nullptr;
    WXdgDecorationManager *m_xdgDecorationManager = nullptr;
    WForeignToplevel *m_foreignToplevel = nullptr;
    ForeignToplevelV1 *m_treelandForeignToplevel = nullptr;
    ShortcutV1 *m_shortcut = nullptr;
    PersonalizationV1 *m_personalization = nullptr;
    WallpaperColorV1 *m_wallpaperColorV1 = nullptr;
    WOutputManagerV1 *m_outputManager = nullptr;
    WindowManagementV1 *m_windowManagement = nullptr;
    WindowManagementV1::DesktopState m_showDesktop = WindowManagementV1::DesktopState::Normal;
    DDEShellManagerV1 *m_ddeShellV1 = nullptr;
    VirtualOutputV1 *m_virtualOutput = nullptr;

    // private data
    QList<Output *> m_outputList;
    QPointer<QQuickItem> m_taskSwitch;

    SurfaceWrapper *m_activatedSurface = nullptr;
    RootSurfaceContainer *m_rootSurfaceContainer = nullptr;
    LockScreen *m_lockScreen = nullptr;
    int m_currentUserId = -1;
    float m_animationSpeed = 1.0;
    OutputMode m_mode = OutputMode::Extension;
    std::optional<QPointF> m_fakelastPressedPosition;

    QPointer<Multitaskview> m_multitaskview;
};