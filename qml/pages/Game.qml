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
import harbour.reversi.Gamemaster 1.0
import harbour.reversi.UIConnection 1.0


Page {
    id: page

    onVisibleChanged: functions.startGame()

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
    }

    Item {
        id: variable
        property string input: "Waiting..."
        property string message: ""
        property bool gamestarted: false

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
            variable.input = "Player " + player + " do your turn"
            uiconnection.newPlayerMessage(variable.input)
        }

        function endOfGame(score1, score2)
        {
            variable.message = "END OF GAME!\nPoints Player1: " + score1 + "\nPoints Player2: " + score2
            uiconnection.endOfGame()
        }

        function getMessage(newmessage)
        {
            variable.message = newmessage
        }

        function buttonPressed(x,y)
        {
            variable.input = "Waiting..."
            uiconnection.newPlayerMessage(variable.input)
            gamemaster.getInput(x,y)
            console.debug(x + "-" + y + " pressed")
        }

        function actualiseBoard()
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

        function startGame()
        {
            if(!variable.gamestarted)
            {
                console.debug("Starting game")
                actualiseBoard()
                uiconnection.startOfGame()
                variable.gamestarted = true
                gamemaster.startGame()
            }
        }
    }

    SilicaFlickable {

        contentHeight: maincolumn.height

        Column {
            id: maincolumn
            width: page.width

            PageHeader {
                title: "Reversi"
            }

            Label {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }

                text: "Starting the Game"
                visible: !variable.gamestarted
                font.pixelSize: Theme.fontSizeHuge
            }

            Label {
                visible: variable.gamestarted
                text: variable.input
                color: Theme.primaryColor
                wrapMode: Text.Wrap

                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
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

                IconButton {
                    id: button00
                    width: icon.width
                    height: icon.height
                    icon.source: variable.board00 + ".png"
                    onClicked: functions.buttonPressed(0,0)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button10
                    icon.source: variable.board10 + ".png"
                    onClicked: functions.buttonPressed(1,0)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button20
                    icon.source: variable.board20 + ".png"
                    onClicked: functions.buttonPressed(2,0)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button30
                    icon.source: variable.board30 + ".png"
                    onClicked: functions.buttonPressed(3,0)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button40
                    icon.source: variable.board40 + ".png"
                    onClicked: functions.buttonPressed(4,0)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button50
                    icon.source: variable.board50 + ".png"
                    onClicked: functions.buttonPressed(5,0)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button60
                    icon.source: variable.board60 + ".png"
                    onClicked: functions.buttonPressed(6,0)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button70
                    icon.source: variable.board70 + ".png"
                    onClicked: functions.buttonPressed(7,0)
                }

                //  Second line

                IconButton {
                    id: button01
                    width: icon.width
                    height: icon.height
                    icon.source: variable.board01 + ".png"
                    onClicked: functions.buttonPressed(0,1)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button11
                    icon.source: variable.board11 + ".png"
                    onClicked: functions.buttonPressed(1,1)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button21
                    icon.source: variable.board21 + ".png"
                    onClicked: functions.buttonPressed(2,1)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button31
                    icon.source: variable.board31 + ".png"
                    onClicked: functions.buttonPressed(3,1)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button41
                    icon.source: variable.board41 + ".png"
                    onClicked: functions.buttonPressed(4,1)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button51
                    icon.source: variable.board51 + ".png"
                    onClicked: functions.buttonPressed(5,1)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button61
                    icon.source: variable.board61 + ".png"
                    onClicked: functions.buttonPressed(6,1)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button71
                    icon.source: variable.board71 + ".png"
                    onClicked: functions.buttonPressed(7,1)
                }

                // Third line

                IconButton {
                    id: button02
                    width: icon.width
                    height: icon.height
                    icon.source: variable.board02 + ".png"
                    onClicked: functions.buttonPressed(0,2)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button12
                    icon.source: variable.board12 + ".png"
                    onClicked: functions.buttonPressed(1,2)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button22
                    icon.source: variable.board22 + ".png"
                    onClicked: functions.buttonPressed(2,2)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button32
                    icon.source: variable.board32 + ".png"
                    onClicked: functions.buttonPressed(3,2)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button42
                    icon.source: variable.board42 + ".png"
                    onClicked: functions.buttonPressed(4,2)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button52
                    icon.source: variable.board52 + ".png"
                    onClicked: functions.buttonPressed(5,2)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button62
                    icon.source: variable.board62 + ".png"
                    onClicked: functions.buttonPressed(6,2)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button72
                    icon.source: variable.board72 + ".png"
                    onClicked: functions.buttonPressed(7,2)
                }

                // Fourth line

                IconButton {
                    id: button03
                    width: icon.width
                    height: icon.height
                    icon.source: variable.board03 + ".png"
                    onClicked: functions.buttonPressed(0,3)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button13
                    icon.source: variable.board13 + ".png"
                    onClicked: functions.buttonPressed(1,3)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button23
                    icon.source: variable.board23 + ".png"
                    onClicked: functions.buttonPressed(2,3)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button33
                    icon.source: variable.board33 + ".png"
                    onClicked: functions.buttonPressed(3,3)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button43
                    icon.source: variable.board43 + ".png"
                    onClicked: functions.buttonPressed(4,3)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button53
                    icon.source: variable.board53 + ".png"
                    onClicked: functions.buttonPressed(5,3)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button63
                    icon.source: variable.board63 + ".png"
                    onClicked: functions.buttonPressed(6,3)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button73
                    icon.source: variable.board73 + ".png"
                    onClicked: functions.buttonPressed(7,3)
                }

                // Fifth line

                IconButton {
                    id: button04
                    width: icon.width
                    height: icon.height
                    icon.source: variable.board04 + ".png"
                    onClicked: functions.buttonPressed(0,4)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button14
                    icon.source: variable.board14 + ".png"
                    onClicked: functions.buttonPressed(1,4)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button24
                    icon.source: variable.board24 + ".png"
                    onClicked: functions.buttonPressed(2,4)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button34
                    icon.source: variable.board34 + ".png"
                    onClicked: functions.buttonPressed(3,4)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button44
                    icon.source: variable.board44 + ".png"
                    onClicked: functions.buttonPressed(4,4)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button54
                    icon.source: variable.board54 + ".png"
                    onClicked: functions.buttonPressed(5,4)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button64
                    icon.source: variable.board64 + ".png"
                    onClicked: functions.buttonPressed(6,4)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button74
                    icon.source: variable.board74 + ".png"
                    onClicked: functions.buttonPressed(7,4)
                }

                // Sixth line

                IconButton {
                    id: button05
                    width: icon.width
                    height: icon.height
                    icon.source: variable.board05 + ".png"
                    onClicked: functions.buttonPressed(0,5)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button15
                    icon.source: variable.board15 + ".png"
                    onClicked: functions.buttonPressed(1,5)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button25
                    icon.source: variable.board25 + ".png"
                    onClicked: functions.buttonPressed(2,5)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button35
                    icon.source: variable.board35 + ".png"
                    onClicked: functions.buttonPressed(3,5)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button45
                    icon.source: variable.board45 + ".png"
                    onClicked: functions.buttonPressed(4,5)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button55
                    icon.source: variable.board55 + ".png"
                    onClicked: functions.buttonPressed(5,5)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button65
                    icon.source: variable.board65 + ".png"
                    onClicked: functions.buttonPressed(6,5)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button75
                    icon.source: variable.board75 + ".png"
                    onClicked: functions.buttonPressed(7,5)
                }

                // Seventh line

                IconButton {
                    id: button06
                    width: icon.width
                    height: icon.height
                    icon.source: variable.board06 + ".png"
                    onClicked: functions.buttonPressed(0,6)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button16
                    icon.source: variable.board16 + ".png"
                    onClicked: functions.buttonPressed(1,6)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button26
                    icon.source: variable.board26 + ".png"
                    onClicked: functions.buttonPressed(2,6)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button36
                    icon.source: variable.board36 + ".png"
                    onClicked: functions.buttonPressed(3,6)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button46
                    icon.source: variable.board46 + ".png"
                    onClicked: functions.buttonPressed(4,6)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button56
                    icon.source: variable.board56 + ".png"
                    onClicked: functions.buttonPressed(5,6)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button66
                    icon.source: variable.board66 + ".png"
                    onClicked: functions.buttonPressed(6,6)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button76
                    icon.source: variable.board76 + ".png"
                    onClicked: functions.buttonPressed(7,6)
                }

                // Eighth line

                IconButton {
                    id: button07
                    width: icon.width
                    height: icon.height
                    icon.source: variable.board07 + ".png"
                    onClicked: functions.buttonPressed(0,7)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button17
                    icon.source: variable.board17 + ".png"
                    onClicked: functions.buttonPressed(1,7)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button27
                    icon.source: variable.board27 + ".png"
                    onClicked: functions.buttonPressed(2,7)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button37
                    icon.source: variable.board37 + ".png"
                    onClicked: functions.buttonPressed(3,7)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button47
                    icon.source: variable.board47 + ".png"
                    onClicked: functions.buttonPressed(4,7)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button57
                    icon.source: variable.board57 + ".png"
                    onClicked: functions.buttonPressed(5,7)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button67
                    icon.source: variable.board67 + ".png"
                    onClicked: functions.buttonPressed(6,7)
                }

                IconButton {
                    width: icon.width
                    height: icon.height
                    id: button77
                    icon.source: variable.board77 + ".png"
                    onClicked: functions.buttonPressed(7,7)
                }
            }

            Label {
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
        }
    }
}
