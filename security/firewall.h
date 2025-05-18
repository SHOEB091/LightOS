/**
 * LightOS Security
 * Firewall header
 */

#ifndef FIREWALL_H
#define FIREWALL_H

// Firewall rule action
typedef enum {
    FIREWALL_ACTION_ALLOW,
    FIREWALL_ACTION_DENY,
    FIREWALL_ACTION_REJECT,
    FIREWALL_ACTION_LOG
} firewall_action_t;

// Firewall rule direction
typedef enum {
    FIREWALL_DIRECTION_IN,
    FIREWALL_DIRECTION_OUT,
    FIREWALL_DIRECTION_FORWARD
} firewall_direction_t;

// Firewall rule protocol
typedef enum {
    FIREWALL_PROTOCOL_ANY,
    FIREWALL_PROTOCOL_TCP,
    FIREWALL_PROTOCOL_UDP,
    FIREWALL_PROTOCOL_ICMP,
    FIREWALL_PROTOCOL_CUSTOM
} firewall_protocol_t;

// Firewall rule state
typedef enum {
    FIREWALL_STATE_ENABLED,
    FIREWALL_STATE_DISABLED
} firewall_state_t;

// Firewall rule structure
typedef struct {
    char id[64];
    char name[128];
    char description[256];
    firewall_action_t action;
    firewall_direction_t direction;
    firewall_protocol_t protocol;
    char protocol_name[32];
    char source_address[64];
    char source_mask[64];
    unsigned int source_port_start;
    unsigned int source_port_end;
    char destination_address[64];
    char destination_mask[64];
    unsigned int destination_port_start;
    unsigned int destination_port_end;
    firewall_state_t state;
    unsigned int priority;
    char* options;
    unsigned int option_count;
    void* private_data;
} firewall_rule_t;

// Firewall chain structure
typedef struct {
    char id[64];
    char name[128];
    char description[256];
    firewall_action_t default_action;
    firewall_state_t state;
    firewall_rule_t** rules;
    unsigned int rule_count;
    void* private_data;
} firewall_chain_t;

// Firewall functions
void firewall_init();
int firewall_add_chain(const char* name, const char* description, firewall_action_t default_action);
int firewall_remove_chain(const char* id);
int firewall_enable_chain(const char* id);
int firewall_disable_chain(const char* id);
firewall_chain_t* firewall_get_chain(const char* id);
int firewall_list_chains(firewall_chain_t** chains, unsigned int* count);
int firewall_add_rule(const char* chain_id, const char* name, const char* description, firewall_action_t action, firewall_direction_t direction, firewall_protocol_t protocol, const char* source_address, const char* source_mask, unsigned int source_port_start, unsigned int source_port_end, const char* destination_address, const char* destination_mask, unsigned int destination_port_start, unsigned int destination_port_end, unsigned int priority);
int firewall_remove_rule(const char* chain_id, const char* rule_id);
int firewall_enable_rule(const char* chain_id, const char* rule_id);
int firewall_disable_rule(const char* chain_id, const char* rule_id);
firewall_rule_t* firewall_get_rule(const char* chain_id, const char* rule_id);
int firewall_list_rules(const char* chain_id, firewall_rule_t** rules, unsigned int* count);
int firewall_set_rule_priority(const char* chain_id, const char* rule_id, unsigned int priority);
int firewall_get_rule_priority(const char* chain_id, const char* rule_id);
int firewall_add_rule_option(const char* chain_id, const char* rule_id, const char* option);
int firewall_remove_rule_option(const char* chain_id, const char* rule_id, const char* option);
int firewall_list_rule_options(const char* chain_id, const char* rule_id, char*** options, unsigned int* count);
int firewall_set_chain_default_action(const char* chain_id, firewall_action_t default_action);
int firewall_get_chain_default_action(const char* chain_id);
int firewall_check_packet(const char* source_address, unsigned int source_port, const char* destination_address, unsigned int destination_port, firewall_protocol_t protocol, firewall_direction_t direction);
int firewall_log_packet(const char* source_address, unsigned int source_port, const char* destination_address, unsigned int destination_port, firewall_protocol_t protocol, firewall_direction_t direction, firewall_action_t action);
int firewall_save_rules(const char* file);
int firewall_load_rules(const char* file);
int firewall_reset_rules();
int firewall_set_log_file(const char* file);
int firewall_get_log_file(char* file, unsigned int file_size);
int firewall_set_log_level(unsigned int level);
int firewall_get_log_level();
int firewall_set_state(firewall_state_t state);
int firewall_get_state();
int firewall_set_default_policy(firewall_action_t action);
int firewall_get_default_policy();
int firewall_add_port_forward(const char* external_address, unsigned int external_port, const char* internal_address, unsigned int internal_port, firewall_protocol_t protocol);
int firewall_remove_port_forward(const char* external_address, unsigned int external_port, firewall_protocol_t protocol);
int firewall_list_port_forwards(char*** forwards, unsigned int* count);
int firewall_add_nat_rule(const char* source_address, const char* source_mask, const char* destination_address, const char* destination_mask, const char* nat_address);
int firewall_remove_nat_rule(const char* source_address, const char* source_mask, const char* destination_address, const char* destination_mask);
int firewall_list_nat_rules(char*** rules, unsigned int* count);
int firewall_add_trusted_interface(const char* interface);
int firewall_remove_trusted_interface(const char* interface);
int firewall_list_trusted_interfaces(char*** interfaces, unsigned int* count);
int firewall_add_dmz_host(const char* address);
int firewall_remove_dmz_host();
int firewall_get_dmz_host(char* address, unsigned int address_size);
int firewall_set_connection_tracking(unsigned int enabled);
int firewall_get_connection_tracking();
int firewall_set_connection_limit(unsigned int limit);
int firewall_get_connection_limit();
int firewall_set_connection_timeout(unsigned int timeout);
int firewall_get_connection_timeout();
int firewall_get_connection_count();
int firewall_list_connections(char*** connections, unsigned int* count);
int firewall_flush_connections();
int firewall_create_config(const char* config_file);
int firewall_parse_config(const char* config_file);
int firewall_set_config(const char* key, const char* value);
int firewall_get_config(const char* key, char* value, unsigned int value_size);

#endif /* FIREWALL_H */
