#ifndef _CALGO_OPT_DIJKSTRA_HPP_
#define _CALGO_OPT_DIJKSTRA_HPP_

#include <calgo/vec.hpp>
#include <calgo/mat.hpp>

namespace ca::opt {

/**
 * @brief Dijkstra algorithm on adjacency matrix graph
 * @tparam T `distance_type`
 */
template<typename T>
class dijkstra {
public:
	using distance_type = T;
	using size_type = typename ca::vec<distance_type>::size_type;

public:
	/**
	 * @brief Get vertices count
	 * @return vertices count
	 */
	size_type verices_count() { return m_distances.n(); }
	/**
	 * @brief Get distances (solution)
	 *
	 * @return distances from source to \f(i\f)-th vertice
	 */
	ca::vec<distance_type>& distances() {
		return m_distances;
	}
	/**
	 * @brief Set source vertex
	 *
	 * @param source new source vertice index
	 */
	void set_source_vertex(const size_type& source) { m_source_vertex = source; }
	/**
	 * @brief Get source vertex
	 *
	 * @return source vertex index
	 */
	const size_type& source_vertex() { return m_source_vertex; }

	inline void check(const ca::mat<distance_type>& adjacency);
	void solve(const ca::mat<distance_type>& adjacency);
	inline void solve_safe(const ca::mat<distance_type>& adjacency) { check(adjacency); solve(adjacency); }

protected:
	ca::vec<distance_type> m_distances;
	size_type m_source_vertex = 0;
};

}

#include "../../../src/dijkstra.inl"

#endif // !_CALGO_OPT_DIJKSTRA_HPP_
