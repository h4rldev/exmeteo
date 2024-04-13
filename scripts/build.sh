#!/usr/bin/env bash

# ExMeteo Build script
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

__NAME__="EXMeteo build script"
__DESCRIPTION__="Compiles & Links to a executable."
__VERSION__="0.0.1"

SRC="$(pwd)/src"
OUT="$(pwd)/out"
BIN="$(pwd)/bin"
DIR="${SRC}/exmeteo"
INCLUDE="$(pwd)/include"

CFLAGS=$(pkg-config --cflags gtk+-3.0)
LINKER_FLAGS=$(pkg-config --libs gtk+-3.0)

if [[ ! -d ${OUT} ]]; then
  mkdir ${OUT}
fi

print_help() {
  echo -e "${__NAME__} v${__VERSION__} \n"
  echo -e "Usage:"
  echo -e "\"-c\" | \"--compile\" ++ [OPTIONAL NAME] \\"
  echo -e "Compiles main.c to [OPTIONAL NAME].o aswell as all c files in the exmeteo subfolder.\n"
  echo -e "\"-l\" | \"--link\"    ++ [OPTIONAL NAME] \\"
  echo -e "Links all the files in \"${OUT}\" to a executable with [OPTIONAL NAME] \n"
  echo -e "[OPTIONAL NAME] default: exmeteo"
  echo -e "Made with <3 by h4rl"
}


compile() {
  local -a C_FILES
  
  local -a TRIMMED_C_FILES
  local -a TRIMMED_C_FILENAMES

  local RECOMPILE
  local RECONVERT
  local TRIMMED_C_FILE
  local TRIMMED_FONT_FILE
  local TRIMMED_C_FILENAME
  local TRIMMED_FONT_FILENAME
  local OBJECT_NAME

  if [[ -n ${1} ]]; then 
    OBJECT_NAME="${1}"
  else 
    OBJECT_NAME="exmeteo"
  fi 

  mapfile -t C_FILES < <(find "${DIR}" -type f -name "*.c")

  for ((i=0; i<${#C_FILES[@]}; i++)); do
    TRIMMED_C_FILE="${C_FILES[${i}]%.*}"
    TRIMMED_C_FILENAME="${TRIMMED_C_FILE##*/}"
    TRIMMED_C_FILES+=("${TRIMMED_C_FILE}")
    TRIMMED_C_FILENAMES+=("${TRIMMED_C_FILENAME}")
    echo "Compiling: ${C_FILES[${i}]}.."
    if [[ -f "${OUT}/${TRIMMED_C_FILENAME}.o" ]]; then
      echo -ne "${TRIMMED_C_FILENAME}.o seems to already exist, you wanna recompile it? [Y/n] "; read RECOMPILE
      if [[ "${RECOMPILE}" != [Nn] ]]; then
        gcc -O3 ${CFLAGS} -c "${C_FILES[${i}]}" -o "${OUT}/${TRIMMED_C_FILENAME}.o"
      fi
    else 
      gcc -O3 ${CFLAGS} -c "${C_FILES[${i}]}" -o "${OUT}/${TRIMMED_C_FILENAME}.o"
    fi
  done

  echo "Compiling: main.c.."
  gcc -O3 ${CFLAGS} -c "${SRC}/main.c" -o "${OUT}/main.o"

  echo "Compiled ${TRIMMED_C_FILENAMES[@]} & main successfully"
}


link() {
  local RELINK
  local -a OBJECTS
  local EXECUTABLE_NAME
  local -a TRIMMED_FILES
  
  if [[ ! -d ${BIN} ]]; then
    mkdir ${BIN}
  fi

  if [[ -n ${1} ]]; then 
    EXECUTABLE_NAME="${1}"
  else 
    EXECUTABLE_NAME="exmeteo"
  fi 

  mapfile -t OBJECTS < <(find "${OUT}" -type f -name "*.o")
  TRIMMED_FILES="${OBJECTS[@]##*/}"
  
  pushd ${OUT} > /dev/null

  echo "Linking ${TRIMMED_FILES[*]}.."

  if [[ -f ${EXECUTABLE_NAME} ]]; then
    echo -ne "${EXECUTABLE_NAME} seems to already exist, you wanna relink it? [Y/n] "; read RELINK
    if [[ "${RELINK}" != [Nn] ]]; then
      gcc ${CFLAGS} -o "${EXECUTABLE_NAME}" ${TRIMMED_FILES[*]} ${LINKER_FLAGS}
    fi
  else
    gcc ${CFLAGS} -o "${EXECUTABLE_NAME}" ${TRIMMED_FILES[*]} ${LINKER_FLAGS}
  fi

  echo "Linked ${TRIMMED_FILES} successfully"
  mv "${EXECUTABLE_NAME}" "${BIN}"

  popd > /dev/null
}

clean_dangling() {
  local DIR1
  local DIR2
  local FILE
  local LINE

  DIR1=${DIR}
  DIR2=${OUT}

  # Extract .c and .o filenames without paths and store them in temporary files
  find "$DIR1" -name "*.c" -exec basename {} \; > "temp_dir1_files.txt"
  find "$DIR2" -name "*.o" -exec basename {} \; > "temp_dir2_files.txt"
  echo "main.c" >> "temp_dir1_files.txt"

  # Compare the lists and find .o files in dir2 that do not have a corresponding .c file in dir1
  grep -Fxv -f <(sed 's/\.c$/.o/' "temp_dir1_files.txt") "temp_dir2_files.txt" > extra_o_files.txt

  # Remove extra .o files from dir2
  while read -r LINE; do
    rm -f "${DIR2}/${LINE}"
  done < extra_o_files.txt

  # Cleanup
  rm temp_dir1_files.txt temp_dir2_files.txt extra_o_files.txt
}


case $1 in
  "-c" | "--compile")
    compile $2
  ;;
  "-l" | "--link")
    clean_dangling
    link $2
  ;;
  * | "--help" | "-h" | "-?")
    print_help
  ;; 
esac
