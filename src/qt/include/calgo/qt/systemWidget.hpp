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
 * @brief System of equations
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

	virtual const ca::mat_view<double> variables() const;
	virtual const ca::vec_view<double> constraints() const;

	virtual void setVariables(const ca::mat_view<double>& vars);
	virtual void setConstraints(const ca::vec_view<double>& constr);

	QHBoxLayout* controlsLayout() { return m_controlslay; };

signals:
	void systemChanged();

protected slots:
	virtual void columnCountChanged();
	virtual void rowCountChanged();

protected:
	MatWidget* m_variables = new MatWidget;
	QSpinBox* m_rows = new QSpinBox;
	QSpinBox* m_cols = new QSpinBox;
	QVBoxLayout* m_lay = new QVBoxLayout;
	QHBoxLayout* m_controlslay = new QHBoxLayout;
	QLabel* m_rowsLabel = new QLabel("rows");
	QLabel* m_colsLabel = new QLabel("cols");
};

}

#endif // !_CALGO_QT_SYSTEM_WIDGET_HPP_
