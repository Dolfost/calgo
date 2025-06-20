#ifndef _CALGO_OPT_PACKING2D_HPP_
#define _CALGO_OPT_PACKING2D_HPP_

#include <calgo/calgo.hpp>

#include <calgo/macros.hpp>

#include <ostream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>

namespace ca::opt {

template<typename T>
class Size2D {
	static_assert(std::is_arithmetic<T>::value, "Not an arithmetic type");
public:
	Size2D(T width = 0, T height = 0): 
		s_width(width), s_height(height) {};
	void set(T w, T h) {
		setWidth(w), setHeight(h);
	}
	T setWidth(T width) {
		T old = s_width;
		s_width = width;
		return old;
	}
	T setHeight(T height) {
		T old = s_height;
		s_height = height;
		return old;
	}
	T wider(T width) {
		T old = s_width;
		s_width += width;
		return old;
	}
	T higher(T height) {
		T old = s_height;
		s_height += height;
		return old;
	}
	void swap() { return std::swap(s_width, s_height); };
	T max() const { return std::max(s_width, s_height); };
	T min() const { return std::min(s_width, s_height); };

	T width() const { return s_width; }
	T height() const { return s_height; }

	bool operator==(const Size2D<T>& other) const {
		return other.s_width == s_width and other.s_height == s_height;
	}
	bool operator<=(const Size2D<T>& other) const {
		return s_width <= other.s_width and s_height <= other.s_height;
	}
	bool operator>=(const Size2D<T>& other) const {
		return s_width >= other.s_width and s_height >= other.s_height;
	}
	bool operator<(const Size2D<T>& other) const {
		return s_width < other.s_width and s_height < other.s_height;
	}
	bool operator>(const Size2D<T>& other) const {
		return not operator<(other);
	}

	friend std::ostream& operator<<(std::ostream& os, const Size2D<T>& size) {
		return os << size.s_width << 'x' << size.s_height;
	}

private:
	T s_width, s_height;
};

template<typename T>
class Bin2D {
	static_assert(std::is_arithmetic<T>::value, "Not an arithmetic type");
public:
	Bin2D(T width = 0, T height = 0): 
		b_size(width, height) {};
	Bin2D(const Size2D<T>& size): 
		b_size(size) {};
	const Size2D<T>& size() const { return b_size; };
	Size2D<T>& size() { return b_size; };
	Size2D<T> setSize(Size2D<T> size) { 
		Size2D<T> old = b_size; 
		b_size = size;
		return old;
	};
	T area() const { return b_size.width()*b_size.height(); }
	T perimeter() const { return 2*(b_size.width()+b_size.height()); }
	T diag() const { 
		return std::sqrt(
			std::pow(b_size.width(), 2) + std::pow(b_size.height(), 2)
		); 
	}

private:
	Size2D<T> b_size;
};

template<typename T>
class Box2D: public Bin2D<T> {
public:
	Box2D(const Size2D<T>& size = {}, T x = 0, T y = 0): 
		b_x(x), b_y(y), Bin2D<T>(size.width(), size.height()) {};
	Box2D(T width, T height, T x = 0, T y = 0): 
		b_x(x), b_y(y), Bin2D<T>(width, height) {};
	void setPosition(T x, T y) { setX(x), setY(y); }
	T setX(T x) {
		T old = b_x;
		b_x = x;
		return old;
	}
	T setY(T y) {
		T old = b_y;
		b_y = y;
		return old;
	}
	T x() const { return b_x; }
	T y() const { return b_y; }

	std::string str() const { 
		std::stringstream out;
		out << '[' << this->size() << " at (" 
			<< b_x << ';' << b_y << ")]";
		return out.str();
	}

	friend std::ostream& operator<<(std::ostream& os, const Box2D<T>& box) {
		return os << box.size() << " at (" << box.b_x << ';' << box.b_y << ')';
	}
private:
	T b_x = 0, b_y = 0;
};

template<typename T>
class Packing2D: public Bin2D<T> {
public:
	using Bin2D<T>::Bin2D;

	virtual void pack(std::vector<Box2D<T>*>& in) {
		if (this->s_comp)
			std::sort(in.begin(), in.end(), this->s_comp);
	}

	CA_CALLBACK(boxPacked, const std::vector<Box2D<T>*>& boxes, std::size_t index)
	CA_CALLBACK(boxesPacked, const std::vector<Box2D<T>*>& boxes)

	using Comp = std::function<bool(const ca::opt::Box2D<T>* a, const ca::opt::Box2D<T>* b)>;
	void setComparator(Comp comp) {
		s_comp = comp;
	}
	const Comp& comparator() const {
		return s_comp;
	}
	Comp& comparator() {
		return s_comp;
	}

	virtual ~Packing2D() = 0;

protected:
	Comp s_comp;

protected:
	bool defaultFinish(const std::vector<Box2D<T>*>& in) {
		if (in.size() == 0) {
			this->setSize({0, 0});
			return true;
		}
		return false;
	}

private:
	T p_width, p_height;
};

template<typename T>
Packing2D<T>::~Packing2D() {}

template<typename T>
class FirstFit2D: virtual public Packing2D<T> {
public:
	using Packing2D<T>::Packing2D;
	virtual void pack(std::vector<Box2D<T>*>& in) override;

};

template<typename T>
class NextFit2D: virtual public Packing2D<T> {
public:
	using Packing2D<T>::Packing2D;
	virtual void pack(std::vector<Box2D<T>*>& in) override;

	std::size_t k() const { return n_k; };
	std::size_t setK(std::size_t k) {
		std::size_t old = n_k;
		n_k = k;
		return old;
	}

private:
	std::size_t n_k = 1;
};

template<typename T>
class TreeFit2D: public virtual Packing2D<T> {
public:
	virtual void pack(std::vector<Box2D<T>*>& in) override;

protected:
	class Node: public Box2D<T> {
	public:
		using Box2D<T>::Box2D;
		Node* down() { return n_down; }
		Node* right() { return n_right; }
		bool used() const { return n_used; }
		void setDown(Node* n) { n_down = n; }
		void setRight(Node* n) { n_right = n;}
		void setUsed(bool u) { n_used = u; }

	public:
		~Node() {
			delete n_down;
			delete n_right;
		};

	private:
		Node* n_down = nullptr;
		Node* n_right = nullptr;
		bool n_used = false;
	};

protected:
	Node* findNode(Node* root, const Size2D<T>& size);
	Node* findNode(const Size2D<T>& size) {
		return findNode(t_root, size);
	};
	Node* splitNode(Node* node, const Size2D<T>& size);
	Node* growNode(const Size2D<T>& size);
	Node* growRight(const Size2D<T>& size);
	Node* growDown(const Size2D<T>& size);

private:
	Node* t_root = nullptr;
};

}

#include <calgo/nomacros.hpp>

#include "../../../src/firstFit2D.inl"
#include "../../../src/nextFit2D.inl"
#include "../../../src/treeFit2D.inl"

#endif // !_CALGO_OPT_PACKING2D_HPP_
