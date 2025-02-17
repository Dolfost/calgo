#pragma once
#include <calgo/opt/dijkstra.hpp>

#include <stdexcept>
#include <vector>
#include <limits>

namespace ca::opt {

template<typename T>
void dijkstra<T>::check(const ca::mat<distance_type>& adjacency) {
	if (not adjacency.is_square())
		throw std::runtime_error("ca::opt::dijkstra: matrix is not square");
	if (m_source_vertex >= adjacency.rows()) // or adjacency.cols()
		throw std::out_of_range("ca::opt::dijkstra: source vertex is out of range");
}

template<typename T>
void dijkstra<T>::solve(const ca::mat<distance_type>& adjacency) {
	size_type n = adjacency.rows();
	m_distances.resize(n);

	std::vector<bool> shortest_path_tree(n); 

	for (size_type i = 0; i < n; i++) {
		m_distances[i] = std::numeric_limits<distance_type>::max(); 
		shortest_path_tree[i] = false;
	}

	// distance from source vertex to itself is 0
	m_distances[m_source_vertex] = 0;

	// find shortest path for all vertices
	for (size_type count = 0; count < n - 1; count++) {
		distance_type min = std::numeric_limits<distance_type>::max();
		size_type min_index;

		for (size_type v = 0; v < n; v++)
			if (shortest_path_tree[v] == false && m_distances[v] <= min)
				min = m_distances[v], min_index = v;

		// mark the vertex as done
		shortest_path_tree[min_index] = true;

		// update m_distances[v] only if it is not processed
		// if there is an edge from u to v, and total
		// weight of path from src to v through u is
		// smaller than current value of m_distances[v]
		for (int v = 0; v < n; v++)
			if (not shortest_path_tree[v] and adjacency(min_index, v)
				and m_distances[min_index] != std::numeric_limits<distance_type>::max()
				and m_distances[min_index] + adjacency(min_index, v) < m_distances[v])
				m_distances[v] = m_distances[min_index] + adjacency(min_index, v);
	}
}

}
