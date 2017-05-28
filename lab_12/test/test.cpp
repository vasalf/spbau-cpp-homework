#include <gtest/gtest.h>
#include <my_vector.h>
#include <string>

template<class T>
class TestDefaultConstructor : public testing::Test {
protected:
    my_vector<T> vec;
    T val;
    TestDefaultConstructor() : val(T()) {}
    virtual ~TestDefaultConstructor() {}
};

typedef testing::Types<int, void*, std::string> types;

TYPED_TEST_CASE(TestDefaultConstructor, types);

TYPED_TEST(TestDefaultConstructor, test) {
    this->vec.resize(10);
    for (std::size_t i = 0; i != 10; i++)
        EXPECT_EQ(this->vec[i], this->val);
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
