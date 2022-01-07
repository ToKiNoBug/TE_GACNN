TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += D:/CppLibs/eigen-3.4.0 \
                               D:/CppLibs/OptimTemplates

SOURCES += \
        Batch.cpp \
        CNN.cpp \
        DataSetMaker.cpp \
        Sequence.cpp \
        TE_defines.cpp \
        main.cpp

HEADERS += \
    Batch.h \
    CNN.h \
    DataSetMaker.h \
    Sequence.h \
    TE_defines.h \
    TrainCNN.h


QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp
