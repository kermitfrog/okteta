/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "selectiontest.hpp"

// test object
#include <selection.hpp>
// Qt
#include <QTest>

namespace Okteta {

// local variables
static constexpr Address Start = 15;
static constexpr Address End = 27;

void SelectionTest::testPlainConstructor()
{
    const Selection selection;
    QVERIFY(!selection.isValid());
    QVERIFY(!selection.started());
    QVERIFY(!selection.justStarted());
}

void SelectionTest::testAnchorConstructor()
{
    const Selection selection(Start);
    QCOMPARE(selection.anchor(), Start);
    QVERIFY(!selection.isValid());
    QVERIFY(selection.started());
    QVERIFY(selection.justStarted());
}

void SelectionTest::testSetStart()
{
    Selection selection;
    selection.setStart(Start);
    QCOMPARE(selection.anchor(), Start);
    QVERIFY(!selection.isValid());
    QVERIFY(selection.started());
    QVERIFY(selection.justStarted());
}

void SelectionTest::testSetStartEnd()
{
    Selection selection;
    selection.setStart(Start);
    selection.setEnd(End);
    QCOMPARE(selection.start(), Start);
    QCOMPARE(selection.end(), End - 1);
    QCOMPARE(selection.anchor(), Start);
    QVERIFY(selection.isValid());
    QVERIFY(selection.started());
    QVERIFY(!selection.justStarted());
    QVERIFY(selection.isForward());

    selection.setStart(End);
    selection.setEnd(Start);
    QCOMPARE(selection.start(), Start);
    QCOMPARE(selection.end(), End - 1);
    QCOMPARE(selection.anchor(), End);
    QVERIFY(selection.isValid());
    QVERIFY(selection.started());
    QVERIFY(!selection.justStarted());
    QVERIFY(!selection.isForward());
}

void SelectionTest::testCancel()
{
    Selection selection;
    selection.setStart(Start);
    selection.setEnd(End);
    selection.cancel();
    QVERIFY(!selection.isValid());
    QVERIFY(!selection.started());
    QVERIFY(!selection.justStarted());
}

void SelectionTest::testSetForward()
{
    Selection selection;
    selection.setStart(Start);
    selection.setEnd(End);
    selection.setForward(false);
    QCOMPARE(selection.start(), Start);
    QCOMPARE(selection.end(), End - 1);
    QCOMPARE(selection.anchor(), End);
    QVERIFY(selection.isValid());
    QVERIFY(selection.started());
    QVERIFY(!selection.justStarted());
    QVERIFY(!selection.isForward());

    selection.setForward(true);
    QCOMPARE(selection.start(), Start);
    QCOMPARE(selection.end(), End - 1);
    QCOMPARE(selection.anchor(), Start);
    QVERIFY(selection.isValid());
    QVERIFY(selection.started());
    QVERIFY(!selection.justStarted());
    QVERIFY(selection.isForward());
}

void SelectionTest::testReverse()
{
    Selection selection;
    selection.setStart(Start);
    selection.setEnd(End);
    selection.reverse();
    QCOMPARE(selection.start(), Start);
    QCOMPARE(selection.end(), End - 1);
    QCOMPARE(selection.anchor(), End);
    QVERIFY(selection.isValid());
    QVERIFY(selection.started());
    QVERIFY(!selection.justStarted());
    QVERIFY(!selection.isForward());

    selection.reverse();
    QCOMPARE(selection.start(), Start);
    QCOMPARE(selection.end(), End - 1);
    QCOMPARE(selection.anchor(), Start);
    QVERIFY(selection.isValid());
    QVERIFY(selection.started());
    QVERIFY(!selection.justStarted());
    QVERIFY(selection.isForward());
}

}

QTEST_MAIN(Okteta::SelectionTest)
