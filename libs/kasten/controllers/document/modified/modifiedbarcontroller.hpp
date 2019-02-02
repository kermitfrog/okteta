/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef MODIFIEDBARCONTROLLER_HPP
#define MODIFIEDBARCONTROLLER_HPP

// lib
#include <kasten/kastencontrollers_export.hpp>
// Kasten gui
#include <kasten/abstractxmlguicontroller.hpp>
// Kasten core
#include <kasten/kastencore.hpp>

class QLabel;

namespace Kasten {
class AbstractModelSynchronizer;
class AbstractDocument;
class StatusBar;

class KASTENCONTROLLERS_EXPORT ModifiedBarController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit ModifiedBarController(StatusBar* statusBar);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS:
    void onContentFlagsChanged(Kasten::ContentFlags contentFlags);
    void onLocalSyncStateChanged(Kasten::LocalSyncState localSyncState);
    void onRemoteSyncStateChanged(Kasten::RemoteSyncState remoteSyncState);
    void onSynchronizerChanged(Kasten::AbstractModelSynchronizer* newSynchronizer);
    void onSynchronizerDeleted(QObject* synchronizer);

private:
    AbstractDocument* mDocument = nullptr;
    AbstractModelSynchronizer* mSynchronizer = nullptr;

    QLabel* mLocalStateLabel;
    QLabel* mRemoteStateLabel;
};

}

#endif