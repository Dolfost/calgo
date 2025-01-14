#include <calgo/qt/systemWidget.hpp>

#include <QHeaderView>

namespace ca::qt {

SystemWidget::SystemWidget(
	QWidget* parent
): QWidget(parent) {
	setLayout(m_lay);

	m_syslay->setSpacing(0);
	m_syslay->addWidget(m_variables);
	m_syslay->addWidget(m_constraints);
	m_lay->addLayout(m_syslay);

	ca::Mat<double>::size_type rows = 3, cols = 3;
	m_constraints->setColumnCount(1);
	m_constraints->setRowCount(rows);
	m_variables->setRowCount(rows);
	m_variables->setColumnCount(cols);

	m_rows->setMinimumWidth(75);
	m_cols->setMinimumWidth(75);
	m_rows->setRange(1, 100);
	m_cols->setRange(1, 100);
	m_rows->setValue(rows);
	m_cols->setValue(cols);
	connect(
		m_rows, &QSpinBox::editingFinished,
		[this]() {
			m_variables->setRowCount(m_rows->value());
			m_constraints->setRowCount(m_rows->value());
			emit systemChanged();
		}
	);
	connect(
		m_cols, &QSpinBox::editingFinished,
		[this]() {
			m_variables->setColumnCount(m_cols->value());
			emit systemChanged();
		}
	);

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
	connect(
		m_constraints->model(), &ca::qt::MatModel::dataChanged,
		emitSystemChanged
	);


	m_constraints->verticalHeader()->hide();
	m_constraints->setMaximumWidth(
		m_constraints->columnWidth(0) + 10 +
		m_constraints->verticalHeader()->width()
	);
}

}
