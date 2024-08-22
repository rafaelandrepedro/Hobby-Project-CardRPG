QT += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    canvas.cpp \
    card.cpp \
    deck.cpp \
    deckbuilder.cpp \
    effect.cpp \
    entity.cpp \
    game.cpp \
    main.cpp \
    mainwindow.cpp \
    customlabel.cpp \
    player.cpp \
    randomizer.cpp \
    texture.cpp \
    util.cpp

HEADERS += \
    canvas.h \
    card.h \
    deck.h \
    deckbuilder.h \
    effect.h \
    entity.h \
    game.h \
    mainwindow.h \
    customlabel.h \
    player.h \
    randomizer.h \
    texture.h \
    util.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
