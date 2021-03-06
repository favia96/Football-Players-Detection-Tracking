// Federico Favia, Tesi Triennale, Università Trento, 2018
//

#include "stdafx.h"
#include <opencv2/core/utility.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <time.h>  

using namespace cv;
using namespace std;

const double eps = 15;

int random(int min, int max) {
	return rand() % (max - min + 1) + min;
}
bool max(double a, double b) {
	if (a >= b) return true;
	else return false;
}
int main() {
	//variabili per video input
	Mat frame, frame_succ;
	int nFrames;
	int fps = 30; //framerate video input
	int durata_video = 30; //[s] durata video video input (5, 18-11, 29, 30, 34, 35, 62, 125)
	nFrames = fps * durata_video; //numero frames video

	//variabili per file txt da leggere con valori detections
	ifstream det_inFile; //apertura file in lettura
	det_inFile.open("sassuolo_30sec_compare.txt");
	if (det_inFile.fail()) { //error check 
		cout << "Error opening file" << endl;
		return 0;
	}

	//scorro file txt e popolo map 
	map<int, vector<string> > video;
	string line, people, people2, people3;
	int countframe = 0;
	int flag_n_giocatore = 1;
	vector<string> tmp;

	srand(time(0)); //initialize random seed
	int casual_flag_1_giocatore, casual_flag_2_giocatore, casual_flag_3_giocatore;
	casual_flag_1_giocatore = random(1, 13);
	do { 
		casual_flag_2_giocatore = random(1, 13);
	} while (casual_flag_2_giocatore == casual_flag_1_giocatore);
	do {
		casual_flag_3_giocatore = random(1, 13);
	} while (casual_flag_3_giocatore == casual_flag_2_giocatore || casual_flag_3_giocatore == casual_flag_1_giocatore);
	string a = to_string(casual_flag_1_giocatore);
	string b = to_string(casual_flag_2_giocatore);
	string c = to_string(casual_flag_3_giocatore);
	
	getline(det_inFile, line); //leggo prima riga
	while (!det_inFile.eof()) {
		if (line.length() == 0) {
			tmp.clear();
			video.insert(pair<int, vector<string> >(countframe, tmp));
			countframe++;
			getline(det_inFile, line);
		}
		else {
			if (countframe == 3 && flag_n_giocatore == 1 && (casual_flag_1_giocatore == 1 || casual_flag_2_giocatore == 1 || casual_flag_3_giocatore == 1)) {
				if (casual_flag_1_giocatore == 1) {
					people = line; //per prendere 1^ riga del primo frame con detections
				}
				else if (casual_flag_2_giocatore == 1) {
					people2 = line;
				}
				else {
					people3 = line;
				}
			}
			tmp.push_back(line);
			getline(det_inFile, line);
			flag_n_giocatore++;
			if (countframe == 3 && flag_n_giocatore == 2 && (casual_flag_1_giocatore == 2 || casual_flag_2_giocatore == 2 || casual_flag_3_giocatore == 2)) {
				if (casual_flag_1_giocatore == 2) {
					people = line; //per prendere 2^ riga del primo frame con detections
				}
				else if (casual_flag_2_giocatore == 2) {
					people2 = line;
				}
				else {
					people3 = line;
				}
			}
			while (line.length() != 0) {
				tmp.push_back(line);
				getline(det_inFile, line); 
				flag_n_giocatore++;
				if (countframe == 3 && flag_n_giocatore == casual_flag_1_giocatore && people.empty()) {
					people = line; //per prendere 3^ riga del primo frame con detections
				}
				if (countframe == 3 && flag_n_giocatore == casual_flag_2_giocatore && people2.empty()) {
					people2 = line; //per prendere 3^ riga del primo frame con detections
				}
				if (countframe == 3 && flag_n_giocatore == casual_flag_3_giocatore && people3.empty()) {
					people3 = line; //per prendere 3^ riga del primo frame con detections
				}
			}
			video.insert(pair<int, vector<string> >(countframe, tmp));
		}
	}
	
	//iteratori
	map<int, vector<string> >::iterator videoi;
	vector<string>::iterator si;

	//find the first 3 max values of accuracy in the 1st frame detected, so the first 3 lines, and save coordinate x and y to new variables
	int percentage_g1[899], xc_g1[899], yc_g1[899]; // g1: numero di elementi corrisponde al numero dei frame
	//vector<int>  percentage_g1, xc_g1, yc_g1;
	//124 (calcio3, nuovo3), 449 (messireal2), 724 (swissfootball), 849 (dutchfootball), 874 (nuovo4_30sec), 899 (sassuolo_30sec) 1549 (nuovo4_1min), 3124 (nuovo4_2min)
	int percentage_g2[899], xc_g2[899], yc_g2[899];//g2: numero di elementi corrisponde al numero dei frame
	//vector<int>  percentage_g2, xc_g2, yc_g2;
	int percentage_g3[899], xc_g3[899], yc_g3[899];//g3: numero di elementi corrisponde al numero dei frame
	//vector<int> percentage_g3, xc_g3, yc_g3;

	int i;
	for (i = 0; i < 3; i++) { //per i primi 3 frame i valori sono 0 dovuti all'aggiornamento
		percentage_g1[i] = 0; xc_g1[i] = 0; yc_g1[i] = 0;
		//percentage_g1.push_back(0); xc_g1.push_back(0); yc_g1.push_back(0);
		percentage_g2[i] = 0;  xc_g2[i] = 0; yc_g2[i] = 0;
		//percentage_g2.push_back(0);  xc_g2.push_back(0); yc_g2.push_back(0);
		percentage_g3[i] = 0; xc_g3[i] = 0; yc_g3[i] = 0;
		//percentage_g3.push_back(0); xc_g3.push_back(0); yc_g3.push_back(0);
	}

	//giocatore1 (people)
	i = 0; stringstream stream(people);
	while (i <= 2) { //leggo primi 3 numeri della prima stringa -> giocatore più probabile (accuracy, xc, yc)
		int n;
		stream >> n;
		if (!stream) {
			break;
		}
		if (i == 0) {
			percentage_g1[3] = n; //la posizione 3 corrisponde al n° del frame
			//percentage_g1.push_back(n);
		}
		else if (i == 1) {
			xc_g1[3] = n;
			//xc_g1.push_back(n);
		}
		else {
			yc_g1[3] = n;
			//yc_g1.push_back(n);
		}
		i++;
	}
	cout << "G" << casual_flag_1_giocatore << " in ordine di accuracy al 1frame: " << percentage_g1[3] << "% " << xc_g1[3] << " " << yc_g1[3] << endl << endl;
	
	//giocatore2 (people2)
	i = 0; stringstream stream2(people2);
	while (i <= 2) { //leggo primi 3 numeri della prima stringa -> giocatore più probabile (accuracy, xc, yc)
		int n;
		stream2 >> n;
		if (!stream2) {
			break;
		}
		if (i == 0) {
			percentage_g2[3] = n; //la posizione 3 corrisponde al n° del frame
			//percentage_g2.push_back(n);
		}
		else if (i == 1) {
			xc_g2[3] = n;
			//xc_g2.push_back(n);
		}
		else {
			yc_g2[3] = n;
			//yc_g2.push_back(n);
		}
		i++;
	}
	cout << "G" << casual_flag_2_giocatore << " in ordine di accuracy al 1frame: " << percentage_g2[3] << "% " << xc_g2[3] << " " << yc_g2[3] << endl << endl;
	
	
	//giocatore3 (people3)
	i = 0; stringstream stream3(people3);
	while (i <= 2) { //leggo primi 3 numeri della prima stringa -> giocatore più probabile (accuracy, xc, yc)
		int n;
		stream3 >> n;
		if (!stream3) {
			break;
		}
		if (i == 0) {
			percentage_g3[3] = n; //la posizione 3 corrisponde al n° del frame
			//percentage_g3.push_back(n);
		}
		else if (i == 1) {
			xc_g3[3] = n;
			//xc_g3.push_back(n);
		}
		else {
			yc_g3[3] = n;
			//yc_g3.push_back(n);
		}
		i++;
	}
	cout << "G" << casual_flag_3_giocatore << " in ordine di accuracy al 1frame: " << percentage_g3[3] << "% " << xc_g3[3] << " " << yc_g3[3] << endl << endl;

	//salvo cooordinate e percentuali dei giocatori scelti
	//int percentage_frame_i[30], xc_frame_i[30], yc_frame_i[30]; //array delle caratt. di max 30 persone (22 calciatori al max) per ogni frame
	vector<int> percentage_frame_i, xc_frame_i, yc_frame_i;
	int f, k;
	double difference_xc_g1 = 0, difference_yc_g1 = 0;
	double difference_xc_g2 = 0, difference_yc_g2 = 0;
	double difference_xc_g3 = 0, difference_yc_g3 = 0;

	for (videoi = video.begin(); videoi != video.end(); videoi++) {
		if (videoi->first > 3) {
			//k = 0;
			for (si = (videoi->second).begin(); si != (videoi->second).end(); si++) {
				stringstream stream4(*si);
				f = 0;
				while (f <= 2) { //leggo primi 3 numeri
					int g;
					stream4 >> g;
					if (!stream4) {
						break;
					}
					if (f == 0) {
						//percentage_frame_i[k] = g;
						percentage_frame_i.push_back(g);
					}
					else if (f == 1) {
						//xc_frame_i[k] = g;
						xc_frame_i.push_back(g);
					}
					else {
						//yc_frame_i[k] = g;
						yc_frame_i.push_back(g);
					}
					f++;
				} //k++;
			}
			//confronto
			for (k = 0; k < xc_frame_i.size(); k++) {
				difference_xc_g1 = abs( xc_frame_i[k] - (xc_g1[(videoi->first) - 1])) / 2;
				difference_yc_g1 = abs( yc_frame_i[k] - (yc_g1[(videoi->first) - 1])) / 2;

				difference_xc_g2 = abs( xc_frame_i[k] - (xc_g2[(videoi->first) - 1])) / 2;
				difference_yc_g2 = abs( yc_frame_i[k] - (yc_g2[(videoi->first) - 1])) / 2;

				difference_xc_g3 = abs( xc_frame_i[k] - (xc_g3[(videoi->first) - 1])) / 2;
				difference_yc_g3 = abs( yc_frame_i[k] - (yc_g3[(videoi->first) - 1])) / 2;

				if (difference_xc_g1 <= eps && difference_yc_g1 <= eps) {
					percentage_g1[videoi->first] = percentage_frame_i[k];
					//percentage_g1.push_back(percentage_frame_i[k]);
					xc_g1[videoi->first] = xc_frame_i[k];
					//xc_g1.push_back(xc_frame_i[k]);
					yc_g1[videoi->first] = yc_frame_i[k];
					//yc_g1.push_back(yc_frame_i[k]);
				}
				if (difference_xc_g2 <= eps && difference_yc_g2 <= eps) {
					percentage_g2[videoi->first] = percentage_frame_i[k];
					//percentage_g2.push_back(percentage_frame_i[k]);
					xc_g2[videoi->first] = xc_frame_i[k];
					//xc_g2.push_back(xc_frame_i[k]);
					yc_g2[videoi->first] = yc_frame_i[k];
					//yc_g2.push_back(yc_frame_i[k]);
				}
				if (difference_xc_g3 <= eps && difference_yc_g3 <= eps) {
					percentage_g3[videoi->first] = percentage_frame_i[k];
					//percentage_g3.push_back(percentage_frame_i[k]);
					xc_g3[videoi->first] = xc_frame_i[k];
					//xc_g3.push_back(xc_frame_i[k]);
					yc_g3[videoi->first] = yc_frame_i[k];
					//yc_g3.push_back(yc_frame_i[k]);
				}
			}
		}
	}
	
	//open the match video to track the 2 people that at the beginning were the most certain detected
	VideoCapture cap("sassuolo_30sec.mp4");
	if (!cap.isOpened()) {  // error check 
		cout << "Cannot open video" << endl;
		return 0;
	}

	cap >> frame;
	VideoWriter video_output;
	video_output = VideoWriter("results.avi", CV_FOURCC('M', 'J', 'P', 'G'), fps, Size(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT)));

	if (!video_output.isOpened()) { //error check
		cout << "the writer isnt opened" << endl;
		return 0;
	}
	
	
	Mat hist_base;

	//bool playVideo = true;
	int flag2;

	cout << "Live:" << endl << "Premi 'p' per stoppare video se vedi occlusione da risolvere:" << endl;
	i = 0;
	
	for (int i = 0; i < nFrames; i++) { //show video with detections and saving it

		flag2 = 0;
		cap >> frame;

		if (frame.empty()) {
			break;
		}

		circle(frame, (Point(xc_g1[i], yc_g1[i])), 25, Scalar(0, 0, 255), 0, 10); //disegno punto calciatore (frame, coord punto, spessore, colore, pieno, ...)
		string str = to_string(percentage_g1[i]);
		string str_2 = to_string(i);
		str = "G" + a + " F" + str_2 + ": " + str + "%";
		putText(frame, str, Point(xc_g1[i] - 25, (yc_g1[i]) - 35), FONT_HERSHEY_PLAIN, 1.5, Scalar(0, 0, 255, 255));
		if (i >= 3 && percentage_g1[i] <= 65) {
			if (flag2 == 0) {
				cout << "Frame " << i << endl;
				flag2 = 1;
			}
			cout << "G" << casual_flag_1_giocatore << " ERROR: ACCURACY <= 65%" << endl;
			putText(frame, "TOO LOW", Point(xc_g1[i] + 27, (yc_g1[i]) + 2.5), FONT_HERSHEY_PLAIN, 1.5, Scalar(0, 0, 255, 255));
		}

		circle(frame, (Point(xc_g2[i], yc_g2[i])), 25, Scalar(255, 0, 0), 0, 10); //disegno punto calciatore (frame, coord punto, spessore, colore, pieno, ...)
		string str_3 = to_string(percentage_g2[i]);
		str_3 = "G" + b + " F" + str_2 + ": " + str_3 + "%";
		putText(frame, str_3, Point(xc_g2[i] - 25, (yc_g2[i]) - 35), FONT_HERSHEY_PLAIN, 1.5, Scalar(255, 0, 0, 255));
		if (i >= 3 && percentage_g2[i] <= 65) {
			if (flag2 != 1) {
				cout << "Frame " << i << endl;
				flag2 = 0;
			}
			cout << "G" << casual_flag_2_giocatore << " ERROR: ACCURACY <= 65%" << endl;
			putText(frame, "TOO LOW", Point(xc_g2[i] + 27, (yc_g2[i]) + 2.5), FONT_HERSHEY_PLAIN, 1.5, Scalar(255, 0, 0, 255));
		}


		circle(frame, (Point(xc_g3[i], yc_g3[i])), 25, Scalar(255, 255, 0), 0, 10); //disegno punto calciatore (frame, coord punto, spessore, colore, pieno, ...)
		string str_4 = to_string(percentage_g3[i]);
		str_4 = "G" + c + " F" + str_2 + ": " + str_4 + "%";
		putText(frame, str_4, Point(xc_g3[i] - 25, (yc_g3[i]) - 35), FONT_HERSHEY_PLAIN, 1.5, Scalar(255, 255, 0, 255));
		if (i >= 3 && percentage_g3[i] <= 65) {
			if (flag2 != 1) {
				cout << "Frame " << i << endl;
				flag2 = 0;
			}
			cout << "G" << casual_flag_3_giocatore << " ERROR: ACCURACY <= 65%" << endl;
			putText(frame, "TOO LOW", Point(xc_g3[i] + 27, (yc_g3[i]) + 2.5), FONT_HERSHEY_PLAIN, 1.5, Scalar(255, 255, 0, 255));
		}

		namedWindow("Video with tracking of first 2 players detected with most accuracy", WINDOW_AUTOSIZE);
		imshow("Video with tracking of first 2 players detected with most accuracy", frame); //display the resulting frame

		video_output.write(frame); //saving the video modified

		waitKey(30);
	}
	
	char m;
	cout << endl << "Premi 'j' per stampare e salvare i risultati: ";
	cin >> m;
	
	if (m == 'j') {
		//writing txt for graphs
		cout << endl;
		string namefile1 = "percentage_g" + a + ".csv";
		ofstream g1(namefile1); //apertura file in scrittura
		if (g1.fail()) { //error check 
			cout << "Error opening file" << endl;
			return 0;
		}

		string namefile2 = "percentage_g" + b + ".csv";
		ofstream g2(namefile2); //apertura file in scrittura
		if (g2.fail()) { //error check 
			cout << "Error opening file" << endl;
			return 0;
		}
	
		string namefile3 = "percentage_g" + c + ".csv";
		ofstream g3(namefile3); //apertura file in scrittura
		if (g3.fail()) { //error check
			cout << "Error opening file" << endl;
			return 0;
		}

		g1 << "N° Frame" << ";" << "Accuracy g" << casual_flag_1_giocatore << endl;
		g2 << "N° Frame" << ";" << "Accuracy g" << casual_flag_2_giocatore << endl;
		g3 << "N° Frame" << ";" << "Accuracy g" << casual_flag_3_giocatore << endl;

		for (i = 0; i < (nFrames - 1); i++) {
			cout << "G" << casual_flag_1_giocatore << " - F" << i << " - Accuracy = " << percentage_g1[i] << "%" << endl;
			g1 << i << ";" << percentage_g1[i] << endl;
		}
		cout << endl;

		for (i = 0; i < (nFrames - 1); i++) {
			cout << "G" << casual_flag_2_giocatore << " - F" << i << " - Accuracy = " << percentage_g2[i] << "%" << endl;
			g2 << i << ";" << percentage_g2[i] << endl;
		}
		cout << endl;

		for (i = 0; i < (nFrames - 1); i++) {
			cout << "G" << casual_flag_3_giocatore << " - F" << i << " - Accuracy = " << percentage_g3[i] << "%" << endl;
			g3 << i << ";" << percentage_g3[i] << endl;
		}
		
		g1.close(); //chiusura file
		g2.close(); //chiusura file
		g3.close(); //chiusura file
	}
	
	det_inFile.close(); //chiusura file
	
	waitKey();
	return 0;
}
