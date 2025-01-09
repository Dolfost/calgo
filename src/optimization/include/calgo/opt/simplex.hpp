#include <calgo/calgo.hpp>

#include <calgo/vector.hpp>
#include <calgo/matrix.hpp>

#include <calgo/macros.hpp>

#include <optional>
#include <limits>

namespace ca::opt {

/**
 * @brief Simplex method
 *
 * @tparam T numerical type 
 * @param v input matrix pointer
 * @param c constraints vector pointer
 * @param f function vector pointer
 *
 * Simeplex method implementation takes the normalized
 * problem in form 
 *
 * @f{equation}
 * \max c_1x_1 + c_2x_2 + ... c_nx_n \\
 * \begin{cases}
 * m_{11}x_1 &+& m_{12}x_2 &+& ... &+& m_{1n}x_n &=& b_1, \\
 * \dots &+& \dots &+& \dots &+& \dots &=& b_i, \\
 * m_{n1}x_1 &+& m_{n2}x_2 &+& \dots &+& m_{nn}x_n &=& b_n
 * \end{cases}
 * @f}
 *
 * Class does not own any of passed pointers so they wont be freed.
 * Calculations are performed in-place.
 */
template<typename T>
class Simplex {
public:
	Simplex(
		Matrix<T>* v = nullptr, 
		Vector<T>* c = nullptr, 
		const Vector<T>* f = nullptr
	) {
		ctx.vars = v;
		ctx.constr = c;
		ctx.func = f;
	}

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
	void setVariables(Matrix<T>* v)   { ctx.vars = v; }
	/**
	 * @brief Sets constraints vector
	 *
	 * @param c constraints vector
	 */
	void setConstraints(Vector<T>* c) { ctx.constr = c; }
	/**
	 * @brief Sets the function vector
	 *
	 * @param f function vetor
	 */
	void setFunction(Vector<T>* f)    { ctx.func = f; }


	T f() const { return ctx.f; }
	Vector<T>* constraints() { return ctx.constr; }
	Matrix<T>* variables()   { return ctx.vars; };
	Vector<T>* function()    { return ctx.func; };
	Vector<T>& netChange()   { return ctx.netEval; }
	const Vector<T>* constraints() const { return ctx.constr; }
	const Matrix<T>* variables()   const { return ctx.vars; };
	const Vector<T>* function()    const { return ctx.func; };
	const Vector<T>& netChange()   const { return ctx.netEval; }
	bool optimal() const { return ctx.optimal; }
	bool unbounded() const { return ctx.unbounded; }
	bool degenerated() const { return ctx.unbounded; }

	void optimize() {
		if (not ctx.vars)
			throw std::runtime_error("simplex: no input variables matrix");
		if (not ctx.constr)
			throw std::runtime_error("simplex: no constraints vector");
		if (not ctx.func)
			throw std::runtime_error("simplex: no function vector");

		init();

		while (iterate());

		for (auto const& x : ctx.bas)
			if (x == 0) {
				ctx.degenerated = true;
				break;
			}
		
	}

	/**
	 * @brief Contains calculation context
	 */
	struct Context {
		Matrix<T>* vars; /// Variables
		Vector<T>* constr; /// Constraints
		const Vector<T>* func; /// Function
		Vector<T> unitCon; /// Unit contibution
		Vector<T> netEval; /// Net evaluation
		Vector<T> bas; /// Basis

		T f; /// Function value

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

	friend std::ostream& operator<<(std::ostream& os, const Simplex<T>& s) {
		s.variables()->system(*s.constraints(), os);
		return os << std::boolalpha 
			<< "Optimal: " <<  s.optimal()
			<< " | Unbounded: " << s.unbounded()
			<< " | Degenerated: " <<  s.degenerated()
			<< " | Max: " << s.f();
	}

private:
	/**
	 * @brief Main iteration function
	 *
	 * @return `true`, if iteration should continue, `false` otherwise
	 */
	bool iterate() {
		if (p_iterationCallback)
			p_iterationCallback(ctx);
		std::size_t col = pivotCol();
		if (ctx.optimal)
			return false;

		std::size_t row = pivotRow(col);

		if (ctx.unbounded)
			return false;

		pivot(row, col);

		return true;
	}

	/**
	 * @brief Find next pivot column
	 *
	 * @return column index
	 */
	std::size_t pivotCol() {
		T val = 0;
		std::size_t col;
		for (std::size_t i = 0; i < s_c; i++) {
			if (ctx.netEval[i] > val) {
				val = ctx.netEval[i];
				col = i;
			}
		}
		
		if (val == 0)
			ctx.optimal = true;

		return col;
	}

	/**
	 * @brief Find next pivot row
	 *
	 * @return row index
	 */
	std::size_t pivotRow(std::size_t col) {
		T val = std::numeric_limits<T>::max();
		std::optional<std::size_t> row;
		for (std::size_t i = 0; i < s_r; i++) {
			if (ctx.vars->at(i)[col] <= 0)
				continue;
			T tmp = ctx.constr->at(i) / ctx.vars->at(i)[col];
			if (tmp < val) {
				val = tmp;
				row = i;
			}
		}

		if (not row) {
			row = 0;
			ctx.unbounded = true;
		}

		return *row;
	}

	/**
	 * @brief Perform pivoting
	 *
	 * @param row pivot row
	 * @param col pivot col
	 */
	void pivot(std::size_t row, std::size_t col) {
		T piv = ctx.vars->at(row)[col];
		ctx.bas[row] = ctx.func->at(col);

		for (auto& e: ctx.vars->at(row))
			e /= piv;
		ctx.constr->at(row) /= piv;

		for (std::size_t i = 0; i < row; i++) {
			T re = ctx.vars->at(i)[col];
			for (std::size_t j = 0; j < s_c; j++)
				ctx.vars->at(i)[j] -= re*ctx.vars->at(row)[j];
			ctx.constr->at(i) -= re*ctx.constr->at(row);
		}
		for (std::size_t i = row + 1; i < s_r; i++) {
			T re = ctx.vars->at(i)[col];
			for (std::size_t j = 0; j < s_c; j++)
				ctx.vars->at(i)[j] -= re*ctx.vars->at(row)[j];
			ctx.constr->at(i) -= re*ctx.constr->at(row);
		}

		for (std::size_t i = 0; i < s_c; i++) {
			ctx.unitCon[i] = 0;
			for (std::size_t j = 0; j < s_r; j++)
				ctx.unitCon[i] += ctx.bas[j]*ctx.vars->at(j)[i];
			ctx.netEval[i] = ctx.func->at(i) - ctx.unitCon[i];
		}

		ctx.f = ctx.bas * *ctx.constr; // dot product;

		if (p_pivotCallback)
			p_pivotCallback(ctx, row, col);
	}

	/**
	 * @brief Initialize simplex tableau
	 */
	void init() {
		s_r = ctx.vars->rows();
		s_c = ctx.vars->cols();

		ctx.f = 0;

		ctx.unbounded = false;
		ctx.optimal = false;
		ctx.degenerated = false;
		ctx.unitCon.resize(s_c, 0);
		ctx.netEval = *ctx.func;
		ctx.bas.resize(s_r, 0);

		if (p_initCallback)
			p_initCallback(ctx);
	}

private:
	Context ctx;

	std::size_t s_r, s_c;
};


}

#include <calgo/nomacros.hpp>
