# Cryptozoology_Basics
- 密码学相关算法介绍：[Cryptozoology_Basics](Cryptozoology_Basics.md)
- 国家标准文件：[Standard_documents](Standard_documents.md)

# OpenSSL
- 官网：[[link]](https://www.openssl.org/)
- GitHub：[[link]](https://github.com/openssl)
- 中文手册：[[link]](https://www.openssl.net.cn/)
- 基于 OpenSSL 的密码学相关操作时延分析：[[src]](/src)，加载源文件，如果是 cpp 文件，则执行`g++ xxx.cpp -o xxx -lcrypto -lssl`生成可执行文件；如果是 c 文件，则执行`gcc xxx.cpp -o xxx -lcrypto -lssl`生成可执行文件。以下是已完成的时延测试代码：
    - SHA 256 512：[[code]](/src/t_sha.c)
    - ECC 点加：[[code]](/src/t_ec_a.cpp)
    - ECC 点乘：[[code]](/src/t_ec_m.cpp)
    - ECDSA SHA256 签名和验证：[[code]](/src/t_ecdsa.cpp)
    - Ed25519 SHA512 签名和验证：[[code]](/src/t_eddsa.c)
    - AES-CTR 256：[[code]](/src/t_aes.c)
