
if [[ $1 = "build" || -z $1 ]]; then
    # check dot
    if ! which dot > /dev/null 2>&1; then
	echo "error: can not find graphviz tools!"
	exit
    fi

    if [[ ! -d ./output ]]; then
	mkdir output
    fi

    (
	cd output
	cmake -G Ninja .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug
	cmake --build .
    )

    if [[ ! -e compile_commands.json ]]; then
	ln -s ./output/compile_commands.json compile_commands.json
    fi
    if [[ ! -e alg ]]; then
	ln -s ./output/alg alg
    fi
elif [[ $1 = "clean" ]]; then
    if [[ -d ./output ]]; then
	rm -rf ./output
    fi
fi
