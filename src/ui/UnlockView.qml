import QtQuick 1.1
import "widget"

Rectangle {
	id: unlockview
    width: 800; height: 220
	color: "#f9f9f9"
	property variant deviceInfo: {}
	property int getTokenBinTryCount: 20
	property string token

	function setStep (step, state) {
		if (loader.state == "unlock")
            progressbar.progress = (step - 1) / 3;
		else {
            progressbar.progress = step / 2;
		}
		var textHandel;
		var progressStep;
		eval("textHandel = text_step_" + step);
		eval("progressStep = progress_step_" + step);
		
		if (state == "success") {
			textHandel.color = "#5CB210";
		}
		else if (state == "failed") {
			textHandel.color = "tomato";
			progressStep.color = "tomato";
			text_progress_message.text = qsTr("Failed!");
			button_finish.enabled = true;
		}
	}

	function getTokenBin () {
    	var request = new XMLHttpRequest();
        request.onreadystatechange = function() {
            if (request.readyState == XMLHttpRequest.DONE) {
                console.log("-----------------------------------");
                console.log(request.responseText);
                console.log("-----------------------------------");
                var info = {};
                if (request.responseText.length > 0)
                	eval("info=" + request.responseText);
                if (info.unlockbin != null && info.unlockbin.length > 0) {
                	setStep(3, "success");
                	setStep(4, "start");
                	var unlockBinFile = util.storageLocation(7) + "/Unlock_code.bin";
                	util.saveBufferFromBase64(unlockBinFile, info.unlockbin);
                	if (fastboot.flash("unlocktoken", unlockBinFile)) {
                		setStep(4, "success");
                		text_progress_message.text = qsTr("Successed!");
                	}
                	else {
                		setStep(4, "failed");
                	}
                	util.removeFile(unlockBinFile);
                	deviceChecker.enableCheckDevice(true);
					button_finish.enabled = true;
                }
                else {
                    progressbar.progress += 1/60;
                	unlockview.getTokenBinTryCount--;
                	if (unlockview.getTokenBinTryCount > 0) {
	            		timer.start();
	            	}
	            	else {
                		setStep(3, "failed");
						deviceChecker.enableCheckDevice(true);
						button_finish.enabled = true;
                	}
                }
            }
        }

        request.open("POST","https://unlocker.kingoapp.com/api/htcunlock", true);
        console.log("https request htc unlock bin");
    	request.send(
    		"imei=" + unlockview.deviceInfo.imei 
    		+ "&sn=" + unlockview.deviceInfo.serialno
    		+ "&token=" + unlockview.token
    		+ "&key=" + "debug_key"
    		);
	}

	function unlock() {
		deviceChecker.enableCheckDevice(false);
		setStep(1, "start");
		if (adb.reboot("bootloader")) {
			setStep(1, "success");
			setStep(2, "start");
			deviceChecker.waitForDevice();
        	unlockview.deviceInfo = fastboot.getAllVar();
			unlockview.token = fastboot.htcGetIdentifierToken();
			if (unlockview.token.length > 0) {
				setStep(2, "success");
				setStep(3, "start");
				getTokenBin();
			}
			else {
				setStep(2, "failed");
			}
		}
		else {
			setStep(1, "failed");
		}
	}

	function relock () {
		deviceChecker.enableCheckDevice(false);
		if (adb.reboot("bootloader")) {
			deviceChecker.waitForDevice();
			setStep(1, "success");
			if (fastboot.relock()) {
				setStep(2, "success");
				text_progress_message.text = qsTr("Successed!");
			}
			else {
				setStep(2, "failed");
			}
		}
		else {
			setStep(1, "failed");
		}
		deviceChecker.enableCheckDevice(true);
		button_finish.enabled = true;
	}

	Timer {
		id: timer
		interval: 10000
		onTriggered: {
			getTokenBin();
		}
	}
	Item {
		width: parent.width; height: 135
		Column {
	        anchors.centerIn: parent
            anchors.verticalCenterOffset: 15
	        spacing: 20
	        Text { smooth: true
	            id: text_progress_message
	            width: parent.width
	            font.pixelSize: 18
	            horizontalAlignment: Text.AlignHCenter
                text: qsTr("Don't disconnect the device and computer.")
	            elide: Text.ElideRight
	            onLinkActivated: window.openUrl(link)
	        }
            Item {
                width: 650; height: 60
                ProgressBar {
                    id: progressbar
                    width: 650; height: 5
                    color_background: "#DFDFDF"
                    color_border: "#f9f9f9"
                    color_block: "#5CB210"
                    progress: 0
                }
                // step 1
	            Text { 
	            	id: text_step_1
	            	text: qsTr("Fastboot Mode")
                    color: progressbar.progress >= 0 ? "#5CB210" : "dimgray"
	                font.pixelSize: 14; smooth: true
                    x: -width / 2
                    anchors { top: progressbar.bottom; topMargin: 10 }
	            }
                Rectangle {
                	id: progress_step_1
                    x: -width / 2
                    width: 13; height: width; radius: width / 2; z: 1
                    color: progressbar.progress >= 0 ? "#5CB210" : "#DFDFDF"
                    anchors { verticalCenter: progressbar.verticalCenter}
                }
                // step 2
	            Text { 
	            	id: text_step_2
	            	text: loader.state == "unlock" ? qsTr("Fetching Unlock Token") : qsTr("Locking Again")
                    color: loader.state == "unlock" ? (progressbar.progress >= 1/3 ? "#5CB210" : "dimgray") : (progressbar.progress >= 1 ? "#5CB210" : "dimgray")
	                font.pixelSize: 14; smooth: true
                    x: (loader.state == "unlock" ? progressbar.width / 3 : progressbar.width) - width / 2
                    anchors { top: progressbar.bottom; topMargin: 10 }
	            }
                Rectangle {
                	id: progress_step_2
                    x: (loader.state == "unlock" ? progressbar.width / 3 : progressbar.width) - width / 2
                    width: 13; height: width; radius: width / 2; z: 1
                    color: loader.state == "unlock" ? (progressbar.progress >= 1/3 ? "#5CB210" : "#DFDFDF") : (progressbar.progress >= 1 ? "#5CB210" : "#DFDFDF")
                    anchors { verticalCenter: progressbar.verticalCenter}
                }
                // step 3
	            Text { 
	            	id: text_step_3
                    text: qsTr("Fetching Unlock Bin"); color: progressbar.progress >= 2/3 ? "#5CB210" : "dimgray"
	                font.pixelSize: 14; smooth: true
                    x: progressbar.width * 2/3 - width / 2
	                visible: loader.state == "unlock"
                    anchors { top: progressbar.bottom; topMargin: 10 }
	            }
                Rectangle {
                	id: progress_step_3
                    x: progressbar.width * 2/3 - width / 2
                    width: 13; height: width; radius: width / 2; z: 1; color: progressbar.progress >= 2/3 ? "#5CB210" : "#DFDFDF"
                    anchors { verticalCenter: progressbar.verticalCenter}
	                visible: loader.state == "unlock"
                }
                // step 4
	            Text { 
	            	id: text_step_4
                    text: qsTr("Unlock"); color: progressbar.progress == 100 ? "#5CB210" : "dimgray"
	                font.pixelSize: 14; smooth: true
                    x: progressbar.width - width / 2
	                visible: loader.state == "unlock"
                    anchors { top: progressbar.bottom; topMargin: 10 }
	            }
                Rectangle {
                	id: progress_step_4
                	x: progressbar.width - width / 2
                    width: 13; height: width; radius: width / 2; z: 1; color: progressbar.progress == 1 ? "#5CB210" : "#DFDFDF"
                    anchors { verticalCenter: progressbar.verticalCenter}
	                visible: loader.state == "unlock"
                }
            }
        }
	}

	Rectangle {
		id: userarea
		width: parent.width; height: 80
		anchors.bottom: parent.bottom
		color: "#f9f9f9"
        Rectangle {
            width: parent.width - 30; height: 1
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#d2d2d2"
        }
		MouseArea { anchors.fill: parent; hoverEnabled: true }
		Row {
			anchors.centerIn: parent
			spacing: 20
			Button {
				id: button_finish
				enabled: false
				Rectangle {
					width: 165; height: 44
					radius: 3; smooth: true
					gradient: Gradient {
                        GradientStop { position: 0; color: button_finish.enabled ? button_finish.containsMouse ? "#78C221" : "#78c221" : "#e7e7e7" }
                        GradientStop { position: 1; color: button_finish.enabled ? button_finish.containsMouse ? "#78C221" : "#5DAA1D" : "#d7d7d7" }
				    }
					border { width: 1; color: button_finish.enabled ? "#538A0D" : "#BDBDBD" }
					Text {
						anchors.centerIn: parent
						text: qsTr("Finish")
						color: button_finish.enabled ? "white" : "#a7a7a7"
                        font { pixelSize: 20 }
					}
				}
				onClicked: {
					loader.source = "DeviceConnectState.qml";
					// banner.reset();
				}
			}
		}
	}

	Component.onCompleted: {
		// banner.getAds();
		if (loader.state == "unlock") {
			unlock();
		}
		else {
			relock();
		}
	}
}
