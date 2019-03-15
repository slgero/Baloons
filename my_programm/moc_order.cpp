/****************************************************************************
** Meta object code from reading C++ file 'order.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Baloons/order.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'order.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Order_t {
    QByteArrayData data[14];
    char stringdata0[258];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Order_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Order_t qt_meta_stringdata_Order = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Order"
QT_MOC_LITERAL(1, 6, 11), // "ChangePrice"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 14), // "on_add_clicked"
QT_MOC_LITERAL(4, 34, 30), // "on_exit_without_saving_clicked"
QT_MOC_LITERAL(5, 65, 14), // "on_del_clicked"
QT_MOC_LITERAL(6, 80, 20), // "on_number_textEdited"
QT_MOC_LITERAL(7, 101, 4), // "arg1"
QT_MOC_LITERAL(8, 106, 24), // "on_save_and_exit_clicked"
QT_MOC_LITERAL(9, 131, 26), // "on_TotalCost_2_textChanged"
QT_MOC_LITERAL(10, 158, 21), // "on_checkBox_2_clicked"
QT_MOC_LITERAL(11, 180, 33), // "on_AdditionalExpenses_textCha..."
QT_MOC_LITERAL(12, 214, 19), // "on_price_textEdited"
QT_MOC_LITERAL(13, 234, 23) // "on_spinBox_valueChanged"

    },
    "Order\0ChangePrice\0\0on_add_clicked\0"
    "on_exit_without_saving_clicked\0"
    "on_del_clicked\0on_number_textEdited\0"
    "arg1\0on_save_and_exit_clicked\0"
    "on_TotalCost_2_textChanged\0"
    "on_checkBox_2_clicked\0"
    "on_AdditionalExpenses_textChanged\0"
    "on_price_textEdited\0on_spinBox_valueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Order[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x08 /* Private */,
       3,    0,   70,    2, 0x08 /* Private */,
       4,    0,   71,    2, 0x08 /* Private */,
       5,    0,   72,    2, 0x08 /* Private */,
       6,    1,   73,    2, 0x08 /* Private */,
       8,    0,   76,    2, 0x08 /* Private */,
       9,    1,   77,    2, 0x08 /* Private */,
      10,    0,   80,    2, 0x08 /* Private */,
      11,    1,   81,    2, 0x08 /* Private */,
      12,    1,   84,    2, 0x08 /* Private */,
      13,    1,   87,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::Int,    7,

       0        // eod
};

void Order::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Order *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ChangePrice(); break;
        case 1: _t->on_add_clicked(); break;
        case 2: _t->on_exit_without_saving_clicked(); break;
        case 3: _t->on_del_clicked(); break;
        case 4: _t->on_number_textEdited((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->on_save_and_exit_clicked(); break;
        case 6: _t->on_TotalCost_2_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->on_checkBox_2_clicked(); break;
        case 8: _t->on_AdditionalExpenses_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->on_price_textEdited((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->on_spinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Order::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_Order.data,
    qt_meta_data_Order,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Order::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Order::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Order.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Order::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
