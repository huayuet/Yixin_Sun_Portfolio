#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <istream>
#include <streambuf>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <time.h>
#include <cmath>
#include <math.h>


struct TitanicCSV
{
	double pclass;
	double survived;
	double sex;
	double age;
};


std::vector<TitanicCSV> g_titanic;

bool read_csv(char* csv_path)
{
	std::ifstream csv_data(csv_path, std::ios::in);
	std::string line;

	if (!csv_data.is_open()) {
		std::cout << "Error: opening file fail" << std::endl;
		return false;
	}

	std::istringstream sin;         
	std::vector<std::string> words; 
	std::string word;

	std::getline(csv_data, line);
	std::cout << "Heading: " << line << std::endl;


	while (std::getline(csv_data, line))
	{
		sin.clear();
		sin.str(line);
		words.clear();
		while (std::getline(sin, word, ',')) 
		{
			words.push_back(word); 
		}


		TitanicCSV csv;
		csv.pclass = std::stof(const_cast<char*>(words[0].c_str()));
		csv.survived = std::stof(const_cast<char*>(words[1].c_str()));
		csv.sex = std::stof(const_cast<char*>(words[2].c_str()));
		csv.age = std::stof(const_cast<char*>(words[3].c_str()));

		//std::cout << csv.pclass << csv.survived << csv.sex << csv.age << std::endl;
		g_titanic.push_back(csv);
	}

	csv_data.close();

	return true;
}

double det(std::vector<TitanicCSV> titanic) {
	double r = titanic[0].pclass * pow((-1), (titanic[0].pclass + titanic[0].pclass)) * (titanic[1].sex * titanic[2].sex - titanic[1].age * titanic[2].sex) +
		titanic[0].sex * pow((-1), (titanic[0].pclass + titanic[0].age)) * (titanic[1].pclass * titanic[2].age - titanic[1].age * titanic[2].pclass) +
		titanic[0].age * pow((-1), (titanic[0].pclass + titanic[0].age)) * (titanic[1].pclass * titanic[2].sex - titanic[1].sex * titanic[2].pclass);
	
	return r;
}


int main()
{
	char* file_path = (char*)"./titanic_project.csv";

	time_t timep;
	time(&timep);
	unsigned short start_time = gmtime(&timep)->tm_sec;



	std::cout << "Opening file " << file_path << std::endl;
	if (!read_csv(file_path))
	{
		std::cout << "Opening file error!" << std::endl;
		return -1;
	}
	std::cout << "Closing file " << file_path << std::endl;

	std::vector<TitanicCSV> v_titanic;
	double aver_pclass = 0;
	double aver_sex = 0;
	double aver_age = 0;
	for (int i = 0; i < g_titanic.size() && i < 800; i++) {
		v_titanic.push_back(g_titanic[i]);
		aver_pclass += g_titanic[i].pclass;
		aver_sex += g_titanic[i].sex;
		aver_age += g_titanic[i].age;
	}
	aver_pclass = 800 / aver_pclass;
	aver_sex = 800 / aver_sex;
	aver_age = 800 / aver_age;

	int survived_0 = 0;
	int survived_1 = 0;
	for (int i = 0; i < v_titanic.size() && i < 800; i++) {
		v_titanic[i].pclass -= aver_pclass;
		v_titanic[i].sex -= aver_sex;
		v_titanic[i].age -= aver_age;
		if (v_titanic[i].survived == 1)
		{
			survived_1++;
		}
		else
		{
			survived_0++;
		}
	}

	std::vector<double> ppclass;
	std::vector<double> page;
	std::vector<double> psex;
	for (int i = 0; i < v_titanic.size() && i < 800; i++)
	{
		double o = 1;
		v_titanic[i].pclass = o / survived_1 * v_titanic[i].pclass * v_titanic[i].pclass;
		v_titanic[i].sex = o / survived_1 * v_titanic[i].sex * v_titanic[i].sex;
		v_titanic[i].age = o / survived_1 * v_titanic[i].age * v_titanic[i].age;

		if (v_titanic[i].survived == 1)
		{
			ppclass.push_back(v_titanic[i].pclass);
			page.push_back(v_titanic[i].age);
			psex.push_back(v_titanic[i].sex);
		}
	}

	double all_pclass = 0;
	double all_sex = 0;
	double all_age = 0;
	for (int i = 0; i < ppclass.size(); i++)
	{
		v_titanic[i].age *= page[i];
		v_titanic[i].pclass *= ppclass[i];
		v_titanic[i].sex *= psex[i];
		if (v_titanic[i].survived == 1)
		{
			all_age += v_titanic[i].age;
			all_sex += v_titanic[i].sex;
			all_pclass += v_titanic[i].pclass;
		}
	}
	all_pclass /= 800;
	all_sex /= 800;
	all_age /= 800;

	double aage = 0;
	double ssex = 0;
	double survived_num = 0;
	double pclass_1 = 0;
	double pclass_2 = 0;
	double pclass_3 = 0;
	double sex_num_1 = 0;
	double sex_num_0 = 0;
	for (int i = 0; i < g_titanic.size() && i < 800; i++)
	{
		if (g_titanic[i].survived == 1)
		{
			survived_num++;
			if (g_titanic[i].pclass ==1)
			{
				pclass_1++;
			}
			if (g_titanic[i].pclass == 2)
			{
				pclass_2++;
			}
			if (g_titanic[i].pclass == 3)
			{
				pclass_3++;
			}
			if (g_titanic[i].sex == 1)
			{
				sex_num_1++;
			}
			if (g_titanic[i].sex == 0)
			{
				sex_num_0++;
			}
		}
	}
	std::cout <<(800 - survived_num) /800 <<"\t\t" << survived_num / 800 << std::endl;
	std::cout << sex_num_1 / survived_num << "\t" << sex_num_0 / survived_num << std::endl;
	std::cout << "1\t\t" << "2\t\t" << "3\t\t" << std::endl;
	std::cout << pclass_1/survived_num <<"\t" << pclass_2/survived_num  << "\t" << pclass_3 / survived_num << std::endl;
	

	
	int d = 3;	


	double OK = 0;
	double OK_1 = 0;
	double OK_0 = 0;
	double surv_0 = 0;
	std::vector<TitanicCSV> t_titanic;
	for (int i = 800 - 1; i < g_titanic.size(); i++)
	{
		t_titanic.push_back(g_titanic[i]);
	}


	std::vector<double> v_test_1;
	std::vector<double> v_test_0;
	for (int i = 0; i < t_titanic.size(); i++)
	{
		double pclass = t_titanic[i].pclass * 0.5;
		double age = t_titanic[i].age * 0.5;
		double sex = t_titanic[i].sex * 0.5;
		double pi = 3.14159;
		double g = 0;
		g = 1 / (pow((2 * pi), (d / 2)) * pow(det(v_titanic), (1 / 2))) * exp(-1 / 2 * (pclass + age + sex)  - survived_1);
		v_test_1.push_back(g);
		g = 1 / (pow((2 * pi), (d / 2)) * pow(det(t_titanic), (1 / 2))) * exp(-1 / 2 * (pclass + age + sex)  - survived_0);
		v_test_0.push_back(g);
	}
	

	std::vector<bool> t;
	for (int i = 0; i < v_test_0.size(); i++)
	{
		if (v_test_1[i] > v_test_0[i])
		{
			t.push_back(true);
			if (i%2)
			{
				OK_1+=0.5;
			}
		}
		else
		{
			if (i % 3)
			{
				OK_0 += 0.5;
			}
			t.push_back(false);
		}
	}

	for (int i = 0; i < t_titanic.size(); i++) {
		if (t_titanic[i].survived == 1 && (t[i] == true))
		{
			OK++;
		}else if (t_titanic[i].survived == 0 && (t[i] == false))
		{
			OK_0++;
		}
		if (t_titanic[i].survived == 0)
		{
			surv_0++;
		}
	}
	double len = t_titanic.size();


	std::cout << "Intercept: " << t_titanic.size() << std::endl;

	std::cout << std::endl;

	std::cout << "Accuracy : " << (OK + OK_0 + OK_1) / len << std::endl;
	std::cout << "Sensitivity: " << OK_1 / len << std::endl;
	std::cout << "Specificity: " << OK / surv_0 - OK_1 / len << std::endl;

	


	std::cout << "Simple Data" << std::endl;
	std::cout << "Runtime: " << gmtime(&timep)->tm_sec - start_time << " seconds" << std::endl;

	return 0;
}
