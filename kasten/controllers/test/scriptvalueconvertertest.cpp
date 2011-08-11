/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2011  Alex Richardson <alex.richardson@gmx.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

//I'm lazy
#undef QT_NO_CAST_FROM_ASCII

#include "view/structures/script/scriptengineinitializer.h"
#include "view/structures/allprimitivetypes.h"

#include <QtTest>
#include <QtCore/QString>
#include <QtCore/QDate>
#include <QtCore/QDebug>
#include <QtScript/QScriptEngine>
#include "view/structures/script/scriptvalueconverter.h"
#include "view/structures/datatypes/datainformation.h"
#include "view/structures/datatypes/primitive/primitivedatainformation.h"

class ScriptValueConverterTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void testPrimitives();
    void testPrimitives_data();
private:
    DataInformation* evaluate(QString code);
    QScriptEngine engine;
};

DataInformation* ScriptValueConverterTest::evaluate(QString code)
{
    QScriptValue value = engine.evaluate(code);
    ScriptValueConverter conv(value, "value");
    return conv.convert();
}


void ScriptValueConverterTest::initTestCase()
{
    ScriptEngineInitializer::addFuctionsToScriptEngine(engine);
}

void ScriptValueConverterTest::testPrimitives_data()
{
    QTest::addColumn<QString>("code");
    QTest::addColumn<QString>("code2");
    QTest::addColumn<QString>("typeString");
    QTest::addColumn<int>("expectedType");

    QTest::newRow("uint8") << "uint8()" << "new uint8()" << "UInt8" <<(int)Type_UInt8;
    QTest::newRow("uint16") << "uint16()" << "new uint16()" << "UInt16" <<(int)Type_UInt16;
    QTest::newRow("uint32") << "uint32()" << "new uint32()" << "UInt32" <<(int)Type_UInt32;
    QTest::newRow("uint64") << "uint64()" << "new uint64()" << "UInt64" <<(int)Type_UInt64;
    QTest::newRow("int8") << "int8()" << "new int8()" << "Int8" <<(int)Type_Int8;
    QTest::newRow("int16") << "int16()" << "new int16()" << "Int16" <<(int)Type_Int16;
    QTest::newRow("int32") << "int32()" << "new int32()" << "Int32" <<(int)Type_Int32;
    QTest::newRow("int64") << "int64()" << "new int64()" << "Int64" <<(int)Type_Int64;
    QTest::newRow("bool8") << "bool8()" << "new bool8()" << "Bool8" <<(int)Type_Bool8;
    QTest::newRow("bool16") << "bool16()" << "new bool16()" << "Bool16" <<(int)Type_Bool16;
    QTest::newRow("bool32") << "bool32()" << "new bool32()" << "Bool32" <<(int)Type_Bool32;
    QTest::newRow("bool64") << "bool64()" << "new bool64()" << "Bool64" <<(int)Type_Bool64;
    QTest::newRow("char") << "char()" << "new char()" << "Char" <<(int)Type_Char;
    QTest::newRow("float") << "float()" << "new float()" << "Float" <<(int)Type_Float;
    QTest::newRow("double") << "double()" << "new double()" << "Double" <<(int)Type_Double;
    //TODO this is lowercase whereas the others are uppercase, should fix it
    QTest::newRow("bitfield") << "bitfield(\"unsigned\", 1)" << "new bitfield(\"unsigned\", 1)"
        << "bitfield" <<(int)Type_Bitfield;
    //QTest::newRow("not primitive") << "array(uint8(), 12)" << "new array(uint8(), 1)"
    //    << "array" <<(int)Type_NotPrimitive;
}

void ScriptValueConverterTest::testPrimitives()
{
    QFETCH(QString, code);
    QFETCH(QString, code2);
    QFETCH(QString, typeString);
    QFETCH(int, expectedType);
    PrimitiveDataType type = (PrimitiveDataType)expectedType; 

    QScriptValue val1 = engine.evaluate(code);
    QScriptValue val2 = engine.evaluate(code2);
    QCOMPARE(val1.property("type").toString(), typeString);
    QCOMPARE(val2.property("type").toString(), typeString);
    ScriptValueConverter c1(val1, "val1");
    ScriptValueConverter c2(val1, "val2");
    if (type == Type_NotPrimitive)
        return; //the cast will fail
    DataInformation* data1 = c1.convert();
    DataInformation* data2 = c2.convert();
    QVERIFY(data1);
    QVERIFY(data2);
    PrimitiveDataInformation* p1 = dynamic_cast<PrimitiveDataInformation*>(data1);
    PrimitiveDataInformation* p2 = dynamic_cast<PrimitiveDataInformation*>(data2);
    QVERIFY(p1);
    QVERIFY(p2);
    QCOMPARE(p1->type(), type);
    QCOMPARE(p2->type(), type);
    if (type == Type_Bitfield)
        return; //the following tests don't work with bitfields
    DataInformation* data3 = evaluate('\"' + typeString + '\"');
    QVERIFY(data3);
    PrimitiveDataInformation* p3 = dynamic_cast<PrimitiveDataInformation*>(data3);
    QVERIFY(p3);
    QCOMPARE(p3->type(), type);
}

QTEST_MAIN(ScriptValueConverterTest)

#include "scriptvalueconvertertest.moc"