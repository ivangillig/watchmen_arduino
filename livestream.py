import cv2
import urllib.request
from cv2 import ROTATE_90_CLOCKWISE
import numpy as np


import serial,time
arduino=serial.Serial('com3',115200,timeout=0.1)
 
f_cas= cv2.CascadeClassifier(cv2.data.haarcascades+'haarcascade_frontalface_default.xml')
eye_cascade=cv2.CascadeClassifier(cv2.data.haarcascades +'haarcascade_eye.xml')
url='http://192.168.0.190/640x480.jpg'
##'''cam.bmp / cam-lo.jpg /cam-hi.jpg / cam.mjpeg '''
cv2.namedWindow("Live Transmission", cv2.WINDOW_AUTOSIZE)

def maprange(a, b, s):
    (a1, a2), (b1, b2) = a, b
    return  b1 + ((s - a1) * (b2 - b1) / (a2 - a1))
 
while True:
    img_resp=urllib.request.urlopen(url)
    imgnp=np.array(bytearray(img_resp.read()),dtype=np.uint8)
    img=cv2.imdecode(imgnp,-1)
    gray=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    face=f_cas.detectMultiScale(gray,scaleFactor=1.1,minNeighbors=5)
    
    # Code para rotar 90°
    #img = cv2.rotate(img, rotateCode=ROTATE_90_CLOCKWISE)

    for x,y,w,h in face:
        cv2.rectangle(img,(x,y),(x+w,y+h),(0,0,255),3)
        roi_gray = gray[y:y+h, x:x+w]
        roi_color = img[y:y+h, x:x+w]

        img_h, img_w, img_c = img.shape
        
        x = round(maprange([0,img_w],[40,0], (x+w//2))) #0 es a la izquierda
        y = round(maprange([0,img_h],[40,0], (y+h//2))) #0 es abajo

        string='{0:d},{1:d}'.format(x,y)
        arduino.write(bytes(string, 'utf-8'))
        #print(string.encode('utf-8'))

        #for testing purpose
        data = str(arduino.readline())
        time.sleep(0.05)
        print('data from arduino:'+data)
        
    img = cv2.flip(img, 1)

    cv2.namedWindow("live transmission", cv2.WINDOW_NORMAL)
    cv2.imshow("live transmission",img)
    key=cv2.waitKey(5)
    if key==ord('q'):
        arduino.close()
        break

 
cv2.destroyAllWindows()