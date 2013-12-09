import QtQuick 1.1
import "widget"

Item {
	width: parent.width; height: 40; z: 100

    Item {
        anchors.fill: parent; anchors.bottomMargin: 5
        Image {
            id: image_logo
            anchors { verticalCenter: parent.verticalCenter; left: parent.left; leftMargin: 12 }
            source: "./image/logo.png"
        }
        Text { smooth: true
            id: text_title
            anchors { verticalCenter: parent.verticalCenter; left: image_logo.right; leftMargin: 5 }
            font { pixelSize: 16 }
            text: qsTr("HTC Bootloader Unlock")
        }
        Text {
            id: text_version
            anchors { bottom: text_title.bottom; left: text_title.right; leftMargin: 5 }
            color: "darkblue"
            font { pixelSize: 12 }
            text: "(Ver " + qApp.applicationVersion + ")"
        }
        Row {
            id: button_group
            anchors { top: parent.top; right: parent.right; rightMargin: 10 }
            Button {
                id: button_min
                Item {
                    width: 30; height: titlebar.height
                    Image { source: button_min.containsMouse ? "./image/button_min_2.png" : "./image/button_min_1.png"; anchors.centerIn: parent
                        anchors.verticalCenterOffset: 4
                    }
                }
                onClicked: window.showMinimized()
            }
            Button {
                id: button_close
                Item {
                    width: 30; height: titlebar.height
                    Image { source: button_close.containsMouse ? "./image/button_close_2.png" : "./image/button_close_1.png"; anchors.centerIn: parent }
                }
                onClicked: window.close()
            }
        }
    }
}
