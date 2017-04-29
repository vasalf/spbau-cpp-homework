#include "huffman.h"

#include <iostream>
#include <fstream>
#include <limits>
#include <vector>
#include <queue>
#include <sstream>
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

TerminalTreeNode::TerminalTreeNode(uint8_t ends_here, TreeNode *par) : TreeNode(par) {
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
        joined_pairs.push_back(std::make_pair(p.second, q.second));
        pq.push(std::make_pair(p.first + q.first, p.second));
    }
    std::reverse(joined_pairs.begin(), joined_pairs.end());
    if (!joined_pairs.empty() && !pq.empty())
        answers[pq.top().second] = root_;
    else if (!pq.empty()) {
        root_->go(0) = new InnerTreeNode(root_);
        last_bit[pq.top().second] = 0;
        answers[pq.top().second] = root_->go(0);
    }
    for (std::pair<uint8_t, uint8_t> p : joined_pairs) {
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

void HuffmanTree::write_symbol_codes(TreeNode *node, std::bitset<256> path, std::size_t len) {
    if (node->is_terminal()) {
        symbol_codes_[node->end_of_char()] = path;
        symbol_length_[node->end_of_char()] = len;
    } else {
        if (node->go(0))
            write_symbol_codes(node->go(0), path << 1, len + 1);
        std::bitset<256> npath = path << 1;
        npath[0] = true;
        if (node->go(1))
            write_symbol_codes(node->go(1), npath, len + 1);
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
    write_symbol_codes(root_, std::bitset<256>(), 0);
}

TreeNode *HuffmanTree::add_path(TreeNode *node, uint8_t symbol, std::bitset<256> code, std::size_t bit, std::size_t len) {
    if (bit == len) {
        TreeNode *new_node = new TerminalTreeNode(symbol, node->parent());
        delete node;
        return new_node;
    } else {
        TreeNode*& next_node = node->go((code >> (len - bit - 1))[0]);
        if (next_node == nullptr)
            next_node = new InnerTreeNode(node);
        next_node = add_path(next_node, symbol, code, bit + 1, len);
        return node;
    }
}

HuffmanTree::HuffmanTree(std::ifstream& stream) {
    root_ = new InnerTreeNode(nullptr);
    for (std::size_t i = 0; i <= std::numeric_limits<uint8_t>::max(); i++) {
        uint8_t len; std::bitset<256> code;
        stream.read(reinterpret_cast<char *>(&len), 1);
        if (len != 0xff) {
            std::unique_ptr<uint8_t[]> to_read(new uint8_t[(len + 7) / 8]);
            stream.read(reinterpret_cast<char *>(to_read.get()), (len + 7) / 8);
            for (std::size_t j = 0; j < len; j++)
                code |= ((to_read.get()[j / 8] >> (j % 8)) << j);
            root_ = add_path(root_, i, code, 0, len);
        }
    }
    write_symbol_codes(root_, std::bitset<256>(), 0);
}

void HuffmanTree::write_to_file(std::ofstream& stream) {
    for (std::size_t i = 0; i <= std::numeric_limits<uint8_t>::max(); i++) {
        if (symbol_length_[i] > 0) {
            stream.write(reinterpret_cast<char *>(&symbol_length_[i]), 1);
            std::unique_ptr<uint8_t[]> to_write(new uint8_t[(symbol_length_[i] + 7) / 8]);
            for (std::size_t j = 0; j < (symbol_length_[i] + 7) / 8; j++) {
                to_write.get()[j] = 0;
                for (int k = 0; k < 8; k++)
                    to_write.get()[j] |= (symbol_codes_[i][j * 8 + k]) << k;
            }
            stream.write(reinterpret_cast<char *>(to_write.get()), (symbol_length_[i] + 7) / 8);
        } else {
            uint8_t tmp = 0xff;
            stream.write(reinterpret_cast<char *>(&tmp), 1);
        }
    }
}

std::bitset<256> HuffmanTree::get_symbol_code(uint8_t symbol) {
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

const std::size_t max_file_size = 10 * 1024 * 1024;

template<size_t bitset_sz>
uint8_t get_first_bits(const std::bitset<bitset_sz> &bs) {
    uint8_t ans = 0;
    for (int i = 0; i < 8; i++)
        ans |= bs[i] << i;
    return ans;
}

void HuffmanArchiveCreator::load_file(std::string filename) {
    std::ifstream file(filename);
    if (!file)
        throw InputOutputException(filename, false);
    std::vector<uint8_t> vec(max_file_size);
    std::size_t sz = file.read(reinterpret_cast<char *>(vec.data()), max_file_size).gcount();
    vec.resize(sz);
    std::cout << sz << std::endl;
    huffman_tree_.reset(new HuffmanTree(vec));
    std::size_t shift = 0;
    out_bytes.push_back(0);
    for (uint8_t byte : vec) {
        std::bitset<256> code = huffman_tree_->get_symbol_code(byte);
        std::size_t len = huffman_tree_->get_symbol_length(byte);
        while (len > 0) {
            if (shift + len < 8) {
                out_bytes.back() |= get_first_bits(code) << (8 - shift - len);
                shift += len;
                len = 0;
            } else if (shift > 0) {
                out_bytes.back() |= get_first_bits(code >> (len - (8 - shift)));
                out_bytes.push_back(0);
                len -= 8 - shift;
                shift = 0;
            } else {
                out_bytes.back() = get_first_bits(code >> (len - 8));
                out_bytes.push_back(0);
                len -= 8;
            }
        }
    }
    if (shift == 0) {
        out_bytes.pop_back();
        shift = 8;
    }
    last_shift = shift;
}

void HuffmanArchiveCreator::write_file(std::string filename) {
    std::ofstream file(filename);
    if (!file)
        throw InputOutputException(filename, true);
    huffman_tree_->write_to_file(file);
    uint8_t casted_shift = last_shift;
    file.write(reinterpret_cast<char *>(&casted_shift), 1);
    std::cout << out_bytes.size() << std::endl << file.tellp() << std::endl;
    file.write(reinterpret_cast<char *>(out_bytes.data()), out_bytes.size());
}

HuffmanArchiveExtractor::HuffmanArchiveExtractor() {}

void HuffmanArchiveExtractor::load_file(std::string filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file)
        throw InputOutputException(filename, false);
    huffman_tree_.reset(new HuffmanTree(file));
    uint8_t last_shift;
    file.read(reinterpret_cast<char *>(&last_shift), 1);
    std::size_t extra = file.tellg();
    std::vector<uint8_t> vec(max_file_size);
    std::size_t sz = file.read(reinterpret_cast<char *>(vec.data()), max_file_size).gcount();
    vec.resize(sz);
    std::cout << sz << std::endl;
    std::size_t i = 0, shift = 0;
    TreeNode *node = huffman_tree_->root();
    if (sz == 0 && last_shift == 8)
        last_shift = 0;
    while (i + 1 < vec.size() || shift < last_shift) {
        if (node->is_terminal()) {
            stored_bytes.push_back(node->end_of_char());
            node = huffman_tree_->root();
        }
        node = node->go((vec[i] >> (7 - shift)) & 1);
        shift++;
        if (shift == 8) {
            if (shift == last_shift && i + 1 == vec.size())
                break;
            i++;
            shift = 0;
        }
    }
    if (sz + last_shift > 0)
        stored_bytes.push_back(node->end_of_char());
    std::cout << stored_bytes.size() << std::endl;
    std::cout << extra << std::endl;
}

void HuffmanArchiveExtractor::write_file(std::string filename) {
    std::ofstream file(filename);
    if (!file)
        throw InputOutputException(filename, true);
    file.write(reinterpret_cast<char *>(stored_bytes.data()), stored_bytes.size());
}

InputOutputException::InputOutputException(std::string filename, bool writing) {
    filename_ = filename;
    writing_ = writing;
}

std::string InputOutputException::message() {
    std::stringstream ans;
    ans << "Couldn't open file " << filename_ << " for ";
    if (writing_)
        ans << "writing.";
    else
        ans << "reading.";
    return ans.str();
}
