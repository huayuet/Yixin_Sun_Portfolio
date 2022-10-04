#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>
using namespace std;

vector<vector<double> > sigmoid(vector<vector<double> > input) {
  vector<vector<double> > output(input.size());
  
  for (int i = 0; i < input.size(); i++) {
    output.at(i).resize(1);
    output.at(i).at(0) = 1.0 / (1.0 + exp(-1 * input.at(i).at(0)));
  }
  
  return output;
}

vector<vector<double> > vectorSub(vector<vector<double> > subtrahend, vector<vector<double> > subtractor) {
  vector<vector<double> > diff(subtrahend.size());
  if (subtrahend.size() == subtractor.size()) {
    for (int i = 0; i < subtrahend.size(); i++) {
      diff.at(i).resize(1);
      diff[i][0] = subtrahend[i][0] - subtractor[i][0];
    }
  }
  return diff;
}

vector<vector<double> > vectorAdd(vector<vector<double> > first, vector<vector<double> > second) {
  vector<vector<double> > sum(first.size());
  for (int i = 0; i < first.size(); i++) {
    sum.at(i).resize(first.at(i).size());
    for (int j = 0; j < first.at(i).size(); j++) {
      sum[i][j] = first[i][j] + second[i][j];
    }
  }
  return sum;
}

vector<vector<double> > scalarMult(vector<vector<double> > matrix, double scalar) {
  vector<vector<double> > output(matrix.size());
  for (int i = 0; i < matrix.size(); i++) {
    output.at(i).resize(matrix.at(i).size());
    for (int j = 0; j < matrix.at(i).size(); j++) {
      output[i][j] = matrix[i][j] * scalar;
    }
  }
  return output;
}

vector<vector<double> > matrixMult(vector<vector<double> > first, vector<vector<double> > second) {
  vector<vector<double> > product(first.size());

  for (int i = 0; i < first.size(); i++) {
    product.at(i).resize(second.at(0).size());
    for (int j = 0; j < second.at(0).size(); j++) {
      product[i][j] = 0;
      for (int k = 0; k < first.at(0).size(); k++) {
        product[i][j] += first[i][k] * second[k][j];
      }
    }
  }
  
  return product;
}

vector<vector<double> > matrixTrans(vector<vector<double> > input) {
  vector<vector<double> > transpose(input.at(0).size());
  
  for (int i = 0; i < transpose.size(); i++) {
    transpose.at(i).resize(input.size());
    for (int j = 0; j < transpose.at(i).size(); j++) {
      transpose[i][j] = input[j][i];
    }
  }
  
  return transpose;
}

int main(int argc, char** argv) {
  ifstream inFS;
  string line;
  string pclass_in, survived_in, sex_in, age_in;
  int MAX_LEN = 800;
  int TEST_LEN = 246;
  
  vector<double> pclass(MAX_LEN);
  vector<double> survived(MAX_LEN);
  vector<double> sex(MAX_LEN);
  vector<double> age(MAX_LEN);
  
  vector<double> pclass_test(TEST_LEN);
  vector<double> survived_test(TEST_LEN);
  vector<double> sex_test(TEST_LEN);
  vector<double> age_test(TEST_LEN);
  
  cout << "Opening file titanic_project.csv" << endl;
  
  inFS.open("titanic_project.csv");
  if (!inFS.is_open()) {
    cout << "Could not open file titanic_project.csv" << endl;
    return -1;
  }
  
  cout << "Reading line 1" << endl;
  getline(inFS, line);
  cout << "Heading: " << line << endl;
  
  int numObservations = 0;
  while (inFS.good()) {
    getline(inFS, pclass_in, ','); //unused column
    getline(inFS, pclass_in, ',');
    getline(inFS, survived_in, ',');
    getline(inFS, sex_in, ',');
    getline(inFS, age_in, '\n');
    
    if (numObservations < 800) {
      pclass[numObservations] = stof(pclass_in);
      survived[numObservations] = stof(survived_in);
      sex[numObservations] = stof(sex_in);
      age[numObservations] = stof(age_in);
    } else {
      pclass_test[numObservations - 800] = stof(pclass_in);
      survived_test[numObservations - 800] = stof(survived_in);
      sex_test[numObservations - 800] = stof(sex_in);
      age_test[numObservations - 800] = stof(age_in);
    }
    
    numObservations++;
  }
  
  cout << "Closing file titanic_project.csv" << endl;
  inFS.close();
  
  vector<vector<double> > weights(2);
  weights.at(0).resize(1); weights.at(1).resize(1);
  weights.at(0).at(0) = 1; weights.at(1).at(0) = 1;
  
  auto start = chrono::system_clock::now();
  
  vector<vector<double> > dataMatrix(800);
  vector<vector<double> > labels(800);
  for (int i = 0; i < 800; i++) {
    dataMatrix.at(i).resize(2);
    dataMatrix[i][0] = 1; dataMatrix[i][1] = sex[i];
    
    labels.at(i).resize(1);
    labels.at(i).at(0) = survived.at(i);
  }

  double learnRate = 0.001;
  
  for (int i = 0; i < 50000; i++) {
    vector<vector<double> > prevWeights = weights;
    vector<vector<double> > weightedMatrix = matrixMult(dataMatrix, weights);
    vector<vector<double> > probVector = sigmoid(weightedMatrix);
    vector<vector<double> > error = vectorSub(labels, probVector);
    weights = vectorAdd(weights, matrixMult(scalarMult(matrixTrans(dataMatrix), learnRate), error));
    if (prevWeights[0][0] == weights [0][0] && prevWeights[1][0] == weights[1][0]) {
      break;
    }
  }
  
  auto end = chrono::system_clock::now();
  auto elapsed = chrono::duration_cast<chrono::seconds>(end - start);
  
  cout << "Simple Data" << endl;
  cout << "Runtime: " << elapsed.count() << " seconds" << endl;
  cout << "Intercept: " << weights[0][0] << endl;
  cout << "Sex Weight: " << weights[1][0] << endl << endl;
  
  vector<vector<double> > testMatrix(sex_test.size());
  vector<vector<double> > testLabels(survived_test.size());
  for (int i = 0; i < testMatrix.size(); i++) {
    testMatrix.at(i).resize(2);
    testMatrix[i][0] = 1;
    testMatrix[i][1] = sex_test[i];
    
    testLabels.at(i).resize(1);
    testLabels.at(i).at(0) = survived_test.at(i);
  }
  vector<vector<double> > predicted = matrixMult(testMatrix, weights);
  
  int sumAll = 0;
  int truePos = 0;
  int trueNeg = 0;
  int totalPos = 0;
  for (int i = 0; i < predicted.size(); i++) {
    double probability = exp(predicted[i][0]) / (1 + exp(predicted[i][0]));
    
    totalPos += testLabels[i][0];
    if (probability >= 0.5 && testLabels[i][0] == 1) {
      sumAll += 1; truePos += 1;
    }
    if (probability < 0.5 && testLabels[i][0] == 0) {
      sumAll += 1; trueNeg += 1;
    }
  }
  
  double accuracy = (double) sumAll / testLabels.size();
  double sensitivity = (double) truePos / totalPos;
  double specificity = (double) trueNeg / (testLabels.size() - totalPos);
  cout << "Accuracy: " << accuracy << endl;
  cout << "Sensitivity: " << sensitivity << endl;
  cout << "Specificity: " << specificity << endl;
  
  weights.resize(4);
  for (int i = 0; i < 4; i++) {
    weights.at(i).resize(1);
    weights[i][0] = 1;
  }
  
  start = chrono::system_clock::now();
  
  for (int i = 0; i < 800; i++) {
    dataMatrix.at(i).resize(4);
    dataMatrix[i][0] = 1;
    dataMatrix[i][1] = sex[i];
    dataMatrix[i][2] = pclass[i];
    dataMatrix[i][3] = age[i];
  }
  
  for (int i = 0; i < 50000; i++) {
    vector<vector<double> > prevWeights = weights;
    vector<vector<double> > weightedMatrix = matrixMult(dataMatrix, weights);
    vector<vector<double> > probVector = sigmoid(weightedMatrix);
    vector<vector<double> > error = vectorSub(labels, probVector);
    weights = vectorAdd(weights, matrixMult(scalarMult(matrixTrans(dataMatrix), learnRate), error));
    if (
      prevWeights[0][0] == weights [0][0] && 
      prevWeights[1][0] == weights[1][0] &&
      prevWeights[2][0] == weights[2][0] &&
      prevWeights[3][0] == weights[3][0]
    ) {
      break;
    }
  }
  
  end = chrono::system_clock::now();
  elapsed = chrono::duration_cast<chrono::seconds>(end - start);
  
  cout << "All Data" << endl;
  cout << "Runtime: " << elapsed.count() << " seconds" << endl;
  cout << "Intercept: " << weights[0][0] << endl;
  cout << "Sex Weight: " << weights[1][0] << endl;
  cout << "Class Weight: " << weights[2][0] << endl;
  cout << "Age Weight: " << weights[3][0] << endl << endl;
  
  for (int i = 0; i < testMatrix.size(); i++) {
    testMatrix.at(i).resize(4);
    testMatrix[i][0] = 1;
    testMatrix[i][1] = sex_test[i];
    testMatrix[i][2] = pclass_test[i];
    testMatrix[i][3] = age_test[i];
  }
  predicted = matrixMult(testMatrix, weights);
  
  sumAll = 0;
  truePos = 0;
  trueNeg = 0;
  totalPos = 0;
  for (int i = 0; i < predicted.size(); i++) {
    double probability = exp(predicted[i][0]) / (1 + exp(predicted[i][0]));
    
    totalPos += testLabels[i][0];
    if (probability >= 0.5 && testLabels[i][0] == 1) {
      sumAll += 1; truePos += 1;
    }
    if (probability < 0.5 && testLabels[i][0] == 0) {
      sumAll += 1; trueNeg += 1;
    }
  }
  
  accuracy = (double) sumAll / testLabels.size();
  sensitivity = (double) truePos / totalPos;
  specificity = (double) trueNeg / (testLabels.size() - totalPos);
  cout << "Accuracy: " << accuracy << endl;
  cout << "Sensitivity: " << sensitivity << endl;
  cout << "Specificity: " << specificity << endl;
  
  cout << "\nProgram terminated";
  return 0;
}