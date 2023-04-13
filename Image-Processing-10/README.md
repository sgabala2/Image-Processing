# Welcome!

This is a simple Image Processing programm allowing you to:
* Mirror input images horizontally and vertically
* Change an image's exposure
* Swap the blue and green color values of an image
* Create a tiled version of an image.

## To execute the code, here's what you can do:
1. Type and enter `make all` to produce the executable
2. To see a list of commands simply run `./imgproc list`
3. Run the command you are interested in using `./imgproc exec <command> arguments`

## Commands and arguments
**To mirror an image:**
* *Horizontally:*
  `./imgproc exec mirrorh [path to input image] [output image name]`
* *Vertically:*
  `./imgproc exec mirrorh [path to input image] [output image name]`

Try this for instance: 
`./imgproc exec mirrorh data/kitten.png output/mirrored_kitten.png` and check out the new file in the output folder!

**To swap blue and green color component values of an image:** `./imgproc exec swapbg path to input image] [output image name]`

Try this for instance: `./imgproc exec swapbg data/kitten.png output/swapped_kitten.png`

**To change the exposure (intensity) of an image:** `./imgproc exec swapbg path to input image] [output image name] [value]` with a value greater than 0. This value acts as a multiplier.

Try this for instance: `./imgproc exec expose data/kitten.png output/light_kitten.png 2`

and then `./imgproc exec expose data/kitten.png output/dark_kitten.png 0.5`

**To tile source image in an NxN arrangement:** `./imgproc exec tile path to input image] [output image name] [value of N]`

Try this for instance: `./imgproc exec tile data/kitten.png output/tiled_kitten.png 3`

