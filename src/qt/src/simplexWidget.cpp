#include <calgo/qt/simplexWidget.hpp>

#include <QHeaderView>
#include <QHBoxLayout>

namespace ca::qt {

SimplexWidget::SimplexWidget(
	QWidget* parent
): SystemWidget(parent) {
	auto funclay = new QHBoxLayout();
	funclay->setContentsMargins(0,0,0,0);
	funclay->addSpacerItem(
	new QSpacerItem(
			m_variables->verticalHeader()->width(),
			0
		)
	);
	funclay->addWidget(m_funciton);
	funclay->addWidget(m_minmax);

	m_funciton->hideIndices();
	m_funciton->setRowCount(1);
	m_funciton->setColumnCount(m_cols->value());

	auto margins = m_syslay->contentsMargins();
	margins.setTop(0);
	m_syslay->setContentsMargins(margins);

	m_minmax->setFixedWidth(m_constraints->width());
	m_minmax->addItems({
		"max",
		"min"
	});

	connect(
		m_minmax, &QComboBox::currentIndexChanged,
		[this]() {
			emit systemChanged();
		}
	);
	connect(
		m_funciton->model(), &ca::qt::MatModel::dataChanged,
		[this]() {
			emit systemChanged();
		}
	);
	connect(
		m_cols, &QSpinBox::editingFinished,
		[this]() {
			m_funciton->setColumnCount(m_cols->value());
		}
	);

	std::size_t height = m_funciton->rowHeight(0)+20;
	m_funciton->setMaximumHeight(height);
	m_minmax->setFixedHeight(height);

	m_lay->insertLayout(0, funclay);
}

}
