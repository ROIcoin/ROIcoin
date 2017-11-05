// Copyright (c) 2011-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ROICOIN_QT_ROICOINADDRESSVALIDATOR_H
#define ROICOIN_QT_ROICOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class ROIcoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit ROIcoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** ROIcoin address widget validator, checks for a valid roicoin address.
 */
class ROIcoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit ROIcoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // ROICOIN_QT_ROICOINADDRESSVALIDATOR_H
