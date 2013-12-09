import QtQuick 1.1
import Components 1.0

Text {
    id: link
    property string url: ""
    property bool enableDesktopService: true
    property alias mouse: mousearea
    font.pixelSize: 14

    signal clicked(string url)

    MouseArea {
        id: mousearea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            if (enableDesktopService && url.length > 0) Util.openUrl(url)
            link.clicked(url)
        }
    }
}
