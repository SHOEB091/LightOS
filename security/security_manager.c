/**
 * LightOS Security
 * Security manager implementation
 */

#include "security_manager.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../kernel/filesystem.h"
#include "../libc/string.h"

// Maximum number of users
#define MAX_USERS 64

// Maximum number of groups
#define MAX_GROUPS 32

// User database
static user_t* users = NULL;
static int user_count = 0;

// Group database
static group_t* groups = NULL;
static int group_count = 0;

// User database file
#define USER_DB_FILE "/etc/passwd"

// Group database file
#define GROUP_DB_FILE "/etc/group"

// Shadow password file
#define SHADOW_FILE "/etc/shadow"

// Initialize the security manager
void security_manager_init() {
    terminal_write("Initializing security manager...\n");
    
    // Allocate memory for user database
    users = (user_t*)allocate_blocks((MAX_USERS * sizeof(user_t) + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    
    if (!users) {
        terminal_write("Error: Failed to allocate memory for user database\n");
        return;
    }
    
    // Clear the user database
    memset(users, 0, MAX_USERS * sizeof(user_t));
    user_count = 0;
    
    // Allocate memory for group database
    groups = (group_t*)allocate_blocks((MAX_GROUPS * sizeof(group_t) + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    
    if (!groups) {
        terminal_write("Error: Failed to allocate memory for group database\n");
        free_blocks(users, (MAX_USERS * sizeof(user_t) + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
        users = NULL;
        return;
    }
    
    // Clear the group database
    memset(groups, 0, MAX_GROUPS * sizeof(group_t));
    group_count = 0;
    
    // Load user database
    security_load_users();
    
    // Load group database
    security_load_groups();
    
    terminal_write("Security manager initialized\n");
    terminal_write_color("Found ", VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    
    // Convert user_count to string
    char count_str[16];
    int temp = user_count;
    int i = 0;
    
    if (temp == 0) {
        count_str[i++] = '0';
    } else {
        while (temp > 0) {
            count_str[i++] = '0' + (temp % 10);
            temp /= 10;
        }
    }
    
    count_str[i] = '\0';
    
    // Reverse the string
    for (int j = 0; j < i / 2; j++) {
        char tmp = count_str[j];
        count_str[j] = count_str[i - j - 1];
        count_str[i - j - 1] = tmp;
    }
    
    terminal_write(count_str);
    terminal_write(" users and ");
    
    // Convert group_count to string
    temp = group_count;
    i = 0;
    
    if (temp == 0) {
        count_str[i++] = '0';
    } else {
        while (temp > 0) {
            count_str[i++] = '0' + (temp % 10);
            temp /= 10;
        }
    }
    
    count_str[i] = '\0';
    
    // Reverse the string
    for (int j = 0; j < i / 2; j++) {
        char tmp = count_str[j];
        count_str[j] = count_str[i - j - 1];
        count_str[i - j - 1] = tmp;
    }
    
    terminal_write(count_str);
    terminal_write(" groups\n");
}

// Load user database
static int security_load_users() {
    // In a real system, we would:
    // 1. Open the user database file
    // 2. Read the user data
    // 3. Parse the data into the user database
    
    // For now, just add some sample users
    if (user_count < MAX_USERS) {
        users[user_count].uid = 0;
        users[user_count].gid = 0;
        strcpy(users[user_count].username, "root");
        strcpy(users[user_count].password_hash, "x"); // Password stored in shadow file
        strcpy(users[user_count].home_directory, "/root");
        strcpy(users[user_count].shell, "/bin/bash");
        users[user_count].admin = 1;
        user_count++;
    }
    
    if (user_count < MAX_USERS) {
        users[user_count].uid = 1000;
        users[user_count].gid = 1000;
        strcpy(users[user_count].username, "user");
        strcpy(users[user_count].password_hash, "x"); // Password stored in shadow file
        strcpy(users[user_count].home_directory, "/home/user");
        strcpy(users[user_count].shell, "/bin/bash");
        users[user_count].admin = 0;
        user_count++;
    }
    
    return 0;
}

// Load group database
static int security_load_groups() {
    // In a real system, we would:
    // 1. Open the group database file
    // 2. Read the group data
    // 3. Parse the data into the group database
    
    // For now, just add some sample groups
    if (group_count < MAX_GROUPS) {
        groups[group_count].gid = 0;
        strcpy(groups[group_count].name, "root");
        groups[group_count].members = (unsigned int*)allocate_block();
        
        if (groups[group_count].members) {
            groups[group_count].members[0] = 0; // root user
            groups[group_count].member_count = 1;
        } else {
            groups[group_count].member_count = 0;
        }
        
        group_count++;
    }
    
    if (group_count < MAX_GROUPS) {
        groups[group_count].gid = 1000;
        strcpy(groups[group_count].name, "user");
        groups[group_count].members = (unsigned int*)allocate_block();
        
        if (groups[group_count].members) {
            groups[group_count].members[0] = 1000; // user user
            groups[group_count].member_count = 1;
        } else {
            groups[group_count].member_count = 0;
        }
        
        group_count++;
    }
    
    return 0;
}

// Save user database
static int security_save_users() {
    // In a real system, we would:
    // 1. Open the user database file
    // 2. Write the user data
    
    return 0;
}

// Save group database
static int security_save_groups() {
    // In a real system, we would:
    // 1. Open the group database file
    // 2. Write the group data
    
    return 0;
}

// Find a user by username
static user_t* security_find_user(const char* username) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return &users[i];
        }
    }
    
    return NULL;
}

// Find a user by UID
static user_t* security_find_user_by_uid(unsigned int uid) {
    for (int i = 0; i < user_count; i++) {
        if (users[i].uid == uid) {
            return &users[i];
        }
    }
    
    return NULL;
}

// Find a group by name
static group_t* security_find_group(const char* name) {
    for (int i = 0; i < group_count; i++) {
        if (strcmp(groups[i].name, name) == 0) {
            return &groups[i];
        }
    }
    
    return NULL;
}

// Find a group by GID
static group_t* security_find_group_by_gid(unsigned int gid) {
    for (int i = 0; i < group_count; i++) {
        if (groups[i].gid == gid) {
            return &groups[i];
        }
    }
    
    return NULL;
}

// Add a user
int security_add_user(const char* username, const char* password, unsigned int uid, unsigned int gid, const char* home_directory, const char* shell, unsigned int admin) {
    if (!username || !password || !home_directory || !shell) {
        return -1;
    }
    
    // Check if the user already exists
    if (security_find_user(username)) {
        terminal_write("Error: User '");
        terminal_write(username);
        terminal_write("' already exists\n");
        return -1;
    }
    
    // Check if the UID is already in use
    if (security_find_user_by_uid(uid)) {
        terminal_write("Error: UID ");
        // Convert uid to string
        char uid_str[16];
        int temp = uid;
        int i = 0;
        
        if (temp == 0) {
            uid_str[i++] = '0';
        } else {
            while (temp > 0) {
                uid_str[i++] = '0' + (temp % 10);
                temp /= 10;
            }
        }
        
        uid_str[i] = '\0';
        
        // Reverse the string
        for (int j = 0; j < i / 2; j++) {
            char tmp = uid_str[j];
            uid_str[j] = uid_str[i - j - 1];
            uid_str[i - j - 1] = tmp;
        }
        
        terminal_write(uid_str);
        terminal_write(" is already in use\n");
        return -1;
    }
    
    // Check if the group exists
    if (!security_find_group_by_gid(gid)) {
        terminal_write("Error: Group with GID ");
        // Convert gid to string
        char gid_str[16];
        int temp = gid;
        int i = 0;
        
        if (temp == 0) {
            gid_str[i++] = '0';
        } else {
            while (temp > 0) {
                gid_str[i++] = '0' + (temp % 10);
                temp /= 10;
            }
        }
        
        gid_str[i] = '\0';
        
        // Reverse the string
        for (int j = 0; j < i / 2; j++) {
            char tmp = gid_str[j];
            gid_str[j] = gid_str[i - j - 1];
            gid_str[i - j - 1] = tmp;
        }
        
        terminal_write(gid_str);
        terminal_write(" does not exist\n");
        return -1;
    }
    
    // Check if we have room for another user
    if (user_count >= MAX_USERS) {
        terminal_write("Error: Maximum number of users reached\n");
        return -1;
    }
    
    // Add the user
    users[user_count].uid = uid;
    users[user_count].gid = gid;
    strcpy(users[user_count].username, username);
    
    // In a real system, we would hash the password
    strcpy(users[user_count].password_hash, password);
    
    strcpy(users[user_count].home_directory, home_directory);
    strcpy(users[user_count].shell, shell);
    users[user_count].admin = admin;
    
    user_count++;
    
    // Save the user database
    security_save_users();
    
    terminal_write("User '");
    terminal_write(username);
    terminal_write("' added successfully\n");
    
    return 0;
}

// Remove a user
int security_remove_user(const char* username) {
    if (!username) {
        return -1;
    }
    
    // Find the user
    int index = -1;
    
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        terminal_write("Error: User '");
        terminal_write(username);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Remove the user from all groups
    for (int i = 0; i < group_count; i++) {
        if (groups[i].members) {
            for (int j = 0; j < groups[i].member_count; j++) {
                if (groups[i].members[j] == users[index].uid) {
                    // Remove the user from the group
                    for (int k = j; k < groups[i].member_count - 1; k++) {
                        groups[i].members[k] = groups[i].members[k + 1];
                    }
                    
                    groups[i].member_count--;
                    break;
                }
            }
        }
    }
    
    // Remove the user
    for (int i = index; i < user_count - 1; i++) {
        users[i] = users[i + 1];
    }
    
    user_count--;
    
    // Save the user database
    security_save_users();
    
    // Save the group database
    security_save_groups();
    
    terminal_write("User '");
    terminal_write(username);
    terminal_write("' removed successfully\n");
    
    return 0;
}

// Modify a user
int security_modify_user(const char* username, const char* password, unsigned int gid, const char* home_directory, const char* shell, unsigned int admin) {
    if (!username) {
        return -1;
    }
    
    // Find the user
    user_t* user = security_find_user(username);
    
    if (!user) {
        terminal_write("Error: User '");
        terminal_write(username);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Check if the group exists
    if (gid != user->gid && !security_find_group_by_gid(gid)) {
        terminal_write("Error: Group with GID ");
        // Convert gid to string
        char gid_str[16];
        int temp = gid;
        int i = 0;
        
        if (temp == 0) {
            gid_str[i++] = '0';
        } else {
            while (temp > 0) {
                gid_str[i++] = '0' + (temp % 10);
                temp /= 10;
            }
        }
        
        gid_str[i] = '\0';
        
        // Reverse the string
        for (int j = 0; j < i / 2; j++) {
            char tmp = gid_str[j];
            gid_str[j] = gid_str[i - j - 1];
            gid_str[i - j - 1] = tmp;
        }
        
        terminal_write(gid_str);
        terminal_write(" does not exist\n");
        return -1;
    }
    
    // Update the user
    if (password) {
        // In a real system, we would hash the password
        strcpy(user->password_hash, password);
    }
    
    if (gid != user->gid) {
        user->gid = gid;
    }
    
    if (home_directory) {
        strcpy(user->home_directory, home_directory);
    }
    
    if (shell) {
        strcpy(user->shell, shell);
    }
    
    user->admin = admin;
    
    // Save the user database
    security_save_users();
    
    terminal_write("User '");
    terminal_write(username);
    terminal_write("' modified successfully\n");
    
    return 0;
}
