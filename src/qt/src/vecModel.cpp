#include <calgo/qt/vecWidget.hpp>

namespace ca::qt {

VecModel::VecModel(const ca::vec<double>& vector, QObject *parent)
: QAbstractListModel(parent), m_vec(vector) 
{}

int VecModel::rowCount(const QModelIndex &parent) const {
	return m_vec.n();
}

QVariant VecModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid())
		return QVariant();

	if (index.row() >= m_vec.n())
		return QVariant();
	if (index.column() >= 1)
		return QVariant();

	if (role == Qt::DisplayRole or role == Qt::EditRole)
		return m_vec[index.row()];
	else
		return QVariant();
}

QVariant VecModel::headerData(
	int section, Qt::Orientation orientation,
	int role
) const {
	if (role != Qt::DisplayRole)
		return QVariant();

	return QString::number(section+1);
}

Qt::ItemFlags VecModel::flags(const QModelIndex &index) const {
	if (!index.isValid())
		return Qt::ItemIsEnabled;

	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool VecModel::setData(
	const QModelIndex &index,
	const QVariant &value, int role
) {
	if (index.isValid() && role == Qt::EditRole) {

		m_vec[index.row()] =  value.toDouble();
		emit dataChanged(index, index, {role});
		return true;
	}
	return false;
}

// rows before position
bool VecModel::insertRows(int position, int rows, const QModelIndex &parent) {
	beginInsertRows(QModelIndex(), position, position + rows - 1);

	m_vec.insert(position, rows);

	endInsertRows();
	return true;
}

// rows from position
bool VecModel::removeRows(int position, int rows, const QModelIndex &parent) {
	beginRemoveRows(QModelIndex(), position, position + rows - 1);

	m_vec.remove(position, rows);

	endRemoveRows();
	return true;
}

const ca::vec<double>& VecModel::vector() {
	return m_vec;
}

void VecModel::setVector(const ca::vec<double>& vector) {
	m_vec = vector;
	emit dataChanged(
		VecModel::createIndex(0, 0),
		VecModel::createIndex(vector.n(), 0)
	);
}

}
