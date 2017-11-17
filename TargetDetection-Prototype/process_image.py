import numpy as np
import cv2
import time

start_time = time.time()

image = cv2.imread('target.jpg')
print("--- Image read: %s seconds ---" % (time.time() - start_time))

temp_time = time.time()
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
print("--- Image gray: %s seconds ---" % (time.time() - temp_time))

temp_time = time.time()
gray = cv2.blur(gray, (3, 3))
cv2.imshow("Gray", gray)
print("--- Image blurred: %s seconds ---" % (time.time() - temp_time))

temp_time = time.time()
# detect edges in the image
edged = cv2.Canny(gray, 150, 255)
# cv2.imwrite("/home/pi/edged.jpg", edged)
print("--- Image cannied: %s seconds ---" % (time.time() - temp_time))

temp_time = time.time()
# find contours (i.e. the 'outlines') in the image
new_image, contours, hierarchy = cv2.findContours(edged.copy(), cv2.RETR_CCOMP, cv2.CHAIN_APPROX_SIMPLE)

print("--- Image found contours: %s seconds ---" % (time.time() - temp_time))
centers = []
approximations = []
lastDimensions = []
matches = []
image_counter = 0
found = False
for contour in contours:
    # approximate the contour
    peri = cv2.arcLength(contour, True)
    approx = cv2.approxPolyDP(contour, 0.02 * peri, True)

    # if the approximated contour has four points its a rectangle
    if len(approx) == 4:
        (x, y, w, h) = cv2.boundingRect(approx)
        if h == 0:
            h = 0.01
        aspect_Ratio = w / float(h)
        if 0.85 <= aspect_Ratio <= 1.15:
            M = cv2.moments(approx)
            m00 = M['m00']
            if m00 == 0:
                m00 = 0.05
            cx = int(M['m10'] / m00)
            cy = int(M['m01'] / m00)
            found = False
            for lastDimension in lastDimensions:
                if np.array_equal((x, y), lastDimension):
                    lastDimensions.append((x, y))
                    print("Found duplicate")
                    found = True
                    break
            if not found:
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
                            image_clone = image.copy()
                            print("--- Found match: %s seconds ---" % (time.time() - start_time))
                            cv2.drawContours(image_clone, [approx], -1, (0, 255, 0), 4)
                            cv2.drawContours(image_clone, [matches[0]], -1, (0, 0, 255), 4)
                            cv2.drawContours(image_clone, [approximations[temp]], -1, (255, 0, 0), 4)
                            cv2.imwrite("/home/pi/output" + str(image_counter) + ".jpg", image_clone)
                            cv2.imshow("Contour", image_clone)
                            image_counter += 1
                    temp += 1
                approximations.append(approx)
                lastDimensions.append((x, y))
                centers.append(np.array((cx, cy)))

print("--- %s seconds ---" % (time.time() - start_time))

# display the output
# cv2.imwrite("/home/pi/output.jpg", image)
cv2.waitKey(0)
cv2.destroyAllWindows()
