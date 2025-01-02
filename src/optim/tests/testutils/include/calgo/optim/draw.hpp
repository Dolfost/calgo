#ifndef _CALGO_OPTIM_TESTING_DRAW_HPP_
#define _CALGO_OPTIM_TESTING_DRAW_HPP_

#include <calgo/optim/packing2D.hpp>

#include <iostream>
#include <sstream>
#include <numeric>
#include <vector>
#include <string>
#include <algorithm>

class ScreenRow: public std::string {
public:
	ScreenRow(std::size_t len, char ch = ' ') {
		resize(len, ch);
	};

	char& operator[](std::size_t x) {
		return std::string::operator[](x);
	}
	const char& operator[](std::size_t x) const {
		return std::string::operator[](x);
	}

	const std::string& str() { return static_cast<const std::string&>(*this); };
	friend std::ostream& operator<<(std::ostream& os, const ScreenRow& sr) {
		return os << static_cast<const std::string&>(sr) << '\n';
	}
};


class ScreenBuffer: public std::vector<ScreenRow> {
public:
	ScreenBuffer(std::size_t width, std::size_t height, char ch = ' ') {
		resize(height, ScreenRow(width));
	}
	friend std::ostream& operator<<(std::ostream& os, const ScreenBuffer& sb) {
		for (auto const& row: sb) {
			os << row;
		}
		return os;
	}
	void hline(std::size_t x, std::size_t y, std::size_t l, char ch = '#') {
		for (std::size_t xi = x; xi - x <= l; xi++) {
			operator[](y)[xi] = ch;
		}
	}
	void vline(std::size_t x, std::size_t y, std::size_t l, char ch = '#') {
		for (std::size_t yi = y; yi - y <= l; yi++) {
			operator[](yi)[x] = ch;
		}
	}
	void point(std::size_t x, std::size_t y, char ch = '*') {
		at(y)[x] = ch;
	}
	template<typename T>
	void box(const ca::optim::Size2D<T>& size, std::size_t x, std::size_t y) {
		if (size == ca::optim::Size2D<T>{0, 0})
			return;
		point(x, y, '+');
		if (size.width() > 1) {
			hline(x + 1, y, size.width() - 2);
			hline(x, y + size.height() - 1, size.width() - 1);
		}
		if (size.height() > 2) {
			vline(x, y + 1, size.height() - 3);
			vline(x + size.width() - 1, y + 1, size.height() - 3);
		}
		point(x + size.width() - 1, y + size.height() - 1, '*');
	}

	template<typename T>
	void draw(const ca::optim::Box2D<T>& ibox) {
		box(
			ibox.size(), 
			ibox.x(), ibox.y()
		);
	}
	void write(const std::string& str, std::size_t x = 0, std::size_t y = 0) {
		at(y).replace(x, str.size(), str);
	}
	void cwrite(const std::string& str, std::size_t x = 0, std::size_t y = 0) {
		at(y).replace(x - str.size()/2, str.size(), str);
	}
};


template<typename T>
void drawBoxesRow(const std::vector<ca::optim::Box2D<T>*>& boxes) {
	T maxHeight = (*std::max_element(
		boxes.cbegin(), boxes.cend(), 
		[](const ca::optim::Box2D<T>* a, const ca::optim::Box2D<T>* b) {
			return a->size().height() < b->size().height();
		}
	))->size().height();
	using namespace std::placeholders;
	T totalWidth = std::accumulate(
		boxes.cbegin(), boxes.cend(), 0,
		[](T sum, const ca::optim::Box2D<T>* box) {
			return sum + box->size().width();
		}
	);
	const std::size_t labelHeigt = 1;
	ScreenBuffer buffer(totalWidth, maxHeight + labelHeigt);
	std::size_t x = 0;
	for (auto const& box: boxes) {
		int y = maxHeight - box->size().height() + labelHeigt;
		buffer.box(box->size(), x, y);
		std::stringstream ss;
		ss << box->size();
		buffer.cwrite(ss.str(), x + box->size().width()/2, y - labelHeigt);

		x += box->size().width();
	}

	std::cout << buffer;
}

template<typename T>
void drawBoxesPlane(const std::vector<ca::optim::Box2D<T>*>& boxes, const ca::optim::Packing2D<T>& pack) {
	ScreenBuffer buffer(pack.size().width(), pack.size().height());
	for (auto const& box: boxes) {
		buffer.draw(*box);
	}
	std::cout << buffer;
}

#endif // !_CALGO_OPTIM_TESTING_DRAW_HPP_
