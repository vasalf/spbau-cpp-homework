#include <gtest/gtest.h>
#include <linq.h>
#include <list>

using namespace std;

TEST(LINQ, from_to_vector_copy_to) {
    vector<int> v = {1, 3, 5, 7};
    vector<int> u = v;

    vector<int> w = from(v.begin(), v.end())
        .to_vector();
    EXPECT_TRUE(w == u);

    list<int> l;
    from(v.begin(), v.end())
        .copy_to(back_inserter(l));
    EXPECT_TRUE(std::equal(l.begin(), l.end(),
                           u.begin(), u.end()));

    w = {};
    from(w.begin(), w.end())
        .copy_to(back_inserter(v));
    EXPECT_TRUE(v == u);
}

TEST(LINQ, drop) {
    vector<int> v = {1, 3, 5, 7};
    vector<string> vs = {"a", "b", "a", "c"};

    vector<int> u = {1, 3};
    from(v.begin(), v.end())
        .drop(2)
        .copy_to(back_inserter(u));
    EXPECT_TRUE(v == u);

    vector<string> us =
    from(vs.begin(), vs.end())
        .drop(4)
        .to_vector();
    EXPECT_EQ(us.size(), 0);
}

TEST(LINQ, take) {
    vector<int> v = {1, 3, 5, 7};
    vector<string> vs = {"a", "b", "a", "c"};

    vector<int> u;
    from(v.begin(), v.end())
        .take(2)
        .copy_to(back_inserter(u));
    from(v.begin(), v.end())
        .drop(2)
        .take(2)
        .copy_to(back_inserter(u));
    EXPECT_TRUE(v == u);

    vector<string> us
        = from(vs.begin(), vs.end())
        .take(4)
        .drop(4)
        .to_vector();
    EXPECT_EQ(us.size(), 0);
}

TEST(LINQ, select) {
    vector<int> v = {1, 2, 3, 4, 1791791791};
    vector<long long> u;
    vector<long long> res = {1*1, 2*2, 3*3, 4*4, (long long)1791791791 * 1791791791};

    u = from(v.begin(), v.end())
        .select<long long>([](const int& f) -> long long { return (long long)f * (long long)f; })
        .to_vector();
    EXPECT_TRUE(u == res);

    int t = 2;
    vector<int> wres = {1+t, 2+t, 3+t, 4+t, 1791791791+t};
    vector<int> w = from(v.begin(), v.end())
        .select([t](const int& f) {return f + t;})
        .to_vector();
    EXPECT_TRUE(w == wres);
}

TEST(LINQ, until_wildcard) {
    vector<int> v = {1, 2, 4, 6, 10, 11, 4, 6, 1};
    vector<int> r1 = {1, 2, 4, 6};
    vector<int> r2 = {1, 2, 4};

    int t = 100;
    vector<int> g1 = from(v.begin(), v.end())
        .until([t](const int& f) {return f * f >= t;})
        .to_vector();
    EXPECT_TRUE(g1 == r1);

    vector<int> g2 = from(v.begin(), v.end())
        .until_eq(6)
        .to_vector();
    EXPECT_TRUE(g2 == r2);
}

TEST(LINQ, where_wildcard) {
    vector<int> v = {1, 2, 4, 6, 10, 11, 4, 6, 1};
    vector<int> r1 = {1, 2, 4, 6, 4, 6, 1};
    vector<int> r2 = {1, 2, 4, 10, 11, 4, 1};

    int t = 100;
    vector<int> g1 = from(v.begin(), v.end())
        .where([t](const int& f) {return f * f < t;})
        .to_vector();
    EXPECT_TRUE(g1 == r1);

    vector<int> g2 = from(v.begin(), v.end())
        .where_neq(6)
        .to_vector();
    EXPECT_TRUE(g2 == r2);    
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
