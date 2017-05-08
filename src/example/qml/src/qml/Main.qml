import QtQuick 2.0
import QtQuick.Window 2.2

Window {


    title: qsTr("GoToC")

    width: 640
    height: 480

    visible: true

    color: "#92BD6C"

    Row {
        id: tools

        anchors.left: parent.left
        anchors.right: parent.right
        height: 50

        Rectangle {
            width: 100
            height: 50

            Text {
                anchors.centerIn: parent
                text: "Null"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    loader.sourceComponent = null
                }
            }
        }

        Rectangle {
            width: 100
            height: 50

            Text {
                anchors.centerIn: parent
                text: "Login"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    loader.sourceComponent = loginComp
                }
            }
        }

        Rectangle {
            width: 100
            height: 50

            Text {
                anchors.centerIn: parent
                text: "Points"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    loader.sourceComponent = pointsComp
                }
            }
        }
    }


    Text {
        anchors.centerIn: parent
        text: "QGoToC"
    }

    Loader {
        id: loader
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: tools.bottom
        anchors.bottom: parent.bottom

    }

    Component {
        id: loginComp

        Login {
            anchors.fill: parent
        }
    }


    Component {
        id: pointsComp

        Points {
            anchors.fill: parent
        }
    }
}
