#ifndef INDEXMANAGER_WT
#define INDEXMANAGER_WT

#include <string>
#include <vector>
#include "BPlusTree_wt.h"
using namespace std;

template <class T>
class IndexManager 
{
public:
	IndexManager(string tableName, string indexName, string attribute, int type);
	~IndexManager();
	int SearchIndex(T value);
	bool InsertElement(T value, int post);
	bool DeleteElement(T value1, T value2, int changePost);
	void find(struct Node<T> *node, int post1, int post2);
	void write();
	T v1;
	T v2;
	BPlusTree<T> index;
protected:
	void ChangePost(struct Node<T> *node, int post1, int post2, int change);
private:
	string TableName;
	string IndexName;
	string Attribute;
	int Type;
};

template <class T>
IndexManager<T>::IndexManager(string tableName, string indexName, string attribute, int type):index(type)
{
	TableName = tableName;
	IndexName = indexName;
	Attribute = attribute;
	Type = type;
}

template <class T>
IndexManager<T>::~IndexManager()
{
}

template <class T>
int IndexManager<T>::SearchIndex(T value)
{
	KEY<T> key;
	key.value = value;
	SearchNode<T> temp;
	temp = index.SearchKey(key);
	if (temp.ifFind == true)
	{
		return temp.ansNode->keys[temp.position].offset;
	}
	else
	{
		return -1;
	}
}

template <class T>
bool IndexManager<T>::InsertElement(T value, int post)
{
	KEY<T> key;
	key.value = value;
	key.offset = post;
	if (index.InsertKey(key))
	{
		return true;
	}
	else
	{
		return false;
	}
}

template <class T>
bool IndexManager<T>::DeleteElement(T value1, T value2, int changePost)
{
	KEY<T> key1, key2;
	key1.value = value1;
	key2.value = value2;
	SearchNode<T> temp1 = index.SearchKey(key1);
	SearchNode<T> temp2 = index.SearchKey(key2);
	if (temp1.ifFind && temp2.ifFind)
	{
		key1 = temp1.ansNode->keys[temp1.position];
		key2 = temp2.ansNode->keys[temp2.position];
		if (index.DeleteKey(key1))
		{
			ChangePost(index.GetRoot(), key1.offset, key2.offset, changePost);
			return true;
		}
		else return false;
	}
	else return false;
}

template <class T>
void IndexManager<T>::find(struct Node<T> *node, int post1, int post2)
{
	if (!node) return;
	for (int i = 0; i < node->keyNum; i++)
	{
		if (node->keys[i].offset == post1)	v1 = node->keys[i].value;
		if (node->keys[i].offset == post2)  v2 = node->keys[i].value;
	}
	for (int i = 0; i < node->keyNum + 1; i++)
	{
		find(node->childNode[i], post1, post2);
	}
}

template <class T>
void IndexManager<T>::write()
{
	string Name = "INDEX_FILE+" + IndexName;
	fstream fout(Name.c_str(), ios::out);
	fout.close();
	index.PrintTree(Name);
}

template <class T>
void IndexManager<T>::ChangePost(struct Node<T> *node, int post1, int post2, int change)
{
	if (!node) return;
	for (int i = 0; i < node->keyNum; i++)
	{
		if (node->keys[i].offset > post1 && node->keys[i].offset <= post2)
		{
			node->keys[i].offset -= change;
		}
	}
	for (int i = 0; i < node->keyNum + 1; i++)
	{
		ChangePost(node->childNode[i], post1, post2, change);
	}
}

#endif
