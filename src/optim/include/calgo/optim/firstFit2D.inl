#include <calgo/optim/packing2D.hpp>

#include <vector>
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
		T height = 0;
		for (auto& row : rows) {
			if (box.size().height() <= row.height()) {
				isPlaced = true;
				box.setPosition(row.width(), height);
				row.wider(box.size().width());
				break;
			}
			height += row.height();
		}
		if (not isPlaced) {
			rows.push_back(box.size());
			box.setPosition(0, height);
		}
	}

	T totalHeight = 0, totalWidth = 0;
	for (auto& row : rows) {
		totalHeight += row.height();
		totalWidth = std::max(totalWidth, row.width());
	}
	this->size().set(totalWidth, totalHeight);
}

}
