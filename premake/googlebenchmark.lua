function googlebenchmark_include_dir()
	local benchmark_dir = path.join(_MAIN_SCRIPT_DIR, "external/benchmark")
	return path.join(benchmark_dir, "include")
end

function add_googlebenchmark_libdir()
	local benchmark_dir = path.join(_MAIN_SCRIPT_DIR, "external/benchmark")
	local lib_dir = path.join(benchmark_dir, "build/src")
	
	filter "configurations:Debug*"
		libdirs(path.join(lib_dir, "Debug"))
	filter "configurations:Release*"
		libdirs(path.join(lib_dir, "Release"))
	filter {}
end

function link_googlebenchmark_lib()
	links "benchmark"
	links "Shlwapi" -- TODO: only on Windows
end
