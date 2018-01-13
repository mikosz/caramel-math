structure.executable_project("caramel-math-benchmark", false, function()
		includedirs(googlebenchmark_include_dir())
		add_googlebenchmark_libdir()
		link_googlebenchmark_lib()
	end
	)
	