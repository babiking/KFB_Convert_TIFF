
  # KFB_Convert_TIFF
  Convert private medical digital scan format .kfb to standard .tiff file
 
 ## Requirements:
 
 * libtiff: http://www.libtiff.org/
 * gflags: https://github.com/gflags/gflags

 ## Make
 git clone https://github.com/babiking/KFB_Convert_TIFF.git
 cd KFB_Convert_TIFF
 mkdir build
 cd build
 cmake -DGFlags_DIR=path_to_gflags/gflags/build/ ..
 make
 
 ## Usage
 ./read_rgb_from_kfb --mode 0 --kfb abs_path_to_your_kfb_file --dll abs_path_to_KFB_LIB --save path_to_save_jpg_or_tiff
 ### note: 
 
 mode 0 means crop a ROI and save into jpeg format, mode 1 means crop a ROI and save into tiff format, mode 2 means save whole raw image into tiff format (TODO)

 
