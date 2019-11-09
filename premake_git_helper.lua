-- *_async functions return a closure that awaits completion when called

local git = {}

local function _execute_async(fmt, ...)
	local cmd = string.format(fmt, ...)

	local execution = assert(io.popen(cmd))

	return function()
		for line in execution:lines() do
			print(line)
		end
		execution:close()
	end
end

local function _clone_async(desc)
	local command = {
		"git clone"
	}

	if desc.branch then
		table.insert(command, "--branch")
		table.insert(command, desc.branch)
	end

	table.insert(command, "--single-branch")
	table.insert(command, "--depth 1")
	table.insert(command, desc.url)
	table.insert(command, path.join(git.path, desc.name))

	return _execute_async(table.concat(command, ' '))
end

local function _pull_async(desc)
	return _execute_async("cd %s && git pull", desc.path)
end

local function _check_and_extend_description(desc)
	local expected = '{ name="<name>", url="<url>", location="<path-relative-to-git.directory>" }'

	assert(desc.name, 'Repo description needs a name. The full expected format is ' .. expected)
	assert(desc.url,  'Repo description needs a url. The full expected format is '  .. expected)

	if not desc.path then
		desc.path = path.join(git.path, desc.name)
	end

	return desc
end

local function _repo_async(desc)
	desc = _check_and_extend_description(desc)

	if not os.isdir(desc.path) then
		return _clone_async(desc)
	else
		return _pull_async(desc)
	end
end

-- Public interface

git.path = "dependencies"

function git.repo(...)
	local args = {...};

	-- Execute everything in parallel
	local awaiters = {}
	for _, desc in ipairs(args) do
		table.insert(awaiters, _repo_async(desc))
	end

	-- Await for all tasks to complete
	for _, awaiter in ipairs(awaiters) do
		awaiter()
	end
end

return git

