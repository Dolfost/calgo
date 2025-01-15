#ifndef _CALGO_QT_SIMPLEX_WIDGET_HPP_
#define _CALGO_QT_SIMPLEX_WIDGET_HPP_

#include <calgo/qt/systemWidget.hpp>
#include <calgo/qt/vecWidget.hpp>

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

	// const ca::Vec<double> function() { return m_constraints->vector(); };

	bool max() {
		return m_minmax->currentIndex() == 0;
	}

protected:
	VecWidget* m_funciton = new VecWidget;
	QComboBox* m_minmax = new QComboBox;
};

}

#endif // !_CALGO_QT_SIMPLEX_WIDGET_HPP_
