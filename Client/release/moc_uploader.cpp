/****************************************************************************
** Meta object code from reading C++ file 'uploader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../uploader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'uploader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Uploader_t {
    QByteArrayData data[16];
    char stringdata0[167];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Uploader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Uploader_t qt_meta_stringdata_Uploader = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Uploader"
QT_MOC_LITERAL(1, 9, 8), // "finished"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 11), // "errorMsgBox"
QT_MOC_LITERAL(4, 31, 8), // "strError"
QT_MOC_LITERAL(5, 40, 9), // "uploadPDU"
QT_MOC_LITERAL(6, 50, 4), // "PDU*"
QT_MOC_LITERAL(7, 55, 3), // "pdu"
QT_MOC_LITERAL(8, 59, 14), // "uploadProgress"
QT_MOC_LITERAL(9, 74, 8), // "progress"
QT_MOC_LITERAL(10, 83, 13), // "sliceUploaded"
QT_MOC_LITERAL(11, 97, 10), // "sliceIndex"
QT_MOC_LITERAL(12, 108, 10), // "uploadFile"
QT_MOC_LITERAL(13, 119, 11), // "uploadSlice"
QT_MOC_LITERAL(14, 131, 15), // "onSliceUploaded"
QT_MOC_LITERAL(15, 147, 19) // "checkUploadComplete"

    },
    "Uploader\0finished\0\0errorMsgBox\0strError\0"
    "uploadPDU\0PDU*\0pdu\0uploadProgress\0"
    "progress\0sliceUploaded\0sliceIndex\0"
    "uploadFile\0uploadSlice\0onSliceUploaded\0"
    "checkUploadComplete"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Uploader[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    1,   60,    2, 0x06 /* Public */,
       5,    1,   63,    2, 0x06 /* Public */,
       8,    1,   66,    2, 0x06 /* Public */,
      10,    1,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   72,    2, 0x0a /* Public */,
      13,    1,   73,    2, 0x0a /* Public */,
      14,    1,   76,    2, 0x08 /* Private */,
      15,    0,   79,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,   11,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,

       0        // eod
};

void Uploader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Uploader *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->errorMsgBox((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->uploadPDU((*reinterpret_cast< PDU*(*)>(_a[1]))); break;
        case 3: _t->uploadProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->sliceUploaded((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->uploadFile(); break;
        case 6: _t->uploadSlice((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->onSliceUploaded((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->checkUploadComplete(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Uploader::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Uploader::finished)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Uploader::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Uploader::errorMsgBox)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Uploader::*)(PDU * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Uploader::uploadPDU)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Uploader::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Uploader::uploadProgress)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Uploader::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Uploader::sliceUploaded)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Uploader::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Uploader.data,
    qt_meta_data_Uploader,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Uploader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Uploader::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Uploader.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Uploader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void Uploader::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Uploader::errorMsgBox(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Uploader::uploadPDU(PDU * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Uploader::uploadProgress(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Uploader::sliceUploaded(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
