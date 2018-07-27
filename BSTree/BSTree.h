/*
二叉查找树
注意:C++不支持对模板的分离编译
*/
#ifndef BSTREE_H
#define BSTREE_H

#include <vector>
#include <iostream>
namespace wewe_ds{

template <typename T>
struct BSNode
{
    // 二叉查找树节点定义
    T value;
    BSNode<T> *lchild;
    BSNode<T> *rchild;
    BSNode<T> *parent; // 指向父节点
    BSNode(T val):value(val), lchild(nullptr), rchild(nullptr), parent(nullptr){}
};

template <typename T>
class BSTree
{
    public:
        BSTree();
        ~BSTree();

        size_t size();
        const BSNode<T>* getRoot(); // 获取根节点
        void insert(T key); // 插入指定元素
        void destory();
        void inOrder(std::vector<T> &vec); // 中序遍历
        void postOrder(std::vector<T> &vec); // 后序遍历
        BSNode<T>* search_recursion(const T key); // 查找指定元素的节点(递归)
        BSNode<T>* search_iterator(const T key); // 查找指定元素的节点(非递归)
        void remove(const T key); // 删除key节点
        BSNode<T>* maxNode();
        BSNode<T>* minNode();
        T predecessor(const T key);
        T successor(const T key);

    protected:

    private:
        BSNode<T>* root;
        size_t m_size;
        void destory(BSNode<T>* &p);
        void inOrder(std::vector<T> &vec, const BSNode<T>* const p);
        void postOrder(std::vector<T> &vec, const BSNode<T>* const p);
        BSNode<T>* search(BSNode<T>* const p, const T key);
        void remove(BSNode<T>* &p); // 删除节点
        BSNode<T>* maxNode(BSNode<T>* p); // 以p节点为根节点的子树最大值
        BSNode<T>* minNode(BSNode<T>* p); // 以p节点为根节点的子树最小值
        BSNode<T>* predecessor(const BSNode<T>* p); // 节点的前驱(节点的左子树中最大节点)
        BSNode<T>* successor(const BSNode<T>* p); // 节点的后继(节点的右子树中最小节点)
};

/* 构造函数 */
template <typename T>
BSTree<T>::BSTree():root(nullptr),m_size(0){}

/* 析构函数 */
template <typename T>
BSTree<T>::~BSTree()
{
    destory();
}

/* 树的节点数量 */
template <typename T>
size_t BSTree<T>::size()
{
    return m_size;
}

/* 获取根节点 */
template <typename T>
const BSNode<T>* BSTree<T>::getRoot()
{
    return root;
}

/* 插入指定元素的节点 */
template <typename T>
void BSTree<T>::insert(T key)
{
    BSNode<T> *new_node = new BSNode<T>(key);

    if(root == nullptr)
    {
        root = new_node;
    }
    else
    {
        BSNode<T> *preNode = nullptr;
        BSNode<T> *node = root;
        while(node != nullptr)
        {
            // 找到叶节点时,preNode保存新插入节点的父节点
            preNode = node;
            if(node->value > key){
                node = node->lchild;
            }
            else if(node->value < key){
                node = node->rchild;
            }
            else
                break;
        }

        new_node->parent = preNode;
        if(preNode->value > key){
            preNode->lchild = new_node;
        }
        else if(preNode->value < key){
            preNode->rchild = new_node;
        }
    }

    m_size += 1;
}

/* 中序遍历树(通过vector传出数据) */
template <typename T>
void BSTree<T>::inOrder(std::vector<T> &vec)
{
    inOrder(vec, root);
}
template <typename T>
void BSTree<T>::inOrder(std::vector<T> &vec, const BSNode<T>* const p)
{
    if(p == nullptr)
        return;

    if(p->lchild != nullptr)
        inOrder(vec, p->lchild);
    vec.push_back(p->value);
    if(p->rchild != nullptr)
        inOrder(vec, p->rchild);
}

/* 后序遍历树(通过vector传出数据) */
template <typename T>
void BSTree<T>::postOrder(std::vector<T> &vec)
{
    postOrder(vec, root);
}
template <typename T>
void BSTree<T>::postOrder(std::vector<T> &vec, const BSNode<T>* const p)
{
    if(p == nullptr)
        return;

    if(p->lchild != nullptr)
        postOrder(vec, p->lchild);
    if(p->rchild != nullptr)
        postOrder(vec, p->rchild);

    vec.push_back(p->value);
}

/* 查找指定元素的节点 */
// 1. 非递归
template <typename T>
BSNode<T>* BSTree<T>::search_iterator(const T key)
{
    if(root == nullptr)
        return nullptr;

    BSNode<T>* node = root;
    while(node != nullptr)
    {
        if(node->value > key)
            node = node->lchild;
        else if(node->value < key)
            node = node->rchild;
        else if(node->value == key) // 找到key节点
            return node;
    }

    return node;
}
// 2. 递归
template <typename T>
BSNode<T>* BSTree<T>::search_recursion(const T key)
{
    return search(root, key);
}
template <typename T>
BSNode<T>* BSTree<T>::search(BSNode<T>* const p, const T key)
{
    if(p == nullptr)
        return nullptr;

    // std::cout << p->value << " ";
    if(key > p->value)
        return search(p->rchild, key);
    if(key < p->value)
        return search(p->lchild, key);
    if(p->value == key)
        return p;
}

/* 节点的前驱节点 */
template <typename T>
T BSTree<T>::predecessor(const T key)
{
    BSNode<T>* node = search_iterator(key);
    if(node != nullptr)
    {
        BSNode<T>* pre = predecessor(node);
        if(pre != nullptr)
            return pre->value;
    }
}
template <typename T>
BSNode<T>* BSTree<T>::predecessor(const BSNode<T>* p)
{
    // 节点p有左孩子,左孩子中的最大值
    BSNode<T>* tmp;
    tmp = const_cast<BSNode<T>*>(p);
    if(tmp->lchild != nullptr)
    {
        return maxNode(tmp->lchild);
    }
    BSNode<T>* node = p->parent;
    if(node != nullptr && node->rchild == p)
    {
        // p是右孩子
        return node;
    }
    if(node != nullptr && node->lchild == p)
    {
        // p是左孩子
        if(node->parent != nullptr && node->parent->rchild == node)
            return node->parent;
    }

    return nullptr;
}

/* 节点的后继节点 */
template <typename T>
T BSTree<T>::successor(const T key)
{
    BSNode<T>* node = search_iterator(key);
    if(node != nullptr)
    {
        BSNode<T>* succ = successor(node);
        if(succ != nullptr)
            return succ->value;
    }
}
template <typename T>
BSNode<T>* BSTree<T>::successor(const BSNode<T>* p)
{
    // 节点p有右孩子
    if(p->rchild != nullptr)
        return minNode(p->rchild);

    BSNode<T>* pre = p->parent;
    if(pre != nullptr && pre->lchild == p)
        return pre;
    if(pre != nullptr && pre->rchild == p)
    {
        if(pre->parent != nullptr && pre->parent->lchild == pre)
            return pre->parent;
    }

    return nullptr;
}

/* 删除节点 */
/* 删除节点 */
template <typename T>
void BSTree<T>::remove(const T key)
{
    BSNode<T> *node = search_iterator(key);
    if(node != nullptr)
    {
        remove(node);
        m_size -= 1;
    }
}
template <typename T>
void BSTree<T>::remove(BSNode<T>* &p)
{
    if(p != nullptr){
        BSNode<T>* pparent = p->parent;
        BSNode<T>* left = p->lchild;
        BSNode<T>* right = p->rchild;
        // 重构子树
        BSNode<T>* child_root;
        if(left != nullptr && right != nullptr)
        {
            // 删除节点左右子树都存在
            // 删节点右子树中最左端节点的值(最小值)替代删除节点的值
            // 删除该最小值节点
            BSNode<T>* tmp = right;
            while(tmp->lchild != nullptr)
            {
                tmp = tmp->lchild;
            }
            if(tmp == right){
                // 最小值节点是删除节点的右节点
                p->value = tmp->value;
                p->rchild = tmp->rchild;
                tmp->rchild->parent = p;
            }
            else{
                p->value = tmp->value;
                tmp->parent->lchild = tmp->rchild;
            }
            delete tmp;
            return;
        }
        else if(left != nullptr)
        {
            // 仅左子树不为空
            // 左子树根节点代替被删除节点
            child_root = left;
        }
        else if(right != nullptr)
        {
            child_root = right;
        }
        else
        {
            child_root = nullptr;
        }

        if(pparent != nullptr){
            // 重构的子树成为被删除节点的父节点的子树
            if(p->value < pparent->value)
                pparent->lchild = child_root;
            else
                pparent->rchild = child_root;

            if(child_root != nullptr)
                child_root->parent = pparent;
        }
        else{
            // 删除的是根节点
            root = child_root;
        }
        delete p;
    }
}

/* 销毁树(公共接口) */
template <typename T>
void BSTree<T>::destory()
{
    destory(root);
}
/* 递归销毁树(私有接口) */
template <typename T>
void BSTree<T>::destory(BSNode<T>* &p)
{
    // 私有的
    // 指针 + 引用参数
    // 不仅可以访问p指向的区域,还可以改变p的值(即指针本身的值可以改变)
    // 如果仅仅使用指针做参数,则指针本身的值不能被改变
    if(p != nullptr)
    {
        if(p->lchild != nullptr)
            destory(p->lchild);
        if(p->rchild != nullptr)
            destory(p->rchild);
        delete p;
        p = nullptr;
    }
}

/* 查找最大值节点 */
template <typename T>
BSNode<T>* BSTree<T>::maxNode()
{
    return maxNode(root);
}
template <typename T>
BSNode<T>* BSTree<T>::maxNode(BSNode<T>* p)
{
    BSNode<T>* node = p;
    if(node != nullptr)
    {
        if(node->rchild == nullptr)
            return node;
        else{
            while(node->rchild != nullptr)
                node = node->rchild;
        }
    }
    return node;
}

/* 查找最小值节点 */
template <typename T>
BSNode<T>* BSTree<T>::minNode()
{
    return minNode(root);
}
template <typename T>
BSNode<T>* BSTree<T>::minNode(BSNode<T>* p)
{
    BSNode<T>* node = p;
    if(node != nullptr)
    {
        if(node->lchild == nullptr)
            return node;
        else{
            while(node->lchild != nullptr)
                node = node->lchild;
        }
    }
    return node;
}

}
#endif // BSTREE_H

/*********************************************************************
void testBSTree()
{
   cout << "\n\n--------  测试BSTree类  START  --------\n\n";
    int nums[10] = {3, 5, 4, 7, 6, 1, 8, 10, 2, 11};
    BSTree<int> tree;
    for(int i = 0; i < 10; i++)
        tree.insert(nums[i]);
    vector<int> vec;
    tree.inOrder(vec);
    for(int i:vec)
        cout << i << endl;

    BSNode<int>* minnode = tree.minNode();
    cout << "MIN NODE: " << minnode->value << endl;
    BSNode<int>* maxnode = tree.maxNode();
    cout << "MAX NODE: " << maxnode->value << endl;

    for(int key:vec){
        if(key != tree.minNode()->value)
            cout << key << "的前驱节点: " << tree.predecessor(key) << endl;
        if(key != tree.maxNode()->value)
            cout << key << "的后继节点: " << tree.successor(key) << endl;
    }

    bool rmagain = true;
    cout << "执行删除操作?(YES:1 NO:0) " << endl;
    cin >> rmagain;
    while(1 && rmagain){
        cout << "remove x: ";
        int x;
        cin >> x;
        vec.clear();
        tree.remove(x);
        tree.inOrder(vec);
        for(int i:vec)
            cout << i << " ";
        cout << endl;
        cout << "继续?(YES:1 NO: 0)" << endl;
        cin >> rmagain;
    }
    BSNode<int> *p = nullptr;
    p = tree.search_iterator(5);
    if( p != nullptr)
        cout << "p: " << p->value << endl;

    cout << "tree size:" << tree.size() << endl;

    cout << "\n\n--------  测试BSTree类 END      --------\n\n";
}

*********************************************************************/
