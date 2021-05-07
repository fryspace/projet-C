//
// Created by bossu on 06/05/2021.
//

#include "ei_widgetclass.h"
#include <string.h>

static ei_widgetclass_t* list_widgetclass = NULL; //liste de nos classes de widgets


void ei_widgetclass_register (ei_widgetclass_t* widgetclass){
    ei_widgetclass_t sent;
    sent.next=list_widgetclass;
    ei_widgetclass_t *queue = &sent;
    while (queue->next != NULL) {
        queue = queue->next;
    }
    queue->next=widgetclass;
    list_widgetclass=sent.next;
}


ei_widgetclass_t*	ei_widgetclass_from_name	(ei_widgetclass_name_t name){
    ei_widgetclass_t *current = list_widgetclass;
    while (current != NULL){
        if (strcmp(ei_widgetclass_stringname(current->name), ei_widgetclass_stringname(name)) == 0){ //si égaux
            return current;
        }
        current = current->next;
    }
    return NULL;
}
