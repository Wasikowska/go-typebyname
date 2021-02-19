
# the leading colon means SILENT error reporting mode
optstring=":gh"
valgrind_friendly=0

while getopts ${optstring} arg; do
    case ${arg} in
	h)
	    echo "usage: build.sh [options] [cmd]"
	    echo "  options:"
	    echo "    -g: create valgrind-friendly executable"
	    echo "    -h: display this help message"
	    echo "  cmd:"
	    echo "    build: build executable. this is the default cmd"
	    echo "    clean: remove output directory"
	    exit 0
	    ;;
	g)
	    valgrind_friendly=1
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

	cmake_args="-G Ninja .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug"
	if (( $valgrind_friendly == 1 )); then
	    cmake ${cmake_args} -DCMAKE_CXX_FLAGS="-O1 -Wall"
	else
	    cmake ${cmake_args}
	fi

	cmake --build .
    )

    if [[ ! -e compile_commands.json ]]; then
	ln -s ./output/compile_commands.json compile_commands.json
    fi
    if [[ ! -e alg ]]; then
	ln -s ./output/alg alg
    fi
elif [[ cmd = "clean" ]]; then
    if [[ -d ./output ]]; then
	rm -rf ./output
    fi
else
    echo "Invalid cmd: ${cmd}"
    exit 3
fi
