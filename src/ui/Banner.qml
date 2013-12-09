import QtQuick 1.1

Rectangle {
	id: banner
	width: parent.width; height: 270
	y: 10; clip: true
	color: "#88000000"
	property alias model: listview_image.model
	property alias index: listview_image.currentIndex

	Timer {
		id: timer
        interval: 10000; running: true; repeat: true
        onTriggered: {
        	if (listview_image.currentIndex < listview_image.model.length - 1) {
        		listview_image.currentIndex++;
        	}
        	else {
        		listview_image.currentIndex = 0;
        	}
        }
    }

	Item {
		id: listview_image
		width: parent.width; height: parent.height
		property int currentIndex: 0
		property variant model: []
		property int currentRotation: 0
		anchors.centerIn: parent
		clip: true
		Repeater {
			model: listview_image.model
			delegate: Item {
				width: listview_image.width; height: listview_image.height
				opacity: listview_image.currentIndex == index
				Behavior on opacity { NumberAnimation { duration: 600 } }
		        Image {
		            id: image
					anchors.centerIn: parent; fillMode: Image.PreserveAspectFit
					smooth: true
					source: modelData.img
					onStatusChanged: {
						if (image.status == Image.Ready) {
							banner.visible = true;
						}
					}
					MouseArea {
						enabled: typeof(modelData.url) != "undefined"
						anchors.fill: parent
						onClicked: window.openUrl(modelData.url)
					}
		        }
			}
		}
	}
    Row {
        anchors { horizontalCenter: parent.horizontalCenter; bottom: parent.bottom; bottomMargin: 20 }
        spacing: 5
        visible: listview_image.model.length > 1
        Repeater {
            model: listview_image.model.length
            delegate: Rectangle {
                width: 12; height: 12; radius: width / 2; smooth: true
                color: index == listview_image.currentIndex ? "#66ffffff" : "#66000000"
                Behavior on color { ColorAnimation { duration: 400 } }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: { 
                    	listview_image.currentIndex = index;
                    	timer.restart();
                    }
                }
            }
        }
    }
}