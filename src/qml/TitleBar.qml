// Copyright (C) 2024 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls
import Waylib.Server
import Treeland
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

Control {
    id: root

    required property SurfaceWrapper surface
    readonly property SurfaceItem surfaceItem: surface.surfaceItem
    readonly property bool noRadius: surface.radius === 0 || surface.noCornerRadius
    property D.Palette backgroundColor: DS.Style.highlightPanel.background
    property D.Palette outerShadowColor: DS.Style.highlightPanel.dropShadow
    property D.Palette innerShadowColor: DS.Style.highlightPanel.innerShadow

    height: 30
    width: surfaceItem.width

    HoverHandler {
        // block hover events to resizing mouse area, avoid cursor change
        cursorShape: Qt.ArrowCursor
    }

    // Normal mouse click
    TapHandler {
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onTapped: (eventPoint, button) => {
            if (button === Qt.RightButton) {
                surface.requestShowWindowMenu(eventPoint.position)
            } else {
                Helper.activateSurface(surface)
            }
        }
        onPressedChanged: {
            if (pressed)
                surface.requestMove()
        }
        onDoubleTapped: (_, button) => {
            if (button === Qt.LeftButton) {
                surface.requestToggleMaximize()
            }
        }
    }

    // Touch screen click
    TapHandler {
        acceptedButtons: Qt.NoButton
        acceptedDevices: PointerDevice.TouchScreen
        onDoubleTapped: surface.requestToggleMaximize()
        onLongPressed: surface.requestShowWindowMenu(point.position)
    }

    Rectangle {
        id: titlebar
        anchors.fill: parent
        color: surface.shellSurface.isActivated ? "white" : "gray"
        layer.enabled: !root.noRadius
        layer.smooth: !root.noRadius
        opacity: !root.noRadius ? 0 : parent.opacity

        Row {
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
            }

            Item {
                id: control

                property D.Palette textColor: DS.Style.button.text
                property D.Palette backgroundColor: DS.Style.windowButton.background
            }

            Loader {
                objectName: "minimizeBtn"
                sourceComponent: D.WindowButton {
                    icon.name: "window_minimize"
                    textColor: control.textColor
                    height: root.height

                    onClicked: {
                        surface.requestMinimize()
                    }
                }
            }

            Loader {
                objectName: "quitFullBtn"
                visible: false
                sourceComponent: D.WindowButton {
                    icon.name: "window_quit_full"
                    textColor: control.textColor
                    height: root.height

                    onClicked: {
                    }
                }
            }

            Loader {
                id: maxOrWindedBtn

                objectName: "maxOrWindedBtn"
                sourceComponent: D.WindowButton {
                    icon.name: surface.isMaximized ? "window_restore" : "window_maximize"
                    textColor: control.textColor
                    height: root.height

                    onClicked: {
                        surface.requestToggleMaximize()
                    }
                }
            }

            Loader {
                objectName: "closeBtn"
                sourceComponent: Item {
                    height: root.height
                    width: closeBtn.implicitWidth
                    Rectangle {
                        anchors.fill: closeBtn
                        color: closeBtn.hovered ? "red" : "transparent"
                    }
                    D.WindowButton {
                        id: closeBtn
                        icon.name: "window_close"
                        textColor: control.textColor
                        height: parent.height

                        onClicked: {
                            surface.requestClose()
                        }
                    }
                }
            }
        }
    }

    Loader {
        anchors.fill: parent
        active: !root.noRadius
        sourceComponent: TRadiusEffect {
            anchors.fill: parent
            sourceItem: titlebar
            topLeftRadius: surface.radius
            topRightRadius: surface.radius
        }
    }
}