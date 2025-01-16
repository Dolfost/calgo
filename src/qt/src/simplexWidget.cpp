#include <calgo/qt/simplexWidget.hpp>

#include <QLabel>
#include <QSignalBlocker>

namespace ca::qt {

SimplexWidget::SimplexWidget(
	QWidget* parent
): SystemWidget(parent) {
	m_minmax->addItems({
		"max", "min" 
	});
	connect(
		m_minmax, &QComboBox::currentIndexChanged,
		[this]() { emit systemChanged(); }
	);
	m_controlslay->addWidget(m_minmax);
	m_rows->setValue(m_rows->value() - 1);
}

void SimplexWidget::rowCountChanged() {
	m_variables->setRowCount(m_rows->value()+1);
}

void SimplexWidget::setConstraints(const ca::VecView<double>& constr) { 
	const QSignalBlocker blocker_r(m_rows);
	m_rows->setValue(constr.n());
	rowCountChanged();
	const QSignalBlocker blocker(m_variables->model());
	std::size_t col = m_variables->matrix().cols()-1;
	for (ca::MatView<double>::size_type i = 0; i < constr.n(); i++)
		m_variables->model()->setData(
			m_variables->model()->index(i+1, col),
			constr[i]
		);
	emit systemChanged();
};

void SimplexWidget::setFunction(const ca::VecView<double>& constr) { 
	const QSignalBlocker blocker_c(m_cols);
	m_cols->setValue(constr.n());
	columnCountChanged();
	const QSignalBlocker blocker(m_variables->model());
	for (ca::MatView<double>::size_type i = 0; i < constr.n(); i++)
		m_variables->model()->setData(
			m_variables->model()->index(0, i),
			constr[i]
		);
	emit systemChanged();
};

void SimplexWidget::setVariables(const ca::MatView<double>& vars )  { 
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
				m_variables->model()->index(i+1, j),
				vars(i, j)
			);
	emit systemChanged();
};

const ca::MatView<double> SimplexWidget::variables() const { 
	return m_variables->matrix().mat(
		1, 0, m_variables->matrix().rows() - 1,
		m_variables->matrix().cols() - 1
	);
};

const ca::VecView<double> SimplexWidget::constraints() const { 
	return m_variables->matrix().col(m_variables->matrix().cols()-1, 1);
};

const ca::VecView<double> SimplexWidget::function() const { 
	return m_variables->matrix().row(
		0, 0, m_variables->matrix().cols()-1
	);
};

}
