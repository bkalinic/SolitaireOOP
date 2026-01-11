/****************************************************************************
** Meta object code from reading C++ file 'GameManager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/guiHeader/GameManager.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GameManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN11GameManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto GameManager::qt_create_metaobjectdata<qt_meta_tag_ZN11GameManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GameManager",
        "cardDrawn",
        "",
        "CardPtr",
        "card",
        "stockEmpty",
        "wasteReset",
        "stockRefilled",
        "newSize",
        "wasteUpdated",
        "wasteSize",
        "stockUpdated",
        "stockSize",
        "gameStateChanged",
        "scoreUpdated",
        "score",
        "movesUpdated",
        "moves",
        "gameWon",
        "cardsDrawn",
        "std::vector<CardPtr>",
        "cards",
        "drawFailed",
        "reason",
        "undoAvailable",
        "available",
        "redoAvailable",
        "cardSelected",
        "cardDeselected"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'cardDrawn'
        QtMocHelpers::SignalData<void(CardPtr)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'stockEmpty'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'wasteReset'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'stockRefilled'
        QtMocHelpers::SignalData<void(int)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 8 },
        }}),
        // Signal 'wasteUpdated'
        QtMocHelpers::SignalData<void(int)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 10 },
        }}),
        // Signal 'stockUpdated'
        QtMocHelpers::SignalData<void(int)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 12 },
        }}),
        // Signal 'gameStateChanged'
        QtMocHelpers::SignalData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'scoreUpdated'
        QtMocHelpers::SignalData<void(int)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 15 },
        }}),
        // Signal 'movesUpdated'
        QtMocHelpers::SignalData<void(int)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 17 },
        }}),
        // Signal 'gameWon'
        QtMocHelpers::SignalData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'cardsDrawn'
        QtMocHelpers::SignalData<void(const std::vector<CardPtr> &)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 20, 21 },
        }}),
        // Signal 'drawFailed'
        QtMocHelpers::SignalData<void(const QString &)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 23 },
        }}),
        // Signal 'undoAvailable'
        QtMocHelpers::SignalData<void(bool)>(24, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 25 },
        }}),
        // Signal 'redoAvailable'
        QtMocHelpers::SignalData<void(bool)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 25 },
        }}),
        // Signal 'cardSelected'
        QtMocHelpers::SignalData<void(CardPtr)>(27, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'cardDeselected'
        QtMocHelpers::SignalData<void()>(28, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<GameManager, qt_meta_tag_ZN11GameManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject GameManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11GameManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11GameManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11GameManagerE_t>.metaTypes,
    nullptr
} };

void GameManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GameManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->cardDrawn((*reinterpret_cast<std::add_pointer_t<CardPtr>>(_a[1]))); break;
        case 1: _t->stockEmpty(); break;
        case 2: _t->wasteReset(); break;
        case 3: _t->stockRefilled((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->wasteUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->stockUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->gameStateChanged(); break;
        case 7: _t->scoreUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->movesUpdated((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->gameWon(); break;
        case 10: _t->cardsDrawn((*reinterpret_cast<std::add_pointer_t<std::vector<CardPtr>>>(_a[1]))); break;
        case 11: _t->drawFailed((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 12: _t->undoAvailable((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 13: _t->redoAvailable((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 14: _t->cardSelected((*reinterpret_cast<std::add_pointer_t<CardPtr>>(_a[1]))); break;
        case 15: _t->cardDeselected(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (GameManager::*)(CardPtr )>(_a, &GameManager::cardDrawn, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameManager::*)()>(_a, &GameManager::stockEmpty, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameManager::*)()>(_a, &GameManager::wasteReset, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameManager::*)(int )>(_a, &GameManager::stockRefilled, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameManager::*)(int )>(_a, &GameManager::wasteUpdated, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameManager::*)(int )>(_a, &GameManager::stockUpdated, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameManager::*)()>(_a, &GameManager::gameStateChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameManager::*)(int )>(_a, &GameManager::scoreUpdated, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameManager::*)(int )>(_a, &GameManager::movesUpdated, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameManager::*)()>(_a, &GameManager::gameWon, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameManager::*)(const std::vector<CardPtr> & )>(_a, &GameManager::cardsDrawn, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameManager::*)(const QString & )>(_a, &GameManager::drawFailed, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameManager::*)(bool )>(_a, &GameManager::undoAvailable, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameManager::*)(bool )>(_a, &GameManager::redoAvailable, 13))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameManager::*)(CardPtr )>(_a, &GameManager::cardSelected, 14))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameManager::*)()>(_a, &GameManager::cardDeselected, 15))
            return;
    }
}

const QMetaObject *GameManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GameManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11GameManagerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GameManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void GameManager::cardDrawn(CardPtr _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void GameManager::stockEmpty()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void GameManager::wasteReset()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void GameManager::stockRefilled(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void GameManager::wasteUpdated(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void GameManager::stockUpdated(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void GameManager::gameStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void GameManager::scoreUpdated(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}

// SIGNAL 8
void GameManager::movesUpdated(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}

// SIGNAL 9
void GameManager::gameWon()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void GameManager::cardsDrawn(const std::vector<CardPtr> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1);
}

// SIGNAL 11
void GameManager::drawFailed(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 11, nullptr, _t1);
}

// SIGNAL 12
void GameManager::undoAvailable(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 12, nullptr, _t1);
}

// SIGNAL 13
void GameManager::redoAvailable(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 13, nullptr, _t1);
}

// SIGNAL 14
void GameManager::cardSelected(CardPtr _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 14, nullptr, _t1);
}

// SIGNAL 15
void GameManager::cardDeselected()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}
QT_WARNING_POP
