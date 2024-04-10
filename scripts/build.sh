#!/usr/bin/env bash

shopt -s nullglob


__VERSION__="0.0.1"
__NAME__="EXMeteo build script"
__DESCRIPTION__="Compiles & Links, exmeteo to a executable."



DIR="$(pwd)/src"
EXMETEO_DIR="${DIR}/exmeteo"
OUT="$(pwd)/out"

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
  local RECOMPILE
  local -a C_FILES
  local TRIMMED_FILE
  local OBJECT_NAME
  local TRIMMED_FILENAME

  if [[ -s "${OUT}" ]]; then
    rm -fr ${OUT}/*.o
  fi

  if [[ -n ${1} ]]; then 
    OBJECT_NAME="${1}"
  else 
    OBJECT_NAME="exmeteo"
  fi 

  mapfile -t C_FILES < <(find "${EXMETEO_DIR}" -type f -name "*.c")

  for ((i=0; i<${#C_FILES[@]}; i++)); do
    TRIMMED_FILE=${C_FILES[${i}]%.*};
    TRIMMED_FILENAME="${TRIMMED_FILE##*/}"
    echo "Compiling: ${C_FILES[${i}]}.."
    if [[ -f "${OUT}/${TRIMMED_FILENAME}.o" ]]; then
      echo -ne "${TRIMMED_FILENAME}.o seems to already exist, you wanna recompile it? [Y/n] "; read RECOMPILE
      if [[ "${RECOMPILE}" != [Nn] ]]; then
        gcc -O3 -c "${C_FILES[${i}]}" -o "${TRIMMED_FILE}.o"
      fi
    else 
      gcc -O3 -c "${C_FILES[${i}]}" -o "${TRIMMED_FILE}.o"
    fi
    mv "${TRIMMED_FILE}.o" "${OUT}"
  done
  echo "Compiling: main.c.."
  gcc -O3 -c "${DIR}/main.c" -o "${OBJECT_NAME}.o"
  mv "${OBJECT_NAME}.o" "${OUT}"
}


link() {
  local RELINK
  local -a OBJECTS
  local EXECUTABLE_NAME
  local -a TRIMMED_FILES
  

  if [[ -n ${1} ]]; then 
    EXECUTABLE_NAME="${1}"
  else 
    EXECUTABLE_NAME="exmeteo"
  fi 

  mapfile -t OBJECTS < <(find "${OUT}" -type f -name "*.o") 
  TRIMMED_FILES="${OBJECTS[@]##*/}"
  
  pushd ${OUT} > /dev/null

  echo "Linking ${TRIMMED_FILES[*]}..";

  if [[ -f ${EXECUTABLE_NAME} ]]; then
    echo -ne "${EXECUTABLE_NAME} seems to already exist, you wanna relink it? [Y/n] "; read RELINK
    if [[ "${RELINK}" != [Nn] ]]; then
      gcc -o "${EXECUTABLE_NAME}" ${TRIMMED_FILES[*]}
    fi
  else
    gcc -o "${EXECUTABLE_NAME}" ${TRIMMED_FILES[*]}
  fi

  popd > /dev/null
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
