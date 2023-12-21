#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <time.h>

#define N 100000

void printHex(const unsigned char *data, size_t len)
{
	for (size_t i = 0; i < len; ++i)
		printf("%02X", data[i]);
	printf("\n");
}

int main()
{
    const char* message = "Hello! SHA";
    unsigned char hash1[SHA256_DIGEST_LENGTH];
    unsigned char hash2[SHA512_DIGEST_LENGTH];
    clock_t start, end;
    double cpu_time_used;
    double sum1 = 0.0;
    double sum2 = 0.0;
	
    for(int i = 0; i < N; i++)
    {
    	printf("**********The %d-th time***************\n", i);
    	
		// SHA-256
		start = clock();
		SHA256((unsigned char*)message, strlen(message), hash1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
		//printf("SHA256 operations time: %f ms\n", cpu_time_used);
		printHex(hash1, sizeof(hash1) / sizeof(hash1[0]));
		sum1 += cpu_time_used;

		// SHA-512
		start = clock();
		SHA512((unsigned char*)message, strlen(message), hash2);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
		//printf("SHA512 operations time: %f ms\n", cpu_time_used);
		printHex(hash2, sizeof(hash2) / sizeof(hash2[0]));
		sum2 += cpu_time_used;
	}
    
    printf("\n\nThe average time for SHA256 operations : %f ms\n\n", sum1/N);
    printf("\n\nThe average time for SHA512 operations : %f ms\n\n", sum2/N);

    return 0;
}
