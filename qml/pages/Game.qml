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

    Component.onCompleted: functions.startGame()

    Connections {
        target: gamemaster

        onGetHumanInput: {
            functions.getHumanInput(player)
        }

        onResult: {
            functions.endOfGame(player1, player2)
        }

        onSendMessage: {
            functions.getMessage(message)
        }

        onBoardChanged: {
            functions.actualiseBoard()
        }

        onLastDiscPlayed :
        {
            functions.lastDiscPlayed(x,y)
        }
    }

    Item {
        id: variable
        property string input: qsTr("Waiting...")
        property string message: ""
        property bool gamestarted: false
        property bool gamefinished: false

        property int lastChangedX: -1
        property int lastChangedY: -1

        property int lastClickedX: -1
        property int lastClickedY: -1

        property int currentPlayer: 0

        property int board00: 0
        property int board10: 0
        property int board20: 0
        property int board30: 0
        property int board40: 0
        property int board50: 0
        property int board60: 0
        property int board70: 0

        property int board01: 0
        property int board11: 0
        property int board21: 0
        property int board31: 0
        property int board41: 0
        property int board51: 0
        property int board61: 0
        property int board71: 0

        property int board02: 0
        property int board12: 0
        property int board22: 0
        property int board32: 0
        property int board42: 0
        property int board52: 0
        property int board62: 0
        property int board72: 0

        property int board03: 0
        property int board13: 0
        property int board23: 0
        property int board33: 0
        property int board43: 0
        property int board53: 0
        property int board63: 0
        property int board73: 0

        property int board04: 0
        property int board14: 0
        property int board24: 0
        property int board34: 0
        property int board44: 0
        property int board54: 0
        property int board64: 0
        property int board74: 0

        property int board05: 0
        property int board15: 0
        property int board25: 0
        property int board35: 0
        property int board45: 0
        property int board55: 0
        property int board65: 0
        property int board75: 0

        property int board06: 0
        property int board16: 0
        property int board26: 0
        property int board36: 0
        property int board46: 0
        property int board56: 0
        property int board66: 0
        property int board76: 0

        property int board07: 0
        property int board17: 0
        property int board27: 0
        property int board37: 0
        property int board47: 0
        property int board57: 0
        property int board67: 0
        property int board77: 0

    }

    Item {
        id: functions

        function getHumanInput(player)
        {
            if(!variable.gamefinished)
            {
                variable.input = qsTr("Player ") + player + qsTr(" do your turn")
                variable.currentPlayer = player
                uiconnection.newPlayerMessage(variable.input)
            }
        }

        function endOfGame(score1, score2)
        {
            if(!variable.gamefinished)
            {
                variable.gamefinished = true
                variable.input = qsTr("Finished!")
                variable.message = qsTr("END OF GAME!\nPoints Player1: ") + score1 + "\n" + qsTr("Points Player2: ") + score2
                uiconnection.endOfGame()
            }
        }

        function getMessage(newmessage)
        {
            if(!variable.gamefinished)
            {
                variable.message = newmessage
            }
        }

        function buttonPressed(x,y)
        {
            variable.lastClickedX = x
            variable.lastClickedY = y

            if(!variable.gamefinished)
            {
                variable.input = qsTr("Waiting...")
                variable.currentPlayer = 0
                uiconnection.newPlayerMessage(variable.input)
            }
            gamemaster.getInput(x,y)
        }

        function actualiseBoard()
        {
            if(!variable.gamefinished)
            {
                variable.board00 = gamemaster.getOwner(0,0)
                variable.board10 = gamemaster.getOwner(1,0)
                variable.board20 = gamemaster.getOwner(2,0)
                variable.board30 = gamemaster.getOwner(3,0)
                variable.board40 = gamemaster.getOwner(4,0)
                variable.board50 = gamemaster.getOwner(5,0)
                variable.board60 = gamemaster.getOwner(6,0)
                variable.board70 = gamemaster.getOwner(7,0)

                variable.board01 = gamemaster.getOwner(0,1)
                variable.board11 = gamemaster.getOwner(1,1)
                variable.board21 = gamemaster.getOwner(2,1)
                variable.board31 = gamemaster.getOwner(3,1)
                variable.board41 = gamemaster.getOwner(4,1)
                variable.board51 = gamemaster.getOwner(5,1)
                variable.board61 = gamemaster.getOwner(6,1)
                variable.board71 = gamemaster.getOwner(7,1)

                variable.board02 = gamemaster.getOwner(0,2)
                variable.board12 = gamemaster.getOwner(1,2)
                variable.board22 = gamemaster.getOwner(2,2)
                variable.board32 = gamemaster.getOwner(3,2)
                variable.board42 = gamemaster.getOwner(4,2)
                variable.board52 = gamemaster.getOwner(5,2)
                variable.board62 = gamemaster.getOwner(6,2)
                variable.board72 = gamemaster.getOwner(7,2)

                variable.board03 = gamemaster.getOwner(0,3)
                variable.board13 = gamemaster.getOwner(1,3)
                variable.board23 = gamemaster.getOwner(2,3)
                variable.board33 = gamemaster.getOwner(3,3)
                variable.board43 = gamemaster.getOwner(4,3)
                variable.board53 = gamemaster.getOwner(5,3)
                variable.board63 = gamemaster.getOwner(6,3)
                variable.board73 = gamemaster.getOwner(7,3)

                variable.board04 = gamemaster.getOwner(0,4)
                variable.board14 = gamemaster.getOwner(1,4)
                variable.board24 = gamemaster.getOwner(2,4)
                variable.board34 = gamemaster.getOwner(3,4)
                variable.board44 = gamemaster.getOwner(4,4)
                variable.board54 = gamemaster.getOwner(5,4)
                variable.board64 = gamemaster.getOwner(6,4)
                variable.board74 = gamemaster.getOwner(7,4)

                variable.board05 = gamemaster.getOwner(0,5)
                variable.board15 = gamemaster.getOwner(1,5)
                variable.board25 = gamemaster.getOwner(2,5)
                variable.board35 = gamemaster.getOwner(3,5)
                variable.board45 = gamemaster.getOwner(4,5)
                variable.board55 = gamemaster.getOwner(5,5)
                variable.board65 = gamemaster.getOwner(6,5)
                variable.board75 = gamemaster.getOwner(7,5)

                variable.board06 = gamemaster.getOwner(0,6)
                variable.board16 = gamemaster.getOwner(1,6)
                variable.board26 = gamemaster.getOwner(2,6)
                variable.board36 = gamemaster.getOwner(3,6)
                variable.board46 = gamemaster.getOwner(4,6)
                variable.board56 = gamemaster.getOwner(5,6)
                variable.board66 = gamemaster.getOwner(6,6)
                variable.board76 = gamemaster.getOwner(7,6)

                variable.board07 = gamemaster.getOwner(0,7)
                variable.board17 = gamemaster.getOwner(1,7)
                variable.board27 = gamemaster.getOwner(2,7)
                variable.board37 = gamemaster.getOwner(3,7)
                variable.board47 = gamemaster.getOwner(4,7)
                variable.board57 = gamemaster.getOwner(5,7)
                variable.board67 = gamemaster.getOwner(6,7)
                variable.board77 = gamemaster.getOwner(7,7)
            }
        }

        function startGame()
        {
            if(!variable.gamestarted)
            {
                actualiseBoard()
                uiconnection.startOfGame()
                variable.gamestarted = true
                gamemaster.startGame()
            }
        }

        function lastDiscPlayed(x, y)
        {
            if(!variable.gamefinished)
            {
                variable.lastChangedX = x
                variable.lastChangedY = y
            }
        }
    }

    SilicaFlickable {

        contentHeight: maincolumn.height

        VerticalScrollDecorator {}

        anchors.fill: parent

        Column {
            id: maincolumn
            width: page.width

            PageHeader {
                title: qsTr("Reversi")
            }

            Label {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }

                text: qsTr("Starting the Game")
                visible: !variable.gamestarted
                font.pixelSize: Theme.fontSizeHuge
            }

            Row {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }

                Image {
                    height: Theme.fontSizeMedium
                    width: height
                    visible: !variable.gamefinished
                    source: variable.currentPlayer + ".png"
                }

                Label {
                    visible: variable.gamestarted
                    text: variable.input
                    color: Theme.primaryColor
                    wrapMode: Text.Wrap
                }
            }

            Grid {
                visible: variable.gamestarted
                width: page.width
                id: maingrid
                columns: 8

                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }

                // First line

                OwnIconButton {
                    id: button00
                    width: parent.width/8
                    height: width
                    icon: ((variable.lastClickedX === 0 && variable.lastClickedY === 0) ? "clicked" : "") + ((variable.lastChangedX === 0 && variable.lastChangedY === 0) ? variable.board00 + "changed.png" : variable.board00 + ".png")
                    onClicked: functions.buttonPressed(0,0)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button10
                    icon: ((variable.lastClickedX === 1 && variable.lastClickedY === 0) ? "clicked" : "") + ((variable.lastChangedX === 1 && variable.lastChangedY === 0) ? variable.board10 + "changed.png" : variable.board10 + ".png")
                    onClicked: functions.buttonPressed(1,0)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button20
                    icon: ((variable.lastClickedX === 2 && variable.lastClickedY === 0) ? "clicked" : "") + ((variable.lastChangedX === 2 && variable.lastChangedY === 0) ? variable.board20 + "changed.png" : variable.board20 + ".png")
                    onClicked: functions.buttonPressed(2,0)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button30
                    icon: ((variable.lastClickedX === 3 && variable.lastClickedY === 0) ? "clicked" : "") + ((variable.lastChangedX === 3 && variable.lastChangedY === 0) ? variable.board30 + "changed.png" : variable.board30 + ".png")
                    onClicked: functions.buttonPressed(3,0)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button40
                    icon: ((variable.lastClickedX === 4 && variable.lastClickedY === 0) ? "clicked" : "") + ((variable.lastChangedX === 4 && variable.lastChangedY === 0) ? variable.board40 + "changed.png" : variable.board40 + ".png")
                    onClicked: functions.buttonPressed(4,0)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button50
                    icon: ((variable.lastClickedX === 5 && variable.lastClickedY === 0) ? "clicked" : "") + ((variable.lastChangedX === 5 && variable.lastChangedY === 0) ? variable.board50 + "changed.png" : variable.board50 + ".png")
                    onClicked: functions.buttonPressed(5,0)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button60
                    icon: ((variable.lastClickedX === 6 && variable.lastClickedY === 0) ? "clicked" : "") + ((variable.lastChangedX === 6 && variable.lastChangedY === 0) ? variable.board60 + "changed.png" : variable.board60 + ".png")
                    onClicked: functions.buttonPressed(6,0)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button70
                    icon: ((variable.lastClickedX === 7 && variable.lastClickedY === 0) ? "clicked" : "") + ((variable.lastChangedX === 7 && variable.lastChangedY === 0) ? variable.board70 + "changed.png" : variable.board70 + ".png")
                    onClicked: functions.buttonPressed(7,0)
                }

                //  Second line

                OwnIconButton {
                    id: button01
                    width: parent.width/8
                    height: width
                    icon: ((variable.lastClickedX === 0 && variable.lastClickedY === 1) ? "clicked" : "") + ((variable.lastChangedX === 0 && variable.lastChangedY === 1) ? variable.board01 + "changed.png" : variable.board01 + ".png")
                    onClicked: functions.buttonPressed(0,1)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button11
                    icon: ((variable.lastClickedX === 1 && variable.lastClickedY === 1) ? "clicked" : "") + ((variable.lastChangedX === 1 && variable.lastChangedY === 1) ? variable.board11 + "changed.png" : variable.board11 + ".png")
                    onClicked: functions.buttonPressed(1,1)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button21
                    icon: ((variable.lastClickedX === 2 && variable.lastClickedY === 1) ? "clicked" : "") + ((variable.lastChangedX === 2 && variable.lastChangedY === 1) ? variable.board21 + "changed.png" : variable.board21 + ".png")
                    onClicked: functions.buttonPressed(2,1)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button31
                    icon: ((variable.lastClickedX === 3 && variable.lastClickedY === 1) ? "clicked" : "") + ((variable.lastChangedX === 3 && variable.lastChangedY === 1) ? variable.board31 + "changed.png" : variable.board31 + ".png")
                    onClicked: functions.buttonPressed(3,1)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button41
                    icon: ((variable.lastClickedX === 4 && variable.lastClickedY === 1) ? "clicked" : "") + ((variable.lastChangedX === 4 && variable.lastChangedY === 1) ? variable.board41 + "changed.png" : variable.board41 + ".png")
                    onClicked: functions.buttonPressed(4,1)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button51
                    icon: ((variable.lastClickedX === 5 && variable.lastClickedY === 1) ? "clicked" : "") + ((variable.lastChangedX === 5 && variable.lastChangedY === 1) ? variable.board51 + "changed.png" : variable.board51 + ".png")
                    onClicked: functions.buttonPressed(5,1)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button61
                    icon: ((variable.lastClickedX === 6 && variable.lastClickedY === 1) ? "clicked" : "") + ((variable.lastChangedX === 6 && variable.lastChangedY === 1) ? variable.board61 + "changed.png" : variable.board61 + ".png")
                    onClicked: functions.buttonPressed(6,1)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button71
                    icon: ((variable.lastClickedX === 7 && variable.lastClickedY === 1) ? "clicked" : "") + ((variable.lastChangedX === 7 && variable.lastChangedY === 1) ? variable.board71 + "changed.png" : variable.board71 + ".png")
                    onClicked: functions.buttonPressed(7,1)
                }

                // Third line

                OwnIconButton {
                    id: button02
                    width: parent.width/8
                    height: width
                    icon: ((variable.lastClickedX === 0 && variable.lastClickedY === 2) ? "clicked" : "") + ((variable.lastChangedX === 0 && variable.lastChangedY === 2) ? variable.board02 + "changed.png" : variable.board02 + ".png")
                    onClicked: functions.buttonPressed(0,2)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button12
                    icon: ((variable.lastClickedX === 1 && variable.lastClickedY === 2) ? "clicked" : "") + ((variable.lastChangedX === 1 && variable.lastChangedY === 2) ? variable.board12 + "changed.png" : variable.board12 + ".png")
                    onClicked: functions.buttonPressed(1,2)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button22
                    icon: ((variable.lastClickedX === 2 && variable.lastClickedY === 2) ? "clicked" : "") + ((variable.lastChangedX === 2 && variable.lastChangedY === 2) ? variable.board22 + "changed.png" : variable.board22 + ".png")
                    onClicked: functions.buttonPressed(2,2)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button32
                    icon: ((variable.lastClickedX === 3 && variable.lastClickedY === 2) ? "clicked" : "") + ((variable.lastChangedX === 3 && variable.lastChangedY === 2) ? variable.board32 + "changed.png" : variable.board32 + ".png")
                    onClicked: functions.buttonPressed(3,2)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button42
                    icon: ((variable.lastClickedX === 4 && variable.lastClickedY === 2) ? "clicked" : "") + ((variable.lastChangedX === 4 && variable.lastChangedY === 2) ? variable.board42 + "changed.png" : variable.board42 + ".png")
                    onClicked: functions.buttonPressed(4,2)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button52
                    icon: ((variable.lastClickedX === 5 && variable.lastClickedY === 2) ? "clicked" : "") + ((variable.lastChangedX === 5 && variable.lastChangedY === 2) ? variable.board52 + "changed.png" : variable.board52 + ".png")
                    onClicked: functions.buttonPressed(5,2)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button62
                    icon: ((variable.lastClickedX === 6 && variable.lastClickedY === 2) ? "clicked" : "") + ((variable.lastChangedX === 6 && variable.lastChangedY === 2) ? variable.board62 + "changed.png" : variable.board62 + ".png")
                    onClicked: functions.buttonPressed(6,2)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button72
                    icon: ((variable.lastClickedX === 7 && variable.lastClickedY === 2) ? "clicked" : "") + ((variable.lastChangedX === 7 && variable.lastChangedY === 2) ? variable.board72 + "changed.png" : variable.board72 + ".png")
                    onClicked: functions.buttonPressed(7,2)
                }

                // Fourth line

                OwnIconButton {
                    id: button03
                    width: parent.width/8
                    height: width
                    icon: ((variable.lastClickedX === 0 && variable.lastClickedY === 3) ? "clicked" : "") + ((variable.lastChangedX === 0 && variable.lastChangedY === 3) ? variable.board03 + "changed.png" : variable.board03 + ".png")
                    onClicked: functions.buttonPressed(0,3)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button13
                    icon: ((variable.lastClickedX === 1 && variable.lastClickedY === 3) ? "clicked" : "") + ((variable.lastChangedX === 1 && variable.lastChangedY === 3) ? variable.board13 + "changed.png" : variable.board13 + ".png")
                    onClicked: functions.buttonPressed(1,3)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button23
                    icon: ((variable.lastClickedX === 2 && variable.lastClickedY === 3) ? "clicked" : "") + ((variable.lastChangedX === 2 && variable.lastChangedY === 3) ? variable.board23 + "changed.png" : variable.board23 + ".png")
                    onClicked: functions.buttonPressed(2,3)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button33
                    icon: ((variable.lastClickedX === 3 && variable.lastClickedY === 3) ? "clicked" : "") + ((variable.lastChangedX === 3 && variable.lastChangedY === 3) ? variable.board33 + "changed.png" : variable.board33 + ".png")
                    onClicked: functions.buttonPressed(3,3)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button43
                    icon: ((variable.lastClickedX === 4 && variable.lastClickedY === 3) ? "clicked" : "") + ((variable.lastChangedX === 4 && variable.lastChangedY === 3) ? variable.board43 + "changed.png" : variable.board43 + ".png")
                    onClicked: functions.buttonPressed(4,3)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button53
                    icon: ((variable.lastClickedX === 5 && variable.lastClickedY === 3) ? "clicked" : "") + ((variable.lastChangedX === 5 && variable.lastChangedY === 3) ? variable.board53 + "changed.png" : variable.board53 + ".png")
                    onClicked: functions.buttonPressed(5,3)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button63
                    icon: ((variable.lastClickedX === 6 && variable.lastClickedY === 3) ? "clicked" : "") + ((variable.lastChangedX === 6 && variable.lastChangedY === 3) ? variable.board63 + "changed.png" : variable.board63 + ".png")
                    onClicked: functions.buttonPressed(6,3)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button73
                    icon: ((variable.lastClickedX === 7 && variable.lastClickedY === 3) ? "clicked" : "") + ((variable.lastChangedX === 7 && variable.lastChangedY === 3) ? variable.board73 + "changed.png" : variable.board73 + ".png")
                    onClicked: functions.buttonPressed(7,3)
                }

                // Fifth line

                OwnIconButton {
                    id: button04
                    width: parent.width/8
                    height: width
                    icon: ((variable.lastClickedX === 0 && variable.lastClickedY === 4) ? "clicked" : "") + ((variable.lastChangedX === 0 && variable.lastChangedY === 4) ? variable.board04 + "changed.png" : variable.board04 + ".png")
                    onClicked: functions.buttonPressed(0,4)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button14
                    icon: ((variable.lastClickedX === 1 && variable.lastClickedY === 4) ? "clicked" : "") + ((variable.lastChangedX === 1 && variable.lastChangedY === 4) ? variable.board14 + "changed.png" : variable.board14 + ".png")
                    onClicked: functions.buttonPressed(1,4)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button24
                    icon: ((variable.lastClickedX === 2 && variable.lastClickedY === 4) ? "clicked" : "") + ((variable.lastChangedX === 2 && variable.lastChangedY === 4) ? variable.board24 + "changed.png" : variable.board24 + ".png")
                    onClicked: functions.buttonPressed(2,4)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button34
                    icon: ((variable.lastClickedX === 3 && variable.lastClickedY === 4) ? "clicked" : "") + ((variable.lastChangedX === 3 && variable.lastChangedY === 4) ? variable.board34 + "changed.png" : variable.board34 + ".png")
                    onClicked: functions.buttonPressed(3,4)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button44
                    icon: ((variable.lastClickedX === 4 && variable.lastClickedY === 4) ? "clicked" : "") + ((variable.lastChangedX === 4 && variable.lastChangedY === 4) ? variable.board44 + "changed.png" : variable.board44 + ".png")
                    onClicked: functions.buttonPressed(4,4)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button54
                    icon: ((variable.lastClickedX === 5 && variable.lastClickedY === 4) ? "clicked" : "") + ((variable.lastChangedX === 5 && variable.lastChangedY === 4) ? variable.board54 + "changed.png" : variable.board54 + ".png")
                    onClicked: functions.buttonPressed(5,4)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button64
                    icon: ((variable.lastClickedX === 6 && variable.lastClickedY === 4) ? "clicked" : "") + ((variable.lastChangedX === 6 && variable.lastChangedY === 4) ? variable.board64 + "changed.png" : variable.board64 + ".png")
                    onClicked: functions.buttonPressed(6,4)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button74
                    icon: ((variable.lastClickedX === 7 && variable.lastClickedY === 4) ? "clicked" : "") + ((variable.lastChangedX === 7 && variable.lastChangedY === 4) ? variable.board74 + "changed.png" : variable.board74 + ".png")
                    onClicked: functions.buttonPressed(7,4)
                }

                // Sixth line

                OwnIconButton {
                    id: button05
                    width: parent.width/8
                    height: width
                    icon: ((variable.lastClickedX === 0 && variable.lastClickedY === 5) ? "clicked" : "") + ((variable.lastChangedX === 0 && variable.lastChangedY === 5) ? variable.board05 + "changed.png" : variable.board05 + ".png")
                    onClicked: functions.buttonPressed(0,5)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button15
                    icon: ((variable.lastClickedX === 1 && variable.lastClickedY === 5) ? "clicked" : "") + ((variable.lastChangedX === 1 && variable.lastChangedY === 5) ? variable.board15 + "changed.png" : variable.board15 + ".png")
                    onClicked: functions.buttonPressed(1,5)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button25
                    icon: ((variable.lastClickedX === 2 && variable.lastClickedY === 5) ? "clicked" : "") + ((variable.lastChangedX === 2 && variable.lastChangedY === 5) ? variable.board25 + "changed.png" : variable.board25 + ".png")
                    onClicked: functions.buttonPressed(2,5)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button35
                    icon: ((variable.lastClickedX === 3 && variable.lastClickedY === 5) ? "clicked" : "") + ((variable.lastChangedX === 3 && variable.lastChangedY === 5) ? variable.board35 + "changed.png" : variable.board35 + ".png")
                    onClicked: functions.buttonPressed(3,5)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button45
                    icon: ((variable.lastClickedX === 4 && variable.lastClickedY === 5) ? "clicked" : "") + ((variable.lastChangedX === 4 && variable.lastChangedY === 5) ? variable.board45 + "changed.png" : variable.board45 + ".png")
                    onClicked: functions.buttonPressed(4,5)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button55
                    icon: ((variable.lastClickedX === 5 && variable.lastClickedY === 5) ? "clicked" : "") + ((variable.lastChangedX === 5 && variable.lastChangedY === 5) ? variable.board55 + "changed.png" : variable.board55 + ".png")
                    onClicked: functions.buttonPressed(5,5)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button65
                    icon: ((variable.lastClickedX === 6 && variable.lastClickedY === 5) ? "clicked" : "") + ((variable.lastChangedX === 6 && variable.lastChangedY === 5) ? variable.board65 + "changed.png" : variable.board65 + ".png")
                    onClicked: functions.buttonPressed(6,5)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button75
                    icon: ((variable.lastClickedX === 7 && variable.lastClickedY === 5) ? "clicked" : "") + ((variable.lastChangedX === 7 && variable.lastChangedY === 5) ? variable.board75 + "changed.png" : variable.board75 + ".png")
                    onClicked: functions.buttonPressed(7,5)
                }

                // Seventh line

                OwnIconButton {
                    id: button06
                    width: parent.width/8
                    height: width
                    icon: ((variable.lastClickedX === 0 && variable.lastClickedY === 6) ? "clicked" : "") + ((variable.lastChangedX === 0 && variable.lastChangedY === 6) ? variable.board06 + "changed.png" : variable.board06 + ".png")
                    onClicked: functions.buttonPressed(0,6)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button16
                    icon: ((variable.lastClickedX === 1 && variable.lastClickedY === 6) ? "clicked" : "") + ((variable.lastChangedX === 1 && variable.lastChangedY === 6) ? variable.board16 + "changed.png" : variable.board16 + ".png")
                    onClicked: functions.buttonPressed(1,6)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button26
                    icon: ((variable.lastClickedX === 2 && variable.lastClickedY === 6) ? "clicked" : "") + ((variable.lastChangedX === 2 && variable.lastChangedY === 6) ? variable.board26 + "changed.png" : variable.board26 + ".png")
                    onClicked: functions.buttonPressed(2,6)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button36
                    icon: ((variable.lastClickedX === 3 && variable.lastClickedY === 6) ? "clicked" : "") + ((variable.lastChangedX === 3 && variable.lastChangedY === 6) ? variable.board36 + "changed.png" : variable.board36 + ".png")
                    onClicked: functions.buttonPressed(3,6)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button46
                    icon: ((variable.lastClickedX === 4 && variable.lastClickedY === 6) ? "clicked" : "") + ((variable.lastChangedX === 4 && variable.lastChangedY === 6) ? variable.board46 + "changed.png" : variable.board46 + ".png")
                    onClicked: functions.buttonPressed(4,6)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button56
                    icon: ((variable.lastClickedX === 5 && variable.lastClickedY === 6) ? "clicked" : "") + ((variable.lastChangedX === 5 && variable.lastChangedY === 6) ? variable.board56 + "changed.png" : variable.board56 + ".png")
                    onClicked: functions.buttonPressed(5,6)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button66
                    icon: ((variable.lastClickedX === 6 && variable.lastClickedY === 6) ? "clicked" : "") + ((variable.lastChangedX === 6 && variable.lastChangedY === 6) ? variable.board66 + "changed.png" : variable.board66 + ".png")
                    onClicked: functions.buttonPressed(6,6)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button76
                    icon: ((variable.lastClickedX === 7 && variable.lastClickedY === 6) ? "clicked" : "") + ((variable.lastChangedX === 7 && variable.lastChangedY === 6) ? variable.board76 + "changed.png" : variable.board76 + ".png")
                    onClicked: functions.buttonPressed(7,6)
                }

                // Eighth line

                OwnIconButton {
                    id: button07
                    width: parent.width/8
                    height: width
                    icon: ((variable.lastClickedX === 0 && variable.lastClickedY === 7) ? "clicked" : "") + ((variable.lastChangedX === 0 && variable.lastChangedY === 7) ? variable.board07 + "changed.png" : variable.board07 + ".png")
                    onClicked: functions.buttonPressed(0,7)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button17
                    icon: ((variable.lastClickedX === 1 && variable.lastClickedY === 7) ? "clicked" : "") + ((variable.lastChangedX === 1 && variable.lastChangedY === 7) ? variable.board17 + "changed.png" : variable.board17 + ".png")
                    onClicked: functions.buttonPressed(1,7)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button27
                    icon: ((variable.lastClickedX === 2 && variable.lastClickedY === 7) ? "clicked" : "") + ((variable.lastChangedX === 2 && variable.lastChangedY === 7) ? variable.board27 + "changed.png" : variable.board27 + ".png")
                    onClicked: functions.buttonPressed(2,7)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button37
                    icon: ((variable.lastClickedX === 3 && variable.lastClickedY === 7) ? "clicked" : "") + ((variable.lastChangedX === 3 && variable.lastChangedY === 7) ? variable.board37 + "changed.png" : variable.board37 + ".png")
                    onClicked: functions.buttonPressed(3,7)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button47
                    icon: ((variable.lastClickedX === 4 && variable.lastClickedY === 7) ? "clicked" : "") + ((variable.lastChangedX === 4 && variable.lastChangedY === 7) ? variable.board47 + "changed.png" : variable.board47 + ".png")
                    onClicked: functions.buttonPressed(4,7)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button57
                    icon: ((variable.lastClickedX === 5 && variable.lastClickedY === 7) ? "clicked" : "") + ((variable.lastChangedX === 5 && variable.lastChangedY === 7) ? variable.board57 + "changed.png" : variable.board57 + ".png")
                    onClicked: functions.buttonPressed(5,7)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button67
                    icon: ((variable.lastClickedX === 6 && variable.lastClickedY === 7) ? "clicked" : "") + ((variable.lastChangedX === 6 && variable.lastChangedY === 7) ? variable.board67 + "changed.png" : variable.board67 + ".png")
                    onClicked: functions.buttonPressed(6,7)
                }

                OwnIconButton {
                    width: parent.width/8
                    height: width
                    id: button77
                    icon: ((variable.lastClickedX === 7 && variable.lastClickedY === 7) ? "clicked" : "") + ((variable.lastChangedX === 7 && variable.lastChangedY === 7) ? variable.board77 + "changed.png" : variable.board77 + ".png")
                    onClicked: functions.buttonPressed(7,7)
                }
            }

            Label {
                id: messageLabel
                visible: variable.gamestarted
                text: variable.message
                color: Theme.primaryColor
                wrapMode: Text.Wrap

                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }
            }

            Button {
                width: parent.width
                visible: variable.gamefinished
                text: qsTr("Restart game")

                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }

                onClicked: {
                    if(variable.gamefinished && gamemaster.initialise(uiconnection.stringPlayer1(), uiconnection.stringPlayer2(), uiconnection.bonus()))
                    {
                        pageStack.replace(Qt.resolvedUrl("Game.qml"))
                    }
                }
            }
        }
    }
}
