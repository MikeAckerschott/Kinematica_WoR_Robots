#ifndef CONFIG_FILE_READER_HPP
#define CONFIG_FILE_READER_HPP

#include <boost/asio.hpp>
#include <fstream>

class ConfigFileReader
{
public:
    ConfigFileReader();

    bool read(double &compassSdev, double &odomSdev, double &lidarSdev);

private:
    std::string filename_;
    //file stream
    std::ifstream file_;

    bool isNumerical(const std::string &str);
};

#endif // CONFIG_FILE_READER_HPP
