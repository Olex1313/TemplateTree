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
    V& get(K key);
    V& operator[](K key);
private:
    Node<K, V>* root;
    int tree_size;
    void add(Node<K, V> **new_node, const K &key, const V &value);
    Node<K, V>* remove(Node<K, V> *root, const K &key);
    void print_in_order(Node<K, V> *node);
    bool contains(Node<K, V> *node, K key);
    void clear_tree(Node<K, V> *node);
    V& search_node(Node<K, V> *node, K key);
    Node<K, V>* find_min(Node<K, V> *node);
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
Node<K, V>* TreeMap<K, V>::find_min(Node<K, V> *node) {
    while(node->left != nullptr){
        node = node->left;
    }
    return node;
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
Node<K, V>* TreeMap<K, V>::remove(Node<K, V> *root, const K &key) {
    if (root == nullptr) {
        return root;
    }
    else if (key < root->key) {
        root->left = remove(root->left, key);
    }
    else if (key > root->key) {
        root->right = remove(root->right, key);
    }
    else {
        // No child
        if (root->left == nullptr && root->right == nullptr) {
            delete root;
            root = nullptr;
            this->tree_size--;
        }
        // One child
        else if (root->left == nullptr) {
            Node<K, V> *temporary = root;
            root = root->right;
            delete temporary;
            this->tree_size--;
        }
        else if (root->right == nullptr) {
            Node<K, V> *temporary = root;
            root = root->left;
            delete temporary;
            this->tree_size--;
        }
        // Two children
        else {
            Node<K, V> *temporary = find_min(root->right);
            root->key = temporary->key;
            root->value = temporary->value;
            root->right = remove(root->right, key);
        }
    }
    return root;
}

template<class K, class V>
void TreeMap<K, V>::remove(const K &key) {
    if (contains(key)) {
        this->root = remove(this->root, key);
    } else {
        return;
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
bool TreeMap<K, V>::contains(Node<K, V> *node, K key) {
    if (node == nullptr) {
        return false;
    }
    if (key == node->key) {
        return true;
    }
    if (key > node->key) {
        return contains(node->right, key);
    } 
    return contains(node->left, key);
}

template<class K, class V>
bool TreeMap<K, V>::contains(K key) {
    return contains(this->root, key);
}

template<class K, class V>
V &TreeMap<K, V>::search_node(Node<K, V> *node, K key) {
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
V& TreeMap<K, V>::get(K key) {
    if (this->contains(key)) {
        return search_node(this->root, key);
    }
    throw "No such key in tree";
}

template<class K, class V>
V& TreeMap<K, V>::operator[](K key) {
    return this->get(key);
}

int main() {
    TreeMap<int, int> map;
    map.add(1, 1);
    map.add(2, 4);
    map.add(5, 25);
    map.add(7, 49);
    map.add(3, 9);
    map.print_in_order();
    std::cout << map.get(1) << std::endl;
    std::cout << map.get(7) << std::endl;
    map.remove(7);
    map[3] = 81;
    map.print_in_order();
    std::cout << "Done" << std::endl;
    return 0;
}

