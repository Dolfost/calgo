#ifndef _CALGO_QT_SYSTEM_WIDGET_HPP_
#define _CALGO_QT_SYSTEM_WIDGET_HPP_

#include <calgo/qt/matWidget.hpp>
#include <calgo/qt/vecWidget.hpp>

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

	const ca::MatView<double> variables() const { 
		return m_variables->matrix().mat(
			0, 0, m_variables->matrix().rows(),
			m_variables->matrix().cols() - 1
		);
	};
	const ca::VecView<double> constraints() const { 
		return m_variables->matrix().col(m_variables->matrix().cols()-1);
	};

	void setVariables(const ca::MatView<double>& vars);
	void setConstraints(const ca::VecView<double>& constr);

signals:
	void systemChanged();

private slots:
	void columnCountChanged();
	void rowCountChanged();

protected:
	MatWidget* m_variables = new MatWidget;
	QSpinBox* m_rows = new QSpinBox;
	QSpinBox* m_cols = new QSpinBox;
	QVBoxLayout* m_lay = new QVBoxLayout;
	QLabel* m_rowsLabel = new QLabel("rows");
	QLabel* m_colsLabel = new QLabel("cols");
};

}

#endif // !_CALGO_QT_SYSTEM_WIDGET_HPP_
