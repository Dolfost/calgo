#ifndef _CALGO_OPTIM_HPP_
#define _CALGO_OPTIM_HPP_

#include <calgo/calgo.hpp>

#include <ostream>
#include <string>
#include <sstream>
#include <vector>

namespace ca::optim {

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

	T width() const { return s_width; }
	T height() const { return s_height; }

	bool operator==(const Size2D<T>& other) const {
		return other.s_width == s_width and other.s_height == s_height;
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
	Bin2D(T width = 1, T height = 1): 
		b_size(width, height) {};
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

	virtual std::vector<Box2D<T>> pack(const std::vector<Box2D<T>>& in) {
		std::vector<Box2D<T>> res = in;
		packInplace(res);
		return res;
	};

	virtual void packInplace(std::vector<Box2D<T>>& in) = 0;

protected:
	bool defaultFinish(const std::vector<Box2D<T>>& in) {
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
class FirstFit2D: virtual public Packing2D<T> {
public:
	using Packing2D<T>::Packing2D;
	virtual void packInplace(std::vector<Box2D<T>>& in) override;
};

template<typename T>
class TreeFit2D: public Packing2D<T> {
public:
	virtual void packInplace(std::vector<Box2D<T>>& in) override;

public:
	class Node: public Box2D<T> {
		Node* down() { return n_down; }
		Node* right() { return n_right; }
		Node* fit() { return n_fit; }
		bool used() const { return n_used; }
		void setDown(Node* n) { n_down = n; }
		void setRight(Node* n) { n_right = n;}
		void setFit(Node* n) { n_fit = n;}
		void setUsed(bool u) { n_used = u; }
	private:

	private:
		Node* n_fit = nullptr;
		Node* n_down = nullptr;
		Node* n_right = nullptr;
		bool n_used = false;
	};

protected:
	Node* findNode(Node* root, Size2D<T> size);
	Node* findNode(Size2D<T> size, Node* node) {
		findNode(t_root, node);
	};
	Node* splitNode(Node* node, Size2D<T> size);
	Node* growNode(Size2D<T> size);
	Node* growRight(Size2D<T> size);
	Node* growDown(Size2D<T> size);

private:
	Node* t_root = nullptr;
};

template<typename T>
class SortedPacking2D: virtual public Packing2D<T> {
public:
	using Comp = std::function<bool(const ca::optim::Box2D<T>& a, const ca::optim::Box2D<T>& b)>;
	void setComparator(Comp comp) {
		s_comp = comp;
	}
	const Comp& comparator() const {
		return s_comp;
	}
protected:
	Comp s_comp = [](const Box2D<T>& a, const Box2D<T>& b) {
		return a.area() < b.area();
	};
};

#define CALGO_OPTIM_DEFINE_SORTED_PACKING(CLS) \
template<typename T> \
class Sorted##CLS: public CLS<T>, public SortedPacking2D<T> { \
	using CLS<T>::FirstFit2D; \
	virtual void packInplace(std::vector<Box2D<T>>& in) { \
		std::sort(in.begin(), in.end(), this->s_comp); \
		CLS<T>::packInplace(in); \
	} \
};

CALGO_OPTIM_DEFINE_SORTED_PACKING(FirstFit2D)
CALGO_OPTIM_DEFINE_SORTED_PACKING(TreeFit2D)

#undef CALGO_OPTIM_DEFINE_SORTED_PACKING

}


#include "firstFit2D.inl"
#include "treeFit2D.inl"

#endif // !_CALGO_OPTIM_HPP_
