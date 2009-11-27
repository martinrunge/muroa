TEMPLATE = app
TARGET = muroagui
QT += core \
    gui \
    xml \
    network
HEADERS += CItemBase.h \
    CCollectionItem.h \
    CCollection.h \
    cstatemachine.h \
    cconnection.h \
    muroagui.h \
    CCollectionModel.h
SOURCES += CItemBase.cpp \
    CCollectionItem.cpp \
    cstatemachine.cpp \
    cconnection.cpp \
    main.cpp \
    muroagui.cpp \
    CCollectionModel.cpp
FORMS += muroagui.ui
RESOURCES += 
