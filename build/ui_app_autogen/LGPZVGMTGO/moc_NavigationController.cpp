/****************************************************************************
** Meta object code from reading C++ file 'NavigationController.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../ui/controllers/NavigationController.hpp"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NavigationController.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_NavigationController_t {
    uint offsetsAndSizes[30];
    char stringdata0[21];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[35];
    char stringdata4[6];
    char stringdata5[17];
    char stringdata6[8];
    char stringdata7[27];
    char stringdata8[13];
    char stringdata9[20];
    char stringdata10[6];
    char stringdata11[12];
    char stringdata12[29];
    char stringdata13[33];
    char stringdata14[13];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_NavigationController_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_NavigationController_t qt_meta_stringdata_NavigationController = {
    {
        QT_MOC_LITERAL(0, 20),  // "NavigationController"
        QT_MOC_LITERAL(21, 10),  // "routeReady"
        QT_MOC_LITERAL(32, 0),  // ""
        QT_MOC_LITERAL(33, 34),  // "application::RoutePresentatio..."
        QT_MOC_LITERAL(68, 5),  // "route"
        QT_MOC_LITERAL(74, 16),  // "navigationFailed"
        QT_MOC_LITERAL(91, 7),  // "message"
        QT_MOC_LITERAL(99, 26),  // "multiDestinationRouteReady"
        QT_MOC_LITERAL(126, 12),  // "requestRoute"
        QT_MOC_LITERAL(139, 19),  // "core::types::NodeId"
        QT_MOC_LITERAL(159, 5),  // "start"
        QT_MOC_LITERAL(165, 11),  // "destination"
        QT_MOC_LITERAL(177, 28),  // "requestMultiDestinationRoute"
        QT_MOC_LITERAL(206, 32),  // "std::vector<core::types::NodeId>"
        QT_MOC_LITERAL(239, 12)   // "destinations"
    },
    "NavigationController",
    "routeReady",
    "",
    "application::RoutePresentationData",
    "route",
    "navigationFailed",
    "message",
    "multiDestinationRouteReady",
    "requestRoute",
    "core::types::NodeId",
    "start",
    "destination",
    "requestMultiDestinationRoute",
    "std::vector<core::types::NodeId>",
    "destinations"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_NavigationController[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   44,    2, 0x06,    1 /* Public */,
       5,    1,   47,    2, 0x06,    3 /* Public */,
       7,    1,   50,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    2,   53,    2, 0x0a,    7 /* Public */,
      12,    2,   58,    2, 0x0a,   10 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 9,   10,   11,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 13,   10,   14,

       0        // eod
};

Q_CONSTINIT const QMetaObject NavigationController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_NavigationController.offsetsAndSizes,
    qt_meta_data_NavigationController,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_NavigationController_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<NavigationController, std::true_type>,
        // method 'routeReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const application::RoutePresentationData &, std::false_type>,
        // method 'navigationFailed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'multiDestinationRouteReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const application::RoutePresentationData &, std::false_type>,
        // method 'requestRoute'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<core::types::NodeId, std::false_type>,
        QtPrivate::TypeAndForceComplete<core::types::NodeId, std::false_type>,
        // method 'requestMultiDestinationRoute'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<core::types::NodeId, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<core::types::NodeId> &, std::false_type>
    >,
    nullptr
} };

void NavigationController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NavigationController *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->routeReady((*reinterpret_cast< std::add_pointer_t<application::RoutePresentationData>>(_a[1]))); break;
        case 1: _t->navigationFailed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->multiDestinationRouteReady((*reinterpret_cast< std::add_pointer_t<application::RoutePresentationData>>(_a[1]))); break;
        case 3: _t->requestRoute((*reinterpret_cast< std::add_pointer_t<core::types::NodeId>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<core::types::NodeId>>(_a[2]))); break;
        case 4: _t->requestMultiDestinationRoute((*reinterpret_cast< std::add_pointer_t<core::types::NodeId>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::vector<core::types::NodeId>>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NavigationController::*)(const application::RoutePresentationData & );
            if (_t _q_method = &NavigationController::routeReady; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NavigationController::*)(const QString & );
            if (_t _q_method = &NavigationController::navigationFailed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NavigationController::*)(const application::RoutePresentationData & );
            if (_t _q_method = &NavigationController::multiDestinationRouteReady; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *NavigationController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NavigationController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NavigationController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NavigationController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void NavigationController::routeReady(const application::RoutePresentationData & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NavigationController::navigationFailed(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NavigationController::multiDestinationRouteReady(const application::RoutePresentationData & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
