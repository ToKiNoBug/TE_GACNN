QT -= gui
QT += concurrent
CONFIG += c++11 console
CONFIG -= app_bundle

INCLUDEPATH += D:/CppLibs/eigen-3.4.0 \
                               D:/CppLibs/AlgoTemplates

SOURCES += \
        Batch.cpp \
        #CNN.cpp \
        #DataSetGenerator.cpp \
        #Sequence.cpp \
        defines.cpp \
        main.cpp

HEADERS += \
    Batch.h \
    #CNN.h \
    #DataSetGenerator.h \
    #Sequence.h \
    defines.h

QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp
