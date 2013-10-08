# About

This is a library of PEM (Probabilistic Elastic Matching) published in CVPR 2013.
Please feel free to use it for your research. More functionalities may be exposed later. Please kindly cite our paper if this library helps in your research.

For details of PEM, please refer to our paper:
[Probabilistic Elastic Matching for Pose Variant Face Verification](http://www.cv-foundation.org/openaccess/content_cvpr_2013/papers/Li_Probabilistic_Elastic_Matching_2013_CVPR_paper.pdf)

```
@InProceedings{Li_2013_CVPR,
author = {Haoxiang Li and Gang Hua and Zhe Lin and Jonathan Brandt and Jianchao Yang},
title = {Probabilistic Elastic Matching for Pose Variant Face Verification},
journal = {The IEEE Conference on Computer Vision and Pattern Recognition (CVPR)},
month = {June},
year = {2013}
}
```

# Links

A PEM model pre-trained with partial LFW-funneled is shared here (1.6GB) [PEM model](http://sdrv.ms/1bzpE7k). 
If you have no idea of what a PEM model is, please continue reading :)

# Components

PEM-Wrapper contains: 

1. source file main.cc to show how to train a PEM pipeline and apply PEM pipeline to testing images.
1. header file pem.h for the train/test APIs
1. static library for Mac OS and Linux platforms

# Dependency

PEM-Wrapper depends on:

1. gnu/g++ verision >= 4.6
1. Math Kernel Library: There are [non-commercial license for MKL](http://software.intel.com/en-us/non-commercial-software-development).
1. [OpenCV](http://opencv.org/) >= 2.4.
1. [Boost](http://www.boost.org/) (filesystem system program_options) >= 1.48

# Build

- Get a copy of PEM-wrapper
    
    git clone https://github.com/pppoe/PEM-Wrapper
    
- Edit the Makefile, 
- Build executable file

```

    $cd PEM-Wrapper
    $make
    $ls bin/*/wrapper_main
    
```
	
- Assume we are under Linux OS, binray file wrapper_main is under bin/linux/

```

    $./bin/linux/wrapper_main
    Allowed options:
      -h [ --help ]         produce help messages
      -p [ --predict ] arg  predict the units (-p pair1-image1 pair1-image2
                            pair2-image1 pair2-image2 ...)
      -t [ --train ] arg    train from the config (-t config_path)
      -m [ --pem ] arg      specify pem path (-m pem_path)
      -s [ --score ] arg    specify predicted scores path (-s predicted_score_path)
    
    Possible combinations:
    ./main -m pem-path -s score-path -t config-path (training)
    ./main -m pem-path -s score-path -p image-path1 image-path2 ... (testing)

```

# Usage

Always assign these two arguments:
-m specified the path to save a full PEM model
-s score path contains the ground-truth label, prediction label, prediction score (0~1)

### For training:
In addition to the two arguments (-m -s), add the -t argument

-t configuration-file

A full PEM-pipeline will be trained with information provided by the configuration-file

### For testing:
In addition to the two arguments (-m -s), add the -p argument

-p image-path1 image-path2	image-path1 image-path3 image-path4

Image pairs (image-path1 vs image-path2) and (image-path3 vs image-path4) will be tested by the PEM-pipeline.

# File-Format

After running a full PEM-pipeline, there will be two files generated for each image file.

## .feature_mp file
This is the file which contains extracted feature descriptors, make sure you removed them before 
re-extracting features.

## .measure_mp file
This is the file which contains the concatenated appearance descriptors as a per-image representation.
You can easily read this binary file for your own usage.

The file format is mostly a stack of floating values with a long int value at the beginning describing 
the number of floating values.

```

    [long int]
    [float] [float] [float] ...

```

You reading code for .measure_mp could be (un-tested)

```
    vector<float> measure;
    long len;
    ifs.read((char*)&len, sizeof(long));
    measure.resize(len);
    ifs.read((char*)&measure.data(), sizeof(float)*len);
```

# Demo

Details of configuration file will be explained later.

### For Training

    $./bin/linux/wrapper_main -m demo/pem -s demo/training-scores.txt -t demo/main.conf
    
    …
    …
    train libsvm  dist                                                             
    Best CV ACC 252 0.5, params 2^C 30 2^G -6
    exp - training process done
    
With the demo/main.conf, we can train a PEM with 5 matched image pairs and 5 nonmatched image pairs. The full PEM-pipeline model will be saved to demo/pem.actorPEM_param. Apparently, with 10 image pairs, we cannot expect to get a working face verification system. But as a demo, this is enough to show how the library works.

### For Testing
After we get the demo/pem.actorPEM_param, we can predict if a pair of face images are matched or not.
        
    $./bin/linux/wrapper_main -m demo/pem -s demo/testing-scores -p demo/Aaron/Aaron_Eckhart_0001.jpg  demo/Aaron/Aaron_Peirsol_0001.jpg  demo/Aaron/Aaron_Peirsol_0002.jpg  demo/Aaron/Aaron_Peirsol_0003.jpg
    
    …
    …
    …
    Prediction: 
    demo/Aaron/Aaron_Eckhart_0001.jpg demo/Aaron/Aaron_Peirsol_0001.jpg : 0.449411 (different) 
    demo/Aaron/Aaron_Peirsol_0002.jpg demo/Aaron/Aaron_Peirsol_0003.jpg : 0.657607  (same)
   
As shown, this command-line will apply PEM on two image pairs: Aaron_Eckhart_0001.jpg vs Aaron_Peirsol_0001.jpg, and Aaron_Peirsol_0002.jpg vs Aaron_Peirsol_0003.jpg.
The first face pair is predicted as nonmatched as they are from two different person while the other one is prediceted as matched.
    

# Configuration

#### Note: This section will be updated later to introduce more options in using PEM

The configuration file consists of keys and values, texts after # are comments
It specified information including:

1. Feature extraction parameters, such as type of descriptor, spacing, patch size etc.
1. UBM training, such as the number of gaussians in the UBM-GMM, the images for training the UBM-GMM
1. SVM training, including the labeled image pairs and their labels

    [keyA]
    value1
    value2
    value3
    [keyB]
    value1
    value2
    value3

Make a copy of demo/main.conf for customization, please only change the value fields.

### Options/Keys

	[training_label-list]
	1
	1
	1
	1
	1
	0
	0
	0
	0
	0
	[training_pair-list]
	./demo/Aaron/Aaron_Peirsol_0001.jpg
	./demo/Aaron/Aaron_Peirsol_0002.jpg
	./demo/Aaron/Aaron_Peirsol_0003.jpg
	./demo/Aaron/Aaron_Peirsol_0004.jpg
	./demo/Aaron/Aaron_Peirsol_0004.jpg
	./demo/Aaron/Aaron_Peirsol_0002.jpg
	./demo/Aaron/Aaron_Peirsol_0001.jpg
	./demo/Aaron/Aaron_Peirsol_0003.jpg
	./demo/Aaron/Aaron_Peirsol_0004.jpg
	./demo/Aaron/Aaron_Peirsol_0001.jpg
	./demo/Aaron/Aaron_Guiel_0001.jpg
	./demo/Aaron/Aaron_Patterson_0001.jpg
	./demo/Aaron/Aaron_Pena_0001.jpg
	./demo/Aaron/Aaron_Tippin_0001.jpg
	./demo/Aaron/Aaron_Eckhart_0001.jpg
	./demo/Aaron/Aaron_Peirsol_0003.jpg
	./demo/Aaron/Aaron_Sorkin_0002.jpg
	./demo/Aaron/Aaron_Peirsol_0004.jpg
	./demo/Aaron/Aaron_Peirsol_0004.jpg
	./demo/Aaron/Aaron_Sorkin_0001.jpg
	[training_unit-list]
	./demo/Aaron/Aaron_Eckhart_0001.jpg
	./demo/Aaron/Aaron_Guiel_0001.jpg
	./demo/Aaron/Aaron_Patterson_0001.jpg
	./demo/Aaron/Aaron_Peirsol_0001.jpg
	./demo/Aaron/Aaron_Peirsol_0002.jpg
	./demo/Aaron/Aaron_Peirsol_0003.jpg
	./demo/Aaron/Aaron_Peirsol_0004.jpg
	./demo/Aaron/Aaron_Pena_0001.jpg
	./demo/Aaron/Aaron_Sorkin_0001.jpg
	./demo/Aaron/Aaron_Sorkin_0002.jpg
	./demo/Aaron/Aaron_Tippin_0001.jpg
	[threads-count]
	24
	######## EM ########
	[spawnpoints-value]
	8
	[em_param_use_equal_variance-bool]
	yes
	######## Descriptor ########
	[scales-count]
	3
	[factor-value]
	0.9
	[spacing-value]
	8
	[binsize-value]
	8
	[gridsize-value]
	8
	[featuretype-unit]
	dsift
	[aggregatetype-unit]
	default
	[withloc-bool]
	yes
	[locfactor-value]
	2.0


``[training_label-list]`` specified a list of labels for images pairs listed under ``[training_pair-list]`` , 1 indicates matched and 0 indicates nonmatched.

Number of items under ``[training_pair-list]`` should twice as the number of items under ``[training_label-list]``. Every two lines under ``[training_pair-list]`` are regarded as an image pair with the corresponded label specified under ``[training_label-list]``, sequencely. 

``[training_unit-list]`` lists the images used to train the UBM-GMM.
``[threads-count]`` is designed to specify number of threads one want to use in training and testing the PEM. This is related to OpenMP and it may not work as expected.

``[spawnpoints-value]`` is the number of gaussians in the UBM-GMM.
Please always keep the value of ``[em_param_use_equal_variance-bool]`` to be yes.

The following keys are for feature extraction.
``[scales-count]`` is the number of scales of the image pyramid.
``[factor-value]`` is the scaling factor of the image pyramid.
``[spacing-value]`` is the spacing. 8 means to extract a descriptor every 8 pixels.
``[gridsize-value]`` is the size of image grid which consists of cells. And the 
``[binsize-value]`` is the size of cells inside the grid. 
Here 

	[binsize-value]
	8
	[gridsize-value]
	8

means to extract one descriptor for each 8x8 grid. If the setting is like 

	[binsize-value]
	8
	[gridsize-value]
	32

It means to extract one descriptor for each 32x32 grid which is the concatenation of 16 descriptors extracted from 16 8x8 cells.

``[featuretype-unit]`` is the type of feature descriptor. Here we support ``dsift`` which is SIFT feature and ``LBP`` which is Local Binary Pattern feature. For details please refer to our paper.

Please keep ``[aggregatetype-unit]`` to be default.

Set ``[withloc-bool]`` to be "yes" will extract spatial-augmented features.
``[locfactor-value]`` is the scaling factor the spatial part of the spatial-augmented features.

	
# Credits to 3rd-Party Libraries

PEM utilized [Vlfeat](http://www.vlfeat.org/) and [Libsvm](http://www.csie.ntu.edu.tw/~cjlin/libsvm/), awesome work!
  
# LICENSE

Usage of copy of this software is restricted to research purpose only. All rights reserved.
