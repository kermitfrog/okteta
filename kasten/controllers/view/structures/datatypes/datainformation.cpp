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
#include "datainformation.h"
#include <KLocale>
#include <KLineEdit>
#include "structuredatainformation.h"
#include "uniondatainformation.h"
#include "primitivedatainformation.h"
#include "staticlengtharraydatainformation.h"

DataInformation::DataInformation(const QString& name, int index,
        DataInformation* parent) :
    QObject(parent), mIndex(index)
{
    setObjectName(name);
}

DataInformation::DataInformation(const DataInformation& d) :
    QObject(NULL), mIndex(d.mIndex)
{
    setObjectName(d.objectName());
}

DataInformation::~DataInformation()
{
}

QString DataInformation::valueString() const
{
    return QString();
}

QString DataInformation::sizeString() const
{
    if (size() % 8 == 0) //no bits remaining
    {
        return i18np("1 byte", "%1 bytes", size() / 8);
    }
    else
    {
        //XXX: is this correct?
        //maybe this is better?
        //QString bytes = i18np("1 byte", "%1 bytes", size() / 8);
        //QString bits = i18np("1 bit", "%1 bits", size() % 8);
        //return i18nc("number of bytes, then number of bits", "%1 %2", bytes, bits);
        return i18np("1 byte 1 bit", "%1 bytes %2 bits", size() / 8, size() % 8);
    }
}

quint64 DataInformation::positionRelativeToParent() const
{
    //FIXME this needs updating to support bitfield marking
    DataInformation* par = static_cast<DataInformation*> (parent());
    if (!par)
    {
        return 0;
    }
    return par->offset(this->mIndex) + par->positionRelativeToParent();
}

void DataInformation::setIndex(int newIndex)
{
    mIndex = newIndex;
}
