#include <calgo/qt/vecWidget.hpp>

namespace ca::qt {

VecWidget::VecWidget(const ca::vec<double>& vector, QWidget* parent):
	QListView(parent), m_model(vector) {
	setModel(&m_model);
	setSelectionMode(VecWidget::SingleSelection);
}

const ca::vec<double>& VecWidget::vector() {
	return m_model.vector();
}

void VecWidget::setVector(const ca::vec<double>& vector) {
	return m_model.setVector(vector);
}

void VecWidget::setSize(std::size_t size) {
	int msize = m_model.rowCount();
	if (size < msize)
		m_model.removeRows(size, msize - size);
	else if (size > msize)
		m_model.insertRows(msize, size - msize);
};

}
