#include <errno.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

struct buf_t {
  uint8_t *data;
  size_t length;
} typedef buf_t;

static int xioctl(int fd, int request, void *arg) {
  int r;

  do {
    r = ioctl(fd, request, arg);
  } while (-1 == r && EINTR == errno);

  return r;
}

int print_caps(int fd) {
  /* Querying capabilities */
  struct v4l2_capability caps = {};
  if (xioctl(fd, VIDIOC_QUERYCAP, &caps) == -1) {
    perror("Querying Capabilities");
    return 1;
  }
  printf("Driver Caps:\n"
         "  Driver: \"%s\"\n"
         "  Card: \"%s\"\n"
         "  Bus: \"%s\"\n"
         "  Version: %d.%d\n"
         "  Capabilities: %08x\n",
         caps.driver, caps.card, caps.bus_info, (caps.version >> 16) && 0xff,
         (caps.version >> 24) && 0xff, caps.capabilities);

  /* Querying cropping capability */
  struct v4l2_cropcap cropcap = {0};
  cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (xioctl(fd, VIDIOC_CROPCAP, &cropcap) == -1) {
    perror("Querying Cropping Capabilities");
    return 1;
  }
  printf("\nCamera Cropping:\n"
         "  Bounds: %dx%d+%d+%d\n"
         "  Default: %dx%d+%d+%d\n"
         "  Aspect: %d/%d\n",
         cropcap.bounds.width, cropcap.bounds.height, cropcap.bounds.left,
         cropcap.bounds.top, cropcap.defrect.width, cropcap.defrect.height,
         cropcap.defrect.left, cropcap.defrect.top,
         cropcap.pixelaspect.numerator, cropcap.pixelaspect.denominator);

  int support_grbg10 = 0;
  struct v4l2_fmtdesc fmtdesc = {0};
  fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  char fourcc[5] = {0};
  printf("  FMT : CE Desc\n--------------------\n");
  while (0 == xioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc)) {
    strncpy(fourcc, (char *)&fmtdesc.pixelformat, 4);
    if (fmtdesc.pixelformat == V4L2_PIX_FMT_SGRBG10) {
      support_grbg10 = 1;
    }

    const char c = fmtdesc.flags & 1 ? 'C' : ' ';
    const char e = fmtdesc.flags & 2 ? 'E' : ' ';
    printf("  %s: %c%c %s\n", fourcc, c, e, fmtdesc.description);
    fmtdesc.index++;
  }

  struct v4l2_format fmt = {0};
  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (-1 == xioctl(fd, VIDIOC_G_FMT, &fmt)) {
    perror("Getting Pixel Format");
    return 1;
  }

  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width = 640;
  fmt.fmt.pix.height = 480;
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
  fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

  if (xioctl(fd, VIDIOC_S_FMT, &fmt) == -1) {
    perror("Setting Pixel Format");
    return 1;
  }

  strncpy(fourcc, (char *)&fmt.fmt.pix.pixelformat, 4);
  printf("\nSelected Camera Mode:\n");
  printf("  Width: %d\n", fmt.fmt.pix.width);
  printf("  Height: %d\n", fmt.fmt.pix.height);
  printf("  PixFmt: %s\n", fourcc);
  printf("  Field: %d\n", fmt.fmt.pix.field);
  printf("  Bytes per line: %d\n", fmt.fmt.pix.bytesperline);
  return 0;
}

int init_mmap(int fd, buf_t *buffer) {
  /* Request buffer for video capture */
  struct v4l2_requestbuffers req = {0};
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;
  req.count = 1;
  if (xioctl(fd, VIDIOC_REQBUFS, &req) == -1) {
    perror("Requesting Buffer");
    return 1;
  }

  /* Create mmap buffer */
  struct v4l2_buffer buf = {0};
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  buf.index = 0;
  if (xioctl(fd, VIDIOC_QUERYBUF, &buf) == -1) {
    perror("Querying Buffer");
    return 1;
  }

  /* Create mmap */
  buffer->data = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
                      buf.m.offset);
  buffer->length = buf.bytesused;

  return 0;
}

static void write_mjpeg(const int img_w, const int img_h, const char *outpath,
                        const uint8_t *data, const size_t length) {
  FILE *outfile = fopen(outpath, "wb");
  fwrite(data, 1, length, outfile);
  fclose(outfile);
}

static void write_pgm(const int img_w, const int img_h, const char *outpath,
                      const uint8_t *data) {
  /* Setup */
  FILE *outfile = fopen(outpath, "wb");

  /* Header */
  fprintf(outfile, "P2\n");                  /* PGM magic number */
  fprintf(outfile, "%d %d\n", img_w, img_h); /* Image width and height */
  fprintf(outfile, "255\n");                 /* maximum gray value */

  /* Image data */
  int counter = 0;
  for (int i = 0; i < img_h; i++) {
    for (int j = 0; j < img_w; j++) {
      fprintf(outfile, "%d ", data[counter]);
      counter += 2;
    }
    fprintf(outfile, "\n");
  }

  /* Clean up */
  fclose(outfile);
}

int capture_image(int fd, buf_t *buffer) {
  /* Buffer */
  struct v4l2_buffer buf = {0};
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  buf.index = 0;
  if (xioctl(fd, VIDIOC_QBUF, &buf) == -1) {
    perror("Query Buffer");
    return 1;
  }

  /* Stream on */
  if (xioctl(fd, VIDIOC_STREAMON, &buf.type) == -1) {
    perror("Start Capture");
    return 1;
  }

  /* Wait for frame */
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(fd, &fds);
  struct timeval tv = {0};
  tv.tv_sec = 2;
  if (select(fd + 1, &fds, NULL, NULL, &tv) == -1) {
    perror("Waiting for Frame");
    return 1;
  }

  /* Dequeue frame */
  if (xioctl(fd, VIDIOC_DQBUF, &buf) == -1) {
    perror("Retrieving Frame");
    return 1;
  }
  buffer->length = buf.bytesused;

  /* Output frame to file */
  /* write_pgm(640, 480, "./img.pgm", buffer->data); */
  write_mjpeg(640, 480, "./img.jpg", buffer->data, buffer->length);

  return 0;
}

int main() {
  /* Open camera device */
  int cam_dev = open("/dev/video0", O_RDWR);
  if (cam_dev == -1) {
    perror("Opening video device");
    return 1;
  }

  if (print_caps(cam_dev)) {
    return 1;
  }

  buf_t buffer;
  if (init_mmap(cam_dev, &buffer)) {
    return 1;
  }

  while (1) {
    printf("frame\n");
    if (capture_image(cam_dev, &buffer)) {
      return 1;
    }
  }

  /* Clean up */
  close(cam_dev);

  return 0;
}
