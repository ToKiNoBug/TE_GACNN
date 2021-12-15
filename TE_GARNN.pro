TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

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
