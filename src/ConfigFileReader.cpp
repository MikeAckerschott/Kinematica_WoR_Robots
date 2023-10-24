#include "ConfigFileReader.hpp"

#include "CompassSensor.hpp"
#include "LidarDistanceSensor.hpp"
#include "OdometerSensor.hpp"

ConfigFileReader::ConfigFileReader()
    : filename_("../../config/config.txt")
{
}

bool ConfigFileReader::read(double &compassSdev, double &odomSdev, double &lidarSdev)
{
    std::ifstream configFile(filename_);
    if (!configFile.is_open())
    {
        std::cout << "Error: could not open config file: " << filename_ << std::endl;
        std::cout << "Make sure you are in the linux/src directory and that the file exists" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(configFile, line))
    {
        size_t pos = line.find(':');
        if (pos != std::string::npos)
        {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 2);

            if (!isNumerical(value))
            {
                std::cout << "Error: '" << value << "' given for key: " << key << " is not a number" << std::endl;
                return false;
            }

            

            if (key == "ODOM_SDEV")
            {
                odomSdev = std::stod(value);
                Model::OdometerSensor::setStdDev(odomSdev);

                std::cout<<"Setting odom sdev to "<<odomSdev<<std::endl;
                //check if set correclty
                std::cout<<"Odom sdev is "<<Model::OdometerSensor::getStdDev()<<std::endl;
            }
            else if (key == "COMPASS_SDEV")
            {
                compassSdev = std::stod(value);
                Model::CompassSensor::setStdDev(compassSdev);

                std::cout<<"Setting compass sdev to "<<compassSdev<<std::endl;
                //check if set correclty
                std::cout<<"Compass sdev is "<<Model::CompassSensor::getStdDev()<<std::endl;
            }
            else if (key == "LIDAR_SDEV")
            {
                lidarSdev = std::stod(value);
                Model::LidarDistanceSensor::setStdDev(lidarSdev);

                std::cout<<"Setting lidar sdev to "<<lidarSdev<<std::endl;
                //check if set correclty
                std::cout<<"Lidar sdev is "<<Model::LidarDistanceSensor::getStdDev()<<std::endl;
            } else {
                std::cout<<"Error: '"<<key<<"' is not a valid key"<<std::endl;
            }
        }
    }

    // Close the file when done
    configFile.close();
    return true;
}

bool ConfigFileReader::isNumerical(const std::string &str)
{
    bool isNumerical = true;
    for (char const &c : str)
    {
        if ((c < '0' || c > '9') && c != '.')
        {
            isNumerical = false;
            break;
        }
    }
    return isNumerical;
}