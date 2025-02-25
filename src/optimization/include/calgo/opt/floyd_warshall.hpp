#ifndef _CALGO_OPT_FLOYD_WARSHALL_HPP_
#define _CALGO_OPT_FLOYD_WARSHALL_HPP_

#include <calgo/vec.hpp>
#include <calgo/mat.hpp>

#include <limits>

namespace ca::opt {

/**
 * @brief Floyd-Warshall algorithm on adjacency matrix graph
 * Where there is not edget between two vertices, it should be marked as with
 * weight no_edge.
 * @tparam T `distance_type`
 */
template<typename T>
class floyd_warshall {
public:
	using distance_type = T;
	using size_type = typename ca::vec<distance_type>::size_type;
	static const constexpr distance_type no_edge = std::numeric_limits<distance_type>::max();

public:
	/**
	 * @brief Get vertices count
	 * @return vertices count
	 */
	size_type verices_count() { return m_distances.rows(); }
	/**
	 * @brief Get distances (solution)
	 *
	 * @return distances from source to \f(i\f)-th vertice
	 */
	ca::mat<distance_type>& distances() {
		return m_distances;
	}

	inline void check(const ca::mat<distance_type>& adjacency);
	template<typename P> typename 
	std::enable_if<std::is_assignable<ca::mat<distance_type>, P>::value>::type solve(P && adjacency) {
		m_distances = std::forward<P>(adjacency);
		m_solve_impl();
	}

	template<typename P> typename 
	std::enable_if<std::is_assignable<ca::mat<distance_type>, P>::value>::type solve_safe(P && adjacency) {
		check(adjacency); solve(std::forward<P>(adjacency));
	}

private:
	void m_solve_impl();

protected:
	ca::mat<distance_type> m_distances;
};

}

#include "../../../src/floyd_warshall.inl"

#endif // !_CALGO_OPT_FLOYD_WARSHALL_HPP_
