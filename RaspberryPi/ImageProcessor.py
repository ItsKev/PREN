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

            # find contours (in the image
            _, contours, _ = cv2.findContours(edged.copy(), cv2.RETR_CCOMP, cv2.CHAIN_APPROX_SIMPLE)

            centers = []
            last_dimensions = []
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

                    # check if the rectangle is a square
                    if 0.85 <= aspect_ratio <= 1.15:
                        moments = cv2.moments(approx)
                        m00 = moments['m00']
                        if m00 == 0:
                            m00 = 0.05

                        # get the center of the square
                        cx = int(moments['m10'] / m00)
                        cy = int(moments['m01'] / m00)
                        is_duplicate = self.is_a_duplicate(last_dimensions, h, w)
                        if not is_duplicate:
                            if self.check_if_target_found(centers, cx, cy, target_found):
                                break
                            last_dimensions.append((w, h))
                            centers.append(np.array((cx, cy)))
            # print("FPS: " + str(1 / (time.time() - loop_time)))
            raw_capture.truncate(0)
            raw_capture.seek(0)
        raw_capture.truncate(0)
        camera.close()
        cv2.destroyAllWindows()

    def check_if_target_found(self, centers, cx, cy, target_found) -> bool:
        first_match = True
        for center in centers:
            distance = np.linalg.norm(np.array((cx, cy)) - center)
            if distance <= 10:
                if first_match:
                    first_match = False
                else:
                    if self.is_target_in_center(cx, cy):
                        print("ImageProcessor_target_detected")
                        target_found()
                        self.detect_image = False
                        return True
        return False

    def is_a_duplicate(self, last_dimensions, height, width) -> bool:
        offset = 3
        for lastDimension in last_dimensions:
            if (lastDimension[0] + offset) >= width >= (lastDimension[0] - offset):
                return True

            if (lastDimension[1] + offset) >= height >= (lastDimension[1] - offset):
                return True
        return False

    def is_target_in_center(self, cx, cy) -> bool:
        center_x = 640 / 2
        center_y = 480 / 2
        offset = 20
        if (((center_x - offset) <= cx <= (center_x + offset)) and
                ((center_y - offset) <= cy <= (center_y + offset))):
            return True
        return False


if __name__ == '__main__':
    image_processor = ImageProcessor()
    image_processor.start_detecting(None)
