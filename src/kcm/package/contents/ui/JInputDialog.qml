
/**
 * SPDX-FileCopyrightText: 2021 Wang Rui <wangrui@jingos.com>
 *                         
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */
 
import QtQuick 2.2
import org.kde.kirigami 2.15
import QtQuick.Controls 2.14
import QtGraphicalEffects 1.15
import QtQuick.Controls.Styles 1.4

Popup {
    id: dailog

    property string title
    property alias inputText: textInput.text
    property alias hintText: textInput.placeholderText
    property alias echoMode: textInput.echoMode
    property var wifiName
    property var devicePath
    property var specificPath
    property var connectionPath
    property int defaultFontSize: theme.defaultFont.pointSize

    signal cancelButtonClicked
    signal okButtonClicked
    signal enteredClick

    anchors.centerIn: applicationWindow().overlay

    height: units.gridUnit * 10.7
    width: units.gridUnit * 22.1

    parent: applicationWindow().overlay
    modal: true
    closePolicy: Popup.NoAutoClose
    Overlay.modal: Rectangle {
        color: "transparent"
    }

    contentItem: Item {
        anchors.fill: parent

        Text {
            id: titleText

            anchors.left: parent.left
            anchors.leftMargin: units.gridUnit * 1.4
            anchors.top: parent.top
            anchors.topMargin: units.gridUnit * 1.4
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            width: units.gridUnit * 12

            font.pointSize: defaultFontSize + 9
            color: "#000000"
            text: dailog.title
        }

        Row {
            anchors.right: parent.right
            anchors.rightMargin: units.gridUnit * 1.4
            anchors.top: parent.top
            anchors.topMargin: units.gridUnit * 1.4

            spacing: units.gridUnit * 2.1

            JIconButton {
                width: 34 * appScale + 10
                height: 34 * appScale + 10
                source: "qrc:/package/contents/image/cancel.png"

                onClicked: {
                    dailog.cancelButtonClicked()
                    dailog.close()
                    dailog.inputText = ""
                }
            }

            JIconButton {
                id: okButton

                width: 34 * appScale + 10
                height: 34 * appScale + 10

                enabled: textInput.length > 0
                source: enabled ? "qrc:/package/contents/image/confirm.png" : "qrc:/package/contents/image/confirm_disable.png"

                onClicked: {
                    dailog.okButtonClicked()
                    textInput.text = ""
                }
            }
        }

        Item {
            anchors.left: parent.left
            anchors.top: titleText.bottom
            anchors.topMargin: units.gridUnit * 2
            anchors.right: parent.right
            anchors.rightMargin: units.gridUnit * 2
            anchors.horizontalCenter: parent.horizontalCenter

            width: parent.width - units.gridUnit * 2.8
            height: units.gridUnit * 3.2

            JIconButton {
                id: closeButton

                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                height: 25 * appScale + 10
                width: 25 * appScale + 10

                visible: textInput.text
                source: "qrc:/package/contents/image/txt_close.png"

                onClicked: {
                    textInput.text = ""
                }
            }

            Rectangle {
                anchors.left: textInput.left
                anchors.bottom: parent.bottom
                anchors.right: closeButton.right

                width: parent.width
                height: 1

                color: "#FFE5E5EA"
            }

            TextField {
                id: textInput

                width: 415 * appScale

                anchors {
                    left: parent.left
                    right: visibleButton.left
                    leftMargin: units.gridUnit * 1.4
                    verticalCenter: parent.verticalCenter
                }

                font.pointSize: defaultFontSize + 2
                color: "#000000"
                focus: true

                onAccepted: {
                    if (okButton.enabled) {
                        dailog.enteredClick()
                        inputText = ""
                    }
                }

                background: Item {
                    width: parent.width
                    height: parent.height
                }

                cursorDelegate: Rectangle {
                    id: cursorBg

                    anchors.verticalCenter: parent.verticalCenter

                    width: units.devicePixelRatio * 2
                    height: parent.height / 2

                    color: "#FF3C4BE8"

                    Timer {
                        id: timer

                        interval: 700
                        repeat: true
                        running: textInput.focus

                        onTriggered: {
                            if (timer.running) {
                                cursorBg.visible = !cursorBg.visible
                            } else {
                                cursorBg.visible = false
                            }
                        }
                    }

                    Connections {
                        target: textInput
                        onFocusChanged: cursorBg.visible = focus
                    }
                }
            }
        }
    }

    background: Rectangle {
        id: bkground

        width: parent.width
        height: parent.height

        radius: 18
        color: "#FEFFFFFF"

        layer.enabled: true
        layer.effect: DropShadow {
            horizontalOffset: 0
            radius: 40
            samples: 25
            color: "#1A000000"
            verticalOffset: 10
            spread: 0
        }
    }

    onClosed: {
        textInput.text = ""
    }
}
