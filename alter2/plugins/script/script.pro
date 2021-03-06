# -------------------------------------------------
# Project created by QtCreator 2010-06-09T14:26:27
# -------------------------------------------------

TARGET = script
TEMPLATE = lib

include("../../../EksCore/GeneralOptions.pri")

QT += script scripttools
INCLUDEPATH += ../../ \
    $$ROOT/EksCore \
    $$ROOT/shift \
    $$ROOT/alter2/plugins/UI/ \
    $$ROOT/alter2/plugins/ShiftAlter/

LIBS += -lalter \
    -lEksCore \
    -lUI \
    -lshift \
    -lShiftAlter

SOURCES += scplugin.cpp \
    scsurface.cpp \
    scshiftdynamicpropertyinformation.cpp \
    scshiftproperty.cpp \
    scembeddedtypes.cpp \
    scshiftpropertycontainer.cpp \
    scshiftentity.cpp \
    scshiftdatabase.cpp \
    scshiftfloatarrayproperty.cpp

HEADERS += scplugin.h \
    scglobal.h \
    scsurface.h \
    scshiftdynamicpropertyinformation.h \
    scshiftproperty.h \
    scembeddedtypes.h \
    scwrappedclass.h \
    scshiftpropertycontainer.h \
    scshiftentity.h \
    scshiftdatabase.h \
    scshiftfloatarrayproperty.h

RESOURCES += \
    ScResources.qrc

OTHER_FILES += \
    CoreUtils.js

