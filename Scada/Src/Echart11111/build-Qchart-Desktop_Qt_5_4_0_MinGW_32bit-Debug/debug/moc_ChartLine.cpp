/****************************************************************************
** Meta object code from reading C++ file 'ChartLine.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Qchart/ChartLine.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ChartLine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_EChartTest_t {
    QByteArrayData data[10];
    char stringdata[90];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EChartTest_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EChartTest_t qt_meta_stringdata_EChartTest = {
    {
QT_MOC_LITERAL(0, 0, 10), // "EChartTest"
QT_MOC_LITERAL(1, 11, 8), // "getXdata"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 8), // "setXdata"
QT_MOC_LITERAL(4, 30, 8), // "getYdata"
QT_MOC_LITERAL(5, 39, 8), // "setYdata"
QT_MOC_LITERAL(6, 48, 8), // "getTitle"
QT_MOC_LITERAL(7, 57, 8), // "setTitle"
QT_MOC_LITERAL(8, 66, 11), // "getSubTitle"
QT_MOC_LITERAL(9, 78, 11) // "setSubTitle"

    },
    "EChartTest\0getXdata\0\0setXdata\0getYdata\0"
    "setYdata\0getTitle\0setTitle\0getSubTitle\0"
    "setSubTitle"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EChartTest[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x0a /* Public */,
       3,    0,   55,    2, 0x0a /* Public */,
       4,    0,   56,    2, 0x0a /* Public */,
       5,    0,   57,    2, 0x0a /* Public */,
       6,    0,   58,    2, 0x0a /* Public */,
       7,    0,   59,    2, 0x0a /* Public */,
       8,    0,   60,    2, 0x0a /* Public */,
       9,    0,   61,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::QStringList,
    QMetaType::Void,
    QMetaType::QStringList,
    QMetaType::Void,
    QMetaType::QString,
    QMetaType::Void,
    QMetaType::QString,
    QMetaType::Void,

       0        // eod
};

void EChartTest::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        EChartTest *_t = static_cast<EChartTest *>(_o);
        switch (_id) {
        case 0: { QStringList _r = _t->getXdata();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = _r; }  break;
        case 1: _t->setXdata(); break;
        case 2: { QStringList _r = _t->getYdata();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = _r; }  break;
        case 3: _t->setYdata(); break;
        case 4: { QString _r = _t->getTitle();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 5: _t->setTitle(); break;
        case 6: { QString _r = _t->getSubTitle();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 7: _t->setSubTitle(); break;
        default: ;
        }
    }
}

const QMetaObject EChartTest::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_EChartTest.data,
      qt_meta_data_EChartTest,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *EChartTest::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EChartTest::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_EChartTest.stringdata))
        return static_cast<void*>(const_cast< EChartTest*>(this));
    return QObject::qt_metacast(_clname);
}

int EChartTest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
