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

release {
    DEFINES += QT_NO_DEBUG_OUTPUT
}

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
    src/rules/minimiseopponentmovementrule.cpp \
    src/rules/maximiseownmovementrule.cpp \
    src/rules/fewerfrontierdiscsrule.cpp \
    src/player/tutorialplayer.cpp \
    src/player/adaptivetreeaiplayer.cpp \
    src/persistenceclass.cpp

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
    templates/0.svg \
    qml/pages/Rules.qml \
    templates/2changed.svg \
    templates/1changed.svg \
    templates/0changed.svg \
    qml/pages/2changed.png \
    qml/pages/1changed.png \
    qml/pages/0changed.png \
    harbour-reversi-ui_en.ts \
    harbour-reversi-ui_de.ts \
    harbour-reversi-ui.pro \
    src/translation/reversi-core_de.ts \
    src/translation/reversi-core.pro \
    src/translation/reversi-core_en.ts \
    ChangeLog.txt \
    qml/pages/clicked2changed.png \
    qml/pages/clicked2.png \
    qml/pages/clicked1changed.png \
    qml/pages/clicked1.png \
    qml/pages/clicked0changed.png \
    qml/pages/clicked0.png \
    templates/clicked2changed.svg \
    templates/clicked2.svg \
    templates/clicked1changed.svg \
    templates/clicked1.svg \
    templates/clicked0changed.svg \
    templates/clicked0.svg \
    qml/cover/harbour-reversi.png \
    .gitignore

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
    src/rules/minimiseopponentmovementrule.h \
    src/rules/maximiseownmovementrule.h \
    src/rules/fewerfrontierdiscsrule.h \
    src/player/tutorialplayer.h \
    src/player/adaptivetreeaiplayer.h \
    src/persistenceclass.h

RESOURCES += \
    src/translation/core-translation.qrc \
    translation-ui.qrc

