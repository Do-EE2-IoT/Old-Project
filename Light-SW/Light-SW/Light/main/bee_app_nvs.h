/*****************************************************************************
 *
 * @file 	bee_app_nvs.c
 * @author 	biennd
 * @date 	13 Jun 2023
 * @brief	All functions relate to init and exculate flash 
 *
 ***************************************************************************/
#ifndef __APP_NVS_H
#define __APP_NVS_H

#define USER_NEWSPACE      "__period_user_"
#define KEY_WORD           "_period_"
void app_nvs_set_value(char *keyWord, int value);
void app_nvs_set_string(char *keyWord, char *str);
void app_nvs_get_value(char *keyWord, int *value);
void app_nvs_get_string(char *keyWord, char *value);

#endif
