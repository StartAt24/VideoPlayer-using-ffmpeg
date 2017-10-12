/****************************************************************************
** Meta object code from reading C++ file 'logindlg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../logindlg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'logindlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CLoginDlg_t {
    QByteArrayData data[10];
    char stringdata0[164];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CLoginDlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CLoginDlg_t qt_meta_stringdata_CLoginDlg = {
    {
QT_MOC_LITERAL(0, 0, 9), // "CLoginDlg"
QT_MOC_LITERAL(1, 10, 10), // "logSuccess"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 18), // "on_openBtn_clicked"
QT_MOC_LITERAL(4, 41, 20), // "on_cameraBtn_clicked"
QT_MOC_LITERAL(5, 62, 19), // "on_videoBtn_clicked"
QT_MOC_LITERAL(6, 82, 18), // "onButtonMinClicked"
QT_MOC_LITERAL(7, 101, 22), // "onButtonRestoreClicked"
QT_MOC_LITERAL(8, 124, 18), // "onButtonMaxClicked"
QT_MOC_LITERAL(9, 143, 20) // "onButtonCloseClicked"

    },
    "CLoginDlg\0logSuccess\0\0on_openBtn_clicked\0"
    "on_cameraBtn_clicked\0on_videoBtn_clicked\0"
    "onButtonMinClicked\0onButtonRestoreClicked\0"
    "onButtonMaxClicked\0onButtonCloseClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CLoginDlg[] = {

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
       1,    0,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    0,   61,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CLoginDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CLoginDlg *_t = static_cast<CLoginDlg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->logSuccess(); break;
        case 1: _t->on_openBtn_clicked(); break;
        case 2: _t->on_cameraBtn_clicked(); break;
        case 3: _t->on_videoBtn_clicked(); break;
        case 4: _t->onButtonMinClicked(); break;
        case 5: _t->onButtonRestoreClicked(); break;
        case 6: _t->onButtonMaxClicked(); break;
        case 7: _t->onButtonCloseClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CLoginDlg::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CLoginDlg::logSuccess)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CLoginDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CLoginDlg.data,
      qt_meta_data_CLoginDlg,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CLoginDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CLoginDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CLoginDlg.stringdata0))
        return static_cast<void*>(const_cast< CLoginDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int CLoginDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void CLoginDlg::logSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
