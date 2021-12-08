QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += D:/CppLibs/eigen-3.4.0

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DataSetGenerator.cpp \
    Gene.cpp \
    RNN.cpp \
    Sequence.cpp \
    defines.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    DataSetGenerator.h \
    Gene.h \
    MainWindow.h \
    RNN.h \
    Sequence.h \
    defines.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
