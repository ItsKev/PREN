import cv2
import time
import numpy as np
from imutils.video.pivideostream import PiVideoStream
from imutils.video import FPS


class ImageProcessor:
    detect_image = True
    slower_already_sent = False

    def start_detecting(self, serial_connection):
        print("ImageProcessor_start_detecting")

        vs = PiVideoStream(resolution=(640, 480)).start()
        time.sleep(2.0)
        fps = FPS().start()

        while True:
            if not self.detect_image:
                break
            image = vs.read()
            gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
            gray = cv2.blur(gray, (5, 5))

            # detect edges in the image
            _ , edged = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
            # edged = cv2.Canny(gray, 100, 200)

            # find contours in the image
            _, contours, _ = cv2.findContours(edged.copy(), cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
            contours = sorted(contours, key=cv2.contourArea, reverse=True)[:9]

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
                            if self.check_if_target_found(centers, cx, cy, serial_connection):
                                break
                            last_dimensions.append((w, h))
                            centers.append(np.array((cx, cy)))
            fps.update()
        fps.stop()
        print('approx. FPS: {:.2f}'.format(fps.fps()))
        vs.stop()
        cv2.destroyAllWindows()
        self.detect_image = True
        self.slower_already_sent = False

    def check_if_target_found(self, centers, cx, cy, serial_connection) -> bool:
        first_match = True
        for center in centers:
            distance = np.linalg.norm(np.array((cx, cy)) - center)
            if distance <= 20:
                if first_match:
                    first_match = False
                else:
                    if self.is_target_in_center(cy):
                        print("ImageProcessor_target_detected")
                        serial_connection.target_found()
                        self.detect_image = False
                        return True
                    # else:
                        # if not self.slower_already_sent:
                           # print("Not in center")
                           # serial_connection.drive_slower()
                           # self.slower_already_sent = True
        return False

    def is_a_duplicate(self, last_dimensions, height, width) -> bool:
        offset = 3
        for lastDimension in last_dimensions:
            if (lastDimension[0] + offset) >= width >= (lastDimension[0] - offset):
                return True

            if (lastDimension[1] + offset) >= height >= (lastDimension[1] - offset):
                return True
        return False

    def is_target_in_center(self, cy) -> bool:
        center_y = 480 / 2
        offset = 20
        if (center_y - offset) <= cy <= (center_y + offset):
            return True
        return False


if __name__ == '__main__':
    image_processor = ImageProcessor()
    image_processor.start_detecting(None)
