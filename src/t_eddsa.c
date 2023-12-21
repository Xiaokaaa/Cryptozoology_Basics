#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <time.h>
#include <openssl/err.h>
#include <openssl/pem.h>

void handleErrors(void)
{
	ERR_print_errors_fp(stderr);
	abort();
}

void printHex(const unsigned char *data, size_t len)
{
	for (size_t i = 0; i < len; ++i)
		printf("%02x", data[i]);
	printf("\n");
}

int main()
{
	int num_iterations = 100000; // total times
	double total_sign_time = 0.0;
	double total_verify_time = 0.0;

	EVP_PKEY *pkey = NULL;
	EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, NULL);
	unsigned char sig[64];
	size_t siglen;
	const char* msg = "Message to be signed";
	size_t msglen = strlen(msg);
	clock_t start, end;
	double cpu_time_used;


	for (int i = 0; i < num_iterations; ++i) {
		
		printf("**********The %d-th time***************\n", i);

		// Key Generation
		if (EVP_PKEY_keygen_init(pctx) <= 0)
			handleErrors();

		if (EVP_PKEY_keygen(pctx, &pkey) <= 0)
			handleErrors();
			
		// Output Public Key in Hex
		const unsigned char *pubkey_data;
		size_t pubkey_len;
		if (EVP_PKEY_get_raw_public_key(pkey, NULL, &pubkey_len) <= 0)
			handleErrors();

		pubkey_data = (unsigned char *)malloc(pubkey_len);
		if (EVP_PKEY_get_raw_public_key(pkey, (unsigned char *)pubkey_data, &pubkey_len) <= 0)
			handleErrors();

		printf("Public Key (Hex):\n");
		printHex(pubkey_data, pubkey_len);

		// Output Private Key in Hex
		const unsigned char *privkey_data;
		size_t privkey_len;
		if (EVP_PKEY_get_raw_private_key(pkey, NULL, &privkey_len) <= 0)
			handleErrors();

		privkey_data = (unsigned char *)malloc(privkey_len);
		if (EVP_PKEY_get_raw_private_key(pkey, (unsigned char *)privkey_data, &privkey_len) <= 0)
			handleErrors();

		printf("\nPrivate Key (Hex):\n");
		printHex(privkey_data, privkey_len);
		
		
		// Signature Generation
		EVP_MD_CTX* mdctx = EVP_MD_CTX_new();

		start = clock();
		if (EVP_DigestSignInit(mdctx, NULL, NULL, NULL, pkey) <= 0)
			handleErrors();

		if (EVP_DigestSign(mdctx, sig, &siglen, (unsigned char*)msg, msglen) <= 0)
			handleErrors();

		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
		printf("\nEdDSA Signature generation time : %f ms\n", cpu_time_used);
		
		total_sign_time += cpu_time_used;
		
		// Output Signture
		printHex(sig, siglen);


		start = clock();
		
		// Signature Verification
		if (EVP_DigestVerifyInit(mdctx, NULL, NULL, NULL, pkey) <= 0)
			handleErrors();

		if (EVP_DigestVerify(mdctx, sig, siglen, (unsigned char*)msg, msglen) <= 0)
			handleErrors();
		
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
		printf("\nEdDSA Signature verification time : %f ms\n", cpu_time_used);
		
		total_verify_time += cpu_time_used;
	}

	// Computer the average time
	double avg_sign_time = total_sign_time / num_iterations;
	double avg_verify_time = total_verify_time / num_iterations;

	printf("\nAverage EdDSA Signature generation time : %f ms\n", avg_sign_time);
	printf("Average EdDSA Signature verification time : %f ms\n", avg_verify_time);

	return 0;
}
