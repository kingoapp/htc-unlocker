import QtQuick 1.1
import "widget"

Rectangle {
    id: deviceconnectstate
    width: parent.width; height: 220
    color: "#f9f9f9"

    function lock() {
        loader.state = "relock";
        loader.source = "UnlockView.qml";
    }
    function unlock() {
        loader.state = "unlock";
        loader.source = "UnlockView.qml";
    }

    Connections {
        target: deviceChecker
        onDeviceStateChanged: {
            switch (state) {
                case 0: { // disconnect 
                    image_connect_state.connected = false
                    text_device_state.text = qsTr("Please connect your device")
                    text_device_desc.text = qsTr("Enable USB debugging mode on your Android device")
                    button_help.visible = true
                    button_lock.enabled = false
                    button_unlock.enabled = false
                } break;
                case 1: { // connected
                    image_connect_state.connected = true
                    text_device_state.text = qsTr("Notifications")
                    text_device_desc.text = qsTr("Unlock bootloader will void your warranty and clear all the user data. \n" + "After unlocking bootloader, you can flash custom ROM and root your device.")
                    button_help.visible = false
                    button_lock.enabled = true
                    button_unlock.enabled = true
                } break;
            }
        }
    }

    Item {
        width: parent.width; height: 135
        Row {
            anchors.centerIn: parent; spacing: 20
            Row {
                spacing: 5; anchors.verticalCenter: parent.verticalCenter
                Image { source: "./image/computer.png"; anchors.verticalCenter: parent.verticalCenter }
                Rectangle {
                    id: indicator_connect
                    width: 35; height: 3; color: image_connect_state.connected ? "#78c221" : "dimgray"
                    anchors.verticalCenter: parent.verticalCenter
                }

                Image { id: image_connect_state; anchors.verticalCenter: parent.verticalCenter
                    property bool connected: false
                    source: connected ? "./image/phont_connect_2.png" : "./image/phont_connect_1.png"
                }
            }
            Column {
                spacing: 5
                anchors.verticalCenter: parent.verticalCenter
                Text { smooth: true
                    id: text_device_state; width: 500
                    text: qsTr("Please connect your device")
                    font { pixelSize: 28 }
                }
                Row {
                    id: row_desc
                    spacing: 10
                    Text { smooth: true
                        id: text_device_desc; color: "dimgray"; height: 30
                        text: qsTr("Enable USB debugging mode on your Android device")
                        lineHeight: 1.2
                        font { pixelSize: 12 }
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    Button {
                        id: button_help
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            width: 75; height: 28
                            radius: 3; smooth: true
                            gradient: Gradient {
                                GradientStop { position: 0; color: button_help.containsMouse ? "#f7f7f7" : "#f7f7f7" }
                                GradientStop { position: 1; color: button_help.containsMouse ? "#f7f7f7" : "#e7e7e7" }
                            }
                            border { width: 1; color: "#c0c0c0" }
                            Text { smooth: true
                                anchors.centerIn: parent
                                text: qsTr("Detail")
                                color: "black"
                            }
                        }
                        onClicked: messageprovider.source = "OpenDebugHelp.qml"
                    }
                }
            }
        }
    }

    Rectangle {
        width: parent.width; height: 80
        anchors.bottom: parent.bottom
        color: "#f9f9f9"
        MouseArea { anchors.fill: parent; hoverEnabled: true }
        Rectangle {
            width: parent.width - 30; height: 1
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#d2d2d2"
        }
        Row {
            anchors.centerIn: parent
            spacing: 50
            Button {
                id: button_unlock
                width: 165; height: 44
                enabled: false
                Rectangle {
                    radius: 3; smooth: true
                    border { width: 1; color: button_unlock.enabled ? "#4a880b" : "#d7d7d7" }
                    anchors.fill: parent
                    gradient: Gradient {
                        GradientStop { position: 0; color: button_unlock.enabled ? button_unlock.containsMouse ? "#78C221" : "#78c221" : "#e7e7e7" }
                        GradientStop { position: 1; color: button_unlock.enabled ? button_unlock.containsMouse ? "#78C221" : "#5DAA1D" : "#d7d7d7" }
                    }
                    Text { smooth: true
                        anchors.centerIn: parent
                        text: qsTr("UnLock"); color: button_unlock.enabled ? "white" : "#a7a7a7"
                        font { pixelSize: 20 }
                    }
                }
                onClicked: {
                    deviceconnectstate.unlock()
                }
            }
            Button {
                id: button_lock
                width: 165; height: 44
                enabled: false
                Rectangle {
                    radius: 3; smooth: true
                    border { width: 1; color: button_lock.enabled ? "#4a880b" : "#d7d7d7" }
                    anchors.fill: parent
                    gradient: Gradient {
                        GradientStop { position: 0; color: button_lock.enabled ? button_lock.containsMouse ? "#78C221" : "#78c221" : "#e7e7e7" }
                        GradientStop { position: 1; color: button_lock.enabled ? button_lock.containsMouse ? "#78C221" : "#5DAA1D" : "#d7d7d7" }
                    }
                    Text { smooth: true
                        anchors.centerIn: parent
                        text: qsTr("Lock"); color: button_lock.enabled ? "white" : "#a7a7a7"
                        font { pixelSize: 20 }
                    }
                }
                onClicked: {
                    deviceconnectstate.lock()
                }
            }
        }
    }
}
