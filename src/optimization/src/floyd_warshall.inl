#pragma once
#include <calgo/opt/floyd_warshall.hpp>

#include <stdexcept>
#include <vector>
#include <limits>

namespace ca::opt {

template<typename T>
void floyd_warshall<T>::check(const ca::mat<distance_type>& adjacency) {
	if (not adjacency.is_square())
		throw std::runtime_error("ca::opt::floyd_warshall: adjacency matrix is not square");
}

template<typename T>
void floyd_warshall<T>::m_solve_impl() {
	size_type n = m_distances.rows();

	for (size_type k = 0; k < n; k++)
		for (size_type 	i = 0; i < n; i++)
			for (size_type j = 0; j < n; j++)
				// if vertex k is on the shortest path from i to j, then update the
				// value of m_distances[i][j]
				if (m_distances(i, j) > (m_distances(i, k) + m_distances(k, j))
					and (m_distances(k, j) != no_edge
					and m_distances(i, k) != no_edge))
					m_distances(i, j) = m_distances(i, k) + m_distances(k, j);
}

}
