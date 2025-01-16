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
	m_lay->addWidget(m_variables);

	m_controlslay->addWidget(m_rowsLabel);
	m_controlslay->addWidget(m_rows);
	m_controlslay->addSpacerItem(new QSpacerItem(10, 0));
	m_controlslay->addWidget(m_colsLabel);
	m_controlslay->addWidget(m_cols);
	m_controlslay->addSpacerItem(new QSpacerItem(10, 0, QSizePolicy::Expanding));
	m_lay->addLayout(m_controlslay);

	auto emitSystemChanged = [this]() {
		emit systemChanged();
	};
	connect(
		m_variables->model(), &ca::qt::MatModel::dataChanged,
		emitSystemChanged
	);
	connect(
		m_variables->model(), &ca::qt::MatModel::rowsInserted,
		emitSystemChanged
	);
	connect(
		m_variables->model(), &ca::qt::MatModel::rowsRemoved,
		emitSystemChanged
	);
	connect(
		m_variables->model(), &ca::qt::MatModel::columnsInserted,
		emitSystemChanged
	);
	connect(
		m_variables->model(), &ca::qt::MatModel::columnsRemoved,
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
	const QSignalBlocker blocker_r(m_rows);
	m_rows->setValue(constr.n());
	rowCountChanged();
	const QSignalBlocker blocker_s(m_variables->model());
	std::size_t col = m_variables->matrix().cols()-1;
	for (ca::MatView<double>::size_type i = 0; i < constr.n(); i++)
		m_variables->model()->setData(
			m_variables->model()->index(i, col),
			constr[i]
		);
	emit systemChanged();
};

void SystemWidget::setVariables(const ca::MatView<double>& vars )  { 
	const QSignalBlocker blocker_r(m_rows);
	const QSignalBlocker blocker_c(m_cols);
	m_rows->setValue(vars.rows());
	rowCountChanged();
	m_cols->setValue(vars.cols());
	columnCountChanged();
	const QSignalBlocker blocker_s(m_variables->model());
	for (ca::MatView<double>::size_type i = 0; i < vars.rows(); i++)
		for (ca::MatView<double>::size_type j = 0; j < vars.cols(); j++)
			m_variables->model()->setData(
				m_variables->model()->index(i, j),
				vars(i, j)
			);
	emit systemChanged();
};

const MatView<double> SystemWidget::variables() const {
	return m_variables->matrix().mat(
		0, 0, m_variables->matrix().rows(),
		m_variables->matrix().cols() - 1
	);
}

const ca::VecView<double> SystemWidget::constraints() const { 
	return m_variables->matrix().col(m_variables->matrix().cols()-1);
};

}
