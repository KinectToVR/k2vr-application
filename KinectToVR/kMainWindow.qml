import QtQuick 2.15
import QtQuick.Templates 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.11
import QtGraphicalEffects 1.15

Item {
    id: main
    width: 3915
    height: 2292
    clip: false
    antialiasing: true
    signal qmlSignal(string msg)

    property var primaryColor: "#121212"
    property var secondaryColor: "#323232"
    property var tertiaryColor: "#444444"

    property var gradientStartColor: "#7826CD"
    property var gradientEndColor: "#A6369B"
    property var secondaryButtonColor: "#2A035D"

    property var textColor: "#9E9E9E"


    /*transform: Scale {
        origin.x: 0; origin.y: 0; xScale: parent.width / 3915; yScale: parent.height / 2292
    }*/
    Item {
        id: buttoncontrols
        width: 3915
        height: 216
        z: 1000

        Image {
            id: image1
            objectName: "k2label"
            x: 32
            y: 32
            width: 192
            height: 138
            anchors.verticalCenterOffset: 10
            anchors.verticalCenter: generalButton.verticalCenter
            fillMode: Image.PreserveAspectFit
            source: "k2vr-logo.png"
        }

        Rectangle {
            id: gradientRect
            width: 10
            height: 10
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop {
                    position: 0
                    color: gradientStartColor
                }
                GradientStop {
                    position: 1
                    color: gradientEndColor
                }
            }
            visible: false // should not be visible on screen.
            layer.enabled: true
            layer.smooth: true
        }

        Button {
            property var foreground: gradientStartColor
            id: generalButton
            objectName: "generalButton"
            x: 296
            y: 0
            width: 255
            height: 208
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: rectangle.bottom

            property bool layer_en: true

            Text {
                x: 5
                width: parent.width - 10
                height: parent.height
                text: "General"
                fontSizeMode: Text.FixedSize
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.Bold
                font.bold: true
                font.pointSize: 48
                font.family: "JostSemi"

                color: "#ffffff"
                layer.enabled: generalButton.layer_en
                layer.samplerName: "maskSource"
                layer.effect: ShaderEffect {
                    property var colorSource: gradientRect
                    fragmentShader: "
uniform lowp sampler2D colorSource;
uniform lowp sampler2D maskSource;
uniform lowp float qt_Opacity;
varying highp vec2 qt_TexCoord0;
void main() {
gl_FragColor =
texture2D(colorSource, qt_TexCoord0)
* texture2D(maskSource, qt_TexCoord0).a
* qt_Opacity;
}
"
                }
            }

            highlighted: false
            flat: true
            onClicked: function () {
                qmlSignal(qsTr("GENERAL"))
                pageIndicator.x = generalButton.x
                pageIndicator.width = generalButton.width

                generalButton.layer_en = true
                controllersButton.layer_en = false
                devicesButton.layer_en = false
                configurationButton.layer_en = false

                // Show general tab
                generalControlTab.visible = true
            }
        }

        Button {
            property var foreground: "#FFFFFF"
            id: controllersButton
            objectName: "controllersButton"
            x: 702
            y: 0
            width: 659
            height: 216
            anchors.top: parent.top
            anchors.bottom: rectangle1.bottom
            property bool layer_en: false

            Text {
                x: 5
                width: parent.width - 10
                height: parent.height
                text: "Headset / Controllers"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.Bold
                font.bold: true
                font.pointSize: 48
                font.family: "JostSemi"

                color: "#ffffff"
                layer.enabled: controllersButton.layer_en
                layer.samplerName: "maskSource"
                layer.effect: ShaderEffect {
                    property var colorSource: gradientRect
                    fragmentShader: "
uniform lowp sampler2D colorSource;
uniform lowp sampler2D maskSource;
uniform lowp float qt_Opacity;
varying highp vec2 qt_TexCoord0;
void main() {
gl_FragColor =
texture2D(colorSource, qt_TexCoord0)
* texture2D(maskSource, qt_TexCoord0).a
* qt_Opacity;
}
"
                }
            }
            highlighted: false
            flat: true
            onClicked: function () {
                qmlSignal(qsTr("CONTROLLERS"))
                pageIndicator.x = controllersButton.x
                pageIndicator.width = controllersButton.width

                generalButton.layer_en = false
                controllersButton.layer_en = true
                devicesButton.layer_en = false
                configurationButton.layer_en = false

                // Show controllers tab
            }
        }

        Button {
            property var foreground: "#FFFFFF"
            id: devicesButton
            objectName: "devicesButton"
            x: 1502
            y: 0
            width: 259
            height: 216
            font.pointSize: 51
            anchors.top: parent.top
            anchors.bottom: rectangle1.bottom
            property bool layer_en: false

            Text {
                x: 5
                width: parent.width - 10
                height: parent.height
                text: "Devices"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.Bold
                font.bold: true
                font.pointSize: 48
                font.family: "JostSemi"

                color: "#ffffff"
                layer.enabled: devicesButton.layer_en
                layer.samplerName: "maskSource"
                layer.effect: ShaderEffect {
                    property var colorSource: gradientRect
                    fragmentShader: "
uniform lowp sampler2D colorSource;
uniform lowp sampler2D maskSource;
uniform lowp float qt_Opacity;
varying highp vec2 qt_TexCoord0;
void main() {
gl_FragColor =
texture2D(colorSource, qt_TexCoord0)
* texture2D(maskSource, qt_TexCoord0).a
* qt_Opacity;
}
"
                }
            }
            highlighted: false
            flat: true
            onClicked: function () {
                qmlSignal(qsTr("DEVICES"))
                pageIndicator.x = devicesButton.x
                pageIndicator.width = devicesButton.width

                generalButton.layer_en = false
                controllersButton.layer_en = false
                devicesButton.layer_en = true
                configurationButton.layer_en = false

                // Show devices tab
            }
        }

        Button {
            property var foreground: "#FFFFFF"
            id: configurationButton
            objectName: "configurationButton"
            x: 1909
            y: 0
            width: 434
            height: 216
            anchors.top: parent.top
            anchors.bottom: rectangle1.bottom
            property bool layer_en: false

            Text {
                x: 5
                width: parent.width - 10
                height: parent.height
                text: "Configuration"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.weight: Font.Bold
                font.bold: true
                font.pointSize: 48
                font.family: "JostSemi"

                color: "#ffffff"
                layer.enabled: configurationButton.layer_en
                layer.samplerName: "maskSource"
                layer.effect: ShaderEffect {
                    property var colorSource: gradientRect
                    fragmentShader: "
uniform lowp sampler2D colorSource;
uniform lowp sampler2D maskSource;
uniform lowp float qt_Opacity;
varying highp vec2 qt_TexCoord0;
void main() {
gl_FragColor =
texture2D(colorSource, qt_TexCoord0)
* texture2D(maskSource, qt_TexCoord0).a
* qt_Opacity;
}
"
                }
            }
            highlighted: false
            flat: true
            onClicked: function () {
                qmlSignal(qsTr("CONFIGURATION"))
                pageIndicator.x = configurationButton.x
                pageIndicator.width = configurationButton.width

                generalButton.layer_en = false
                controllersButton.layer_en = false
                devicesButton.layer_en = false
                configurationButton.layer_en = true

                // Show configuration tab
            }
        }

        Rectangle {
            id: rectangle
            x: 8
            y: 0
            width: 200
            height: 62
            color: secondaryColor
            visible: true
            anchors.rightMargin: 0
            z: -1
            anchors.right: parent.right
            anchors.topMargin: 154
            anchors.top: parent.top
            anchors.leftMargin: 0
            anchors.left: parent.left
        }

        Label {
            id: label
            x: 3327
            y: 78
            width: 526
            height: 60
            color: "#8cffffff"
            text: qsTr("KinectToVR v1.0.0")
            horizontalAlignment: Text.AlignRight
            font.bold: true
            font.pointSize: 36
        }

        Rectangle {
            id: pageIndicator
            objectName: "pageIndicator"
            x: generalButton.x
            y: 201
            width: generalButton.width
            height: 15
            color: gradientStartColor
            anchors.leftMargin: 0
            anchors.bottom: rectangle.bottom

            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop {
                    position: 0.0
                    color: gradientStartColor
                }
                GradientStop {
                    position: 1.0
                    color: gradientEndColor
                }
            }
        }

        Rectangle {
            id: rectangle1
            x: 0
            y: 0
            width: 200
            height: 216
            color: secondaryColor
            anchors.fill: parent
            visible: true
            z: -1
            radius: 60
        }
    }

    //---------------------------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------------------------


    /*
        GENERAL CONTROL TAB BEGINS HERE
    */
    //---------------------------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------------------------
    Item {
        id: controlTabs
        x: 0
        width: 3916
        height: 2068
        anchors.topMargin: 0
        anchors.bottomMargin: 8
        anchors.leftMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        clip: false
        antialiasing: true
        y: 216

        Item {
            id: generalControlTab

            Canvas {
                width: 1979
                height: 1368
                anchors.bottom: parent.bottom
                anchors.rightMargin: 0
                anchors.right: parent.right
                anchors.leftMargin: 1606
                anchors.topMargin: 0
                anchors.top: parent.top
                anchors.left: parent.left
            }

            StackView {
                id: pages1
                x: 0
                y: 0
                z: -10
                width: 3915
                height: 2068
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 0

                Image {
                    id: image
                    x: 0
                    y: -215
                    width: 3915
                    height: 2282
                    visible: false
                    anchors.leftMargin: 0
                    anchors.left: parent.left
                    anchors.right: parent.right
                    fillMode: Image.Stretch
                    source: "../main.png"
                }
            }

            Label {
                id: label1
                x: 50
                y: 42
                width: 294
                height: 78
                color: textColor
                text: qsTr("Trackers:")
                font.bold: true
                font.pointSize: 48
                font.family: "JostSemi"
            }

            Button {
                id: calibrationButton
                objectName: "caibrationButton"
                x: 50
                y: 141
                width: 776
                height: 200
                hoverEnabled: true

                Text {
                    text: qsTr("Begin Calibration")
                    font.bold: true
                    rightPadding: 0
                    bottomPadding: 0
                    leftPadding: 0
                    topPadding: 0
                    anchors.rightMargin: 0
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 0
                    anchors.topMargin: 0
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "White"
                    font.pointSize: 48
                    font.family: "JostSemi"
                    anchors.centerIn: parent
                }
                highlighted: false
                flat: true

                Rectangle {
                    visible: parent.hovered
                    anchors.fill: parent
                    radius: 20
                    color: "#43fdfdfd"
                }

                background: Rectangle {
                    id: bg
                    x: 289
                    y: -287
                    anchors.fill: parent

                    gradient: Gradient {
                        orientation: Gradient.Horizontal
                        GradientStop {
                            position: 0.0
                            color: gradientStartColor
                        }
                        GradientStop {
                            position: 1.0
                            color: gradientEndColor
                        }
                    }

                    radius: 20
                }
                onClicked: {
                    _cppContext.cppSlot("BEGINCALIBRATION")
                    calibrationWindow.visible = true
                    generalControlTab.enabled = false
                    autoCalibTab.visible = false
                    manualCalibTab.visible = false
                }
            }

            Button {
                id: offsetsButton
                objectName: "offsetsButton"
                x: 877
                y: 141
                width: 628
                height: 200
                highlighted: false
                flat: true
                hoverEnabled: true
                
                Rectangle {
                    visible: parent.hovered
                    anchors.fill: parent
                    radius: 20
                    color: "#43fdfdfd"
                }

                Text {
                    color: "#ffffff"
                    text: qsTr("Adjust Offsets")
                    font.bold: true
                    anchors.centerIn: parent
                    verticalAlignment: Text.AlignVCenter
                    anchors.topMargin: 0
                    anchors.rightMargin: 0
                    anchors.bottomMargin: 0
                    font.family: "JostSemi"
                    topPadding: 0
                    bottomPadding: 0
                    horizontalAlignment: Text.AlignHCenter
                    anchors.leftMargin: 0
                    leftPadding: 0
                    font.pointSize: 48
                    rightPadding: 0
                }
                background: Rectangle {
                    id: bg1
                    color: secondaryButtonColor
                    radius: 20
                }
                onClicked: {
                    /* Little hack to refresh offsets spinners */
                    if (offsetsControl.visibleOffsetsWindowIndex == 0) {
                        waistOffsetControl.visible = true
                        leftFootOffsetControl.visible = false
                        rightFootOffsetControl.visible = false
                    } else if (offsetsControl.visibleOffsetsWindowIndex == 1) {
                        waistOffsetControl.visible = false
                        leftFootOffsetControl.visible = true
                        rightFootOffsetControl.visible = false
                    } else if (offsetsControl.visibleOffsetsWindowIndex == 2) {
                        waistOffsetControl.visible = false
                        leftFootOffsetControl.visible = false
                        rightFootOffsetControl.visible = true
                    }

                    _cppContext.cppSlot("SHOWOFFSETS")
                    offsetsControl.visible = true
                    generalControlTab.enabled = false
                }
            }

            Button {
                property bool connected: true
                property var textL: "Disconnect Trackers"
                id: disconnectTrackersButton
                x: 50
                y: 390
                width: 1455
                height: 128
                highlighted: false
                flat: true
                hoverEnabled: true
                
                Rectangle {
                    visible: parent.hovered
                    anchors.fill: parent
                    radius: 20
                    color: "#43fdfdfd"
                }

                Text {
                    color: "#ffffff"
                    text: disconnectTrackersButton.textL
                    font.bold: true
                    anchors.centerIn: parent
                    anchors.topMargin: 0
                    verticalAlignment: Text.AlignVCenter
                    anchors.rightMargin: 0
                    anchors.bottomMargin: 0
                    bottomPadding: 0
                    topPadding: 0
                    font.family: "JostSemi"
                    anchors.leftMargin: 0
                    horizontalAlignment: Text.AlignHCenter
                    leftPadding: 0
                    rightPadding: 0
                    font.pointSize: 48
                }
                background: Rectangle {
                    id: bg2
                    color: secondaryButtonColor
                    radius: 20
                }
                objectName: "disconnectTrackersButton"
                onClicked: {
                    _cppContext.cppSlot("TRACKERSCONNECTCHANGED")
                    connected = !connected
                    disconnectTrackersButton.textL
                            = connected ? "Disconnect Trackers" : "Reconnect Trackers"
                }
            }

            Label {
                id: label11
                x: 50
                y: 991
                width: 556
                height: 82
                color: textColor
                text: qsTr("Tracking Settings:")
                font.bold: true
                font.family: "JostSemi"
                font.pointSize: 48
            }

            Label {
                id: label2
                x: 127
                y: 1160
                width: 215
                height: 82
                color: textColor
                text: qsTr("Waist:")
                font.bold: true
                horizontalAlignment: Text.AlignRight
                font.family: "JostSemi"
                font.pointSize: 48
            }

            Label {
                id: label3
                x: 127
                y: 1395
                width: 215
                height: 82
                color: textColor
                text: qsTr("Feet:")
                font.bold: true
                horizontalAlignment: Text.AlignRight
                font.family: "JostSemi"
                font.pointSize: 48
            }

            Label {
                id: label4
                x: 127
                y: 1634
                width: 215
                height: 82
                color: textColor
                text: qsTr("Filter:")
                font.bold: true
                horizontalAlignment: Text.AlignRight
                font.family: "JostSemi"
                font.pointSize: 48
            }

            Label {
                id: label5
                x: 375
                y: 1860
                width: 912
                height: 82
                color: textColor
                text: qsTr("Mirror tracking when turning:")
                font.bold: true
                font.family: "JostSemi"
                horizontalAlignment: Text.AlignRight
                font.pointSize: 48
            }

            CheckBox {
                id: checkBox
                objectName: "flipCheckBox"
                x: 1319
                y: 1804
                width: 191
                height: 186
                text: qsTr("")
                visible: true
                //checkState: Qt.Checked
                checked: true
                onCheckStateChanged: _cppContext.cppSlot("FLIPCHANGED")
                hoverEnabled: true
                
                Rectangle {
                    visible: parent.hovered
                    anchors.fill: parent
                    radius: 20
                    color: "#43fdfdfd"
                }

                Component.onCompleted: checked = _get.get(qsTr("FLIPSKELETON"))

                indicator: Rectangle {
                    implicitWidth: parent.width
                    implicitHeight: parent.height
                    radius: 20
                    border.width: 20
                    border.color: "#FFFFFF"

                    Rectangle {
                        id: bg01
                        x: 14
                        y: 14
                        implicitWidth: parent.width / 1.17
                        implicitHeight: parent.height / 1.17
                        radius: 12
                        color: primaryColor
                        border.width: 0
                    }

                    Rectangle {
                        visible: checkBox.checked
                        x: 74
                        y: 81
                        width: 37
                        height: 15
                        implicitWidth: parent.width / 1.17
                        implicitHeight: parent.height / 1.17
                        radius: 12
                        color: "#ffffff"
                        border.width: 0
                        transform: Rotation {
                            angle: 45
                        }

                        Rectangle {
                            x: 22
                            y: 15
                            width: 54
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -90
                            }
                        }
                    }
                }
            }

            Rectangle {
                id: rectangle11
                x: 0
                y: 8
                width: 3915
                height: 2068
                color: primaryColor
                z: -11
                radius: 60
            }

            Rectangle {
                id: rectangle111
                x: 0
                y: 0
                width: 3915
                height: 104
                color: primaryColor
                z: -12
            }

            ComboBox {
                id: hipsComboBox
                objectName: "hipsComboBox"
                x: 380
                y: 1101
                width: 1130
                height: 185
                font.bold: false
                font.pointSize: 53
                font.family: "JostSemi"
                hoverEnabled: true
                model: [" Device-inferred Rotation", " Follow Headset", " Disable Rotation"]
                onDisplayTextChanged: _cppContext.multiCpp(
                                          "HIPS", hipsComboBox.displayText)
                
                Rectangle {
                    visible: parent.hovered
                    anchors.fill: parent
                    radius: 20
                    color: "#43fdfdfd"
                }

                Rectangle {
                    x: 953
                    y: 42
                    width: 163
                    height: 101
                    z: 10
                    color: "#00000000"

                    Rectangle {
                        x: 41
                        y: 30
                        width: 60
                        height: 15
                        implicitWidth: parent.width / 1.17
                        implicitHeight: parent.height / 1.17
                        radius: 12
                        color: "#ffffff"
                        border.width: 0
                        transform: Rotation {
                            angle: 45
                        }

                        Rectangle {
                            x: 45
                            y: 15
                            width: 61
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -90
                            }
                        }
                    }
                }

                delegate: ItemDelegate {
                    width: hipsComboBox.width
                    contentItem: Text {
                        text: modelData
                        color: "#FFFFFF"
                        font: hipsComboBox.font
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    highlighted: hipsComboBox.highlightedIndex === index
                }

                contentItem: Text {
                    leftPadding: 25
                    color: "#ffffff"
                    width: hipsComboBox.width - 110
                    text: hipsComboBox.displayText
                    font.weight: Font.Bold
                    font.bold: false
                    font.pointSize: 48
                    font.family: "JostSemi"
                    styleColor: "#ffffff"
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitWidth: 120
                    implicitHeight: 40
                    border.color: "white"
                    border.width: 20
                    radius: 20
                    Rectangle {
                        id: bg02
                        x: 13
                        y: 13
                        width: 1103
                        height: parent.height - 28
                        implicitWidth: 120
                        implicitHeight: 40
                        border.color: primaryColor
                        border.width: 0
                        radius: 12
                        color: primaryColor
                    }
                }

                popup: Popup {
                    y: hipsComboBox.height + 17
                    width: hipsComboBox.width
                    implicitHeight: contentItem.implicitHeight
                    bottomPadding: 5
                    topPadding: 5

                    contentItem: ListView {
                        clip: true
                        implicitHeight: contentHeight + 30
                        model: hipsComboBox.popup.visible ? hipsComboBox.delegateModel : null
                        currentIndex: hipsComboBox.highlightedIndex

                        //ScrollIndicator.vertical: ScrollIndicator { }
                    }

                    background: Rectangle {
                        border.color: "white"
                        border.width: 20
                        radius: 20
                        color: "white"
                        Rectangle {
                            x: 13
                            y: 13
                            height: parent.height - 28
                            width: parent.width - 28
                            border.color: primaryColor
                            border.width: 0
                            radius: 12
                            color: primaryColor
                        }
                    }
                }
            }

            ComboBox {
                id: feetComboBox
                objectName: "feetComboBox"
                x: 380
                y: 1340
                width: 1130
                height: 185
                font.bold: false
                font.pointSize: 53
                font.family: "JostSemi"
                hoverEnabled: true
                model: [" Device-inferred Rotation", " Follow Headset", " Disable Rotation"]
                onDisplayTextChanged: _cppContext.multiCpp(
                                          "FEET", feetComboBox.displayText)
                
                Rectangle {
                    visible: parent.hovered
                    anchors.fill: parent
                    radius: 20
                    color: "#43fdfdfd"
                }

                Rectangle {
                    x: 953
                    y: 42
                    width: 163
                    height: 101
                    z: 10
                    color: "#00000000"

                    Rectangle {
                        x: 41
                        y: 30
                        width: 60
                        height: 15
                        implicitWidth: parent.width / 1.17
                        implicitHeight: parent.height / 1.17
                        radius: 12
                        color: "#ffffff"
                        border.width: 0
                        transform: Rotation {
                            angle: 45
                        }

                        Rectangle {
                            x: 45
                            y: 15
                            width: 61
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -90
                            }
                        }
                    }
                }

                delegate: ItemDelegate {
                    width: feetComboBox.width
                    contentItem: Text {
                        text: modelData
                        color: "#FFFFFF"
                        font: feetComboBox.font
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    highlighted: feetComboBox.highlightedIndex === index
                }

                contentItem: Text {
                    leftPadding: 25
                    color: "#ffffff"
                    width: feetComboBox.width - 110
                    text: feetComboBox.displayText
                    font.weight: Font.Bold
                    font.bold: false
                    font.pointSize: 48
                    font.family: "JostSemi"
                    styleColor: "#ffffff"
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitWidth: 120
                    implicitHeight: 40
                    border.color: "white"
                    border.width: 20
                    radius: 20
                    Rectangle {
                        id: bg03
                        x: 13
                        y: 13
                        width: 1103
                        height: parent.height - 28
                        implicitWidth: 120
                        implicitHeight: 40
                        border.color: primaryColor
                        border.width: 0
                        radius: 12
                        color: primaryColor
                    }
                }

                popup: Popup {
                    y: feetComboBox.height + 17
                    width: feetComboBox.width
                    implicitHeight: contentItem.implicitHeight
                    bottomPadding: 5
                    topPadding: 5

                    contentItem: ListView {
                        clip: true
                        implicitHeight: contentHeight + 30
                        model: feetComboBox.popup.visible ? feetComboBox.delegateModel : null
                        currentIndex: feetComboBox.highlightedIndex

                        //ScrollIndicator.vertical: ScrollIndicator { }
                    }

                    background: Rectangle {
                        border.color: "white"
                        border.width: 20
                        radius: 20
                        color: "white"
                        Rectangle {
                            x: 13
                            y: 13
                            height: parent.height - 28
                            width: parent.width - 28
                            border.color: primaryColor
                            border.width: 0
                            radius: 12
                            color: primaryColor
                        }
                    }
                }
            }

            ComboBox {
                id: filterComboBox
                objectName: "filterComboBox"
                x: 380
                y: 1580
                width: 1130
                height: 185
                editable: false
                font.bold: false
                font.pointSize: 53
                font.family: "JostSemi"
                hoverEnabled: true
                model: [" Linear Interpolation", " Lowpass Optical", " Extended Kalman", " Disable Filter"]
                onDisplayTextChanged: _cppContext.multiCpp(
                                          "FILTER", filterComboBox.displayText)
                
                Rectangle {
                    visible: parent.hovered
                    anchors.fill: parent
                    radius: 20
                    color: "#43fdfdfd"
                }

                Rectangle {
                    x: 953
                    y: 42
                    width: 163
                    height: 101
                    z: 10
                    color: "#00000000"

                    Rectangle {
                        x: 41
                        y: 30
                        width: 60
                        height: 15
                        implicitWidth: parent.width / 1.17
                        implicitHeight: parent.height / 1.17
                        radius: 12
                        color: "#ffffff"
                        border.width: 0
                        transform: Rotation {
                            angle: 45
                        }

                        Rectangle {
                            x: 45
                            y: 15
                            width: 61
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -90
                            }
                        }
                    }
                }

                delegate: ItemDelegate {
                    width: filterComboBox.width
                    contentItem: Text {
                        text: modelData
                        color: "#FFFFFF"
                        font: filterComboBox.font
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    highlighted: filterComboBox.highlightedIndex === index
                }

                contentItem: Text {
                    leftPadding: 25
                    color: "#ffffff"
                    text: filterComboBox.displayText
                    width: filterComboBox.width - 110
                    font.weight: Font.Bold
                    font.bold: false
                    font.pointSize: 48
                    font.family: "JostSemi"
                    styleColor: "#ffffff"
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitWidth: 120
                    implicitHeight: 40
                    border.color: "white"
                    border.width: 20
                    radius: 20
                    Rectangle {
                        id: bg04
                        x: 13
                        y: 13
                        width: 1103
                        height: parent.height - 28
                        implicitWidth: 120
                        implicitHeight: 40
                        border.color: primaryColor
                        border.width: 0
                        radius: 12
                        color: primaryColor
                    }
                }

                popup: Popup {
                    y: filterComboBox.height + 17
                    width: filterComboBox.width
                    implicitHeight: contentItem.implicitHeight
                    bottomPadding: 5
                    topPadding: 5

                    contentItem: ListView {
                        clip: true
                        implicitHeight: contentHeight + 40
                        model: filterComboBox.popup.visible ? filterComboBox.delegateModel : null
                        currentIndex: filterComboBox.highlightedIndex

                        //ScrollIndicator.vertical: ScrollIndicator { }
                    }

                    background: Rectangle {
                        border.color: "white"
                        border.width: 20
                        radius: 20
                        color: "white"
                        Rectangle {
                            x: 13
                            y: 13
                            height: parent.height - 28
                            width: parent.width - 28
                            border.color: primaryColor
                            border.width: 0
                            radius: 12
                            color: primaryColor
                        }
                    }
                }
            }

            Button {
                // Set these two to true to show skeleton by default
                property bool connected: true
                property bool show: true
                property var textL: "    Skeleton"
                id: skeletonButton
                x: 1649
                y: 1940
                z: 1
                width: 417
                height: 106
                highlighted: false
                flat: true
                hoverEnabled: true
                
                Rectangle {
                    visible: parent.hovered
                    anchors.fill: parent
                    radius: 20
                    color: "#43fdfdfd"
                }

                Text {
                    color: "#ffffff"
                    text: skeletonButton.textL
                    font.bold: true
                    anchors.centerIn: parent
                    anchors.topMargin: 0
                    verticalAlignment: Text.AlignVCenter
                    anchors.rightMargin: 0
                    anchors.bottomMargin: 0
                    bottomPadding: 0
                    topPadding: 0
                    font.family: "JostSemi"
                    anchors.leftMargin: 0
                    horizontalAlignment: Text.AlignHCenter
                    leftPadding: 0
                    rightPadding: 0
                    font.pointSize: 37
                }

                Image {
                    id: image11
                    x: 23
                    y: 18
                    width: 92
                    height: 70
                    source: "Vector-Union-2.png"
                    fillMode: Image.PreserveAspectFit
                }

                background: Rectangle {
                    id: bg001
                    color: secondaryButtonColor
                    radius: 20
                }
                objectName: "skeletonButton"
                onClicked: {
                    _cppContext.cppSlot("SKELETON")
                    connected = !connected
                    show = !show
                }
            }

            Label {
                id: label6
                x: 3240
                y: 53
                z: 1
                width: 612
                height: 56
                color: "#8cffffff"
                text: qsTr("Kinect for Xbox 360 (V1)")
                font.bold: true
                font.pointSize: 38
                font.family: "JostSemi"
                anchors.verticalCenterOffset: 24
                anchors.verticalCenter: rectangle1.verticalCenter
                horizontalAlignment: Text.AlignRight
            }

            Item {
                id: pt
                objectName: "noSkeletonFound"
                x: 1909
                y: 728
                width: 1727
                height: 749

                Rectangle {
                    id: rectangle3
                    x: 0
                    y: 0
                    width: 2247
                    height: 2076
                    color: "#000000"
                    anchors.leftMargin: -301
                    anchors.topMargin: -728
                    anchors.top: parent.top
                    anchors.left: parent.left
                    z: -1
                }

                Canvas {
                    id: painterCanvas
                    property bool painted: false
                    property bool run: true
                    objectName: "painterCanvas"
                    x: -301
                    y: -728
                    width: 2028
                    height: 2068
                    anchors.bottomMargin: 0
                    anchors.bottom: rectangle3.bottom
                    anchors.rightMargin: -279
                    anchors.right: parent.right
                    anchors.top: rectangle3.top
                    anchors.left: rectangle3.left
                    visible: true

                    onPaint: {
                        painterCanvas.visible = _get.get(qsTr("SKELETONSTATE"))
                                && skeletonButton.show && _get.get(
                                    qsTr("VISIBLE"))
                        var cx = getContext("2d")
                        cx.reset()

                        cx.lineWidth = 12
                        cx.strokeStyle = Qt.rgba(1, 1, 1, 1)
                        cx.beginPath()

                        cx.moveTo(_get.get(qsTr("KINECTPOSE"), 3, 0),
                                  _get.get(qsTr("KINECTPOSE"), 3, 1))
                        cx.lineTo(_get.get(qsTr("KINECTPOSE"), 2, 0),
                                  _get.get(qsTr("KINECTPOSE"), 2, 1))
                        cx.moveTo(_get.get(qsTr("KINECTPOSE"), 2, 0),
                                  _get.get(qsTr("KINECTPOSE"), 2, 1))
                        cx.lineTo(_get.get(qsTr("KINECTPOSE"), 4, 0),
                                  _get.get(qsTr("KINECTPOSE"), 4, 1))
                        cx.moveTo(_get.get(qsTr("KINECTPOSE"), 2, 0),
                                  _get.get(qsTr("KINECTPOSE"), 2, 1))
                        cx.lineTo(_get.get(qsTr("KINECTPOSE"), 8, 0),
                                  _get.get(qsTr("KINECTPOSE"), 8, 1))
                        cx.moveTo(_get.get(qsTr("KINECTPOSE"), 2, 0),
                                  _get.get(qsTr("KINECTPOSE"), 2, 1))
                        cx.lineTo(_get.get(qsTr("KINECTPOSE"), 1, 0),
                                  _get.get(qsTr("KINECTPOSE"), 1, 1))
                        cx.moveTo(_get.get(qsTr("KINECTPOSE"), 1, 0),
                                  _get.get(qsTr("KINECTPOSE"), 1, 1))
                        cx.lineTo(_get.get(qsTr("KINECTPOSE"), 0, 0),
                                  _get.get(qsTr("KINECTPOSE"), 0, 1))
                        cx.moveTo(_get.get(qsTr("KINECTPOSE"), 0, 0),
                                  _get.get(qsTr("KINECTPOSE"), 0, 1))
                        cx.lineTo(_get.get(qsTr("KINECTPOSE"), 12, 0),
                                  _get.get(qsTr("KINECTPOSE"), 12, 1))
                        cx.moveTo(_get.get(qsTr("KINECTPOSE"), 0, 0),
                                  _get.get(qsTr("KINECTPOSE"), 0, 1))
                        cx.lineTo(_get.get(qsTr("KINECTPOSE"), 16, 0),
                                  _get.get(qsTr("KINECTPOSE"), 16, 1))

                        cx.moveTo(_get.get(qsTr("KINECTPOSE"), 4, 0),
                                  _get.get(qsTr("KINECTPOSE"), 4, 1))
                        cx.lineTo(_get.get(qsTr("KINECTPOSE"), 5, 0),
                                  _get.get(qsTr("KINECTPOSE"), 5, 1))
                        cx.moveTo(_get.get(qsTr("KINECTPOSE"), 5, 0),
                                  _get.get(qsTr("KINECTPOSE"), 5, 1))
                        cx.lineTo(_get.get(qsTr("KINECTPOSE"), 6, 0),
                                  _get.get(qsTr("KINECTPOSE"), 6, 1))
                        cx.moveTo(_get.get(qsTr("KINECTPOSE"), 6, 0),
                                  _get.get(qsTr("KINECTPOSE"), 6, 1))
                        cx.lineTo(_get.get(qsTr("KINECTPOSE"), 7, 0),
                                  _get.get(qsTr("KINECTPOSE"), 7, 1))

                        cx.moveTo(_get.get(qsTr("KINECTPOSE"), 8, 0),
                                  _get.get(qsTr("KINECTPOSE"), 8, 1))
                        cx.lineTo(_get.get(qsTr("KINECTPOSE"), 9, 0),
                                  _get.get(qsTr("KINECTPOSE"), 9, 1))
                        cx.moveTo(_get.get(qsTr("KINECTPOSE"), 9, 0),
                                  _get.get(qsTr("KINECTPOSE"), 9, 1))
                        cx.lineTo(_get.get(qsTr("KINECTPOSE"), 10, 0),
                                  _get.get(qsTr("KINECTPOSE"), 10, 1))
                        cx.moveTo(_get.get(qsTr("KINECTPOSE"), 10, 0),
                                  _get.get(qsTr("KINECTPOSE"), 10, 1))
                        cx.lineTo(_get.get(qsTr("KINECTPOSE"), 11, 0),
                                  _get.get(qsTr("KINECTPOSE"), 11, 1))

                        cx.moveTo(_get.get(qsTr("KINECTPOSE"), 12, 0),
                                  _get.get(qsTr("KINECTPOSE"), 12, 1))
                        cx.lineTo(_get.get(qsTr("KINECTPOSE"), 13, 0),
                                  _get.get(qsTr("KINECTPOSE"), 13, 1))
                        cx.moveTo(_get.get(qsTr("KINECTPOSE"), 13, 0),
                                  _get.get(qsTr("KINECTPOSE"), 13, 1))
                        cx.lineTo(_get.get(qsTr("KINECTPOSE"), 14, 0),
                                  _get.get(qsTr("KINECTPOSE"), 14, 1))
                        cx.moveTo(_get.get(qsTr("KINECTPOSE"), 14, 0),
                                  _get.get(qsTr("KINECTPOSE"), 14, 1))
                        cx.lineTo(_get.get(qsTr("KINECTPOSE"), 15, 0),
                                  _get.get(qsTr("KINECTPOSE"), 15, 1))

                        cx.moveTo(_get.get(qsTr("KINECTPOSE"), 16, 0),
                                  _get.get(qsTr("KINECTPOSE"), 16, 1))
                        cx.lineTo(_get.get(qsTr("KINECTPOSE"), 17, 0),
                                  _get.get(qsTr("KINECTPOSE"), 17, 1))
                        cx.moveTo(_get.get(qsTr("KINECTPOSE"), 17, 0),
                                  _get.get(qsTr("KINECTPOSE"), 17, 1))
                        cx.lineTo(_get.get(qsTr("KINECTPOSE"), 18, 0),
                                  _get.get(qsTr("KINECTPOSE"), 18, 1))
                        cx.moveTo(_get.get(qsTr("KINECTPOSE"), 18, 0),
                                  _get.get(qsTr("KINECTPOSE"), 18, 1))
                        cx.lineTo(_get.get(qsTr("KINECTPOSE"), 19, 0),
                                  _get.get(qsTr("KINECTPOSE"), 19, 1))

                        cx.moveTo(0, 0)
                        cx.lineTo(0, 0)

                        cx.closePath()
                        cx.stroke()
                    }

                    Timer {
                        interval: 12
                        running: true
                        repeat: true
                        onTriggered: {
                            if (painterCanvas.run)
                                painterCanvas.requestPaint()

                            if (_get.get(qsTr("SKELETONSTATE"))
                                    && skeletonButton.show && _get.get(
                                        qsTr("VISIBLE")))
                                painterCanvas.run = true
                            else
                                painterCanvas.run = false
                        }
                    }
                }

                Item {
                    visible: skeletonButton.show && !painterCanvas.visible

                    Label {
                        id: label7
                        x: 190
                        y: 32
                        width: 1504
                        height: 176
                        color: "#ffffff"
                        text: qsTr("No skeleton found")
                        font.bold: true
                        font.pointSize: 127
                        font.family: "JostSemi"
                    }

                    TextArea {
                        x: 72
                        y: 250
                        width: 1529
                        height: 398
                        Text {
                            x: 13
                            y: 14
                            color: "#ffffff"
                            text: "Make sure the device works properly\nand stand at least 3 meters (9 feet)\naway from the sensor."
                            lineHeight: 1.2
                            padding: 0
                            horizontalAlignment: Text.AlignHCenter
                            font.bold: true
                            font.pointSize: 65
                            font.family: "JostSemi"
                        }
                    }

                    Image {
                        id: alert
                        x: 34
                        y: 63
                        width: 133
                        height: 126
                        source: "Group-2.png"
                    }
                }

                Rectangle {
                    id: rectangle2
                    x: 5
                    y: 5
                    width: 94
                    height: 2012
                    color: "#000000"
                    anchors.topMargin: -728
                    anchors.top: parent.top
                    z: -1
                    anchors.leftMargin: 1912
                    anchors.left: parent.left
                }

                Rectangle {
                    x: 1912
                    y: -722
                    width: 94
                    height: 2062
                    color: "#000000"
                    anchors.topMargin: 12
                    radius: 60
                    anchors.horizontalCenter: rectangle2.horizontalCenter
                    anchors.right: rectangle2.right
                    anchors.bottom: rectangle3.bottom
                    anchors.top: rectangle2.top
                    anchors.left: rectangle2.left
                }
            }
        }

        Item {
            id: offsetsControl
            width: 3915
            height: 2292
            visible: false
            clip: false
            antialiasing: true
            y: -parent.y
            z: 100
            property int visibleOffsetsWindowIndex: 0

            Rectangle {
                id: rectangleioo
                width: 3915
                height: 2292
                y: 0
                color: "#cc000010"
                radius: 59
                visible: true
            }

            Image {
                id: imageio
                x: 0
                y: 0
                width: 3915
                height: 2292
                visible: true
                fillMode: Image.PreserveAspectFit
                source: "../offsets.png"

                Rectangle {
                    id: rectangleio
                    x: 652
                    y: 358
                    width: 2618
                    height: 1578
                    color: primaryColor
                    radius: 40
                }
            }

            Item {
                id: waistOffsetControl

                SpinBox {
                    id: controlW
                    objectName: "controlW"
                    from: -36000
                    value: 0
                    to: 36000
                    stepSize: 10
                    anchors.centerIn: parent
                    editable: true

                    onValueChanged: _cppContext.multiCpp("WAISTPITCH",
                                                         '' + value)

                    property int decimals: 2
                    property real realValue: value / 100
                    x: 1033
                    width: 806
                    height: 181
                    anchors.verticalCenterOffset: 1026
                    anchors.horizontalCenterOffset: 1439

                    validator: DoubleValidator {
                        bottom: Math.min(controlW.from, controlW.to)
                        top: Math.max(controlW.from, controlW.to)
                    }

                    textFromValue: function (value, locale) {
                        return Number(value / 100).toLocaleString(
                                    locale, 'f',
                                    controlW.decimals) + qsTr("deg")
                    }

                    valueFromText: function (text, locale) {
                        return Number.fromLocaleString(locale, text) * 100
                    }

                    contentItem: TextInput {
                        z: 5
                        text: controlW.textFromValue(controlW.value,
                                                     controlW.locale)
                        anchors.bottomMargin: 15
                        anchors.rightMargin: 280
                        anchors.topMargin: 15
                        anchors.leftMargin: 50
                        anchors.fill: parent
                        font.weight: Font.DemiBold
                        font.pointSize: 50
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignLeft

                        color: "#ffffff"
                        selectionColor: "#21be2b"
                        selectedTextColor: "#ffffff"
                        verticalAlignment: Text.AlignVCenter

                        readOnly: !controlW.editable
                        validator: controlW.validator
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }

                    background: Rectangle {
                        color: primaryColor
                        implicitWidth: 140
                        border.color: "#ffffff"
                        border.width: 15
                        radius: 40
                    }

                    up.indicator: Rectangle {
                        x: parent.width - width - 15
                        y: 15
                        width: 140
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 95
                            y: 106
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -135
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }

                    down.indicator: Rectangle {
                        x: 542
                        y: 15
                        width: 130
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 35
                            y: 53
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: 45
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }
                }

                SpinBox {
                    id: control1W
                    objectName: "control1W"
                    from: -36000
                    value: 0
                    to: 36000
                    stepSize: 10
                    anchors.centerIn: parent
                    editable: true

                    property int decimals: 2
                    property real realValue: value / 100
                    x: 1033
                    width: 806
                    height: 181
                    anchors.verticalCenterOffset: 1267
                    anchors.horizontalCenterOffset: 1439

                    validator: DoubleValidator {
                        bottom: Math.min(control1W.from, control1W.to)
                        top: Math.max(control1W.from, control1W.to)
                    }

                    textFromValue: function (value, locale) {
                        return Number(value / 100).toLocaleString(
                                    locale, 'f',
                                    control1W.decimals) + qsTr("deg")
                    }

                    valueFromText: function (text, locale) {
                        return Number.fromLocaleString(locale, text) * 100
                    }

                    contentItem: TextInput {
                        z: 5
                        text: control1W.textFromValue(control1W.value,
                                                      control1W.locale)
                        anchors.bottomMargin: 15
                        anchors.rightMargin: 280
                        anchors.topMargin: 15
                        anchors.leftMargin: 50
                        anchors.fill: parent
                        font.weight: Font.DemiBold
                        font.pointSize: 50
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignLeft

                        color: "#ffffff"
                        selectionColor: "#21be2b"
                        selectedTextColor: "#ffffff"
                        verticalAlignment: Text.AlignVCenter

                        readOnly: !control1W.editable
                        validator: control1W.validator
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }

                    background: Rectangle {
                        color: primaryColor
                        implicitWidth: 140
                        border.color: "#ffffff"
                        border.width: 15
                        radius: 40
                    }

                    up.indicator: Rectangle {
                        x: parent.width - width - 15
                        y: 15
                        width: 140
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 95
                            y: 106
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -135
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }

                    down.indicator: Rectangle {
                        x: 542
                        y: 15
                        width: 130
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 35
                            y: 53
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: 45
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }
                }

                SpinBox {
                    id: control2W
                    objectName: "control2W"
                    from: -36000
                    value: 0
                    to: 36000
                    stepSize: 10
                    anchors.centerIn: parent
                    editable: true

                    property int decimals: 2
                    property real realValue: value / 100
                    x: 1033
                    width: 806
                    height: 181
                    anchors.verticalCenterOffset: 1501
                    anchors.horizontalCenterOffset: 1439

                    validator: DoubleValidator {
                        bottom: Math.min(control2W.from, control2W.to)
                        top: Math.max(control2W.from, control2W.to)
                    }

                    textFromValue: function (value, locale) {
                        return Number(value / 100).toLocaleString(
                                    locale, 'f',
                                    control2W.decimals) + qsTr("deg")
                    }

                    valueFromText: function (text, locale) {
                        return Number.fromLocaleString(locale, text) * 100
                    }

                    contentItem: TextInput {
                        z: 5
                        text: control2W.textFromValue(control2W.value,
                                                      control2W.locale)
                        anchors.bottomMargin: 15
                        anchors.rightMargin: 280
                        anchors.topMargin: 15
                        anchors.leftMargin: 50
                        anchors.fill: parent
                        font.weight: Font.DemiBold
                        font.pointSize: 50
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignLeft

                        color: "#ffffff"
                        selectionColor: "#21be2b"
                        selectedTextColor: "#ffffff"
                        verticalAlignment: Text.AlignVCenter

                        readOnly: !control2W.editable
                        validator: control2W.validator
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }

                    background: Rectangle {
                        color: primaryColor
                        implicitWidth: 140
                        border.color: "#ffffff"
                        border.width: 15
                        radius: 40
                    }

                    up.indicator: Rectangle {
                        x: parent.width - width - 15
                        y: 15
                        width: 140
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 95
                            y: 106
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -135
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }

                    down.indicator: Rectangle {
                        x: 542
                        y: 15
                        width: 130
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 35
                            y: 53
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: 45
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }
                }

                SpinBox {
                    id: control3W
                    objectName: "control3W"
                    from: -100000
                    value: 0
                    to: 100000
                    stepSize: 1
                    anchors.centerIn: parent
                    editable: true

                    property int decimals: 2
                    property real realValue: value / 100
                    x: 2261
                    width: 806
                    height: 181
                    anchors.verticalCenterOffset: 1026
                    anchors.horizontalCenterOffset: 2661

                    validator: DoubleValidator {
                        bottom: Math.min(control3W.from, control3W.to)
                        top: Math.max(control3W.from, control3W.to)
                    }

                    textFromValue: function (value, locale) {
                        return Number(value / 100).toLocaleString(
                                    locale, 'f', control3W.decimals) + qsTr("m")
                    }

                    valueFromText: function (text, locale) {
                        return Number.fromLocaleString(locale, text) * 100
                    }

                    contentItem: TextInput {
                        z: 5
                        text: control3W.textFromValue(control3W.value,
                                                      control3W.locale)
                        anchors.bottomMargin: 15
                        anchors.rightMargin: 280
                        anchors.topMargin: 15
                        anchors.leftMargin: 50
                        anchors.fill: parent
                        font.weight: Font.DemiBold
                        font.pointSize: 50
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignLeft

                        color: "#ffffff"
                        selectionColor: "#21be2b"
                        selectedTextColor: "#ffffff"
                        verticalAlignment: Text.AlignVCenter

                        readOnly: !control3W.editable
                        validator: control3W.validator
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }

                    background: Rectangle {
                        color: primaryColor
                        implicitWidth: 140
                        border.color: "#ffffff"
                        border.width: 15
                        radius: 40
                    }

                    up.indicator: Rectangle {
                        x: parent.width - width - 15
                        y: 15
                        width: 140
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 95
                            y: 106
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -135
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }

                    down.indicator: Rectangle {
                        x: 542
                        y: 15
                        width: 130
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 35
                            y: 53
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: 45
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }
                }

                SpinBox {
                    id: control4W
                    objectName: "control4W"
                    from: -100000
                    value: 0
                    to: 100000
                    stepSize: 1
                    anchors.centerIn: parent
                    editable: true

                    property int decimals: 2
                    property real realValue: value / 100
                    x: 2261
                    width: 806
                    height: 181
                    anchors.verticalCenterOffset: 1267
                    anchors.horizontalCenterOffset: 2661

                    validator: DoubleValidator {
                        bottom: Math.min(control4W.from, control4W.to)
                        top: Math.max(control4W.from, control4W.to)
                    }

                    textFromValue: function (value, locale) {
                        return Number(value / 100).toLocaleString(
                                    locale, 'f', control4W.decimals) + qsTr("m")
                    }

                    valueFromText: function (text, locale) {
                        return Number.fromLocaleString(locale, text) * 100
                    }

                    contentItem: TextInput {
                        z: 5
                        text: control4W.textFromValue(control4W.value,
                                                      control4W.locale)
                        anchors.bottomMargin: 15
                        anchors.rightMargin: 280
                        anchors.topMargin: 15
                        anchors.leftMargin: 50
                        anchors.fill: parent
                        font.weight: Font.DemiBold
                        font.pointSize: 50
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignLeft

                        color: "#ffffff"
                        selectionColor: "#21be2b"
                        selectedTextColor: "#ffffff"
                        verticalAlignment: Text.AlignVCenter

                        readOnly: !control4W.editable
                        validator: control4W.validator
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }

                    background: Rectangle {
                        color: primaryColor
                        implicitWidth: 140
                        border.color: "#ffffff"
                        border.width: 15
                        radius: 40
                    }

                    up.indicator: Rectangle {
                        x: parent.width - width - 15
                        y: 15
                        width: 140
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 95
                            y: 106
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -135
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }

                    down.indicator: Rectangle {
                        x: 542
                        y: 15
                        width: 130
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 35
                            y: 53
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: 45
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }
                }

                SpinBox {
                    id: control5W
                    objectName: "control5W"
                    from: -100000
                    value: 0
                    to: 100000
                    stepSize: 1
                    anchors.centerIn: parent
                    editable: true

                    property int decimals: 2
                    property real realValue: value / 100
                    x: 2261
                    width: 806
                    height: 181
                    anchors.verticalCenterOffset: 1501
                    anchors.horizontalCenterOffset: 2661

                    validator: DoubleValidator {
                        bottom: Math.min(control5W.from, control5W.to)
                        top: Math.max(control5W.from, control5W.to)
                    }

                    textFromValue: function (value, locale) {
                        return Number(value / 100).toLocaleString(
                                    locale, 'f', control5W.decimals) + qsTr("m")
                    }

                    valueFromText: function (text, locale) {
                        return Number.fromLocaleString(locale, text) * 100
                    }

                    contentItem: TextInput {
                        z: 5
                        text: control5W.textFromValue(control5W.value,
                                                      control5W.locale)
                        anchors.bottomMargin: 15
                        anchors.rightMargin: 280
                        anchors.topMargin: 15
                        anchors.leftMargin: 50
                        anchors.fill: parent
                        font.weight: Font.DemiBold
                        font.pointSize: 50
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignLeft

                        color: "#ffffff"
                        selectionColor: "#21be2b"
                        selectedTextColor: "#ffffff"
                        verticalAlignment: Text.AlignVCenter

                        readOnly: !control5W.editable
                        validator: control5W.validator
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }

                    background: Rectangle {
                        color: primaryColor
                        implicitWidth: 140
                        border.color: "#ffffff"
                        border.width: 15
                        radius: 40
                    }

                    up.indicator: Rectangle {
                        x: parent.width - width - 15
                        y: 15
                        width: 140
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 95
                            y: 106
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -135
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }

                    down.indicator: Rectangle {
                        x: 542
                        y: 15
                        width: 130
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 35
                            y: 53
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: 45
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }
                }

                Label {
                    id: labelW
                    x: 821
                    y: 833
                    width: 291
                    height: 75
                    color: textColor
                    text: qsTr("Rotation:")
                    font.weight: Font.DemiBold
                    font.bold: false
                    font.pointSize: 50
                    font.family: "JostSemi"
                }

                Label {
                    id: label1W
                    x: 832
                    y: 988
                    width: 158
                    height: 75
                    color: textColor
                    text: qsTr("Roll:")
                    font.bold: false
                    font.weight: Font.DemiBold
                    font.family: "JostSemi"
                    font.pointSize: 50
                }

                Label {
                    id: label2W
                    x: 832
                    y: 1229
                    width: 158
                    height: 75
                    color: textColor
                    text: qsTr("Yaw:")
                    font.bold: false
                    font.weight: Font.DemiBold
                    font.family: "JostSemi"
                    font.pointSize: 50
                }

                Label {
                    id: label3W
                    x: 832
                    y: 1463
                    width: 158
                    height: 75
                    color: textColor
                    text: qsTr("Pitch:")
                    font.bold: false
                    font.weight: Font.DemiBold
                    font.family: "JostSemi"
                    font.pointSize: 50
                }

                Label {
                    id: label4W
                    x: 2044
                    y: 833
                    width: 291
                    height: 75
                    color: textColor
                    text: qsTr("Position:")
                    font.bold: false
                    font.weight: Font.DemiBold
                    font.family: "JostSemi"
                    font.pointSize: 50
                }

                Label {
                    id: label5W
                    x: 2055
                    y: 988
                    width: 158
                    height: 75
                    color: textColor
                    text: qsTr("X:")
                    font.bold: false
                    font.weight: Font.DemiBold
                    font.family: "JostSemi"
                    font.pointSize: 50
                }

                Label {
                    id: label6W
                    x: 2055
                    y: 1229
                    width: 158
                    height: 75
                    color: textColor
                    text: qsTr("Y:")
                    font.bold: false
                    font.weight: Font.DemiBold
                    font.family: "JostSemi"
                    font.pointSize: 50
                }

                Label {
                    id: label7W
                    x: 2055
                    y: 1463
                    width: 158
                    height: 75
                    color: textColor
                    text: qsTr("Z:")
                    font.bold: false
                    font.weight: Font.DemiBold
                    font.family: "JostSemi"
                    font.pointSize: 50
                }
            }
            Item {
                id: leftFootOffsetControl

                SpinBox {
                    id: controlL
                    objectName: "controlL"
                    from: -36000
                    value: 0
                    to: 36000
                    stepSize: 10
                    anchors.centerIn: parent
                    editable: true

                    property int decimals: 2
                    property real realValue: value / 100
                    x: 1033
                    width: 806
                    height: 181
                    anchors.verticalCenterOffset: 1026
                    anchors.horizontalCenterOffset: 1439

                    validator: DoubleValidator {
                        bottom: Math.min(controlL.from, controlL.to)
                        top: Math.max(controlL.from, controlL.to)
                    }

                    textFromValue: function (value, locale) {
                        return Number(value / 100).toLocaleString(
                                    locale, 'f',
                                    controlL.decimals) + qsTr("deg")
                    }

                    valueFromText: function (text, locale) {
                        return Number.fromLocaleString(locale, text) * 100
                    }

                    contentItem: TextInput {
                        z: 5
                        text: controlL.textFromValue(controlL.value,
                                                     controlL.locale)
                        anchors.bottomMargin: 15
                        anchors.rightMargin: 280
                        anchors.topMargin: 15
                        anchors.leftMargin: 50
                        anchors.fill: parent
                        font.weight: Font.DemiBold
                        font.pointSize: 50
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignLeft

                        color: "#ffffff"
                        selectionColor: "#21be2b"
                        selectedTextColor: "#ffffff"
                        verticalAlignment: Text.AlignVCenter

                        readOnly: !controlL.editable
                        validator: controlL.validator
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }

                    background: Rectangle {
                        color: primaryColor
                        implicitWidth: 140
                        border.color: "#ffffff"
                        border.width: 15
                        radius: 40
                    }

                    up.indicator: Rectangle {
                        x: parent.width - width - 15
                        y: 15
                        width: 140
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 95
                            y: 106
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -135
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }

                    down.indicator: Rectangle {
                        x: 542
                        y: 15
                        width: 130
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 35
                            y: 53
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: 45
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }
                }

                SpinBox {
                    id: control1L
                    objectName: "control1L"
                    from: -36000
                    value: 0
                    to: 36000
                    stepSize: 10
                    anchors.centerIn: parent
                    editable: true

                    property int decimals: 2
                    property real realValue: value / 100
                    x: 1033
                    width: 806
                    height: 181
                    anchors.verticalCenterOffset: 1267
                    anchors.horizontalCenterOffset: 1439

                    validator: DoubleValidator {
                        bottom: Math.min(control1L.from, control1L.to)
                        top: Math.max(control1L.from, control1L.to)
                    }

                    textFromValue: function (value, locale) {
                        return Number(value / 100).toLocaleString(
                                    locale, 'f',
                                    control1L.decimals) + qsTr("deg")
                    }

                    valueFromText: function (text, locale) {
                        return Number.fromLocaleString(locale, text) * 100
                    }

                    contentItem: TextInput {
                        z: 5
                        text: control1L.textFromValue(control1L.value,
                                                      control1L.locale)
                        anchors.bottomMargin: 15
                        anchors.rightMargin: 280
                        anchors.topMargin: 15
                        anchors.leftMargin: 50
                        anchors.fill: parent
                        font.weight: Font.DemiBold
                        font.pointSize: 50
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignLeft

                        color: "#ffffff"
                        selectionColor: "#21be2b"
                        selectedTextColor: "#ffffff"
                        verticalAlignment: Text.AlignVCenter

                        readOnly: !control1L.editable
                        validator: control1L.validator
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }

                    background: Rectangle {
                        color: primaryColor
                        implicitWidth: 140
                        border.color: "#ffffff"
                        border.width: 15
                        radius: 40
                    }

                    up.indicator: Rectangle {
                        x: parent.width - width - 15
                        y: 15
                        width: 140
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 95
                            y: 106
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -135
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }

                    down.indicator: Rectangle {
                        x: 542
                        y: 15
                        width: 130
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 35
                            y: 53
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: 45
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }
                }

                SpinBox {
                    id: control2L
                    objectName: "control2L"
                    from: -36000
                    value: 0
                    to: 36000
                    stepSize: 10
                    anchors.centerIn: parent
                    editable: true

                    property int decimals: 2
                    property real realValue: value / 100
                    x: 1033
                    width: 806
                    height: 181
                    anchors.verticalCenterOffset: 1501
                    anchors.horizontalCenterOffset: 1439

                    validator: DoubleValidator {
                        bottom: Math.min(control2L.from, control2L.to)
                        top: Math.max(control2L.from, control2L.to)
                    }

                    textFromValue: function (value, locale) {
                        return Number(value / 100).toLocaleString(
                                    locale, 'f',
                                    control2L.decimals) + qsTr("deg")
                    }

                    valueFromText: function (text, locale) {
                        return Number.fromLocaleString(locale, text) * 100
                    }

                    contentItem: TextInput {
                        z: 5
                        text: control2L.textFromValue(control2L.value,
                                                      control2L.locale)
                        anchors.bottomMargin: 15
                        anchors.rightMargin: 280
                        anchors.topMargin: 15
                        anchors.leftMargin: 50
                        anchors.fill: parent
                        font.weight: Font.DemiBold
                        font.pointSize: 50
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignLeft

                        color: "#ffffff"
                        selectionColor: "#21be2b"
                        selectedTextColor: "#ffffff"
                        verticalAlignment: Text.AlignVCenter

                        readOnly: !control2L.editable
                        validator: control2L.validator
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }

                    background: Rectangle {
                        color: primaryColor
                        implicitWidth: 140
                        border.color: "#ffffff"
                        border.width: 15
                        radius: 40
                    }

                    up.indicator: Rectangle {
                        x: parent.width - width - 15
                        y: 15
                        width: 140
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 95
                            y: 106
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -135
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }

                    down.indicator: Rectangle {
                        x: 542
                        y: 15
                        width: 130
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 35
                            y: 53
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: 45
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }
                }

                SpinBox {
                    id: control3L
                    objectName: "control3L"
                    from: -100000
                    value: 0
                    to: 100000
                    stepSize: 1
                    anchors.centerIn: parent
                    editable: true

                    property int decimals: 2
                    property real realValue: value / 100
                    x: 2261
                    width: 806
                    height: 181
                    anchors.verticalCenterOffset: 1026
                    anchors.horizontalCenterOffset: 2661

                    validator: DoubleValidator {
                        bottom: Math.min(control3L.from, control3L.to)
                        top: Math.max(control3L.from, control3L.to)
                    }

                    textFromValue: function (value, locale) {
                        return Number(value / 100).toLocaleString(
                                    locale, 'f', control3L.decimals) + qsTr("m")
                    }

                    valueFromText: function (text, locale) {
                        return Number.fromLocaleString(locale, text) * 100
                    }

                    contentItem: TextInput {
                        z: 5
                        text: control3L.textFromValue(control3L.value,
                                                      control3L.locale)
                        anchors.bottomMargin: 15
                        anchors.rightMargin: 280
                        anchors.topMargin: 15
                        anchors.leftMargin: 50
                        anchors.fill: parent
                        font.weight: Font.DemiBold
                        font.pointSize: 50
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignLeft

                        color: "#ffffff"
                        selectionColor: "#21be2b"
                        selectedTextColor: "#ffffff"
                        verticalAlignment: Text.AlignVCenter

                        readOnly: !control3L.editable
                        validator: control3L.validator
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }

                    background: Rectangle {
                        color: primaryColor
                        implicitWidth: 140
                        border.color: "#ffffff"
                        border.width: 15
                        radius: 40
                    }

                    up.indicator: Rectangle {
                        x: parent.width - width - 15
                        y: 15
                        width: 140
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 95
                            y: 106
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -135
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }

                    down.indicator: Rectangle {
                        x: 542
                        y: 15
                        width: 130
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 35
                            y: 53
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: 45
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }
                }

                SpinBox {
                    id: control4L
                    objectName: "control4L"
                    from: -100000
                    value: 0
                    to: 100000
                    stepSize: 1
                    anchors.centerIn: parent
                    editable: true

                    property int decimals: 2
                    property real realValue: value / 100
                    x: 2261
                    width: 806
                    height: 181
                    anchors.verticalCenterOffset: 1267
                    anchors.horizontalCenterOffset: 2661

                    validator: DoubleValidator {
                        bottom: Math.min(control4L.from, control4L.to)
                        top: Math.max(control4L.from, control4L.to)
                    }

                    textFromValue: function (value, locale) {
                        return Number(value / 100).toLocaleString(
                                    locale, 'f', control4L.decimals) + qsTr("m")
                    }

                    valueFromText: function (text, locale) {
                        return Number.fromLocaleString(locale, text) * 100
                    }

                    contentItem: TextInput {
                        z: 5
                        text: control4L.textFromValue(control4L.value,
                                                      control4L.locale)
                        anchors.bottomMargin: 15
                        anchors.rightMargin: 280
                        anchors.topMargin: 15
                        anchors.leftMargin: 50
                        anchors.fill: parent
                        font.weight: Font.DemiBold
                        font.pointSize: 50
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignLeft

                        color: "#ffffff"
                        selectionColor: "#21be2b"
                        selectedTextColor: "#ffffff"
                        verticalAlignment: Text.AlignVCenter

                        readOnly: !control4L.editable
                        validator: control4L.validator
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }

                    background: Rectangle {
                        color: primaryColor
                        implicitWidth: 140
                        border.color: "#ffffff"
                        border.width: 15
                        radius: 40
                    }

                    up.indicator: Rectangle {
                        x: parent.width - width - 15
                        y: 15
                        width: 140
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 95
                            y: 106
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -135
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }

                    down.indicator: Rectangle {
                        x: 542
                        y: 15
                        width: 130
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 35
                            y: 53
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: 45
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }
                }

                SpinBox {
                    id: control5L
                    objectName: "control5L"
                    from: -100000
                    value: 0
                    to: 100000
                    stepSize: 1
                    anchors.centerIn: parent
                    editable: true

                    property int decimals: 2
                    property real realValue: value / 100
                    x: 2261
                    width: 806
                    height: 181
                    anchors.verticalCenterOffset: 1501
                    anchors.horizontalCenterOffset: 2661

                    validator: DoubleValidator {
                        bottom: Math.min(control5L.from, control5L.to)
                        top: Math.max(control5L.from, control5L.to)
                    }

                    textFromValue: function (value, locale) {
                        return Number(value / 100).toLocaleString(
                                    locale, 'f', control5L.decimals) + qsTr("m")
                    }

                    valueFromText: function (text, locale) {
                        return Number.fromLocaleString(locale, text) * 100
                    }

                    contentItem: TextInput {
                        z: 5
                        text: control5L.textFromValue(control5L.value,
                                                      control5L.locale)
                        anchors.bottomMargin: 15
                        anchors.rightMargin: 280
                        anchors.topMargin: 15
                        anchors.leftMargin: 50
                        anchors.fill: parent
                        font.weight: Font.DemiBold
                        font.pointSize: 50
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignLeft

                        color: "#ffffff"
                        selectionColor: "#21be2b"
                        selectedTextColor: "#ffffff"
                        verticalAlignment: Text.AlignVCenter

                        readOnly: !control5L.editable
                        validator: control5L.validator
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }

                    background: Rectangle {
                        color: primaryColor
                        implicitWidth: 140
                        border.color: "#ffffff"
                        border.width: 15
                        radius: 40
                    }

                    up.indicator: Rectangle {
                        x: parent.width - width - 15
                        y: 15
                        width: 140
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 95
                            y: 106
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -135
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }

                    down.indicator: Rectangle {
                        x: 542
                        y: 15
                        width: 130
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 35
                            y: 53
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: 45
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }
                }

                Label {
                    id: labelL
                    x: 821
                    y: 833
                    width: 291
                    height: 75
                    color: textColor
                    text: qsTr("Rotation:")
                    font.weight: Font.DemiBold
                    font.bold: false
                    font.pointSize: 50
                    font.family: "JostSemi"
                }

                Label {
                    id: label1L
                    x: 832
                    y: 988
                    width: 158
                    height: 75
                    color: textColor
                    text: qsTr("Roll:")
                    font.bold: false
                    font.weight: Font.DemiBold
                    font.family: "JostSemi"
                    font.pointSize: 50
                }

                Label {
                    id: label2L
                    x: 832
                    y: 1229
                    width: 158
                    height: 75
                    color: textColor
                    text: qsTr("Yaw:")
                    font.bold: false
                    font.weight: Font.DemiBold
                    font.family: "JostSemi"
                    font.pointSize: 50
                }

                Label {
                    id: label3L
                    x: 832
                    y: 1463
                    width: 158
                    height: 75
                    color: textColor
                    text: qsTr("Pitch:")
                    font.bold: false
                    font.weight: Font.DemiBold
                    font.family: "JostSemi"
                    font.pointSize: 50
                }

                Label {
                    id: label4L
                    x: 2044
                    y: 833
                    width: 291
                    height: 75
                    color: textColor
                    text: qsTr("Position:")
                    font.bold: false
                    font.weight: Font.DemiBold
                    font.family: "JostSemi"
                    font.pointSize: 50
                }

                Label {
                    id: label5L
                    x: 2055
                    y: 988
                    width: 158
                    height: 75
                    color: textColor
                    text: qsTr("X:")
                    font.bold: false
                    font.weight: Font.DemiBold
                    font.family: "JostSemi"
                    font.pointSize: 50
                }

                Label {
                    id: label6L
                    x: 2055
                    y: 1229
                    width: 158
                    height: 75
                    color: textColor
                    text: qsTr("Y:")
                    font.bold: false
                    font.weight: Font.DemiBold
                    font.family: "JostSemi"
                    font.pointSize: 50
                }

                Label {
                    id: label7L
                    x: 2055
                    y: 1463
                    width: 158
                    height: 75
                    color: textColor
                    text: qsTr("Z:")
                    font.bold: false
                    font.weight: Font.DemiBold
                    font.family: "JostSemi"
                    font.pointSize: 50
                }
            }
            Item {
                id: rightFootOffsetControl

                SpinBox {
                    id: controlR
                    objectName: "controlR"
                    from: -36000
                    value: 0
                    to: 36000
                    stepSize: 10
                    anchors.centerIn: parent
                    editable: true

                    property int decimals: 2
                    property real realValue: value / 100
                    x: 1033
                    width: 806
                    height: 181
                    anchors.verticalCenterOffset: 1026
                    anchors.horizontalCenterOffset: 1439

                    validator: DoubleValidator {
                        bottom: Math.min(controlR.from, controlR.to)
                        top: Math.max(controlR.from, controlR.to)
                    }

                    textFromValue: function (value, locale) {
                        return Number(value / 100).toLocaleString(
                                    locale, 'f',
                                    controlR.decimals) + qsTr("deg")
                    }

                    valueFromText: function (text, locale) {
                        return Number.fromLocaleString(locale, text) * 100
                    }

                    contentItem: TextInput {
                        z: 5
                        text: controlR.textFromValue(controlR.value,
                                                     controlR.locale)
                        anchors.bottomMargin: 15
                        anchors.rightMargin: 280
                        anchors.topMargin: 15
                        anchors.leftMargin: 50
                        anchors.fill: parent
                        font.weight: Font.DemiBold
                        font.pointSize: 50
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignLeft

                        color: "#ffffff"
                        selectionColor: "#21be2b"
                        selectedTextColor: "#ffffff"
                        verticalAlignment: Text.AlignVCenter

                        readOnly: !controlR.editable
                        validator: controlR.validator
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }

                    background: Rectangle {
                        color: primaryColor
                        implicitWidth: 140
                        border.color: "#ffffff"
                        border.width: 15
                        radius: 40
                    }

                    up.indicator: Rectangle {
                        x: parent.width - width - 15
                        y: 15
                        width: 140
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 95
                            y: 106
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -135
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }

                    down.indicator: Rectangle {
                        x: 542
                        y: 15
                        width: 130
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 35
                            y: 53
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: 45
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }
                }

                SpinBox {
                    id: control1R
                    objectName: "control1R"
                    from: -36000
                    value: 0
                    to: 36000
                    stepSize: 10
                    anchors.centerIn: parent
                    editable: true

                    property int decimals: 2
                    property real realValue: value / 100
                    x: 1033
                    width: 806
                    height: 181
                    anchors.verticalCenterOffset: 1267
                    anchors.horizontalCenterOffset: 1439

                    validator: DoubleValidator {
                        bottom: Math.min(control1R.from, control1R.to)
                        top: Math.max(control1R.from, control1R.to)
                    }

                    textFromValue: function (value, locale) {
                        return Number(value / 100).toLocaleString(
                                    locale, 'f',
                                    control1R.decimals) + qsTr("deg")
                    }

                    valueFromText: function (text, locale) {
                        return Number.fromLocaleString(locale, text) * 100
                    }

                    contentItem: TextInput {
                        z: 5
                        text: control1R.textFromValue(control1R.value,
                                                      control1R.locale)
                        anchors.bottomMargin: 15
                        anchors.rightMargin: 280
                        anchors.topMargin: 15
                        anchors.leftMargin: 50
                        anchors.fill: parent
                        font.weight: Font.DemiBold
                        font.pointSize: 50
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignLeft

                        color: "#ffffff"
                        selectionColor: "#21be2b"
                        selectedTextColor: "#ffffff"
                        verticalAlignment: Text.AlignVCenter

                        readOnly: !control1R.editable
                        validator: control1R.validator
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }

                    background: Rectangle {
                        color: primaryColor
                        implicitWidth: 140
                        border.color: "#ffffff"
                        border.width: 15
                        radius: 40
                    }

                    up.indicator: Rectangle {
                        x: parent.width - width - 15
                        y: 15
                        width: 140
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 95
                            y: 106
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -135
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }

                    down.indicator: Rectangle {
                        x: 542
                        y: 15
                        width: 130
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 35
                            y: 53
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: 45
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }
                }

                SpinBox {
                    id: control2R
                    objectName: "control2R"
                    from: -36000
                    value: 0
                    to: 36000
                    stepSize: 10
                    anchors.centerIn: parent
                    editable: true

                    property int decimals: 2
                    property real realValue: value / 100
                    x: 1033
                    width: 806
                    height: 181
                    anchors.verticalCenterOffset: 1501
                    anchors.horizontalCenterOffset: 1439

                    validator: DoubleValidator {
                        bottom: Math.min(control2R.from, control2R.to)
                        top: Math.max(control2R.from, control2R.to)
                    }

                    textFromValue: function (value, locale) {
                        return Number(value / 100).toLocaleString(
                                    locale, 'f',
                                    control2R.decimals) + qsTr("deg")
                    }

                    valueFromText: function (text, locale) {
                        return Number.fromLocaleString(locale, text) * 100
                    }

                    contentItem: TextInput {
                        z: 5
                        text: control2R.textFromValue(control2R.value,
                                                      control2R.locale)
                        anchors.bottomMargin: 15
                        anchors.rightMargin: 280
                        anchors.topMargin: 15
                        anchors.leftMargin: 50
                        anchors.fill: parent
                        font.weight: Font.DemiBold
                        font.pointSize: 50
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignLeft

                        color: "#ffffff"
                        selectionColor: "#21be2b"
                        selectedTextColor: "#ffffff"
                        verticalAlignment: Text.AlignVCenter

                        readOnly: !control2R.editable
                        validator: control2R.validator
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }

                    background: Rectangle {
                        color: primaryColor
                        implicitWidth: 140
                        border.color: "#ffffff"
                        border.width: 15
                        radius: 40
                    }

                    up.indicator: Rectangle {
                        x: parent.width - width - 15
                        y: 15
                        width: 140
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 95
                            y: 106
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -135
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }

                    down.indicator: Rectangle {
                        x: 542
                        y: 15
                        width: 130
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 35
                            y: 53
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: 45
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }
                }

                SpinBox {
                    id: control3R
                    objectName: "control3R"
                    from: -100000
                    value: 0
                    to: 100000
                    stepSize: 1
                    anchors.centerIn: parent
                    editable: true

                    property int decimals: 2
                    property real realValue: value / 100
                    x: 2261
                    width: 806
                    height: 181
                    anchors.verticalCenterOffset: 1026
                    anchors.horizontalCenterOffset: 2661

                    validator: DoubleValidator {
                        bottom: Math.min(control3R.from, control3R.to)
                        top: Math.max(control3R.from, control3R.to)
                    }

                    textFromValue: function (value, locale) {
                        return Number(value / 100).toLocaleString(
                                    locale, 'f', control3R.decimals) + qsTr("m")
                    }

                    valueFromText: function (text, locale) {
                        return Number.fromLocaleString(locale, text) * 100
                    }

                    contentItem: TextInput {
                        z: 5
                        text: control3R.textFromValue(control3R.value,
                                                      control3R.locale)
                        anchors.bottomMargin: 15
                        anchors.rightMargin: 280
                        anchors.topMargin: 15
                        anchors.leftMargin: 50
                        anchors.fill: parent
                        font.weight: Font.DemiBold
                        font.pointSize: 50
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignLeft

                        color: "#ffffff"
                        selectionColor: "#21be2b"
                        selectedTextColor: "#ffffff"
                        verticalAlignment: Text.AlignVCenter

                        readOnly: !control3R.editable
                        validator: control3R.validator
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }

                    background: Rectangle {
                        color: primaryColor
                        implicitWidth: 140
                        border.color: "#ffffff"
                        border.width: 15
                        radius: 40
                    }

                    up.indicator: Rectangle {
                        x: parent.width - width - 15
                        y: 15
                        width: 140
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 95
                            y: 106
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -135
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }

                    down.indicator: Rectangle {
                        x: 542
                        y: 15
                        width: 130
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 35
                            y: 53
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: 45
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }
                }

                SpinBox {
                    id: control4R
                    objectName: "control4R"
                    from: -100000
                    value: 0
                    to: 100000
                    stepSize: 1
                    anchors.centerIn: parent
                    editable: true

                    property int decimals: 2
                    property real realValue: value / 100
                    x: 2261
                    width: 806
                    height: 181
                    anchors.verticalCenterOffset: 1267
                    anchors.horizontalCenterOffset: 2661

                    validator: DoubleValidator {
                        bottom: Math.min(control4R.from, control4R.to)
                        top: Math.max(control4R.from, control4R.to)
                    }

                    textFromValue: function (value, locale) {
                        return Number(value / 100).toLocaleString(
                                    locale, 'f', control4R.decimals) + qsTr("m")
                    }

                    valueFromText: function (text, locale) {
                        return Number.fromLocaleString(locale, text) * 100
                    }

                    contentItem: TextInput {
                        z: 5
                        text: control4R.textFromValue(control4R.value,
                                                      control4R.locale)
                        anchors.bottomMargin: 15
                        anchors.rightMargin: 280
                        anchors.topMargin: 15
                        anchors.leftMargin: 50
                        anchors.fill: parent
                        font.weight: Font.DemiBold
                        font.pointSize: 50
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignLeft

                        color: "#ffffff"
                        selectionColor: "#21be2b"
                        selectedTextColor: "#ffffff"
                        verticalAlignment: Text.AlignVCenter

                        readOnly: !control4R.editable
                        validator: control4R.validator
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }

                    background: Rectangle {
                        color: primaryColor
                        implicitWidth: 140
                        border.color: "#ffffff"
                        border.width: 15
                        radius: 40
                    }

                    up.indicator: Rectangle {
                        x: parent.width - width - 15
                        y: 15
                        width: 140
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 95
                            y: 106
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -135
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }

                    down.indicator: Rectangle {
                        x: 542
                        y: 15
                        width: 130
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 35
                            y: 53
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: 45
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }
                }

                SpinBox {
                    id: control5R
                    objectName: "control5R"
                    from: -100000
                    value: 0
                    to: 100000
                    stepSize: 1
                    anchors.centerIn: parent
                    editable: true

                    property int decimals: 2
                    property real realValue: value / 100
                    x: 2261
                    width: 806
                    height: 181
                    anchors.verticalCenterOffset: 1501
                    anchors.horizontalCenterOffset: 2661

                    validator: DoubleValidator {
                        bottom: Math.min(control5R.from, control5R.to)
                        top: Math.max(control5R.from, control5R.to)
                    }

                    textFromValue: function (value, locale) {
                        return Number(value / 100).toLocaleString(
                                    locale, 'f', control5R.decimals) + qsTr("m")
                    }

                    valueFromText: function (text, locale) {
                        return Number.fromLocaleString(locale, text) * 100
                    }

                    contentItem: TextInput {
                        z: 5
                        text: control5R.textFromValue(control5R.value,
                                                      control5R.locale)
                        anchors.bottomMargin: 15
                        anchors.rightMargin: 280
                        anchors.topMargin: 15
                        anchors.leftMargin: 50
                        anchors.fill: parent
                        font.weight: Font.DemiBold
                        font.pointSize: 50
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignLeft

                        color: "#ffffff"
                        selectionColor: "#21be2b"
                        selectedTextColor: "#ffffff"
                        verticalAlignment: Text.AlignVCenter

                        readOnly: !control5R.editable
                        validator: control5R.validator
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }

                    background: Rectangle {
                        color: primaryColor
                        implicitWidth: 140
                        border.color: "#ffffff"
                        border.width: 15
                        radius: 40
                    }

                    up.indicator: Rectangle {
                        x: parent.width - width - 15
                        y: 15
                        width: 140
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 95
                            y: 106
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: -135
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }

                    down.indicator: Rectangle {
                        x: 542
                        y: 15
                        width: 130
                        height: parent.height - 30
                        color: "#00000000"
                        implicitWidth: 140
                        implicitHeight: 40
                        border.color: "#00000000"

                        Rectangle {
                            x: 35
                            y: 53
                            width: 60
                            height: 15
                            implicitWidth: parent.width / 1.17
                            implicitHeight: parent.height / 1.17
                            radius: 12
                            color: "#ffffff"
                            border.width: 0
                            transform: Rotation {
                                angle: 45
                            }

                            Rectangle {
                                x: 45
                                y: 15
                                width: 61
                                height: 15
                                implicitWidth: parent.width / 1.17
                                implicitHeight: parent.height / 1.17
                                radius: 12
                                color: "#ffffff"
                                border.width: 0
                                transform: Rotation {
                                    angle: -90
                                }
                            }
                        }
                    }
                }

                Label {
                    id: labelR
                    x: 821
                    y: 833
                    width: 291
                    height: 75
                    color: textColor
                    text: qsTr("Rotation:")
                    font.weight: Font.DemiBold
                    font.bold: false
                    font.pointSize: 50
                    font.family: "JostSemi"
                }

                Label {
                    id: label1R
                    x: 832
                    y: 988
                    width: 158
                    height: 75
                    color: textColor
                    text: qsTr("Roll:")
                    font.bold: false
                    font.weight: Font.DemiBold
                    font.family: "JostSemi"
                    font.pointSize: 50
                }

                Label {
                    id: label2R
                    x: 832
                    y: 1229
                    width: 158
                    height: 75
                    color: textColor
                    text: qsTr("Yaw:")
                    font.bold: false
                    font.weight: Font.DemiBold
                    font.family: "JostSemi"
                    font.pointSize: 50
                }

                Label {
                    id: label3R
                    x: 832
                    y: 1463
                    width: 158
                    height: 75
                    color: textColor
                    text: qsTr("Pitch:")
                    font.bold: false
                    font.weight: Font.DemiBold
                    font.family: "JostSemi"
                    font.pointSize: 50
                }

                Label {
                    id: label4R
                    x: 2044
                    y: 833
                    width: 291
                    height: 75
                    color: textColor
                    text: qsTr("Position:")
                    font.bold: false
                    font.weight: Font.DemiBold
                    font.family: "JostSemi"
                    font.pointSize: 50
                }

                Label {
                    id: label5R
                    x: 2055
                    y: 988
                    width: 158
                    height: 75
                    color: textColor
                    text: qsTr("X:")
                    font.bold: false
                    font.weight: Font.DemiBold
                    font.family: "JostSemi"
                    font.pointSize: 50
                }

                Label {
                    id: label6R
                    x: 2055
                    y: 1229
                    width: 158
                    height: 75
                    color: textColor
                    text: qsTr("Y:")
                    font.bold: false
                    font.weight: Font.DemiBold
                    font.family: "JostSemi"
                    font.pointSize: 50
                }

                Label {
                    id: label7R
                    x: 2055
                    y: 1463
                    width: 158
                    height: 75
                    color: textColor
                    text: qsTr("Z:")
                    font.bold: false
                    font.weight: Font.DemiBold
                    font.family: "JostSemi"
                    font.pointSize: 50
                }
            }

            Label {
                id: labelio
                x: 733
                y: 420
                width: 464
                height: 86
                color: textColor
                text: qsTr("Adjust Offsets:")
                font.weight: Font.Bold
                font.pointSize: 48
                font.family: "JostSemi"
            }

            Button {
                id: waistOffsetButton
                x: 733
                y: 553
                width: 792
                height: 193
                hoverEnabled: true
                onHoveredChanged: {
                    owbg.color = waistOffsetButton.hovered ? (offsetsControl.visibleOffsetsWindowIndex === 0 ? "#3EB828" : "#7081FF") : (offsetsControl.visibleOffsetsWindowIndex === 0 ? gradientStartColor : secondaryButtonColor)
                }

                Text {
                    text: qsTr("Waist")
                    font.bold: true
                    rightPadding: 0
                    bottomPadding: 0
                    leftPadding: 0
                    topPadding: 0
                    anchors.rightMargin: 0
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 0
                    anchors.topMargin: 0
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "White"
                    font.pointSize: 48
                    font.family: "JostSemi"
                    anchors.centerIn: parent
                }
                highlighted: false
                flat: true
                background: Rectangle {
                    id: owbg
                    color: gradientStartColor
                    radius: 30
                    Rectangle {
                        color: owbg.color
                        anchors.leftMargin: 662
                        anchors.fill: parent
                    }
                }
                onClicked: {
                    waistOffsetControl.visible = true
                    leftFootOffsetControl.visible = false
                    rightFootOffsetControl.visible = false
                    offsetsControl.visibleOffsetsWindowIndex = 0

                    owbg.color = gradientStartColor
                    olfbg.color = secondaryButtonColor
                    orfbg.color = secondaryButtonColor
                }
            }

            Button {
                id: leftFootOffsetButton
                x: 1550
                y: 553
                width: 792
                height: 193
                hoverEnabled: true
                highlighted: false
                flat: true
                onHoveredChanged: {
                    olfbg.color = leftFootOffsetButton.hovered ? (offsetsControl.visibleOffsetsWindowIndex === 1 ? "#3EB828" : "#7081FF") : (offsetsControl.visibleOffsetsWindowIndex === 1 ? gradientStartColor : secondaryButtonColor)
                }
                background: Rectangle {
                    id: olfbg
                    color: secondaryButtonColor
                }
                Text {
                    color: "#ffffff"
                    text: qsTr("Left Foot")
                    anchors.topMargin: 0
                    font.family: "JostSemi"
                    rightPadding: 0
                    horizontalAlignment: Text.AlignHCenter
                    anchors.leftMargin: 0
                    anchors.rightMargin: 0
                    font.bold: true
                    topPadding: 0
                    anchors.bottomMargin: 0
                    font.pointSize: 48
                    anchors.centerIn: parent
                    bottomPadding: 0
                    leftPadding: 0
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: {
                    waistOffsetControl.visible = false
                    leftFootOffsetControl.visible = true
                    rightFootOffsetControl.visible = false
                    offsetsControl.visibleOffsetsWindowIndex = 1

                    owbg.color = secondaryButtonColor
                    if (connectOffsetsCheckBox.checked === true) {
                        olfbg.color = gradientStartColor
                        orfbg.color = gradientStartColor
                    } else {
                        olfbg.color = gradientStartColor
                        orfbg.color = secondaryButtonColor
                    }
                }
            }

            Button {
                id: rightFootOffsetButton
                x: 2367
                y: 553
                width: 792
                height: 193
                hoverEnabled: true
                highlighted: false
                flat: true
                onHoveredChanged: {
                    orfbg.color = rightFootOffsetButton.hovered ? (offsetsControl.visibleOffsetsWindowIndex === 2 ? "#3EB828" : (offsetsControl.visibleOffsetsWindowIndex === 1 && connectOffsetsCheckBox.checked ? "#3EB828" : "#7081FF")) : (offsetsControl.visibleOffsetsWindowIndex === 2 ? gradientStartColor : (offsetsControl.visibleOffsetsWindowIndex === 1 && connectOffsetsCheckBox.checked ? gradientStartColor : secondaryButtonColor))
                }
                background: Rectangle {
                    id: orfbg
                    color: secondaryButtonColor
                    radius: 30
                    Rectangle {
                        color: orfbg.color
                        anchors.rightMargin: 683
                        anchors.fill: parent
                    }
                }
                Text {
                    color: "#ffffff"
                    text: qsTr("Right Foot")
                    anchors.topMargin: 0
                    font.family: "JostSemi"
                    rightPadding: 0
                    horizontalAlignment: Text.AlignHCenter
                    anchors.leftMargin: 0
                    anchors.rightMargin: 0
                    font.bold: true
                    topPadding: 0
                    anchors.bottomMargin: 0
                    font.pointSize: 48
                    anchors.centerIn: parent
                    bottomPadding: 0
                    leftPadding: 0
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: {
                    waistOffsetControl.visible = false

                    owbg.color = secondaryButtonColor
                    if (connectOffsetsCheckBox.checked === true) {
                        olfbg.color = gradientStartColor
                        orfbg.color = gradientStartColor
                        leftFootOffsetControl.visible = true
                        rightFootOffsetControl.visible = false
                        offsetsControl.visibleOffsetsWindowIndex = 1
                    } else {
                        olfbg.color = secondaryButtonColor
                        orfbg.color = gradientStartColor
                        leftFootOffsetControl.visible = false
                        rightFootOffsetControl.visible = true
                        offsetsControl.visibleOffsetsWindowIndex = 2
                    }
                }
            }

            CheckBox {
                id: connectOffsetsCheckBox
                objectName: "connectOffsetsCheckBox"
                x: 2284
                y: 580
                width: 140
                height: 140
                text: qsTr("")
                visible: true
                checkState: Qt.Checked
                checked: true
                onCheckStateChanged: {
                    if (offsetsControl.visibleOffsetsWindowIndex === 1
                            || offsetsControl.visibleOffsetsWindowIndex === 2) {
                        waistOffsetControl.visible = true
                        leftFootOffsetControl.visible = false
                        rightFootOffsetControl.visible = false
                        offsetsControl.visibleOffsetsWindowIndex = 0

                        owbg.color = gradientStartColor
                        olfbg.color = secondaryButtonColor
                        orfbg.color = secondaryButtonColor
                    }
                    _cppContext.multiCpp("CONNECTOFFSETSCHANGED",
                                         connectOffsetsCheckBox.checked)
                }

                hoverEnabled: true
                onHoveredChanged: {
                    cobg.color = connectOffsetsCheckBox.hovered ? "#7180EE" : secondaryButtonColor
                }

                indicator: Rectangle {
                    color: primaryColor
                    implicitWidth: parent.width
                    implicitHeight: parent.height
                    radius: 20
                    border.width: 20
                    border.color: primaryColor

                    Rectangle {
                        id: cobg
                        x: 0
                        y: 0
                        width: 140
                        height: 140
                        implicitWidth: parent.width / 1.17
                        implicitHeight: parent.height / 1.17
                        radius: 30
                        color: secondaryButtonColor
                        border.width: 15
                        border.color: primaryColor

                        Image {
                            anchors.rightMargin: 0
                            anchors.leftMargin: 0
                            fillMode: Image.PreserveAspectFit
                            anchors.bottomMargin: 34
                            anchors.topMargin: 34
                            visible: connectOffsetsCheckBox.checked
                            source: "icons8_lock_2.png"
                            anchors.fill: parent
                        }
                        Image {
                            anchors.rightMargin: 0
                            anchors.leftMargin: 0
                            fillMode: Image.PreserveAspectFit
                            anchors.bottomMargin: 33
                            anchors.topMargin: 34
                            visible: !connectOffsetsCheckBox.checked
                            source: "icons8_lock_21.png"
                            anchors.fill: parent
                        }
                    }
                }
            }

            Button {
                id: confirmOffsetButton
                x: 2566
                y: 1680
                width: 643
                height: 193
                background: Rectangle {
                    id: orfbg1
                    color: secondaryButtonColor
                    radius: 30
                }
                flat: true
                hoverEnabled: true
                Text {
                    color: "#ffffff"
                    text: qsTr("Confirm")
                    anchors.leftMargin: 0
                    anchors.bottomMargin: 0
                    font.pointSize: 48
                    anchors.centerIn: parent
                    topPadding: 0
                    leftPadding: 0
                    font.bold: true
                    bottomPadding: 0
                    font.family: "JostSemi"
                    rightPadding: 0
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.topMargin: 0
                    anchors.rightMargin: 0
                }
                highlighted: false
                onHoveredChanged: {
                    orfbg1.color = confirmOffsetButton.hovered ? "#7081FF" : secondaryButtonColor
                }

                onClicked: {
                    offsetsControl.visible = false
                    _cppContext.cppSlot("OFFSETSAPPROVED")
                    generalControlTab.enabled = true
                }
            }

            Button {
                id: cancelOffsetButton
                x: 1953
                y: 1680
                width: 568
                height: 193
                background: Rectangle {
                    id: orfbg2
                    color: primaryColor
                    radius: 30
                    border.color: secondaryButtonColor
                    border.width: 15
                }
                flat: true
                Text {
                    color: secondaryButtonColor
                    text: qsTr("Cancel")
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 0
                    font.pointSize: 48
                    anchors.centerIn: parent
                    leftPadding: 0
                    topPadding: 0
                    font.bold: true
                    bottomPadding: 0
                    font.family: "JostSemi"
                    horizontalAlignment: Text.AlignHCenter
                    rightPadding: 0
                    verticalAlignment: Text.AlignVCenter
                    anchors.rightMargin: 0
                    anchors.topMargin: 0
                }
                highlighted: false
                hoverEnabled: true
                onClicked: {
                    /* Little hack to refresh offsets spinners */
                    if (offsetsControl.visibleOffsetsWindowIndex == 0) {
                        waistOffsetControl.visible = false
                        leftFootOffsetControl.visible = true
                        rightFootOffsetControl.visible = false
                    } else if (offsetsControl.visibleOffsetsWindowIndex == 1) {
                        waistOffsetControl.visible = true
                        leftFootOffsetControl.visible = false
                        rightFootOffsetControl.visible = false
                    } else if (offsetsControl.visibleOffsetsWindowIndex == 2) {
                        waistOffsetControl.visible = true
                        leftFootOffsetControl.visible = false
                        rightFootOffsetControl.visible = false
                    }

                    offsetsControl.visible = false
                    _cppContext.cppSlot("OFFSETSCANCELLED")
                    generalControlTab.enabled = true
                }
                onHoveredChanged: {
                    orfbg2.color = cancelOffsetButton.hovered ? "#5D6BD4" : primaryColor
                }
            }
        }

        Item {
            id: calibrationWindow
            width: 3915
            height: 2292
            visible: false
            clip: false
            antialiasing: true
            y: -parent.y
            z: 100

            Rectangle {
                width: 3915
                height: 2292
                y: 0
                color: "#cc000010"
                radius: 59
                visible: true
            }

            Rectangle {
                x: 652
                y: 358
                width: 2618
                height: 1504
                anchors.centerIn: parent
                color: primaryColor
                radius: 40
                id: chooseCalibTab
                visible: true

                Label {
                    x: 22
                    y: 42
                    width: 1649
                    height: 181
                    color: "#ffffff"
                    text: "Pick a calibration mode"
                    anchors.verticalCenterOffset: -573
                    anchors.horizontalCenterOffset: 0
                    anchors.centerIn: parent
                    font.pointSize: 94
                    font.family: "JostSemi"
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                }

                Button {
                    id: autoCalibButton
                    x: 90
                    y: 300
                    width: 1180
                    height: 1123
                    highlighted: false
                    flat: true
                    hoverEnabled: true
                    
                    Rectangle {
                        visible: parent.hovered
                        anchors.fill: parent
                        radius: 20
                        color: "#43fdfdfd"
                    }

                    background: Rectangle {
                        id: bgac
                        color: secondaryButtonColor
                        radius: 20
                    }

                    objectName: "autoCalibButton"
                    onClicked: {
                        _cppContext.cppSlot("START_AUTOCALIB")
                        cancelAutoCalibButton.visible = true
                        startAutoCalibButton.txt = "Start"
                        calibSeconds.color = secondaryButtonColor
                        chooseCalibTab.visible = false
                        autoCalibTab.visible = true
                    }

                    Label {
                        x: 22
                        y: 42
                        width: 1180
                        height: 993
                        color: "#ffffff"
                        text: "Automatic"
                        horizontalAlignment: Text.AlignHCenter
                        font.bold: true
                        font.pointSize: 70
                        font.family: "JostSemi"
                        anchors.centerIn: parent

                        Label {
                            x: 0
                            y: 860
                            width: 1180
                            height: 200
                            color: "#ffffff"
                            text: "Stand still in 3 different spots then\nlook in the direction of the sensor"
                            horizontalAlignment: Text.AlignHCenter
                            font.bold: true
                            font.pointSize: 48
                            font.family: "JostSemi"
                        }
                    }

                    Image {
                        x: 1146
                        y: 682
                        width: 1053
                        height: 618
                        anchors.verticalCenterOffset: -39
                        anchors.horizontalCenterOffset: 1
                        fillMode: Image.PreserveAspectFit
                        source: "Group-15.png"
                        anchors.centerIn: parent
                    }
                }

                Button {
                    id: manualCalibButton
                    x: 1348
                    y: 300
                    width: 1180
                    height: 1123
                    highlighted: false
                    flat: true
                    background: Rectangle {
                        id: bgmc
                        color: secondaryButtonColor
                        radius: 20
                    }
                    objectName: "manualCalibButton"

                    onClicked: {
                        _cppContext.cppSlot("START_MANUALCALIB")
                        chooseCalibTab.visible = false
                        manualCalibTab.visible = true
                    }
                    
                    Rectangle {
                        visible: parent.hovered
                        anchors.fill: parent
                        radius: 20
                        color: "#43fdfdfd"
                    }

                    hoverEnabled: true

                    Label {
                        x: 22
                        y: 42
                        width: 1180
                        height: 993
                        color: "#ffffff"
                        text: "Manual"
                        font.kerning: false
                        font.preferShaping: false
                        anchors.centerIn: parent
                        font.pointSize: 70
                        font.family: "JostSemi"
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        Label {
                            x: 0
                            y: 860
                            width: 1180
                            height: 200
                            color: "#ffffff"
                            text: "Use joysticks or trackpads to move\nand rotate the virtual trackers"
                            font.pointSize: 48
                            font.family: "JostSemi"
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }

                    Image {
                        x: 1146
                        y: 682
                        width: 1053
                        height: 618
                        anchors.centerIn: parent
                        anchors.horizontalCenterOffset: 1
                        source: "Group-16.png"
                        fillMode: Image.PreserveAspectFit
                        anchors.verticalCenterOffset: -39
                    }
                }
            }

            Rectangle {
                x: 652
                y: 358
                width: 2618
                height: 1578
                anchors.centerIn: parent
                color: primaryColor
                radius: 40
                id: manualCalibTab
                visible: false

                Image {
                    source: "Group-25.png"
                    anchors.centerIn: parent
                }

                Label {
                    x: 22
                    y: 42
                    width: 1649
                    height: 181
                    color: "#ffffff"
                    text: "Manual Calibration"
                    anchors.verticalCenterOffset: -573
                    anchors.horizontalCenterOffset: 0
                    anchors.centerIn: parent
                    font.pointSize: 94
                    font.family: "JostSemi"
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                }

                Button {
                    id: confirmManualCalibButton
                    x: 1917
                    y: 1323
                    width: 643
                    height: 193
                    background: Rectangle {
                        id: cmbg
                        color: secondaryButtonColor
                        radius: 30
                        border.width: 0
                    }
                    flat: true
                    hoverEnabled: true
                    Text {
                        color: "#ffffff"
                        text: qsTr("Confirm")
                        anchors.leftMargin: 0
                        anchors.bottomMargin: 0
                        font.pointSize: 48
                        anchors.centerIn: parent
                        topPadding: 0
                        leftPadding: 0
                        font.bold: true
                        bottomPadding: 0
                        font.family: "JostSemi"
                        rightPadding: 0
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.topMargin: 0
                        anchors.rightMargin: 0
                    }
                    highlighted: false
                    onHoveredChanged: {
                        cmbg.color = confirmManualCalibButton.hovered ? "#7081FF" : secondaryButtonColor
                    }
                    onClicked: {
                        _cppContext.cppSlot("MANUALCALIBRATION_APPROVED")
                        chooseCalibTab.visible = true
                        calibrationWindow.visible = false
                        generalControlTab.enabled = true
                    }
                }

                Button {
                    id: cancelManualCalibButton
                    x: 1304
                    y: 1323
                    width: 568
                    height: 193
                    visible: true
                    background: Rectangle {
                        id: cmbg1
                        color: primaryColor
                        radius: 30
                        border.color: secondaryButtonColor
                        border.width: 15
                    }
                    flat: true
                    Text {
                        color: secondaryButtonColor
                        text: qsTr("Cancel")
                        anchors.bottomMargin: 0
                        anchors.leftMargin: 0
                        font.pointSize: 48
                        anchors.centerIn: parent
                        leftPadding: 0
                        topPadding: 0
                        font.bold: true
                        bottomPadding: 0
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignHCenter
                        rightPadding: 0
                        verticalAlignment: Text.AlignVCenter
                        anchors.rightMargin: 0
                        anchors.topMargin: 0
                    }
                    highlighted: false
                    hoverEnabled: true
                    onClicked: {
                        _cppContext.cppSlot("MANUALCALIBRATION_CANCELLED")
                        chooseCalibTab.visible = true
                        calibrationWindow.visible = false
                        generalControlTab.enabled = true
                    }
                    onHoveredChanged: {
                        cmbg1.color = cancelManualCalibButton.hovered ? "#5D6BD4" : primaryColor
                    }
                }
            }

            Rectangle {
                x: 652
                y: 358
                width: 2618
                height: 1578
                anchors.centerIn: parent
                color: primaryColor
                radius: 40
                id: autoCalibTab
                visible: false

                Label {
                    x: 22
                    y: 42
                    width: 1649
                    height: 181
                    color: "#ffffff"
                    text: "Automatic Calibration"
                    anchors.verticalCenterOffset: -573
                    anchors.horizontalCenterOffset: 0
                    anchors.centerIn: parent
                    font.pointSize: 94
                    font.family: "JostSemi"
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                }

                Label {
                    y: 388
                    color: "#ffffff"
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Follow the directions"
                    anchors.horizontalCenterOffset: 1
                    font.bold: true
                    font.pointSize: 52
                    font.family: "JostSemi"
                }

                Rectangle {
                    width: 500
                    height: 500
                    anchors.centerIn: parent
                    radius: 300
                    color: secondaryButtonColor
                    id: calibSeconds

                    Label {
                        color: "#ffffff"
                        objectName: "Autocalib_seconds"
                        anchors.centerIn: parent
                        text: "5"
                        font.bold: true
                        font.pointSize: 180
                        font.family: "JostSemi"
                        styleColor: "#000000"
                    }
                    Label {
                        y: 571
                        color: "#ffffff"
                        objectName: "Autocalib_move"
                        anchors.horizontalCenter: parent.horizontalCenter
                        //text: "Move somewhere else"
                        anchors.horizontalCenterOffset: 1
                        font.bold: true
                        font.pointSize: 60
                        font.family: "JostSemi"
                    }
                }

                Button {
                    id: startAutoCalibButton
                    objectName: "startAutoCalibButton"
                    property string txt: "Start"
                    x: 1917
                    y: 1323
                    width: 643
                    height: 193
                    background: Rectangle {
                        id: cabg
                        color: secondaryButtonColor
                        radius: 30
                    }
                    flat: true
                    hoverEnabled: true
                    Text {
                        color: "#ffffff"
                        text: startAutoCalibButton.txt
                        anchors.leftMargin: 0
                        anchors.bottomMargin: 0
                        font.pointSize: 48
                        anchors.centerIn: parent
                        topPadding: 0
                        leftPadding: 0
                        font.bold: true
                        bottomPadding: 0
                        font.family: "JostSemi"
                        rightPadding: 0
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.topMargin: 0
                        anchors.rightMargin: 0
                    }
                    highlighted: false
                    onHoveredChanged: {
                        cabg.color = startAutoCalibButton.hovered ? "#42C929" : secondaryButtonColor
                    }
                    onClicked: {
                        _cppContext.cppSlot("AUTOCALIBRATION_STARTED")
                        calibSeconds.color = gradientStartColor
                        cancelAutoCalibButton.visible = false
                        startAutoCalibButton.visible = false
                        abortAutoCalibButton.visible = true
                    }
                }

                Button {
                    id: cancelAutoCalibButton
                    objectName: "cancelAutoCalibButton"
                    x: 1304
                    y: 1323
                    width: 568
                    height: 193
                    visible: true
                    background: Rectangle {
                        id: cabg1
                        color: primaryColor
                        radius: 30
                        border.color: secondaryButtonColor
                        border.width: 15
                    }
                    flat: true
                    Text {
                        color: secondaryButtonColor
                        text: qsTr("Cancel")
                        anchors.bottomMargin: 0
                        anchors.leftMargin: 0
                        font.pointSize: 48
                        anchors.centerIn: parent
                        leftPadding: 0
                        topPadding: 0
                        font.bold: true
                        bottomPadding: 0
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignHCenter
                        rightPadding: 0
                        verticalAlignment: Text.AlignVCenter
                        anchors.rightMargin: 0
                        anchors.topMargin: 0
                    }
                    highlighted: false
                    hoverEnabled: true
                    onClicked: {
                        _cppContext.cppSlot("AUTOCALIBRATION_CANCELLED")
                        chooseCalibTab.visible = true
                        calibrationWindow.visible = false
                        generalControlTab.enabled = true
                    }
                    onHoveredChanged: {
                        cabg1.color = cancelAutoCalibButton.hovered ? "#5D6BD4" : primaryColor
                    }
                }

                Button {
                    id: abortAutoCalibButton
                    objectName: "abortAutoCalibButton"
                    x: 1917
                    y: 1323
                    width: 643
                    height: 193
                    visible: false
                    background: Rectangle {
                        id: cabg11
                        color: primaryColor
                        radius: 30
                        border.color: secondaryButtonColor
                        border.width: 15
                    }
                    flat: true
                    Text {
                        color: secondaryButtonColor
                        text: qsTr("Cancel")
                        anchors.bottomMargin: 0
                        anchors.leftMargin: 0
                        font.pointSize: 48
                        anchors.centerIn: parent
                        leftPadding: 0
                        topPadding: 0
                        font.bold: true
                        bottomPadding: 0
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignHCenter
                        rightPadding: 0
                        verticalAlignment: Text.AlignVCenter
                        anchors.rightMargin: 0
                        anchors.topMargin: 0
                    }
                    highlighted: false
                    hoverEnabled: true
                    function closeAutoCalibration() {
                        chooseCalibTab.visible = true
                        calibrationWindow.visible = false
                        generalControlTab.enabled = true
                        cancelAutoCalibButton.visible = true
                        startAutoCalibButton.visible = true
                        abortAutoCalibButton.visible = false
                    }
                    onClicked: {
                        _cppContext.cppSlot("AUTOCALIBRATION_ABORTED")
                        closeAutoCalibration()
                    }
                    onHoveredChanged: {
                        cabg11.color = abortAutoCalibButton.hovered ? "#5D6BD4" : primaryColor
                    }
                }
            }
        }
    }
}

/* FORMAT
Rectangle {
                x: 652
                y: 358
                width: 2618
                height: 1578
                anchors.centerIn: parent
                color: primaryColor
                radius: 40
                id: manualCalibTab

                Button {
                    id: confirmManualCalibButton
                    x: 1917
                    y: 1323
                    width: 643
                    height: 193
                    background: Rectangle {
                        id: cmbg
                        color: secondaryButtonColor
                        radius: 30
                    }
                    flat: true
                    hoverEnabled: true
                    Text {
                        color: "#ffffff"
                        text: qsTr("Confirm")
                        anchors.leftMargin: 0
                        anchors.bottomMargin: 0
                        font.pointSize: 48
                        anchors.centerIn: parent
                        topPadding: 0
                        leftPadding: 0
                        font.bold: true
                        bottomPadding: 0
                        font.family: "JostSemi"
                        rightPadding: 0
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.topMargin: 0
                        anchors.rightMargin: 0
                    }
                    highlighted: false
                    onHoveredChanged: {
                        cmbg.color = confirmManualCalibButton.hovered ? "#7081FF" : secondaryButtonColor
                    }
                    onClicked: {
                        _cppContext.cppSlot("MANUALCALIBRATION_APPROVED")
                    }
                }

                Button {
                    id: cancelManualCalibButton
                    x: 1304
                    y: 1323
                    width: 568
                    height: 193
                    visible: true
                    background: Rectangle {
                        id: cmbg1
                        color: primaryColor
                        radius: 30
                        border.color: secondaryButtonColor
                        border.width: 15
                    }
                    flat: true
                    Text {
                        color: secondaryButtonColor
                        text: qsTr("Cancel")
                        anchors.bottomMargin: 0
                        anchors.leftMargin: 0
                        font.pointSize: 48
                        anchors.centerIn: parent
                        leftPadding: 0
                        topPadding: 0
                        font.bold: true
                        bottomPadding: 0
                        font.family: "JostSemi"
                        horizontalAlignment: Text.AlignHCenter
                        rightPadding: 0
                        verticalAlignment: Text.AlignVCenter
                        anchors.rightMargin: 0
                        anchors.topMargin: 0
                    }
                    highlighted: false
                    hoverEnabled: true
                    onClicked: {
                        _cppContext.cppSlot("MANUALCALIBRATION_CANCELLED")
                    }
                    onHoveredChanged: {
                        cmbg1.color = cancelManualCalibButton.hovered ? "#5D6BD4" : primaryColor
                    }
                }

            }
*/


/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}
}
##^##*/

