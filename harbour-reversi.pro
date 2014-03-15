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

SOURCES += src/harbour-reversi.cpp \
    src/gamemaster.cpp \
    src/gameboard.cpp \
    src/player.cpp \
    src/humanplayer.cpp \
    src/randomaiplayer.cpp

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
    src/gamemaster.h \
    src/gameboard.h \
    src/player.h \
    src/humanplayer.h \
    src/randomaiplayer.h

