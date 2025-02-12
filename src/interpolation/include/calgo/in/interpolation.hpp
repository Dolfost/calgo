#ifndef _CALGO_IN_HPP_
#define _CALGO_IN_HPP_

#include <calgo/calgo.hpp>
#include <calgo/vec.hpp>

namespace ca::in {

// TODO: replace lists with const ca::VecView
class Interpolation {
public:
	Interpolation(ListT x = {}, ListT y = {}) {
		setX(x), setY(y);
	}

	virtual void setX(ListT x) { i_x = x; i_n = i_x.size(); };
	virtual void setY(ListT y) { i_y = y; };

	ListT x() { return i_x; };
	ListT y() { return i_y; };
	virtual std::size_t n() { return i_n; };

	virtual ~Interpolation() {};

public:
	double hx(std::size_t i) {
		return i_x[i] - i_x[i-1];
	}
	double hy(std::size_t i) {
		return i_y[i] - i_y[i-1];
	}

protected:
	ListT i_x, i_y;
	std::size_t i_n;
};

class ExplicitInterpolation: public Interpolation {
public:
	using Interpolation::Interpolation;

	virtual double interpolate(double) = 0;
	double operator()(double x) { return interpolate(x); };
};

class ParametricInterpolation: public Interpolation {
public:
	using Interpolation::Interpolation;

	virtual double interpolateX(double t) = 0;
	virtual double interpolateY(double t) = 0;
	std::pair<double, double> operator()(double t) { return {interpolateX(t), interpolateY(t)}; };
};

template<typename T, typename I = T>
class Nodes {
public:
	using value_type = T;
	using size_type = typename vec<T>::size_type;
	using interval_type = I;

public:
	Nodes(interval_type a = 0, interval_type b = 1, size_type n = 10): m_a(a), m_b(b), m_n(n) {};

	const vec<value_type>& nodes() const { return m_nodes; }
	value_type weight(size_type i) { 
		if (i == m_n-2) 
			i--; 
		return m_nodes[i+1] - m_nodes[i]; 
	}
	const interval_type& a() const { return m_a; }
	const interval_type& b() const { return m_b; }
	const size_type& n() const { return m_n; }

	virtual ~Nodes() = default;

protected:
	virtual void calculate() = 0;

protected:
	vec<value_type> m_nodes;
	size_type m_n;
	interval_type m_a, m_b;
};

#define CA_ADD_NODES(CLS) \
template<typename T, typename I = T> \
class CLS##Nodes: public Nodes<T, I> { \
public: \
	using value_type = T; \
	using size_type = typename vec<T>::size_type; \
	using interval_type = I; \
	CLS##Nodes(interval_type a = 0, interval_type b = 1, size_type n = 10): Nodes<T, I>(a, b, n) {  \
		calculate(); \
	}; \
protected: \
	void calculate() override; \
}; \

CA_ADD_NODES(Uniform)
CA_ADD_NODES(Chebyshev)

#undef CA_ADD_NODES

}

#include "../../../src/nodes.inl"

#endif // !_CALGO_IN_HPP_
