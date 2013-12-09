import QtQuick 1.1

Rectangle {
	width: parent.width; height: parent.height
	color: "#FAFAFA"

	Column {
		anchors.centerIn: parent
		spacing: 40
		Row {
			anchors { horizontalCenter: parent.horizontalCenter }
			spacing: 20
			Image {
				source: "./image/logo.png"
				width: 60; height: 60; smooth: true
				fillMode: Image.PreserveAspectFit
				anchors.verticalCenter: parent.verticalCenter
			}
			Column {
				anchors.verticalCenter: parent.verticalCenter
				spacing: 8
				Text {
					text: qsTr("HTC Bootloader Unlocker")
					font { pixelSize: 18; bold: true }
				}
				Text {
					text: qsTr("Kingo Product")
					font { pixelSize: 14 }
				}
			}
		}
		Row {
			anchors { horizontalCenter: parent.horizontalCenter }
			spacing: 30
			Column {
				id: column_msg
				anchors.verticalCenter: parent.verticalCenter
				// width: 300
				spacing: 10
				Text {
					text: qsTr("Device Not Detected，Please connect your device")
					font { pixelSize: 14 }
				}
				Text {
					text: qsTr("1. Connect your device to computer via USB\n2. Enable USB Debugging under Settings of your device")
					font { pixelSize: 12 }
					lineHeight: 1.6
				}
				Text {
					text: qsTr("How to enable USB Debugging?")
					font { pixelSize: 12 }
				}
			}
			Image {
				source: "./image/useline.png"
			}
			Column {
				anchors.verticalCenter: parent.verticalCenter
				// width: 300
				height: column_msg.height
				spacing: 10
				Text {
					text: qsTr("Cannot connect to HTC Bootloader Unlocker")
					font { pixelSize: 12 }
				}
			}
		}
	}
}
