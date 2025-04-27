#include <calgo/qt/symmetricMatModel.hpp>

namespace ca::qt {

SymmetricMatModel::SymmetricMatModel(const ca::mat<double>& matrix, QObject *parent)
: MatModel(matrix, parent) {}


bool SymmetricMatModel::setData(
	const QModelIndex &index,
	const QVariant &value, int role
) {
	if (index.isValid() && role == Qt::EditRole) {
		m_matrix(index.row(), index.column()) = value.toDouble();
		m_matrix(index.column(), index.row()) = value.toDouble();
		emit dataChanged(index, index, {role});
		if (index.column() != index.row()) {
			QModelIndex inverted = QAbstractTableModel::createIndex(index.column(), index.row());
			emit dataChanged(inverted, inverted, {role});
		}
		return true;
	}
	return false;
}


}
