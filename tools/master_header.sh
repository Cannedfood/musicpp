#!/bin/bash

genHeader() {
	touch -c "$1"
	HEADER="$(realpath $1)"
	DIRECTORY="$(realpath $2)"

	echo "${DIRECTORY} -> ${HEADER}"

	( # In a subshell
		# Enter include directory
		cd "${DIRECTORY}" || exit -1

		# Clear file / Add pragma once
		printf "#pragma once\n\n"           >  "${HEADER}" || exit -1
		# Include all headers starting with a capital letter
		printf "#include \"$3%s\"\n" [A-Z]*.hpp >> "${HEADER}" || exit -1
	)
}

genHeader 'src/musicpp.hpp'        'src/musicpp/'        'musicpp/'
genHeader 'src/musicpp/Guitar.hpp' 'src/musicpp/guitar/' 'guitar/'
genHeader 'src/musicpp/Sheet.hpp'  'src/musicpp/sheet/'  'sheet/'
