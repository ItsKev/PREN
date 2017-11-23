import numpy as np
import cv2
import time

def main():
    cap = cv2.VideoCapture(0)
    cap.set(cv2.CAP_PROP_FPS, 30)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 360)

    while cap.isOpened():
        loop_time = time.time()
        ret, image = cap.read()
        if not ret:
            break
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        gray = cv2.blur(gray, (3, 3))

        # detect edges in the image
        edged = cv2.Canny(gray, 150, 255)

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
                    is_duplicate = is_a_duplicate(last_dimensions, h, w)
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
                                    print("Found")
                                    cv2.drawContours(image, [approx], -1, (0, 255, 0), 4)
                                    cv2.drawContours(image, [matches[0]], -1, (0, 0, 255), 4)
                                    cv2.drawContours(image, [approximations[temp]], -1, (255, 0, 0), 4)
                            temp += 1
                        approximations.append(approx)
                        last_dimensions.append((w, h))
                        centers.append(np.array((cx, cy)))
        print("LoopTime: " + str(time.time() - loop_time))
        #cv2.imshow("Image", image)
        #cv2.waitKey(1)
    cap.release()
    cv2.destroyAllWindows()


def is_a_duplicate(last_dimensions, height, width):
    offset = 3
    for lastDimension in last_dimensions:
        if (lastDimension[0] + offset) >= width >= (lastDimension[0] - offset):
            return True

        if (lastDimension[1] + offset) >= height >= (lastDimension[1] - offset):
            return True
    return False

if __name__ == '__main__':
    main()