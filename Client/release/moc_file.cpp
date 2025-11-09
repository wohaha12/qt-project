/****************************************************************************
** Meta object code from reading C++ file 'file.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../file.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'file.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_File_t {
    QByteArrayData data[15];
    char stringdata0[246];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_File_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_File_t qt_meta_stringdata_File = {
    {
QT_MOC_LITERAL(0, 0, 4), // "File"
QT_MOC_LITERAL(1, 5, 19), // "on_mkDir_PB_clicked"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 23), // "on_flushFile_PB_clicked"
QT_MOC_LITERAL(4, 50, 31), // "on_listWidget_itemDoubleClicked"
QT_MOC_LITERAL(5, 82, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(6, 99, 4), // "item"
QT_MOC_LITERAL(7, 104, 20), // "on_return_PB_clicked"
QT_MOC_LITERAL(8, 125, 20), // "on_mvFile_PB_clicked"
QT_MOC_LITERAL(9, 146, 24), // "on_uploadFile_PB_clicked"
QT_MOC_LITERAL(10, 171, 23), // "on_shareFile_PB_clicked"
QT_MOC_LITERAL(11, 195, 11), // "uploadError"
QT_MOC_LITERAL(12, 207, 8), // "strError"
QT_MOC_LITERAL(13, 216, 20), // "updateUploadProgress"
QT_MOC_LITERAL(14, 237, 8) // "progress"

    },
    "File\0on_mkDir_PB_clicked\0\0"
    "on_flushFile_PB_clicked\0"
    "on_listWidget_itemDoubleClicked\0"
    "QListWidgetItem*\0item\0on_return_PB_clicked\0"
    "on_mvFile_PB_clicked\0on_uploadFile_PB_clicked\0"
    "on_shareFile_PB_clicked\0uploadError\0"
    "strError\0updateUploadProgress\0progress"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_File[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x08 /* Private */,
       3,    0,   60,    2, 0x08 /* Private */,
       4,    1,   61,    2, 0x08 /* Private */,
       7,    0,   64,    2, 0x08 /* Private */,
       8,    0,   65,    2, 0x08 /* Private */,
       9,    0,   66,    2, 0x08 /* Private */,
      10,    0,   67,    2, 0x08 /* Private */,
      11,    1,   68,    2, 0x0a /* Public */,
      13,    1,   71,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::Int,   14,

       0        // eod
};

void File::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<File *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_mkDir_PB_clicked(); break;
        case 1: _t->on_flushFile_PB_clicked(); break;
        case 2: _t->on_listWidget_itemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 3: _t->on_return_PB_clicked(); break;
        case 4: _t->on_mvFile_PB_clicked(); break;
        case 5: _t->on_uploadFile_PB_clicked(); break;
        case 6: _t->on_shareFile_PB_clicked(); break;
        case 7: _t->uploadError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->updateUploadProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject File::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_File.data,
    qt_meta_data_File,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *File::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *File::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_File.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int File::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
