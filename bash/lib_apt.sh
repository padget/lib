# Module apt de lib permettant l'utilisation idempotente 
# de apt pour l'installation de package et la mise à jour 
# de ceux-ci ainsi que de la base des packages

# --package
# --update
# --upgrade

lib_apt() {
  local package
  local update
  local upgrade
  local state

  update="false"
  upgrade="false"

  while [ $# -gt 0 ]
  do 
    case $1 in
    "--state") state=$2; shift 2;;
    "--package") package=$2; shift 2;;
    "--update") update=true; shift 1;;
    "--upgrade") upgrade="true"; shift 1;;
    *) echo "Unexpected option: $1 - this should not happen."; return 2;;
    esac
  done

  if [ "$update" = "true" ]; then
    apt update
  fi

  if [ "$upgrade" = "true" ]; then
    apt upgrade
  fi

  case "$state" in
  "present")  
    apt install -y "$package" 
    ;;
  "absent") 
    apt remove -y "$package" 
    ;;
  *) echo "Unexpected value for 'state' option : $state";;
  esac

}