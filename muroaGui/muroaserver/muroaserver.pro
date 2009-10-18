TEMPLATE = app
TARGET = muroaserver
QT += core \
    gui \
    xml \
    network
HEADERS += CCollectionItem.h \
    CCollection.h \
    cconnection.h \
    ccontenthandler.h \
    cnetwork.h \
    cstatemachine.h \
    muroaserver.h
SOURCES += CCollectionItem.cpp \
    CCollection.cpp \
    cconnection.cpp \
    ccontenthandler.cpp \
    cnetwork.cpp \
    cstatemachine.cpp \
    main.cpp \
    muroaserver.cpp
FORMS += muroaserver.ui
RESOURCES += 
