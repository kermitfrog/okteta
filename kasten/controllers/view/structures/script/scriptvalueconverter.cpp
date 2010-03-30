/*
 *   This file is part of the Okteta Kasten Framework, part of the KDE project.
 *
 *   Copyright 2010 Alex Richardson <alex.richardson@gmx.de>
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

#include "scriptvalueconverter.h"

#include <KDebug>
#include <QtScript/QScriptValueList>
#include <QtScript/QScriptValueIterator>
#include <QtCore/QScopedPointer>

#include "../datatypes/staticlengtharraydatainformation.h"
#include "../datatypes/uniondatainformation.h"
#include "../datatypes/structuredatainformation.h"
#include "../datatypes/enumdatainformation.h"
#include "../datatypes/boolbitfielddatainformation.h"
#include "../datatypes/unsignedbitfielddatainformation.h"
#include "../datatypes/signedbitfielddatainformation.h"
#include "../datatypes/primitivedatainformation.h"
#include "scriptutils.h"

ScriptValueConverter::ScriptValueConverter(QScriptValue& value,
        QScriptEngine& engine, QString name) :
    mValue(value), mEngine(engine), mName(name)
{
}

ScriptValueConverter::~ScriptValueConverter()
{
}

DataInformation* ScriptValueConverter::convert()
{
    QScriptValue evaluatedVal;
    if (mValue.isFunction())
    {
        QScriptValue thisObj = mEngine.newObject();
        QScriptValueList args;
        evaluatedVal = mValue.call(thisObj, args);
    }

    else if (mValue.isObject())
        evaluatedVal = mValue;
    else
    {
        kWarning() << "value is neither function nor object";
        return NULL;
    }
    //    dumpQScriptValue(evaluatedVal,__FILE__,__LINE__);
    return toDataInformation(evaluatedVal, mName); //could be NULL
}
DataInformation* ScriptValueConverter::toDataInformation(QScriptValue& value,
        QString name) const
{
    DataInformation* returnVal;
    if (!value.isObject())
        return NULL; //no point trying to convert

    if (name.isEmpty())
        name = i18n("<no name specified>");

    QString type = value.property("type").toString();

    if (type == QLatin1String("array"))
        returnVal = toArray(value, name);

    else if (type == QLatin1String("struct"))
        returnVal = toStruct(value, name);

    else if (type == QLatin1String("union"))
        returnVal = toUnion(value, name);

    else if (type == QLatin1String("bitfield"))
        returnVal = toBitfield(value, name);

    //    else if (type == QLatin1String("enum"))
    //        returnVal = toEnum(value, valueName);
    //TODO enums, they aren't implemented yet

    //now it can only be a primitive type or something invalid

    else
        returnVal = toPrimitive(value, name);

    return returnVal;
}

AbstractArrayDataInformation * ScriptValueConverter::toArray(QScriptValue& value,
        QString& name) const
{
    //we can safely assume that type == "array"
    int length = value.property("length").toInt32();
    if (length <= 0)
    {
        kWarning() << "array length <= 0 -> return NULL";
        return NULL;
    }
    QScriptValue childType = value.property("childType");
    QScopedPointer<DataInformation> arrayType(toDataInformation(childType,
            childType.property("type").toString()));
    if (!arrayType)
    {
        kWarning() << "could not parse array type -> return NULL";
        return NULL;
    }

    return new StaticLengthArrayDataInformation(name, length, *arrayType);

}

AbstractBitfieldDataInformation* ScriptValueConverter::toBitfield(
        QScriptValue& value, QString& name) const
{
    //we can safely assume that type == "bitfield"
    int width = value.property("width").toInt32();
    if (width <= 0)
    {
        kWarning() << "bitfield has a length of <= 0 -> return NULL";
        return NULL;
    }
    QString bitfieldType = value.property("bitfieldType").toString();
    if (bitfieldType.toLower() == QLatin1String("bool"))
        return new BoolBitfieldDataInformation(name, Type_Bitfield, width);

    else if (bitfieldType.toLower() == QLatin1String("bool"))
        return new BoolBitfieldDataInformation(name, Type_Bitfield, width);

    else if (bitfieldType.toLower() == QLatin1String("bool"))
        return new BoolBitfieldDataInformation(name, Type_Bitfield, width);

    kWarning() << "invalid bitfield type specified:" << bitfieldType;
    return NULL;
}

PrimitiveDataInformation * ScriptValueConverter::toPrimitive(QScriptValue& value,
        QString& name) const
{
    QString typeString = value.property("type").toString();
    PrimitiveDataType primitiveType = PrimitiveDataInformation::typeStringToType(
            typeString);
    if (primitiveType == Type_NotPrimitive)
    {
        kWarning() << "unrecognised type: '" << typeString << "' -> return NULL";
        return NULL;
    }
    return PrimitiveDataInformation::newInstance(name, primitiveType);
}

StructureDataInformation* ScriptValueConverter::toStruct(QScriptValue& value,
        QString& name) const
{
    QList<DataInformation*> fields;

    QScriptValue valueChildren = value.property("children");
    QScriptValueIterator it(valueChildren);
    while (it.hasNext())
    {
        it.next();
        QScriptValue val = it.value();
        if (!val.isObject())
            continue;
        DataInformation* data = toDataInformation(val, it.name());
        if (data)
            fields.append(data);
    }
    if (fields.length() == 0)
    {
        kWarning() << "no children were found in element '" << name
                << "' -> return NULL";
        return NULL;
    }

    StructureDataInformation* structure = new StructureDataInformation(name);

    foreach(DataInformation* data, fields)
        {
            structure->addDataTypeToStruct(data);
        }
    return structure;
}

UnionDataInformation* ScriptValueConverter::toUnion(QScriptValue& value,
        QString& name) const
{
    QList<DataInformation*> fields;

    QScriptValue valueChildren = value.property("children");
    QScriptValueIterator it(valueChildren);
    while (it.hasNext())
    {
        it.next();
        QScriptValue val = it.value();
        if (!val.isObject())
            continue;
        DataInformation* data = toDataInformation(val, it.name());
        if (data)
            fields.append(data);
    }
    if (fields.length() == 0)
    {
        kWarning() << "no children were found in element '" << name
                << "' -> return NULL";
        return NULL;
    }

    UnionDataInformation* unionInf = new UnionDataInformation(name);

    foreach(DataInformation* data, fields)
        {
            unionInf->addDataTypeToUnion(data);
        }
    return unionInf;
}

//    EnumDataInformation* ScriptValueConverter::toEnum(QScriptValue& value, QString& name) const{}
