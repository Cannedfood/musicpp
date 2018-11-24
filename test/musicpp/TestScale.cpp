#include <catch2/catch.hpp>

#include <musicpp/Scale.hpp>

using namespace music;

TEST_CASE("Test scale") {
	CHECK(minor().rotate(2) == major());
	CHECK(major().rotate(5) == minor());
}
