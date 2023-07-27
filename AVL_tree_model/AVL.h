#include <memory>
#include <string>

class AVL;

class AVLNode {
 public:
 	AVLNode(int key);
 	AVLNode(int key, std::weak_ptr<AVLNode> parent);
 	bool IsLeaf() const;
 	//bool IsMissingChild() const;
 	bool HasLeftChild() const;
 	bool HasRightChild() const;
 	void DeleteChild(std::shared_ptr<AVLNode> v);
 	void ReplaceChild(std::shared_ptr<AVLNode> v, std::shared_ptr<AVLNode> u);

 private:
  int key_;
  std::weak_ptr<AVLNode> parent_;
  std::shared_ptr<AVLNode> left_;
  std::shared_ptr<AVLNode> right_;
  int height;
  int bf;

  friend AVL;
}; 

class AVL {
 public:
 	AVL();

 	void Insert(int key);
 	bool Delete(int key);
 	bool Find(int key) const;
 	std::string JSON() const;
 	size_t size() const;
 	bool empty() const;
 	int DeleteMin();

	void updateHeight_BF(std::shared_ptr<AVLNode> node);
	void rightRotate(std::shared_ptr<AVLNode> node);
	void leftRotate(std::shared_ptr<AVLNode> node);
	void rotation_check(std::shared_ptr<AVLNode> node);

 private:
	void DeleteLeaf(std::shared_ptr<AVLNode> currentNode);
	int DeleteMin(std::shared_ptr<AVLNode> currentNode);

 	std::shared_ptr<AVLNode> root_;
 	size_t size_;
	size_t height;
}; // class AVL