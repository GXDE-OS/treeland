// Copyright (C) 2024 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Shapes
import Waylib.Server
import Treeland

Item {
    id: root

    required property SurfaceItem surface
    // surface?.parent maybe is a `SubsurfaceContainer`
    readonly property SurfaceWrapper wrapper: surface?.parent as SurfaceWrapper
    readonly property real cornerRadius: wrapper?.radius ?? 0

    anchors.fill: parent
    opacity: content.alphaModifier

    Loader {
        id: blurLoader

        // 把模糊裁剪到真实窗口几何(排除 CSD 应用自绘的透明阴影边距), 避免模糊溢出到窗口四周。
        readonly property rect cg: wrapper?.contentGeometry ?? Qt.rect(0, 0, 0, 0)
        readonly property bool hasCg: cg.width > 0 && cg.height > 0

        x: hasCg ? cg.x : 0
        y: hasCg ? cg.y : 0
        width: hasCg ? cg.width : root.width
        height: hasCg ? cg.height : root.height
        active: wrapper?.blur ?? false
        sourceComponent: Blur {
            anchors.fill: parent
            radiusEnabled: cornerRadius > 0
            radius: cornerRadius
        }
    }

    SurfaceItemContent {
        id: content
        surface: root.surface?.surface ?? null
        anchors.fill: parent
        opacity: effectLoader.active ? 0 : parent.opacity
        live: root.surface && !(root.surface.flags & SurfaceItem.NonLive)
        smooth: root.surface?.smooth ?? true

        onDevicePixelRatioChanged: {
            if (wrapper) {
                wrapper.updateSurfaceSizeRatio()
            }
        }
    }

    Loader {
        id: effectLoader

        anchors.fill: parent
        active: {
            if (GraphicsInfo.api === GraphicsInfo.Software)
                return false;

            if (!root.wrapper)
                return false;
            // 圆角裁剪不再要求"服务端装饰(decoration 非空)": 无边框/CSD 窗口(如 DTK 应用,
            // 自绘标题栏)也由合成器统一裁圆角。仍受 noCornerRadius(最大化/全屏/平铺时关闭)约束。
            return (cornerRadius > 0) &&
                    !root.wrapper.noCornerRadius &&
                    root.wrapper.visibleDecoration;
        }

        sourceComponent: Shape {
            fillMode: Shape.PreserveAspectFit
            preferredRendererType: Shape.CurveRenderer
            ShapePath {
                strokeWidth: 0
                fillItem: content
                PathRectangle {
                    readonly property real scale: width / content.width

                    x: content.bufferSourceRect.x
                    y: content.bufferSourceRect.y
                    width: content.bufferSourceRect.width
                    height: content.bufferSourceRect.height
                    topLeftRadius: wrapper?.noTitleBar ? cornerRadius * scale : 0
                    topRightRadius: wrapper?.noTitleBar ? cornerRadius * scale : 0
                    bottomLeftRadius: cornerRadius * scale
                    bottomRightRadius: cornerRadius * scale
                }
            }
        }
    }
}
