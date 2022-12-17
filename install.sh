#!/bin/bash

dst=$1
if [ "$dst" = "" ]; then 
  dst="/usr/local/bin"
fi

dst="$dst/kangoroo"

if ! ([ -f ./dic.utf8 ] && [ -f ./de.idx ] && [ -f ./kkdic.meta ]); then
  echo 'ERROR: Kangoroo resources not found'
  echo 'Please chdir to the resources directory and start this script'
  echo 'Aborting.'
  exit 1
fi

if [ -f ./bin/kangoroo ]; then
  bin="/bin/kangoroo"
elif [ -f ./kangoroo ]; then
  bin="/kangoroo"
else
  echo 'ERROR: Kangoroo binary not found'
  echo 'Please place it either in this directory or in a subdirectory ./bin'
  echo 'Aborting.'
  exit 1
fi  

echo '#!/bin/bash' >$dst
if ! [ $? = 0 ]; then
  echo "ERROR: Could not write $dst"
  echo 'please make sure you have write permission'
  echo 'Aborting.'
  exit 1
fi

echo 'if ! [ -f ~/.kangoroo ]; then' >>$dst
echo "  echo '`pwd`/' >~/.kangoroo" >>$dst
echo '  echo "$HOME/kangoroo-learn/" >>~/.kangoroo' >>$dst
echo "fi" >>$dst
echo 'if ! [ -d ~/kangoroo-learn ]; then' >>$dst
echo "  mkdir ~/kangoroo-learn/" >>$dst
echo "fi" >>$dst
echo "`pwd`$bin" >>$dst
chmod 755 $dst
echo 'Installation of Kangoroo complete.'
