/**
 * LightOS Localization
 * Language Support header
 */

#ifndef LANGUAGE_SUPPORT_H
#define LANGUAGE_SUPPORT_H

// Language support functions
void language_support_init();
int language_support_add_language(unsigned int language);
int language_support_remove_language(unsigned int language);
int language_support_is_language_installed(unsigned int language);
int language_support_get_installed_languages(unsigned int* languages, unsigned int* count);
int language_support_set_default_language(unsigned int language);
unsigned int language_support_get_default_language();
int language_support_install_language_pack(unsigned int language);
int language_support_uninstall_language_pack(unsigned int language);
int language_support_update_language_pack(unsigned int language);
int language_support_get_language_pack_version(unsigned int language, char* version, unsigned int version_size);
int language_support_get_language_pack_size(unsigned int language, unsigned int* size);
int language_support_get_language_pack_author(unsigned int language, char* author, unsigned int author_size);
int language_support_get_language_pack_description(unsigned int language, char* description, unsigned int description_size);
int language_support_get_language_pack_license(unsigned int language, char* license, unsigned int license_size);
int language_support_get_language_pack_url(unsigned int language, char* url, unsigned int url_size);
int language_support_get_language_pack_date(unsigned int language, char* date, unsigned int date_size);
int language_support_get_language_pack_compatibility(unsigned int language, char* compatibility, unsigned int compatibility_size);
int language_support_get_language_pack_dependencies(unsigned int language, char* dependencies, unsigned int dependencies_size);
int language_support_get_language_pack_conflicts(unsigned int language, char* conflicts, unsigned int conflicts_size);
int language_support_get_language_pack_replaces(unsigned int language, char* replaces, unsigned int replaces_size);
int language_support_get_language_pack_provides(unsigned int language, char* provides, unsigned int provides_size);
int language_support_get_language_pack_suggests(unsigned int language, char* suggests, unsigned int suggests_size);
int language_support_get_language_pack_recommends(unsigned int language, char* recommends, unsigned int recommends_size);
int language_support_get_language_pack_enhances(unsigned int language, char* enhances, unsigned int enhances_size);
int language_support_get_language_pack_breaks(unsigned int language, char* breaks, unsigned int breaks_size);
int language_support_get_language_pack_pre_depends(unsigned int language, char* pre_depends, unsigned int pre_depends_size);
int language_support_get_language_pack_post_depends(unsigned int language, char* post_depends, unsigned int post_depends_size);
int language_support_get_language_pack_pre_install(unsigned int language, char* pre_install, unsigned int pre_install_size);
int language_support_get_language_pack_post_install(unsigned int language, char* post_install, unsigned int post_install_size);
int language_support_get_language_pack_pre_remove(unsigned int language, char* pre_remove, unsigned int pre_remove_size);
int language_support_get_language_pack_post_remove(unsigned int language, char* post_remove, unsigned int post_remove_size);
int language_support_get_language_pack_pre_upgrade(unsigned int language, char* pre_upgrade, unsigned int pre_upgrade_size);
int language_support_get_language_pack_post_upgrade(unsigned int language, char* post_upgrade, unsigned int post_upgrade_size);
int language_support_get_language_pack_pre_downgrade(unsigned int language, char* pre_downgrade, unsigned int pre_downgrade_size);
int language_support_get_language_pack_post_downgrade(unsigned int language, char* post_downgrade, unsigned int post_downgrade_size);
int language_support_get_language_pack_pre_configure(unsigned int language, char* pre_configure, unsigned int pre_configure_size);
int language_support_get_language_pack_post_configure(unsigned int language, char* post_configure, unsigned int post_configure_size);
int language_support_get_language_pack_pre_deconfigure(unsigned int language, char* pre_deconfigure, unsigned int pre_deconfigure_size);
int language_support_get_language_pack_post_deconfigure(unsigned int language, char* post_deconfigure, unsigned int post_deconfigure_size);
int language_support_get_language_pack_pre_purge(unsigned int language, char* pre_purge, unsigned int pre_purge_size);
int language_support_get_language_pack_post_purge(unsigned int language, char* post_purge, unsigned int post_purge_size);
int language_support_get_language_pack_pre_abort_install(unsigned int language, char* pre_abort_install, unsigned int pre_abort_install_size);
int language_support_get_language_pack_post_abort_install(unsigned int language, char* post_abort_install, unsigned int post_abort_install_size);
int language_support_get_language_pack_pre_abort_upgrade(unsigned int language, char* pre_abort_upgrade, unsigned int pre_abort_upgrade_size);
int language_support_get_language_pack_post_abort_upgrade(unsigned int language, char* post_abort_upgrade, unsigned int post_abort_upgrade_size);
int language_support_get_language_pack_pre_abort_remove(unsigned int language, char* pre_abort_remove, unsigned int pre_abort_remove_size);
int language_support_get_language_pack_post_abort_remove(unsigned int language, char* post_abort_remove, unsigned int post_abort_remove_size);
int language_support_get_language_pack_pre_abort_deconfigure(unsigned int language, char* pre_abort_deconfigure, unsigned int pre_abort_deconfigure_size);
int language_support_get_language_pack_post_abort_deconfigure(unsigned int language, char* post_abort_deconfigure, unsigned int post_abort_deconfigure_size);

#endif /* LANGUAGE_SUPPORT_H */
