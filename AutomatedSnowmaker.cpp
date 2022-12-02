#include <iostream>
#include <fstream>

#include "FuzzyControl.hpp"


int main(int argc, char *argv[]){

    // Temperature
    auto cold = new FuzzySet(new TriangleMF(std::numeric_limits<int>::min(),0,30));
    auto cool = new FuzzySet(new TriangleMF(-10,20,40));
    auto warm = new FuzzySet(new TriangleMF(20,40,std::numeric_limits<int>::max()));
    Fuzzifier temperature;
    temperature.addValue(cold);
    temperature.addValue(cool);
    temperature.addValue(warm);

    // Sun Exposure
    auto dark = new FuzzySet(new TriangleMF(std::numeric_limits<int>::min(),10,40));
    auto overcast = new FuzzySet(new TriangleMF(20,40,60));
    auto sunny = new FuzzySet(new TriangleMF(50,70,std::numeric_limits<int>::max()));
    Fuzzifier sunExposure;
    sunExposure.addValue(dark);
    sunExposure.addValue(overcast);
    sunExposure.addValue(sunny);

    // Dew Point
    auto lowHumidity = new FuzzySet(new TriangleMF(std::numeric_limits<int>::min(),10,31));
    auto medHumidity = new FuzzySet(new TriangleMF(10,30,40));
    auto highHumidity = new FuzzySet(new TriangleMF(35,50,std::numeric_limits<int>::max()));
    Fuzzifier dewPoint;
    dewPoint.addValue(lowHumidity);
    dewPoint.addValue(medHumidity);
    dewPoint.addValue(highHumidity);

    // Output Variable
    auto fast = FuzzySet(new TriangleMF(80,90,100));
    auto medium = FuzzySet(new TriangleMF(50,60,70));
    auto slow = FuzzySet(new TriangleMF(30,40,50));
    auto off = FuzzySet(new TriangleMF(-1,0,1));


    SimplifiedPIECA fc({FuzzyRule({cold,dark,lowHumidity},fast),
                        FuzzyRule({cold,overcast,lowHumidity},medium),
                        FuzzyRule({cold,dark,medHumidity},medium),
                        FuzzyRule({cool,dark,lowHumidity},medium),
                        FuzzyRule({cool,dark,medHumidity},slow),
                        FuzzyRule({cool,overcast,lowHumidity},slow),
                        FuzzyRule({sunny},off),
                        FuzzyRule({warm},off),
                        FuzzyRule({highHumidity},off),
                        },
                       {temperature,
                        sunExposure,
                        dewPoint
                       }
                       );

    std::ofstream csvFile{"out.csv"};
    if(csvFile){
        csvFile << "sun,temp,dew,out" << std::endl;
        for(int i = 0; i < 100; ++i){
            for(int j = -10; j < 50; ++j){
                for(int k = -10; k < j+2; ++k){
                    csvFile << i << "," << j <<  "," << k << "," << fc.fuzzDefuzz({double(j),double(i),double(k)}) << std::endl;
                }
            }
        }
    }
    csvFile.close();

}

