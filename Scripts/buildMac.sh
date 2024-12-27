#!/bin/bash
brew install ninja
# CMake into build directory
echo -e "***Running CMake..."
cmake -G Ninja -B build | tee temp.txt
if [ "${PIPESTATUS[0]}" -ne "0" ] ; then
	echo -e "::error::CMake failed to generate the project!"
	echo -e "## \xF0\x9F\x9A\xA8\xF0\x9F\x9A\xA8 CMake failed to generate the project! \xF0\x9F\x98\xAD\xF0\x9F\x98\xAD" >> ${GITHUB_STEP_SUMMARY}
    echo -e "### CMake Log"  >> ${GITHUB_STEP_SUMMARY}
    echo -en "<pre>" >> ${GITHUB_STEP_SUMMARY}
    cat temp.txt >> ${GITHUB_STEP_SUMMARY}
    echo -e "</pre>" >> ${GITHUB_STEP_SUMMARY}
	exit 1
fi

cd build
build_failed=0
# Compile labs based on build file
while read p; do
	echo -e "***Building $p..."
	echo -e "## $p Build\n" >> ${GITHUB_STEP_SUMMARY}
	cmake --build . --target $p --config Debug | tee temp.txt
	if [ "${PIPESTATUS[0]}" -ne "0" ] ; then
		cat temp.txt >> diagnostics.txt
		echo -e "::error::Code for $p did not compile!"
		echo -e "\xE2\x9D\x8C Did not compile!\n" >> ${GITHUB_STEP_SUMMARY}
		echo -e "\xE2\x9A\xA0 There may be compiler warnings (won't know until build is fixed)\n" >> ${GITHUB_STEP_SUMMARY}
		echo -en "<details closed><summary>Build Log</summary><pre>" >> ${GITHUB_STEP_SUMMARY}
		cat temp.txt >> ${GITHUB_STEP_SUMMARY}
		echo -e "</pre></details>\n" >> ${GITHUB_STEP_SUMMARY}
		build_failed=1
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
done < ../BuildActual.txt

cd ..
Scripts/diagnostics-mac.py
if [[ "$build_failed" == 1 ]] ; then
	exit 1
fi
