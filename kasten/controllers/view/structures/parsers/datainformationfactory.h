/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2012 Alex Richardson <alex.richardson@gmx.de>
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

#ifndef DATAINFORMATIONFACTORY_H_
#define DATAINFORMATIONFACTORY_H_

#include <QScriptValue>

#include "../datatypes/primitive/bitfield/abstractbitfielddatainformation.h"
#include "../datatypes/primitive/primitivedatainformation.h"
#include "../datatypes/primitive/enumdefinition.h"
#include "../datatypes/primitive/flagdatainformation.h"
#include "../datatypes/primitive/enumdatainformation.h"
#include "../datatypes/array/arraydatainformation.h"

#include "../script/scriptlogger.h"
#include "abstractstructureparser.h" //TODO remove

struct CommonParsedData : public ParserInfo {
    inline explicit CommonParsedData(const ParserInfo& i, QScriptEngine* engine)
        : ParserInfo(i), engine(engine), endianess(DataInformation::EndianessInherit) {}
    QScriptEngine* engine;
    QScriptValue updateFunc;
    /** if this is not empty it is evaluated */
    QString updateFuncString;
    QScriptValue validationFunc;
    /** if this is not empty it is evaluated */
    QString validationFuncString;
    DataInformation::DataInformationEndianess endianess;
private:
    Q_DISABLE_COPY(CommonParsedData);
};

struct BitfieldParsedData : public ParserInfo {
    inline explicit BitfieldParsedData(const ParserInfo& i) : ParserInfo(i), widthConversionOkay(false), width(-1) {}
    QString type;
    QString widthStr;
    bool widthConversionOkay;
    int width;
private:
    Q_DISABLE_COPY(BitfieldParsedData);
};

struct PrimitiveParsedData : public ParserInfo {
    inline explicit PrimitiveParsedData(const ParserInfo& i) : ParserInfo(i) {}
    QString type;
private:
    Q_DISABLE_COPY(PrimitiveParsedData);
};

struct EnumParsedData : public ParserInfo {
    inline explicit EnumParsedData(const ParserInfo& i) : ParserInfo(i) {}
    QString type;
    QString enumName;
    EnumDefinition::Ptr enumDef; //TODO QMap<QString, QScriptValue> instead
    /** only used if enumDef is null, to allow sharing (only possible in OSD) */
    QScriptValue enumValuesObject;
private:
    Q_DISABLE_COPY(EnumParsedData);
};

struct ArrayParsedData : public ParserInfo {
    inline explicit ArrayParsedData(const ParserInfo& i) : ParserInfo(i), length(-1), arrayType(0) {}
    int length;
    QScriptValue lengthFunction;
    DataInformation* arrayType;
private:
    Q_DISABLE_COPY(ArrayParsedData);
};

namespace DataInformationFactory
{
AbstractBitfieldDataInformation* newBitfield(const BitfieldParsedData& pd);
PrimitiveDataInformation* newPrimitive(const PrimitiveParsedData& pd);
EnumDataInformation* newEnum(const EnumParsedData& pd);
FlagDataInformation* newFlags(const EnumParsedData& pd);
ArrayDataInformation* newArray(const ArrayParsedData& pd);

bool commonInitialization(DataInformation* data, const CommonParsedData& pd);
}

#endif /* DATAINFORMATIONFACTORY_H_ */
