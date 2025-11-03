#!/bin/bash

#	This script is parsed by application,
#	see system_info.c

totalmem=0;
#	using fact that devices are "files" in operating systems
#	and knowing their "locations"
#	there can be loop
for mem in /sys/devices/system/memory/memory*; do
#	check value from this specific "file"
  [[ "$(cat ${mem}/online)" == "1" ]] \
    && totalmem=$((totalmem+$((0x$(cat /sys/devices/system/memory/block_size_bytes)))));
#	above takes hex value from file and adds
done

#GUID of this script - must exactly match the one used in system_info.c
#	for simplicity case-sensitive
echo 0ca24ee1-ec84-4da0-99ec-772ae82b4cce

echo ${totalmem}