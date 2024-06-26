# uvc-undistort
OpenCV undistort implementation for UVC gadget.

## Overview
`uvc-undistort` is a project that implements image undistortion using OpenCV for UVC (USB Video Class) gadgets. This repository aims to provide efficient image correction and manipulation.



## To Do
- **Basic Greyscale Filter**: Test if OpenCV filters are being applied to thge UVC device by implementing a basic greyscale filter.
- **Code Update**: Update the `FisheyeCameraModel` C++ code to match the most recent Python [code](https://github.com/e-Trailer-BAP/eVision-prototype/tree/main).
- **Projection and Flipping**: Determine the optimal stage for projection and flipping; current consideration is to handle these on the Hub. At this moment it is still available in  `FisheyeCameraModel`
- **Timestamp Streams**: Implement timestamping of 
