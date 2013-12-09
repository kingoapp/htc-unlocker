import QtQuick 1.1

MouseArea {
	width: 824; height: 554
    property variant clickPos: "1,1" 
    onPressed: clickPos = Qt.point(mouse.x,mouse.y) 
    onPositionChanged: { 
        var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
        window.pos = Qt.point(window.pos.x + delta.x, window.pos.y + delta.y)
    } 

    BorderImage {
	    id: mainwindow
	    anchors.fill: parent
	    focus: true
        source: window.isMac() ? null : "./image/shadow.png";
	    smooth: true
        border { left: 12; top: 12; right: 12; bottom: 12 }
	    Rectangle {
	    	color: "#f9f9f9"
	    	anchors { fill: parent; margins: 12 }
	    	radius: 6; smooth: true
		    Column {
		    	anchors.fill: parent
				TitleBar { id: titlebar }
				Banner {
					id: banner
					model: [
						{"img": "./image/banner1.png"}
					]
					function getAds (argument) {
				    	var request = new XMLHttpRequest();
				        request.onreadystatechange = function() {
				            if (request.readyState == XMLHttpRequest.DONE) {
				                console.log("-----------------------------------");
				                console.log(request.responseText);
				                console.log("-----------------------------------");
				                if (request.responseText.length > 0) {
				                	var tmpModel = [];
				                	eval("tmpModel =" + request.responseText);
				                	if (tmpModel.length > 0)
				                		banner.model = tmpModel;
				                }
				            }
				        }

				        request.open("GET","https://unlocker.kingoapp.com/ads", true);
				        console.log("get ads list");
				    	request.send();
					}
					function reset () {
						banner.model = [
							{"img": "./image/banner1.png"}
						];
					}
					Component.onCompleted: {
						banner.getAds();
					}
					// Banner {
					// 	id: banner_ads
					// 	visible: false
					// }
				}
				Loader {
					id: loader
					width: parent.width; height: 220
					source: "DeviceConnectState.qml"
				}
		    }
	        Loader { id: messageprovider
	            anchors { fill: parent; topMargin: titlebar.height }
	        }
		}
	}
}
