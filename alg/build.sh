
if [[ $1 = "build" ]]; then
   if [[ ! -d ./build ]]; then
      mkdir build
   fi

   (
       cd build
       cmake -G Ninja .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug
       cmake --build .
   )

   if [[ ! -e compile_commands.json ]]; then
       ln -s ./build/compile_commands.json compile_commands.json
   fi
elif [[ $1 = "clean" ]]; then
    if [[ -d ./build ]]; then
	rm -rf ./build
    fi
else
    echo "usage: ./build.sh build|clean"
fi
