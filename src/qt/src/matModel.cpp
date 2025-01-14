#include <calgo/qt/matWidget.hpp>

namespace ca::qt {

MatModel::MatModel(const ca::Mat<double>& matrix, QObject *parent)
: QAbstractTableModel(parent), m_matrix(matrix) 
{}

int MatModel::rowCount(const QModelIndex &parent) const {
	return m_matrix.rows();
}

int MatModel::columnCount(const QModelIndex &parent) const {
	return m_matrix.cols();
}

QVariant MatModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid())
		return QVariant();

	if (index.row() >= m_matrix.rows())
		return QVariant();
	if (index.column() >= m_matrix.cols())
		return QVariant();

	if (role == Qt::DisplayRole or role == Qt::EditRole)
		return m_matrix[index.row()][index.column()];
	else
		return QVariant();
}

QVariant MatModel::headerData(
	int section, Qt::Orientation orientation,
	int role
) const {
	if (role != Qt::DisplayRole)
		return QVariant();

	return QString::number(section+1);
}

Qt::ItemFlags MatModel::flags(const QModelIndex &index) const {
	if (!index.isValid())
		return Qt::ItemIsEnabled;

	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool MatModel::setData(
	const QModelIndex &index,
	const QVariant &value, int role
) {
	if (index.isValid() && role == Qt::EditRole) {

		m_matrix[index.row()][index.column()] =  value.toDouble();
		emit dataChanged(index, index, {role});
		return true;
	}
	return false;
}

// rows before position
bool MatModel::insertRows(int position, int rows, const QModelIndex &parent) {
	beginInsertRows(QModelIndex(), position, position + rows - 1);

	m_matrix.insertRows(position, rows);

	endInsertRows();
	return true;
}

// rows from position
bool MatModel::removeRows(int position, int rows, const QModelIndex &parent) {
	beginRemoveRows(QModelIndex(), position, position + rows - 1);

	m_matrix.removeRows(position, rows);

	endRemoveRows();
	return true;
}

bool MatModel::insertColumns(int position, int cols, const QModelIndex &parent) {
	beginInsertColumns(QModelIndex(), position, position + cols - 1);

	m_matrix.insertCols(position, cols);

	endInsertColumns();
	return true;
}

bool MatModel::removeColumns(int position, int cols, const QModelIndex &parent) {
	beginRemoveColumns(QModelIndex(), position, position+cols-1);

	m_matrix.removeCols(position, cols);

	endRemoveColumns();
	return true;
}

const ca::Mat<double>& MatModel::matrix() {
	return m_matrix;
}

}
