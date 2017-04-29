#include <gtest/gtest.h>
#include <huffman.h>
#include <memory>
#include <limits>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <bitset>
#include <random>

using namespace std;

TEST(TreeNode, is_terminal) {
    unique_ptr<TreeNode> inner(new InnerTreeNode(nullptr));
    unique_ptr<TreeNode> terminal(new TerminalTreeNode('a', nullptr));
    EXPECT_FALSE(inner->is_terminal());
    EXPECT_TRUE(terminal->is_terminal());
}

TEST(TreeNode, go) {
    unique_ptr<TreeNode> par(new InnerTreeNode(nullptr));
    unique_ptr<TreeNode> a(new TerminalTreeNode('a', par.get()));
    unique_ptr<TreeNode> b(new TerminalTreeNode('b', par.get()));
    EXPECT_EQ(par->go(0), nullptr);
    EXPECT_EQ(par->go(1), nullptr);
    par->go(0) = a.get();
    par->go(1) = b.get();
    EXPECT_EQ(par->go(0), a.get());
    EXPECT_EQ(par->go(1), b.get());
}

TEST(TreeNode, end_of_char) {
    for (size_t i = 0; i <= numeric_limits<uint8_t>::max(); i++) {
        unique_ptr<TreeNode> node(new TerminalTreeNode(i, nullptr));
        EXPECT_EQ(node->end_of_char(), i);
    }
}

template<class NodeType>
NodeType *construct_tree_node();

template<>
InnerTreeNode *construct_tree_node<InnerTreeNode>() {
    return new InnerTreeNode(nullptr);
}

template<>
TerminalTreeNode *construct_tree_node<TerminalTreeNode>() {
    return new TerminalTreeNode(0, nullptr);
}

template<class NodeType>
class TreeNodeParentTest : public testing::Test {
protected:
    unique_ptr<NodeType> parent_, node_;
    
    TreeNodeParentTest(): parent_(construct_tree_node<NodeType>()),
                          node_(construct_tree_node<NodeType>()) {}

    virtual ~TreeNodeParentTest() {}
};

typedef testing::Types<InnerTreeNode, TerminalTreeNode> tree_node_types;

TYPED_TEST_CASE(TreeNodeParentTest, tree_node_types);

TYPED_TEST(TreeNodeParentTest, parent) {
    EXPECT_EQ(this->node_->parent(), nullptr);
    this->node_->parent() = this->parent_.get();
    EXPECT_EQ(this->node_->parent(), this->parent_.get());
}

// is `a` a prefix of `b`
bool is_prefix(const bitset<256>& a, size_t len_a, const bitset<256>& b, size_t len_b) {
    return (a >> (len_a - len_b)) == b;
}

struct custom_compare {
    bool operator()(const pair<bitset<256>, size_t>& a, const pair<bitset<256>, size_t>& b) const {
        for (size_t i = 0; i != 256; i++)
            if (a.first[i] != b.first[i])
                return a.first[i] < b.first[i];
        return a.second < b.second;
    }
};

bool is_correct_tree(HuffmanTree& tree, vector<uint8_t> data) {
    size_t freq[numeric_limits<uint8_t>::max() + 1];
    fill(freq, freq + numeric_limits<uint8_t>::max() + 1, 0);
    sort(data.begin(), data.end());
    data.resize(distance(data.begin(), unique(data.begin(), data.end())));
    map<pair<bitset<256>, size_t>, uint8_t, custom_compare> code_of;
    for (uint8_t x : data) {
        bitset<256> code = tree.get_symbol_code(x);
        size_t length = tree.get_symbol_length(x);
        if (code_of.count(make_pair(code, length))) {
            // codes of some two symbols are equal
            return false;
        }
        code_of[make_pair(code, length)] = x;
    }
    for (uint8_t x : data) {
        for (uint8_t y : data) {
            if (x == y || tree.get_symbol_length(x) < tree.get_symbol_length(y))
                continue;
            if (is_prefix(tree.get_symbol_code(x), tree.get_symbol_length(x),
                          tree.get_symbol_code(y), tree.get_symbol_length(y))) {
                // code of `x` is a prefix of code of `y`
                return false;
            }
        }
    }
    for (uint8_t x : data) {
        for (uint8_t y : data) {
            if (freq[x] < freq[y] && tree.get_symbol_length(x) > tree.get_symbol_length(y)) {
                // code lengths are not sorted by frequency
                return false;
            }
        }
    }
    return true;
}

TEST(HuffmanTree, build_manual_test_1) {
    string s = "asdfghijkl";
    vector<uint8_t> vec;
    copy(s.begin(), s.end(), back_inserter(vec));

    HuffmanTree tree(vec);
    EXPECT_TRUE(is_correct_tree(tree, vec));
}

TEST(HuffmanTree, build_manual_test_2) {
    string s = "aabbccddeeffgghhiijjkkll";
    vector<uint8_t> vec;
    copy(s.begin(), s.end(), back_inserter(vec));

    HuffmanTree tree(vec);
    EXPECT_TRUE(is_correct_tree(tree, vec));
}

TEST(HuffmanTree, build_manual_test_3) {
    string s = "aaaaaaaaaaaaaaaaaaaaaaaaaab";
    vector<uint8_t> vec;
    copy(s.begin(), s.end(), back_inserter(vec));

    HuffmanTree tree(vec);
    EXPECT_TRUE(is_correct_tree(tree, vec));
}

TEST(HuffmanTree, single_symbol) {
    string s = "a";
    vector<uint8_t> vec;
    copy(s.begin(), s.end(), back_inserter(vec));

    HuffmanTree tree(vec);
    EXPECT_TRUE(is_correct_tree(tree, vec));
}

TEST(HuffmanTree, repeated_single_symbol) {
    string s = "aaaaaaaa";
    vector<uint8_t> vec;
    copy(s.begin(), s.end(), back_inserter(vec));

    HuffmanTree tree(vec);
    EXPECT_TRUE(is_correct_tree(tree, vec));
}

TEST(HuffmanTree, empty_data) {
    vector<uint8_t> vec;

    HuffmanTree tree(vec);
    EXPECT_TRUE(is_correct_tree(tree, vec));
}

TEST(HuffmanTree, long_vector_of_zeros) {
    vector<uint8_t> vec(1000, 0);

    HuffmanTree tree(vec);
    EXPECT_TRUE(is_correct_tree(tree, vec));
}

struct random_generator {
    mt19937 rnd;
    uniform_int_distribution<uint8_t> dist;
    random_generator(uint8_t min, uint8_t max) : dist(min, max) {
        rnd = mt19937(rand());
    }
    uint8_t operator()() {
        return dist(rnd);
    }
};

TEST(HuffmanTree, short_random_vector) {
    random_generator rnd(numeric_limits<uint8_t>::min(),
                         numeric_limits<uint8_t>::max());
    vector<uint8_t> vec(10);
    generate(vec.begin(), vec.end(), rnd);

    HuffmanTree tree(vec);
    EXPECT_TRUE(is_correct_tree(tree, vec));
}

TEST(HuffmanTree, middle_sized_random_vector) {
    random_generator rnd(numeric_limits<uint8_t>::min(),
                         numeric_limits<uint8_t>::max());
    vector<uint8_t> vec(1000);
    generate(vec.begin(), vec.end(), rnd);

    HuffmanTree tree(vec);
    EXPECT_TRUE(is_correct_tree(tree, vec));
}

TEST(HuffmanTree, long_random_vector) {
    random_generator rnd(numeric_limits<uint8_t>::min(),
                         numeric_limits<uint8_t>::max());
    vector<uint8_t> vec(1000 * 1000);
    generate(vec.begin(), vec.end(), rnd);

    HuffmanTree tree(vec);
    EXPECT_TRUE(is_correct_tree(tree, vec));
}

TEST(HuffmanTree, short_random_binary_vector) {
    random_generator rnd(0, 1);
    vector<uint8_t> vec(10);
    generate(vec.begin(), vec.end(), rnd);

    HuffmanTree tree(vec);
    EXPECT_TRUE(is_correct_tree(tree, vec));
}

TEST(HuffmanTree, middle_sized_random_binary_vector) {
    random_generator rnd(0, 1);
    vector<uint8_t> vec(1000);
    generate(vec.begin(), vec.end(), rnd);

    HuffmanTree tree(vec);
    EXPECT_TRUE(is_correct_tree(tree, vec));
}

TEST(HuffmanTree, long_random_binary_vector) {
    random_generator rnd(0, 1);
    vector<uint8_t> vec(1000 * 1000);
    generate(vec.begin(), vec.end(), rnd);

    HuffmanTree tree(vec);
    EXPECT_TRUE(is_correct_tree(tree, vec));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
