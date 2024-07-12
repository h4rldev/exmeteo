#!/usr/bin/env bash

# exMeteo: build.sh
#
# built with this inspirational message in mind: Why tf would I use make
#
# Licensed under the:
# BSD 3-Clause License
#
# Copyright (c) 2024, H4rl
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#
# Made with <3 by h4rl.

shopt -s nullglob

__NAME__="exMeteo: build.sh"
__AUTHOR__="h4rl"
__DESCRIPTION__="Compiles & links exMeteo into a executable."
__LICENSE__="BSD 3-Clause License"
__VERSION__="0.1.0"

SRC="$(pwd)/src"
OUT="$(pwd)/out"
BIN="$(pwd)/bin"
DIR="${SRC}/exmeteo"
#INCLUDE="$(pwd)/include"
COLOR=true

if ${COLOR}; then
	RED='\033[0;31m'
	GREEN='\033[0;32m'
	YELLOW='\033[1;33m'
	BLUE='\033[0;34m'
	CYAN='\033[0;36m'
	CLEAR='\033[0m'
fi

if [[ ${3} == "--debug" ]]; then
	DEBUG=true
else
	DEBUG=false
fi

#CFLAGS=$(pkg-config --cflags gtk+-3.0)
CFLAGS="-O3"
#LINKER_FLAGS="$(pkg-config --libs gtk+-3.0) -lcurl -ljansson"
LINKER_FLAGS="-lcurl -ljansson"

if [[ ! -d ${OUT} ]]; then
	mkdir "${OUT}"
fi

print_help() {
	echo -e "${RED}${__NAME__}${CLEAR} v${GREEN}${__VERSION__}${CLEAR}"
	echo -e "Licensed under: ${CYAN}${__LICENSE__}${CLEAR}\n"
	echo -e "USAGE: ${GREEN}${0}${CLEAR} ${RED}{FLAGS} ${BLUE}{EXECUTABLE_NAME}${CLEAR}"
	echo -e "${RED}-c${CLEAR} | ${RED}--compile${CLEAR}"
	echo -e "Compiles ${CYAN}${SRC}/main.c${CLEAR} & all files in ${CYAN}${DIR}/${CLEAR}.\n"
	echo -e "${RED}-l${CLEAR} | ${RED}--link${CLEAR}"
	echo -e "Links all the files in ${CYAN}${OUT}/${CLEAR} to an executable with ${BLUE}{EXECUTABLE_NAME}${CLEAR}\n"
	echo -e "${RED}-d${CLEAR} | ${RED}--delete${CLEAR} | ${RED}--clean${CLEAR}"
	echo -e "Cleans all files in ${CYAN}${BIN}/${CLEAR} & ${CYAN}${OUT}/${CLEAR}\n"
	echo -e "Made with ${RED}<3${CLEAR} by ${__AUTHOR__}"
}

# iterates over every file in src/exmeteo/ and compiles them, then compiles src/main.c, moving them to out/

compile() {
	local -a C_FILES

	local -a TRIMMED_C_FILES
	local -a TRIMMED_C_FILENAMES

	local RECOMPILE
	local TRIMMED_C_FILE
	local TRIMMED_C_FILENAME

	mapfile -t C_FILES < <(find "${DIR}" -type f -name "*.c")

	for ((i = 0; i < ${#C_FILES[@]}; i++)); do
		TRIMMED_C_FILE="${C_FILES[${i}]%.*}"
		TRIMMED_C_FILENAME="${TRIMMED_C_FILE##*/}"
		TRIMMED_C_FILES+=("${TRIMMED_C_FILE}")
		TRIMMED_C_FILENAMES+=("${TRIMMED_C_FILENAME}")
		echo -e "${BLUE}>${CLEAR} Compiling: ${CYAN}${C_FILES[${i}]}${CLEAR}.."
		if [[ -f "${OUT}/${TRIMMED_C_FILENAME}.o" ]]; then
			echo -ne "${YELLOW}!${CLEAR} ${CYAN}${TRIMMED_C_FILENAME}.o${CLEAR} seems to already exist, you wanna recompile it? [${GREEN}Y${CLEAR}/${RED}n${CLEAR}]: "
			read -r RECOMPILE
			if [[ ! ${RECOMPILE} =~ [Nn] ]]; then
				if ${DEBUG}; then
					gcc "${CFLAGS}" -ggdb -c "${C_FILES[${i}]}" -o "${OUT}/${TRIMMED_C_FILENAME}.o"
				else
					gcc -O3 "${CFLAGS}" -c "${C_FILES[${i}]}" -o "${OUT}/${TRIMMED_C_FILENAME}.o"
				fi
			fi
		else
			if ${DEBUG}; then
				gcc ${CFLAGS} -ggdb -c "${C_FILES[${i}]}" -o "${OUT}/${TRIMMED_C_FILENAME}.o"
			else
				gcc ${CFLAGS} -c "${C_FILES[${i}]}" -o "${OUT}/${TRIMMED_C_FILENAME}.o"
			fi
		fi
	done

	echo -e "${BLUE}>${CLEAR} Compiling: main.c.."
	gcc "${CFLAGS}" -c "${SRC}/main.c" -o "${OUT}/main.o"

	echo -e "${GREEN}✓${CLEAR} Compiled ${CYAN}${TRIMMED_C_FILENAMES[*]}${CLEAR} & ${CYAN}main${CLEAR} successfully"
}

# links all object files in out/ to an executable in /bin

link() {
	local RELINK
	local -a OBJECTS
	local EXECUTABLE_NAME
	local -a TRIMMED_FILES

	if [[ ! -d ${BIN} ]]; then
		mkdir "${BIN}"
	fi

	if [[ -n ${1} ]]; then
		EXECUTABLE_NAME="${1}"
	else
		echo -e "${RED}!${CLEAR} No executable name set, using default."
		EXECUTABLE_NAME="exmeteo"
	fi

	mapfile -t OBJECTS < <(find "${OUT}" -type f -name "*.o")
	TRIMMED_FILES="${OBJECTS[*]##*/}"

	pushd "${OUT}" >/dev/null || echo "Failed to pushd" && exit 1

	echo -e "${BLUE}>${CLEAR} Linking: ${CYAN}${TRIMMED_FILES[*]}${CLEAR}.."

	if [[ -f "${BIN}/${EXECUTABLE_NAME}" ]]; then
		echo -ne "${YELLOW}!${CLEAR} ${CYAN}${EXECUTABLE_NAME}${CLEAR} seems to already exist, you wanna relink it? [${GREEN}Y${CLEAR}/${RED}n${CLEAR}]: "
		read -r RELINK
		if [[ ! ${RELINK} =~ [Nn] ]]; then
			gcc ${CFLAGS} -o "${BIN}/${EXECUTABLE_NAME}" ${TRIMMED_FILES[*]} ${LINKER_FLAGS}
			echo -e "${GREEN}✓${CLEAR} Linked ${CYAN}${TRIMMED_FILES}${CLEAR} successfully"
		fi
	else
		gcc ${CFLAGS} -o "${BIN}/${EXECUTABLE_NAME}" ${TRIMMED_FILES[*]} ${LINKER_FLAGS}
		echo -e "${GREEN}✓${CLEAR} Linked ${CYAN}${TRIMMED_FILES}${CLEAR} successfully"

	fi

	popd >/dev/null || echo "Failed to popd" && exit 1
}

# removes dangling object files that shouldn't be there, used to be required, not that much as of lately though.

clean_dangling() {
	local DIR1
	local DIR2
	local LINE

	DIR1=${DIR}
	DIR2=${OUT}

	# Extract .c and .o filenames without paths and store them in temporary files
	find "$DIR1" -name "*.c" -exec basename {} \; >"temp_dir1_files.txt"
	find "$DIR2" -name "*.o" -exec basename {} \; >"temp_dir2_files.txt"
	echo "main.c" >>"temp_dir1_files.txt"

	# Compare the lists and find .o files in dir2 that do not have a corresponding .c file in dir1
	grep -Fxv -f <(sed 's/\.c$/.o/' "temp_dir1_files.txt") "temp_dir2_files.txt" >"extra_o_files.txt"

	# Remove extra .o files from dir2
	while read -r LINE; do
		rm -f "${DIR2}/${LINE}"
	done <"extra_o_files.txt"

	# Cleanup
	rm temp_dir1_files.txt temp_dir2_files.txt extra_o_files.txt
}

# cleans both /out && /bin

clean() {
	local CLEAN

	echo -e "${RED}!${CLEAR} Cleaning ${CYAN}${OUT}${CLEAR} & ${CYAN}${BIN}${CLEAR}."
	echo -ne "${RED}!${CLEAR} You sure you want to proceed? [${GREEN}y${CLEAR}/${RED}N${CLEAR}]: "
	read -r CLEAN
	if [[ ${CLEAN} =~ [Nn] ]]; then
		rm -fr "${OUT:?}/*"
		rm -fr "${BIN:?}/*"
		echo -e "${GREEN}✓${CLEAR} Cleaned ${CYAN}${OUT}${CLEAR} & ${CYAN}${BIN}${CLEAR} successfully."
	else
		echo -e "${GREEN}✓${CLEAR} Cancelled."
	fi
}

case $1 in
"-c" | "--compile")
	compile
	;;
"-l" | "--link")
	clean_dangling
	link "${2}"
	;;
"-d" | "--delete" | "--clean")
	clean
	;;
"--help" | "-h" | "-?" | *)
	print_help "${@}"
	;;
esac
