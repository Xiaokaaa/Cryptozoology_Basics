#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <time.h>

#define AES_BLOCK_SIZE 32
#define N 1000000  // Number of executions

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

unsigned char *generateStringWithNumber(const char *base_string, int added_number) {

    unsigned char *new_string;

    // Calculate required memory size
    size_t base_len = strlen(base_string);
    size_t total_len = base_len + snprintf(NULL, 0, "%d", added_number) + 1;

    // Allocate enough memory
    new_string = (unsigned char *)malloc(total_len);

    if (new_string == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    strcat((char *)new_string, base_string);
    
    // Add a number to the end of the string
    char number_string[20];
    sprintf(number_string, "%d", added_number);
    strcat((char *)new_string, number_string);

    return new_string;
}


int main (void)
{
	clock_t start, end;
	double cpu_time_used;
	
	double total_encrypted_time = 0.0;
	double total_decrypted_time = 0.0;
	
	unsigned char key[EVP_MAX_KEY_LENGTH]; 
	unsigned char iv[EVP_MAX_IV_LENGTH]; 

	unsigned char ciphertext[1024];
	unsigned char decryptedtext[1024];
	int len;
	int ciphertext_len;
	int decryptedtext_len;

	const char *base_string = "What do you guess this decrypted text is? Hei~Hei~";

    
	for(int i = 0; i < N; ++i){
		printf("%s\n", generateStringWithNumber(base_string, i)); 

		unsigned char *plaintext = generateStringWithNumber(base_string, i);
		
		EVP_CIPHER_CTX *ctx;
		
		// Key and IV setup
		// A 256 bit key
		memset(key, 'k', 32);
		// A 128 bit IV
		memset(iv, 'i', 16);
		// Create and initialise the context 
		if(!(ctx = EVP_CIPHER_CTX_new())) 
			handleErrors();

		// Initialise the encryption operation
		if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, key, iv))
			handleErrors();
			
		start = clock();

		// Provide the message to be encrypted, and obtain the encrypted output
		if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, strlen (plaintext)))
			handleErrors();

		ciphertext_len = len;

		// Finalise the encryption
		if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) 
			handleErrors();

		ciphertext_len += len;

		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;

		printf("Ciphertext is:\n");
		BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

		printf("Encryption time : %f ms\n", cpu_time_used);
		
		total_encrypted_time += cpu_time_used;
		
		// Initialise the decryption operation
		if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, key, iv))
			handleErrors();

		start = clock();

		// Provide the message to be decrypted, and obtain the decrypted output
		if(1 != EVP_DecryptUpdate(ctx, decryptedtext, &len, ciphertext, ciphertext_len))
			handleErrors();

		decryptedtext_len = len;

		// Finalise the decryption
		if(1 != EVP_DecryptFinal_ex(ctx, decryptedtext + len, &len)) 
			handleErrors();

		decryptedtext_len += len;

		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;

		// Add a NULL terminator
		decryptedtext[decryptedtext_len] = '\0';

		printf("Decrypted text is:\n");
		printf("%s\n", decryptedtext);

		printf("Decryption time : %f ms\n\n", cpu_time_used);
		
		total_decrypted_time += cpu_time_used;	

		// Clean up
		EVP_CIPHER_CTX_free(ctx);		
	}
	
	printf("total_encrypted_time: %f ms\n\n", total_encrypted_time);
	printf("Average aes 256 encrypted time: %f ms\n\n", total_encrypted_time/N);
	printf("total_decrypted_time: %f ms\n\n", total_decrypted_time);
	printf("Average aes 256 decrypted time: %f ms\n\n", total_decrypted_time/N);
	
	printf("Average aes 256 encrypted and decrypted time: %f ms\n\n", (total_encrypted_time + total_decrypted_time)/N);
	

    return 0;
}
