# rectselect

rectselect is a minimalist tool written in C++ to request rectangular selections from image files from a user for further processing in shell scripts. It may be used for semi-scripted image processing (cropping, annotating, ...).

## Installation

Required dependencies: `libgtkmm-3.0-dev`

```
meson build
meson compile -C build/
meson install -C build/
```

## Usage

```
rectselect <filename> <selection count>
```

Example (request 2 rectangular selections from example.jpg):

```
rectselect example.jpg 2
```

Selections can be created using two mouse clicks. The first click of a selection can be revised using the Escape key. Pressing `Q` will cancel the collection of further selections.

The resulting geometries are written to stdout, one by line. Example: `3724x1413+1702+762` (width, height, x-offset, y-offset).

Example usage in a shell script for cropping with ImageMagick:

```
#!/bin/sh

for F in *.JPG ; do
        echo "$F"
        GEOM_COUNT=0
        for G in $(rectselect "$F" 2) ; do
                convert "$F" -crop "$G" "out/$F-$GEOM_COUNT.jpg" &
                GEOM_COUNT=$(expr $GEOM_COUNT + 1)
        done

done
```