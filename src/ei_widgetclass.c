//
// Created by bossu on 06/05/2021.
//

#include "ei_widgetclass.h"
#include <string.h>

static ei_widgetclass_t* list_widgetclass = NULL; //liste de nos classes de widgets




ei_widgetclass_t*	ei_widgetclass_from_name	(ei_widgetclass_name_t name){
    ei_widgetclass_t *current = list_widgetclass;
    while (current != NULL){
        if (strcmp(ei_widgetclass_stringname(current->name), ei_widgetclass_stringname(name)) == 0){
            return current;
        }
        current = current->next;
    }
    return NULL;
}