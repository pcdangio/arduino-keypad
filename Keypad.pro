TEMPLATE = lib
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $(ProgramFiles)\Arduino\hardware\arduino\avr\cores\arduino
INCLUDEPATH += $$PWD\src

HEADERS += \
    src/utility/key.h \
    src/keypad.h

SOURCES += \
    src/utility/key.cpp \
    src/keypad.cpp

DISTFILES += \
    library.properties \
    keywords.txt
