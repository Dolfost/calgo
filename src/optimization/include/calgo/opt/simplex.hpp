#ifndef _CALGO_OPT_SIMPLEX_HPP_
#define _CALGO_OPT_SIMPLEX_HPP_

#include <calgo/calgo.hpp>

#include <calgo/vec.hpp>
#include <calgo/mat.hpp>

#include <calgo/macros.hpp>

namespace ca::opt {

/**
 * @brief Simplex method
 *
 * @tparam T numerical type 
 * @param v input matrix pointer
 * @param c constraints vector pointer
 * @param f function vector pointer
 *
 * Simeplex method implementation that solves normalized
 * problem in form 
 * @f{equation}
 * (\min)\max c_1x_1 + c_2x_2 + \dots + c_mx_m + \dots + c_nx_n,\quad c_i = 0\ \forall i \geq m, \\
 * \begin{cases}
 * a_{11}x_1 &+& a_{12}x_2 &+& \dots &+& a_{1m} &+& \dots &+& a_{1n}x_n &=& b_1, \\
 * \dots &+& \dots &+& \dots &+& \dots &+& \dots &+& \dots &=& b_i, \\
 * a_{n1}x_1 &+& a_{n2}x_2 &+& \dots &+& a_{nm} &+& \dots &+& a_{nn}x_n &=& b_n.
 * \end{cases}\\
 * x_i \geq 0,\quad i = \overline{1, n}, \quad m < n,\\
 * a_{ij} = \begin{cases} 
 * 	a_{ij}, &\text{ if } j < m, \\ 
 * 	1, &\text{ if } j = i+m-1,\\
 * 	0, &\text{ if } j \neq i+m-1,
 * 	\end{cases} \text{ where } j \geq m.
 * @f}
 *
 * Class does not own any of passed pointers so they wont be freed.
 * Calculations are performed in-place. Default operation is maximization.
 */
template<typename T>
class Simplex {
public:
	Simplex(
		Mat<T>* v = nullptr, 
		Vec<T>* c = nullptr, 
		const Vec<T>* f = nullptr
	); 

	/**
	 * @brief Sets variable matrix
	 *
	 * @param v variable matrix if for m
	 * @f{equation}
	 * v = \begin{bmatrix}
	 * m_{11} & m_{12} & m_{13} & \dots & m_{1n} \\
	 * m_{21} & m_{22} & m_{23} & \dots & m_{2n} \\
	 * \dots & \dots & \dots & \dots & \dots \\
	 * m_{n1} & m_{n2} & m_{n3} & \dots & m_{nn}
	 * \end{bmatrix}
	 * @f}
	 */
	void setVariables(Mat<T>* v) { s_ctx.vars = v; }
	/**
	 * @brief Sets constraints vector
	 *
	 * @param c constraints vector
	 */
	void setConstraints(Vec<T>* c) { s_ctx.constr = c; }
	/**
	 * @brief Sets the function vector
	 *
	 * @param f function vetor
	 */
	void setFunction(Vec<T>* f) { s_ctx.func = f; }


	/**
	 * @brief Specify maximization or minimization
	 *
	 * @param m `true` if maximize, `false` if minimise
	 */
	void setMaximize(bool m) { s_maximize = m; };

	/**
	 * @brief Get maximization(minimization) result
	 *
	 * @return @f(\max(\min)@f) value
	 *
	 * @sa setMaximize
	 */
	T f() const { return s_ctx.f; }
	Vec<T>* constraints() { return s_ctx.constr;  }
	Mat<T>* variables()   { return s_ctx.vars;    };
	const Vec<T>* function() const { return s_ctx.func;    };
	const Vec<T>* constraints() const { return s_ctx.constr;  }
	const Mat<T>* variables()   const { return s_ctx.vars;    };
	const Vec<T>& netEvaluation() const { return s_ctx.netEval; }
	const Vec<T>& basis() { return s_ctx.basis; }
	const Vec<T>& basisIndexes() { return s_ctx.basisIndexes; }
	bool maximize() const { return s_maximize; }
	bool optimal() const { return s_ctx.optimal; }
	bool unbounded() const { return s_ctx.unbounded; }
	bool degenerated() const { return s_ctx.unbounded; }

	/**
	 * @brief Solve linear programing problem
	 */
	void optimize();

	/**
	 * @brief Simplex calculation context
	 */
	struct Context {
		Mat<T>* vars; ///< variables
		Vec<T>* constr; ///< constraints
		const Vec<T>* func; ///< function
		Vec<T> netEval; ///< net evaluation
		Vec<T> basis; ///< basis
		Vec<typename Vec<T>::size_type> basisIndexes; ///< indexes of basis variables

		T f; ///< function value

		bool unbounded;
		bool optimal;
		bool degenerated;
	};

	/**
	 * @brief Pivot callback
	 * @param ctx Simplex context
	 * @param row pivot row
	 * @param col pivot col
	 *
	 * Gets called after every pivoting.
	 * May not run at all, if input problem is unbounded, or optimal.
	 */
	CA_CALLBACK(pivot, const Context& ctx, std::size_t row, std::size_t col);
	/**
	 * @brief Initialization callback
	 * @param ctx Simplex context
	 *
	 * Gets called after initial table creation.
	 * Runs only once.
	 */
	CA_CALLBACK(init, const Context&);
	/**
	 * @brief Iteration callback
	 * @param ctx Simplex context
	 *
	 * Gets called before every pivot column and row searching. 
	 * It is guaranteed that it will run atleast once.
	 */
	CA_CALLBACK(iteration, const Context&);

	/**
	 * @brief Validity callback 
	 * @param ctx Simplex context
	 *
	 * Gets called before any changes to input data after 
	 * optimize() is called and input 
	 * data checked for range errors.
	 */
	CA_CALLBACK(valid, const Context&);

	template<typename D>
	friend std::ostream& operator<<(std::ostream& os, const Simplex<D>& s);

protected:
	/**
	 * @brief Main iteration function
	 *
	 * @return `true`, if iteration should continue, `false` otherwise
	 */
	bool iterate();

	/**
	 * @brief Find next pivot column
	 *
	 * @return column index
	 */
	std::size_t pivotCol();

	/**
	 * @brief Find next pivot row
	 *
	 * @return row index
	 */
	std::size_t pivotRow(std::size_t col);

	/**
	 * @brief Perform pivoting
	 *
	 * @param row pivot row
	 * @param col pivot col
	 */
	void pivot(std::size_t row, std::size_t col);

	/**
	 * @brief Initialize simplex tableau
	 */

	void init();

protected:
	Context s_ctx;

	std::size_t s_r, s_c;
	bool s_maximize = true;
	bool (*s_comparator)(const T& a, const T& b);
};


}

#include <calgo/nomacros.hpp>

#include "../../../src/simplex.inl"

#endif // !_CALGO_OPT_SIMPLEX_HPP_
