#!/bin/bash
# CMake into build directory
echo -e "\e[36m***Running CMake..."
cmake -G "Visual Studio 17 2022" -A x64 -B build | tee temp.txt
if [ "${PIPESTATUS[0]}" -ne "0" ] ; then
	echo -e "::error::CMake failed to generate the project!"
	echo -e "## \xF0\x9F\x9A\xA8\xF0\x9F\x9A\xA8 CMake failed to generate the project! \xF0\x9F\x98\xAD\xF0\x9F\x98\xAD\n" >> ${GITHUB_STEP_SUMMARY}
	echo -e "### CMake Log\n"  >> ${GITHUB_STEP_SUMMARY}
	echo -e "<pre>" >> ${GITHUB_STEP_SUMMARY}
	cat temp.txt >> ${GITHUB_STEP_SUMMARY}
	echo -e "</pre>" >> ${GITHUB_STEP_SUMMARY}
	exit 1
fi

build_failed=0
# Compile labs based on build file
while read p; do
	echo -e "\e[36m***Building" $p
	cd build
	echo -e "## $p Build\n" >> ${GITHUB_STEP_SUMMARY}
	cmake --build . --target $p --config Debug -- -v:q -clp:DisableConsoleColor | tee temp.txt
	if [ "${PIPESTATUS[0]}" -ne "0" ] ; then
		cat temp.txt >> diagnostics.txt
		echo -e "::error::Code for $p did not compile!"
		echo -e "\xE2\x9D\x8C Did not compile!\n" >> ${GITHUB_STEP_SUMMARY}
		echo -e "\xE2\x9A\xA0 There may be compiler warnings (won't know until build is fixed)\n" >> ${GITHUB_STEP_SUMMARY}
		echo -en "<details closed><summary>Build Log</summary><pre>" >> ${GITHUB_STEP_SUMMARY}
		cat temp.txt >> ${GITHUB_STEP_SUMMARY}
		echo -e "</pre></details>\n" >> ${GITHUB_STEP_SUMMARY}
		echo -e "\xE2\x9A\xA0 clang-format is not run for labs which fail to build\n" >> ${GITHUB_STEP_SUMMARY}
		echo -e "\xE2\x9A\xA0 clang-tidy is not run for labs which fail to build\n" >> ${GITHUB_STEP_SUMMARY}
		build_failed=1
		cd ..
		continue
	fi
	cat temp.txt >> diagnostics.txt

	echo -e "\xE2\x9C\x85 Compiled\n" >> ${GITHUB_STEP_SUMMARY}
	if grep -q warning temp.txt; then
		echo -e "\xE2\x9A\xA0 There are compiler warnings\n" >> ${GITHUB_STEP_SUMMARY}
		echo -en "<details closed><summary>Build Log</summary><pre>" >> ${GITHUB_STEP_SUMMARY}
		cat temp.txt >> ${GITHUB_STEP_SUMMARY}
		echo -e "</pre></details>\n" >> ${GITHUB_STEP_SUMMARY}
	else
		echo -e "\xE2\x9C\x85 There were no compiler warnings\n" >> ${GITHUB_STEP_SUMMARY}
	fi

	cd ..

	Scripts/run-clang-format.bat $p test
	cat clang-format-temp.txt >> clang-format-out.txt
	if grep -q clang-format-violations clang-format-temp.txt; then
		echo -e "\xE2\x9A\xA0 There are clang-format warnings\n" >> ${GITHUB_STEP_SUMMARY}
		echo -en "<details closed><summary>clang-format Log</summary><pre>" >> ${GITHUB_STEP_SUMMARY}
		cat clang-format-temp.txt >> ${GITHUB_STEP_SUMMARY}
		echo -e "</pre></details>\n" >> ${GITHUB_STEP_SUMMARY}
	else
		echo -e "\xE2\x9C\x85 There were no clang-format warnings\n" >> ${GITHUB_STEP_SUMMARY}
	fi

	clang-tidy --quiet --config-file=.clang-tidy $p/*.cpp -- -ILibraries/SDL/include -ILibraries/GLEW/include -ILibraries/rapidjson/include -ILibraries/stb/include | tee temp.txt
	cat temp.txt >> clang-tidy-out.txt
	if grep -q "warning:" temp.txt; then
		echo -e "\xE2\x9A\xA0 There are clang-tidy warnings\n" >> ${GITHUB_STEP_SUMMARY}
		echo -en "<details closed><summary>clang-tidy Log</summary><pre>" >> ${GITHUB_STEP_SUMMARY}
		cat temp.txt >> ${GITHUB_STEP_SUMMARY}
		echo -e "</pre></details>\n" >> ${GITHUB_STEP_SUMMARY}
	else
		echo -e "\xE2\x9C\x85 There were no clang-tidy warnings\n" >> ${GITHUB_STEP_SUMMARY}
	fi
done < BuildActual.txt

Scripts/diagnostics-win.py
Scripts/diagnostics-clang-format.py
Scripts/diagnostics-clang-tidy.py
if [[ "$build_failed" == 1 ]] ; then
	exit 1
fi
