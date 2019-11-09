local git = require 'premake_git_helper.lua'

git.path = 'thirdparty'
git.repo(
	{ name = "wg",     url = "git@github.com:Cannedfood/wg3.git", branch = "dev" },
	{ name = "stx",    url = "https://github.com/Cannedfood/stx.git", branch = "dev" },
	{ name = "nanovg",  url = "https://github.com/memononen/nanovg.git"    },
	{ name = "catch2",     url = "https://github.com/catchorg/Catch2.git" },
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

project 'stx'
	kind 'SharedLib'
	files 'thirdparty/stx/src/**.cpp'
	links { 'pthread', 'stdc++fs' }

project 'wg'
	kind 'SharedLib'
	files 'thirdparty/wg3/src/**.cpp'
	links { 'GL', 'glfw', 'stx' }

for _, folder in ipairs(os.matchdirs "example/*") do
	local name = path.getbasename(folder)

	project(name)
		if name:match("lib")
		then kind 'SharedLib'
		else
			kind 'WindowedApp'
			links 'common-lib'
		end
		files(path.join(folder, '**.cpp'));
		links { 'wg', 'stx', 'musicpp', 'jack' }
end

project 'test'
	kind 'ConsoleApp'
	files 'test/**.cpp'
	links 'musicpp'
