/**
 * LightOS Security
 * Cryptography header
 */

#ifndef CRYPTO_H
#define CRYPTO_H

// Hash algorithm
typedef enum {
    HASH_ALGORITHM_MD5,
    HASH_ALGORITHM_SHA1,
    HASH_ALGORITHM_SHA256,
    HASH_ALGORITHM_SHA512,
    HASH_ALGORITHM_CUSTOM
} hash_algorithm_t;

// Encryption algorithm
typedef enum {
    ENCRYPTION_ALGORITHM_AES_128,
    ENCRYPTION_ALGORITHM_AES_256,
    ENCRYPTION_ALGORITHM_RSA_1024,
    ENCRYPTION_ALGORITHM_RSA_2048,
    ENCRYPTION_ALGORITHM_RSA_4096,
    ENCRYPTION_ALGORITHM_CUSTOM
} encryption_algorithm_t;

// Encryption mode
typedef enum {
    ENCRYPTION_MODE_ECB,
    ENCRYPTION_MODE_CBC,
    ENCRYPTION_MODE_CFB,
    ENCRYPTION_MODE_OFB,
    ENCRYPTION_MODE_CTR,
    ENCRYPTION_MODE_GCM,
    ENCRYPTION_MODE_CUSTOM
} encryption_mode_t;

// Key type
typedef enum {
    KEY_TYPE_SYMMETRIC,
    KEY_TYPE_PUBLIC,
    KEY_TYPE_PRIVATE,
    KEY_TYPE_CUSTOM
} key_type_t;

// Key structure
typedef struct {
    char id[64];
    char name[128];
    key_type_t type;
    encryption_algorithm_t algorithm;
    unsigned int size;
    char* data;
    unsigned int data_size;
    char creation_date[32];
    char expiration_date[32];
    unsigned int revoked;
    void* private_data;
} key_t;

// Certificate structure
typedef struct {
    char id[64];
    char name[128];
    char subject[256];
    char issuer[256];
    char serial_number[64];
    char* public_key;
    unsigned int public_key_size;
    char* signature;
    unsigned int signature_size;
    char valid_from[32];
    char valid_to[32];
    unsigned int revoked;
    void* private_data;
} certificate_t;

// Cryptography functions
void crypto_init();
int crypto_generate_key(key_type_t type, encryption_algorithm_t algorithm, unsigned int size, key_t* key);
int crypto_import_key(const char* data, unsigned int data_size, key_type_t type, key_t* key);
int crypto_export_key(const key_t* key, char* data, unsigned int* data_size);
int crypto_delete_key(const char* id);
key_t* crypto_get_key(const char* id);
int crypto_list_keys(key_t** keys, unsigned int* count);
int crypto_encrypt(const char* data, unsigned int data_size, const key_t* key, encryption_mode_t mode, const char* iv, char* encrypted_data, unsigned int* encrypted_data_size);
int crypto_decrypt(const char* encrypted_data, unsigned int encrypted_data_size, const key_t* key, encryption_mode_t mode, const char* iv, char* data, unsigned int* data_size);
int crypto_hash(const char* data, unsigned int data_size, hash_algorithm_t algorithm, char* hash, unsigned int* hash_size);
int crypto_hash_file(const char* file, hash_algorithm_t algorithm, char* hash, unsigned int* hash_size);
int crypto_sign(const char* data, unsigned int data_size, const key_t* private_key, char* signature, unsigned int* signature_size);
int crypto_verify(const char* data, unsigned int data_size, const char* signature, unsigned int signature_size, const key_t* public_key);
int crypto_generate_random(char* data, unsigned int data_size);
int crypto_generate_salt(char* salt, unsigned int salt_size);
int crypto_hash_password(const char* password, const char* salt, char* hash, unsigned int hash_size);
int crypto_verify_password(const char* password, const char* salt, const char* hash, unsigned int hash_size);
int crypto_generate_certificate(const char* subject, const char* issuer, const key_t* public_key, const key_t* private_key, certificate_t* certificate);
int crypto_import_certificate(const char* data, unsigned int data_size, certificate_t* certificate);
int crypto_export_certificate(const certificate_t* certificate, char* data, unsigned int* data_size);
int crypto_verify_certificate(const certificate_t* certificate, const key_t* public_key);
int crypto_delete_certificate(const char* id);
certificate_t* crypto_get_certificate(const char* id);
int crypto_list_certificates(certificate_t** certificates, unsigned int* count);
int crypto_encrypt_file(const char* input_file, const char* output_file, const key_t* key, encryption_mode_t mode, const char* iv);
int crypto_decrypt_file(const char* input_file, const char* output_file, const key_t* key, encryption_mode_t mode, const char* iv);
int crypto_sign_file(const char* file, const key_t* private_key, char* signature, unsigned int* signature_size);
int crypto_verify_file(const char* file, const char* signature, unsigned int signature_size, const key_t* public_key);
int crypto_set_key_store(const char* key_store);
int crypto_get_key_store(char* key_store, unsigned int key_store_size);
int crypto_set_certificate_store(const char* certificate_store);
int crypto_get_certificate_store(char* certificate_store, unsigned int certificate_store_size);
int crypto_set_random_seed(const char* seed, unsigned int seed_size);
int crypto_get_random_seed(char* seed, unsigned int* seed_size);
int crypto_set_entropy_source(const char* source);
int crypto_get_entropy_source(char* source, unsigned int source_size);
int crypto_set_hash_iterations(unsigned int iterations);
int crypto_get_hash_iterations();
int crypto_set_key_derivation_function(const char* function);
int crypto_get_key_derivation_function(char* function, unsigned int function_size);
int crypto_set_cipher_suite(const char* suite);
int crypto_get_cipher_suite(char* suite, unsigned int suite_size);
int crypto_set_tls_version(const char* version);
int crypto_get_tls_version(char* version, unsigned int version_size);
int crypto_set_ssl_options(unsigned int options);
int crypto_get_ssl_options();
int crypto_create_ssl_context(const char* certificate_file, const char* key_file, void** context);
int crypto_destroy_ssl_context(void* context);
int crypto_ssl_connect(void* context, const char* host, unsigned int port, void** connection);
int crypto_ssl_accept(void* context, void* socket, void** connection);
int crypto_ssl_read(void* connection, char* data, unsigned int* data_size);
int crypto_ssl_write(void* connection, const char* data, unsigned int data_size);
int crypto_ssl_close(void* connection);
int crypto_ssl_get_peer_certificate(void* connection, certificate_t* certificate);
int crypto_ssl_get_cipher(void* connection, char* cipher, unsigned int cipher_size);
int crypto_ssl_get_version(void* connection, char* version, unsigned int version_size);
int crypto_ssl_get_error(void* connection, char* error, unsigned int error_size);
int crypto_create_config(const char* config_file);
int crypto_parse_config(const char* config_file);
int crypto_set_config(const char* key, const char* value);
int crypto_get_config(const char* key, char* value, unsigned int value_size);

#endif /* CRYPTO_H */
