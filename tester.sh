#!/bin/bash
FILE1=output1.txt
FILE2=out.txt
if cmp --silent -- "$FILE1" "$FILE2"; then
  echo "files contents are identical"
else
  echo "files differ"
fi

