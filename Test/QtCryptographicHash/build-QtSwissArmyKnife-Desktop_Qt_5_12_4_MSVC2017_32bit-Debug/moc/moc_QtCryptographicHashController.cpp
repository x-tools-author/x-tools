/****************************************************************************
** Meta object code from reading C++ file 'QtCryptographicHashController.hh'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Modules/QtCryptographicHash/src/QtCryptographicHashController.hh"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtCryptographicHashController.hh' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QtCryptographicHashController_t {
    QByteArrayData data[7];
    char stringdata0[160];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtCryptographicHashController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtCryptographicHashController_t qt_meta_stringdata_QtCryptographicHashController = {
    {
QT_MOC_LITERAL(0, 0, 29), // "QtCryptographicHashController"
QT_MOC_LITERAL(1, 30, 25), // "on_openPushButton_clicked"
QT_MOC_LITERAL(2, 56, 0), // ""
QT_MOC_LITERAL(3, 57, 40), // "on_algorithmComboBox_currentI..."
QT_MOC_LITERAL(4, 98, 5), // "index"
QT_MOC_LITERAL(5, 104, 30), // "on_startStopPushButton_clicked"
QT_MOC_LITERAL(6, 135, 24) // "on_upperCheckBox_clicked"

    },
    "QtCryptographicHashController\0"
    "on_openPushButton_clicked\0\0"
    "on_algorithmComboBox_currentIndexChanged\0"
    "index\0on_startStopPushButton_clicked\0"
    "on_upperCheckBox_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtCryptographicHashController[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    1,   35,    2, 0x08 /* Private */,
       5,    0,   38,    2, 0x08 /* Private */,
       6,    0,   39,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QtCryptographicHashController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QtCryptographicHashController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_openPushButton_clicked(); break;
        case 1: _t->on_algorithmComboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_startStopPushButton_clicked(); break;
        case 3: _t->on_upperCheckBox_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QtCryptographicHashController::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_QtCryptographicHashController.data,
    qt_meta_data_QtCryptographicHashController,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QtCryptographicHashController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtCryptographicHashController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QtCryptographicHashController.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int QtCryptographicHashController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
