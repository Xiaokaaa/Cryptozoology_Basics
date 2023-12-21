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

    // 创建一个新的椭圆曲线组
    EC_GROUP *curve = EC_GROUP_new_by_curve_name(NID_secp256k1);

    // 创建并生成两个新的椭圆曲线点
    EC_POINT *p = EC_POINT_new(curve);
    EC_POINT *q = EC_POINT_new(curve);
    EC_POINT *r = EC_POINT_new(curve);
    const EC_POINT *generator = EC_GROUP_get0_generator(curve);	//获取生成元
    EC_POINT *gen = EC_POINT_dup(generator, curve);
    BIGNUM *bn1 = BN_new();
    BIGNUM *bn2 = BN_new();

    // 生成点 p,q
    BN_rand(bn1, 256, -1, 0);
    EC_POINT_mul(curve, p, NULL, gen, bn1, NULL);
    
    BN_rand(bn2, 256, -1, 0);
    
    
    // 计算标量乘操作的时间
    start = clock();
    EC_POINT_mul(curve, q, NULL, gen, bn2, NULL);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
    printf("EC_POINT_mul Time: %f ms\n", cpu_time_used);


    BN_CTX *ctx = BN_CTX_new();
    
    
    
    char *p_str = EC_POINT_point2hex(curve, p, POINT_CONVERSION_UNCOMPRESSED, NULL);
    cout << "p_str ：" << p_str << endl;
    
    char *q_str = EC_POINT_point2hex(curve, q, POINT_CONVERSION_UNCOMPRESSED, NULL);
    cout << "q_str ：" << q_str << endl;
    
    
    // 计算点加操作的时间
    start = clock();
    EC_POINT_add(curve, r, p, q, ctx);
    end = clock();
    
    char *r_str = EC_POINT_point2hex(curve, r, POINT_CONVERSION_UNCOMPRESSED, NULL);
    cout << "r_str ：" << r_str << endl;
    

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
    printf("EC_POINT_add Time: %f ms\n", cpu_time_used);

    EC_POINT_sub(curve, r, p, q, ctx);
    r_str = EC_POINT_point2hex(curve, r, POINT_CONVERSION_UNCOMPRESSED, NULL);
    cout << "r_str ：" << r_str << endl;
	
	
	
    // 计算标量乘操作的时间
    start = clock();
    EC_POINT_mul(curve, r, NULL, p, bn1, ctx);
    end = clock();
    
    r_str = EC_POINT_point2hex(curve, r, POINT_CONVERSION_UNCOMPRESSED, NULL);
    cout << "r_str ：" << r_str << endl;

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
    printf("EC_POINT_mul Time: %f ms\n", cpu_time_used);

    // 清理资源
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
