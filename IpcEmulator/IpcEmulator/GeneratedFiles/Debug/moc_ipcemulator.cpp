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
    QByteArrayData data[8];
    char stringdata0[117];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IpcEmulator_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IpcEmulator_t qt_meta_stringdata_IpcEmulator = {
    {
QT_MOC_LITERAL(0, 0, 11), // "IpcEmulator"
QT_MOC_LITERAL(1, 12, 18), // "on_playBtn_clicked"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 27), // "on_progressBar_valueChanged"
QT_MOC_LITERAL(4, 60, 5), // "value"
QT_MOC_LITERAL(5, 66, 20), // "on_recordBtn_clicked"
QT_MOC_LITERAL(6, 87, 18), // "on_backBtn_clicked"
QT_MOC_LITERAL(7, 106, 10) // "clearTimer"

    },
    "IpcEmulator\0on_playBtn_clicked\0\0"
    "on_progressBar_valueChanged\0value\0"
    "on_recordBtn_clicked\0on_backBtn_clicked\0"
    "clearTimer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IpcEmulator[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08 /* Private */,
       3,    1,   40,    2, 0x08 /* Private */,
       5,    0,   43,    2, 0x08 /* Private */,
       6,    0,   44,    2, 0x08 /* Private */,
       7,    0,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void IpcEmulator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IpcEmulator *_t = static_cast<IpcEmulator *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_playBtn_clicked(); break;
        case 1: _t->on_progressBar_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_recordBtn_clicked(); break;
        case 3: _t->on_backBtn_clicked(); break;
        case 4: _t->clearTimer(); break;
        default: ;
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
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
