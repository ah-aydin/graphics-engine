#include "fileUtils.h"

#include <string.h>

#include <Logging/Log.h>

int endsWith(const char* s, const char* part)
{
	return strstr(s, part) - s == strlen(s) - strlen(part);
}

std::string readShaderFile(const char* fileName)
{
	FILE* file;
	fopen_s(&file, fileName, "r");

	if (!file)
	{
		log_error("ERROR::READ_SHADER_FILE::FAILED_TO_OPEN_FILE::%s", fileName);
		return std::string();
	}
	
	fseek(file, 0L, SEEK_END);
	const long bytesinfile = ftell(file);
	fseek(file, 0L, SEEK_SET);

	char* buffer = (char*) _malloca(static_cast<size_t>(bytesinfile) + 1);
	if (!buffer)
	{
		return std::string();
	}

	const size_t bytesread = fread(buffer, 1, bytesinfile, file);
	fclose(file);

	buffer[bytesread] = 0;

	static constexpr unsigned char BOM[] = { 0xEF, 0xBB, 0xBF };

	std::string code(buffer);

	while (code.find("#include ") != code.npos)
	{
		const auto pos = code.find("#include ");
		const auto p1 = code.find('<', pos);
		const auto p2 = code.find('>', pos);
		if (p1 == code.npos || p2 == code.npos || p1 >= p2)
		{
			log_error("ERROR::READ_SHADER_FILE::FAILED_TO_PROCESS_INCLUDE_STATEMENT\n%s", code.c_str());
			return std::string();
		}
		const std::string name = code.substr(p1 + 1, p2 - p1 - 1);
		const std::string include = readShaderFile(name.c_str());
		code.replace(pos, p2 - pos + 1, include.c_str());
	}

	return code;
}

void printShaderSource(const char* text)
{
	int line = 1;
	printf("\n(%3i) ", line);
	while (text && *text++)
	{
		if (*text == '\n')
		{
			printf("\n(%3i) ", ++line);
		}
		else if (*text == '\r')
		{

		}
		else
		{
			printf("%c", *text);
		}
	}

	printf("\n");
}