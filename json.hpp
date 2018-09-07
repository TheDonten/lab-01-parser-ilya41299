
#include "stdafx.h"
#include <any>
#include <iostream>
#include <fstream>
#include <optional>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <sstream>


class Json {
private:	
	std::unordered_map <std::string, std::any> Mymap;
	void pars_obj(std::istringstream& stream) 
	{
		std::string key;
		
		std::any value;
		char c;
		if (!(stream >> c && c != '"'))
			throw "Error_input";	
			while (stream >> c && c != '"') 
			{
				if (!((c>= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')))
					throw "Error_input";
			}
	
		if (stream>>c && c!=':')
			throw "Error_input";
		if (stream >> c && c != '"')
		{
			std::string end;
			switch (c)
			{

			case 'f':
			{
				while (stream >> c && c != ',')
				{
					end.push_back(c);
				}
				if (end == "alse") value = false;
				else throw "Error_input";
				break;
			}
			case 't':
			{
				while (stream >> c && c != ',')
				{
					end.push_back(c);
				}
				if (end == "rue") value = true;
				else throw "Error_input";
				break;
			}

			case '"':
			{
				while (stream >> c && c != '"')
				{
					if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')))
						throw "Error_input";
					else end.push_back(c);
				}
				value = end;
				break;
			}
			case '[': {//вызов функции чтения массива
			}
			case '{': {}
			default:
			{
				throw "Error_input";
				break;
			}
			}

		}
	};
public:
  
	// Конструктор из строки, содержащей Json-данные.
	Json(const std::string& s);

	// Метод возвращает true, если данный экземпляр содержит в себе JSON-массив. Иначе false.
	bool is_array() const;
	// Метод возвращает true, если данный экземпляр содержит в себе JSON-объект. Иначе false.
	bool is_object() const;

	// Метод возвращает значение по ключу key, если экземпляр является JSON-объектом.
	// Значение может иметь один из следующих типов: Json, std::string, double, bool или быть пустым.
	// Если экземпляр является JSON-массивом, генерируется исключение.
	std::any& operator[](const std::string& key);

	// Метод возвращает значение по индексу index, если экземпляр является JSON-массивом.
	// Значение может иметь один из следующих типов: Json, std::string, double, bool или быть пустым.
	// Если экземпляр является JSON-объектом, генерируется исключение.
	std::any& operator[](int index);

	// Метод возвращает объект класса Json из строки, содержащей Json-данные.
	static Json parse(const std::string& s);

	// Метод возвращает объекта класса Json из файла, содержащего Json-данные в текстовом формате.
	static Json parseFile(const std::string& path_to_file);
};

Json::Json(const std::string& s) 
{
	parseFile(s);
};

 Json Json::parseFile(const std::string& path_to_file) 
 {
	 if (std::filesystem::exists(path_to_file) == false)	//проверка на существование файла с данным именем
		 throw("Error 404");
	 
	 std::ifstream file(path_to_file);

	 std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	 
	 std::istringstream stream(data);
	 char c;
	 stream >> c;

	 switch (c)
	 {
	 case '{': 
	 {
		 pars_obj(stream);
	 }
	 default:
		 break;
	 }
 };

 Json Json::parse(const std::string& s)
 {
	
 };

std::optional<std::string> read_data(const std::string & filename) 
{
	//Если файл не существует, возвращаем пустоту
	//Иначе происходит чтение файла
	if (std::filesystem::exists(filename) == false)
		return std::nullopt;//не хранит данных

	std::ifstream file(filename);

	std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	return data;
}
