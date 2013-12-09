import QtQuick 1.1

Row {
	z: 100
	anchors { right: parent.right; verticalCenter: parent.verticalCenter }
	Button {
		id: button_min
		width: 30; height: 30
		Image {
			source: button_min.containsMouse ? "./image/min1.png" : "./image/min.png"
		}
		onClicked: window.showMinimized();
	}
	Button {
		id: button_close
		width: 30; height: 30
		Image {
			source: button_close.containsMouse ? "./image/close1.png" : "./image/close.png"
		}
		onClicked: window.close();
	}
}