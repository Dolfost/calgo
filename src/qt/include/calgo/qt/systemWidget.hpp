#ifndef _CALGO_QT_SYSTEM_WIDGET_HPP_
#define _CALGO_QT_SYSTEM_WIDGET_HPP_

#include <calgo/qt/matWidget.hpp>

#include <QWidget>
#include <QLayout>
#include <QSpinBox>
#include <QLabel>

namespace ca::qt {

/**
 * @brief System of equations widget
 *
 */
class SystemWidget: public QWidget {
	Q_OBJECT
public:
	SystemWidget(QWidget* parent = nullptr);

	void setColsLabel(const QString& label) { m_colsLabel->setText(label); };
	void setRowsLabel(const QString& label) { m_rowsLabel->setText(label); };
	QString colsLabel() { return m_colsLabel->text(); }
	QString rowsLabel() { return m_rowsLabel->text(); };

	const ca::Mat<double>& variables() { return m_variables->matrix(); };
	const ca::Vec<double> constraints() { 
		ca::Vec<double>::size_type size = m_constraints->model()->rowCount();
		ca::Vec<double> c(size);
		for (ca::Vec<double>::size_type i = 0; i < size; i++) {
			c[i] = m_constraints->matrix()(0, i);
		}
		return c;
	};

signals:
	void systemChanged();

protected:
	MatWidget* m_variables = new MatWidget;
	MatWidget* m_constraints = new MatWidget;
	QSpinBox* m_rows = new QSpinBox;
	QSpinBox* m_cols = new QSpinBox;
	QVBoxLayout* m_lay = new QVBoxLayout;
	QHBoxLayout* m_syslay = new QHBoxLayout;
	QLabel* m_rowsLabel = new QLabel("rows");
	QLabel* m_colsLabel = new QLabel("cols");
};

}

#endif // !_CALGO_QT_SYSTEM_WIDGET_HPP_
