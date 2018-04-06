import cv2
import time
import numpy as np
from picamera import PiCamera
from picamera.array import PiRGBArray


class ImageProcessor:
    detect_image = True

    def start_detecting(self, target_found):
        print("ImageProcessor_start_detecting")

        camera = PiCamera()
        camera.resolution = (640, 480)
        camera.framerate = 30
        camera.video_stabilization = True
        raw_capture = PiRGBArray(camera, size=(640, 480))
        time.sleep(1)

        for frame in camera.capture_continuous(raw_capture, format="bgr", use_video_port=True):
            if not self.detect_image:
                break
            image = frame.array
            loop_time = time.time()
            gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
            gray = cv2.blur(gray, (5, 5))

            # detect edges in the image
            edged = cv2.Canny(gray, 100, 200)

            # find contours (i.e. the 'outlines') in the image
            new_image, contours, hierarchy = cv2.findContours(edged.copy(), cv2.RETR_CCOMP, cv2.CHAIN_APPROX_SIMPLE)

            centers = []
            approximations = []
            last_dimensions = []
            matches = []
            for contour in contours:
                # approximate the contour
                peri = cv2.arcLength(contour, True)
                approx = cv2.approxPolyDP(contour, 0.02 * peri, True)

                # if the approximated contour has four points its a rectangle
                if len(approx) == 4:
                    (x, y, w, h) = cv2.boundingRect(approx)
                    if h == 0:
                        h = 0.01
                    aspect_ratio = w / float(h)
                    if 0.85 <= aspect_ratio <= 1.15:
                        moments = cv2.moments(approx)
                        m00 = moments['m00']
                        if m00 == 0:
                            m00 = 0.05
                        cx = int(moments['m10'] / m00)
                        cy = int(moments['m01'] / m00)
                        is_duplicate = self.is_a_duplicate(last_dimensions, h, w)
                        if not is_duplicate:
                            temp = 0
                            first_match = True
                            for center in centers:
                                distance = np.linalg.norm(np.array((cx, cy)) - center)
                                if distance <= 10:
                                    if first_match:
                                        matches.clear()
                                        matches.append(approximations[temp])
                                        first_match = False
                                    else:
                                        print("ImageProcessor_target_detected")
                                        target_found()
                                        self.stop_detecting()
                                temp += 1
                            approximations.append(approx)
                            last_dimensions.append((w, h))
                            centers.append(np.array((cx, cy)))
            print("FPS: " + str(1 / (time.time() - loop_time)))
            raw_capture.truncate(0)
            raw_capture.seek(0)
        raw_capture.truncate(0)
        camera.close()
        cv2.destroyAllWindows()

    def stop_detecting(self):
        print("ImageProcessor_stop_detecting")
        self.detect_image = False

    def is_a_duplicate(self, last_dimensions, height, width):
        offset = 3
        for lastDimension in last_dimensions:
            if (lastDimension[0] + offset) >= width >= (lastDimension[0] - offset):
                return True

            if (lastDimension[1] + offset) >= height >= (lastDimension[1] - offset):
                return True
        return False


if __name__ == '__main__':
    image_processor = ImageProcessor()
    image_processor.start_detecting(None)
