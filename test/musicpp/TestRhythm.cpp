#include <catch2/catch.hpp>
#include <musicpp/Rhythm.hpp>

using namespace music;

// Static tests
static_assert(Duration::eighth()       * 2u == Duration::quarter());
static_assert(Duration::sixteenth()    * 2u == Duration::eighth());
static_assert(Duration::thirtysecond() * 2u == Duration::sixteenth());
static_assert(Duration::sixtyfourth()  * 2u == Duration::thirtysecond());

static_assert(Duration::sixtyfourth().triplets()    * 3  * 64 == Duration::whole());
static_assert(Duration::sixtyfourth().quintuplets() * 5  * 64 == Duration::whole());
static_assert(Duration::sixtyfourth().septuplets()  * 7  * 64 == Duration::whole());
static_assert(Duration::sixtyfourth().tuplet(11)    * 11 * 64 == Duration::whole());

TEST_CASE("Duration maths work", "[rhythm]") {
	// TODO
}
