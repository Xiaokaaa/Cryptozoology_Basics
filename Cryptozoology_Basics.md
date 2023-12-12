- [算法](#算法)
  - [双线性映射e函数](#双线性映射e函数)
  - [RSA 算法](#rsa-算法)
  - [DSA 算法](#dsa-算法)
  - [Diffie-Hellman密钥交换](#diffie-hellman密钥交换)
  - [椭圆曲线密码体制(ECC)](#椭圆曲线密码体制ecc)
  - [ECDSA算法](#ecdsa算法)
  - [EdDSA 算法](#eddsa-算法)
  - [公钥证书](#公钥证书)
- [开发工具](#开发工具)
  - [OpenSSL](#openssl)
  - [MIRACL](#miracl)


## 算法
### 双线性映射e函数
对于一个质数双线性群可以由五元组 $(p, G_1, G_2, G_T, e)$ 来描述。五元组 $p$ 中是一个与给定安全常数 $λ$ 相关的大质数，均是阶为 $p$ 的乘法循环群，$e : G_{1} \times G_{2} \to G_{T}$ 为双线性映射，它满足以下3个条件：
>- 双线性（Bilinearity）
>  
>   对于任意的 $g \in G_1, h \in G_2, a, b \in \mathbb{Z}_p$，有 $e(g^a, h^b) = e(g, h)^{ab}$ ；
>   
>   或对任意 $R, S, T \in G_1, \quad e(R + S, T) = e(R, T) e(S, T), \quad e(R, S + T) = e(R, S) e(R, T)$；
>- 非退化性（Non-degeneracy）
>       
>   至少存在元素 $g \in G_1, h \in G_2$，满足 $e(g_1, g_2) \neq 1$；
>- 可计算性（Efficiency）
>   
>   对于任意的 $u \in G_1, v \in G_2$，存在有效算法可以高效地计算出 $e(u,v)$。

对任意 $S,T∈G_1$ 一些性质：
>- $e(S, \infty) = 1, \quad e(\infty, S) = 1$；
>- $e(S, -T) = e(-S, T) = e(S, T)^{-1}$；
>- $e(aS, bT) = e(S, T)^{ab}$；
>- $e(S, T) = e(T, S)$；
>- 对于所有 $R \in G_1$ 若 $e(S, R) = 1$，则 $S = \infty$。

参考连接：[https://www.math.uwaterloo.ca/~ajmeneze/publications/pairings.pdf
](https://www.math.uwaterloo.ca/~ajmeneze/publications/pairings.pdf)



### RSA 算法
RSA 算法是 R.Rivest、A.Shamir 和 L.Adleman 于 1977 年在美国麻省理工学院开发，于 1978 年首次公布。

它是一个基于数论的非对称(公开钥)密码体制，是一种分组密码体制。它的安全性是基于大整数素因子分解的困难性，而大整数因子分解问题是数学上的著名难题，至今没有有效的方法予以解决，因此可以确保RSA算法的安全性。RSA系统是公钥系统的最具有典型意义的方法，大多数使用公钥密码进行加密和数字签名的产品和标准使用的都是RSA算法。

RSA 算法是第一个既能用于数据加密也能用于数字签名的算法，因此它为公用网络上信息的加密和鉴别提供了一种基本的方法。它通常是先生成一对 RSA 密钥，其中之一是保密密钥，由用户保存；另一个为公开密钥，可对外公开，甚至可在网络服务器中注册，人们用公钥加密文件发送给个人，个人就可以用私钥解密接受。为提高保密强度，RSA 密钥至少为 500 位长，一般推荐使用 1024 位。


RSA算法的安全性基于数论中大素数分解的困难性，所以，RSA需采用足够大的整数。因子分解越困难，密码就越难以破译，加密强度就越高。

其密钥生成算法如下：

>- 选择两质数 $p, q$
>- 计算 $n = p \cdot q$
>- 计算 $n$ 的欧拉函数 $\Phi(n) = (p - 1)(q - 1)$
>- 选择整数 $e$，使 $e$ 与 $\Phi(n)$ 互质，且 $1 < e < \Phi(n)$
>- 计算 $d$，使 $d \cdot e \equiv 1 \mod \Phi(n)$
>- 其中，公钥 $KU = \{e, n\}$，私钥 $KR = \{d, n\}$

其加密/解密过程：

>- 利用 RSA 加密，首先需将明文数字化，取长度小于 $\log_2 n$ 位的数字作为明文块
>- 对于明文块 $M$ 和密文块 $C$ ，加/解密的形式如下：
>- 加密：$C = M^{e} \mod n$
>- 解密：$M = C^{d} \mod n = (M^{e})^{d} \mod n = M^{ed} \mod n$

其签名/验签过程：

>- 对于消息 $m$ 签名/验签的形式如下：
>- 签名：$S = H(m)^{d} \mod n$
>- 验签：$H(m) = S^{e} \mod n$


### DSA 算法
DSA（Digital Signature Algorithm，数字签名算法，用作数字签名标准的一部分），于 1991 年由 NIST 提出，它是另一种公开密钥算法，是 Schnorr 和 ElGammal 方式的变体，它不能用作加密，只用作数字签名。DSA 使用公开密钥，为接受者验证数据的完整性和数据发送者的身份。它也可用于由第三方去确定签名和所签数据的真实性。DSA 算法的安全性基于解离散对数的困难性，比 RSA 算法要快很多。 

签名/验签过程：

>- KeyGen：
>- $p$：512~1024 位的素数，64 的倍数
>- $q$：160 位，与 $p−1$ 互素的因子
>- $g$：$g = h^{\frac{p-1}{q}} \mod p$，其中 $ℎ$ 小于 $p−1$ 并且 $h^{\frac{p-1}{q}} \mod p>1$
>- $x$：$x < q$（私钥）
>- $y$：$y=g^{x} \mod p$ （公钥）

>- Sign：
>- 选一个随机数 $k$ ($k < q$)；
>- 签名 $r$：$r=(g^{k} \mod p) \mod q$；
>- 签名 $s$：$s=k^{-1}(H(m)+xr) \mod q$；
>- 签名结果：$(m,r,s)$。

>- Verify：
>- $w = s^{-1} \mod q$；
>- $u_1 = w \cdot H(m) \mod q$；
>- $u_2 = wr \mod q$；
>- $v = (g^{u_1}y^{u_2} \mod p) \mod q$；
>- 若 $v = r$ ，则签名被验证。

参考连接：[https://blog.csdn.net/aaqian1/article/details/89299520](https://blog.csdn.net/aaqian1/article/details/89299520)


### Diffie-Hellman密钥交换
DH 算法是 W.Diffie 和 M.Hellman 提出的基于有限域上计算离散对数的困难性的公钥算法，它实质是通信双方进行密钥协定的协议：一方使用私钥和另方的公钥得到一个第三方计算不出来的对称密钥。离散对数的研究现状表明：所使用的DH密钥至少需要1024位，才能保证有足够的中、长期安全。
步骤如下：
发送方和接收方设置相同的大素数 $n$ 和 $g$，这两个数不是保密的，他们可以通过非安全通道来协商这两个素数；
发送方选择一个大随机整数 $x$，计算 $X = g^{x} \mod n$，发送 $X$ 给接收者；
接收方选择一个大随机整数 $y$，计算 $Y = g^{y} \mod n$，发送 $Y$ 给发送方；
双方计算密钥：发送方密钥为 $k_1 = Y^{x} \mod n$，接收方密钥为 $k_2 = X^{y} \mod n$。其中$k_{1} = k_{2} = g^{xy} \mod n$。

**注**：DH 算法不能抵御中间人攻击，中间人可以伪造假的 $X$ 和 $Y$ 分别发送给双方来获取他们的秘密密钥。为了防止中间人攻击，通常会在 Diffie-Hellman 密钥交换之上使用一种称为公钥基础设施（PKI）的系统。在 PKI 中，每个参与者都有一个由可信任的第三方（如证书颁发机构）签名的公钥证书。


### 椭圆曲线密码体制(ECC)
1985 年，基于定义在椭圆曲线点群上的离散对数问题的难解性，N. Koblitz 和 V. Miller 分别独立提出了椭圆曲线密码体制。

为了用椭圆曲线构造密码系统，首先需要找到一个单向陷门函数，椭圆曲线上的数量乘就是这样的单向陷门函数。

 椭圆曲线的数量乘定义：设 $E$ 为域 $K$ 上的椭圆曲线，$G$ 为 $E$ 上的一点，这个点被一个正整数 $k$ 相乘的乘法定义为 $k$ 个 $G$ 相加，因而有

> $k \cdot G = G + G + \ldots + G$   (共有 $k$ 个 $G$)
<div align="left">
    <img src="image/ecc_add.gif" alt="ecc_add.gif" width="250" />
</div>

椭圆曲线离散对数问题：若存在椭圆曲线上的另一点 $N \neq G$，满足方程 $kG = N$。容易看出，给定 $k$ 和 $G$，计算 $N$ 相对容易。而给定 $N$ 和 $G$，计算 $k = \log_G N$ 相对困难。

椭圆曲线离散对数问题比有限域上的离散对数问题更难求解。对于有理点数有大素数因子的椭圆离散对数问题，目前还没有有效的攻击方法。

常用有限域上的椭圆曲线
> 椭圆曲线由以下参数组成 $T = (p, a, b, G, n, h)$：
> 
>- $p$：有限域中的大素数，长度一般 224 比特、256 比特、384 比特
>- $a$：整数，椭圆方程系数
>- $b$：整数，椭圆方程系数
>- $G$：椭圆曲线上某点，生成元
>- $n$：为一个素数，表示椭圆曲线的阶
>- $ℎ$：余因数
>
> 其中 $G$ 包含 $G_x$ 和 $G_y$ 一共2个参数，非压缩模式以 04 开始，压缩模式以 03 开始，实际使用中一般采用非压缩模式
> 
> 通过模数 $p$ 和系数 $a，b$ 构成椭圆曲线方程 $y^2=x^3+ax+b \mod p$
>
> TLS支持很多椭圆曲线，常用的有2个 $secp256r1$ 和 $secp284r1$: $secp256r1$ 大素数 $p$ 长度 256 比特（32字节）和 $secp384r1$ 大素数 $p$ 长度 384 比特（48字节）

参考链接
- OpenSSL 中文手册 · 公钥算法：[[link]](https://www.openssl.net.cn/docs/3.html)
- 哔哩哔哩 · RSA加密过程详解：[[link]](https://www.bilibili.com/video/BV1YQ4y1a7n1/?spm_id_from=333.999.0.0&vd_source=f17606480b273d73e7f15a69fe00985e)


### ECDSA算法
 椭圆曲线数字签名算法（ECDSA）依赖于曲线有限域上的循环群，属于离散对数问题，是 ElGamal 签名方案的变体，是使用椭圆曲线密码（ECC）对数字签名算法（DSA）的模拟。
签名步骤如下：
> KeyGen：
>- 选一条椭圆曲线 $E_{p}(a,b)$ 和基点 $G$；
>- 选择私钥 $q$ （$q < n$，$n$ 为该 $G$ 的阶），计算公钥 $Q = q \cdot G$。
> 
> Sign：
>- 选择一个随机数 $k(k < n)$；
>- 计算点 $P =k \cdot G = (x_{1},y_{1})$，令 $r = x_{1} \mod n$；
>- 计算消息 $m$ 的哈希值 $z = H(m)$；
>- 计算 $s=k^{−1}(z+rq) \mod n$，得到签名 $(r,s)$；
>- 如果 $r$ 或 $s$ 其中任意一个为 0 则重新选择随机数 $k$ 再次计算。
>
> Verify：
>- 计算 $\hat{z}=H(m)$；
>- 计算 $u_1=\hat{z}s^{−1} \mod n，u_2=rs^{−1} \mod n$；
>- 计算点 $(\hat{x_1},\hat{y_1})=u_1 G + u_2 Q$；
>- 验证等式 $\hat{r}=\hat{x_1} \mod n = r$，如果等式成立输出 1，否则输出 0。
> 
> 证明：
>
> $$ \begin{align*}
> (\hat{x_1},\hat{y_1}) &= u_1G + u_2Q \\
> &= \hat{z}s^{-1}G + rs^{-1}Q \\
> &= \hat{z}\left(k^{-1}(z+rq)\right)^{-1}G + r\left(k^{-1}(z+rq)\right)^{-1}Q \\
> &= k \cdot G \\
> &= (x_1,y_1)
> \end{align*} 
> $$

ECDSA算法在应用中存在的安全隐患
>- ECDSA 具有延展性，也是唯一的延展性，即：如果 $(r,s)$ 是合法签名，那么 $(r,n-s)$ 也是合法签名。
>
>   证明：
> 
> $$ \begin{align*}
> n−s &= k^{−1}(z+rq) \\ 
> k(n−s) &= (z+rq) \\
> k(n−s) \cdot G &= z \cdot G+rq \cdot G \\
> − ks \cdot G &= z \cdot G+r \cdot Q \\
> − P &= s^{−1}z \cdot G+s^{−1}r \cdot Q 
> \end{align*} $$
> 
>        所以，双方签名时，$s=min\{s,n−s\}$ 就可以避免延展攻击了。
> 
>- 重复使用随机数会导致私钥泄露，即：用相同私钥和 $k$ 对两个消息进行签名, 则任何人都可以通过两个签名值恢复出私钥。
>- 两个用户使用相同的 $k$ 分别对不同的消息进行签名, 则任一方可推算出对方的私钥
>- 相同私钥和 $k$ 同时用于 ECDSA 签名和 Schnorr 签名时, 任何人都能够恢复出私钥
>- 如果 $k$ 值泄露, 则任何知道该随机数值的人可以使用该随机数产生签名值恢复私钥

参考连接

哔哩哔哩 · 基础密码学系列课程：[[link]](https://www.bilibili.com/video/BV1g24y1E7J4/?p=3&spm_id_from=pageDriver)



### EdDSA 算法
EdDSA 签名机制是定义在 Edwards25519 曲线上的变种 Schnorr 签名, 其设计初衷是在不牺牲安全性的前提下提升签名/验签速度, 并同时解决前述的 ECDSA 在应用方面存在的一些问题（消除了由于 ECDSA 中重复使用随机数而导致密钥泄漏的风险、有效抵御了侧信道攻击）。

EdDSA 有 7 个参数：整数 $b > 10$ 是公钥的位长，一个具有 $2b$ 比特输出的加密哈希函数 $H$。

Ed25519 的基本参数记为 $PP=(q,F_q,c,d,B,n,H_1,H_2)$，其中 $q=2^{255−19}$ 为 $F_q$ 特征，参数 $c,d \in F_q$ 确定了爱德华曲线 $E_{c,d}:cx^2+y^2=1+dx^2y^2$；定义点 $B$ 为曲线的基点 $B \in E_{c,d}(F_q )$，素数 $n$ 表示基点 $B$ 的阶，满足 $nB = 0$ 且 $2^3 n=\# E_{c,d}$。
签名步骤如下：
> KeyGen：
>- 任意选取 $b'$ 位的随机字符串 $\alpha$ 作为私钥，计算 $H_1(\alpha)= (ℎ_0,ℎ_1,⋯,ℎ_{2b'−1})$，令 $a= (ℎ_0,ℎ_1,⋯,ℎ_{b'−1})，b= (ℎ_{b'},ℎ_{b'+1},⋯,ℎ_{2b'−1})$；
>- 利用 $a$ 计算整数 $x=(2^{b'−2}+ \textstyle \sum_{i=3}^{b'−3} 2^i \cdot ℎ_i) \mod n$ 作为签名辅助私钥;
>- 计算签名公钥 $A=x \cdot B$。
>
> Sign：
>- ①计算签名消息 $m$ 的哈希值 $e = H_2(m)$；
>- ②计算临时密钥 $r = H_2(b,e) \mod n$；
>- ③计算 $R = r \cdot B$ 若 $R$ 为无穷远点则返回 ①；
>- ④计算 $ℎ= H_2(R,A,e) \mod n$；
>- ⑤计算 $s=(r+ℎx) \mod n$，若 $s = 0$ 返回 ①；
>- ⑥签名者对消息 $m$ 的签名为 $\sigma = (R,s)$，发送签名至验证者。
> 
> Verify：
>- ①验证者计算 $ℎ= H_2(R,A,e) \mod n$；
>- ②验证者验证等式 $8s \cdot B= 8R+8ℎ \cdot A$ 是否成立，若成立，则签名 $\sigma = (R,s)$ 为消息 $m$ 的有效签名。

解决了 Schnorr 签名的缺点，对于不同消息临时密钥 $r$ 不同，肯定不能解方程求得私钥。**Edwards25519** 使用了 SHA-512 哈希函数（64 字节），提供大约 128 比特的安全强度 (与 $secp256k1$ 和 $secp256r1$ 安全强度一致)。

参考连接
- **EdDSA 提出**：D. J. Bernstein, N. Duif, T. Lange, P. Schwabe, and B.-Y. Yang, “High-speed high-security signatures,” in Proc. CHES (Lecture Notes in Computer Science), vol. 6917, B. Preneel and T. Takagi, Eds. Berlin, Germany: Springer, Oct. 2011, pp. 124–142. [[paper]](https://eprint.iacr.org/2011/368)
- Simon Josefsson and Ilari Liusvaara. 2017. Edwards-Curve Digital Signature
Algorithm (EdDSA). RFC 8032. [[paper]](https://doi.org/10.17487/RFC8032)
- **基于 C++ 的 EdDSA 开源代码**：O. Peters. Portable C Implementation of Ed25519, a High-Speed HighSecurity Public-Key Signature System. Accessed: 2020. [[link]](https://github.com/orlp/ed25519)
- 哔哩哔哩 · 数字签名与KZG承诺：[[link]](https://www.bilibili.com/video/BV17e411N7Jm/?spm_id_from=333.337.search-card.all.click&vd_source=f17606480b273d73e7f15a69fe00985e)
- 抗差分故障攻击的两方协同EdDSA签名方案：[[paper]](http://www.jos.org.cn/html/2023/2/6505.htm)

EdDSA 与 ECDSA 算法对比

- 签名阶段
  
> |  | ECDSA | EdDSA | 运算规模 |
> |:-----:|:-----:|:-----:|:-----:|
> | 点乘 | 1 | 1 | $n^2$ |
> | 乘法 | 2 | 1 |  |
> | 加法 | 1 | 1 |  |
> | 哈希 | 1 | 3 |  |
> | 模逆 | 1 | 0 | $9n^2$ |
> | 求模 | 1 | 3 | $n$ |

- 验证阶段
  
> |  | ECDSA | EdDSA |
> |:-----:|:-----:|:-----:|
> | 点乘 | 2 | 2 |
> | 乘法 | 2 | 0 |
> | 加法 | 1 | 1 |
> | 哈希 | 1 | 1 |
> | 模逆 | 1 | 0 |
> | 求模 | 3 | 1 |


EdDSA 签名机制优势

>- 在多种计算平台上都能达到较高的性能；
>- 签名过程中不需要唯一的随机数,能够避免随机数引发的安全问题；
>- 对于侧信道攻击等具有更好的免疫效果；
>- 公钥和签名值都较小 (Ed25519 公钥为 32 个字节, 签名值为 64 字节)；
>- 计算公式是完备 (Complete), 无需对不相信的点执行点的验证操作；
>- EdDSA 能抵抗碰撞, 底层哈希函数的碰撞不会破坏 EdDSA 签名机制 (PureEdDSA)。

参考连接：[https://chrislinn.ink/notes/crypto/e_dsa](https://chrislinn.ink/notes/crypto/e_dsa)


参考链接：
- 爱编程的大丙 · 常用密码技术：[[link]](https://subingwen.cn/golang/cryptology/#1-%E5%AF%86%E7%A0%81)

ECDH方案的具体实例是X25519，EdDSA方案的具体实例是Ed25519。 X22519使用Curve25519的x坐标，Ed25519使用与Curve25519双有理等价的曲线。


### 公钥证书
怎么样才能知道自己得到的公钥是否合法呢？可以将公钥当做消息，对它加上数字签名，像这样对公钥施加数字签名所得到的的就是公钥证书。


## 开发工具
### OpenSSL


### MIRACL
- 官方文档：[https://github.com/miracl/MIRACL](https://github.com/miracl/MIRACL)