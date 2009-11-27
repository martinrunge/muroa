TEMPLATE = app
TARGET = muroagui
QT += core \
    gui \
    xml \
    network
HEADERS += CCollectionItem.h \
    CCollection.h \
    cstatemachine.h \
    cconnection.h \
    muroagui.h \
    CCollectionModel.h \
    CItemBase.h
SOURCES += CCollectionItem.cpp \
    CCollection.cpp \
    cstatemachine.cpp \
    cconnection.cpp \
    main.cpp \
    muroagui.cpp \
    CCollectionModel.cpp \
    CItemBase.cpp
FORMS += muroagui.ui
RESOURCES += 
