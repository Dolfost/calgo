#ifndef _CALGO_QT_VEC_WIDGET_HPP_
#define _CALGO_QT_VEC_WIDGET_HPP_

#include <QListView>

#include <calgo/vec.hpp>
#include <calgo/qt/vecModel.hpp>

namespace ca::qt {

/**
 * @brief Array widget 
 *
 * This class wraps the VecModel class in uniform manner. Not meant to be
 * inheritated.
 *
 * @sa ca::Vec ca::qt::VecModel
 */
class VecWidget: public QListView {
public:
	VecWidget(const ca::vec<double>& vec = {}, QWidget* parent = nullptr);

	void setSize(std::size_t size);

	/**
	 * @brief Get array object from model
	 */
	const ca::vec<double>& vector();
	void setVector(const ca::vec<double>& vector);

private:
	VecModel m_model;
};

}

#endif // !_CALGO_QT_VEC_WIDGET_HPP_
