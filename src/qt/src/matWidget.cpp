#include <calgo/qt/matWidget.hpp>

#include <QHeaderView>

namespace ca::qt {

MatWidget::MatWidget(const ca::Mat<double>& matrix, QWidget* parent):
QTableView(parent), m_model(matrix) {
	setModel(&m_model);
	setSelectionMode(MatWidget::SingleSelection);
}

const ca::Mat<double>& MatWidget::matrix() {
	return m_model.matrix();
}

void MatWidget::setMatrix(const ca::Mat<double>& matrix) {
	return m_model.setMatrix(matrix);
}

void MatWidget::showIndices() {
	horizontalHeader()->show();
	verticalHeader()->show();
}

void MatWidget::hideIndices() {
	horizontalHeader()->hide();
	verticalHeader()->hide();
}

void MatWidget::setRowCount(std::size_t rows) {
	int rowCount = m_model.rowCount();
	if (rows < rowCount)
		m_model.removeRows(rows, rowCount - rows);
	else if (rows > rowCount)
		m_model.insertRows(rowCount, rows - rowCount);
};

void MatWidget::setColumnCount(std::size_t cols) {
	int colCount = m_model.columnCount();
	if (cols < colCount)
		m_model.removeColumns(cols, colCount - cols);
	else if (cols > colCount)
		m_model.insertColumns(colCount, cols - colCount);
};

}
