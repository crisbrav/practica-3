TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        codificador.cpp \
        decodificador.cpp \
        main.cpp \
        sistema_bancario.cpp

HEADERS += \
    codificador.h \
    decodificador.h \
    sistema_bancario.h
