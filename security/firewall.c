/**
 * LightOS Security
 * Firewall implementation
 */

#include "firewall.h"
#include "../kernel/kernel.h"
#include "../kernel/memory.h"
#include "../libc/string.h"
#include "../networking/network.h"

// Maximum number of chains
#define MAX_CHAINS 10

// Maximum number of rules per chain
#define MAX_RULES_PER_CHAIN 100

// Maximum number of port forwards
#define MAX_PORT_FORWARDS 100

// Maximum number of NAT rules
#define MAX_NAT_RULES 100

// Maximum number of trusted interfaces
#define MAX_TRUSTED_INTERFACES 10

// Chain array
static firewall_chain_t* chains[MAX_CHAINS];
static unsigned int chain_count = 0;

// Port forward array
static char* port_forwards[MAX_PORT_FORWARDS];
static unsigned int port_forward_count = 0;

// NAT rule array
static char* nat_rules[MAX_NAT_RULES];
static unsigned int nat_rule_count = 0;

// Trusted interface array
static char* trusted_interfaces[MAX_TRUSTED_INTERFACES];
static unsigned int trusted_interface_count = 0;

// DMZ host
static char dmz_host[64] = "";

// Configuration
static char log_file[256] = "/var/log/firewall.log";
static unsigned int log_level = 1;
static firewall_state_t firewall_state = FIREWALL_STATE_ENABLED;
static firewall_action_t default_policy = FIREWALL_ACTION_DENY;
static unsigned int connection_tracking = 1;
static unsigned int connection_limit = 10000;
static unsigned int connection_timeout = 3600;

// Initialize the firewall
void firewall_init() {
    terminal_write("Initializing firewall...\n");
    
    // Clear the chain array
    for (int i = 0; i < MAX_CHAINS; i++) {
        chains[i] = NULL;
    }
    
    chain_count = 0;
    
    // Clear the port forward array
    for (int i = 0; i < MAX_PORT_FORWARDS; i++) {
        port_forwards[i] = NULL;
    }
    
    port_forward_count = 0;
    
    // Clear the NAT rule array
    for (int i = 0; i < MAX_NAT_RULES; i++) {
        nat_rules[i] = NULL;
    }
    
    nat_rule_count = 0;
    
    // Clear the trusted interface array
    for (int i = 0; i < MAX_TRUSTED_INTERFACES; i++) {
        trusted_interfaces[i] = NULL;
    }
    
    trusted_interface_count = 0;
    
    // Parse the configuration file
    firewall_parse_config("/etc/firewall/firewall.conf");
    
    // Create default chains
    firewall_add_chain("INPUT", "Incoming traffic", FIREWALL_ACTION_DENY);
    firewall_add_chain("OUTPUT", "Outgoing traffic", FIREWALL_ACTION_ALLOW);
    firewall_add_chain("FORWARD", "Forwarded traffic", FIREWALL_ACTION_DENY);
    
    // Add default rules
    
    // Allow loopback traffic
    firewall_add_rule("INPUT", "Allow loopback", "Allow all traffic on the loopback interface", 
                     FIREWALL_ACTION_ALLOW, FIREWALL_DIRECTION_IN, FIREWALL_PROTOCOL_ANY, 
                     "127.0.0.1", "255.0.0.0", 0, 65535, "0.0.0.0", "0.0.0.0", 0, 65535, 0);
    
    // Allow established connections
    firewall_add_rule("INPUT", "Allow established", "Allow traffic from established connections", 
                     FIREWALL_ACTION_ALLOW, FIREWALL_DIRECTION_IN, FIREWALL_PROTOCOL_ANY, 
                     "0.0.0.0", "0.0.0.0", 0, 65535, "0.0.0.0", "0.0.0.0", 0, 65535, 1);
    
    // Allow SSH
    firewall_add_rule("INPUT", "Allow SSH", "Allow incoming SSH connections", 
                     FIREWALL_ACTION_ALLOW, FIREWALL_DIRECTION_IN, FIREWALL_PROTOCOL_TCP, 
                     "0.0.0.0", "0.0.0.0", 0, 65535, "0.0.0.0", "0.0.0.0", 22, 22, 2);
    
    terminal_write("Firewall initialized\n");
}

// Add a chain
int firewall_add_chain(const char* name, const char* description, firewall_action_t default_action) {
    if (!name) {
        terminal_write("Error: Chain name cannot be NULL\n");
        return -1;
    }
    
    // Check if a chain with the same name already exists
    for (unsigned int i = 0; i < chain_count; i++) {
        if (strcmp(chains[i]->name, name) == 0) {
            terminal_write("Error: Chain with name '");
            terminal_write(name);
            terminal_write("' already exists\n");
            return -1;
        }
    }
    
    // Check if we have room for another chain
    if (chain_count >= MAX_CHAINS) {
        terminal_write("Error: Maximum number of chains reached\n");
        return -1;
    }
    
    // Generate a unique ID
    char id[64];
    sprintf(id, "chain-%u", chain_count + 1);
    
    // Allocate memory for the chain
    firewall_chain_t* chain = (firewall_chain_t*)allocate_block();
    
    if (!chain) {
        terminal_write("Error: Failed to allocate memory for chain\n");
        return -1;
    }
    
    // Initialize the chain
    strncpy(chain->id, id, sizeof(chain->id) - 1);
    chain->id[sizeof(chain->id) - 1] = '\0';
    strncpy(chain->name, name, sizeof(chain->name) - 1);
    chain->name[sizeof(chain->name) - 1] = '\0';
    
    if (description) {
        strncpy(chain->description, description, sizeof(chain->description) - 1);
        chain->description[sizeof(chain->description) - 1] = '\0';
    } else {
        chain->description[0] = '\0';
    }
    
    chain->default_action = default_action;
    chain->state = FIREWALL_STATE_ENABLED;
    chain->rules = NULL;
    chain->rule_count = 0;
    chain->private_data = NULL;
    
    // Allocate memory for the rules array
    chain->rules = (firewall_rule_t**)allocate_blocks((MAX_RULES_PER_CHAIN * sizeof(firewall_rule_t*) + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    
    if (!chain->rules) {
        terminal_write("Error: Failed to allocate memory for rules array\n");
        free_block(chain);
        return -1;
    }
    
    // Clear the rules array
    for (int i = 0; i < MAX_RULES_PER_CHAIN; i++) {
        chain->rules[i] = NULL;
    }
    
    // Add the chain to the array
    chains[chain_count++] = chain;
    
    terminal_write("Added chain '");
    terminal_write(name);
    terminal_write("'\n");
    
    return 0;
}

// Remove a chain
int firewall_remove_chain(const char* id) {
    if (!id) {
        terminal_write("Error: Chain ID cannot be NULL\n");
        return -1;
    }
    
    // Find the chain
    int index = -1;
    for (unsigned int i = 0; i < chain_count; i++) {
        if (strcmp(chains[i]->id, id) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        terminal_write("Error: Chain '");
        terminal_write(id);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Check if this is a default chain
    if (strcmp(chains[index]->name, "INPUT") == 0 || 
        strcmp(chains[index]->name, "OUTPUT") == 0 || 
        strcmp(chains[index]->name, "FORWARD") == 0) {
        terminal_write("Error: Cannot remove default chain '");
        terminal_write(chains[index]->name);
        terminal_write("'\n");
        return -1;
    }
    
    terminal_write("Removing chain '");
    terminal_write(chains[index]->name);
    terminal_write("'...\n");
    
    // Free the chain's rules
    for (unsigned int i = 0; i < chains[index]->rule_count; i++) {
        if (chains[index]->rules[i]) {
            if (chains[index]->rules[i]->options) {
                free_blocks(chains[index]->rules[i]->options, (chains[index]->rules[i]->option_count * sizeof(char*) + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
            }
            
            if (chains[index]->rules[i]->private_data) {
                free_block(chains[index]->rules[i]->private_data);
            }
            
            free_block(chains[index]->rules[i]);
        }
    }
    
    // Free the rules array
    free_blocks(chains[index]->rules, (MAX_RULES_PER_CHAIN * sizeof(firewall_rule_t*) + MEMORY_BLOCK_SIZE - 1) / MEMORY_BLOCK_SIZE);
    
    // Free the chain's private data
    if (chains[index]->private_data) {
        free_block(chains[index]->private_data);
    }
    
    // Free the chain
    free_block(chains[index]);
    
    // Remove the chain from the array
    for (unsigned int i = index; i < chain_count - 1; i++) {
        chains[i] = chains[i + 1];
    }
    
    chains[--chain_count] = NULL;
    
    terminal_write("Chain removed\n");
    
    return 0;
}

// Enable a chain
int firewall_enable_chain(const char* id) {
    if (!id) {
        terminal_write("Error: Chain ID cannot be NULL\n");
        return -1;
    }
    
    // Find the chain
    firewall_chain_t* chain = firewall_get_chain(id);
    
    if (!chain) {
        terminal_write("Error: Chain '");
        terminal_write(id);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Enable the chain
    chain->state = FIREWALL_STATE_ENABLED;
    
    terminal_write("Chain '");
    terminal_write(chain->name);
    terminal_write("' enabled\n");
    
    return 0;
}

// Disable a chain
int firewall_disable_chain(const char* id) {
    if (!id) {
        terminal_write("Error: Chain ID cannot be NULL\n");
        return -1;
    }
    
    // Find the chain
    firewall_chain_t* chain = firewall_get_chain(id);
    
    if (!chain) {
        terminal_write("Error: Chain '");
        terminal_write(id);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Disable the chain
    chain->state = FIREWALL_STATE_DISABLED;
    
    terminal_write("Chain '");
    terminal_write(chain->name);
    terminal_write("' disabled\n");
    
    return 0;
}

// Get a chain by ID
firewall_chain_t* firewall_get_chain(const char* id) {
    if (!id) {
        return NULL;
    }
    
    for (unsigned int i = 0; i < chain_count; i++) {
        if (strcmp(chains[i]->id, id) == 0) {
            return chains[i];
        }
    }
    
    return NULL;
}

// List all chains
int firewall_list_chains(firewall_chain_t** chains_out, unsigned int* count) {
    if (!chains_out || !count) {
        terminal_write("Error: Chains and count cannot be NULL\n");
        return -1;
    }
    
    *chains_out = chains;
    *count = chain_count;
    
    return 0;
}

// Add a rule to a chain
int firewall_add_rule(const char* chain_id, const char* name, const char* description, firewall_action_t action, firewall_direction_t direction, firewall_protocol_t protocol, const char* source_address, const char* source_mask, unsigned int source_port_start, unsigned int source_port_end, const char* destination_address, const char* destination_mask, unsigned int destination_port_start, unsigned int destination_port_end, unsigned int priority) {
    if (!chain_id || !name) {
        terminal_write("Error: Chain ID and rule name cannot be NULL\n");
        return -1;
    }
    
    // Find the chain
    firewall_chain_t* chain = firewall_get_chain(chain_id);
    
    if (!chain) {
        terminal_write("Error: Chain '");
        terminal_write(chain_id);
        terminal_write("' not found\n");
        return -1;
    }
    
    // Check if a rule with the same name already exists in this chain
    for (unsigned int i = 0; i < chain->rule_count; i++) {
        if (strcmp(chain->rules[i]->name, name) == 0) {
            terminal_write("Error: Rule with name '");
            terminal_write(name);
            terminal_write("' already exists in chain '");
            terminal_write(chain->name);
            terminal_write("'\n");
            return -1;
        }
    }
    
    // Check if we have room for another rule
    if (chain->rule_count >= MAX_RULES_PER_CHAIN) {
        terminal_write("Error: Maximum number of rules reached for chain '");
        terminal_write(chain->name);
        terminal_write("'\n");
        return -1;
    }
    
    // Generate a unique ID
    char id[64];
    sprintf(id, "rule-%u", chain->rule_count + 1);
    
    // Allocate memory for the rule
    firewall_rule_t* rule = (firewall_rule_t*)allocate_block();
    
    if (!rule) {
        terminal_write("Error: Failed to allocate memory for rule\n");
        return -1;
    }
    
    // Initialize the rule
    strncpy(rule->id, id, sizeof(rule->id) - 1);
    rule->id[sizeof(rule->id) - 1] = '\0';
    strncpy(rule->name, name, sizeof(rule->name) - 1);
    rule->name[sizeof(rule->name) - 1] = '\0';
    
    if (description) {
        strncpy(rule->description, description, sizeof(rule->description) - 1);
        rule->description[sizeof(rule->description) - 1] = '\0';
    } else {
        rule->description[0] = '\0';
    }
    
    rule->action = action;
    rule->direction = direction;
    rule->protocol = protocol;
    
    // Set the protocol name
    switch (protocol) {
        case FIREWALL_PROTOCOL_ANY:
            strcpy(rule->protocol_name, "any");
            break;
        
        case FIREWALL_PROTOCOL_TCP:
            strcpy(rule->protocol_name, "tcp");
            break;
        
        case FIREWALL_PROTOCOL_UDP:
            strcpy(rule->protocol_name, "udp");
            break;
        
        case FIREWALL_PROTOCOL_ICMP:
            strcpy(rule->protocol_name, "icmp");
            break;
        
        case FIREWALL_PROTOCOL_CUSTOM:
            strcpy(rule->protocol_name, "custom");
            break;
        
        default:
            strcpy(rule->protocol_name, "unknown");
            break;
    }
    
    if (source_address) {
        strncpy(rule->source_address, source_address, sizeof(rule->source_address) - 1);
        rule->source_address[sizeof(rule->source_address) - 1] = '\0';
    } else {
        strcpy(rule->source_address, "0.0.0.0");
    }
    
    if (source_mask) {
        strncpy(rule->source_mask, source_mask, sizeof(rule->source_mask) - 1);
        rule->source_mask[sizeof(rule->source_mask) - 1] = '\0';
    } else {
        strcpy(rule->source_mask, "0.0.0.0");
    }
    
    rule->source_port_start = source_port_start;
    rule->source_port_end = source_port_end;
    
    if (destination_address) {
        strncpy(rule->destination_address, destination_address, sizeof(rule->destination_address) - 1);
        rule->destination_address[sizeof(rule->destination_address) - 1] = '\0';
    } else {
        strcpy(rule->destination_address, "0.0.0.0");
    }
    
    if (destination_mask) {
        strncpy(rule->destination_mask, destination_mask, sizeof(rule->destination_mask) - 1);
        rule->destination_mask[sizeof(rule->destination_mask) - 1] = '\0';
    } else {
        strcpy(rule->destination_mask, "0.0.0.0");
    }
    
    rule->destination_port_start = destination_port_start;
    rule->destination_port_end = destination_port_end;
    rule->state = FIREWALL_STATE_ENABLED;
    rule->priority = priority;
    rule->options = NULL;
    rule->option_count = 0;
    rule->private_data = NULL;
    
    // Add the rule to the chain
    chain->rules[chain->rule_count++] = rule;
    
    // Sort the rules by priority
    // In a real system, we would sort the rules by priority
    
    terminal_write("Added rule '");
    terminal_write(name);
    terminal_write("' to chain '");
    terminal_write(chain->name);
    terminal_write("'\n");
    
    return 0;
}
