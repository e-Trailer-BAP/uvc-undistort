#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fcntl.h>
#include <unistd.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <string.h>
#include "FisheyeCameraModel.hpp"

using namespace cv;

#define VIRTUAL_DEVICE "/dev/video10"

int main(int argc, char **argv)
{
    // Open the video source
    VideoCapture cap(0);
    if (!cap.isOpened())
    {
        fprintf(stderr, "Error: Could not open video source.\n");
        return -1;
    }

    // Get video properties
    int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    int fps = cap.get(CAP_PROP_FPS);

    // Open the virtual video device
    int vd = open(VIRTUAL_DEVICE, O_WRONLY);
    if (vd == -1)
    {
        perror("Opening virtual video device");
        return -1;
    }

    // Configure the virtual video device
    struct v4l2_format v;
    memset(&v, 0, sizeof(v));
    v.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    v.fmt.pix.width = frame_width;
    v.fmt.pix.height = frame_height;
    v.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    v.fmt.pix.field = V4L2_FIELD_NONE;
    v.fmt.pix.bytesperline = frame_width * 2;
    v.fmt.pix.sizeimage = frame_width * frame_height * 2;
    v.fmt.pix.colorspace = V4L2_COLORSPACE_SRGB;
    if (ioctl(vd, VIDIOC_S_FMT, &v) == -1)
    {
        perror("Setting format");
        close(vd);
        return -1;
    }

    // Configure fisheye camera model
    FisheyeCameraModel camera_model("camera_params.yaml", "camera");

    Mat frame(frame_height, frame_width, CV_8UC2);
    while (true)
    {
        cap >> frame; // Capture a frame

        // If the frame is empty, break immediately
        if (frame.empty())
        {
            std::cerr << "Error: Failed to capture image" << std::endl;
            break;
        }

        // // Resize the frame to your desired resolution
        // cv::Size desired_size(960, 640); // Your desired resolution
        // cv::resize(frame, frame, desired_size);

        frame = camera_model.undistort(frame);
        // frame = camera_model.project(frame);
        // frame = camera_model.flip(frame);

        // Write the frame to the virtual video device
        if (write(vd, frame.data, frame_width * frame_height * 2) == -1)
        {
            perror("Writing to virtual video device");
            break;
        }

        // Display the processed frame (optional)
        imshow("Processed Frame", frame);
        if (waitKey(1) == 'q')
        {
            break;
        }
    }

    // Release resources
    close(vd);
    cap.release();
    destroyAllWindows();

    return 0;
}
