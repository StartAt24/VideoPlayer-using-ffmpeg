/****************************************************************************
** Meta object code from reading C++ file 'ipcemulator.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ipcemulator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ipcemulator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_IpcEmulator_t {
    QByteArrayData data[11];
    char stringdata0[172];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IpcEmulator_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IpcEmulator_t qt_meta_stringdata_IpcEmulator = {
    {
QT_MOC_LITERAL(0, 0, 11), // "IpcEmulator"
QT_MOC_LITERAL(1, 12, 13), // "notifyRefresh"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 18), // "on_playBtn_clicked"
QT_MOC_LITERAL(4, 46, 29), // "on_progressBar_sliderReleased"
QT_MOC_LITERAL(5, 76, 20), // "on_recordBtn_clicked"
QT_MOC_LITERAL(6, 97, 18), // "on_backBtn_clicked"
QT_MOC_LITERAL(7, 116, 10), // "clearTimer"
QT_MOC_LITERAL(8, 127, 28), // "on_progressBar_sliderPressed"
QT_MOC_LITERAL(9, 156, 11), // "receive_Sec"
QT_MOC_LITERAL(10, 168, 3) // "sec"

    },
    "IpcEmulator\0notifyRefresh\0\0"
    "on_playBtn_clicked\0on_progressBar_sliderReleased\0"
    "on_recordBtn_clicked\0on_backBtn_clicked\0"
    "clearTimer\0on_progressBar_sliderPressed\0"
    "receive_Sec\0sec"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IpcEmulator[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   57,    2, 0x08 /* Private */,
       4,    0,   58,    2, 0x08 /* Private */,
       5,    0,   59,    2, 0x08 /* Private */,
       6,    0,   60,    2, 0x08 /* Private */,
       7,    0,   61,    2, 0x08 /* Private */,
       8,    0,   62,    2, 0x08 /* Private */,
       9,    1,   63,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,

       0        // eod
};

void IpcEmulator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IpcEmulator *_t = static_cast<IpcEmulator *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->notifyRefresh((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->on_playBtn_clicked(); break;
        case 2: _t->on_progressBar_sliderReleased(); break;
        case 3: _t->on_recordBtn_clicked(); break;
        case 4: _t->on_backBtn_clicked(); break;
        case 5: _t->clearTimer(); break;
        case 6: _t->on_progressBar_sliderPressed(); break;
        case 7: _t->receive_Sec((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (IpcEmulator::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IpcEmulator::notifyRefresh)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject IpcEmulator::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_IpcEmulator.data,
      qt_meta_data_IpcEmulator,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *IpcEmulator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IpcEmulator::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_IpcEmulator.stringdata0))
        return static_cast<void*>(const_cast< IpcEmulator*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int IpcEmulator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void IpcEmulator::notifyRefresh(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
