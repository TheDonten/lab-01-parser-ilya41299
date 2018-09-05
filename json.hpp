#include "stdafx.h"
#include <any>
#include <iostream>
#include <fstream>
#include <optional>
#include <string>
#include <unordered_map>
#include <filesystem>


class Json {
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

std::optional<std::string> read_data(const std::string & filename) 
{
	//Если файл не существует - возвращаем пустоту
	//Иначе происходит чтение файла
	if (std::filesystem::exists(filename) == false)
		return std::nullopt;//не хранит данных

	std::ifstream file(filename);

	std::string data((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());

	return data;
}

int main()
{
	auto data = read_data("Test_JSON.txt");
	if (data)
		std::cout << data.value() << std::endl;
	else
		std::cout << "file does not exist" << std::endl;

    return 0;
}

