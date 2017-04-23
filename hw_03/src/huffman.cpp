#include "huffman.h"

#include <iostream>
#include <fstream>
#include <limits>
#include <vector>
#include <queue>
#include <algorithm>

TreeNode::TreeNode(TreeNode *par) : par_(par) {}

TreeNode*& TreeNode::parent() {
    return par_;
}

TreeNode::~TreeNode() {}

InnerTreeNode::InnerTreeNode(TreeNode *par) : TreeNode(par) {
    next_[0] = next_[1] = nullptr;
}

bool InnerTreeNode::is_terminal() const {
    return false;
}

TreeNode*& InnerTreeNode::go(int bit) {
    return next_[bit];
}

TerminalTreeNode::TerminalTreeNode(char ends_here, TreeNode *par) : TreeNode(par) {
    ends_here_ = ends_here;
}

bool TerminalTreeNode::is_terminal() const {
    return true;
}

uint8_t TerminalTreeNode::end_of_char() const {
    return ends_here_;
}

template<class T>
using min_heap = std::priority_queue<T, std::vector<T>, std::greater<T> >;

void HuffmanTree::build_tree_from_frequency_array(int freq[]) {
    min_heap<std::pair<int, uint8_t> > pq;
    std::vector<std::pair<uint8_t, uint8_t> > joined_pairs;
    for (std::size_t i = 0; i <= std::numeric_limits<uint8_t>::max(); i++)
        if (freq[i] > 0)
            pq.push(std::make_pair(freq[i], (uint8_t)i));
    root_ = new InnerTreeNode(nullptr);
    TreeNode *answers[std::numeric_limits<uint8_t>::max() + 1];
    int last_bit[std::numeric_limits<uint8_t>::max()];
    while (pq.size() > 1) {
        auto p = pq.top();
        pq.pop();
        auto q = pq.top();
        pq.pop();
        //std::cout << "joined " << p.second << " and " << q.second << std::endl;
        joined_pairs.push_back(std::make_pair(p.second, q.second));
        pq.push(std::make_pair(p.first + q.first, p.second));
    }
    std::reverse(joined_pairs.begin(), joined_pairs.end());
    answers[pq.top().second] = root_;
    //std::cout << pq.top().second  << " is on the top" << std::endl;
    for (std::pair<uint8_t, uint8_t> p : joined_pairs) {
        //std::cout << "disjoin " << p.first << " and " << p.second << std::endl;
        TreeNode *new_left_node = new InnerTreeNode(answers[p.first]);
        TreeNode *new_right_node = new InnerTreeNode(answers[p.first]);
        answers[p.first]->go(0) = new_left_node;
        answers[p.first]->go(1) = new_right_node;
        answers[p.first] = new_left_node;
        last_bit[p.first] = 0;
        answers[p.second] = new_right_node;
        last_bit[p.second] = 1;
    }
    for (std::size_t i = 0; i <= std::numeric_limits<uint8_t>::max(); i++) {
        if (freq[i] > 0) {
            TreeNode *new_node = new TerminalTreeNode(i, answers[i]->parent());
            answers[i]->parent()->go(last_bit[i]) = new_node;
            delete answers[i];
            answers[i] = new_node;
        }
    }
}

void HuffmanTree::write_symbol_codes(TreeNode *node, uint8_t path, std::size_t len) {
    if (node->is_terminal()) {
        symbol_codes_[node->end_of_char()] = path;
        symbol_length_[node->end_of_char()] = len;
    } else {
        write_symbol_codes(node->go(0), path << 1, len + 1);
        write_symbol_codes(node->go(1), (path << 1) | 1, len + 1);
    }
}

void HuffmanTree::recursively_delete_nodes(TreeNode *node) {
    if (node == nullptr)
        return;
    if (!node->is_terminal()) {
        recursively_delete_nodes(node->go(0));
        recursively_delete_nodes(node->go(1));
    }
    delete node;
}

HuffmanTree::HuffmanTree(std::vector<uint8_t> bytes) {
    int freq[std::numeric_limits<uint8_t>::max() + 1];
    std::fill(std::begin(freq), std::end(freq), 0);
    for (uint8_t byte: bytes)
        freq[byte]++;
    build_tree_from_frequency_array(freq);
    write_symbol_codes(root_, 0, 0);
}

TreeNode *HuffmanTree::add_path(TreeNode *node, uint8_t symbol, uint8_t code, std::size_t bit, std::size_t len) {
    if (bit == len) {
        TreeNode *new_node = new TerminalTreeNode(symbol, node->parent());
        delete node;
        return new_node;
    } else {
        TreeNode*& next_node = node->go((code >> (len - bit - 1)) & 1);
        if (next_node == nullptr)
            next_node = new InnerTreeNode(node);
        next_node = add_path(next_node, symbol, code, bit + 1, len);
        return node;
    }
}

HuffmanTree::HuffmanTree(std::ifstream& stream) {
    root_ = new InnerTreeNode(nullptr);
    for (std::size_t i = 0; i <= std::numeric_limits<uint8_t>::max(); i++) {
        uint8_t len, code;
        stream.read(reinterpret_cast<char *>(&len), 1);
        stream.read(reinterpret_cast<char *>(&code), 1);
        if (len != 0xff)
            root_ = add_path(root_, i, code, 0, len);
    }
    write_symbol_codes(root_, 0, 0);
}

/* 2 bytes for each of 256 symbols: length and code */

void HuffmanTree::write_to_file(std::ofstream& stream) {
    for (std::size_t i = 0; i <= std::numeric_limits<uint8_t>::max(); i++) {
        if (symbol_length_[i] > 0)
            stream.write(reinterpret_cast<char *>(&symbol_length_[i]), 1);
        else {
            uint8_t tmp = 0xff;
            stream.write(reinterpret_cast<char *>(&tmp), 1);
        }
        stream.write(reinterpret_cast<char *>(&symbol_codes_[i]), 1);
    }
}

uint8_t HuffmanTree::get_symbol_code(uint8_t symbol) {
    return symbol_codes_[symbol];
}

std::size_t HuffmanTree::get_symbol_length(uint8_t symbol) {
    return symbol_length_[symbol];
}

TreeNode *HuffmanTree::root() const {
    return root_;
}

HuffmanTree::~HuffmanTree() {
    recursively_delete_nodes(root_);
}

HuffmanArchiveCreator::HuffmanArchiveCreator() {}

const std::size_t max_file_size = 5 * 1024 * 1024;

void HuffmanArchiveCreator::load_file(std::string filename) {
    std::ifstream file(filename);
    std::vector<uint8_t> vec(max_file_size);
    std::size_t sz = file.readsome(reinterpret_cast<char *>(vec.data()), max_file_size);
    vec.resize(sz);
    huffman_tree_.reset(new HuffmanTree(vec));
    std::size_t shift = 0;
    out_bytes.push_back(0);
    for (uint8_t byte : vec) {
        uint8_t code = huffman_tree_->get_symbol_code(byte);
        std::size_t len = huffman_tree_->get_symbol_length(byte);
        if (shift + len < 8) {
            out_bytes.back() |= (code << (8 - shift - len));
            shift += len;
        } else {
            out_bytes.back() = (out_bytes.back() | (code >> (8 - shift))) & 0xff;
            out_bytes.push_back((code << (16 - len - shift)) & 0xff);
            shift = (shift + len - 8);
        }
    }
    last_shift = shift;
}

void HuffmanArchiveCreator::write_file(std::string filename) {
    std::ofstream file(filename);
    huffman_tree_->write_to_file(file);
    uint8_t casted_shift = last_shift;
    file.write(reinterpret_cast<char *>(&casted_shift), 1);
    file.write(reinterpret_cast<char *>(out_bytes.data()), out_bytes.size());
}

HuffmanArchiveExtractor::HuffmanArchiveExtractor() {}

void HuffmanArchiveExtractor::load_file(std::string filename) {
    std::ifstream file(filename);
    huffman_tree_.reset(new HuffmanTree(file));
    uint8_t last_shift;
    file.read(reinterpret_cast<char *>(&last_shift), 1);
    std::vector<uint8_t> vec(513 + max_file_size);
    std::size_t sz = file.readsome(reinterpret_cast<char *>(vec.data()), 513 + max_file_size);
    vec.resize(sz);
    std::size_t i = 0, shift = 0;
    TreeNode *node = huffman_tree_->root();
    while (i + 1 < vec.size() || shift < last_shift) {
        if (node->is_terminal()) {
            stored_bytes.push_back(node->end_of_char());
            node = huffman_tree_->root();
        }
        node = node->go((vec[i] >> (7 - shift)) & 1);
        shift++;
        if (shift == 8) {
            i++;
            shift = 0;
        }
    }
    stored_bytes.push_back(node->end_of_char());
}

void HuffmanArchiveExtractor::write_file(std::string filename) {
    std::ofstream file(filename);
    file.write(reinterpret_cast<char *>(stored_bytes.data()), stored_bytes.size());
}
