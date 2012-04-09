/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010, 2011 Alex Richardson <alex.richardson@gmx.de>
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

#ifndef SCRIPTHANDLER_H_
#define SCRIPTHANDLER_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QSharedData>

#include <QtScript/QScriptEngine>
#include <QtScriptTools/QScriptEngineDebugger>

#include <config-structtool.h>
#include "scripthandlerinfo.h"

class DataInformation;
class ScriptLogger;

class ScriptHandler : public QSharedData
{
    Q_DISABLE_COPY(ScriptHandler)
public:
    typedef QExplicitlySharedDataPointer<ScriptHandler> Ptr;

    ScriptHandler(QScriptEngine* engine, QString scriptFile, QString name);
    virtual ~ScriptHandler();

    DataInformation* initialDataInformationFromScript();
    void validateData(DataInformation* data);
    void updateDataInformation(DataInformation* data);
    QScriptEngine* engine() const;
    ScriptHandlerInfo* handlerInfo();
    ScriptLogger* logger() const;
protected:
    bool loadFile();
    QScopedPointer<QScriptEngine> mEngine;
    QString mFile;
    QString mName;
    QScopedPointer<ScriptLogger> mLogger;
#ifdef OKTETA_DEBUG_SCRIPT
    QScopedPointer<QScriptEngineDebugger> mDebugger;
#endif
    ScriptHandlerInfo mHandlerInfo;
};

inline ScriptLogger* ScriptHandler::logger() const
{
    return mLogger.data();
}

inline QScriptEngine* ScriptHandler::engine() const
{
    return mEngine.data();
}

inline ScriptHandlerInfo* ScriptHandler::handlerInfo()
{
    return &mHandlerInfo;
}

#endif /* SCRIPTHANDLER_H_ */
