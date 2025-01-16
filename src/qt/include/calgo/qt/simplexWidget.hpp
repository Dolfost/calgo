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


	void setVariables(const ca::MatView<double>& vars) override;
	void setConstraints(const ca::VecView<double>& constr) override;
	void setFunction(const ca::VecView<double>& vars);
	const ca::MatView<double> variables() const override;
	const ca::VecView<double> constraints() const override;
	virtual const ca::VecView<double> function() const;

	bool maximize() { return m_minmax->currentIndex() == 0; }

protected slots:
	void rowCountChanged() override;

protected:
	QComboBox* m_minmax = new QComboBox;
};

}

#endif // !_CALGO_QT_SIMPLEX_WIDGET_HPP_
