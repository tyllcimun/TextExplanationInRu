include(../textExplanationsInRu/textExplanationsInRu.pri)

QT = core \
    testlib \
    qml \
    xml

SOURCES += \
    main.cpp \
    test_expressiontonodes.cpp \
    test_getexplanation.cpp \
    test_getexplanationinru.cpp \
    test_iscustomtypewithfileds.cpp \
    test_isfunction.cpp \
    test_isidentifier.cpp \
    test_isreducibleunaryselfinverse.cpp \
    test_removeconsecutiveduplicates.cpp \
    test_toexplanation.cpp

HEADERS += \
    test_expressiontonodes.h \
    test_getexplanation.h \
    test_getexplanationinru.h \
    test_iscustomtypewithfileds.h \
    test_isfunction.h \
    test_isidentifier.h \
    test_isreducibleunaryselfinverse.h \
    test_removeconsecutiveduplicates.h \
    test_toexplanation.h

QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -fprofile-arcs -ftest-coverage
