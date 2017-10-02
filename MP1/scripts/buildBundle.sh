#!/bin/bash

# create the bundle directory
mkdir -p ../bundle/GIF
mkdir -p ../bundle/code

# Grab GIFs and add them into bundle
find ../data -type f | grep '.gif' | xargs -I arg cp arg ../bundle/GIF/.

# Grab source code and add it into bundle
cp ../src/* ../bundle/code/.

# add report to the directory and rename it
cp ../docs/report_mp1.pdf ../bundle/.
mv ../bundle/report_mp1.pdf ../bundle/howard_christian_a1.pdf

# create ZIP file of GIFs and code
zip -r ../bundle/code.zip ../bundle/code
zip -r ../bundle/animations.zip ../bundle/GIF
