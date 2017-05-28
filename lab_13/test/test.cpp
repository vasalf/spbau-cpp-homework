#include <gtest/gtest.h>
#include <my_array.h>
#include <array>

template<class T>
T construct_some();

template<>
int construct_some<int>() {
    return 1;
}

template<>
bool construct_some<bool>() {
    return true;
}

template<>
std::string construct_some<std::string>() {
    return "meow";
}

template<class T>
class TestArray : public testing::Test {
protected:
    my_array<T,1> arr1;
    my_array<T,10> arr10;
    my_array<T,100> arr100;
    my_array<T,1000> arr1000;
    T default_value;
    T some;
    TestArray() {
        default_value = T();
        arr1[0] = default_value;
        some = construct_some<T>();
    }
    virtual ~TestArray() {}
};

typedef testing::Types<int, bool, std::string> array_types;

TYPED_TEST_CASE(TestArray, array_types);

TYPED_TEST(TestArray, rewrite) {
    EXPECT_EQ(this->arr1[0], this->default_value);
    auto obj = this->arr1[0];
    EXPECT_EQ(obj, this->default_value);
    this->arr1[0] = this->some;
    EXPECT_EQ(this->arr1[0], this->some);
}

template<typename T, std::size_t N>
bool random_test(my_array<T, N>& arr) {
    std::array<T, N> stdarr;
    for (std::size_t i = 0; i < N; i++) {
        T val;
        if (rand() & 1)
            val = T();
        else
            val = construct_some<T>();
        stdarr[i] = val;
        arr[i] = val;
    }
    for (std::size_t i = 0; i < N; i++) {
        if (arr[i] != stdarr[i])
            return false;
    }
    return true;
}

TYPED_TEST(TestArray, random) {
    typedef typename decltype(this->arr1)::value_type T;
    EXPECT_TRUE((random_test<T, 1>)(this->arr1));
    EXPECT_TRUE((random_test<T, 10>)(this->arr10));
    EXPECT_TRUE((random_test<T, 100>)(this->arr100));
    EXPECT_TRUE((random_test<T, 1000>)(this->arr1000));
}

int main(int argc, char * argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
