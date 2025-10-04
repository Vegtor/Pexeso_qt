QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#qmakeforce.target = dummy
#qmakeforce.commands = $$QMAKE_DEL_FILE Makefile ##to force rerun of qmake
#qmakeforce.depends = FORCE
#PRE_TARGETDEPS += $$qmakeforce.target
#QMAKE_EXTRA_TARGETS += qmakeforce

SOURCES += \
    card.cpp \
    main.cpp \
    memory_game.cpp \
    multiplayer.cpp \
    options.cpp \
    start_menu.cpp \
    window.cpp

HEADERS += \
    card.h \
    memory_game.h \
    multiplayer.h \
    options.h \
    start_menu.h \
    window.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    memory_game.ui \
    multiplayer.ui \
    options.ui \
    start_menu.ui

RESOURCES += \
    icons.qrc
