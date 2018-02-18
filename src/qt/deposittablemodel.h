// Copyright (c) 2011-2013 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ROICOIN_QT_DEPOSITTABLEMODEL_H
#define ROICOIN_QT_DEPOSITTABLEMODEL_H

#include "roicoinunits.h"
#include "wallet/wallet.h"
#include "clientmodel.h"

#include <QAbstractTableModel>
#include <QStringList>

/** UI model for the transaction table of a wallet.
 */
class DepositTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    explicit DepositTableModel(ClientModel *clientModel, QObject *parent=0);
    ~DepositTableModel();

    enum ColumnIndex {
        Status = 0,
        Principal = 1,
        AccruedInterest = 2,
        AccruedValue = 3,
        OnMaturation = 4,
        TermDays = 5,
	DepositBlock = 6,
	MaturationBlock = 7,
	EstimatedDate = 8,
    };

    /** Roles to get specific information from a row.
        These are independent of column.
    */
    enum RoleIndex {
        /** sorting */
        SortRole = Qt::UserRole,
    };

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    int update(std::vector<COutput>& termDepositInfo);

private:
    std::vector<COutput> termDepositInfoData;
    QStringList columns;

    ClientModel *clientModel;

public Q_SLOTS:
	// reserved for more features
};

#endif // ROICOIN_QT_DEPOSITTABLEMODEL_H

