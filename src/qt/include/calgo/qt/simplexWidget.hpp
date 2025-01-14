#ifndef _CALGO_QT_SIMPLEX_WIDGET_HPP_
#define _CALGO_QT_SIMPLEX_WIDGET_HPP_

#include <calgo/qt/systemWidget.hpp>

#include <QWidget>
#include <QLayout>
#include <QComboBox>
#include <QLabel>

namespace ca::qt {

/**
 * @brief Simplex widget
 *
 */
class SimplexWidget: public SystemWidget {
	Q_OBJECT
public:
	SimplexWidget(QWidget* parent = nullptr);

	const ca::Vec<double> function() { 
		ca::Vec<double>::size_type size = m_funciton->model()->columnCount();
		ca::Vec<double> c(size);
		for (ca::Vec<double>::size_type i = 0; i < size; i++)
			c[i] = m_funciton->matrix()(0, i);
		return c;
	};

	bool max() {
		return m_minmax->currentIndex() == 0;
	}

protected:
	MatWidget* m_funciton = new MatWidget;
	QComboBox* m_minmax = new QComboBox;
};

}

#endif // !_CALGO_QT_SIMPLEX_WIDGET_HPP_
