/*
  Copyright (C) 2014 Marcus Soll
  Copyright (C) 2013 Jolla Ltd.
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    Item {
        id: gamestarter
        property bool showerror: false

        function startgame() {
            if(gamemaster.initialise(player1.currentItem.text, player2.currentItem.text, bonusslider.sliderValue))
            {
                showerror = false
                pageStack.push(Qt.resolvedUrl("Game.qml"))
            }
            else
            {
                showerror = true
            }
        }

        function changeLanguage(languageSelected) {
            if(language.currentIndex != 0)
            {
                uiconnection.setIndexPlayer1(player1.currentIndex)
                uiconnection.setIndexPlayer2(player2.currentIndex)
                uiconnection.setBonus(bonusslider.sliderValue)
                uiconnection.changeLanguage(languageSelected)
                pageStack.replace(Qt.resolvedUrl("Start.qml"))
            }
        }
    }

    onVisibleChanged: uiconnection.configureGame()

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("About.qml"))
            }

            MenuItem {
                text: qsTr("Rules")
                onClicked: pageStack.push(Qt.resolvedUrl("Rules.qml"))
            }

            MenuItem {
                text: qsTr("Start game")
                onClicked: gamestarter.startgame()
            }

        }

        // Tell SilicaFlickable the height of its content.
        contentHeight: maincolumn.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: maincolumn

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("Game Configuration")
            }

            Label {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }

                color: Theme.primaryColor
                text: qsTr("Error starting the game")
                visible: gamestarter.showerror
            }
            
            Row {

                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }

                Image{
                    y: parent.height / 2 - height/2
                    source: "1.png"
                }

                ComboBox {
                    id: player1
                    width: page.width
                    label: qsTr("Player 1")
                    currentIndex: uiconnection.indexPlayer1()

                    menu: ContextMenu {
                        MenuItem { text: "Human" }
                        MenuItem { text: "Tutorial" }
                        MenuItem { text: "Random AI"}
                        MenuItem { text: "Greedy AI"}
                        MenuItem { text: "Tree AI"}
                        MenuItem { text: "Balanced AI"}
                        MenuItem { text: "Static Rule AI"}
                        MenuItem { text: "Adaptive Tree AI"}
                    }
                }
            }

            Row {

                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }

                Image{
                    y: parent.height / 2 - height/2
                    source: "2.png"
                }

                ComboBox {
                    id: player2
                    width: page.width
                    label: qsTr("Player 2")
                    currentIndex: uiconnection.indexPlayer2()

                    menu: ContextMenu {
                        MenuItem { text: "Human" }
                        MenuItem { text: "Tutorial" }
                        MenuItem { text: "Random AI"}
                        MenuItem { text: "Greedy AI"}
                        MenuItem { text: "Tree AI"}
                        MenuItem { text: "Balanced AI"}
                        MenuItem { text: "Static Rule AI"}
                        MenuItem { text: "Adaptive Tree AI"}
                    }
                }
            }

            Slider {
                id: bonusslider
                width: page.width
                minimumValue: -10
                maximumValue: 10
                value: uiconnection.bonus()
                stepSize: 1
                valueText: value
                label: qsTr("Bonus for 2nd player")
            }

            ComboBox {
                id: language
                width: page.width
                label: qsTr("Language")
                currentIndex: 0
                menu: ContextMenu {
                    MenuItem { text: qsTr("Select Language") }
                    MenuItem { text: "de" }
                    MenuItem { text: "en" }
                }
                onCurrentIndexChanged: gamestarter.changeLanguage(currentItem.text)
            }

        }
    }
}
