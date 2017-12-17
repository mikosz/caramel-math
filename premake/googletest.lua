function add_googletest_files()
	local gmock_dir = path.join(ROOT_DIR, "external/googletest/googlemock")
	local gtest_dir = path.join(ROOT_DIR, "external/googletest/googletest")

	includedirs(gmock_dir)
	includedirs(gmock_dir.."/include/")
	includedirs(gtest_dir)
	includedirs(gtest_dir.."/include/")

	local gmock_all_file = gmock_dir.."/src/gmock-all.cc"
	local gtest_all_file = gtest_dir.."/src/gtest-all.cc"
	files { gmock_all_file, gtest_all_file }
	vpaths { [ ".external" ] = { gmock_all_file, gtest_all_file } }
end
