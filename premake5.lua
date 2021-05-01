workspace 'Synth'

language   'C++'
cppdialect 'C++17'

configurations { 'dev', 'debug', 'release' }
includedirs {
	'src/',
	'thirdparty/',
}

------------------------------------
---------- Projects ------------------
------------------------------------

project 'musicpp'
	kind 'StaticLib'
	files 'src/musicpp/**.cpp'

project 'test'
	kind 'ConsoleApp'
	files 'test/**.cpp'
	links 'musicpp'
