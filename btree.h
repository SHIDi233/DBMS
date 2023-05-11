/**
*  Name:B+Tree
*  Author: Shi haoyuan
*  Description:B+树数据结构
*  Date: 2023-5-11
*/

#ifndef BTREE_H
#define BTREE_H
#include <string>
using namespace std;

#define M 2//m*2=key数，m*2+1=值；//插入满格灌，删除一个节点不能少于(M+1)/2,若zhi为5，不能少于2
class Inter_Node;

class Node
{
public:
    Node();
    virtual ~Node();
    Node* GetBrother(int& flag);
    Inter_Node* Parent;
    int key[M * 2];//4
    int count;
    int isLeaf;
    void Print();
};
Node::~Node() {}

class Inter_Node : public Node
{
public:
    Inter_Node();
    virtual ~Inter_Node();//虚函数声明定义
    bool Insert(int value, Node* pNode);
    bool Delete(int value);
    int Split(Inter_Node* pNode, int key);//分裂
    bool Merge(Inter_Node* pNode);
    bool Slib(Inter_Node* pNode);
    Node* Child[M * 2 + 1];//5个查找路径
};
Inter_Node::~Inter_Node()
{
    for (int i = 0; i < M * 2 + 1; i++)
        Child[i] = nullptr;
}

//叶子结点
class Leaf_Node : public Node
{
public:
    Leaf_Node();
    virtual ~Leaf_Node();
    bool Insert(int value);
    bool Delete(int value);
    int Split(Leaf_Node* pNode);
    bool Merge(Leaf_Node* pNode);
    Leaf_Node* Pre_Node;
    Leaf_Node* Next_Node;
};
Leaf_Node::~Leaf_Node() {}

//B+树
class Bplus
{
public:
    Bplus();
    virtual ~Bplus();
    bool Search(int data, string& sPath);
    bool Insert(int data);
    bool Delete(int data);
    void Print();

protected:
    Leaf_Node* Find(int data);
    bool Add_Node(Inter_Node* pNode, int key, Node* New_Node);
    bool Remove_Node(Inter_Node* pNode, int key);
    Node* Root;
};
Bplus::~Bplus() {}

#endif // BTREE_H
