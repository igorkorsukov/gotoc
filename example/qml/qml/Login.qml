import QtQuick 2.0
import "GoToC"

Rectangle {

    id: blank

    color: "#A555FC"


    GoForm {
        id: form
        key: 201

        property var login: form.value("login")
        property var pass: form.value("pass")

        onNotify: {
            switch (name) {
            case "sign_success": signResult.success(); break;
            case "sign_failed": signResult.failed(); break;
            case "sign_reset": signResult.reset(); break;
            }
        }
    }


    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

        Text {
            id: caption
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
            verticalAlignment: Text.AlignVCenter
            text: form.value("title")
        }

        Item {
            id: login

            anchors.left: parent.left
            anchors.right: parent.right
            height: 50

            Text {
                id: loginLabel
                anchors.left: parent.left
                anchors.right: parent.right
                height: 20
                text: form.login.title
            }

            TextEdit {
                anchors.top: loginLabel.bottom
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                text: form.login.value
                onTextChanged: {
                    form.changed("login", text)
                }
            }

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                height: 2
                color: "#4C4C4C"
            }
        }

        Item {
            id: pass

            anchors.left: parent.left
            anchors.right: parent.right
            height: 50

            Text {
                id: passLabel
                anchors.left: parent.left
                anchors.right: parent.right
                height: 20
                text: form.pass.title
            }

            TextEdit {
                anchors.top: passLabel.bottom
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                text: form.pass.value
                onTextChanged: {
                    form.changed("pass", text)
                }
            }

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                height: 2
                color: "#4C4C4C"
            }
        }

        Rectangle {
            id: sign
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
            color: "#A40903"

            Text {
                anchors.centerIn: parent
                text: "Sign"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    form.clicked("sign")
                }
            }
        }

        Rectangle {
            id: signResult
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
            color: blank.color

            Text {
                id: signResText
                anchors.centerIn: parent
            }

            function success() {
                signResult.color = "#19A15F"
                signResText.text = "Success"
            }

            function failed() {
                signResult.color = "#DE5145"
                signResText.text = "Failed"
            }

            function reset() {
                signResult.color = blank.color
                signResText.text = ""
            }
        }
    }
}
