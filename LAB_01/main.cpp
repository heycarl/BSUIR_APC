#include <windows.h>
#include <iostream>
#include <sstream>

#include "logger.hpp"

HANDLE COM_Port_1;
LPCTSTR Port_Name_1 = "//./COM3";
HANDLE COM_Port_2;
LPCTSTR Port_Name_2 = "//./COM4";

void Read_from_COM()
{
	DWORD Size;
	char Received_Char;
	ReadFile(COM_Port_2, &Received_Char, 1, &Size, nullptr);
	if (Size>0) {
		std::cout << "Received char: " << Received_Char << std::endl;
	}
}

int main()
{
	COM_Port_1 = CreateFile(Port_Name_1, GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	COM_Port_2 = CreateFile(Port_Name_2, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (COM_Port_1==INVALID_HANDLE_VALUE || COM_Port_2==INVALID_HANDLE_VALUE) {
		if (GetLastError()==ERROR_FILE_NOT_FOUND) {
			LOG_ERROR("COM-port does not exist!");
			return 1;
		}
		LOG_ERROR("Unspecified error");
		return 1;
	}

	DCB Serial_Params = { 0 };
	Serial_Params.DCBlength = sizeof(Serial_Params);
	if (!GetCommState(COM_Port_1, &Serial_Params)) {
		LOG_ERROR("get COM state error");
		return 1;
	}
	Serial_Params.BaudRate = CBR_9600;
	Serial_Params.ByteSize = 8;
	Serial_Params.StopBits = ONESTOPBIT;
	Serial_Params.Parity = NOPARITY;
	if (!SetCommState(COM_Port_2, &Serial_Params)) {
		LOG_ERROR("set COM state error");
		return 1;
	}

	char data = 'h';
	DWORD Size = sizeof(data);
	DWORD Bytes_Written;

	if (WriteFile(COM_Port_1, &data, Size, &Bytes_Written, nullptr)==0) {
		LOG_ERROR("Write COM failed");
		return 1;
	}

	std::stringstream ss;
	ss << Size << " bytes exists, " << Bytes_Written << " bytes sent ";
	LOG_INFO(ss.str());

	Read_from_COM();

	CloseHandle(COM_Port_1);
	CloseHandle(COM_Port_2);
	LOG_INFO("Handlers closed");

	return 0;
}

