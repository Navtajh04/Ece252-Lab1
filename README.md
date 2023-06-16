# Ece252-Lab1

This repository contains 2 main functionalities: findpng and catpng

some sample PNG images are provided in the `images` directory for testing and to see how catpng and findpng work

### catpng

Catpng can be used to vertically concatenate multiple PNG files of the same width. 

To use catpng, make sure you are on a linux system and run the command `cd lab1/catpng` then run `make` and `cd build`.

To run catpng, the format is `./catpng [PNG FILE 1 ABS PATH] [PNG FILE 2 ABS PATH] ...` where you can add as many png files as you want to concatenate and they will be concatenated from top to bottom in the order that they are specified.

### findpng

Findpng will search the specified directory for any real PNG files (ignoring files that are .png but are not actually png images) and print out the paths to those files.

To use findpng, make sure you are on a linux system and run the command `cd lab1/findpng` then run `make` and `cd build`.

Now you can use findpng as follows `./findpng [DIRECTORY ABSOLUTE PATH]` and it will search that directory for any real PNG files

