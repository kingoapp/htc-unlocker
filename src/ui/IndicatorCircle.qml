import QtQuick 1.1

Item {
    id: indicatorcircle
    property int radius: 10
    property int count: 9

    property Component indicator: Rectangle {
        width: 4; height: 4
        color: "#222222"; smooth: true
    }
    width: radius * 2; height: width

    Repeater {
        id: repeater
        model: indicatorcircle.count
        delegate: Item {
            id: indicatoritem
            anchors.fill: parent
            visible: false
            transform: Rotation { id: item_rotation; origin.x: indicatoritem.width / 2; origin.y: indicatoritem.height / 2 }
            Item {
                anchors.fill: parent
                transform: Rotation { id: inneritem_rotation; origin.x: indicatoritem.width / 2; origin.y: indicatoritem.height / 2 }
                Loader {
                    id: indicatorContainer
                    property int itemIndex: index
                    anchors { left: parent.right; verticalCenter: parent.verticalCenter }
                    sourceComponent: indicatorcircle.indicator
                }
            }
            SequentialAnimation {
                running: true
                PauseAnimation { duration: index * 360 * 1.75 / repeater.count}
                ScriptAction { script: { indicatoritem.visible = true } }
                ParallelAnimation {
                    SequentialAnimation {
                        loops: -1
                        NumberAnimation { target: item_rotation; property: "angle"
                            from: 0
                            to: 360
                            duration: 2500; easing.type: Easing.OutInQuart
                        }
                    }
                    SequentialAnimation {
                        loops: -1
                        NumberAnimation { target: inneritem_rotation; property: "angle"; from: 0; to: 360; duration: 1750; easing.type: Easing.Linear }
                    }
                }
            }
        }
    }
}
