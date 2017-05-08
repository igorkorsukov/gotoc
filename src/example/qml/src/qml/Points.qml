import QtQuick 2.0
import "GoToC"

Rectangle {

    id: blank

    color: "#FFCE44"


    GoForm {
        id: form

        key: 202
    }

    ListView {
        id: view
        anchors.fill: parent
        clip: true

        model: form.model
        delegate: Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 10
            height: 35
            color: blank.color

            Text {
                id: titleLabel
                anchors.left: parent.left
                anchors.right: parent.right
                height: 15
                text: item.title
            }

            Text {
                id: subtitleLabel
                anchors.top: titleLabel.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: 15
                text: item.subtitle
            }

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                height: 2
                color: "#4C4C4C"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: form.clicked("item", model.index)
            }
        }
    }
}
