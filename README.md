# Cryptozoology_Basics
- 密码学相关算法介绍：[Cryptozoology_Basics](Cryptozoology_Basics.md)
- 国家标准文件：[国家标准文件](国家标准文件.md)
- 基于 OpenSSL 的密码学相关操作时延分析：[[src]](/src)，加载源文件，如果是 cpp 文件，则执行`g++ xxx.cpp -o xxx -lcrypto -lssl`生成可执行文件；如果是 c 文件，则执行`gcc xxx.cpp -o xxx -lcrypto -lssl`生成可执行文件。
    - Ed25519 时延测试：[[eddsat.c]](/src/eddsat.c)
    - ECDSA SHA256 时延测试：[[ecdsa.cpp]](/src/ecdsa.cpp)