#include <calgo/qt/systemWidget.hpp>

#include <QHeaderView>

namespace ca::qt {

SystemWidget::SystemWidget(
	QWidget* parent
): QWidget(parent) {
	setLayout(m_lay);

	ca::Mat<double>::size_type size = 3;
	m_variables->setRowCount(size);
	m_variables->setColumnCount(size+1);

	m_rows->setMinimumWidth(75);
	m_cols->setMinimumWidth(75);
	m_rows->setRange(1, 100);
	m_cols->setRange(1, 100);
	m_rows->setValue(size);
	m_cols->setValue(size);
	connect(
		m_rows, &QSpinBox::editingFinished,
		this, &SystemWidget::rowCountChanged
	);
	connect(
		m_cols, &QSpinBox::editingFinished,
		this, &SystemWidget::columnCountChanged
	);
	connect(
		m_rows, &QSpinBox::valueChanged,
		[this]() { emit systemChanged(); }
	);
	connect(
		m_cols, &QSpinBox::valueChanged,
		[this]() { emit systemChanged(); }
	);

	m_lay->addWidget(m_variables);

	auto controlslay = new QHBoxLayout;
	controlslay->addWidget(m_rowsLabel);
	controlslay->addWidget(m_rows);
	controlslay->addSpacerItem(new QSpacerItem(10, 0));
	controlslay->addWidget(m_colsLabel);
	controlslay->addWidget(m_cols);
	controlslay->addSpacerItem(new QSpacerItem(20, 0, QSizePolicy::Expanding));
	m_lay->addLayout(controlslay);

	auto emitSystemChanged = [this]() {
		emit systemChanged();
	};
	connect(
		m_variables->model(), &ca::qt::MatModel::dataChanged,
		emitSystemChanged
	);
}

void SystemWidget::rowCountChanged() {
	m_variables->setRowCount(m_rows->value());
}

void SystemWidget::columnCountChanged() {
	int colCount = m_variables->model()->columnCount();
	int cols = m_cols->value();
	if (cols < colCount-1) {
		m_variables->model()->removeColumns(cols, colCount - cols - 1);
	} else if (cols > colCount-1) {
		m_variables->model()->insertColumns(colCount-1, cols - (colCount - 1));
	}
}

void SystemWidget::setConstraints(const ca::VecView<double>& constr) { 
	m_rows->setValue(constr.n());
	rowCountChanged();
	std::size_t col = m_variables->matrix().cols()-1;
	for (ca::MatView<double>::size_type i = 0; i < constr.n(); i++)
		m_variables->model()->setData(
			m_variables->model()->index(i, col),
			constr[i]
		);
};

void SystemWidget::setVariables(const ca::MatView<double>& vars )  { 
	m_rows->setValue(vars.rows());
	rowCountChanged();
	m_cols->setValue(vars.cols());
	columnCountChanged();
	for (ca::MatView<double>::size_type i = 0; i < vars.rows(); i++)
		for (ca::MatView<double>::size_type j = 0; j < vars.cols(); j++)
			m_variables->model()->setData(
				m_variables->model()->index(i, j),
				vars(i, j)
			);
};

}
