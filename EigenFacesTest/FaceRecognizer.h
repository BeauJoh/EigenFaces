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
