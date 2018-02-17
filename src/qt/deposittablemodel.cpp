// Copyright (c) 2011-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "deposittablemodel.h"

#include "addresstablemodel.h"
#include "guiconstants.h"
#include "guiutil.h"
#include "optionsmodel.h"
#include "scicon.h"
#include "transactiondesc.h"
#include "transactionrecord.h"
#include "walletmodel.h"

#include "main.h"
#include "sync.h"
#include "uint256.h"
#include "util.h"
#include "wallet/wallet.h"

#include <QColor>
#include <QDateTime>
#include <QDebug>
#include <QIcon>
#include <QList>

// Amount column is right-aligned it contains numbers
static int column_alignments[] = {
		Qt::AlignLeft|Qt::AlignVCenter, /* status */
		Qt::AlignRight|Qt::AlignVCenter, /* Principal */
		Qt::AlignRight|Qt::AlignVCenter, /* Interest */
		Qt::AlignRight|Qt::AlignVCenter, /* Value */
		Qt::AlignRight|Qt::AlignVCenter, /* Maturation */
		Qt::AlignRight|Qt::AlignVCenter, /* Days */
		Qt::AlignRight|Qt::AlignVCenter, /* deposit block */
		Qt::AlignRight|Qt::AlignVCenter, /* maturation */
		Qt::AlignCenter|Qt::AlignVCenter,  /* Date */
};

DepositTableModel::DepositTableModel(ClientModel *clientModel, QObject *parent)
: QAbstractTableModel(parent)
{
	this->clientModel = clientModel;

	this->columns << tr("Status") << tr("Principal") << tr("Accrued Interest") << tr("Accrued Value")
		<< tr("On Maturation") << tr("Term (Days)") << tr("Deposit Block") << tr("Maturation Block")
		<< tr("Estimated Date");

}

static bool compareByDepth(COutput& a, COutput& b) 
{
    return b.nDepth < a.nDepth;
}

int DepositTableModel::update(std::vector<COutput>& termDepositInfo)
{
	this->beginResetModel();
	this->termDepositInfoData = termDepositInfo;
	qSort(this->termDepositInfoData.begin(), this->termDepositInfoData.end(), compareByDepth);
	this->endResetModel();
	LogPrintf("list of deposits updated, count=%d\n",termDepositInfo.size());

	return termDepositInfo.size();
}

DepositTableModel::~DepositTableModel()
{
}

int DepositTableModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return termDepositInfoData.size();
}

int DepositTableModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return columns.length();
}

QVariant DepositTableModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (index.row() >= termDepositInfoData.size() || index.row() < 0)
		return QVariant();

	// TODO: allow to change units
	int nDisplayUnit = ROIcoinUnits::ROI;

	if (role == Qt::TextAlignmentRole)
        {
            return column_alignments[index.column()];
        }
	else if (role == Qt::DisplayRole || role == SortRole)
	{
		COutput ctermDeposit = termDepositInfoData.at(index.row());

		CTxOut termDeposit = ctermDeposit.tx->vout[ctermDeposit.i];
		int curHeight = this->clientModel->getNumBlocks();
		int lockHeight = curHeight-ctermDeposit.nDepth;
		int releaseBlock = termDeposit.scriptPubKey.GetTermDepositReleaseBlock();
		int term  = releaseBlock - lockHeight;
		int blocksRemaining = releaseBlock - curHeight;
		CAmount withInterest = termDeposit.GetValueWithInterest(lockHeight, (curHeight<releaseBlock?curHeight:releaseBlock));
		CAmount matureValue = termDeposit.GetValueWithInterest(lockHeight, releaseBlock);
		int blocksSoFar = curHeight-lockHeight;

		double interestRatePerBlock = pow(((0.0+matureValue)/termDeposit.nValue),1.0/term);
		double interestRate = (pow(interestRatePerBlock,365*720)-1)*100;

		switch(index.column()) {
			case Status:
				if (curHeight >= releaseBlock) {
					return tr("Matured (Warning: this amount is no longer earning interest of any kind)");
				} else {
					return tr("Earned");
				}
				break;
			case Principal:
				if (role == SortRole)
					return qint64(termDeposit.nValue);
				else
					return ROIcoinUnits::format(nDisplayUnit, termDeposit.nValue);
			case AccruedInterest:
				if (role == SortRole)
					return qint64(withInterest-termDeposit.nValue);
				else
					return ROIcoinUnits::format(nDisplayUnit, withInterest-termDeposit.nValue);
			case AccruedValue:
				if (role == SortRole)
					return qint64(withInterest);
				else
					return ROIcoinUnits::format(nDisplayUnit, withInterest);
			case OnMaturation:
				if (role == SortRole)
					return qint64(matureValue);
				else
					return ROIcoinUnits::format(nDisplayUnit, matureValue);
			case TermDays:
				if (role == SortRole)
					return (term)/720;
				else
					return QString::number((term)/720);
			case DepositBlock:
				if (role == SortRole)
					return lockHeight;
				else
					return QString::number(lockHeight); // .rightJustified(7,'0');
			case MaturationBlock:
				if (role == SortRole)
					return releaseBlock;
				else
					return QString::number(releaseBlock); // .rightJustified(7,'0');
			case EstimatedDate:

				time_t rawtime;
				char buffer[16];
				time(&rawtime);
				rawtime += blocksRemaining * 120;
				struct tm *timeinfo = localtime(&rawtime);
				strftime(buffer, 16, "%Y/%m/%d", timeinfo);
				std::string str(buffer);

				if (role == SortRole) {
					return qint64(mktime(timeinfo));
				} else {
					return QString(buffer);
				}
		}
	}


	return QVariant();
}

QVariant DepositTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal)
	{
		if (role == Qt::DisplayRole)
		{
			return columns[section];
		}
		else if (role == Qt::TextAlignmentRole)
		{
			return column_alignments[section];
		}
		else if (role == Qt::ToolTipRole)
		{
			switch(section)
			{

				case Status:
				return tr("Deposit Status");
				case Principal:
				return tr("Principal");
				case AccruedInterest:
				return tr("Accrued Interest");
				case AccruedValue:
				return tr("Accrued Value");
				case OnMaturation:
				return tr("Value On Maturation");
				case TermDays:
				return tr("Term (Days)");
				case DepositBlock:
				return tr("Deposit Block");
				case MaturationBlock:
				return tr("Maturation Block");
				case EstimatedDate:
				return tr("Estimated Date");
			}
		}
	}

	// return QVariant();
	return QAbstractTableModel::headerData(section, orientation, role);
}
