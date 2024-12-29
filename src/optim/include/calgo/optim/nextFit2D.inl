#include <calgo/optim/packing2D.hpp>

#include <list>

namespace ca::optim {

template<typename T>
void NextFit2D<T>::packInplace(std::vector<Box2D<T>>& in) {
	if (this->defaultFinish(in))
		return;

	std::list<Size2D<T>> rows; 
	T filledHeight = 0;
	T totalWidth = 0;
	for (auto& box: in) {
		bool isPlaced = false;
		T height = filledHeight;
		for (auto& row: rows) {
			if (box.size().height() <= row.height()) {
				isPlaced = true;
				box.setPosition(row.width(), height);
				row.wider(box.size().width());
				totalWidth = std::max(totalWidth, row.width());
				break;
			}
			height += row.height();
		}
		if (not isPlaced) {
			rows.push_back(box.size());
			box.setPosition(0, height);
			if (rows.size() > n_k) {
				filledHeight += rows.front().height();
				rows.pop_front();
			}
			totalWidth = std::max(totalWidth, box.size().width());
		}
	}

	T totalHeight = filledHeight;
	for (auto& row: rows) {
		totalHeight += row.height();
	}
	this->size().set(totalWidth, totalHeight);
}

}
