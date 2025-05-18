/**
 * LightOS Security
 * Security manager header
 */

#ifndef SECURITY_MANAGER_H
#define SECURITY_MANAGER_H

// User structure
typedef struct {
    unsigned int uid;
    unsigned int gid;
    char username[32];
    char password_hash[64];
    char home_directory[256];
    char shell[64];
    unsigned int admin;
} user_t;

// Group structure
typedef struct {
    unsigned int gid;
    char name[32];
    unsigned int* members;
    unsigned int member_count;
} group_t;

// Permission structure
typedef struct {
    unsigned int uid;
    unsigned int gid;
    unsigned int mode;
} permission_t;

// Security context structure
typedef struct {
    unsigned int uid;
    unsigned int gid;
    unsigned int* supplementary_gids;
    unsigned int supplementary_gid_count;
    unsigned int capabilities;
} security_context_t;

// Capability flags
#define CAP_CHOWN           0x00000001
#define CAP_DAC_OVERRIDE    0x00000002
#define CAP_DAC_READ_SEARCH 0x00000004
#define CAP_FOWNER          0x00000008
#define CAP_FSETID          0x00000010
#define CAP_KILL            0x00000020
#define CAP_SETGID          0x00000040
#define CAP_SETUID          0x00000080
#define CAP_SETPCAP         0x00000100
#define CAP_SYS_ADMIN       0x00000200
#define CAP_SYS_BOOT        0x00000400
#define CAP_SYS_CHROOT      0x00000800
#define CAP_SYS_MODULE      0x00001000
#define CAP_SYS_NICE        0x00002000
#define CAP_SYS_RESOURCE    0x00004000
#define CAP_SYS_TIME        0x00008000
#define CAP_NET_ADMIN       0x00010000
#define CAP_NET_BIND        0x00020000
#define CAP_NET_RAW         0x00040000
#define CAP_MKNOD           0x00080000
#define CAP_AUDIT_CONTROL   0x00100000
#define CAP_AUDIT_WRITE     0x00200000
#define CAP_ALL             0xFFFFFFFF

// Security manager functions
void security_manager_init();
int security_add_user(const char* username, const char* password, unsigned int uid, unsigned int gid, const char* home_directory, const char* shell, unsigned int admin);
int security_remove_user(const char* username);
int security_modify_user(const char* username, const char* password, unsigned int gid, const char* home_directory, const char* shell, unsigned int admin);
user_t* security_get_user(const char* username);
user_t* security_get_user_by_uid(unsigned int uid);
int security_add_group(const char* name, unsigned int gid);
int security_remove_group(const char* name);
int security_add_user_to_group(const char* username, const char* group_name);
int security_remove_user_from_group(const char* username, const char* group_name);
group_t* security_get_group(const char* name);
group_t* security_get_group_by_gid(unsigned int gid);
int security_authenticate(const char* username, const char* password);
security_context_t* security_create_context(unsigned int uid);
void security_free_context(security_context_t* context);
int security_check_permission(security_context_t* context, permission_t* permission, unsigned int access);
int security_set_file_permission(const char* path, unsigned int uid, unsigned int gid, unsigned int mode);
int security_get_file_permission(const char* path, permission_t* permission);
int security_check_file_permission(security_context_t* context, const char* path, unsigned int access);
int security_set_capability(security_context_t* context, unsigned int capability);
int security_clear_capability(security_context_t* context, unsigned int capability);
int security_check_capability(security_context_t* context, unsigned int capability);

#endif /* SECURITY_MANAGER_H */
