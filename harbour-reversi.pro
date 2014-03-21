# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = harbour-reversi

CONFIG += sailfishapp

SOURCES += src/uiconnection.cpp \
    src/harbour-reversi.cpp \
    src/core/gamemaster.cpp \
    src/core/gameboard.cpp \
    src/player/player.cpp \
    src/player/humanplayer.cpp \
    src/player/randomaiplayer.cpp \
    src/player/greedyaiplayer.cpp \
    src/player/treeaiplayer.cpp \
    src/player/balancedaiplayer.cpp \
    src/player/staticruleaiplayer.cpp \
    src/rules/rule.cpp \
    src/rules/greedyrule.cpp \
    src/rules/openingrule.cpp \
    src/rules/endgamerule.cpp \
    src/rules/cornerrule.cpp \
    src/rules/minimiseopponentmovementrule.cpp

OTHER_FILES += qml/harbour-reversi.qml \
    qml/cover/CoverPage.qml \
    rpm/harbour-reversi.spec \
    rpm/harbour-reversi.yaml \
    harbour-reversi.desktop \
    qml/pages/Start.qml \
    qml/pages/Game.qml \
    LICENSE.txt \
    qml/pages/About.qml \
    qml/pages/2.png \
    qml/pages/1.png \
    qml/pages/0.png \
    README.md \
    templates/harbour-reversi.svg \
    templates/2.svg \
    templates/1.svg \
    templates/0.svg

HEADERS += \
    src/uiconnection.h \
    src/core/gamemaster.h \
    src/core/gameboard.h \
    src/player/player.h \
    src/player/humanplayer.h \
    src/player/randomaiplayer.h \
    src/player/greedyaiplayer.h \
    src/player/treeaiplayer.h \
    src/player/balancedaiplayer.h \
    src/player/staticruleaiplayer.h \
    src/rules/rule.h \
    src/rules/greedyrule.h \
    src/rules/openingrule.h \
    src/rules/endgamerule.h \
    src/rules/cornerrule.h \
    src/rules/minimiseopponentmovementrule.h

