#!/usr/bin/env bash

shopt -s nullglob

DIR="$(pwd)/exmeteo"

compile() {
  local RECOMPILE
  local -a C_FILES
  local TRIMMED_FILE
  mapfile -t C_FILES < <(find "${DIR}" -type f -name "*.c")

  for ((i=0; i<${#C_FILES[@]}; i++)); do
    TRIMMED_FILE=${C_FILES[${i}]%.*}; 
    echo "Compiling: ${C_FILES[${i}]}.."
    if [[ -f "${TRIMMED_FILE}.o" ]]; then
      echo -ne "${TRIMMED_FILE}.o seems to already exist, you wanna recompile it? [Y/n] "; read RECOMPILE
      if [[ "${RECOMPILE}" != [Nn] ]]; then
        gcc -O3 -c "${C_FILES[${i}]}" -o "${TRIMMED_FILE}.o"
      fi
    else 
      gcc -O3 -c "${C_FILES[${i}]}" -o "${TRIMMED_FILE}.o"
    fi
  done
  echo "Compiling: main.c.."
  gcc -O3 -o ${2}.o -c main.c
}


link() {
  local RELINK
  local -a OBJECTS
  mapfile -t OBJECTS < <(find "${DIR}" -type f -name "*.o") 

  if [[ -f ${2} ]]; then
    echo -ne "${2} seems to already exist, you wanna relink it? [Y/n] "; read RELINK
    if [[ "${RECOMPILE}" != [Nn] ]]; then
      gcc -o $2 "${2}.o" "${OBJECTS[*]}"
    fi
  else 
    gcc -o $2 "${2}.o" "${OBJECTS[*]}"
  fi
}


case $1 in
  "-c" | "--compile")
    compile $2
  ;;
  "-l" | "--link")
    link $2
  ;;
  * | "--help" | "-h" | "-?")
    echo "Help WIP"
  ;; 
esac
