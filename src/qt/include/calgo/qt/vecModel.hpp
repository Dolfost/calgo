#ifndef _CALGO_QT_VEC_MODEL_HPP_
#define _CALGO_QT_VEC_MODEL_HPP_

#include <QAbstractListModel>

#include <calgo/vec.hpp>

namespace ca::qt {

/**
 * @brief List model implementation of ca::Vec
 *
 * @sa ca::Vec ca::qt::VecWidget
 */
class VecModel: public QAbstractListModel {
	Q_OBJECT
public:
	VecModel(const ca::vec<double>& vec, QObject *parent = nullptr);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant headerData(
		int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole
	) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	bool setData(
		const QModelIndex &index, const QVariant &value,
		int role = Qt::EditRole
	) override;
	bool insertRows(
		int position, int rows, 
		const QModelIndex &index = QModelIndex()
	) override;
	bool removeRows(
		int position, int rows, 
		const QModelIndex &index = QModelIndex()
	) override;

	const ca::vec<double>& vector();
	void setVector(const ca::vec<double>& vector);

private:
	ca::vec<double> m_vec;
};

}

#endif // !_CALGO_QT_VEC_MODEL_HPP_
