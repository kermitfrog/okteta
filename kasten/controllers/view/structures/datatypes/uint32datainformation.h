/*
 *   This file is part of the Okteta Kasten module, part of the KDE project.
 *
 *   Copyright 2009 Alex Richardson <alex.richardson@gmx.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef UINT32DATAINFORMATION_H_
#define UINT32DATAINFORMATION_H_

#include "unsignedprimitivedatainformation.h"
#include "../poddecoder/typeeditors/uint32editor.h"

class UInt32DataInformation: public UnsignedPrimitiveDataInformation
{
PRIMITIVEDATAINFORMATION_SUBCLASS_CONSTRUCTORS(UInt32,UnsignedPrimitive)
public:
    inline int getSize() const
    {
        return 32;
    }
    inline QString getTypeName() const
    {
        return i18nc("Data type", "unsigned int");
    }
    DATAINFORMATION_CLONE(UInt32)
    virtual QString getValueString() const;

    inline QWidget* createEditWidget(QWidget* parent) const
    {
        UInt32Editor* ret = new UInt32Editor(parent);
        ret->setBase(displayBase());
        return ret;
    }
};

#endif /* UINT32DATAINFORMATION_H_ */
