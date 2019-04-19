#!/bin/bash

compiler="SDK/2018.3/gnu/aarch32/lin/gcc-arm-none-eabi/bin/"

flag=$(echo $PATH | grep -o $compiler)
#echo $flag

if [[ -z "$flag" ]]; then
    case $USER in
        "ansel")
	        PATH=/opt/Xilinx/SDK/2018.3/gnu/aarch32/lin/gcc-arm-none-eabi/bin/:$PATH
            ;;
        "fausto"|"giul"|"chiabetor")
			# TODO change and put your path here, add one entry for each user
	        PATH=/opt/Xilinx/SDK/2018.3/gnu/aarch32/lin/gcc-arm-none-eabi/bin/:$PATH
	        ;;
	    *)
            echo "Default case"
            ;;
    esac
	echo "Setting Compiler"
fi

prebuild="SDK/2018.3/bin/"

flag=$(echo $PATH | grep -o $prebuild)
#echo $flag

if [[ -z "$flag" ]]; then
	case $USER in
        "ansel")
	        PATH=/opt/Xilinx/SDK/2018.3/bin/:$PATH
            ;;
        "fausto"|"giul"|"chiabetor")
			# TODO change and put your path here, add one entry for each user
	        PATH=/opt/Xilinx/SDK/2018.3/bin/:$PATH
	        ;;
	    *)
            echo "Default case"
            ;;
    esac
	echo "Setting pre-build tool"
fi

