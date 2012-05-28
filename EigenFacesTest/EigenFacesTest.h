/*
 *  EigenFacesTest.h
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
