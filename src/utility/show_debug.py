import cv2

def show_debug(img, title="title"):
    shape = img.shape
    height = 800
    width = round((800 / shape[0]) * shape[1])
    cv2.namedWindow(title, cv2.WINDOW_AUTOSIZE)
    cv2.imshow(title, cv2.resize(img, (width, height)))
    keyCode = cv2.waitKey(0)
    cv2.destroyAllWindows()
    if (keyCode & 0xFF) == ord("q"):
        exit()
    # wait_time = 1000
    # while cv2.getWindowProperty(title, cv2.WND_PROP_VISIBLE) >= 1:
    #     keyCode = cv2.waitKey(wait_time)
    #     if (keyCode & 0xFF) == ord("q"):
    #         cv2.destroyAllWindows()
    #         break