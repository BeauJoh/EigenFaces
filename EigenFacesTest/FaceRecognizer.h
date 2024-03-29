/*
 *  FaceRecognizer.cpp
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

#ifndef __FACERECOGNIZER_H__
#define __FACERECOGNIZER_H__

#include <iostream>
#include <vector>
#include <float.h>
#include <OpenCV/cv.h>

using namespace std;
using namespace cv;
class FaceRecognizer {

public:
	FaceRecognizer(const Mat& trFaces, const vector<int>& trImageIDToSubjectIDMap, int components);
	~FaceRecognizer();
	void init(const Mat& trFaces, const vector<int>& trImageIDToSubjectIDMap, int components);
	int recognize(const Mat& instance);
	Mat getAverage();
	Mat getEigenvectors();
	Mat getEigenvalues();

private:
	PCA *pca;
	vector<Mat> projTrFaces;
	vector<int> trImageIDToSubjectIDMap;
};

#endif
