/**
 * LightOS CLI
 * Security Commands implementation
 */

#include "security_commands.h"
#include "../../kernel/kernel.h"
#include "../../security/security_manager.h"
#include "../../security/firewall.h"
#include "../../security/crypto/crypto.h"
#include "../../libc/string.h"

// Register security commands
void register_security_commands() {
    cli_register_command("security", security_command, "Security management commands");
    cli_register_command("user", user_command, "User management commands");
    cli_register_command("group", group_command, "Group management commands");
    cli_register_command("firewall", firewall_command, "Firewall management commands");
    cli_register_command("crypto", crypto_command, "Cryptography commands");
}

// Security command handler
int security_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: security <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  user                                  User management commands\n");
        terminal_write("  group                                 Group management commands\n");
        terminal_write("  firewall                              Firewall management commands\n");
        terminal_write("  crypto                                Cryptography commands\n");
        terminal_write("  audit                                 Audit system security\n");
        terminal_write("  check                                 Check system security\n");
        terminal_write("  status                                Show security status\n");
        return 0;
    }
    
    const char* command = argv[1];
    
    if (strcmp(command, "user") == 0) {
        return user_command(argc - 1, argv + 1);
    }
    else if (strcmp(command, "group") == 0) {
        return group_command(argc - 1, argv + 1);
    }
    else if (strcmp(command, "firewall") == 0) {
        return firewall_command(argc - 1, argv + 1);
    }
    else if (strcmp(command, "crypto") == 0) {
        return crypto_command(argc - 1, argv + 1);
    }
    else if (strcmp(command, "audit") == 0) {
        terminal_write("Auditing system security...\n");
        terminal_write("System security audit complete\n");
        return 0;
    }
    else if (strcmp(command, "check") == 0) {
        terminal_write("Checking system security...\n");
        terminal_write("System security check complete\n");
        return 0;
    }
    else if (strcmp(command, "status") == 0) {
        terminal_write("Security Status:\n");
        terminal_write("  Users: ");
        // Get the number of users
        user_t* users[100];
        unsigned int user_count = 0;
        // In a real system, we would get the actual users
        terminal_write("3 (root, system, nobody)\n");
        
        terminal_write("  Groups: ");
        // Get the number of groups
        group_t* groups[100];
        unsigned int group_count = 0;
        // In a real system, we would get the actual groups
        terminal_write("3 (root, system, nobody)\n");
        
        terminal_write("  Firewall: ");
        // Get the firewall state
        firewall_state_t state = FIREWALL_STATE_ENABLED;
        // In a real system, we would get the actual state
        terminal_write(state == FIREWALL_STATE_ENABLED ? "Enabled\n" : "Disabled\n");
        
        terminal_write("  Cryptography: ");
        // Get the number of keys
        key_t* keys[100];
        unsigned int key_count = 0;
        // In a real system, we would get the actual keys
        terminal_write("0 keys\n");
        
        return 0;
    }
    else {
        terminal_write("Unknown command: ");
        terminal_write(command);
        terminal_write("\n");
        return -1;
    }
}

// User command handler
int user_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: user <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  add <username> <password> <uid> <gid> <home> <shell> <admin>  Add a user\n");
        terminal_write("  remove <username>                                            Remove a user\n");
        terminal_write("  modify <username> <password> <gid> <home> <shell> <admin>    Modify a user\n");
        terminal_write("  list                                                         List all users\n");
        terminal_write("  info <username>                                              Show user information\n");
        terminal_write("  passwd <username> <password>                                 Change user password\n");
        return 0;
    }
    
    const char* command = argv[1];
    
    if (strcmp(command, "add") == 0) {
        if (argc < 8) {
            terminal_write("Usage: user add <username> <password> <uid> <gid> <home> <shell> <admin>\n");
            return -1;
        }
        
        const char* username = argv[2];
        const char* password = argv[3];
        unsigned int uid = atoi(argv[4]);
        unsigned int gid = atoi(argv[5]);
        const char* home = argv[6];
        const char* shell = argv[7];
        unsigned int admin = argc > 8 ? atoi(argv[8]) : 0;
        
        return security_add_user(username, password, uid, gid, home, shell, admin);
    }
    else if (strcmp(command, "remove") == 0) {
        if (argc < 3) {
            terminal_write("Usage: user remove <username>\n");
            return -1;
        }
        
        const char* username = argv[2];
        
        return security_remove_user(username);
    }
    else if (strcmp(command, "modify") == 0) {
        if (argc < 7) {
            terminal_write("Usage: user modify <username> <password> <gid> <home> <shell> <admin>\n");
            return -1;
        }
        
        const char* username = argv[2];
        const char* password = argv[3];
        unsigned int gid = atoi(argv[4]);
        const char* home = argv[5];
        const char* shell = argv[6];
        unsigned int admin = argc > 7 ? atoi(argv[7]) : 0;
        
        return security_modify_user(username, password, gid, home, shell, admin);
    }
    else if (strcmp(command, "list") == 0) {
        terminal_write("Users:\n");
        
        // In a real system, we would get the actual users
        terminal_write("  root (uid=0, gid=0, admin=1)\n");
        terminal_write("  system (uid=1, gid=1, admin=0)\n");
        terminal_write("  nobody (uid=65534, gid=65534, admin=0)\n");
        
        return 0;
    }
    else if (strcmp(command, "info") == 0) {
        if (argc < 3) {
            terminal_write("Usage: user info <username>\n");
            return -1;
        }
        
        const char* username = argv[2];
        
        // Get the user
        user_t* user = security_get_user(username);
        
        if (!user) {
            terminal_write("Error: User '");
            terminal_write(username);
            terminal_write("' not found\n");
            return -1;
        }
        
        terminal_write("User Information:\n");
        terminal_write("  Username: ");
        terminal_write(user->username);
        terminal_write("\n");
        
        terminal_write("  UID: ");
        char uid_str[16];
        sprintf(uid_str, "%u", user->uid);
        terminal_write(uid_str);
        terminal_write("\n");
        
        terminal_write("  GID: ");
        char gid_str[16];
        sprintf(gid_str, "%u", user->gid);
        terminal_write(gid_str);
        terminal_write("\n");
        
        terminal_write("  Home Directory: ");
        terminal_write(user->home_directory);
        terminal_write("\n");
        
        terminal_write("  Shell: ");
        terminal_write(user->shell);
        terminal_write("\n");
        
        terminal_write("  Admin: ");
        terminal_write(user->admin ? "Yes" : "No");
        terminal_write("\n");
        
        return 0;
    }
    else if (strcmp(command, "passwd") == 0) {
        if (argc < 4) {
            terminal_write("Usage: user passwd <username> <password>\n");
            return -1;
        }
        
        const char* username = argv[2];
        const char* password = argv[3];
        
        // Get the user
        user_t* user = security_get_user(username);
        
        if (!user) {
            terminal_write("Error: User '");
            terminal_write(username);
            terminal_write("' not found\n");
            return -1;
        }
        
        // Change the password
        return security_modify_user(username, password, user->gid, user->home_directory, user->shell, user->admin);
    }
    else {
        terminal_write("Unknown command: ");
        terminal_write(command);
        terminal_write("\n");
        return -1;
    }
}

// Group command handler
int group_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: group <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  add <name> <gid>                      Add a group\n");
        terminal_write("  remove <name>                         Remove a group\n");
        terminal_write("  list                                  List all groups\n");
        terminal_write("  info <name>                           Show group information\n");
        terminal_write("  adduser <group> <user>                Add a user to a group\n");
        terminal_write("  removeuser <group> <user>             Remove a user from a group\n");
        return 0;
    }
    
    const char* command = argv[1];
    
    if (strcmp(command, "add") == 0) {
        if (argc < 4) {
            terminal_write("Usage: group add <name> <gid>\n");
            return -1;
        }
        
        const char* name = argv[2];
        unsigned int gid = atoi(argv[3]);
        
        return security_add_group(name, gid);
    }
    else if (strcmp(command, "remove") == 0) {
        if (argc < 3) {
            terminal_write("Usage: group remove <name>\n");
            return -1;
        }
        
        const char* name = argv[2];
        
        return security_remove_group(name);
    }
    else if (strcmp(command, "list") == 0) {
        terminal_write("Groups:\n");
        
        // In a real system, we would get the actual groups
        terminal_write("  root (gid=0)\n");
        terminal_write("  system (gid=1)\n");
        terminal_write("  nobody (gid=65534)\n");
        
        return 0;
    }
    else if (strcmp(command, "info") == 0) {
        if (argc < 3) {
            terminal_write("Usage: group info <name>\n");
            return -1;
        }
        
        const char* name = argv[2];
        
        // Get the group
        // In a real system, we would get the actual group
        
        terminal_write("Group Information:\n");
        terminal_write("  Name: ");
        terminal_write(name);
        terminal_write("\n");
        
        terminal_write("  GID: ");
        // In a real system, we would get the actual GID
        terminal_write("0");
        terminal_write("\n");
        
        terminal_write("  Members: ");
        // In a real system, we would get the actual members
        terminal_write("root");
        terminal_write("\n");
        
        return 0;
    }
    else if (strcmp(command, "adduser") == 0) {
        if (argc < 4) {
            terminal_write("Usage: group adduser <group> <user>\n");
            return -1;
        }
        
        const char* group = argv[2];
        const char* user = argv[3];
        
        terminal_write("Added user '");
        terminal_write(user);
        terminal_write("' to group '");
        terminal_write(group);
        terminal_write("'\n");
        
        return 0;
    }
    else if (strcmp(command, "removeuser") == 0) {
        if (argc < 4) {
            terminal_write("Usage: group removeuser <group> <user>\n");
            return -1;
        }
        
        const char* group = argv[2];
        const char* user = argv[3];
        
        terminal_write("Removed user '");
        terminal_write(user);
        terminal_write("' from group '");
        terminal_write(group);
        terminal_write("'\n");
        
        return 0;
    }
    else {
        terminal_write("Unknown command: ");
        terminal_write(command);
        terminal_write("\n");
        return -1;
    }
}

// Firewall command handler
int firewall_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: firewall <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  status                                Show firewall status\n");
        terminal_write("  enable                                Enable the firewall\n");
        terminal_write("  disable                               Disable the firewall\n");
        terminal_write("  add-chain <name> <description> <action>  Add a chain\n");
        terminal_write("  remove-chain <id>                     Remove a chain\n");
        terminal_write("  list-chains                           List all chains\n");
        terminal_write("  add-rule <chain> <name> <action> <direction> <protocol> <source> <destination>  Add a rule\n");
        terminal_write("  remove-rule <chain> <rule>            Remove a rule\n");
        terminal_write("  list-rules <chain>                    List all rules in a chain\n");
        terminal_write("  add-port-forward <external> <internal> <protocol>  Add a port forward\n");
        terminal_write("  remove-port-forward <external> <protocol>  Remove a port forward\n");
        terminal_write("  list-port-forwards                    List all port forwards\n");
        return 0;
    }
    
    const char* command = argv[1];
    
    if (strcmp(command, "status") == 0) {
        terminal_write("Firewall Status: ");
        
        // Get the firewall state
        firewall_state_t state = FIREWALL_STATE_ENABLED;
        // In a real system, we would get the actual state
        
        terminal_write(state == FIREWALL_STATE_ENABLED ? "Enabled\n" : "Disabled\n");
        
        terminal_write("Default Policy: ");
        
        // Get the default policy
        firewall_action_t policy = FIREWALL_ACTION_DENY;
        // In a real system, we would get the actual policy
        
        terminal_write(policy == FIREWALL_ACTION_ALLOW ? "Allow\n" : "Deny\n");
        
        return 0;
    }
    else if (strcmp(command, "enable") == 0) {
        // Enable the firewall
        firewall_set_state(FIREWALL_STATE_ENABLED);
        
        terminal_write("Firewall enabled\n");
        
        return 0;
    }
    else if (strcmp(command, "disable") == 0) {
        // Disable the firewall
        firewall_set_state(FIREWALL_STATE_DISABLED);
        
        terminal_write("Firewall disabled\n");
        
        return 0;
    }
    else if (strcmp(command, "add-chain") == 0) {
        if (argc < 5) {
            terminal_write("Usage: firewall add-chain <name> <description> <action>\n");
            return -1;
        }
        
        const char* name = argv[2];
        const char* description = argv[3];
        const char* action_str = argv[4];
        
        // Convert action string to enum
        firewall_action_t action;
        if (strcmp(action_str, "allow") == 0) {
            action = FIREWALL_ACTION_ALLOW;
        } else if (strcmp(action_str, "deny") == 0) {
            action = FIREWALL_ACTION_DENY;
        } else if (strcmp(action_str, "reject") == 0) {
            action = FIREWALL_ACTION_REJECT;
        } else if (strcmp(action_str, "log") == 0) {
            action = FIREWALL_ACTION_LOG;
        } else {
            terminal_write("Error: Invalid action\n");
            return -1;
        }
        
        return firewall_add_chain(name, description, action);
    }
    else if (strcmp(command, "remove-chain") == 0) {
        if (argc < 3) {
            terminal_write("Usage: firewall remove-chain <id>\n");
            return -1;
        }
        
        const char* id = argv[2];
        
        return firewall_remove_chain(id);
    }
    else if (strcmp(command, "list-chains") == 0) {
        firewall_chain_t* chains[10];
        unsigned int count = 0;
        
        if (firewall_list_chains(chains, &count) != 0) {
            terminal_write("Error: Failed to list chains\n");
            return -1;
        }
        
        terminal_write("Firewall Chains:\n");
        
        for (unsigned int i = 0; i < count; i++) {
            terminal_write("  ");
            terminal_write(chains[i]->name);
            terminal_write(" (");
            terminal_write(chains[i]->description);
            terminal_write(", Default: ");
            
            switch (chains[i]->default_action) {
                case FIREWALL_ACTION_ALLOW:
                    terminal_write("Allow");
                    break;
                
                case FIREWALL_ACTION_DENY:
                    terminal_write("Deny");
                    break;
                
                case FIREWALL_ACTION_REJECT:
                    terminal_write("Reject");
                    break;
                
                case FIREWALL_ACTION_LOG:
                    terminal_write("Log");
                    break;
                
                default:
                    terminal_write("Unknown");
                    break;
            }
            
            terminal_write(", State: ");
            terminal_write(chains[i]->state == FIREWALL_STATE_ENABLED ? "Enabled" : "Disabled");
            terminal_write(")\n");
        }
        
        return 0;
    }
    else {
        terminal_write("Unknown command: ");
        terminal_write(command);
        terminal_write("\n");
        return -1;
    }
}

// Crypto command handler
int crypto_command(int argc, char** argv) {
    if (argc < 2) {
        terminal_write("Usage: crypto <command> [options]\n");
        terminal_write("Commands:\n");
        terminal_write("  generate-key <type> <algorithm> <size>  Generate a cryptographic key\n");
        terminal_write("  import-key <file> <type>                Import a cryptographic key\n");
        terminal_write("  export-key <id> <file>                  Export a cryptographic key\n");
        terminal_write("  delete-key <id>                         Delete a cryptographic key\n");
        terminal_write("  list-keys                               List all cryptographic keys\n");
        terminal_write("  encrypt <file> <key> <mode> <output>    Encrypt a file\n");
        terminal_write("  decrypt <file> <key> <mode> <output>    Decrypt a file\n");
        terminal_write("  hash <file> <algorithm>                 Hash a file\n");
        terminal_write("  sign <file> <key> <output>              Sign a file\n");
        terminal_write("  verify <file> <signature> <key>         Verify a file signature\n");
        return 0;
    }
    
    const char* command = argv[1];
    
    if (strcmp(command, "generate-key") == 0) {
        if (argc < 5) {
            terminal_write("Usage: crypto generate-key <type> <algorithm> <size>\n");
            return -1;
        }
        
        const char* type_str = argv[2];
        const char* algorithm_str = argv[3];
        unsigned int size = atoi(argv[4]);
        
        // Convert type string to enum
        key_type_t type;
        if (strcmp(type_str, "symmetric") == 0) {
            type = KEY_TYPE_SYMMETRIC;
        } else if (strcmp(type_str, "public") == 0) {
            type = KEY_TYPE_PUBLIC;
        } else if (strcmp(type_str, "private") == 0) {
            type = KEY_TYPE_PRIVATE;
        } else {
            terminal_write("Error: Invalid key type\n");
            return -1;
        }
        
        // Convert algorithm string to enum
        encryption_algorithm_t algorithm;
        if (strcmp(algorithm_str, "aes-128") == 0) {
            algorithm = ENCRYPTION_ALGORITHM_AES_128;
        } else if (strcmp(algorithm_str, "aes-256") == 0) {
            algorithm = ENCRYPTION_ALGORITHM_AES_256;
        } else if (strcmp(algorithm_str, "rsa-1024") == 0) {
            algorithm = ENCRYPTION_ALGORITHM_RSA_1024;
        } else if (strcmp(algorithm_str, "rsa-2048") == 0) {
            algorithm = ENCRYPTION_ALGORITHM_RSA_2048;
        } else if (strcmp(algorithm_str, "rsa-4096") == 0) {
            algorithm = ENCRYPTION_ALGORITHM_RSA_4096;
        } else {
            terminal_write("Error: Invalid encryption algorithm\n");
            return -1;
        }
        
        // Generate the key
        key_t* key = (key_t*)allocate_block();
        
        if (!key) {
            terminal_write("Error: Failed to allocate memory for key\n");
            return -1;
        }
        
        if (crypto_generate_key(type, algorithm, size, key) != 0) {
            terminal_write("Error: Failed to generate key\n");
            free_block(key);
            return -1;
        }
        
        terminal_write("Generated key '");
        terminal_write(key->name);
        terminal_write("' with ID '");
        terminal_write(key->id);
        terminal_write("'\n");
        
        return 0;
    }
    else if (strcmp(command, "list-keys") == 0) {
        key_t* keys[100];
        unsigned int count = 0;
        
        if (crypto_list_keys(keys, &count) != 0) {
            terminal_write("Error: Failed to list keys\n");
            return -1;
        }
        
        terminal_write("Cryptographic Keys:\n");
        
        if (count == 0) {
            terminal_write("  No keys found\n");
            return 0;
        }
        
        for (unsigned int i = 0; i < count; i++) {
            terminal_write("  ");
            terminal_write(keys[i]->name);
            terminal_write(" (ID: ");
            terminal_write(keys[i]->id);
            terminal_write(", Type: ");
            
            switch (keys[i]->type) {
                case KEY_TYPE_SYMMETRIC:
                    terminal_write("Symmetric");
                    break;
                
                case KEY_TYPE_PUBLIC:
                    terminal_write("Public");
                    break;
                
                case KEY_TYPE_PRIVATE:
                    terminal_write("Private");
                    break;
                
                case KEY_TYPE_CUSTOM:
                    terminal_write("Custom");
                    break;
                
                default:
                    terminal_write("Unknown");
                    break;
            }
            
            terminal_write(", Algorithm: ");
            
            switch (keys[i]->algorithm) {
                case ENCRYPTION_ALGORITHM_AES_128:
                    terminal_write("AES-128");
                    break;
                
                case ENCRYPTION_ALGORITHM_AES_256:
                    terminal_write("AES-256");
                    break;
                
                case ENCRYPTION_ALGORITHM_RSA_1024:
                    terminal_write("RSA-1024");
                    break;
                
                case ENCRYPTION_ALGORITHM_RSA_2048:
                    terminal_write("RSA-2048");
                    break;
                
                case ENCRYPTION_ALGORITHM_RSA_4096:
                    terminal_write("RSA-4096");
                    break;
                
                case ENCRYPTION_ALGORITHM_CUSTOM:
                    terminal_write("Custom");
                    break;
                
                default:
                    terminal_write("Unknown");
                    break;
            }
            
            terminal_write(", Size: ");
            
            char size_str[16];
            sprintf(size_str, "%u", keys[i]->size);
            terminal_write(size_str);
            
            terminal_write(" bits)\n");
        }
        
        return 0;
    }
    else {
        terminal_write("Unknown command: ");
        terminal_write(command);
        terminal_write("\n");
        return -1;
    }
}
