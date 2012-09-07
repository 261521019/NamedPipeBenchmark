#include <cstdio>

#include "../NamedPipeBenchmark/NamedPipe.h"

int main(int argc, char** argv) {
	NamedPipe np;
	int32_t r;
	r = np.CreatePipeServer(SERVER_ACCESS, DEFAULT_PIPE_NAME);
	if (r != 0) {
		printf("Could not create pipe: %d\n", r);
		return -1;
	}
	printf("Created pipe server.\n");
	fflush(stdout);
	r = np.Connect();
	if (r != 0) {
		printf("Could not connect to pipe: %d\n", r);
		return -2;
	}
	printf("Entering transmission loop.\n");
	fflush(stdout);
	// 1MB
	void* readBuffer = malloc(MSG_SIZE);
	void* writeBuffer = malloc(MSG_SIZE);
	int32_t count = 0;
	while (true) {
		r = np.Read(readBuffer, MSG_SIZE);
		if (r != MSG_SIZE) {
			printf("Read invalid size: %d\n", r);
			break;
		}
		r = np.Write(writeBuffer, MSG_SIZE);
		if (r != MSG_SIZE) {
			printf("Write invalid size: %d\n", r);
			break;
		}
		count++;
		if (count % 1000 == 0) {
			int K = count / 1000;
			printf("%dK transmissions\n", K);
			fflush(stdout);
		}
	}
	fflush(stdout);
	return 0;
}