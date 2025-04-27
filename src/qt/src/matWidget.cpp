#include "calgo/qt/matModel.hpp"
#include <calgo/qt/matWidget.hpp>

#include <QHeaderView>

namespace ca::qt {

MatWidget::MatWidget(const ca::mat<double>& matrix, QWidget* parent):
QTableView(parent) {
	auto model = new MatModel(matrix);
	QTableView::setModel(model);
	setSelectionMode(MatWidget::SingleSelection);
}

const ca::mat<double>& MatWidget::matrix() {
	return static_cast<MatModel*>(model())->matrix();
}

void MatWidget::setModel(QAbstractTableModel* model) {
	QTableView::setModel(model);
}

QAbstractTableModel* MatWidget::model() {
	return static_cast<QAbstractTableModel*>(QTableView::model());
}

void MatWidget::setMatrix(const ca::mat<double>& matrix) {
	return static_cast<MatModel*>(model())->setMatrix(matrix);
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
	int rowCount = static_cast<MatModel*>(model())->rowCount();
	if (rows < rowCount)
		static_cast<MatModel*>(model())->removeRows(rows, rowCount - rows);
	else if (rows > rowCount)
		static_cast<MatModel*>(model())->insertRows(rowCount, rows - rowCount);
};

void MatWidget::setColumnCount(std::size_t cols) {
	int colCount = static_cast<MatModel*>(model())->columnCount();
	if (cols < colCount)
		static_cast<MatModel*>(model())->removeColumns(cols, colCount - cols);
	else if (cols > colCount)
		static_cast<MatModel*>(model())->insertColumns(colCount, cols - colCount);
};

}
