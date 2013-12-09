import QtQuick 1.1

Item {
	width: parent.width; height: 50

	Row {
		anchors { verticalCenter: parent.verticalCenter; left: parent.left; leftMargin: 5 }
		spacing: 8
		Image {
			id: image_status
			anchors.verticalCenter: parent.verticalCenter
			source: "./image/icon-unconnected.png"
		}
		Text {
			id: text_status
			anchors.verticalCenter: parent.verticalCenter
			text: qsTr("No device")
		}
	}
	Text {
		anchors { verticalCenter: parent.verticalCenter; right: parent.right; rightMargin: 8 }
		text: qsTr("Version: ") + qApp.applicationVersion
	}

	Rectangle {
		width: parent.width; height: 1; color: "#D4D4D4"
		anchors.top: parent.top
		Rectangle {
			width: parent.width; height: 1; color: "#FFFFFF"
			anchors.top: parent.bottom
		}
	}
	Connections {
		target: deviceChecker
		onDeviceStateChanged: {
			switch (state) {
				case 0: { // disconnect 
					image_status.source = "./image/icon-unconnected.png";
					text_status.text = qsTr("No device");
				} break;
				case 1: { // connected
					image_status.source = "./image/icon-connected.png";
					text_status.text = qsTr("Device connected");
				} break;
			}
		}
	}
}