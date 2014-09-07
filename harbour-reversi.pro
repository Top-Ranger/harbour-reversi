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
    src/player/rules/rule.cpp \
    src/player/rules/greedyrule.cpp \
    src/player/rules/openingrule.cpp \
    src/player/rules/endgamerule.cpp \
    src/player/rules/cornerrule.cpp \
    src/player/rules/minimiseopponentmovementrule.cpp \
    src/player/rules/maximiseownmovementrule.cpp \
    src/player/rules/fewerfrontierdiscsrule.cpp \
    src/player/tutorialplayer.cpp \
    src/player/adaptivetreeaiplayer.cpp \
    src/persistenceclass.cpp \
    src/player/controlaiplayer.cpp \
    src/player/assemblyaiplayer.cpp \
    src/player/AssemblyAIPlayer/core.cpp \
    src/player/AssemblyAIPlayer/insanecore.cpp \
    src/player/AssemblyAIPlayer/assemblyaihelper.cpp \
    src/player/AssemblyAIPlayer/cornercore.cpp \
    src/player/AssemblyAIPlayer/centercore.cpp \
    src/player/AssemblyAIPlayer/greedycore.cpp \
    src/player/AssemblyAIPlayer/edgecore.cpp

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
    .gitignore \
    qml/pages/OwnIconButton.qml

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
    src/player/rules/rule.h \
    src/player/rules/greedyrule.h \
    src/player/rules/openingrule.h \
    src/player/rules/endgamerule.h \
    src/player/rules/cornerrule.h \
    src/player/rules/minimiseopponentmovementrule.h \
    src/player/rules/maximiseownmovementrule.h \
    src/player/rules/fewerfrontierdiscsrule.h \
    src/player/tutorialplayer.h \
    src/player/adaptivetreeaiplayer.h \
    src/persistenceclass.h \
    src/player/controlaiplayer.h \
    src/player/assemblyaiplayer.h \
    src/player/AssemblyAIPlayer/core.h \
    src/player/AssemblyAIPlayer/insanecore.h \
    src/player/AssemblyAIPlayer/assemblyaihelper.h \
    src/player/AssemblyAIPlayer/cornercore.h \
    src/player/AssemblyAIPlayer/centercore.h \
    src/player/AssemblyAIPlayer/greedycore.h \
    src/player/AssemblyAIPlayer/edgecore.h

RESOURCES += \
    src/translation/core-translation.qrc \
    translation-ui.qrc

