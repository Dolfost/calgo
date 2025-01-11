#pragma once
#include <calgo/opt/packing2D.hpp>

namespace ca::opt {

template<typename T>
void TreeFit2D<T>::pack(std::vector<Box2D<T>*>& in) {
	if (this->defaultFinish(in))
		return;
	Packing2D<T>::pack(in);

	t_root = new Node(in[0]->size(), 0, 0);
	std::size_t boxCount = in.size();
	for (std::size_t i = 0; i < boxCount; i++) {
		Box2D<T>* box = in[i];
		Node* node = nullptr;
		if ((node = findNode(box->size())))
			node = splitNode(node, box->size());
		else
			node = growNode(box->size());
		box->setPosition(node->x(), node->y());

		if (this->p_boxPackedCallback)
			this->p_boxPackedCallback(in, i);
	}

	this->setSize(t_root->size());

	delete t_root;
	t_root = nullptr;

	if (this->p_boxesPackedCallback)
		this->p_boxesPackedCallback(in);
}

template<typename T>
typename TreeFit2D<T>::Node* TreeFit2D<T>::findNode(Node* root, const Size2D<T>& size) {
	if (root->used()) {
		Node* node = findNode(root->right(), size); 
		if (node) return node;
		return findNode(root->down(), size);
	} else if (size <= root->size())
		return root;
	return nullptr;
}

template<typename T>
typename TreeFit2D<T>::Node* TreeFit2D<T>::splitNode(Node* node, const Size2D<T>& size) {
	node->setUsed(true);
	node->setDown(
		new Node(
			ca::opt::Size2D<T>(
				node->size().width(), 
				node->size().height() - size.height()
			),
			node->x(), node->y() + size.height()
		)
	);
	node->setRight(
		new Node(
			ca::opt::Size2D<T>(
				node->size().width() - size.width(), 
				size.height()
			),
			node->x() + size.width(), node->y()
		)
	);
	return node;
}

template<typename T>
typename TreeFit2D<T>::Node* TreeFit2D<T>::growNode(const Size2D<T>& size) {
	bool canGrowDown = (size.width() <= t_root->size().width());
	bool canGrowRigth = (size.height() <= t_root->size().height());
	bool shouldGrowRight = 
		(canGrowRigth and (t_root->size().height() >= (t_root->size().width() + size.width())));
	bool shouldGrowDown = 
		(canGrowDown and (t_root->size().width() >= (t_root->size().height() + size.height())));

	if (shouldGrowRight)
		return growRight(size);
	else if (shouldGrowDown)
		return growDown(size);
	else if (canGrowRigth)
		return growRight(size);
	else if (canGrowDown)
		return growDown(size);
	return nullptr; // should not happen if root size is correct
}

template<typename T>
typename TreeFit2D<T>::Node* TreeFit2D<T>::growRight(const Size2D<T>& size) {
	Node* node = new Node(
		Size2D<T>(t_root->size().width() + size.width(), t_root->size().height()),
		0, 0
	);
	node->setUsed(true);
	node->setDown(t_root);
	node->setRight(
		new Node(
			Size2D<T>(size.width(), t_root->size().height()), 
			t_root->size().width(), 0
		)
	);
	t_root = node;

	node = nullptr;
	if ((node = findNode(size)))
		 return splitNode(node, size);
	return nullptr;
}

template<typename T>
typename TreeFit2D<T>::Node* TreeFit2D<T>::growDown(const Size2D<T>& size) {
	Node* node = new Node(
		Size2D<T>(t_root->size().width(), t_root->size().height() + size.height()),
		0, 0
	);
	node->setUsed(true);
	node->setDown(
		new Node(
			Size2D<T>(t_root->size().width(), size.height()), 
			0, t_root->size().height()
		)
	);
	node->setRight(t_root);
	t_root = node;

	node = nullptr;
	if ((node = findNode(size)))
		 return splitNode(node, size);
	return nullptr;
}

}
