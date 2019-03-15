/****************************************************************************
** Meta object code from reading C++ file 'myorders.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Baloons/myorders.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'myorders.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MyOrders_t {
    QByteArrayData data[11];
    char stringdata0[185];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyOrders_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyOrders_t qt_meta_stringdata_MyOrders = {
    {
QT_MOC_LITERAL(0, 0, 8), // "MyOrders"
QT_MOC_LITERAL(1, 9, 16), // "on_right_clicked"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 15), // "on_left_clicked"
QT_MOC_LITERAL(4, 43, 20), // "on_PBgo_away_clicked"
QT_MOC_LITERAL(5, 64, 19), // "on_PBcancel_clicked"
QT_MOC_LITERAL(6, 84, 21), // "on_PBcomplete_clicked"
QT_MOC_LITERAL(7, 106, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(8, 128, 23), // "on_checkBoxDate_clicked"
QT_MOC_LITERAL(9, 152, 27), // "on_dateEdit_userDateChanged"
QT_MOC_LITERAL(10, 180, 4) // "date"

    },
    "MyOrders\0on_right_clicked\0\0on_left_clicked\0"
    "on_PBgo_away_clicked\0on_PBcancel_clicked\0"
    "on_PBcomplete_clicked\0on_pushButton_clicked\0"
    "on_checkBoxDate_clicked\0"
    "on_dateEdit_userDateChanged\0date"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyOrders[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    1,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QDate,   10,

       0        // eod
};

void MyOrders::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MyOrders *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_right_clicked(); break;
        case 1: _t->on_left_clicked(); break;
        case 2: _t->on_PBgo_away_clicked(); break;
        case 3: _t->on_PBcancel_clicked(); break;
        case 4: _t->on_PBcomplete_clicked(); break;
        case 5: _t->on_pushButton_clicked(); break;
        case 6: _t->on_checkBoxDate_clicked(); break;
        case 7: _t->on_dateEdit_userDateChanged((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MyOrders::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_MyOrders.data,
    qt_meta_data_MyOrders,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MyOrders::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyOrders::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MyOrders.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int MyOrders::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
