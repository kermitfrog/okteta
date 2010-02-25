/*
 *   This file is part of the Okteta Kasten Framework, part of the KDE project.
 *
 *   Copyright 2009, 2010 Alex Richardson <alex.richardson@gmx.de>
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
#ifndef INT32DATAINFORMATION_H_
#define INT32DATAINFORMATION_H_

#include "signedprimitivedatainformation.h"
#include "../poddecoder/typeeditors/sint32editor.h"

class Int32DataInformation: public SignedPrimitiveDataInformation
{
Q_OBJECT
PRIMITIVEDATAINFORMATION_SUBCLASS_CONSTRUCTORS(Int32,SignedPrimitive)

public:
    inline int size() const
    {
        return 32;
    }
    inline QString typeName() const
    {
        return i18nc("Data type", "int");
    }
    DATAINFORMATION_CLONE(Int32)
    virtual QString valueString() const;

    virtual void setWidgetData(QWidget* w) const;

    inline QWidget* createEditWidget(QWidget* parent) const
    {
        SInt32Editor* ret = new SInt32Editor(parent);
        ret->setBase(displayBase());
        return ret;
    }
};

#endif /* INT32DATAINFORMATION_H_ */
