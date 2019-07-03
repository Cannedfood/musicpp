#include <catch2/catch.hpp>

#include <musicpp/sheet/KeySignature.hpp>

#include <iostream>

using namespace music;
using namespace music::sheet;

void print(KeySignature const& key) {
	for(auto s : key.accidentals) {
		printf("%c", !s?' ':key.accidental == FLAT?'b':'#');
	}
	printf("\n");
}

TEST_CASE("Key") {
	CHECK(KeySignature{C,major()} == KeySignature{A,minor()});

	puts("C major");
	print(KeySignature{C,major()});
	puts("A minor");
	print(KeySignature{A,minor()});

	/*
	std::cout << "A Minor:\n";
	auto a_minor = minor(A);
	for(auto note : a_minor.notes) {
		std::cout << "\t" << to_string(note, a_minor.accidental) << "\n";
	}

	std::cout << "C Major:\n";
	auto c_major = major(C);
	for(auto note : c_major.notes()) {
		std::cout << "\t" << to_string(note, c_major.accidental) << "\n";
	}
	*/
}
