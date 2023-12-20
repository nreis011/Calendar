#include <vector>
#include <string>
#include <iostream>
#include <fstream> 
#include <filesystem>

std::vector<std::string> days{"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"};
std::vector<std::string> months{"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December",};
std::vector<int> daysinMonth{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int firstYearDay (int);