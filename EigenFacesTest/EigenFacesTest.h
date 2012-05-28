/*
   File name: EigenFaceTest.h
   Date:      2011-07-14 13:27
   Author:    Kristján Rúnarsson
*/

#ifndef __EIGENFACETEST_H__
#define __EIGENFACETEST_H__

void msgTestHeader(){
	cout << endl;
	cout << "Identification results for each subject test image:" << endl;
	cout << "---------------------------------------------------" << endl;

}

void msgTestResult(string subjectFaceImage, int recAs, int currentTestSubjectID, bool isCorrectlyRec)
{
	cout  << "File: " << subjectFaceImage << ", recognized ID: " << recAs << ", actual ID: " << currentTestSubjectID;

	if(isCorrectlyRec)
	{
		cout << " --> true" << endl;
	}
	else
	{
		cout << " --> false" << endl;
	}
}

void msgSummary(int numCorrect, int numIncorrect, int numTestFiles)
{
	cout << endl;
	cout << "Summary" << endl;
	cout << "-------" << endl;
	cout << "True:    " << numCorrect << endl;
	cout << "False:  " << numIncorrect << endl;
	cout << "Correct: " << ((float)numCorrect/(float)numTestFiles)*100.0 << "%" << endl;
}

void msgSubjBySubj(vector<int> &corrPerId)
{
	cout << endl;
	cout << "Total correct identifications for each subject:" << endl;
	cout << "-----------------------------------------------" << endl;

	for(int i=0; i<40; i++)
	{
		cout << "Correct for subject s" << i+1 << ": " << corrPerId[i] << endl;
	}
}

#endif
