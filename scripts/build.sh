#!/usr/bin/env bash

shopt -s nullglob

__NAME__="EXMeteo build script"
__DESCRIPTION__="Compiles & Links, exmeteo to a executable."
__VERSION__="0.0.1"


LINKER_FLAGS="-lm -lglfw -lvulkan"


SRC="$(pwd)/src"
OUT="$(pwd)/out"
BIN="$(pwd)/bin"
DIR="${SRC}/exmeteo"
FONT_FOLDER="${SRC}/fonts"

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
  local -a FONTS
  
  local -a TRIMMED_C_FILES
  local -a TRIMMED_FONT_FILES
  local -a TRIMMED_C_FILENAMES
  local -a TRIMMED_FONT_FILENAMES

  local RECOMPILE
  local TRIMMED_C_FILE
  local TRIMMED_FONT_FILE
  local TRIMMED_C_FILENAME
  local TRIMMED_FONT_FILENAME
  local OBJECT_NAME


  if [[ -s "${OUT}" ]]; then
    rm -fr ${OUT}/*.o
  fi

  if [[ -n ${1} ]]; then 
    OBJECT_NAME="${1}"
  else 
    OBJECT_NAME="exmeteo"
  fi 

  mapfile -t FONTS < <(find "${FONT_FOLDER}" -type f -name "*.ttf")
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
        gcc -O3 -c "${C_FILES[${i}]}" -o "${OUT}/${TRIMMED_C_FILENAME}.o"
      fi
    else 
      gcc -O3 -c "${C_FILES[${i}]}" -o "${OUT}/${TRIMMED_C_FILENAME}.o"
    fi
  done

  for ((i=0; i<${#FONTS[@]}; i++)); do
    TRIMMED_FONT_FILE="${FONTS[${i}]%.*}"
    TRIMMED_FONT_FILENAME="${TRIMMED_FONT_FILE##*/}"
    TRIMMED_FONT_FILES+=("${TRIMMED_FONT_FILE}")
    TRIMMED_FONT_FILENAMES+=("${TRIMMED_FONT_FILENAME}")

    echo "Making ${FONTS[${i}]} into object.."
    if [[ -f "${OUT}/${TRIMMED_FILENAME}.o" ]]; then
      echo -ne "${TRIMMED_FILENAME}.o seems to already exist, you wanna reconvert it? [Y/n] "; read RECOMPILE
      if [[ "${RECOMPILE}" != [Nn] ]]; then
        ld -r -b binary -o "${OUT}/${TRIMMED_FONT_FILENAME}.o" "${FONTS[${i}]}"
      fi
    else 
      ld -r -b binary -o "${OUT}/${TRIMMED_FONT_FILENAME}.o" "${FONTS[${i}]}"
    fi
  done

  echo "Compiling: main.c.."
  gcc -O3 -c "${SRC}/main.c" -o "${OUT}/${OBJECT_NAME}.o"

  echo "Compiled ${TRIMMED_C_FILENAMES[@]} & ${OBJECT_NAME} successfully"
  echo "Converted fontfiles: ${TRIMMED_FONT_FILENAMES[@]} to binaries successfully"
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
      gcc -o "${EXECUTABLE_NAME}" ${TRIMMED_FILES[*]} $LINKER_FLAGS
    fi
  else
    gcc -o "${EXECUTABLE_NAME}" ${TRIMMED_FILES[*]} $LINKER_FLAGS
  fi

  popd > /dev/null
  
  echo "Linked ${TRIMMED_FILES} successfully"
  mv "${EXECUTABLE_NAME}" "${BIN}"
}


case $1 in
  "-c" | "--compile")
    compile $2
  ;;
  "-l" | "--link")
    link $2
  ;;
  * | "--help" | "-h" | "-?")
    print_help
  ;; 
esac
