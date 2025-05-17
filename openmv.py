import sensor
from machine import UART
from pyb import LED
import time
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)
sensor.set_hmirror(True)
sensor.set_vflip(True)
uart = UART(3, 250000)
red_blob = (36, 11, 4, 37, -22, 32)
black_blob = (0, 43, -26, -1, -10, 18)

red_roi_mid = (5,134,315,47)
red_left_roi = (0,0,5,240)
red_right_roi = (315,0,5,240)

black_roi_mid = (146,1,28,129)


'''
发送0:
发送1:只扫到中间色块
发送2:什么都没扫到->白色
发送3:两边都扫到红色
发送4:扫到黑色
发送5:只扫到左边红色
发送6:只扫到右边红色
'''

def Img_Init():
    led_1 = LED(1)
    led_2 = LED(2)
    led_3 = LED(3)
    led_1.on()
    led_2.on()
    led_3.on()

    img = sensor.snapshot()
    img.draw_rectangle(red_left_roi,color=(255, 0, 0))
    img.draw_rectangle(red_right_roi,color=(255, 0, 0))
    return img

def Find_Black(img):
    blobs_black = img.find_blobs([black_blob], roi=black_roi_mid,merge=True,area_threshold=900,pixels_threshold=80)
    if blobs_black:
        for blob in blobs_black:
            img.draw_rectangle((blob.x(), blob.y(), blob.w(), blob.h()), color=(0, 0, 0))
            img.draw_cross(blob.cx(), blob.cy(), color=(255, 255, 255))
            uart.write("s4,168,97,e")
            print(4)
        return True
    else:

        return False

def Find_Red_In_Mid(img):
    blobs = img.find_blobs([red_blob], roi=red_roi_mid, merge=True)
    if blobs:
        for blob in blobs:
            img.draw_rectangle((blob.x(), blob.y(), blob.w(), blob.h()), color=(255, 0, 0))
            img.draw_cross(blob.cx(), blob.cy(), color=(0, 255, 0))
            uart.write(f"s1,{blob.cx()},{blob.cy()},e")
            print(1)
            return True
    else:
        return False

def Find_Red_In_Edge(img):
    left_blobs_Flag = 0
    right_blobs_Flag = 0
    Left_blobs = img.find_blobs([red_blob], roi=red_left_roi, merge=True)
    right_blobs = img.find_blobs([red_blob], roi=red_right_roi, merge=True)
    if Left_blobs:
        left_blobs_Flag = 1
    if right_blobs:
        right_blobs_Flag = 1
    if left_blobs_Flag and right_blobs_Flag:
        uart.write("s3,168,97,e")
        print(3)
        return 1
    elif left_blobs_Flag:

        for blob in Left_blobs:
            img.draw_rectangle((blob.x(), blob.y(), blob.w(), blob.h()), color=(255, 0, 0))
            img.draw_cross(blob.cx(), blob.cy(), color=(0, 255, 0))
            uart.write("s5,168,97,e")
            print(5)
        return 2
    elif right_blobs_Flag:
        for blob in right_blobs:
            img.draw_rectangle((blob.x(), blob.y(), blob.w(), blob.h()), color=(255, 0, 0))
            img.draw_cross(blob.cx(), blob.cy(), color=(0, 255, 0))
            uart.write("s6,168,97,e")
            print(6)
        return 3
    else:
        return False

if __name__ == "__main__":
    while True:
        img = Img_Init()
        if Find_Black(img) is False:# 如果没找到黑色色块,就在边沿找红色色块
            if Find_Red_In_Edge(img) is False:#如果没在边沿找到红色,就在中间找红色色块
                if Find_Red_In_Mid(img) is False:
                    uart.write("s2,0,e")
                    print(2)


