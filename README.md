DA3D: Data Adaptive Dual Domain Denoising
=========================================

Nicola Pierazzo, Jean-Michel Morel, and Gabriele Facciolo   
<{nicola.pierazzo,morel,facciolo}@cmla.ens-cachan.fr>, CMLA, ENS Paris-Saclay, France   
Complete IPOL article available at: http://www.ipol.im/pub/art/2016/203   
For future releases of the code visit: https://github.com/gfacciol/da3d   

DA3D (Data Adaptive Dual Domain Denoising) is a last step denoising method
that takes as input a noisy image and as a guide the result of any
state-of-the-art denoising algorithm. The method performs frequency domain
shrinkage on shape and data-adaptive patches. Unlike other dual denoising
methods, DA3D doesn’t process all the image samples, which allows it to use
large patches (64×64 pixels). The shape and data-adaptive patches are
dynamically selected, effectively concentrating the computations on areas with
more details, thus accelerating the process considerably. DA3D also reduces the
staircasing artifacts sometimes present in smooth parts of the guide images.

The effectiveness of DA3D is confirmed by extensive experimentation.
DA3D improves the result of almost all state-of-the-art methods, and this
improvement requires little additional computation time.


Building
--------

To compile, use

    $ mkdir build
    $ cd build
    $ cmake .. [-DCMAKE_CXX_COMPILER=/path/of/c++/compiler -DCMAKE_C_COMPILER=/path/of/c/compiler] [-DCMAKE_BUILD_TYPE=Debug]
    $ make

To rebuild, e.g. when the code is modified, use

    $ cd build
    $ make


This code only supports the PNG, JPEG, and TIFF (float) image formats
and requires the libpng, libtiff, libjpeg, and libfftw libraries.


Usage
-----

Running the program without parameters prints its usage instructions:

    $ da3d

      usage: ./da3d noisy guide sigma output

The guide image must be computed from the noisy image using another denoising algorithm for instance:
* [BM3D](http://www.ipol.im/pub/art/2012/l-bm3d/) -- https://github.com/gfacciol/bm3d
* [NL-Bayes](http://www.ipol.im/pub/art/2013/16/) -- https://github.com/npd/nl-bayes
* [NL-Means](http://www.ipol.im/pub/art/2011/bcm_nlm/) -- https://github.com/npd/nlmeans
* [Multiscale DCT Denoising](http://www.ipol.im/pub/pre/201/) -- https://github.com/gfacciol/DCTdenoising
