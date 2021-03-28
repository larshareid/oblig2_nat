#! /bin/sh
cd
filler=$(ps -eaf | grep ./fork | grep nobody | awk '{printf "%s", $2}')
echo $filler

sudo kill -9 $filler
echo "Server killed"
