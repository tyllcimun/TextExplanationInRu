VPATH = $$PWD #Что бы файлы подключались относительно оригинального расположения файла
INCLUDEPATH += $$PWD

QT = core \
     xml \
     qml

CONFIG += c++17 console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        codeentity.cpp \
        expression.cpp \
        expressionnode.cpp \
        expressiontranslator.cpp \
        expressionxmlparser.cpp \
        teexception.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    codeentity.h \
    expression.h \
    expressionnode.h \
    expressiontranslator.h \
    expressionxmlparser.h \
    teexception.h
