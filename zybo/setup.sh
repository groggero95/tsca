#!/bin/bash

compiler="SDK/2018.3/gnu/aarch32/lin/gcc-arm-none-eabi/bin/"

flag=$(echo $PATH | grep -o $compiler)
#echo $flag

if [[ -z "$flag" ]]; then
    case $USER in
        "ansel")
	        PATH=/opt/Xilinx/SDK/2018.3/gnu/aarch32/lin/gcc-arm-none-eabi/bin/:$PATH
            ;;
        "fausto")
          PATH=/home/fausto/xilinx/SDK/2018.3/gnu/aarch32/lin/gcc-arm-none-eabi/bin/:$PATH
            #echo $PATH
            ;;
        "simone")
          PATH=/mnt/6760628c-3810-49ef-88a9-ed56dc6f2462/tools/Xilinx/SDK/2018.3/gnu/aarch32/lin/gcc-arm-none-eabi/bin/:$PATH
            ;;
        "giul")
	        PATH=/opt/Xilinx/SDK/2018.3/gnu/aarch32/lin/gcc-arm-none-eabi/bin/:$PATH
	         ;;
	    *)
            echo "Default case"
            ;;
    esac
	echo "Setting Compiler"
  # echo $PATH
fi

prebuild="SDK/2018.3/bin/"

flag=$(echo $PATH | grep -o $prebuild)
#echo $flag

if [[ -z "$flag" ]]; then
	case $USER in
        "ansel")
	        PATH=/opt/Xilinx/SDK/2018.3/bin/:$PATH
            ;;
        "fausto")
          PATH=/home/fausto/xilinx/SDK/2018.3/bin/:$PATH
            ;;
        "simone")
          PATH=/mnt/6760628c-3810-49ef-88a9-ed56dc6f2462/tools/Xilinx/SDK/2018.3/bin/:$PATH
            ;;
        "giul")
			# TODO change and put your path here, add one entry for each user
	        PATH=/opt/Xilinx/SDK/2018.3/bin/:$PATH
	         ;;
	    *)
            echo "Default case"
            ;;
    esac
	echo "Setting pre-build tool"
  # echo $PATH
fi
