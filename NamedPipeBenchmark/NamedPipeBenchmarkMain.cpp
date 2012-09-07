#include <cstdio>

#include "NamedPipe.h"

int main(int argc, char** argv) {
	NamedPipe np;
	int32_t r;
	r = np.ConnectPipeCLient(CLIENT_ACCESS, DEFAULT_PIPE_NAME);
	if (r != 0) {
		return -1;
	}
	printf("Opened pipe.\n");

	// 1MB
	void* readBuffer = malloc(MSG_SIZE);
	void* writeBuffer = malloc(MSG_SIZE);
	int32_t count = 0;
	LARGE_INTEGER start, end, freq;
	QueryPerformanceFrequency(&freq);
	double secondsPerTick = 1.0 / freq.QuadPart;
	QueryPerformanceFrequency(&start);
	while (true) {
		r = np.Write(writeBuffer, MSG_SIZE);
		if (r != MSG_SIZE) {
			printf("Write invalid size: %d\n", r);
			break;
		}
		r = np.Read(readBuffer, MSG_SIZE);
		if (r != MSG_SIZE) {
			printf("Read invalid size: %d\n", r);
			break;
		}
		count++;
		if (count % 1000 == 0) {
			QueryPerformanceCounter(&end);
			uint64_t ticks = end.QuadPart - start.QuadPart;
			double seconds = (double)ticks / (double)freq.QuadPart;
			QueryPerformanceCounter(&start);
			uint64_t bytes = MSG_SIZE*2*1000.0;
			double numerator = (double)bytes;
			double rate = numerator / seconds;
			int K = count / 1000;
			printf("%dK transmissions\n", K);
			printf("Transmission rate: %f\n", rate/(1024*1024));
		}
	}

	return 0;
}