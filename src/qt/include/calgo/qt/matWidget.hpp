#ifndef _CALGO_QT_MAT_WIDGET_HPP_
#define _CALGO_QT_MAT_WIDGET_HPP_

#include <QTableView>

#include <calgo/mat.hpp>
#include <calgo/qt/matModel.hpp>

namespace ca::qt {

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
	MatWidget(const ca::mat<double>& matrix = {}, QWidget* parent = nullptr);

	void setRowCount(std::size_t rows);
	void setColumnCount(std::size_t cols);

	/**
	 * @brief Get matrix object from model
	 */
	const ca::mat<double>& matrix();
	void setMatrix(const ca::mat<double>& matrix);

	/**
	 * @brief Show headers
	 */
	void showIndices();
	/**
	 * @brief Hide headers
	 */
	void hideIndices();

	/**
	 * @brief Set the table model
	 *
	 * @param model the model pointer
	 */
	void setModel(QAbstractTableModel* model);
	/**
	 * @brief Get the table model
	 *
	 * @return model pointer
	 */
	QAbstractTableModel* model();
};

}

#endif // !_CALGO_QT_MAT_WIDGET_HPP_
