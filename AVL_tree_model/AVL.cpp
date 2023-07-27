#include "AVL.h"

#include <cassert>
#include <iostream>
#include <string>
#include <queue>
#include <algorithm>

#include "json.hpp"


AVLNode::AVLNode(int key) :
	key_(key),
	parent_(std::weak_ptr<AVLNode>()),
	left_(nullptr),
	right_(nullptr) {}

AVLNode::AVLNode(int key, std::weak_ptr<AVLNode> parent) :
	key_(key),
	parent_(parent),
	left_(nullptr),
	right_(nullptr) {}

bool AVLNode::IsLeaf() const {
	return left_ == nullptr && right_ == nullptr;
}

bool AVLNode::HasLeftChild() const {
	return left_ != nullptr;
}

bool AVLNode::HasRightChild() const {
	return right_ != nullptr;
}

void AVLNode::DeleteChild(std::shared_ptr<AVLNode> v) {
	if (left_ == v) {
		left_ = nullptr;
	} else if (right_ == v) {
		right_ = nullptr;
	} else {
		std::cerr << "AVLNode::DeleteChild Error: non-child passed as argument\n";
		exit(EXIT_FAILURE);
	}
}

void AVLNode::ReplaceChild(std::shared_ptr<AVLNode> v, std::shared_ptr<AVLNode> u) {
	if (left_ == u || right_ == u) {
		std::cerr << "AVLNode::ReplaceChild Error: child passed as replacement\n";
	}
	if (left_ == v) {
		left_ = u;
		u->parent_ = v->parent_;
	} else if (right_ == v) {
		right_ = u;
		u->parent_ = v->parent_;
	} else {
		std::cerr << "AVLNode::ReplaceChild Error: non-child passed as argument\n";
		exit(EXIT_FAILURE);
	}
}

//-----------------------------------------------------------------------------------

AVL::AVL() : root_(nullptr), size_(0) {}

void AVL::Insert(int key) {
	if (root_ == nullptr) {
		root_ = std::make_shared<AVLNode>(key);
		size_++;
		return;
	}
	std::shared_ptr<AVLNode> currentNode = root_, lastNode = nullptr, node;
	while (currentNode != nullptr) {
		lastNode = currentNode;
		currentNode = (key < currentNode->key_) ?
			currentNode->left_ : currentNode->right_;
	}
	if (key < lastNode->key_) {
		lastNode->left_ = std::make_shared<AVLNode>(key, lastNode);
		node = lastNode->left_;
	} else {
		lastNode->right_ = std::make_shared<AVLNode>(key, lastNode);
		node = lastNode->right_;
	}
	size_++;
	updateHeight_BF(node);
	rotation_check(node);
	
}

void AVL::updateHeight_BF(std::shared_ptr<AVLNode> node)
{
	while(node != nullptr)
	{
		if(node->IsLeaf())
		{
			node->height = 0;
			node->bf = 0;
		}
		else if (node->HasLeftChild() && node->HasRightChild())
		{
			node->height = 1 + std::max(node->left_->height, node->right_->height);
			node->bf = node->right_->height - node->left_->height;
		}
		else if (node->HasLeftChild() && !node->HasRightChild())
		{
			node->height = 1 + node->left_->height;
			node->bf = -1 - node->left_->height;
		}
		else if (!node->HasLeftChild() && node->HasRightChild())
		{
			node->height = 1+node->right_->height;
			node->bf = node->right_->height+1;
		} 
		node = node->parent_.lock();
	}
}

void AVL::rotation_check(std::shared_ptr<AVLNode> node)
{
	while(node != nullptr)
	{
		if(node->bf < -1) // left heavy
		{
			if(node->left_->bf < 0) // left left case =>  right rotation
			{
				rightRotate(node);
			} 
			else if (node->left_->bf > 0) // left right case =>  RL rotation
			{
				leftRotate(node->left_);
				rightRotate(node);
			}
		}
		else if (node->bf > 1) // right heavy
		{
			if(node->right_->bf >0) // right right case => left rotation
			{
				leftRotate(node);
			} 
			else if(node->right_->bf < 0) // right left case => LR rotations
			{
				rightRotate(node->right_);
				leftRotate(node);
			}
		}
		node = node->parent_.lock();
	}
}

void AVL::rightRotate(std::shared_ptr<AVLNode> node)
{
	int has_right_sub = 0;
	std::shared_ptr<AVLNode> child = node->left_;
	if(root_ == node){ // if the node being rotated is the root
		//rotation
		std::shared_ptr<AVLNode> child = node->left_;
		if(child->HasRightChild()){
			node->left_ = child->right_;
			has_right_sub = 1;
		} 
		child->right_ = node;
		// update parent relationship
		node->parent_ = child;
		if(has_right_sub==1) node->left_->parent_ = node;
		// update the new root (child)'s parent pointer
		root_ = child;
		child->parent_.reset();
		// make sure the leaf are clean (node's right_ and left_ should be nullptr)
		if(has_right_sub ==0 && !root_->right_->IsLeaf()) root_->right_->left_ = nullptr;
	}	
	else if (root_ != node)// if the node being rotated is NOT the root (i.e., it has a parent)
	{
		std::shared_ptr<AVLNode> parent = node->parent_.lock();
		// rotation
		if(child->HasRightChild()){
			node->left_ = child->right_;
			has_right_sub = 1;
		} 
		child->right_ = node;
		// update parent relationship
		node->parent_ = child;
		if(has_right_sub==1) node->left_->parent_ = node;
		// update the new root (child)'s parent pointer
		if(parent->key_ > node->key_) parent->left_ = child;
		else parent->right_ = child;
		child->parent_ = parent;
		// make sure the leaf are clean (node's right_ and left_ should be nullptr)
		if(has_right_sub ==0 && !node->IsLeaf()) node->left_ = nullptr;
	}
	updateHeight_BF(node);
}

void AVL::leftRotate(std::shared_ptr<AVLNode> node)
{
	int has_left_sub = 0;
	std::shared_ptr<AVLNode> child = node->right_;
	if(root_ == node){ // if the node being rotated is the root
		// rotation
		if(child->HasLeftChild()){
			has_left_sub = 1;
			node->right_ = child->left_;
		}
		child->left_ = node;
		// update parent relationships
		node->parent_ = child;
		if(has_left_sub==1) node->right_->parent_ = node;
		// update the new root (child)'s parent pointer
		root_ = child;
		child->parent_.reset();
		// make sure the leaf are clean (right_ and left_ of leaf should be nullptr)
		if(has_left_sub==0 && !root_->left_->IsLeaf()) root_->left_->right_ = nullptr;
	}
	else if (root_ != node)// if the node being rotated is NOT the root (i.e., it has a parent)
	{
		std::shared_ptr<AVLNode> parent = node->parent_.lock();
		// rotation
		if(child->HasLeftChild())
		{
			has_left_sub = 1;
			node->right_ = child->left_;
		} 
		child->left_ = node;
		// update parent relationships
		node->parent_ = child;
		if(has_left_sub==1) node->right_->parent_ = node;
		// update the new root (child)'s parent pointer
		if(parent->key_ > node->key_) parent->left_ = child;
		else parent->right_ = child;
		child->parent_ = parent;
		// make sure the leaf are clean (right_ and left_ of leaf should be nullptr)
		if(has_left_sub==0 && !node->IsLeaf()) node->right_ = nullptr;
	}
	updateHeight_BF(node);
}



bool AVL::Delete(int key) {
	std::shared_ptr<AVLNode> currentNode = root_;
	while (currentNode != nullptr) {
		if (currentNode->key_ == key) {
			if (currentNode->IsLeaf()) {
				DeleteLeaf(currentNode);
			} else if (currentNode->left_ == nullptr) {
				assert(currentNode->right_ != nullptr);
				std::shared_ptr<AVLNode> parent = currentNode->parent_.lock();
				parent->ReplaceChild(currentNode, currentNode->right_);
				size_--; assert(size_ >= 0);
			} else if (currentNode->right_ == nullptr) {
				assert(currentNode->left_ != nullptr);
				std::shared_ptr<AVLNode> parent = currentNode->parent_.lock();
				parent->ReplaceChild(currentNode, currentNode->left_);
				size_--; assert(size_ >= 0);
			} else {
				currentNode->key_ = DeleteMin(currentNode);
			}
		}
		currentNode = (key < currentNode->key_) ?
			currentNode->left_ : currentNode->right_;
	}
	return false;
}

int AVL::DeleteMin() {
	return DeleteMin(root_);
}


void AVL::DeleteLeaf(std::shared_ptr<AVLNode> currentNode) {
	std::shared_ptr<AVLNode> parent = currentNode->parent_.lock();
	if (parent == nullptr) {
		// Delete root
		root_ = nullptr;
		size_--; assert(size_ == 0);
	} else {
		if (parent->right_ == currentNode) {
			parent->right_ = nullptr;
		} else if (parent->left_ == currentNode) {
			parent->left_ = nullptr;
		} else {
			std::cerr << "AVL::DeleteLeaf Error: inconsistent state\n";
		}
		size_--; assert(size_ >= 0);
	}
}

int AVL::DeleteMin(std::shared_ptr<AVLNode> currentNode) {
	std::shared_ptr<AVLNode> lastNode = nullptr;
	while (currentNode != nullptr) {
		lastNode = currentNode;
		currentNode = currentNode->left_;
	}
	int result = lastNode->key_;
	std::shared_ptr<AVLNode> parent = lastNode->parent_.lock();
	if (parent == nullptr) {
		// lastNode is root
		if (lastNode->right_ != nullptr) {
			root_ = lastNode->right_;
			lastNode->right_->parent_.reset();
		} else {
			root_ = nullptr;
		}
	} else {
		// lastNode under the root
		if (lastNode->right_ != nullptr) {
			parent->left_ = lastNode->right_;
			lastNode->right_->parent_ = parent;
		} else {
			parent->left_ = nullptr;
		}
  }
	size_--; assert(size_ >= 0);
	return result;
}

size_t AVL::size() const {
	return size_;
}

bool AVL::empty() const {
	return size_ == 0;
}

bool AVL::Find(int key) const {
	std::shared_ptr<AVLNode> currentNode = root_;
	while (currentNode != nullptr) {
		if (currentNode->key_ == key) {
			return true;
		}
		currentNode = (key < currentNode->key_) ?
			currentNode->left_ : currentNode->right_;
	}
	return false;
}

std::string AVL::JSON() const {
	nlohmann::json result;
	std::queue< std::shared_ptr<AVLNode> > nodes;
	if (root_ != nullptr) {
		result["root"] = root_->key_;
		nodes.push(root_);
		while (!nodes.empty()) {
			//std::cout<<"entered the loop of creating tree. \n";
			auto v = nodes.front();
			nodes.pop();
			std::string key = std::to_string(v->key_);
			if (v->left_ != nullptr) {
				result[key]["left"] = v->left_->key_;
				nodes.push(v->left_);
				//std::cout<<"established the left child \n";
			}
			if (v->right_ != nullptr) {
				result[key]["right"] = v->right_->key_;
				nodes.push(v->right_);
				//std::cout<<"established the right child \n";
			}
			if (v->parent_.lock() != nullptr) {
				result[key]["parent"] = v->parent_.lock()->key_;
				//std::cout<<"established the parent \n";
			} else {
				result[key]["root"] = true;
			}
			result[key]["height"] = v->height;
			result[key]["balance factor"] = v->bf;
		}
	}
	result["height"] = root_->height;
	result["size"] = size_;
	return result.dump(2) + "\n";
}
