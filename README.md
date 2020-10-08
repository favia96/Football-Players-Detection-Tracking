## Football Players Detection and  Tracking
# Federico Favia - BSc Thesis in Information and Communications Engineering, University of Trento, October 2018

Convolutional Neural Networks' use for detection and tracking of players in sports events" ("Impiego di reti neurali convoluzionali per il rilevamento e tracciamento di giocatori in contesto sportivo"), developed within the student internship at [Xtensa Srl](https://xtensa.it/).
- Use and modification of [YoloV3 by Redmon et Al.](https://arxiv.org/pdf/1804.02767.pdf), state-of-the-art real-time object detection neural network, to recognize players in football games videos (see the folder yolo_modifications). 
Inspiration for the work were [Yolo](https://pjreddie.com/darknet/yolo/) and the 2 repositories https://pjreddie.com/darknet/yolo/ and https://github.com/AlexeyAB/darknet
- The Darknet files are in C, the network was not retrained but modified in order to show just the detection predictions for persons (so referee included), in output a txt files is used for the coordinates predictions and confidence for every frame.
- Then software development of tracker for the same players through C++ and OpenCV library (see tirocinio.cpp) and evaluation of tracking's consistency through Matlab (see graphs.m). 
- Finally, comparison of this approach with respect to classical algorithms, such as Mean shift.

![Yolo](https://github.com/favia96/Football-Players-Detection-Tracking/blob/master/images_and_results/leftcam_det.JPG)
![Tracking](https://github.com/favia96/Football-Players-Detection-Tracking/blob/master/images_and_results/cplusplus.JPG)
