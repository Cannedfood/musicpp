local git = require 'premake_git_helper'

git.path = 'thirdparty'
git.repo(
	{ name = "catch2",     url = "https://github.com/catchorg/Catch2.git" }
)

workspace 'Synth'

language   'C++'
cppdialect 'C++17'

configurations { 'dev', 'debug', 'release' }
filter 'configurations:release or dev'
	optimize 'Speed'
filter 'configurations:dev or debug'
	symbols 'On'
filter 'configurations:debug'
	optimize 'Debug'
filter 'configurations:release'
	flags 'LinkTimeOptimization'
filter { 'configurations:dev or debug', 'toolset:gcc or clang' }
	buildoptions { '-Wall', '-Wextra', '-Wno-unused-parameter' }
filter {}

flags { 'MultiProcessorCompile', 'NoIncrementalLink' }
vectorextensions 'SSE2'
floatingpoint 'Fast'

platforms { 'x64', 'x86', 'ARM' }
filter 'platforms:x64'
	architecture 'x86_64'
filter 'platforms:x86'
	architecture 'x86'
filter 'platforms:ARM'
	architecture 'ARM'
filter {}

includedirs {
	'src',
	'thirdparty/nanovg/src/',
	'thirdparty/catch2/single_include/',
	'thirdparty/wg3/src/',
	'thirdparty/stx/src/'
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
