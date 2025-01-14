#ifndef _CALGO_QT_MATRIX_WIDGET_HPP_
#define _CALGO_QT_MATRIX_WIDGET_HPP_

#include <QTableView>
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
	MatModel(const ca::Mat<double>& matrix, QObject *parent = nullptr);

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

	const ca::Mat<double>& matrix();

private:
	ca::Mat<double> m_matrix;
};

/**
 * @brief Matrix widget 
 *
 * This class wraps the MatModel class in uniform manner. Not meant to be
 * inheritated.
 *
 * @sa ca::Mat ca::qt::MatModel
 */
class MatWidget: public QTableView {
public:
	MatWidget(const ca::Mat<double>& matrix = {}, QWidget* parent = nullptr);

	void setRowCount(std::size_t rows);
	void setColumnCount(std::size_t cols);

	/**
	 * @brief Get matrix object from model
	 */
	const ca::Mat<double>& matrix();

	/**
	 * @brief Show headers
	 */
	void showIndices();
	/**
	 * @brief Hide headers
	 */
	void hideIndices();

private:
	MatModel m_model;
};

}

#endif // !_CALGO_QT_MATRIX_WIDGET_HPP_
