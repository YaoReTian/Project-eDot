QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Entities/bullet.cpp \
    Entities/bulletmanager.cpp \
    Entities/enemy.cpp \
    Entities/gameitem.cpp \
    UI/progressbar.cpp \
    Utils/jsonparser.cpp \
    Utils/vector.cpp \
    Utils/vectorfield.cpp \
    chunk.cpp \
    database.cpp \
    Utils/keymap.cpp \
    gameover.cpp \
    gamescene.cpp \
    main.cpp \
    Entities/player.cpp \
    Entities/sprite.cpp \
    mainwindow.cpp \
    tilelayer.cpp \
    tilemap.cpp \
    tileset.cpp \
    view.cpp

HEADERS += \
    Entities/bullet.h \
    Entities/bulletmanager.h \
    Entities/enemy.h \
    Entities/gameitem.h \
    UI/progressbar.h \
    Utils/jsonparser.h \
    Utils/vector.h \
    Utils/vectorfield.h \
    chunk.h \
    database.h \
    Utils/global.h \
    Utils/keymap.h \
    Entities/player.h \
    Entities/sprite.h \
    gameobject.h \
    gameover.h \
    gamescene.h \
    mainwindow.h \
    tilelayer.h \
    tilemap.h \
    tileset.h \
    view.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    Project-eDot_en_GB.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md \
    GameDatabase.db

RESOURCES += \
    res.qrc
