import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import Qt.labs.settings 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("qml live application")

    function reload(url) {
        loader.source = ""
        LiveApp.clearCache()
        loader.source = url
    }

    Item {
        id: contentItem
        anchors.fill: parent

        Loader {
            id: loader
            anchors.fill: parent
        }

        Label {
            id: warningLabel
            anchors {
                left: parent.left
                right: parent.right
                verticalCenter: parent.verticalCenter
            }
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
            visible: loader.status === Loader.Error
                     || !loader.sourceComponent
            text: loader.status === Loader.Error ? qsTr("QML Error: Loading QML file failed:\n") + loader.sourceComponent.errorString()
                                                 : qsTr("Please choose a qml file to work with")
        }
    }

    Rectangle {
        id: toolBar
        anchors {
            left: parent.left
            right: parent.right
        }
        height: 30
        color: "#E5E3E8"
        y: mouseArea.containsMouse || fileDialog.visible ? 0 : -height + 5
        opacity: 0.85
        Row {
            id: buttons
            anchors { left: parent.left; verticalCenter: parent.verticalCenter }
            spacing: 3
            Button {
                height: 20
                text: qsTr("Change qml")
                onClicked: fileDialog.open()
            }
            Button {
                height: 20
                width: 50
                text: qsTr("Reload")
                onClicked: window.reload(LiveApp.rootQmlFileUrl())
            }
            Button {
                height: 20
                width: 80
                text: qsTr("Restart app")
                onClicked: LiveApp.restartApplication()
            }
        }

        CheckBox {
            id: onTopCheckBox
            anchors { right: parent.right; verticalCenter: parent.verticalCenter }
            text: qsTr("On top")
            height: 20
            onClicked: {
                if (checked) {
                    window.flags = window.flags | Qt.WindowStaysOnTopHint
                } else {
                    window.flags = window.flags & ~Qt.WindowStaysOnTopHint
                }
            }
        }
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            onPressed: mouse.accepted = false
        }

        Behavior on y {
            NumberAnimation {
                duration: 200
            }
        }
    }

    FileDialog {
        id: fileDialog
        selectMultiple: false
        title: qsTr("Choose qml file...")
        onAccepted: LiveApp.setRootQmlFilePath(fileUrl)
        nameFilters: ["*.qml"]
    }

    Settings {
        id: windowSettings
        category: "window"
        property alias width: window.width
        property alias height: window.height
        property alias x: window.x
        property alias y: window.y
        property alias visibility: window.visibility
        property alias flags: window.flags
        property string screen: ""
    }

    Component.onCompleted: {
        onTopCheckBox.checked = window.flags & Qt.WindowStaysOnTopHint
        for (var i = 0; i < Qt.application.screens.length; ++i) {
            let screen = Qt.application.screens[i]
            if (screen.serialNumber === windowSettings.screen) {
                window.screen = screen
                return
            }
        }
    }
    Component.onDestruction: {
        windowSettings.screen = window.screen.serialNumber
    }
}
