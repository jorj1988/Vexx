# -------------------------------------------------
# Project created by QtCreator 2010-01-16T14:33:07
# -------------------------------------------------
TARGET = EksCore
TEMPLATE = lib

include("../GeneralOptions.pri")

SOURCES += ../src/XObject.cpp \
    ../src/XMessageRouter.cpp \
    ../src/XMessageHandler.cpp \
    ../src/XVariant.cpp \
    ../src/XColour.cpp \
    ../src/XFileSequence.cpp \
    ../src/XChildEvent.cpp \
    ../src/XBaseObject.cpp \
    ../src/XSize.cpp \
    ../src/XCurve.cpp \
    ../src/XMetaType.cpp \
    ../src/XTime.cpp \
    ../src/XProfiler.cpp
HEADERS += ../XObject \
    ../XGlobal \
    ../XVector \
    ../XVariant \
    ../XString \
    ../XSet \
    ../XMap \
    ../XList \
    ../XHash \
    ../XMessageRouter \
    ../XMessageHandler \
    ../XProperty \
    ../XVector2D \
    ../XVector3D \
    ../XVector4D \
    ../XColour \
    ../XQuaternion \
    ../XDebug \
    ../XMatrix4x4 \
    ../XMatrix3x3 \
    ../XGenericMatrix \
    ../XFileSequence \
    ../XChildEvent \
    ../XSignal \
    ../XBaseObject \
    ../XSize \
    ../XCurve \
    ../XShared \
    ../XMetaType \
    ../XFlags \
    ../XRandomAccessAllocator \
    ../XWeakShared \
    ../XTime \
    ../XProfiler
INCLUDEPATH += ../
