#include <calgo/optim/packing2D.hpp>

#include <algorithm>

namespace ca::optim {

template<typename T>
void FirstFit2D<T>::packInplace(std::vector<Box2D<T>>& in) {
	if (this->defaultFinish(in))
		return;

	std::vector<Size2D<T>> rows; 
	rows.reserve(in.size()/2);
	for (auto& box : in) {
		bool isPlaced = false;
		T cumHeight = 0;
		for (auto& row : rows) {
			if (box.size().height() <= row.height()) {
				isPlaced = true;
				box.setPosition(row.width(), cumHeight);
				row.wider(box.size().width());
				break;
			}
			cumHeight += row.height();
		}
		if (not isPlaced) {
			rows.push_back(box.size());
			Box2D<T> newBox = box;
			box.setPosition(0, cumHeight);
		}
	}

	T h = 0, w = 0;
	for (auto& row : rows) {
		h += row.height();
		w = std::max(w, row.width());
	}
	this->size().set(w, h);
}

}
