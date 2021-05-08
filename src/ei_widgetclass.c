//
// Created by bossu on 06/05/2021.
//

#include "ei_widgetclass.h"
#include <string.h>
#include <malloc.h>
#include "ei_frame.h"
#include <string.h>

ei_widgetclass_t* list_widgetclass = NULL; //liste de nos classes de widgets
ei_widgetclass_t frame;


void ei_widgetclass_register (ei_widgetclass_t* widgetclass){
    widgetclass->next=NULL;
    if (list_widgetclass==NULL){
        list_widgetclass=widgetclass;
    }
    else {
        ei_widgetclass_t* sent = list_widgetclass;

        while (sent->next != NULL) {
            sent = sent->next;
        }
        sent->next = widgetclass;

    }
}


ei_widgetclass_t* ei_widgetclass_from_name (ei_widgetclass_name_t name){
    ei_widgetclass_t *current = list_widgetclass;
    while (current != NULL){
        if (strcmp(ei_widgetclass_stringname(current->name), ei_widgetclass_stringname(name)) == 0){ //si égaux
            return current;
        }
        current = current->next;
    }
    return NULL;
}


