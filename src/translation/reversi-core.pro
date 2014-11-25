TRANSLATIONS += reversi-core_de.ts \
    reversi-core_en.ts

SOURCES += ../uiconnection.cpp \
    ../core/gamemaster.cpp \
    ../core/gameboard.cpp \
    ../player/player.cpp \
    ../player/humanplayer.cpp \
    ../player/randomaiplayer.cpp \
    ../player/greedyaiplayer.cpp \
    ../player/treeaiplayer.cpp \
    ../player/balancedaiplayer.cpp \
    ../player/staticruleaiplayer.cpp \
    ../player/tutorialplayer.cpp \
    ../player/adaptivetreeaiplayer.cpp \
    ../player/assemblyaiplayer.cpp \
    ../player/rules/rule.cpp \
    ../player/rules/greedyrule.cpp \
    ../player/rules/openingrule.cpp \
    ../player/rules/endgamerule.cpp \
    ../player/rules/cornerrule.cpp \
    ../player/rules/minimiseopponentmovementrule.cpp \
    ../player/rules/maximiseownmovementrule.cpp \
    ../player/rules/fewerfrontierdiscsrule.cpp

HEADERS += \
    ../core/gamemaster.h \
    ../core/gameboard.h \
    ../player/player.h \
    ../player/humanplayer.h \
    ../player/randomaiplayer.h \
    ../player/greedyaiplayer.h \
    ../player/treeaiplayer.h \
    ../player/balancedaiplayer.h \
    ../player/staticruleaiplayer.h \
    ../player/tutorialplayer.h \
    ../player/adaptivetreeaiplayer.cpp \
    ../player/assemblyaiplayer.cpp \
    ../player/rules/rule.h \
    ../player/rules/greedyrule.h \
    ../player/rules/openingrule.h \
    ../player/rules/endgamerule.h \
    ../player/rules/cornerrule.h \
    ../player/rules/minimiseopponentmovementrule.h \
    ../player/rules/maximiseownmovementrule.h \
    ../player/rules/fewerfrontierdiscsrule.h 

