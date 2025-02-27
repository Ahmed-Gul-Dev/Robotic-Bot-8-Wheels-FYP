import numpy as np
import cv2
import firebase_admin
from firebase_admin import credentials, initialize_app, storage
import urllib.request
from PIL import Image
import time

print('Sending Images to Firebase 2024')
cred_obj = firebase_admin.credentials.Certificate('feeder-73a76-firebase-adminsdk.json')
initialize_app(cred_obj, {'storageBucket': 'feeder-73a76.appspot.com'})
max_time = 8000
start_time = round(time.time()*1000)
# print(start_time )

# define a video capture object
# cap = cv2.VideoCapture(0)
destination_file_name = r"Downloaded.png"
while True:
    # _, frame = cap.read()
    # if (round(time.time()*1000) - start_time > max_time):
    #     start_time = round(time.time()*1000)
    #     cv2.imwrite('Sample.png', frame)
    #     fileName = "Sample.png"
    #     bucket = storage.bucket()
    #     blob = bucket.blob(fileName)
    #     blob.upload_from_filename(fileName)
    #     print('Uploaded. Success!')

    # cv2.imshow("Output", frame)
    # if cv2.waitKey(1) & 0xFF == ord('q'):
    #     break
    if (round(time.time() * 1000) - start_time > max_time):
        start_time = round(time.time() * 1000)
        bucket = storage.bucket()
        blob = bucket.blob('plants.png')
        blob.download_to_filename(destination_file_name)
        # time.sleep(1)
        img = cv2.imread('Downloaded.png')
        cv2.imshow("Out", img)
        cv2.waitKey(0)

# cap.release()
cv2.destroyAllWindows()



# # Upload Image to Firebase
# fileName = "sample.PNG"
# bucket = storage.bucket()
# blob = bucket.blob(fileName)
# blob.upload_from_filename(fileName)

# Opt : if you want to make public access from the URL
# blob.make_public()
# print("your file url", blob.public_url)
#
#
# # Download Image from Firebase
# urllib.request.urlretrieve(blob.public_url, "downloaded.jpg")
# # Opening the image and displaying it (to confirm its presence)
# img = Image.open(r"downloaded.jpg")
# img.show()




# # import the opencv library
# import cv2
#
# # define a video capture object
# cap = cv2.VideoCapture(0)
#
# while True:
#     _, frame = cap.read()
#
#     cv2.imshow("Output", frame)
#     if cv2.waitKey(1) & 0xFF == ord('q'):
#         break
#
# cap.release()
# cv2.destroyAllWindows()
