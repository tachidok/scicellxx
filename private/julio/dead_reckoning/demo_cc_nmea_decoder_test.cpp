#include <iostream>
#include <cmath>
#include <fstream>

// Include general/common includes, utilities and initialisation
#include "../../../src/general/common_includes.h"
#include "../../../src/general/utilities.h"
#include "../../../src/general/initialise.h"

// The nmea decoder
#include "cc_nmea_decoder.h"

using namespace chapchom;

#define X_MIN -32768
#define X_MAX 32767
#define FX_MIN_ACC -2.0
#define FX_MAX_ACC 2.0
#define FX_MIN_GYRO -250.0
#define FX_MAX_GYRO 250.0

// In charge of mapping the input value to the new scale
double scale(double x_min, double x_max, double fx_min, double fx_max, double x)
{
 const double s = (fx_max - fx_min) / (x_max - x_min);
 return ((x - x_min) * s) + fx_min;
}

int main(int argc, char *argv[])
{
 // Create the nmea decoder
 CCNMEADecoder nmea_decoder(20);
 // Create the object to deal with a file
 std::ifstream infile;
 infile.open("./bin/Cadenas_GNSS.txt", std::ios::in);
 if (infile.fail())
  {
   // Error message
   std::ostringstream error_message;
   error_message << "We could not open the NMEA strings file" << std::endl;
   throw ChapchomLibError(error_message.str(),
                          CHAPCHOM_CURRENT_FUNCTION,
                          CHAPCHOM_EXCEPTION_LOCATION);
  }
 
 // Extracted character
 char character;
 //o Loop until eof
 while(!infile.eof())
  {
   infile.get(character);
   //std::cerr << character;
   nmea_decoder.parse(character);
   // Check whether any of the data structures has new information
   if (nmea_decoder.is_accelerometer_data_ready())
    {
     // Get the data structure
     struct PSTM3DACC pstm3dacc = nmea_decoder.get_pstm3dacc();
     const double acc_x = scale(X_MIN, X_MAX, FX_MIN_ACC, FX_MAX_ACC, pstm3dacc.acc_x);
     const double acc_y = scale(X_MIN, X_MAX, FX_MIN_ACC, FX_MAX_ACC, pstm3dacc.acc_y);
     const double acc_z = scale(X_MIN, X_MAX, FX_MIN_ACC, FX_MAX_ACC, pstm3dacc.acc_z);
     // Consume accelerometer data
     nmea_decoder.consume_accelerometer_data();
     std::cout << "Accelerometer:(" << acc_x << ", " << acc_y << ", " <<  acc_z << ")" << std::endl;
    }
   
   if (nmea_decoder.is_gyro_data_ready())
    {
     // Get the data structure
     struct PSTM3DGYRO pstm3dgyro = nmea_decoder.get_pstm3dgyro();
     const double raw_x = scale(X_MIN, X_MAX, FX_MIN_GYRO, FX_MAX_GYRO, pstm3dgyro.raw_x);
     const double raw_y = scale(X_MIN, X_MAX, FX_MIN_GYRO, FX_MAX_GYRO, pstm3dgyro.raw_y);
     const double raw_z = scale(X_MIN, X_MAX, FX_MIN_GYRO, FX_MAX_GYRO, pstm3dgyro.raw_z);
     // Consume gyro data
     nmea_decoder.consume_gyro_data();
     std::cout << "Gyro:(" << raw_x << ", " << raw_y << ", " <<  raw_z << ")" << std::endl;
    }
   
   if (nmea_decoder.is_GPRMC_data_ready())
    {
     // Get the data structure
     struct GPRMC gprmc = nmea_decoder.get_gprmc();
     const double latitude = gprmc.latitude;
     const double longitude = gprmc.longitude;
     const double course_degrees = gprmc.course_degrees;
     // Consume GPRMC data
     nmea_decoder.consume_GPRMC_data();
     std::cout << "GPRMC:(" << latitude << ", " << longitude << ", " <<  course_degrees << ")" << std::endl;
    }
   
  }
 
 // Close the file
 infile.close();
 
 // Finalise chapcom
 finalise_chapchom();
 
 return 0;
 
}