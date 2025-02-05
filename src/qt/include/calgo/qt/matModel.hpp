#ifndef _CALGO_QT_MAT_MODEL_HPP_
#define _CALGO_QT_MAT_MODEL_HPP_

#include <QAbstractTableModel>

#include <calgo/mat.hpp>

namespace ca::qt {

/**
 * @brief Table model implementation of ca::Mat
 *
 * @sa ca::Mat ca::qt::MatWidget
 */
class MatModel: public QAbstractTableModel {
	Q_OBJECT
public:
	MatModel(const ca::mat<double>& matrix, QObject *parent = nullptr);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
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
	bool insertColumns(
		int position, int cols, 
		const QModelIndex &index = QModelIndex()
	) override;
	bool removeColumns(
		int position, int cols, 
		const QModelIndex &index = QModelIndex()
	) override;

	const ca::mat<double>& matrix();
	void setMatrix(const ca::mat<double>& matrix);

private:
	ca::mat<double> m_matrix;
};

}

#endif // !_CALGO_QT_MAT_MODEL_HPP_
