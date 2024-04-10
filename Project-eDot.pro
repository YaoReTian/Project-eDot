QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    UI/button.cpp \
    UI/buttonmenu.cpp \
    UI/userinterface.cpp \
    database.cpp \
    Utils/keymap.cpp \
    main.cpp \
    Entities/movingsprite.cpp \
    Entities/player.cpp \
    Entities/sprite.cpp \
    tile.cpp \
    tilemap.cpp \
    view.cpp \
    worldscene.cpp

HEADERS += \
    UI/button.h \
    UI/buttonmenu.h \
    UI/userinterface.h \
    database.h \
    Utils/global.h \
    Utils/keymap.h \
    Entities/movingsprite.h \
    Entities/player.h \
    Entities/sprite.h \
    tile.h \
    tilemap.h \
    view.h \
    worldscene.h

FORMS +=

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
