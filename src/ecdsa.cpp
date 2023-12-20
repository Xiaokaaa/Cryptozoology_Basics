#include <stdio.h>
#include <iostream>
#include <string.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/objects.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <time.h>

using namespace std;

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

    EC_KEY *eckey = NULL;
    const BIGNUM *priv_key_bn = NULL;
    const EC_POINT *pub_key = NULL;
    const EC_GROUP *group = NULL;
    ECDSA_SIG *ecdsa_sig = NULL;
    unsigned char digest[SHA256_DIGEST_LENGTH];
    unsigned char *signature = NULL;
    const char* plaintext = "Message to be signed";
    int sig_len;
    clock_t start, end;
    double cpu_time_used;

	for (int i = 0; i < num_iterations; ++i) {
		
		printf("**********The %d-th time***************", i);
		
		// Creat the curve
		eckey = EC_KEY_new_by_curve_name(NID_secp256k1);
		if (eckey == NULL)
			handleErrors();

		// Key Generation
		if (!EC_KEY_generate_key(eckey))
			handleErrors();
			
		priv_key_bn = EC_KEY_get0_private_key(eckey);
		pub_key = EC_KEY_get0_public_key(eckey);
		char *pub_key_str = EC_POINT_point2hex(EC_KEY_get0_group(eckey), pub_key, POINT_CONVERSION_UNCOMPRESSED, NULL);//xy一起输出，‘04’开头表示非压缩格式
		cout << "priv_key：" << BN_bn2hex(priv_key_bn) << endl; 
		cout << "pub_key ：" << pub_key_str << endl;
		

		SHA256((unsigned char*)plaintext, strlen(plaintext), (unsigned char*)&digest);    

		start = clock();
		ecdsa_sig = ECDSA_do_sign((unsigned char*)&digest, SHA256_DIGEST_LENGTH, eckey);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
		printf("\nSignature generation time : %f ms\n", cpu_time_used);
		
		total_sign_time += cpu_time_used;
		
		if (ecdsa_sig == NULL)
			handleErrors();

		start = clock();
		int verify_status = ECDSA_do_verify((unsigned char*)&digest, SHA256_DIGEST_LENGTH, ecdsa_sig, eckey);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
		printf("\nSignature verification time : %f ms\n", cpu_time_used);
		
		total_verify_time += cpu_time_used;

		if (verify_status == -1)
			handleErrors();
		else if (verify_status == 0)
			printf("Signature NOT verified\n");
		else  // verify_status == 1
			printf("Signature verified\n");
	}
    // Computer the average time
    double avg_sign_time = total_sign_time / num_iterations;
    double avg_verify_time = total_verify_time / num_iterations;

    printf("\nAverage ECDSA Signature generation time : %f ms\n", avg_sign_time);
    printf("Average ECDSA Signature verification time : %f ms\n", avg_verify_time);

    return 0;
}
