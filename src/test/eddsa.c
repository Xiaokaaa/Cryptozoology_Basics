#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/pem.h>

void print_binary(unsigned char byte) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (byte >> i) & 1);
    }
}

void print_binary_signature(const unsigned char *signature, size_t signature_len) {
    for (size_t i = 0; i < signature_len; i++) {
        print_binary(signature[i]);
        printf(" ");  // 可选：在字节之间添加空格
    }
    printf("\n");
}

void bin_to_hex(const unsigned char *bin, size_t len, char **hex) {
    if (!bin || len == 0) {
        *hex = NULL;
        return;
    }
    *hex = (char *)malloc(len * 2 + 1);
    for (size_t i = 0; i < len; i++) {
        sprintf((*hex) + i * 2, "%02x", bin[i]);
    }
    (*hex)[len * 2] = '\0';
}

int generate_key(EVP_PKEY **keypair)
{
    EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, NULL);	//为ED25519创建一个新的公钥上下文
    if (!pctx) return -1;

    if (EVP_PKEY_keygen_init(pctx) <= 0) return -1;	//初始化密钥生成

    if (EVP_PKEY_keygen(pctx, keypair) <= 0) return -1;	//生成密钥

    EVP_PKEY_CTX_free(pctx);

    return 0;
}

int sign_message(EVP_PKEY *keypair, const unsigned char *message, size_t message_len, unsigned char **signature, size_t *signature_len)
{
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (!mdctx) return -1;

    if (EVP_DigestSignInit(mdctx, NULL, NULL, NULL, keypair) <= 0) return -1;	//初始化密钥生成

    if (EVP_DigestSign(mdctx, NULL, signature_len, message, message_len) <= 0) return -1;	//（第一次调用）来确定签名需要的大小

    *signature = (unsigned char *)malloc(*signature_len);
    if (!*signature) return -1;

    if (EVP_DigestSign(mdctx, *signature, signature_len, message, message_len) <= 0) return -1;	//（第二次调用）来生成实际的签名

    EVP_MD_CTX_free(mdctx);

    return 0;
}

int verify_signature(EVP_PKEY *keypair, const unsigned char *message, size_t message_len, unsigned char *signature, size_t signature_len)
{
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (!mdctx) return -1;

    if (EVP_DigestVerifyInit(mdctx, NULL, NULL, NULL, keypair) <= 0) return -1;	//初始化摘要验证上下文，设置要使用的公钥。

    int verification_result = EVP_DigestVerify(mdctx, signature, signature_len, message, message_len);	//验证消息的签名

    EVP_MD_CTX_free(mdctx);

    return verification_result;
}

int main()
{
    EVP_PKEY *keypair = NULL;
    unsigned char *signature = NULL;
    size_t signature_len;

    const unsigned char message[] = "Hello, EDDSA!";
    size_t message_len = strlen((char *)message);

    // Generate key
    if (generate_key(&keypair) != 0) {
        fprintf(stderr, "Error generating key pair.\n");
        return 1;
    }

    // Sign the message
    if (sign_message(keypair, message, message_len, &signature, &signature_len) != 0) {
        fprintf(stderr, "Error signing message.\n");
        return 1;
    }
    
    print_binary_signature(signature, signature_len);
    
    char *hex_signature = NULL;
    bin_to_hex(signature, signature_len, &hex_signature);
    if (hex_signature) {
        printf("Signature in HEX: %s\n", hex_signature);
        free(hex_signature);
    }
    
    // Verify the message
    int verify_res = verify_signature(keypair, message, message_len, signature, signature_len);
    if (verify_res == 1) {
        printf("Signature is valid!\n");
    } else if (verify_res == 0) {
        printf("Signature is invalid!\n");
    } else {
        fprintf(stderr, "Error verifying signature.\n");
        return 1;
    }

    EVP_PKEY_free(keypair);
    free(signature);

    return 0;
}

