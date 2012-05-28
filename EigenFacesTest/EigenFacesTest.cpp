/*
 *  EigenFacesTest.cpp
 *  EigenFacesTest
 *
 *
 *  Created by Kristján Rúnarsson on the 14/07/11
 *  Modified and Ported to XCode by Beau Johnston on the 28/05/12. 
 *  Copyright (C) 2012 by Beau Johnston.
 *
 *  Please email me if you have any comments, suggestions or advice:
 *                              beau@inbeta.org
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */



#include <OpenCV/cv.h>
#include <OpenCV/highgui.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "FaceRecognizer.h"
#include "EigenFacesTest.h"

using namespace std;
using namespace cv;

// Scale every value in a matrix so that it is in the 0-255 range. Thanks to
// the guys at stackoverflow for this one:
//
//     http://stackoverflow.com/questions/4974709/pca-svm-using-c-syntax-in-opencv-2-2
//
Mat toGrayscale(const Mat& mat) {
	Mat gMat(mat.rows, mat.cols, CV_8UC1);

	double min, max;
	minMaxLoc(mat, &min, &max);

	for(int row = 0; row < mat.rows; row++) {
		for(int col = 0; col < mat.cols; col++) {
			gMat.at<uchar>(row, col) = 255 * ((mat.at<float>(row, col) - min) / (max - min));
		}
	}

	return gMat;
}

// Test lists consist of the subject ID of the person of whom a face image is and the
// path to that image.
void readFile(const string& fileName, vector<string>& files, vector<int>& indexNumToSubjectIDMap)
{
	std::ifstream file(fileName.c_str(), ifstream::in);

	if(!file) {
		cerr << "Unable to open file: " << fileName << endl;
		exit(0);
	}

	std::string line, path, trueSubjectID;
	while (std::getline(file, line)) {
		std::stringstream liness(line);
		std::getline(liness, trueSubjectID, ';');
		std::getline(liness, path);

		path.erase(std::remove(path.begin(), path.end(), '\r'), path.end());
		path.erase(std::remove(path.begin(), path.end(), '\n'), path.end());
		path.erase(std::remove(path.begin(), path.end(), ' '), path.end());

		files.push_back(path);
		indexNumToSubjectIDMap.push_back(atoi(trueSubjectID.c_str()));
	}
}

void usage()
{
	cout << "eigenfacetest [-vs] [-t list of training images] [-r list of test images]" << endl;
	cout << " -v: Verbose mode." << endl;
	cout << " -s: The projected face vectors can be retrieved and rendered" << endl;
	cout << "     as images. This option shows the average face and a " << endl;
	cout << "     few of the eigenfaces." << endl;
	cout << " -r: A list of test files with faces to try and recognize." << endl ;
	cout << " -t: A List of training files with faces to... well... train on." << endl;
	cout << " -h: This text." << endl;
	exit(1);
}

void unableToLoad()
{
	cerr << "Unable to load face images. The program expects a set of face images in a subdirectory" << endl;
	cerr << "of the execution directory named 'att_faces'. This face database can be freely downloaded " << endl; 
	cerr << "from the Cambridge University Copmputer Lab's website:" << endl;
	cerr << "     http://www.cl.cam.ac.uk/research/dtg/attarchive/facedatabase.html" << endl;
	exit(1);
}

int main(int argc, char *argv[]) {
	vector<string> teFaceFiles;
	vector<int> teImageIDToSubjectIDMap;
	vector<string> trFaceFiles;
	vector<int> trImageIDToSubjectIDMap;

	string trainingList = "./train_all_1-9.txt";
	string testList = "./test_all_1-9.txt";

    //doShow is originally set to false
	bool isVerbose=false, doShow=true, doWrite=true;
	char c;
	while ( (c = getopt(argc, argv, ":ast:r:v")) != -1) {
		switch (c) {
			case 's':
				doShow = true;
				break;
			case 'v':
				isVerbose = true;
				break;
			case 'r':
				testList = optarg;
				break;
			case 't':
				trainingList = optarg;
				break;
			case 'h':
				usage();
				break;
			case '?':
				cout << "Invalid option: '-" << (char)optopt << "'" << endl;
				usage();
				break;
			case ':':
				cout << "Missing argument value for: '-" << (char)optopt << "'" << endl;
				usage();
				break;
			default:
				cerr << "getopt returned character code 0%o" << c << endl;
				usage();
		}
	}



	readFile(testList, teFaceFiles, teImageIDToSubjectIDMap);
	readFile(trainingList, trFaceFiles, trImageIDToSubjectIDMap);

	Mat teImg = imread(trFaceFiles[0], 0);
	Mat trImg = imread(trFaceFiles[0], 0);

	if(teImg.empty() || trImg.empty())
	{
		unableToLoad();
	}

	
	// Eigenfaces operates on a vector representation of the image so we calculate the
	// size of this vector.  Now we read the face images and reshape them into vectors 
	// for the face recognizer to operate on.
	int imgVectorSize = teImg.cols * teImg.rows; 

	// Create a matrix that has 'imgVectorSize' rows and as many columns as there are images.
	Mat trImgVectors(imgVectorSize, trFaceFiles.size(), CV_32FC1);

	// Load the vector.
	for(unsigned int i = 0; i < trFaceFiles.size(); i++)
	{
		Mat tmpTrImgVector = trImgVectors.col(i);
		Mat tmp_img;
		imread(trFaceFiles[i], 0).convertTo(tmp_img, CV_32FC1);
		tmp_img.reshape(1, imgVectorSize).copyTo(tmpTrImgVector);
	}

	// On instantiating the FaceRecognizer object automatically processes
	// the training images and projects them.
	FaceRecognizer fr(trImgVectors, trImageIDToSubjectIDMap, 20);

	if(isVerbose){
		msgTestHeader();
	}

	// This vector keeps track of how many times each face is correctly recognized
	// The index numbers stand for the ID number of a subject in the test set.
	vector<int> corrPerId;
	for(int i=0; i<40; i++)
	{
		corrPerId.push_back(0);
	}

	int correct=0, incorrect=0;
	for(unsigned int i=0; i<teFaceFiles.size(); i++)
	{
		Mat rmat = imread(teFaceFiles[i],0);
		
		rmat.convertTo(teImg, CV_32FC1);
		
		// Look up the true subject ID of the current test subject.
		int currentTestSubjectID = teImageIDToSubjectIDMap[i];

		// Compare a face from the list of test faces with the set of training faces stored in
		// the recognizer and try to find the closest match.
		int recAs = fr.recognize(rmat.reshape(1, imgVectorSize));

		// The recognizer returns the subject ID of the subject he thinks this is. If a subject is correctly 
		// recognized the subject ID returned by the recognizer should be the same as the subject ID from the 
		// test file so we must look up the ID from the test file.
		bool isCorrectlyRec;
		if(recAs == currentTestSubjectID)
		{
			isCorrectlyRec = true;
			correct++; // Track total correct recognitions for all subjects.
			corrPerId[recAs-1]++; // Track total correct recognitions for this subject
		} 
		else
		{
			isCorrectlyRec = false;
			incorrect++; // Total false recognitions for all subjects.
		}

		
		if(isVerbose) {
			msgTestResult(teFaceFiles[i], recAs, currentTestSubjectID, isCorrectlyRec);
		}
		
	}

	if(isVerbose){
		msgSubjBySubj(corrPerId);
	}

	msgSummary(correct, incorrect, teFaceFiles.size());

	// Just for fun let's show the average face and some of the test faces on screen.
	if(doShow)
	{
		Mat oAvgImage = toGrayscale(fr.getAverage()).reshape(1, teImg.rows);
		Mat sAvgImage;
		resize(oAvgImage, sAvgImage, Size(teImg.cols*2, teImg.rows*2), 0, 0, INTER_LINEAR);

		imshow("The average face", sAvgImage);

		int eigenCount=6;
		if(fr.getEigenvectors().rows < eigenCount){
			eigenCount = fr.getEigenvectors().rows;
		}

		for(int i = 0; i < eigenCount; i++) {
			stringstream windowTitle;
			windowTitle << "Eigenface No. " << i;

			Mat oEigenImage = toGrayscale(fr.getEigenvectors().row(i)).reshape(1, teImg.rows);
			Mat sEigenImage;
			resize(oEigenImage, sEigenImage, Size(teImg.cols*2, teImg.rows*2), 0, 0, INTER_LINEAR);

            imshow(windowTitle.str(), sEigenImage);
            
            if (doWrite) {
                char filename[250];
                
            
                strcpy( filename, (const char*)( windowTitle.str().c_str()));
                       
                windowTitle << ".bmp";
                imwrite(windowTitle.str(), sEigenImage);
			}
            
		}

		waitKey(0);
	}
}
