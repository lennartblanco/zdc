#!/bin/sh

COMPILER_BINARY="./xdc_bin"

help()
{
	echo "help not available, hah!"
}

outfile=""
infiles=""

while getopts "hi:o:" optname; do
    case "${optname}" in
	"h")	help; exit ;;
	"i")	infiles="$infiles $OPTARG" ;;
	"o")	outfile="> $OPTARG" ;;
    esac
done

cat $infiles | $COMPILER_BINARY $outfile
