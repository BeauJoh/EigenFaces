/*
 *  FaceRecognizer.h
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

#include "FaceRecognizer.h"

FaceRecognizer::FaceRecognizer(const Mat& trFaces, const vector<int>& trImageIDToSubjectIDMap, int components)
{
	init(trFaces, trImageIDToSubjectIDMap, components);
}

void FaceRecognizer::init(const Mat& trFaces, const vector<int>& trImageIDToSubjectIDMap, int components)
{
	int numSamples = trFaces.cols;
	this->pca = new PCA(trFaces, Mat(),CV_PCA_DATA_AS_COL, components);

	for(int sampleIdx = 0; sampleIdx < numSamples; sampleIdx++) {
		this->projTrFaces.push_back(pca->project(trFaces.col(sampleIdx)));
	}

	this->trImageIDToSubjectIDMap = trImageIDToSubjectIDMap;
}

FaceRecognizer::~FaceRecognizer()
{
	delete pca;
}

int FaceRecognizer::recognize(const Mat& unknown) 
{
	// Take the vector representation of unknown's face image and project it
	// into face space.
	Mat unkProjected = pca->project(unknown);

	// I now want to know which individual in my training data set has the shortest distance
	// to unknown.
	double closestFaceDist = DBL_MAX;

	int closestFaceID = -1;
	for(unsigned int i = 0; i < projTrFaces.size(); i++) {
		// Given two vectors and the NORM_L2 type ID the norm function computes the Euclidean distance:
		//     dist(SRC1,SRC2) = SQRT(SUM(SRC1(I) - SRC2(I))^2)
		// between the projections of the current training face and unknown.
		Mat src1 = projTrFaces[i];
		Mat src2 = unkProjected;
		double dist = norm(src1, src2, NORM_L2);

		// Every time I find somebody with a shorter distance I save the distance, map his
		// index number to his actual ID in the set of traing images and save the actual ID.
		if(dist < closestFaceDist) {
			closestFaceDist = dist;
			closestFaceID = this->trImageIDToSubjectIDMap[i];
		}
	}
	
	return closestFaceID;
}

// Returns the vector containing the average face.
Mat FaceRecognizer::getAverage()
{
	return pca->mean;
}


// Returns a matrix containing the eigenfaces.
Mat FaceRecognizer::getEigenvectors()
{
	return pca->eigenvectors;
}

// Returns a matrix containing the eigenvalues.
Mat FaceRecognizer::getEigenvalues()
{
	return pca->eigenvalues;
}
