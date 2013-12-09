import QtQuick 1.1

Rectangle {
	id: connectedview
	width: parent.width; height: parent.height
	color: "#FAFAFA"

	signal unlock()
	signal lock()

	Item {
		width: parent.width; height: parent.height - userarea.height
		Column {
			anchors.centerIn: parent
			spacing: 20
			Text {
				text: qsTr("Notifications")
				font { pixelSize: 20 }
			}
			Text {
				text: qsTr("Unlock your Bootloader will\n1. void your warranty claim\n2. wipe off user data")
				font { pixelSize: 14 }
				lineHeight: 1.6
			}
			Text {
				text: qsTr("After unlocking your Bootloader, you can\n1. flash custom ROM\n2. root your device")
				font { pixelSize: 14 }
				lineHeight: 1.6
			}
		}
	}
	Rectangle {
		id: userarea
		width: parent.width; height: 100
		anchors.bottom: parent.bottom
		color: "#f2f2f2"
		MouseArea { anchors.fill: parent; hoverEnabled: true }
		Row {
			anchors.centerIn: parent
			spacing: 50
			Button {
				id: button_unlock
				Rectangle {
					width: 150; height: 50
					radius: 3; smooth: true
					gradient: Gradient {
				        GradientStop { position: 0.0; color: button_unlock.containsMouse ? "#5AA822" : "#77C229" }
				        GradientStop { position: 1.0; color: button_unlock.containsMouse ? "#77C229" : "#5AA822" }
				    }
				    border { width: 1; color: "#538A0D" }
					color: button_unlock.containsMouse ? "#33CC33" : "#4D9900"
					Text {
						anchors.centerIn: parent
						text: qsTr("Unlock")
						color: "white"
					}
				}
				onClicked: connectedview.unlock()
			}
			Button {
				id: button_lock
				Rectangle {
					width: 150; height: 50
					radius: 3; smooth: true
					gradient: Gradient {
				        GradientStop { position: 0.0; color: button_lock.containsMouse ? "#F05013" : "#F36508" }
				        GradientStop { position: 1.0; color: button_lock.containsMouse ? "#F36508" : "#F05013" }
				    }
				    border { width: 1; color: "#B64B0B" }
					Text {
						anchors.centerIn: parent
						text: qsTr("Relock")
						color: "white"
					}
				}
				onClicked: connectedview.lock()
			}
		}
	}
}