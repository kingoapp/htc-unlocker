import QtQuick 1.1
import "widget"

Rectangle {
    id: opendebughelp
    width: parent.width; height: 220
    color: "#f9f9f9"
    state: "2.3"

    function init() {

    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
    }

    Item {
        width: parent.width; height: 135
        Column {
            anchors { fill: parent; topMargin: 15 }
            spacing: 10
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: -1
                Button {
                    id: button_2_3
                    z: opendebughelp.state == "2.3" ? 1 : 0
                    Rectangle {
                        width: 80; height: 35
                        border { width: 1; color: opendebughelp.state == "2.3" ? "#0F70A5" : "#B3B7BC" }
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: opendebughelp.state == "2.3" ? "#0BADF7" : "#F6F6F6" }
                            GradientStop { position: 1.0; color: opendebughelp.state == "2.3" ? "#0F8AC5" : "#EDEDEE" }
                        }
                        Text { 
                            anchors.centerIn: parent
                            text: qsTr("Ver 2.3")
                            color: opendebughelp.state == "2.3" ? "white" : "gray"
                        }
                    }
                    onClicked: { opendebughelp.state = "2.3" }
                }
                Button {
                    id: button_4_0
                    z: opendebughelp.state == "4.0" ? 1 : 0
                    Rectangle {
                        width: 80; height: 35
                        border { width: 1; color: opendebughelp.state == "4.0" ? "#0F70A5" : "#B3B7BC" }
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: opendebughelp.state == "4.0" ? "#0BADF7" : "#F6F6F6" }
                            GradientStop { position: 1.0; color: opendebughelp.state == "4.0" ? "#0F8AC5" : "#EDEDEE" }
                        }
                        Text { 
                            anchors.centerIn: parent
                            text: qsTr("Ver 4.0")
                            color: opendebughelp.state == "4.0" ? "white" : "gray"
                        }
                    }
                    onClicked: { opendebughelp.state = "4.0" }
                }
                Button {
                    id: button_4_3
                    z: opendebughelp.state == "4.2" ? 1 : 0
                    Rectangle {
                        width: 135; height: 35
                        border { width: 1; color: opendebughelp.state == "4.2" ? "#0F70A5" : "#B3B7BC" }
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: opendebughelp.state == "4.2" ? "#0BADF7" : "#F6F6F6" }
                            GradientStop { position: 1.0; color: opendebughelp.state == "4.2" ? "#0F8AC5" : "#EDEDEE" }
                        }
                        Text { 
                            anchors.centerIn: parent
                            text: qsTr("Ver 4.2 or higher")
                            color: opendebughelp.state == "4.2" ? "white" : "gray"
                        }
                    }
                    onClicked: { opendebughelp.state = "4.2" }
                }
            }
            Item {
                width: 750; height: 310
                anchors.horizontalCenter: parent.horizontalCenter
                Flickable {
                    id: image_container
                    width: 750; height: 310; clip: true
                    contentWidth: image.width; contentHeight: image.height

                    Image {
                        id: image
                        source: {
                            switch (opendebughelp.state) {
                                case "2.3": return "./image/setting_debug_mode_2_3.png";
                                case "4.0": return "./image/setting_debug_mode_4_0.png";
                                case "4.2": return "./image/setting_debug_mode_4_3.png";
                            }
                        }
                    }
                }
                Button {
                    id: button_left
                    visible: image_container.width < image_container.contentWidth
                    anchors { left: image_container.left; leftMargin: 10; verticalCenter: image_container.verticalCenter }
                    Image {
                        source: "./image/btn_left.png"
                        opacity: button_left.containsMouse ? 1 : 0.8
                    }
                    onClicked: {
                        if (image_container.contentX < 100) {
                            image_container.contentX = 0;
                        }
                        else {
                            image_container.contentX -= 100;
                        }
                    }
                }
                Button {
                    id: button_right
                    visible: image_container.width < image_container.contentWidth
                    anchors { right: image_container.right; rightMargin: 10; verticalCenter: image_container.verticalCenter }
                    Image {
                        source: "./image/btn_right.png"
                        opacity: button_right.containsMouse ? 1 : 0.8
                    }
                    onClicked: {
                        if (image_container.contentX < image_container.contentWidth - image_container.width - 100) {
                            image_container.contentX += 100;
                        }
                        else {
                            image_container.contentX = image_container.contentWidth - image_container.width;
                        }
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
        Button {
            id: button_ok
            width: 165; height: 44
            anchors.centerIn: parent
            Rectangle {
                radius: 3; smooth: true
                border { width: 1; color: "#4a880b" }
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop { position: 0; color: button_ok.containsMouse ? "#78C221" : "#78c221" }
                    GradientStop { position: 1; color: button_ok.containsMouse ? "#78C221" : "#5DAA1D" }
                }
                Text { smooth: true
                    anchors.centerIn: parent
                    text: qsTr("Return"); color: "white"
                    font { pixelSize: 20 }
                }
            }
            onClicked: { messageprovider.source = "" }
        }
    }
}
