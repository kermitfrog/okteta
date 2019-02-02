/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2004,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OKTETA_KTABCONTROLLER_HPP
#define OKTETA_KTABCONTROLLER_HPP

// lib
#include "kcontroller.hpp"

namespace Okteta {
class AbstractByteArrayView;

class KTabController : public KController
{
public:
    KTabController(AbstractByteArrayView* view, KController* parent);

public: // KController API
    bool handleKeyPress(QKeyEvent* keyEvent) override;

public:
    bool tabChangesFocus() const;
    /// default is true
    void setTabChangesFocus(bool tabChangesFocus);

protected:
    AbstractByteArrayView* mView;
    /** flag if tab key should be ignored */
    bool mTabChangesFocus : 1;
};

inline bool KTabController::tabChangesFocus()    const { return mTabChangesFocus; }
inline void KTabController::setTabChangesFocus(bool tabChangesFocus) { mTabChangesFocus = tabChangesFocus; }

}

#endif