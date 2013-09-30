#! /bin/bash
if [ -f `which convert` ]; then
    if [ $# -ge 1 ] && [ -f $1 ]; then
        for f in $(cat $1)
        do
            convert -gravity Center -crop 150x150+0+0 $f $f
        done
    else
        echo "Usage:- crop.sh image-list\nimages will be cropped at center to 150x150 in-place"
    fi
else
    echo "convert not found: Please install convert"
fi
