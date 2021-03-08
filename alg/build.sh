#! /bin/bash

# the leading colon means SILENT error reporting mode
optstring=":ghri"

valgrind_friendly=0
debug=1
incremental=0

while getopts ${optstring} arg; do
    case ${arg} in
	h)
	    echo "usage: build.sh [options] [cmd]"
	    echo "  options:"
	    echo "    -g: create valgrind-friendly executable"
	    echo "    -h: display this help message"
	    echo "    -r: create release type executable"
	    echo "  cmd:"
	    echo "    build: build executable. this is the default cmd"
	    echo "    clean: remove output directory"
	    exit 0
	    ;;
	g)
	    valgrind_friendly=1
	    ;;
	r)
	    debug=0
	    ;;
	i)
	    incremental=1
	    ;;
	:)
	    echo "$0: Must supply an argument to -$OPTARG." >&2
	    exit 1
	    ;;
	?)
	echo "Invalid option: -${OPTARG}."
	exit 2
	;;
    esac
done

# OPTIND is the index of the cmd
if (( ${OPTIND} > $# )); then
    cmd=build
else
    shift $(( ${OPTIND} - 1 ))
    cmd=$1
fi

if [[ $cmd = "build" ]]; then
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
	export CC=`which gcc`

	if (( $incremental == 0 )); then
	    cmake_args_prefix="-G Ninja .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON"

	    cmake_build_type="Debug"
	    if (( $debug == 0 )); then
		cmake_build_type="Release"
	    fi

	    cmake_cxx_flags=""
	    if (( $valgrind_friendly == 1 )); then
		cmake_cxx_flags="-Wall -O1"
	    elif (( $debug == 1 )); then
		cmake_cxx_flags="-Wall -Og -ggdb3"
	    else
		cmake_cxx_flags="-Wall -O3 -g"
	    fi

	    # the following code is WRONG!!!
	    #
	    # the problem is, you put some double-quote chars cmake_args,
	    # when it expanded, it lost its magic, and reduced to a normal
	    # chararter.
	    #
	    # cmake ${cmake_args}

	    echo cmake ${cmake_args_prefix} -DCMAKE_BUILD_TYPE=${cmake_build_type} -DCMAKE_CXX_FLAGS="${cmake_cxx_flags}"
	    cmake ${cmake_args_prefix} -DCMAKE_BUILD_TYPE=${cmake_build_type} -DCMAKE_CXX_FLAGS="${cmake_cxx_flags}"
	fi

	cmake --build .
    )

    if [[ ! -e compile_commands.json ]]; then
	ln -s ./output/compile_commands.json compile_commands.json
    fi
    if [[ ! -e alg ]]; then
	ln -s ./output/alg alg
    fi
elif [[ $cmd = "clean" ]]; then
    if [[ -d ./output ]]; then
	rm -rf ./output
    fi
else
    echo "Invalid cmd: ${cmd}"
    exit 3
fi
