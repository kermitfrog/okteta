/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef CHARSETCONVERSIONTOOL_HPP
#define CHARSETCONVERSIONTOOL_HPP

// lib
#include <kasten/okteta/oktetakastencontrollers_export.hpp>
// Kasten core
#include <kasten/abstracttool.hpp>
// Okteta core
#include <okteta/byte.hpp>
// Qt
#include <QList>

namespace Okteta {
class AbstractByteArrayModel;
class CharCodec;
}
template <class Key, class T> class QMap;

namespace Kasten {

class ByteArrayView;

/**
 */
class OKTETAKASTENCONTROLLERS_EXPORT CharsetConversionTool : public AbstractTool
{
    Q_OBJECT

public:
    enum ConversionDirection
    {
        ConvertFrom,
        ConvertTo
    };

public:
    CharsetConversionTool();
    ~CharsetConversionTool() override;

public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    QString title() const override;

    void setTargetModel(AbstractModel* model) override;

public: // status
    bool isApplyable() const; // candidate for AbstractTool API

    QString otherCharCodecName() const;
    ConversionDirection conversionDirection() const;
    bool isSubstitutingMissingChars() const;
    Okteta::Byte substituteByte() const;

public Q_SLOTS: // settings
    void setConversionDirection(int conversionDirection);
    void setOtherCharCodecName(const QString& codecName);
    void setSubstitutingMissingChars(bool isSubstitutingMissingChars);
    void setSubstituteByte(int byte);

public Q_SLOTS: // actions
    void convertChars();

Q_SIGNALS:
    void isApplyableChanged(bool isApplyable);    // candidate for AbstractTool API
    void conversionDone(bool success, int convertedBytesCount,
                        const QMap<Okteta::Byte, int>& failedPerByteCount);

private Q_SLOTS:
    void onViewChanged();

private: // settings
    QString mOtherCharCodecName;
    ConversionDirection mConversionDirection = ConvertFrom;
    bool mSubstitutingMissingChars = false;
    Okteta::Byte mSubstituteByte = 0;

private: // sources
    ByteArrayView* mByteArrayView = nullptr;
    Okteta::AbstractByteArrayModel* mByteArrayModel = nullptr;
};

}

#endif