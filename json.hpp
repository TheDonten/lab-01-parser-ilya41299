#include <any>
#include <iostream>
#include <fstream>
#include <optional>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <sstream>


class Json
{
private:
	std::unordered_map<std::string, std::any> map;
	std::vector<std::any> array;

	void find_key(std::istringstream& stream, std::string& key);
	void find_value(std::istringstream& stream, std::any& value, bool& flag);
	std::vector<std::any> parse_input_arr(std::istringstream& stream);
	Json pars_obj(std::istringstream& stream);

public:
	Json() {};
	// Конструктор из строки, содержащей Json-данные.
	Json(const std::string& s);

	// Метод возвращает true, если данный экземпляр содержит в себе JSON-массив. Иначе false.
	bool is_array() const;
	// Метод возвращает true, если данный экземпляр содержит в себе JSON-объект. Иначе false.
	bool is_object() const;

	// Метод возвращает значение по ключу key, если экземпляр является JSON-объектом.
	// Значение может иметь один из следующих типов: Json, std::string, double, bool или быть
	// пустым.
	// Если экземпляр является JSON-массивом, генерируется исключение.
	std::any& operator[](const std::string& key);

	// Метод возвращает значение по индексу index, если экземпляр является JSON-массивом.
	// Значение может иметь один из следующих типов: Json, std::string, double, bool или быть
	// пустым.
	// Если экземпляр является JSON-объектом, генерируется исключение.
	std::any& operator[](int index);

	// Метод возвращает объект класса Json из строки, содержащей Json-данные.
	static Json parse(const std::string& s);

	// Метод возвращает объекта класса Json из файла, содержащего Json-данные в текстовом формате.
	static Json parseFile(const std::string& path_to_file);
};

Json Json::pars_obj(std::istringstream& stream)
{
	char c;
	bool flag = true;
	Json MyJ;
	while (flag)
	{
		if (stream >> c && c != '"')
		{
			if (c == '}')
				break;
			else
				throw std::logic_error("Error input");
		}
		std::string key;
		find_key(stream, key);
		if ((stream >> c && c == ':') == false)
			throw std::logic_error("Error input");
		std::any value;
		find_value(stream, value, flag);
		MyJ.map.insert(std::pair(key, value));
	}
	return MyJ;
};

std::vector<std::any> Json::parse_input_arr(std::istringstream& stream)
{
	char c;
	std::any value;
	std::vector<std::any> v;
	bool flag = true;
	while (flag)
	{
		find_value(stream, value, flag);
		v.push_back(value);
		value.reset();
	}
	return array;
};

void Json::find_key(std::istringstream& stream, std::string& key)
{
	char c;

	while (stream >> c && c != '"')
	{
		if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')))
			throw std::logic_error("Error input");
		else
			key.push_back(c);
	}
};

void Json::find_value(std::istringstream& stream, std::any& value, bool& flag)
{
	char c;
	if (!(stream >> c))
		throw std::logic_error("Error input");

	std::string end;
	switch (c)
	{

	case 'f':
	{
		while (stream >> c && (c != ',' && c != '}' && c != ']'))
		{
			end.push_back(c);
		}
		if (end == "alse")
			value = false;
		else
			throw "Error_input";
		if (c == '}' || c == ']')
		{
			flag = false;
		}
		else if (c != ',')
			throw "Error_input";
		break;
	}
	case 't':
	{
		while (stream >> c && (c != ',' || c != '}' || c != ']'))
		{
			end.push_back(c);
		}
		if (end == "rue")
			value = true;
		else
			throw "Error_input";
		if (c == '}' || c == ']')
		{
			flag = false;
		}
		else if (c != ',')
			throw "Error_input";
		break;
	}

	case 'n':
	{
		while (stream >> c && (c != ',' || c != '}' || c != ']'))
		{
			end.push_back(c);
		}
		if (end == "ull")
			value = std::nullopt;
		else
			throw "Error_input";
		if (c == '}' || c == ']')
		{
			flag = false;
		}
		else if (c != ',')
			throw "Error_input";
		break;
	}

	case '"':
	{
		while (stream >> c && c != '"')
		{
			if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')))
				throw "Error_input";
			else
				end.push_back(c);
		}

		value = end;
		stream >> c;
		if (c == '}' || c == ']')
		{
			flag = false;
		}
		else if (c != ',')
			throw "Error_input";
		break;
	}
	case '[':
	{
		Json arr;
		value = arr.parse_input_arr(stream);
		if (stream >> c)
		{
			if (c == '}')
				flag = false;
			else if (c != ',')
				throw "Error_input";
		}
		break;
	}
	case '{':
	{
		//рекурсия
		Json new_obj;
		new_obj.pars_obj(stream);
		value = new_obj;
		break;
	}
	default:
	{
		if (isdigit(c))
		{
			end.push_back(c);
		}
		else
			throw "Error_input";
		bool is_double = false;

		while (stream >> c && (c != ',' && c != '}' && c != ']'))
		{
			if (c == '.')
			{
				if (is_double)
					throw "Error_input";
				else
				{
					is_double = true;
					end.push_back(c);
				}
			}
			else if (isdigit(c))
				end.push_back(c);
		}

		std::stringstream str_ch(end);
		if (is_double)
		{
			double res = 0;
			str_ch >> res;
			value = res;
		}
		else
		{
			int res = 0;
			str_ch >> res;
			value = res;
		}
		if (c == '}' || c == ']')
		{
			flag = false;
		}
		else if (c != ',')
			throw "Error_input";
		break;
	}
	}
};

Json::Json(const std::string& s)
{
	//в конструкторе проверяем строка или имя файла перед нами
	std::string name;
	for (unsigned i = 1; i < 5; i++)
	{
		name.push_back(s[s.size() - i]);
	}

	if (name == "txt.")
		parseFile(s);

	else
		parse(s);
};

Json Json::parseFile(const std::string& path_to_file)
{
	if (std::filesystem::exists(path_to_file)
		== false) //проверка на существование файла с данным именем
		throw std::logic_error("Error input");
	else
	{
		std::ifstream file(path_to_file);
		std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

		return parse(data);
	}
};

Json Json::parse(const std::string& s)
{
	std::istringstream stream(s);
	char c;
	stream >> c;
	switch (c)
	{
	case ('{'):
	{
		Json obj;
		return obj.pars_obj(stream);
	}
	case ('['):
	{
		Json ar;
		ar.array = ar.parse_input_arr(stream);
		return ar;
	}
	default:
		throw std::logic_error("Error input");
		break;
	}
};

