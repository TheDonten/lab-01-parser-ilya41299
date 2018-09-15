#include <any>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>


class Json
{
private:
	std::unordered_map<std::string, std::any> map;
	std::vector<std::any> array;

	enum type { Object, Array, null };

	type My_type;

	void find_key(std::istringstream& stream, std::string& key);
	void find_value(std::istringstream& stream, std::any& value, bool& flag);
	std::vector<std::any> parse_input_arr(std::istringstream& stream);
	Json pars_obj(std::istringstream& stream);

public:
	Json() { My_type = null; }
	// Конструктор из строки, содержащей Json-данные.
	Json(const std::string& s);

	// Метод возвращает true, если данный экземпляр содержит в себе JSON-массив. Иначе false.
	bool is_array() const;

	// Метод возвращает true, если данный экземпляр содержит в себе JSON-объект. Иначе false.
	bool is_object() const;

	// Метод возвращает true, если данный экземпляр не содержит ничего, т.е. null. Иначе false.
	bool is_null() const;

	// Метод возвращает значение по ключу key, если экземпляр является JSON-объектом.
	// Значение может иметь один из следующих типов: Json, std::string, double, bool или быть пустым.
	// Если экземпляр является JSON-массивом, генерируется исключение.
	const std::any& operator[](const std::string& key) const;

	// Метод возвращает значение по индексу index, если экземпляр является JSON-массивом.
	// Значение может иметь один из следующих типов: Json, std::string, double, bool или быть пустым.
	// Если экземпляр является JSON-объектом, генерируется исключение.
	const std::any& operator[](int index) const;
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
}

std::vector<std::any> Json::parse_input_arr(std::istringstream& stream)
{
	std::any value;
	std::vector<std::any> v;
	bool flag = true;
	while (flag)
	{
		find_value(stream, value, flag);
		v.push_back(value);
		value.reset();
	}
	return v;
}

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
}

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
		while (stream >> c && (c != ',' && c != '}' && c != ']'))
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
			value = null;
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
		Json My_arr;
		
		My_arr.array = My_arr.parse_input_arr(stream);
		
		if (stream >> c)
		{
			if (c == '}')
				flag = false;
			else if (c != ',')
				throw "Error_input";
		}
		My_arr.My_type = Array;
		value = My_arr;
		break;
	}
	case '{':
	{
		//рекурсия
		Json new_obj;
		new_obj = pars_obj(stream);
		new_obj.My_type = Object;

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
}

Json::Json(const std::string& s)
{
	//в конструкторе проверяем строка или имя файла перед нами
	std::string name;
	for (unsigned i = 1; i < 5; i++)
	{
		name.push_back(s[s.size() - i]);
	}
	Json J;
	if (name == "txt.")
		J = parseFile(s);
	else
		J = parse(s);

	this->array = J.array;
	this->map = J.map;
	this->My_type = J.My_type;
}

Json Json::parseFile(const std::string& path_to_file)
{
	std::ifstream file(path_to_file);
	if (!file.is_open())
		throw std::logic_error("Error input");
	else
	{
		std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
		return parse(data);
	}
}

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

		obj = obj.pars_obj(stream);
		obj.My_type = Object;
		return obj;
	}
	case ('['):
	{
		Json arr;
		arr.My_type = Array;
		arr.array = arr.parse_input_arr(stream);
		return arr;
	}
	default:
		throw std::logic_error("Error input");
		break;
	}
}

bool Json::is_array() const
{
	if (My_type == Array) return true;
	return false;
}

bool Json::is_object() const
{
	if (My_type == Object)
		return true;
	else
		return false;
}

bool Json::is_null() const
{
	if (My_type == null)
		return true;
	else
		return false;
}

const std::any& Json::operator[](const std::string& key) const
{
	return map.at(key);
}

const std::any& Json::operator[](int index) const
{
	return array.at(index);
}



