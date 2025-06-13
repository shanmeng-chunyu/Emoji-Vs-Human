QT       += core gui multimedia multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    brick.cpp \
    bullet.cpp \
    card.cpp \
    human.cpp \
    main.cpp \
    pausebutton.cpp \
    pausewidget.cpp \
    plant.cpp \
    playscene.cpp \
    startwindow.cpp

HEADERS += \
    brick.h \
    bullet.h \
    card.h \
    human.h \
    pausebutton.h \
    pausewidget.h \
    plant.h \
    playscene.h \
    startwindow.h

FORMS += \
    playscene.ui \
    startwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
