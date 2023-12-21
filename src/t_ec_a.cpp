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
	double total_ecadd_time = 0.0;

	// Choose ECC curve
	EC_GROUP *curve = EC_GROUP_new_by_curve_name(NID_secp256k1);

	// Initialization
	EC_POINT *p = EC_POINT_new(curve);
	EC_POINT *q = EC_POINT_new(curve);
	EC_POINT *r = EC_POINT_new(curve);
	const EC_POINT *generator = EC_GROUP_get0_generator(curve);	//Get the generator
	EC_POINT *gen = EC_POINT_dup(generator, curve);
	BIGNUM *bn1 = BN_new();
	BIGNUM *bn2 = BN_new();

	BN_CTX *ctx = BN_CTX_new();

	for (int i = 0; i < num_iterations; ++i) {
		
		printf("**********The %d-th time***************\n", i);


		// Generate point pairs
		BN_rand(bn1, 256, -1, 0);
		//cout << "bn1：" << BN_bn2hex(bn1) << endl; 

		EC_POINT_mul(curve, p, NULL, gen, bn1, NULL);
		//cout << "p_str：" << EC_POINT_point2hex(curve, p, POINT_CONVERSION_UNCOMPRESSED, NULL) << endl;

		BN_rand(bn2, 256, -1, 0);
		//cout << "bn2：" << BN_bn2hex(bn2) << endl; 

		EC_POINT_mul(curve, q, NULL, gen, bn2, NULL);
		//cout << "q_str：" << EC_POINT_point2hex(curve, q, POINT_CONVERSION_UNCOMPRESSED, NULL) << endl;


		// Calculate the time of point addition operation.
		start = clock();
		EC_POINT_add(curve, r, p, q, ctx);
		end = clock();

		//cout << "p + q：" << EC_POINT_point2hex(curve, r, POINT_CONVERSION_UNCOMPRESSED, NULL) << endl;

		cpu_time_used = ((double) (end - start)/ CLOCKS_PER_SEC)  * 1000;
		total_ecadd_time += cpu_time_used;
		printf("EC_POINT_add Time: %f ms\n\n", cpu_time_used);
	} 

	// Computer the average time
	double avg_ecadd_time = total_ecadd_time / num_iterations;
	printf("\nThe average time for ECC point addition operations : %f ms\n\n", avg_ecadd_time);

	// Free
	EC_POINT_free(p);
	EC_POINT_free(q);
	EC_POINT_free(r);
	EC_POINT_free(gen);
	EC_GROUP_free(curve);
	BN_free(bn1);
	BN_free(bn2);
	BN_CTX_free(ctx);

	return 0;
}
