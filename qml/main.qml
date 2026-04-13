/*
 * Copyright (C) 2021 CutefishOS Team.
 *
 * Author:     Reion Wong <reionwong@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: control

    focus: true

    property rect cropRect
    property bool cropping: false

    // 常量替代 FishUI.Units
    readonly property int smallSpacing: 4
    readonly property int largeSpacing: 8
    readonly property int smallRadius: 4
    readonly property color highlightColor: "#3daee9"

    Keys.enabled: true
    Keys.onEscapePressed: view.quit()

    Keys.onLeftPressed: {
        if (selectLayer.visible) {
            var newX = selectLayer.x - 10
            if (newX < control.x)
                newX = control.x
            selectLayer.x = newX
        }
    }

    Keys.onRightPressed: {
        if (selectLayer.visible) {
            var newX = selectLayer.x + 10
            if (newX > control.width - selectLayer.width)
                newX = control.width - selectLayer.width
            selectLayer.x = newX
        }
    }

    Keys.onUpPressed: {
        if (selectLayer.visible) {
            var newY = selectLayer.y - 10
            if (newY < control.y)
                newY = control.y
            selectLayer.y = newY
        }
    }

    Keys.onDownPressed: {
        if (selectLayer.visible) {
            var newY = selectLayer.y + 10
            if (newY > control.height - selectLayer.height)
                newY = control.height - selectLayer.height
            selectLayer.y = newY
        }
    }

    function refreshImage() {
        image.source = ""
        image.source = "file:///tmp/cutefish-screenshot.png"

        selectImage.source = ""
        selectImage.source = "file:///tmp/cutefish-screenshot.png"
    }

    function save() {
        view.saveFile(Qt.rect(selectLayer.x * Screen.devicePixelRatio,
                              selectLayer.y * Screen.devicePixelRatio,
                              selectLayer.width * Screen.devicePixelRatio,
                              selectLayer.height * Screen.devicePixelRatio))
    }

    function copyToClipboard() {
        view.copyToClipboard(Qt.rect(selectLayer.x * Screen.devicePixelRatio,
                                     selectLayer.y * Screen.devicePixelRatio,
                                     selectLayer.width * Screen.devicePixelRatio,
                                     selectLayer.height * Screen.devicePixelRatio))
    }

    Connections {
        target: view

        function onRefresh() {
            control.refreshImage()
        }
    }

    Image {
        id: image
        anchors.fill: parent
        asynchronous: true

        Rectangle {
            id: dimRect
            anchors.fill: parent
            color: "#000"
            opacity: 0.5
        }
    }

    Rectangle {
        id: selectLayer

        property int newX: 0
        property int newY: 0

        z: 999
        height: 0
        width: 0
        x: 0
        y: 0
        visible: false
        clip: true

        function reset() {
            selectLayer.x = 0
            selectLayer.y = 0
            selectLayer.newX = 0
            selectLayer.newY = 0
            selectLayer.visible = false
            selectLayer.width = 0
            selectLayer.height = 0
        }

        Image {
            id: selectImage
            width: control.width
            height: control.height
            asynchronous: true
            x: -selectLayer.x
            y: -selectLayer.y
        }

        Rectangle {
            anchors.fill: parent
            color: "transparent"
            border.width: 2
            border.color: control.highlightColor
        }

        DragHandler {
            target: selectLayer

            xAxis.enabled: true
            xAxis.minimum: control.x
            xAxis.maximum: control.width - selectLayer.width

            yAxis.enabled: true
            yAxis.minimum: control.y
            yAxis.maximum: control.height - selectLayer.height
        }

        MouseArea {
            id: selectLayerMouseArea
            anchors.fill: parent
            cursorShape: Qt.SizeAllCursor
            acceptedButtons: Qt.LeftButton
            onDoubleClicked: control.save()
        }
    }

    Rectangle {
        id: sizeToolTip
        visible: selectLayer.visible && selectLayer.width > 1 && selectLayer.height > 1

        width: sizeLabel.implicitWidth + control.largeSpacing
        height: sizeLabel.implicitHeight + control.largeSpacing

        z: 999
        x: selectLayer.x
        y: {
            var newY = selectLayer.y - sizeToolTip.height - control.smallSpacing
            if (newY < control.y)
                newY = control.y
            return newY
        }

        radius: control.smallRadius
        color: Qt.rgba(0.15, 0.15, 0.15, 0.9)
        border.width: 1
        border.color: Qt.rgba(1, 1, 1, 0.15)

        Label {
            id: sizeLabel
            anchors.centerIn: parent
            color: "white"
            text: "%1 * %2".arg(parseInt(selectLayer.width)).arg(parseInt(selectLayer.height))
        }
    }

    Rectangle {
        id: tools

        width: toolsLayout.implicitWidth + control.largeSpacing
        height: 36 + control.smallSpacing

        visible: selectLayer.visible && selectLayer.width > 1 && selectLayer.height > 1
        z: 999

        x: {
            var newX = selectLayer.x + selectLayer.width - tools.width
            if (newX < control.x)
                return control.x
            return newX
        }

        y: {
            var newY = 0
            if (selectLayer.y + selectLayer.height + tools.height + control.smallSpacing >= control.height) {
                newY = selectLayer.y - tools.height - control.smallSpacing
            } else {
                newY = selectLayer.y + selectLayer.height + control.smallSpacing
            }
            if (newY < control.y || newY > control.y + control.height)
                newY = control.height - tools.height
            return newY
        }

        radius: control.smallRadius
        color: "white"

        MouseArea {
            anchors.fill: parent
        }

        RowLayout {
            id: toolsLayout
            anchors.fill: parent
            anchors.leftMargin: control.smallSpacing
            anchors.rightMargin: control.smallSpacing
            anchors.topMargin: control.smallSpacing / 2
            anchors.bottomMargin: control.smallSpacing / 2

            ImageButton {
                iconMargins: control.largeSpacing
                size: 36
                source: "qrc:/images/save.svg"
                onClicked: control.save()
            }

            ImageButton {
                iconMargins: control.largeSpacing
                size: 36
                source: "qrc:/images/cancel.svg"
                onClicked: view.quit()
            }

            ImageButton {
                iconMargins: control.largeSpacing
                size: 36
                source: "qrc:/images/ok.svg"
                onClicked: control.copyToClipboard()
            }
        }
    }

    // Global mouse area for selection
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        cursorShape: Qt.CrossCursor

        onPressed: {
            selectLayer.visible = true
            selectLayer.x = mouseX
            selectLayer.y = mouseY
            selectLayer.newX = mouseX
            selectLayer.newY = mouseY
            selectLayer.width = 0
            selectLayer.height = 0
        }

        onPositionChanged: {
            if (!mouseArea.pressed)
                return

            if (mouseX >= selectLayer.newX) {
                selectLayer.width = mouseX < (control.x + control.width) ? (mouseX - selectLayer.x) : selectLayer.width
            } else {
                selectLayer.x = mouseX < control.x ? control.x : mouseX
                selectLayer.width = selectLayer.newX - selectLayer.x
            }

            if (mouseY >= selectLayer.newY) {
                selectLayer.height = mouseY < (control.y + control.height) ? (mouseY - selectLayer.y) : selectLayer.height
            } else {
                selectLayer.y = mouseY < control.y ? control.y : mouseY
                selectLayer.height = selectLayer.newY - selectLayer.y
            }
        }
    }
}
