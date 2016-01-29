TRANSLATIONS += reversi-core_de.ts \
    reversi-core_en.ts

SOURCES += \
    ../core/gamemaster.cpp \
    ../core/gameboard.cpp \
    ../player/player.cpp \
    ../player/humanplayer.cpp \
    ../player/randomaiplayer.cpp \
    ../player/greedyaiplayer.cpp \
    ../player/treeaiplayer.cpp \
    ../player/balancedaiplayer.cpp \
    ../player/staticruleaiplayer.cpp \
    ../player/rules/rule.cpp \
    ../player/rules/greedyrule.cpp \
    ../player/rules/openingrule.cpp \
    ../player/rules/endgamerule.cpp \
    ../player/rules/cornerrule.cpp \
    ../player/rules/minimiseopponentmovementrule.cpp \
    ../player/rules/maximiseownmovementrule.cpp \
    ../player/rules/fewerfrontierdiscsrule.cpp \
    ../player/tutorialplayer.cpp \
    ../player/adaptivetreeaiplayer.cpp \
    ../player/controlaiplayer.cpp \
    ../player/assemblyaiplayer.cpp \
    ../player/AssemblyAIPlayer/core.cpp \
    ../player/AssemblyAIPlayer/insanecore.cpp \
    ../player/AssemblyAIPlayer/assemblyaihelper.cpp \
    ../player/AssemblyAIPlayer/cornercore.cpp \
    ../player/AssemblyAIPlayer/centercore.cpp \
    ../player/AssemblyAIPlayer/greedycore.cpp \
    ../player/AssemblyAIPlayer/edgecore.cpp \
    ../player/rules/rulehelper.cpp \
    ../player/AssemblyAIPlayer/frontierdiscscore.cpp \
    ../player/AssemblyAIPlayer/movementcore.cpp \
    ../player/AssemblyAIPlayer/freemovecore.cpp \
    ../player/AssemblyAIPlayer/areacontrolcore.cpp \
    ../player/neuralnetworkaiplayer.cpp \
    ../player/montecarloplayer.cpp

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
    ../player/rules/rule.h \
    ../player/rules/greedyrule.h \
    ../player/rules/openingrule.h \
    ../player/rules/endgamerule.h \
    ../player/rules/cornerrule.h \
    ../player/rules/minimiseopponentmovementrule.h \
    ../player/rules/maximiseownmovementrule.h \
    ../player/rules/fewerfrontierdiscsrule.h \
    ../player/tutorialplayer.h \
    ../player/adaptivetreeaiplayer.h \
    ../player/controlaiplayer.h \
    ../player/assemblyaiplayer.h \
    ../player/AssemblyAIPlayer/core.h \
    ../player/AssemblyAIPlayer/insanecore.h \
    ../player/AssemblyAIPlayer/assemblyaihelper.h \
    ../player/AssemblyAIPlayer/cornercore.h \
    ../player/AssemblyAIPlayer/centercore.h \
    ../player/AssemblyAIPlayer/greedycore.h \
    ../player/AssemblyAIPlayer/edgecore.h \
    ../player/rules/rulehelper.h \
    ../player/AssemblyAIPlayer/frontierdiscscore.h \
    ../player/AssemblyAIPlayer/movementcore.h \
    ../player/AssemblyAIPlayer/freemovecore.h \
    ../player/AssemblyAIPlayer/areacontrolcore.h \
    ../player/neuralnetworkaiplayer.h \
    ../player/montecarloplayer.h
