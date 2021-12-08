QT       -= core gui

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += D:/CppLibs/eigen-3.4.0

CONFIG += c++17

SOURCES += \
    DataSetGenerator.cpp \
    Gene.cpp \
    GeneticRNN.cpp \
    RNN.cpp \
    Sequence.cpp \
    defines.cpp \
    main.cpp \
    #MainWindow.cpp

HEADERS += \
    DataSetGenerator.h \
    Gene.h \
    GeneticRNN.h \
    #MainWindow.h \
    RNN.h \
    Sequence.h \
    defines.h

#FORMS += \
#    MainWindow.ui

