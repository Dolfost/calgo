#pragma once
#include <calgo/opt/packing2D.hpp>

#include <list>

namespace ca::opt {

template<typename T>
void NextFit2D<T>::pack(std::vector<Box2D<T>*>& in) {
	if (this->defaultFinish(in))
		return;
	Packing2D<T>::pack(in);

	std::list<Size2D<T>> rows; 
	T filledHeight = 0;
	T totalWidth = 0;
	std::size_t boxCount = in.size();
	for (std::size_t i = 0; i < boxCount; i++) {
		Box2D<T>* box = in[i];
		bool isPlaced = false;
		T height = filledHeight;
		for (auto& row: rows) {
			if (box->size().height() <= row.height()) {
				isPlaced = true;
				box->setPosition(row.width(), height);
				row.wider(box->size().width());
				totalWidth = std::max(totalWidth, row.width());
				break;
			}
			height += row.height();
		}
		if (not isPlaced) {
			rows.push_back(box->size());
			box->setPosition(0, height);
			if (rows.size() > n_k) {
				filledHeight += rows.front().height();
				rows.pop_front();
			}
			totalWidth = std::max(totalWidth, box->size().width());
		}
		if (this->p_boxPackedCallback)
			this->p_boxPackedCallback(in, i);
	}

	T totalHeight = filledHeight;
	for (auto& row: rows) {
		totalHeight += row.height();
	}
	this->size().set(totalWidth, totalHeight);

	if (this->p_boxesPackedCallback)
		this->p_boxesPackedCallback(in);
}

}
