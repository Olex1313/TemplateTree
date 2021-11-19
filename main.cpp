#include <iostream>
#include <string>
template<class K, class V> struct Node {
    K key;
    V value;
    Node<K, V> *left;
    Node<K, V> *right;
};

template <class K, class V>
class TreeMap {
public:
    TreeMap();
    TreeMap(const TreeMap<K, V> &parent);
    ~TreeMap();
    void clear_tree();
    int size();
    void load(const std::string &filename);
    void dump(const std::string &filename);
    void add(const K &key, const V &value);
    void remove(const K &key);
    void print_in_order();
    bool contains(K key);
    V get(K key);
private:
    Node<K, V>* root;
    int tree_size;
    void add(Node<K, V> **new_node, const K &key, const V &value);
    void print_in_order(Node<K, V> *node);
    bool contains(Node<K, V> *node, K key);
    void clear_tree(Node<K, V> *node);
    V search_node(Node<K, V> *node, K key);
};

template<class K, class V>
TreeMap<K, V>::TreeMap() {
    this->root = nullptr;
    this->tree_size = 0;
}

template<class K, class V>
TreeMap<K, V>::~TreeMap() {
    clear_tree(this->root);
}

template<class K, class V>
int TreeMap<K, V>::size() {
    return this->tree_size;
}

template<class K, class V>
void TreeMap<K, V>::add(Node<K, V> **node, const K &key, const V &value) {
    if (*node == nullptr) {
        Node<K, V> *temporary = new Node<K, V>;
        temporary->key = key;
        temporary->value = value;
        temporary->left = nullptr;
        temporary->right = nullptr;
        *node = temporary;
        this->tree_size++;
    } else {
        if (key > (*node)->key) {
            add(&(*node)->right, key, value);
        } else {
            add(&(*node)->left, key, value);
        }
    }
}

template<class K, class V>
void TreeMap<K, V>::add(const K &key, const V &value) {
    add(&(this->root), key, value);
}

template<class K, class V>
void TreeMap<K, V>::print_in_order() {
    print_in_order(this->root);
}

template<class K, class V>
void TreeMap<K, V>::print_in_order(Node<K, V> *node) {
    if (node != nullptr) {
        print_in_order(node->left);
        std::cout << "Key: " << node->key << " " << "Value: " << node->value << std::endl;
        print_in_order(node->right);
    }
}

template<class K, class V>
void TreeMap<K, V>::clear_tree(Node<K, V> *node) {
    if (node != nullptr) {
        clear_tree(node->left);
        clear_tree(node->right);
        delete node;
    }
}

template<class K, class V>
bool TreeMap<K, V>::contains(Node<K, V> *node, K value) {
    if (node == nullptr) {
        return false;
    }
    if (value == node->value) {
        return true;
    }
    if (value > node->value) {
        return contains(node->right, value);
    } 
    return contains(node->left, value);
}

template<class K, class V>
bool TreeMap<K, V>::contains(K key) {
    return contains(this->root, key);
}

template<class K, class V>
V TreeMap<K, V>::search_node(Node<K, V> *node, K key) {
    if (node == nullptr) {
        throw std::exception();
    }
    if (key == node->key) {
        return node->value;
    }
    if (key > node->key) {
        return search_node(node->right, key);
    }
    return search_node(node->left, key);
}

template<class K, class V>
V TreeMap<K, V>::get(K key) {
    if (this->contains(key)) {
        return search_node(this->root, key);
    }
    throw "No such key in tree";
}

int main() {
    TreeMap<int, int> map;
    map.add(1, 1);
    map.add(2, 2);
    map.print_in_order();
    std::cout << map.get(1) << std::endl;
    std::cout << "Done" << std::endl;
    return 0;
}
