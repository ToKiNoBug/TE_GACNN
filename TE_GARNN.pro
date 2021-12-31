QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += D:/CppLibs/eigen-3.4.0

SOURCES += \
        Batch.cpp \
        DataSetGenerator.cpp \
        Gene.cpp \
        GeneticRNN.cpp \
        RNN.cpp \
        Sequence.cpp \
        defines.cpp \
        main.cpp

HEADERS += \
    Batch.h \
    DataSetGenerator.h \
    Gene.h \
    GeneticRNN.h \
    RNN.h \
    Sequence.h \
    defines.h

QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp
