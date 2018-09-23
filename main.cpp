#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>
using nlohmann::json;

using nlohmann::json;

std::string read()
{
    std::string buf, string = "";
    do
    {
        std::getline(std::cin, buf);
        string += buf;
    } while (buf != "");

    std::istringstream stream(string);
    string.clear();
    while (stream >> buf)
    {
        string += buf;
    }
    return string;
}

int main()
{
    json data = json::parse(read());
    json array = json::array();
    for (auto i : data)
    {
        array.push_back(i);
    }
    data.clear();
    unsigned int i = 0;
    for (auto j : array)
    {
        json object = 
          { 
            { "ticker", array[0][i] },
            { "id", array[1][i] },
            { "description", array[2][i++] } 
          };
        data.push_back(object);
    }
    std::cout << read();
    return 0;
}
