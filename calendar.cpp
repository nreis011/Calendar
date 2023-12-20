#include "calendar.h"
#include "tinyxml2/tinyxml2.h"
#include "tinyxml2/tinyxml2.cpp"

using namespace tinyxml2;

// Given a day, month and year, returns the int index of vector days.
int firstYearDay (int year){
    // Zeller's congruence algorithm
    int day = 1;
    int month = 13;
    year--;

    int k = year % 100;
    int j = year/100;
    day = (day + ((13*(month + 1))/5) + k + (k/4) + (j/4) - (2*j)) % 7;

    if (day < 2)
        day += 5;
    
    else
        day -=2;

    return day;
}

// argv should be: name of program, year, name of file
// Main is the generator itself, it only does the HTML generation and logic processing for it.
int main(int argc, char* argv[]){
    // Take argv 1, containing year & get first day for year-1.
    if(argc < 3){
        return -1;
    }

    const char* fname = (std::string(argv[2]) + ".html").c_str();
    int year = std::atoi(argv[1])-1;
    int firstDay = firstYearDay(year);
    bool leapYear = false;

    // Build calendar from first day of year-1 until 31st december year+1

    // XML document as a surrogate for HTML
    XMLDocument doc;
    XMLElement* html = doc.NewElement("html");
    doc.InsertEndChild(html);
    
    // HTML startup: head & body
    XMLElement* head = html->InsertNewChildElement("head");
    XMLElement* style = head->InsertNewChildElement("style");
    style-> InsertNewText("table, th, td {border: 1px solid black; border-collapse: collapse;}");

    XMLElement* body = html->InsertNewChildElement("body");

    // For year-1 until year+1, create the calendar

    for(int y = 0; y < 3; y++, year++){
        int week = 0;
        if ((year % 100 == 0 && year % 400 == 0) || (year % 100 != 0 && year % 4 == 0))
            daysinMonth[1] = 29;
        else
            daysinMonth[1] = 28;

        // Create year table and caption
        XMLElement* HTMLyear = body->InsertNewChildElement("table");
        HTMLyear->SetAttribute("style", "width:100%");

        XMLElement* caption = HTMLyear->InsertNewChildElement("caption");
        caption->InsertNewText(std::to_string(year).c_str());

        // table row tag for month headers
        XMLElement* HTMLtr = HTMLyear->InsertNewChildElement("tr");

        // For each month
        for (int m = 0; m < 12; m++){
            // Create the table header for that month
            XMLElement* HTMLmonth = HTMLtr->InsertNewChildElement("th");
            HTMLmonth->InsertNewText(months[m].c_str());
        }
        
        // table row tag for table display
        HTMLtr = HTMLyear->InsertNewChildElement("tr");

        // For each for the month headers
        for (int m = 0; m < 12; m++){

            // create a table display with a table with the day of the week as a header
            XMLElement* HTMLmonth = HTMLtr->InsertNewChildElement("td");
            HTMLmonth = HTMLmonth->InsertNewChildElement("table");
            HTMLmonth->SetAttribute("style", "width:100%; height:100%");
            XMLElement* HTMLmonthrow = HTMLmonth->InsertNewChildElement("tr");
            XMLElement* HTMLday = HTMLmonthrow->InsertNewChildElement("td");
            HTMLday->InsertNewText("Wk");

            // create the header for each day of the week
            for(int d = 0; d < 7; d++){
                XMLElement* HTMLday = HTMLmonthrow->InsertNewChildElement("th");
                HTMLday->InsertNewText(days[d].c_str());
            }

            // Logic for creating the numbers of the calendar.
            // Iterate over the days in a given month
            int d = 0; // day variable, will be handy in tracking first day of month
            for(int monthday = 1; monthday <= daysinMonth[m];){
                // Make a row for every week
                week++;
                HTMLmonthrow = HTMLmonth->InsertNewChildElement("tr");
                XMLElement* HTMLday = HTMLmonthrow->InsertNewChildElement("th");
                // write the week
                HTMLday->InsertNewText(std::to_string(week).c_str());

                // Generate the days for that week
                for(d = 0; d < 7 && monthday <= daysinMonth[m]; d++, monthday++){
                    if (monthday == 1){
                        for (;d < firstDay; d++){
                            // Generate empty squares before the first day of that month
                            XMLElement* HTMLday = HTMLmonthrow->InsertNewChildElement("th");
                        }
                    }
                    // Print out the day of the month number
                    XMLElement* HTMLday = HTMLmonthrow->InsertNewChildElement("th");
                    HTMLday->InsertNewText(std::to_string(monthday).c_str());
                }
            }
            // Check if it ended on a Sunday.
            if(d == 7)
                firstDay = 0;
            
            // If it didn't, save the number, fill out the rest of the month with empty squares.
            else{
                firstDay = d;
                for(;d < 7; d++){
                    XMLElement* HTMLday = HTMLmonthrow->InsertNewChildElement("th");
                }
            }
        }
    }
        
    // Save to file!
    doc.SaveFile(fname);

    return 0;
}