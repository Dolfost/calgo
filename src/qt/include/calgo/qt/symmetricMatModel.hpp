#ifndef _CALGO_QT_SYMMETRIC_MAT_MODEL_HPP_
#define _CALGO_QT_SYMMETRIC_MAT_MODEL_HPP_

#include <calgo/qt/matModel.hpp>

namespace ca::qt {

/**
 * @brief Symmetric table model implementation of ca::Mat
 *
 * @sa ca::Mat ca::qt::MatModel
 */
class SymmetricMatModel: public MatModel {
	Q_OBJECT
public:
	SymmetricMatModel(const ca::mat<double>& matrix, QObject *parent = nullptr);

	bool setData(
		const QModelIndex &index, const QVariant &value,
		int role = Qt::EditRole
	) override;
};

}

#endif // !_CALGO_QT_SYMMETRIC_MAT_MODEL_HPP_
