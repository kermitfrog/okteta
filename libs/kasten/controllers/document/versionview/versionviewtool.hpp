/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef VERSIONVIEWTOOL_HPP
#define VERSIONVIEWTOOL_HPP

// lib
#include <kasten/kastencontrollers_export.hpp>
// Kasten core
#include <kasten/abstracttool.hpp>

namespace Kasten {

/**
*/
class KASTENCONTROLLERS_EXPORT VersionViewTool : public AbstractTool
{
    Q_OBJECT

public:
    VersionViewTool();
    ~VersionViewTool() override;

public:
    AbstractModel* model() const;

public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    QString title() const override;

    void setTargetModel(AbstractModel* model) override;

Q_SIGNALS:
    void modelChanged(Kasten::AbstractModel* model);

private: // sources
    AbstractModel* mModel = nullptr;
};

inline AbstractModel* VersionViewTool::model() const { return mModel; }

}

#endif