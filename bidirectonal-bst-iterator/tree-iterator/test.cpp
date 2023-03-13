#include "tree.hpp"
#include <algorithm>
#include <doctest.h>

TEST_CASE("can iterate full") {
    tree t{{{1, 2, 3}, 4, {5, 6, 7}}};
    int v = 1;
    for (int i : t) {
        CAPTURE(i);
        CHECK(i == v++);
    }
    REQUIRE(v == 8);
}

TEST_CASE("can iterate not full") {
    tree t{{{1, 2, nullptr}, 3, {nullptr, 4, 5}}};
    int v = 1;
    for (int i : t) {
        CAPTURE(i);
        CHECK(i == v++);
    }
    REQUIRE(v == 6);
}

TEST_CASE("can reverse iterate") {
    tree t{{{1, 2, 3}, 4, {5, 6, 7}}};
    int v = 7;
    std::for_each(std::make_reverse_iterator(t.end()),
                  std::make_reverse_iterator(t.begin()), [&v](int i) {
                      CAPTURE(i);
                      CHECK(i == v--);
                  });
    REQUIRE(v == 0);
}
