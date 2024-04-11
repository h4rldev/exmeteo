default:
  just --list

compile name="exmeteo":
  scripts/build.sh -c {{name}}

link name="exmeteo":
  scripts/build.sh -l {{name}}

@build name="exmeteo":
  echo "Compiling.."
  just compile {{name}}
  echo "Linking.."
  just link {{name}}

@run name="exmeteo":
  echo "Compiling.."
  just compile {{name}}
  echo "Linking.."
  just link {{name}}
  echo "Running!"
  ./bin/{{name}}
