# Extended Visual Cryptography for Natural Images

This is an unofficial C++ implementation for the Journal of WSCG. 2002, vol. 10, no. 1-2, p. 303-310 [Extended Visual Cryptography for Natural Images](https://otik.uk.zcu.cz/handle/11025/5993).

![](https://i.imgur.com/I09Cch8.png)

## Usage

1. The input images are in directory ```./data```. The size of each image are as the following:
    * three 01 cats 128: 128 * 128 pixels
    * three 01 cats 256: 256 * 256 pixels
    * three cats 64: 64 * 64 pixels
    * three cats 128: 128 * 128 pixels
    * three cats 256: 256 * 256 pixels
2. Copy a directory from directory ```./data``` to the current directory and name it ```input```. (Note: Do not change the directory and filenames because they are hardcoded)
  * ```c.raw``` is the image to be encrypted.
  * ```a.raw``` and ```b.raw``` are two other pictures.
3. 
```sh
bash ./scripts/run.sh image_size K_fill K_adjust
# K_fill: one pixel extends to K_fill*K_fill subpixels
# K_adjust: the contrast of the output images
# sample usage: bash ./run.sh 256 3 60
```

Finally, the generated pictures are placed in ```./res```.

* If you want to use your own dataset, make sure all three images are grayscale and the same size, and in raw format.
* To view to raw format, you can use [Photopea](https://www.photopea.com/).

## Demo

![](https://i.imgur.com/49L3CCA.png)
![](https://i.imgur.com/zmX58ZB.png)
![](https://i.imgur.com/jamiCsD.png)
![](https://i.imgur.com/ZlszWcG.png)
![](https://i.imgur.com/eqymifO.png)

## Citation
Please cite the original paper if you use this code of parts of it:
```
@article{nakajima2002extended,
  title={Extended visual cryptography for natural images},
  author={Nakajima, Mizuko and Yamaguchi, Yasushi},
  year={2002},
  publisher={UNION Agency}
}
```
