#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <assert.h>

template<class K, class V> struct Node {
    K key;
    V value;
    Node<K, V> *left;
    Node<K, V> *right;
};

template<class K, class V>
std::ostream& operator<<(std::ostream& os, const Node<K, V> &node) {
    os << node.key << " : " << node.value;
    return os;
}

template <class K, class V>
class TreeMap {
public:
    TreeMap();
    TreeMap(const TreeMap<K, V> &parent);
    ~TreeMap();
    void clear_tree();
    int size() const;
    void load(const std::string &filename);
    void dump(const std::string &filename) const;
    void add(const K &key, const V &value);
    void remove(const K &key);
    void print_in_order() const;
    bool contains(const K &key) const;
    V& get(const K &key) const;
    V& operator[](const K &key) const;
    std::vector<V> get_values() const;
    std::vector<K> get_keys() const;
private:
    Node<K, V>* root;
    int tree_size;
    void add(Node<K, V> **new_node, const K &key, const V &value);
    Node<K, V>* remove(Node<K, V> *root, const K &key);
    void print_in_order(Node<K, V> *node) const;
    bool contains(Node<K, V> *node, const K &key) const;
    void clear_tree(Node<K, V> *node);
    V& search_node(Node<K, V> *node, const K &key) const;
    Node<K, V>* find_min(Node<K, V> *node);
    void get_values(std::vector<V> &values, Node<K, V> *node) const;
    void get_keys(std::vector<K> &keys, Node<K, V> *node) const;
};

template<class K, class V>
TreeMap<K, V>::TreeMap() {
    this->root = nullptr;
    this->tree_size = 0;
}

template<class K, class V>
TreeMap<K, V>::TreeMap(const TreeMap<K, V> &parent) {
    std::vector<K> keys = parent.get_keys();
    this->root = nullptr;
    this->tree_size = 0;
    for (auto key : keys) {
        this->add(key, parent[key]);
    }
}

template<class K, class V>
void TreeMap<K, V>::load(const std::string &filename) {
    clear_tree();
    std::ifstream f(filename);
    if (!f.is_open()) {
        throw std::runtime_error("Couldn't open file");
    }
    for (std::string line; std::getline(f, line); ) {
        K key;
        V value;
        std::string token;
        std::stringstream ss (line);
        std::getline(ss, token, ':');
        std::stringstream converter (token);
        converter >> key;
        converter.clear();
        std::getline(ss, token);
        converter.str(token);
        converter >> value;
        add(key, value);
    }
}

template<class K, class V>
void TreeMap<K, V>::dump(const std::string &filename) const {
    std::vector<K> keys = get_keys();
    std::vector<V> values = get_values();
    std::ofstream f(filename);
    if (!f) {
        throw std::runtime_error("Couldn't open file");
    }
    for (int i = 0; i < size(); ++i) {
        f << keys[i] << ":" << values[i] << std::endl;
    }
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
int TreeMap<K, V>::size() const {
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
void TreeMap<K, V>::print_in_order() const {
    print_in_order(this->root);
}

template<class K, class V>
void TreeMap<K, V>::print_in_order(Node<K, V> *node) const {
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
void TreeMap<K, V>::clear_tree() {
    clear_tree(this->root);
    this->root = nullptr;
    this->tree_size = 0;
}

template<class K, class V>
bool TreeMap<K, V>::contains(Node<K, V> *node, const K &key) const {
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
bool TreeMap<K, V>::contains(const K &key) const {
    return contains(this->root, key);
}

template<class K, class V>
V &TreeMap<K, V>::search_node(Node<K, V> *node, const K &key) const {
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
V& TreeMap<K, V>::get(const K &key) const {
    if (this->contains(key)) {
        return search_node(this->root, key);
    }
    throw std::runtime_error("No such key in map");
}

template<class K, class V>
V& TreeMap<K, V>::operator[](const K &key) const {
    return this->get(key);
}

template<class K, class V>
void TreeMap<K, V>::get_values(std::vector<V> &values, Node<K, V> *node) const {
    if (node != nullptr) {
        get_values(values, node->left);
        values.push_back(node->value);
        get_values(values, node->right);
    }
}

template<class K, class V>
std::vector<V> TreeMap<K, V>::get_values() const {
    std::vector<V> values;
    get_values(values, this->root);
    return values;
}

template<class K, class V>
void TreeMap<K, V>::get_keys(std::vector<K> &keys, Node<K, V> *node) const {
    if (node != nullptr) {
        get_keys(keys, node->left);
        keys.push_back(node->key);
        get_keys(keys, node->right);
    }
}

template<class K, class V>
std::vector<K> TreeMap<K, V>::get_keys() const {
    std::vector<K> keys;
    get_keys(keys, this->root);
    return keys;
}

template<class K, class V>
bool operator==(const TreeMap<K, V> &lhs, const TreeMap<K, V> &rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }
    if (lhs.size() == 0 && rhs.size() == 0) {
        return true;
    }
    std::vector<K> left_keys = lhs.get_keys();
    for (auto key : left_keys) {
        if (lhs[key] != rhs[key]) {
            return false;
        }
    }
    return true;
}

int main() {
    TreeMap<int, int> map;
   
    // TEST 1, default constructor
    assert(map.size() == 0);
    map.add(1, 11);
    std::cerr << "PASS 1 TEST" << std::endl;

    // TEST 2, add method
    assert(map.size() == 1);
    std::cerr << "PASS 2 TEST" << std::endl;

    // TEST 3, contans method
    assert(map.contains(1));
    std::cerr << "PASS 3 TEST" << std::endl;

    // TEST 4, operator [] overload
    assert(map[1] == 11);
    std::cerr << "PASS 4 TEST" << std::endl;

    // TEST 5, change value of specific key
    map.add(2, 4);
    int previous_size = map.size();
    map.remove(5);
    assert(map.size() == previous_size);
    map.remove(2);
    assert(map.size() == previous_size - 1);
    std::cerr << "PASS 5 TEST" << std::endl;

    // TEST 6, remove method
    assert(!map.contains(2));
    std::cerr << "PASS 6 TEST" << std::endl;
 
    // TEST 7, cleared == default
    map.clear_tree();
    TreeMap<int, int> default_tree;
    assert(map == default_tree);
    std::cerr << "PASS 7 TEST" << std::endl;
   
    // TEST 8, copy == parent
    map.add(5, 25);
    map.add(7, 49);
    map.add(3, 9);
    TreeMap<int, int> copy = TreeMap(map);
    assert(copy == map);
    std::cerr << "PASS 8 TEST" << std::endl;

    // TEST 9, file dumping
    std::string test_file = "test.txt";
    map.dump(test_file);
    TreeMap<int, int> loaded_map;
    loaded_map.load(test_file);
    assert(loaded_map == map);
    std::cerr << "PASS 9 TEST" << std::endl;

    std::cout << "Passed all the tests" << std::endl;
    return 0;
}

