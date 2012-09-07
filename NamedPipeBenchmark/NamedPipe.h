#pragma once

#include <cstdint>
#include <Windows.h>

#define DEFAULT_PIPE_NAME "\\\\.\\pipe\\namedpipebenchmark"
#define SERVER_ACCESS PIPE_ACCESS_DUPLEX
#define CLIENT_ACCESS (GENERIC_READ | GENERIC_WRITE)
#define MSG_SIZE (64*1024)

class NamedPipe {
	HANDLE _pipe;
public:
	NamedPipe() {
		_pipe = INVALID_HANDLE_VALUE;
	}

	~NamedPipe() {
		if (_pipe != INVALID_HANDLE_VALUE) {
			CloseHandle(_pipe);
		}
	}

	int32_t CreatePipeServer(int32_t direction, const char* name) {
		int32_t openMode = 0;
		int32_t pipeMode = PIPE_TYPE_BYTE|PIPE_READMODE_BYTE;
		int32_t instances = PIPE_UNLIMITED_INSTANCES;
		int32_t bufferSize = 64 * 1024;
		int32_t defaultTimeout = 0; // results in default of 50ms
		LPSECURITY_ATTRIBUTES securityAttributes = NULL;
		_pipe = CreateNamedPipe(name,
								openMode|PIPE_ACCESS_DUPLEX,
								pipeMode,
								instances,
								bufferSize,
								bufferSize,
								defaultTimeout,
								securityAttributes);
		if (_pipe == INVALID_HANDLE_VALUE) {
			return GetLastError();
		}
		return 0;
	}

	int32_t ConnectPipeCLient(int32_t direction, const char* name) {
		int32_t desiredAccess = direction;
		int32_t shareMode = 0;
		LPSECURITY_ATTRIBUTES securityAttributes = NULL;
		_pipe = CreateFile(name, desiredAccess, shareMode, securityAttributes, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (_pipe == INVALID_HANDLE_VALUE) {
			return GetLastError();
		}
		return 0;
	}

	int32_t Connect() {
		LPOVERLAPPED overLapped = NULL;
		BOOL r = ConnectNamedPipe(_pipe, overLapped);
		if (r) {
			return 0;
		}
		return GetLastError();
	}

	int32_t Disconnect() {
		BOOL r = DisconnectNamedPipe(_pipe);
		if (r) {
			return 0;
		}
		return GetLastError();
	}

	int32_t Read(void* buffer, int32_t requestedBytes) {
		DWORD readBytes;
		BOOL r = ReadFile(_pipe, buffer, requestedBytes, &readBytes, NULL);
		if (r) {
			return readBytes;
		}
		return GetLastError();
	}

	int32_t Write(const void* buffer, int32_t requestedBytes) {
		DWORD writtenBytes;
		BOOL r = WriteFile(_pipe, buffer, requestedBytes, &writtenBytes, NULL);
		if (r) {
			return writtenBytes;
		}
		return GetLastError();
	}
};