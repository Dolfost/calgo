#include <calgo/optim/packing2D.hpp>

#include <vector>
#include <algorithm>

namespace ca::optim {

template<typename T>
void FirstFit2D<T>::pack(std::vector<Box2D<T>*>& in) {
	if (this->defaultFinish(in))
		return;
	Packing2D<T>::pack(in);

	std::vector<Size2D<T>> rows; 
	rows.reserve(in.size()/2);
	std::size_t boxCount = in.size();
	for (std::size_t i = 0; i < boxCount; i++) {
		Box2D<T>* box = in[i];
		bool isPlaced = false;
		T height = 0;
		for (auto& row : rows) {
			if (box->size().height() <= row.height()) {
				isPlaced = true;
				box->setPosition(row.width(), height);
				row.wider(box->size().width());
				break;
			}
			height += row.height();
		}
		if (not isPlaced) {
			rows.push_back(box->size());
			box->setPosition(0, height);
		}
		if (this->p_boxPackedCallback)
			this->p_boxPackedCallback(in, i);
	}

	T totalHeight = 0, totalWidth = 0;
	for (auto& row : rows) {
		totalHeight += row.height();
		totalWidth = std::max(totalWidth, row.width());
	}
	this->size().set(totalWidth, totalHeight);

	if (this->p_boxesPackedCallback)
		this->p_boxesPackedCallback(in);
}

}
