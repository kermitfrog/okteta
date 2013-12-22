/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2006-2007,2009,2013 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kreplaceprompt.h"

// KDE
#include <KLocalizedString>
// Qt
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtCore/QEventLoop>


namespace Kasten2
{

KReplacePrompt::KReplacePrompt( QWidget* parent )
 : QDialog( parent )
{
    setModal( true );
    setWindowTitle( i18nc("@title:window prompt for iterative replacement","Replace") );

    // dialog buttons
    QDialogButtonBox* dialogButtonBox = new QDialogButtonBox;
    QPushButton* button = dialogButtonBox->addButton( i18nc("@action:button","Replace &All"),
                                                      QDialogButtonBox::ApplyRole );
    connect( button, &QAbstractButton::clicked, this, &KReplacePrompt::onReplaceAllButton );
    button = dialogButtonBox->addButton( i18nc("@action:button","&Skip"),
                                         QDialogButtonBox::ApplyRole );
    connect( button, &QAbstractButton::clicked, this, &KReplacePrompt::onSkipButton );
    QPushButton* replaceButton = dialogButtonBox->addButton( i18nc("@action:button","Replace"),
                                                             QDialogButtonBox::ApplyRole );
    connect( replaceButton, &QAbstractButton::clicked, this, &KReplacePrompt::onReplaceButton );
    button = dialogButtonBox->addButton( QDialogButtonBox::Close );
    connect( button, &QAbstractButton::clicked, this, &KReplacePrompt::onCloseButton );


    // main layout
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget( dialogButtonBox );

    setLayout( layout );
    resize(minimumSize());

    replaceButton->setDefault( true );
}

ReplaceBehaviour KReplacePrompt::query()
{
    QEventLoop eventLoop;
    mEventLoop = &eventLoop;
    eventLoop.exec();

    return mResult;
}

void KReplacePrompt::onReplaceAllButton()
{
    mResult = ReplaceAll;
    mEventLoop->quit();
}

void KReplacePrompt::onSkipButton()
{
    mResult = SkipCurrent;
    mEventLoop->quit();
}

void KReplacePrompt::onReplaceButton()
{
    mResult = ReplaceCurrent;
    mEventLoop->quit();
}

void KReplacePrompt::onCloseButton()
{
    mResult = CancelReplacing;
    mEventLoop->quit();
}

}
