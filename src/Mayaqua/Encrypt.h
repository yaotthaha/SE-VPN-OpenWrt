// SoftEther VPN Source Code - Developer Edition Master Branch
// Mayaqua Kernel
// © 2020 Nokia

// Encrypt.h
// Header of Encrypt.c

#ifndef	ENCRYPT_H
#define	ENCRYPT_H

#include "MayaType.h"
#ifdef linux
	#include <pthread.h>
#endif
#include <openssl/opensslv.h>

// Constant
#define	MIN_SIGN_HASH_SIZE		(15 + SHA1_SIZE)
#define	SIGN_HASH_SIZE			(MIN_SIGN_HASH_SIZE)

#define DES_KEY_SIZE				8			// DES key size
#define	DES_IV_SIZE					8			// DES IV size
#define DES_BLOCK_SIZE				8			// DES block size
#define DES3_KEY_SIZE				(8 * 3)		// 3DES key size
#define RSA_KEY_SIZE				1024		// RSA key size
#define DH_KEY_SIZE					128			// DH key size
#define	RSA_MIN_SIGN_HASH_SIZE		(15 + SHA1_HASH_SIZE)	// Minimum RSA hash size
#define	RSA_SIGN_HASH_SIZE			(RSA_MIN_SIGN_HASH_SIZE)	// RSA hash size
#define MD5_HASH_SIZE				16			// MD5 hash size
#define SHA1_HASH_SIZE				20			// SHA-1 hash size
#define SHA1_BLOCK_SIZE				64			// SHA-1 block size
#define HMAC_SHA1_96_KEY_SIZE		20			// HMAC-SHA-1-96 key size
#define HMAC_SHA1_96_HASH_SIZE		12			// HMAC-SHA-1-96 hash size
#define HMAC_SHA1_SIZE				(SHA1_HASH_SIZE)	// HMAC-SHA-1 hash size
#define	AES_IV_SIZE					16			// AES IV size
#define	AES_MAX_KEY_SIZE			32			// Maximum AES key size

// IANA definitions taken from IKEv1 Phase 1
#define SHA1_160						2
#define SHA2_256						4
#define SHA2_384						5
#define SHA2_512						6

// HMAC block size
#define	HMAC_BLOCK_SIZE					64
// The block size for sha-384 and sha-512 as defined by rfc4868
#define HMAC_BLOCK_SIZE_1024					128
#define HMAC_BLOCK_SIZE_MAX					512

#define DH_GROUP1_PRIME_768 \
	"FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD1" \
	"29024E088A67CC74020BBEA63B139B22514A08798E3404DD" \
	"EF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245" \
	"E485B576625E7EC6F44C42E9A63A3620FFFFFFFFFFFFFFFF"

#define DH_GROUP2_PRIME_1024 \
	"FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD1" \
	"29024E088A67CC74020BBEA63B139B22514A08798E3404DD" \
	"EF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245" \
	"E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7ED" \
	"EE386BFB5A899FA5AE9F24117C4B1FE649286651ECE65381" \
	"FFFFFFFFFFFFFFFF"

#define DH_GROUP5_PRIME_1536 \
	"FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD1" \
	"29024E088A67CC74020BBEA63B139B22514A08798E3404DD" \
	"EF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245" \
	"E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7ED" \
	"EE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3D" \
	"C2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F" \
	"83655D23DCA3AD961C62F356208552BB9ED529077096966D" \
	"670C354E4ABC9804F1746C08CA237327FFFFFFFFFFFFFFFF"

#define	DH_SIMPLE_160	"AEE7561459353C95DDA966AE1FD25D95CD46E935"

#define	DH_SET_2048 \
	"FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD1" \
	"29024E088A67CC74020BBEA63B139B22514A08798E3404DD" \
	"EF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245" \
	"E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7ED" \
	"EE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3D" \
	"C2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F" \
	"83655D23DCA3AD961C62F356208552BB9ED529077096966D" \
	"670C354E4ABC9804F1746C08CA18217C32905E462E36CE3B" \
	"E39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9" \
	"DE2BCBF6955817183995497CEA956AE515D2261898FA0510" \
	"15728E5A8AACAA68FFFFFFFFFFFFFFFF"

#define	DH_SET_3072	\
	"FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD1"\
	"29024E088A67CC74020BBEA63B139B22514A08798E3404DD"\
	"EF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245"\
	"E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7ED"\
	"EE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3D"\
	"C2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F"\
	"83655D23DCA3AD961C62F356208552BB9ED529077096966D"\
	"670C354E4ABC9804F1746C08CA18217C32905E462E36CE3B"\
	"E39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9"\
	"DE2BCBF6955817183995497CEA956AE515D2261898FA0510"\
	"15728E5A8AAAC42DAD33170D04507A33A85521ABDF1CBA64"\
	"ECFB850458DBEF0A8AEA71575D060C7DB3970F85A6E1E4C7"\
	"ABF5AE8CDB0933D71E8C94E04A25619DCEE3D2261AD2EE6B"\
	"F12FFA06D98A0864D87602733EC86A64521F2B18177B200C"\
	"BBE117577A615D6C770988C0BAD946E208E24FA074E5AB31"\
	"43DB5BFCE0FD108E4B82D120A93AD2CAFFFFFFFFFFFFFFFF"

#define	DH_SET_4096 \
	"FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD1" \
	"29024E088A67CC74020BBEA63B139B22514A08798E3404DD" \
	"EF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245" \
	"E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7ED" \
	"EE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3D" \
	"C2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F" \
	"83655D23DCA3AD961C62F356208552BB9ED529077096966D" \
	"670C354E4ABC9804F1746C08CA18217C32905E462E36CE3B" \
	"E39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9" \
	"DE2BCBF6955817183995497CEA956AE515D2261898FA0510" \
	"15728E5A8AAAC42DAD33170D04507A33A85521ABDF1CBA64" \
	"ECFB850458DBEF0A8AEA71575D060C7DB3970F85A6E1E4C7" \
	"ABF5AE8CDB0933D71E8C94E04A25619DCEE3D2261AD2EE6B" \
	"F12FFA06D98A0864D87602733EC86A64521F2B18177B200C" \
	"BBE117577A615D6C770988C0BAD946E208E24FA074E5AB31" \
	"43DB5BFCE0FD108E4B82D120A92108011A723C12A787E6D7" \
	"88719A10BDBA5B2699C327186AF4E23C1A946834B6150BDA" \
	"2583E9CA2AD44CE8DBBBC2DB04DE8EF92E8EFC141FBECAA6" \
	"287C59474E6BC05D99B2964FA090C3A2233BA186515BE7ED" \
	"1F612970CEE2D7AFB81BDD762170481CD0069127D5B05AA9" \
	"93B4EA988D8FDDC186FFB7DC90A6C08F4DF435C934063199" \
	"FFFFFFFFFFFFFFFF"

// Macro
#define	HASHED_DATA(p)			(((UCHAR *)p) + 15)

#if OPENSSL_VERSION_NUMBER >= 0x10100000L && !defined(LIBRESSL_VERSION_NUMBER)
typedef struct PKCS12_st PKCS12;
typedef struct evp_md_st EVP_MD;
#else
#include <openssl/pkcs12.h>
typedef struct env_md_st EVP_MD;
#endif

typedef struct aes_key_st AES_KEY;
typedef struct bignum_st BIGNUM;
typedef struct bio_st BIO;
typedef struct DES_ks DES_key_schedule;
typedef struct dh_st DH;
typedef struct evp_cipher_st EVP_CIPHER;
typedef struct evp_cipher_ctx_st EVP_CIPHER_CTX;
typedef struct evp_pkey_st EVP_PKEY;
typedef struct rc4_key_st RC4_KEY;
typedef struct ssl_st SSL;
typedef struct x509_st X509;
typedef struct X509_crl_st X509_CRL;

// Crypt context
struct CRYPT
{
	RC4_KEY *Rc4Key;
};

// Name in the certificate
struct NAME
{
	wchar_t *CommonName;		// CN
	wchar_t *Organization;		// O
	wchar_t *Unit;				// OU
	wchar_t *Country;			// C
	wchar_t *State;				// ST
	wchar_t *Local;				// L
};

// Serial number
struct X_SERIAL
{
	UINT size;
	UCHAR *data;
};

// Certificate
struct X
{
	X509 *x509;
	NAME *issuer_name;
	NAME *subject_name;
	bool root_cert;
	UINT64 notBefore;
	UINT64 notAfter;
	X_SERIAL *serial;
	bool do_not_free;
	bool is_compatible_bit;
	UINT bits;
	bool has_basic_constraints;
	char issuer_url[256];
};

// Key
struct K
{
	EVP_PKEY *pkey;
	bool private_key;
};

// PKCS#12
struct P12
{
	PKCS12 *pkcs12;
};

// CEL
struct X_CRL
{
	X509_CRL *Crl;
};

// Constant
#define	MD5_SIZE	16
#define	SHA1_SIZE	20
#define	SHA256_SIZE	32
#define	SHA384_SIZE	48
#define	SHA512_SIZE	64

// Key element of DES
struct DES_KEY_VALUE
{
	DES_key_schedule *KeySchedule;
	UCHAR KeyValue[DES_KEY_SIZE];
};

// DES key
struct DES_KEY
{
	DES_KEY_VALUE *k1, *k2, *k3;
};

// AES key
struct AES_KEY_VALUE
{
	AES_KEY *EncryptKey;
	AES_KEY *DecryptKey;
	UCHAR KeyValue[AES_MAX_KEY_SIZE];
	UINT KeySize;
};

// DH
struct DH_CTX
{
	DH *dh;
	BUF *MyPublicKey;
	BUF *MyPrivateKey;
	UINT Size;
};

// Cipher object
struct CIPHER
{
	char Name[MAX_PATH];
	bool IsNullCipher, IsAeadCipher;
	const EVP_CIPHER *Cipher;
	EVP_CIPHER_CTX *Ctx;
	bool Encrypt;
	UINT BlockSize, IvSize, KeySize;
};

// Message digest object
struct MD
{
	char Name[MAX_PATH];
	bool IsNullMd;
	bool IsHMac;
	const EVP_MD *Md;
	void *Ctx;
	UINT Size;
};


// Lock of the OpenSSL
extern LOCK **ssl_lock_obj;

// Function prototype
CRYPT *NewCrypt(void *key, UINT size);
void FreeCrypt(CRYPT *c);
void Encrypt(CRYPT *c, void *dst, void *src, UINT size);
void InitCryptLibrary();
void Rand(void *buf, UINT size);
UINT HashToUINT(void *data, UINT size);
UINT64 Rand64();
UINT Rand32();
USHORT Rand16();
UCHAR Rand8();
bool Rand1();
UINT HashPtrToUINT(void *p);

BIO *BufToBio(BUF *b);
BUF *BioToBuf(BIO *bio);
BIO *NewBio();
void FreeBio(BIO *bio);
X *BioToX(BIO *bio, bool text);
LIST *BioToXList(BIO *bio, bool text);
X *BufToX(BUF *b, bool text);
LIST *BufToXList(BUF *b, bool text);
void FreeX509(X509 *x509);
void FreeX(X *x);
void FreeXList(LIST *chain);
BIO *XToBio(X *x, bool text);
BUF *XToBuf(X *x, bool text);
K *BioToK(BIO *bio, bool private_key, bool text, char *password);
int PKeyPasswordCallbackFunction(char *buf, int bufsize, int verify, void *param);
void FreePKey(EVP_PKEY *pkey);
void FreeK(K *k);
K *OpensslEngineToK(char *key_file_name, char *engine_name);
K *BufToK(BUF *b, bool private_key, bool text, char *password);
bool IsEncryptedK(BUF *b, bool private_key);
bool IsBase64(BUF *b);
BIO *KToBio(K *k, bool text, char *password);
BUF *KToBuf(K *k, bool text, char *password);
X *FileToX(char *filename);
X *FileToXW(wchar_t *filename);
bool XToFile(X *x, char *filename, bool text);
bool XToFileW(X *x, wchar_t *filename, bool text);
K *FileToKW(wchar_t *filename, bool private_key, char *password);
bool KToFileW(K *k, wchar_t *filename, bool text, char *password);
bool CheckXandK(X *x, K *k);
bool CompareX(X *x1, X *x2);
NAME *X509NameToName(void *xn);
wchar_t *GetUniStrFromX509Name(void *xn, int nid);
void LoadXNames(X *x);
void FreeXNames(X *x);
void FreeName(NAME *n);
bool CompareName(NAME *n1, NAME *n2);
K *GetKFromX(X *x);
bool CheckSignature(X *x, K *k);
X *X509ToX(X509 *x509);
bool CheckXEx(X *x, X *x_issuer, bool check_name, bool check_date);
bool Asn1TimeToSystem(SYSTEMTIME *s, void *asn1_time);
bool StrToSystem(SYSTEMTIME *s, char *str);
UINT64 Asn1TimeToUINT64(void *asn1_time);
bool SystemToAsn1Time(void *asn1_time, SYSTEMTIME *s);
bool UINT64ToAsn1Time(void *asn1_time, UINT64 t);
bool SystemToStr(char *str, UINT size, SYSTEMTIME *s);
void LoadXDates(X *x);
bool CheckXDate(X *x, UINT64 current_system_time);
bool CheckXDateNow(X *x);
NAME *NewName(wchar_t *common_name, wchar_t *organization, wchar_t *unit,
			  wchar_t *country, wchar_t *state, wchar_t *local);
void *NameToX509Name(NAME *nm);
void FreeX509Name(void *xn);
bool AddX509Name(void *xn, int nid, wchar_t *str);
X509 *NewRootX509(K *pub, K *priv, NAME *name, UINT days, X_SERIAL *serial);
X *NewRootX(K *pub, K *priv, NAME *name, UINT days, X_SERIAL *serial);
X509 *NewX509(K *pub, K *priv, X *ca, NAME *name, UINT days, X_SERIAL *serial);
X *NewX(K *pub, K *priv, X *ca, NAME *name, UINT days, X_SERIAL *serial);
UINT GetDaysUntil2038();
UINT GetDaysUntil2038Ex();
X_SERIAL *NewXSerial(void *data, UINT size);
void FreeXSerial(X_SERIAL *serial);
P12 *BioToP12(BIO *bio);
P12 *PKCS12ToP12(PKCS12 *pkcs12);
P12 *BufToP12(BUF *b);
BIO *P12ToBio(P12 *p12);
BUF *P12ToBuf(P12 *p12);
void FreePKCS12(PKCS12 *pkcs12);
void FreeP12(P12 *p12);
bool P12ToFileW(P12 *p12, wchar_t *filename);
bool ParseP12(P12 *p12, X **x, K **k, char *password);
bool ParseP12Ex(P12 *p12, X **x, K **k, LIST **cc, char *password);
bool IsEncryptedP12(P12 *p12);
P12 *NewP12(X *x, K *k, char *password);
X *CloneX(X *x);
LIST *CloneXList(LIST *chain);
K *CloneK(K *k);
void FreeCryptLibrary();
void GetPrintNameFromX(wchar_t *str, UINT size, X *x);
void GetPrintNameFromXA(char *str, UINT size, X *x);
void GetPrintNameFromName(wchar_t *str, UINT size, NAME *name);
void GetAllNameFromX(wchar_t *str, UINT size, X *x);
void GetAllNameFromName(wchar_t *str, UINT size, NAME *name);
void GetAllNameFromNameEx(wchar_t *str, UINT size, NAME *name);
BUF *BigNumToBuf(const BIGNUM *bn);
BIGNUM *BinToBigNum(void *data, UINT size);
X_SERIAL *CloneXSerial(X_SERIAL *src);
bool CompareXSerial(X_SERIAL *s1, X_SERIAL *s2);
void GetXDigest(X *x, UCHAR *buf, bool sha1);
NAME *CopyName(NAME *n);


bool RsaGen(K **priv, K **pub, UINT bit);
bool RsaCheck();
bool RsaCheckEx();
bool RsaSign(void *dst, void *src, UINT size, K *k);
bool RsaSignEx(void *dst, void *src, UINT size, K *k, UINT bits);
bool HashForSign(void *dst, UINT dst_size, void *src, UINT src_size);
bool RsaVerify(void *data, UINT data_size, void *sign, K *k);
bool RsaVerifyEx(void *data, UINT data_size, void *sign, K *k, UINT bits);
UINT RsaPublicSize(K *k);
BUF *RsaPublicToBuf(K *k);

DES_KEY_VALUE *DesNewKeyValue(void *value);
void DesFreeKeyValue(DES_KEY_VALUE *v);
void Des3Encrypt2(void *dest, void *src, UINT size, DES_KEY_VALUE *k1, DES_KEY_VALUE *k2, DES_KEY_VALUE *k3, void *ivec);
void Des3Decrypt2(void *dest, void *src, UINT size, DES_KEY_VALUE *k1, DES_KEY_VALUE *k2, DES_KEY_VALUE *k3, void *ivec);
void Sha(UINT sha_type, void *dst, void *src, UINT size);

void DesEncrypt(void *dest, void *src, UINT size, DES_KEY_VALUE *k, void *ivec);
void DesDecrypt(void *dest, void *src, UINT size, DES_KEY_VALUE *k, void *ivec);
void DesEcbEncrypt(void *dst, void *src, void *key_7bytes);

bool DhCompute(DH_CTX *dh, void *dst_priv_key, void *src_pub_key, UINT key_size);
DH_CTX *DhNewGroup1();
DH_CTX *DhNewGroup2();
DH_CTX *DhNewGroup5();
DH_CTX *DhNewSimple160();
DH_CTX *DhNew2048();
DH_CTX *DhNew3072();
DH_CTX *DhNew4096();
DH_CTX *DhNewFromBits(UINT bits);
DH_CTX *DhNew(char *prime, UINT g);
void DhFree(DH_CTX *dh);

AES_KEY_VALUE *AesNewKey(void *data, UINT size);
void AesFreeKey(AES_KEY_VALUE *k);
void AesEncrypt(void *dest, void *src, UINT size, AES_KEY_VALUE *k, void *ivec);
void AesDecrypt(void *dest, void *src, UINT size, AES_KEY_VALUE *k, void *ivec);

bool IsAesNiSupported();

void OpenSSL_InitLock();
void OpenSSL_FreeLock();
void OpenSSL_Lock(int mode, int n, const char *file, int line);
void FreeOpenSSLThreadState();
char *OpenSSL_Error();

// Encryption/Decryption
CIPHER *NewCipher(char *name);
void FreeCipher(CIPHER *c);
void SetCipherKey(CIPHER *c, void *key, bool enc);
UINT CipherProcess(CIPHER *c, void *iv, void *dest, void *src, UINT size);
UINT CipherProcessAead(CIPHER *c, void *iv, void *tag, UINT tag_size, void *dest, void *src, UINT src_size, void *aad, UINT aad_size);

// Hashing
MD *NewMd(char *name);
MD *NewMdEx(char *name, bool hmac);
bool SetMdKey(MD *md, void *key, UINT key_size);
UINT MdProcess(MD *md, void *dest, void *src, UINT size);
void FreeMd(MD *md);
void HashMd4(void *dst, void *src, UINT size);
void HashSha1(void *dst, void *src, UINT size);
void Md5(void *dst, void *src, UINT size);
void Sha(UINT sha_type, void *dst, void *src, UINT size);
void Sha0(void *dst, void *src, UINT size);
void Sha1(void *dst, void *src, UINT size);
void Sha2_256(void *dst, void *src, UINT size);
void Sha2_384(void *dst, void *src, UINT size);
void Sha2_512(void *dst, void *src, UINT size);
UINT HMacSha1(void *dst, void *key, UINT key_size, void *data, UINT data_size);
UINT HMacMd5(void *dst, void *key, UINT key_size, void *data, UINT data_size);
void Enc_tls1_PRF(unsigned char *label, int label_len, const unsigned char *sec, int slen, unsigned char *out1, int olen);

int GetSslClientCertIndex();

#endif	// ENCRYPT_H
