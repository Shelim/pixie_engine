#!/bin/bash

cd "$(dirname "$0")"

if [ -d "./engine/depedency" ]; then
   
echo "Use download_dependencies to download required depedencies before doing anything else!"

else

cd ./_engine/build/doxygen

if [ -f ../../docs/index.html ]; then
    ./view_docs.sh  
else

echo "Documentation does not exists, regeneraing now... Please allow a few seconds to complete"

    ./generate_docs.sh
    ./view_docs.sh

fi

fi