# !Created by babiking@sensetime on May 13th, 2018 to test openslide APIs and code encapsulation/wrapper pipeline

import openslide
import numpy as np
from matplotlib import pyplot as plt



# !API-1: Open slide for .tiff/.svs/.mrxs/.bif/.ndpi/.vms/.vmu/.svslide file
slide = openslide.OpenSlide("data/test.tiff")



# !API-2: Level-count i.e. scanning resolution levels or scale factor 40X
levelCount = slide.level_count



# !API-3: Dimensions
[nImageWidth, nImageHeight] = slide.dimensions
# ! or dimensions at level k
k = 0
[nImageWidthK, nImageHeightK] = slide.level_dimensions[k]    # [nImageWidth, nImageHeight]
slideDownsampleK = slide.level_downsamples[k]



# !API-4: Read ROI area
'''
    Function:
        read_region(location, level, size)
    Input:
        [1] locate i.e. [pX, pY] the left-top corner of read-in slideImage
        [2] level  i.e. downsampling scale level
        [3] size   i.e. size of ROI area
    Output:
        [1] roi_image
'''
roi_image = np.asarray(slide.read_region((0,0), 0, (256, 256)))
plt.figure()
plt.imshow(roi_image)



# !API-5: Get thumbnail image at specific size
thumbnail_image = np.asarray(slide.get_thumbnail((1024, 1024)))
plt.figure()
plt.imshow(thumbnail_image)
plt.show()



# !API-6: Close file
slide.close()

