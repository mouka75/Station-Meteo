/****************************************************************************
** Meta object code from reading C++ file 'client.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "client.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'client.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_client[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       7,   21,   21,   21, 0x08,
      22,   21,   21,   21, 0x08,
      38,   21,   21,   21, 0x08,
      49,   21,   21,   21, 0x08,
      62,  105,   21,   21, 0x08,
     112,   21,   21,   21, 0x08,
     120,   21,   21,   21, 0x08,
     145,   21,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_client[] = {
    "client\0messageRTDM()\0\0donneesRecues()\0"
    "connecte()\0deconnecte()\0"
    "erreurSocket(QAbstractSocket::SocketError)\0"
    "erreur\0start()\0acquisitionTemperature()\0"
    "getTemperature()\0"
};

void client::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        client *_t = static_cast<client *>(_o);
        switch (_id) {
        case 0: _t->messageRTDM(); break;
        case 1: _t->donneesRecues(); break;
        case 2: _t->connecte(); break;
        case 3: _t->deconnecte(); break;
        case 4: _t->erreurSocket((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 5: _t->start(); break;
        case 6: _t->acquisitionTemperature(); break;
        case 7: _t->getTemperature(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData client::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject client::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_client,
      qt_meta_data_client, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &client::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *client::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *client::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_client))
        return static_cast<void*>(const_cast< client*>(this));
    return QObject::qt_metacast(_clname);
}

int client::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
