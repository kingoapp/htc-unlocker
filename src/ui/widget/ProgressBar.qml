import QtQuick 1.1

Row {
    id: progressbar
    spacing: 5
    width: 100
    height: 20

    property real progress: 0
    property color color_border: "#99CDF1"
    property color color_background: "white"
    property color color_block: "#99CDF1"
    property color color_text: "black"
    property bool enableAnimation: true
    property int animationTime: 300

    Rectangle {
        width: parent.width; height: parent.height; color: progressbar.color_background
        border { width: 1; color: progressbar.color_border }
        Rectangle {
            height: parent.height
            width: parent.width * progressbar.progress
            color: progressbar.color_block
            Behavior on width { enabled: progressbar.enableAnimation
                NumberAnimation { duration: animationTime/*; easing.type: Easing.OutQuart*/ } }
        }
    }
}
