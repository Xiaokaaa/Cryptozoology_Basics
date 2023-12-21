#include <stdio.h>
#include <iostream>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include <openssl/bn.h>
#include <time.h>

using namespace std;

int main() {
    
	clock_t start, end;
	double cpu_time_used;
	int num_iterations = 100000; // total times
	double total_ecm_time = 0.0;

	// Choose ECC curve
	EC_GROUP *curve = EC_GROUP_new_by_curve_name(NID_secp256k1);

	// Initialization
	EC_POINT *p = EC_POINT_new(curve);
	const EC_POINT *generator = EC_GROUP_get0_generator(curve);	//Get the generator
	EC_POINT *gen = EC_POINT_dup(generator, curve);
	BIGNUM *bn = BN_new();
	
	BN_CTX *ctx = BN_CTX_new();

	for (int i = 0; i < num_iterations; ++i) {
		
		printf("**********The %d-th time***************\n", i);

		// Generate point pairs
		BN_rand(bn, 256, -1, 0);
		//cout << "bn：" << BN_bn2hex(bn1) << endl; 
		
		start = clock();
		EC_POINT_mul(curve, p, NULL, gen, bn, NULL);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
		
		printf("\nPoint multiplication time: %f ms\n", cpu_time_used);
		total_ecm_time += cpu_time_used;
		
		//cout << "p_str：" << EC_POINT_point2hex(curve, p, POINT_CONVERSION_UNCOMPRESSED, NULL) << endl;

	} 

	// Computer the average time
	double avg_ecm_time = total_ecm_time / num_iterations;
	printf("\nThe average time for ECC point multiplication operations: %f ms\n\n", avg_ecm_time);

	// Free
	EC_POINT_free(p);
	EC_POINT_free(gen);
	EC_GROUP_free(curve);
	BN_free(bn);
	BN_CTX_free(ctx);

	return 0;
}

