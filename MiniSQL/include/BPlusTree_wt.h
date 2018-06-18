#ifndef BPLUSTREE_WT
#define BPLUSTREE_WT

#include "GlobalVar.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

template <class T>
struct KEY
{
	T value;
	int offset;
};

template <class T>
struct Node
{
	vector <KEY<T>> keys;
	int keyNum;
	struct Node<T>* parentNode;
	vector <struct Node<T>*> childNode;
};

template <class T>
struct SearchNode
{
	struct Node<T>* ansNode;
	int position;
	bool ifFind;
};
template <class T>
class BPlusTree
{
public:
	BPlusTree(int type);
	~BPlusTree();
	Node<T>* GetRoot();
	SearchNode<T> SearchKey(KEY<T> key);
	bool InsertKey(KEY<T> key);
	bool DeleteKey(KEY<T> key);
	void PrintTree(string filename);
protected:
	void Destroy(Node<T>* node);
	Node<T>* CreateNode();
	SearchNode<T> Search(KEY<T> key, Node<T>* node);
	bool InsertNode(Node<T>* node, int pos, KEY<T> key);
	bool DeleteNode(Node<T>* node, int pos);
	void LegalizeNode(Node<T>* node, KEY<T> value1, KEY<T> value2, int pos);
	void SetMinKey(Node<T>* node, KEY<T> previous, KEY<T> now);
	void Split(Node<T>* node);
	void BorrowOrCombine(Node<T>* node, KEY<T> value1, KEY<T> value2, int pos);
	void PrintNode(string filename, Node<T>* node);
private:
	Node<T>*  Root;
	int Size;
	int Upper;
	int Lower;
};

template <class T>
BPlusTree<T>::BPlusTree(int type):Root(NULL)
{
	Size = floor((float)MAX_PAGE_SIZE / type);
	Lower = ceil((float)Size / 2);
	Upper = Size;
}

template <class T>
BPlusTree<T>::~BPlusTree()
{
	if (Root)
	{
		Destroy(Root);
	}
}

template <class T>
void BPlusTree<T>::Destroy(Node<T>* Node)
{
}

template <class T>
Node<T>* BPlusTree<T>::GetRoot()
{
	return Root;
}

template <class T>
Node<T>* BPlusTree<T>::CreateNode()
{
	Node<T>* node = new Node<T>;
	node->keyNum = 0;
	node->parentNode = NULL;
	return node;
}

template <class T>
SearchNode<T> BPlusTree<T>::SearchKey(KEY<T> key)
{
	if (!Root)
	{
		return{ NULL, -1, false };
	}
	else
	{
		return{ Search(key, Root) };
	}
}

template <class T>
SearchNode<T> BPlusTree<T>::Search(KEY<T> key, Node<T>* node)
{
	int pos = -1;
	bool ifFind = false;
	int i = 0;
	for (i = 0; i < node->keyNum; i++)
	{
		if (node->keys[i].value >= key.value)
		{
			pos = i;
			if (node->keys[i].value == key.value)
			{
				ifFind = true;
				if (node->childNode[0])//If not a leaf node, the next position to search is the keys[i]'s childNode.
				{
					pos++;
				}
			}
			break;
		}
	}
	if (i == node->keyNum)
	{
		pos = node->keyNum;
	}
	if (!node->childNode[0]) //If no children, return the result.
	{
		return{ node, pos, ifFind };
	}
	else
	{
		return{ Search(key, node->childNode[pos]) };
	}
}

template <class T>
bool BPlusTree<T>::InsertKey(KEY<T> key)
{
	if (!Root)
	{
		Root = CreateNode();
		Root->keys.push_back(key);
		Root->childNode.push_back(NULL);
		Root->childNode.push_back(NULL);
		Root->keyNum = 1;
		return true;
	}
	else 
	{
		SearchNode<T> Insertion = SearchKey(key);
		if (Insertion.ifFind) return false;
		else
		{
			return InsertNode(Insertion.ansNode, Insertion.position, key);
		}
	}
}

template <class T>
bool BPlusTree<T>::InsertNode(Node<T>* node, int pos, KEY<T> key)
{
	node->keys.insert(node->keys.begin() + pos, key);
	node->childNode.push_back(NULL);
	node->keyNum++;
	if (node->keyNum > Upper)
	{
		LegalizeNode(node, key, key, pos);
	}
	return true;
}

template <class T>
bool BPlusTree<T>::DeleteKey(KEY<T> key)
{
	SearchNode<T> Deletion = SearchKey(key);
	if (!Deletion.ifFind || !Deletion.ansNode)
	{
		return false;
	}
	else
	{
		return DeleteNode(Deletion.ansNode, Deletion.position);
	}
}

template <class T>
bool BPlusTree<T>::DeleteNode(Node<T>* node, int pos)
{
	KEY<T> value1;
	KEY<T> value2;
	value1 = node->keys[pos];
	node->keys.erase(node->keys.begin() + pos);
	node->childNode.erase(node->childNode.begin() + pos + 1);
	node->keyNum--;
	value2 = node->keys[0];
	if (node->keyNum >= Lower)
	{
		if (pos == 0)
		{
			SetMinKey(node, value1, value2);
		}
	}
	else
	{
		LegalizeNode(node, value1, value2, pos);
	}
	return true;
}

template <class T>
void BPlusTree<T>::SetMinKey(Node<T>* node, KEY<T> previous, KEY<T> now)
{
	if (node != Root)
	{
		Node<T> *parent = node->parentNode;
		int i = 0;
		for (i = 0; i < parent->keyNum; i++)
		{
			if (parent->keys[i].value == previous.value)
			{
				break;
			}
		}
		if (i == parent->keyNum)
		{
			SetMinKey(parent, previous, now); //If the parents do not consist of the previous value, find its parents?
		}
		else
		{
			parent->keys[i] = now; //Update the value in the parent node.
			return;
		}
	}
	else
	{
		for (int i = 0; i < node->keyNum; i++)
		{
			if (node->keys[i].value == previous.value)
			{
				node->keys[i] = now;
				break;
			}
		}
		return;
	}
	return;
}

template <class T>
void BPlusTree<T>::LegalizeNode(Node<T>* node, KEY<T> value1, KEY<T> value2, int pos)
{
	if (!node) return;
	int num;
	if (!node->childNode[0]) //If the node is a leaf node
	{
		num = node->keyNum;
	}
	else //If the node is not a leaf node
	{
		num = node->childNode.size();
	}
	if (num < Lower)
	{
		BorrowOrCombine(node, value1, value2, pos);
		LegalizeNode(node->parentNode, value1, value2, pos);
		return;
	}
	if (num > Upper)
	{
		Split(node);
		LegalizeNode(node->parentNode, value1, value2, pos);
		return;
	}
	return;
}

template <class T>
void BPlusTree<T>::BorrowOrCombine(Node<T>* node, KEY<T> value1, KEY<T> value2, int pos)
{
	if (!node) return;
	if (node == Root)
	{
		if (!node->childNode[0]) return; //Only a root exists
		else if (node->childNode.size() == 1) //Only a child (the root original key is deleted)
		{
			Root = node->childNode[0];
			Root->parentNode = NULL;
			node = Root;
			return;
		}
		return;
	}
	//find the left and right sibling and their parents position
	Node<T> *Left, *Right;
	Node<T> *parent = node->parentNode;
	int LeftParentPos, RightParentPos;
	int i = 0;
	for (i = 0; i < parent->keyNum + 1; i++)
	{
		if (parent->childNode[i] == node) break;
	}
	if (i == 0)
	{
		Left = NULL;
		Right = parent->childNode[i + 1];
		RightParentPos = i + 1;
	}
	else if (i == parent->keyNum)
	{
		Right = NULL;
		Left = parent->childNode[i - 1];
		LeftParentPos = i - 1;
	}
	else
	{
		Left = parent->childNode[i - 1];
		Right = parent->childNode[i + 1];
		LeftParentPos = i - 1;
		RightParentPos = i + 1;
	}

	KEY<T> v1, v2;
	if (!node->childNode[0]) //If it is a leaf node
	{
		if (Right && Right->keyNum > Lower) //Then borrow from the right sibling
		{
			v1 = Right->keys[0];
			v2 = Right->keys[1];
			node->keyNum++;
			node->keys.push_back(Right->keys[0]);
			Right->keys.erase(Right->keys.begin());
			node->childNode.push_back(NULL);
			Right->childNode.erase(Right->childNode.begin());
			Right->keyNum--;
			SetMinKey(Right, v1, v2);
			if (pos == 0)
			{
				SetMinKey(node, v1, v2);
			}
			return;
		}
		else if (Left && Left->keyNum > Lower) //Then borrow from the left sibling
		{
			node->keyNum++;
			node->keys.insert(node->keys.begin(), Left->keys[Left->keyNum - 1]);
			Left->keys.erase(Left->keys.begin() + Left->keyNum - 1);
			node->childNode.push_back(NULL);
			Left->childNode.erase(Left->childNode.begin() + Left->keyNum-1);
			Left->keyNum--;
			value2 = node->keys[0];
			SetMinKey(node, value1, value2);
			return;
		}
		else if (Right && Right->keyNum == Lower) //Then combine with the right sibling
		{
			for (int j = 0; j < Lower; j++)
			{
				node->keys.push_back(Right->keys[j]);
				node->childNode.push_back(NULL);
				node->keyNum++;
			}
			parent->keys.erase(parent->keys.begin() + RightParentPos-1);
			parent->childNode.erase(parent->childNode.begin() + RightParentPos);
			parent->keyNum--;
			SetMinKey(node, value1, value2);
			delete Right;
			return;
		}
		else if (Left && Left->keyNum == Lower) //Then combine with the left sibling
		{
			for (int j = 0; j < Lower; j++)
			{
				Left->keys.push_back(node->keys[j]);
				Left->childNode.push_back(NULL);
				Left->keyNum++;
			}
			parent->keys.erase(parent->keys.begin() + LeftParentPos);
			parent->childNode.erase(parent->childNode.begin() + LeftParentPos + 1);
			parent->keyNum--;
			node = Left;
			return;
		}
	}
	else //If not a leaf node 
	{
		if (Right && Right->childNode.size() > Lower) //Then borrow from the right sibling
		{
			v1 = Right->childNode[0]->keys[0];
			v2 = Right->childNode[1]->keys[0];
			node->childNode.push_back(Right->childNode[0]);
			SetMinKey(Right->childNode[0], v1, v2);
			Right->childNode[0]->parentNode = node;

			Right->childNode.erase(Right->childNode.begin());
			Right->keys.erase(Right->keys.begin());
			node->keys.push_back(v1);
			node->keyNum++;
			Right->keyNum--;
			return;
		}
		if (Left && Left->childNode.size() > Lower) //Then borrow from the left sibling
		{
			v1 = node->childNode[0]->keys[0];
			v2 = Left->childNode[Left->keyNum]->keys[0];
			node->childNode.insert(node->childNode.begin(), Left->childNode[Left->keyNum]);
			SetMinKey(node->childNode[0], v1, v2);
			Left->childNode[Left->keyNum]->parentNode = node;

			Left->childNode.erase(Left->childNode.begin() + Left->keyNum);
			Left->keys.erase(Left->keys.begin() + Left->keyNum - 1);
			node->keys.insert(node->keys.begin(), v2);
			node->keyNum++;
			Left->keyNum--;
			return;
		}
		if (Right && Right->childNode.size() == Lower) //Then combine with the right sibling
		{
			for (int j = 0; j < Right->keyNum + 1; j++) {
				node->childNode.push_back(Right->childNode[j]);
				node->keys.push_back(Right->childNode[j]->keys[0]);
				Right->childNode[j]->parentNode = node;
				node->keyNum++;
			}
			parent->childNode.erase(parent->childNode.begin() + RightParentPos);
			parent->keys.erase(parent->keys.begin() + RightParentPos - 1);
			parent->keyNum--;
			delete Right;
			return;
		}
		if (Left && Left->childNode.size() == Lower) //Then combine with the left sibling
		{
			for (int j = 0; j < node->keyNum + 1; j++) 
			{
				Left->keys.push_back(node->childNode[j]->keys[0]);
				Left->childNode.push_back(node->childNode[j]);
				node->childNode[j]->parentNode = Left;
				Left->keyNum++;
			}
			parent->childNode.erase(parent->childNode.begin() + LeftParentPos + 1);
			parent->keys.erase(parent->keys.begin() + LeftParentPos);
			parent->keyNum--;
			node = Left;
			return;
		}
	}
}

template <class T>
void BPlusTree<T>::Split(Node<T>* node)
{
	int leftNum = Lower - (node->childNode[0] != NULL); //If not leaf node, then leftNum = Lower-1;
	int rightNum = node->keyNum - leftNum;

	Node<T> *rightNode = CreateNode();
	//initialize data into the right node (keyNum and keys)
	rightNode->keyNum = rightNum;
	node->keyNum = leftNum;
	for (int i = 0; i < rightNum; i++)
	{
		rightNode->keys.push_back(node->keys[leftNum]);
		node->keys.erase(node->keys.begin() + leftNum);
	}
	//initialize data into the children of right node (assign childNode)
	for (int i = 0; i < rightNum; i++)
	{
		rightNode->childNode.push_back(node->childNode[leftNum + 1]);
		node->childNode.erase(node->childNode.begin() + leftNum + 1);
	}
	rightNode->childNode.insert(rightNode->childNode.begin(), node->childNode[leftNum]);
	//assign parentNode
	if (rightNode->childNode[0])
	{
		for (int i = 1; i < rightNum + 1; i++)
		{
			rightNode->childNode[i]->parentNode = rightNode;
		}
	}
	//judge whether the node is root
	if (node == Root)
	{
		Root = CreateNode();
		Root->keyNum = 1;
		Root->childNode.push_back(node);
		Root->childNode.push_back(rightNode);
		Root->keys.push_back(rightNode->keys[0]);
		Root->parentNode = NULL;
		if (rightNode->childNode[0])
		{
			rightNode->keyNum--;
			rightNode->keys.erase(rightNode->keys.begin());
			rightNode->childNode.erase(rightNode->childNode.begin());
		}
		node->parentNode = Root;
		rightNode->parentNode = Root;
		return;
	}
	Node<T> *parent = node->parentNode;
	int pos;
	//find the position of parent key
	for (pos = 0; pos < parent->keyNum+1; pos++)
	{
		if (parent->childNode[pos] == node)
		{
			break;
		}
	}
	//split and upload the middle key
	if (pos == 0) //Then the split happens in the first childnode, and keys[pos-1] does not exist.
	{
		parent->keys.push_back(parent->keys[pos]);
		for (int j = parent->keyNum; j > pos; j--) //Note that the final key of parents is parent->key[parent->keyNum-1]
		{
			parent->keys[j] = parent->keys[j - 1];
		}
	}
	else //Then the keys[pos-1] must exist.
	{
		parent->keys.push_back(parent->keys[pos - 1]);
		for (int j = parent->keyNum; j >= pos; j--) //Note that the final key of parents is parent->key[parent->keyNum-1]
		{
			parent->keys[j] = parent->keys[j - 1];
		}
	}
	parent->childNode.push_back(parent->childNode[pos]);
	for (int j = parent->keyNum + 1; j > pos; j--) 
	{
		parent->childNode[j] = parent->childNode[j - 1];
	}
	parent->keyNum++;
	parent->childNode[pos + 1] = rightNode;
	parent->keys[pos] = rightNode->keys[0];

	if (rightNode->childNode[0]) {
		rightNode->keyNum--;
		rightNode->keys.erase(rightNode->keys.begin());
		rightNode->childNode.erase(rightNode->childNode.begin());
	}
	rightNode->parentNode = parent;
	return;
}

template <class T>
void BPlusTree<T>::PrintTree(string filename)
{
	if (!Root)
	{
		cout << "EMPTY" << endl;
	}
	else
	{
		PrintNode(filename, Root);
	}
}

template <class T>
void BPlusTree<T>::PrintNode(string filename, Node<T>* node)
{
	ofstream file(filename.c_str(), ios::app); //If no such file exists, create a new file.
	if (!node)	return;
	for (int i = 0; i < node->keyNum; i++)
	{
		file << node->keys[i].value << ",";
	}
	if (node->parentNode)
	{
		file << "------parent: " << node->parentNode->keys[0].value << endl;
	}
	for (int i = 0; i < node->keyNum + 1; i++)
	{
		PrintNode(filename, node->childNode[i]);
	}
}

#endif

