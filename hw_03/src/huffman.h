#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <map>
#include <memory>
#include <fstream>

class TreeNode {
    TreeNode *par_;
public:
    TreeNode(TreeNode *par);
    virtual bool is_terminal() const = 0;
    virtual TreeNode*& go(int bit) = 0;
    virtual uint8_t end_of_char() const = 0;
    TreeNode*& parent();
    virtual ~TreeNode();
};

class InnerTreeNode : public TreeNode {
    TreeNode *next_[2];
public:
    InnerTreeNode(TreeNode *par);
    virtual bool is_terminal() const;
    virtual TreeNode*& go(int bit);
    virtual uint8_t end_of_char() const { return 0; };
};

class TerminalTreeNode : public TreeNode {
    uint8_t ends_here_;
public:
    TerminalTreeNode(char ends_here, TreeNode *par);
    virtual bool is_terminal() const;
    virtual uint8_t end_of_char() const;
    virtual TreeNode*& go([[gnu::unused]] int bit) { return parent(); }
};

class HuffmanTree {
    TreeNode *root_;
    std::map<uint8_t, uint8_t> symbol_codes_;
    std::map<uint8_t, std::size_t> symbol_length_;
    void build_tree_from_frequency_array(int freq[]);
    void recursively_delete_nodes(TreeNode *node);
    void write_symbol_codes(TreeNode *node, uint8_t path, std::size_t len);
    TreeNode *add_path(TreeNode *node, uint8_t symbol, uint8_t code, std::size_t bit, std::size_t len);
public:
    HuffmanTree(const HuffmanTree& other) = delete;
    HuffmanTree& operator=(HuffmanTree other) = delete;
    HuffmanTree(std::vector<uint8_t> bytes);
    // Reads a header from file
    HuffmanTree(std::ifstream& stream);
    void write_to_file(std::ofstream& stream);
    uint8_t get_symbol_code(uint8_t symbol);
    std::size_t get_symbol_length(uint8_t symbol);
    TreeNode *root() const;
    ~HuffmanTree();
};

class HuffmanArchiveCreator {
    std::unique_ptr<HuffmanTree> huffman_tree_;
    std::vector<uint8_t> out_bytes;
    std::size_t last_shift;
public:
    HuffmanArchiveCreator();
    void load_file(std::string filename);
    void write_file(std::string filename);
};

class HuffmanArchiveExtractor {
    std::unique_ptr<HuffmanTree> huffman_tree_;
    std::vector<uint8_t> stored_bytes;
public:
    HuffmanArchiveExtractor();
    void load_file(std::string filename);
    void write_file(std::string filename);
};
