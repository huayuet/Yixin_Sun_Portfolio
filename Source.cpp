#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;


float getsum(vector<float> v) {

    float sum = 0;
    for (int i = 0; i < v.size(); i++) {
        sum += v[i];
    }
    return sum;
}
float getmean(vector<float> v) {
    float sum = getsum(v);
    float mean = sum / v.size();
    return mean;
}
float getmedian(vector<float> v) {

    int size = v.size();
    sort(v.begin(), v.end());
    float median;

    if (size % 2 == 0) {
        median = (v[size / 2] + v[size / 2 - 1]) / 2;
    }
    else {
        median = v[size / 2];
    }
    return median;
}
std::pair<float,float> getrange(vector<float> v) {
    std::pair<float, float> result;
    float max = 0;
    float min = 10000;
    for (int i = 0; i < v.size(); i++) {
        if (v[1] > max){
            max = v[i];
        }
        if (v[1] < min) {
            min = v[i];
        }
    }
    result = std::make_pair(min, max);
    return result;
}
float getcov(vector<float> x, vector<float> y) {
    float rm = getmean(x);
    float medv = getmean(y);
    float numerator = 0;

    for (int i = 0; i < x.size(); i++) {
        numerator += (x[i] - rm) * (y[i] - medv);
    }

    float covariance = numerator / (x.size() - 1);

    return covariance;
}
float getcorr(vector<float> x, vector<float> y) {
    float rm_cov = getcov(x, x);
    float stand_rm = sqrt(rm_cov);
    float medv_cov = getcov(y, y);
    float stand_medv = sqrt(medv_cov);
    float down = stand_rm * stand_medv;
    float up_cov = getcov(x, y);

    float corr = up_cov / down;
    return corr;



    
}





int main()
{
    ifstream inFile("Boston.csv");
    if (!inFile.is_open()) throw runtime_error("Could not open file");
    string linestr;
    string rm_in, medv_in;
    getline(inFile, linestr);
    vector<float> rm;
    vector<float> medv;
    
    while (getline(inFile, linestr)) {
        stringstream ss(linestr);
        getline(ss, rm_in, ',');
        getline(ss, medv_in, '\n');
        rm.push_back(stof(rm_in));
        medv.push_back(stof(medv_in));
    }
    inFile.close();

    
    cout << "stats for rm \n";
    cout << "Sum of rm : " << getsum(rm) << "\n";
    cout << "Mean of rm : " << getmean(rm) << "\n";
    cout << "Median of rm : " << getmedian(rm) << "\n";
    std::pair<float, float> result = getrange(rm);
    cout << "Range of rm : " << result.first << " to " << result.second << " which is " << result.second - result.first << "\n";


    cout << "stats for medv \n";
    cout << "Sum of medv : " << getsum(medv) << "\n";
    cout << "Mean of medv : " << getmean(medv) << "\n";
    cout << "Median of medv : " << getmedian(medv) << "\n";
    std::pair<float, float> result2 = getrange(medv);
    cout << "Range of medv : " << result2.first << " to " << result2.second << " which is " << result2.second - result2.first << "\n";

    cout << "Covariance between rm and medv : " << getcov(rm, medv) << "\n";
    cout << "Correlation between rm and medv : " << getcorr(rm, medv) << "\n";

}
