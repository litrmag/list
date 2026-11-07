#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    int* data;
    size_t capacity;
    size_t size;
} Array;

typedef struct
{
    Array* elements;
    Array* next;
    Array* prev;
    int next_free;
    size_t size_list;
} List;

typedef enum
{
    ELEMENTS,
    NEXT,
    PREV
} DataType;

bool ListInit (List* list);
bool ArrayInit (Array* arr);

bool AddToList (List* list, size_t index, int value);
bool DeleteFromList (List* list, size_t index);
bool UpdateList (List* list, size_t index, int value);
bool GetElementFromList (List* list, size_t index, int* value);
void ListDump (List* list);

bool PushBack (List* list, DataType dt, int value);
bool SetElement (List* list, DataType dt, size_t index, int value);
bool GetElement (List* list, DataType dt, size_t index, int *value);

bool ExpandSize (Array* arr);
bool GetSize (Array* array, size_t* size);
int GetElementUnsecure (List* list, DataType dt, size_t index);

size_t FastAddToList (List* list, size_t index, int value);
size_t FastDeleteFromList (List* list, size_t index);
bool FastUpdateList (List* list, size_t index, int value);
bool FastGetElementFromList (List* list, size_t index, int* value);

int main ()
{
    List* list = (List*)calloc(1, sizeof(List));
    ListInit (list);
    AddToList (list, 0, 5);
    AddToList (list, 1, 10);
    AddToList (list, 2, 20);
    AddToList (list, 3, 30);
    AddToList (list, 4, 40);
    AddToList (list, 4, 35);
    DeleteFromList (list, 3);
    DeleteFromList (list, 4);
    DeleteFromList (list, 1);
    AddToList (list, 5, 52);

    ListDump (list);
    for (int i = 0; i <= 6; i++)
    {
        int value = 0;
        GetElement (list, ELEMENTS, i, &value);
        printf ("[%d] - %d\n", i, value);
        int next = 0;
        GetElement (list, NEXT, i, &next);
        printf ("next[%d] - %d\n", i, next);
        int prev = 0;
        GetElement (list, PREV, i, &prev);
        printf ("prev[%d] - %d\n\n", i, prev);
    }
    return 0;
}

bool ListInit (List* list)
{
    list->elements = (Array*)calloc(1, sizeof(Array));
    list->next = (Array*)calloc(1, sizeof(Array));
    list->prev = (Array*)calloc(1, sizeof(Array));

    ArrayInit (list->elements);
    ArrayInit (list->next);
    ArrayInit (list->prev);

    PushBack (list, ELEMENTS, 0);
    PushBack (list, NEXT, 0);
    PushBack (list, PREV, 0);

    list->next_free = -1;
    list->size_list = 0;
    return true;
}

bool ArrayInit (Array* arr)
{
    arr->data = (int*)calloc(2, sizeof(int));
    if (arr->data == NULL)
    {
        printf ("Allocation memory error\n");
        exit(1);
        return false;
    }
    arr->capacity = 2;
    arr->size = 0;
    return true;
}

bool AddToList (List* list, size_t index, int value)
{
    if (index > list->size_list)
    {
        index = list->size_list;
    }

    if (list->next_free == -1)
    {
        size_t size_elements = 0;
        GetSize (list->elements, &size_elements);
        list->next_free = size_elements;

        PushBack (list, ELEMENTS, -1);
        PushBack (list, NEXT, -1);
        PushBack (list, PREV, -1);
    }

    int index_prev_element = 0;
    for (int i = 0; i < index; i++)
    {
        GetElement (list, NEXT, index_prev_element, &index_prev_element);
    }

    int index_next_element = 0;
    GetElement (list, NEXT, index_prev_element, &index_next_element);
    int index_current_element = list->next_free;

    //printf("index: %d p: %d c: %d n: %d\n", index, index_prev_element, index_current_element, index_next_element);

    SetElement (list, PREV, index_current_element, index_prev_element);
    GetElement (list, NEXT, index_current_element, &(list->next_free));
    SetElement (list, NEXT, index_current_element, index_next_element);
    SetElement (list, ELEMENTS, index_current_element, value);

    SetElement (list, PREV, index_next_element, index_current_element);
    SetElement (list, NEXT, index_prev_element, index_current_element);

    (list->size_list)++;
    return true;
}

bool DeleteFromList (List* list, size_t index)
{
    int index_prev_element = 0;
    for (int i = 0; i < index; i++)
    {
        GetElement (list, NEXT, index_prev_element, &index_prev_element);
    }

    int index_current_element = 0;
    GetElement (list, NEXT, index_prev_element, &index_current_element);
    int index_next_element = 0;
    GetElement (list, NEXT, index_current_element, &index_next_element);

    SetElement (list, PREV, index_next_element, index_prev_element);
    SetElement (list, NEXT, index_prev_element, index_next_element);
    SetElement (list, NEXT, index_current_element, list->next_free);
    list->next_free = index_current_element;
    SetElement (list, PREV, index_current_element, -1);
    SetElement (list, ELEMENTS, index_current_element, -1);

    (list->size_list)--;
    return true;
}

bool UpdateList (List* list, size_t index, int value)
{
    int index_current_element = 0;
    for (int i = 0; i <= index; i++)
    {
        GetElement (list, NEXT, index_current_element, &index_current_element);
    }
    SetElement (list, ELEMENTS, index_current_element, value);
    return true;
}

bool GetElementFromList (List* list, size_t index, int* value)
{
    if (index >= list->size_list) {
        printf ("Index out of range\n");
        return false;
    }

    int index_current_element = 0;
    for (int i = 0; i <= index; i++)
    {
        GetElement (list, NEXT, index_current_element, &index_current_element);
    }
    GetElement (list, ELEMENTS, index_current_element, value);
    return true;
}

void ListDump (List* list)
{
    FILE* file = fopen ("list_dump.txt", "w");
    fprintf (file, "digraph G{\nrankdir = LR;\n");
    fprintf (file, "node[color = \"#DB7093\",fontsize = 14];\n");
    size_t size = 0;
    GetSize (list->elements, &size);

    fprintf (file, "%d[shape = record, label =\" index = %d | elem = %d | { tail = %d | head = %d } \",style = \"rounded, filled\", fillcolor = \"#FFC0CB\"];\n",
                       0, 0, GetElementUnsecure (list, ELEMENTS, 0), GetElementUnsecure (list, PREV, 0), GetElementUnsecure (list, NEXT, 0));
    for (int i = 1; i < size; i++)
    {
        if (list->prev->data[i] != -1)
            fprintf (file, "%d[shape = record, label = \" index = %d | elem = %d | { prev = %d | next = %d } \", style = \"rounded, filled\", fillcolor = \"#B0E0E6\"];",
                            i, i, GetElementUnsecure (list, ELEMENTS, i), GetElementUnsecure (list, PREV, i), GetElementUnsecure (list, NEXT, i));
        else {
            char err[] = "EMPTY";
            fprintf (file, "%d[shape = record, label = \" index = %d | %s | next = %d\",style = \"rounded, filled\", fillcolor = \"#00FF7F\"];\n",
                            i, i, "EMPTY", GetElementUnsecure (list, NEXT, i));
        }
    }
    for (int i = 0; i < size - 1; i++)
    {
        fprintf (file, "%d->%d[color = \"#FFFFFF\", weight = 100];\n", i, i + 1);
    }
    for (int i = 0; i < size; i++)
    {
        if (i == 0 || GetElementUnsecure (list, NEXT, i) == 0)
            fprintf (file, "%d->%d[color =\"#000000\", weight = 1];\n", i, GetElementUnsecure (list, NEXT, i));
        else if (GetElementUnsecure (list, NEXT, i) != -1)
            fprintf (file, "%d->%d[color =\"#FF1493\", weight = 1];\n", i, GetElementUnsecure (list, NEXT, i));

        /*
        if (i == 0 || GetElementUnsecure (list, PREV, i) == 0)
            fprintf (file, "%d->%d[color =\"#87CEFA\", weight = 1];\n", i, GetElementUnsecure (list, PREV, i));
        else if (GetElementUnsecure (list, PREV, i) != -1)
            fprintf (file, "%d->%d[color =\"#0000FF\", weight = 1];\n", i, GetElementUnsecure (list, PREV, i));
*/
    }
    if (list->next_free != -1)
        fprintf (file, "free[style = \"filled\", fillcolor = \"#7B68EE\"];\nfree->%d\n", list->next_free);
    fprintf (file, "}\n");
    fclose (file);
}

bool PushBack (List* list, DataType dt, int value)
{
    Array* array = NULL;
    switch (dt)
    {
        case ELEMENTS: array = list->elements;
                       break;
        case NEXT:     array = list->next;
                       break;
        case PREV:     array = list->prev;
                       break;
        default:       printf("Unknown DataType\n");
                       exit(-1);
    }
    ExpandSize (array);
    array->data[array->size] = value;
    (array->size)++;
    return true;
}



bool SetElement (List* list, DataType dt, size_t index, int value)
{
    Array* array = NULL;
    switch (dt)
    {
        case ELEMENTS: array = list->elements;
                       break;
        case NEXT:     array = list->next;
                       break;
        case PREV:     array = list->prev;
                       break;
        default:       printf("Unknown DataType\n");
                       exit(-1);
    }
    if (index >= array->size)
    {
        printf ("Access beyond the memory limit\n");
        return false;
    }
    (array->data)[index] = value;
    return true;
}

bool GetElement (List* list, DataType dt, size_t index, int *value)
{
    Array* array = NULL;
    switch (dt)
    {
        case ELEMENTS: array = list->elements;
                       break;
        case NEXT:     array = list->next;
                       break;
        case PREV:     array = list->prev;
                       break;
        default:       printf("Unknown DataType\n");
                       exit(-1);
    }
    if (index >= array->size)
    {
        printf ("Access beyond the memory limit\n");
        return false;
    }
    *value = (array->data)[index];
    return true;
}

bool ExpandSize (Array* arr)
{
    if (arr->size >= arr->capacity)
    {
        size_t new_capacity = 2 * arr->capacity;
        int* new_data = (int*)realloc(arr->data, new_capacity * sizeof(int));
        if (new_data != NULL)
        {
            arr->data = new_data;
            arr->capacity = new_capacity;
            return true;
        } else {
            printf ("Allocation memory error\n");
            exit (1);
            return false;
        }
    }
    return true;
}

bool GetSize (Array* array, size_t* size)
{
    if (!array)
        return false;
    *size = array->size;
    return true;
}

int GetElementUnsecure (List* list, DataType dt, size_t index)
{
    int value = 0;
    if (GetElement (list, dt, index, &value))
        return value;
    else
        exit (1);
}

size_t FastAddToList (List* list, size_t index, int value)
{
    if (list->next_free == -1)
    {
        size_t size_elements = 0;
        GetSize (list->elements, &size_elements);//list->elements->size;

        PushBack (list, ELEMENTS, -1);
        PushBack (list, NEXT, -1);
        PushBack (list, PREV, -1);
    }
    int index_prev_element = index;

    int index_next_element = 0;
    GetElement (list, NEXT, index_prev_element, &index_next_element);
    int index_current_element = list->next_free;

    //printf("index: %d p: %d c: %d n: %d\n", index, index_prev_element, index_current_element, index_next_element);

    SetElement (list, PREV, index_current_element, index_prev_element);
    GetElement (list, NEXT, index_current_element, &(list->next_free));
    SetElement (list, NEXT, index_current_element, index_next_element);
    SetElement (list, ELEMENTS, index_current_element, value);

    SetElement (list, PREV, index_next_element, index_current_element);
    SetElement (list, NEXT, index_prev_element, index_current_element);

    (list->size_list)++;
    return index_current_element;
}

size_t FastDeleteFromList (List* list, size_t index)
{
    int index_prev_element = 0;
    GetElement (list, PREV, index, &index_prev_element);
    int index_next_element = 0;
    GetElement (list, NEXT, index, &index_next_element);

    SetElement (list, PREV, index_next_element, index_prev_element);
    SetElement (list, NEXT, index_prev_element, index_next_element);

    SetElement (list, NEXT, index, list->next_free);
    list->next_free = index;
    SetElement (list, ELEMENTS, index, -1);
    SetElement (list, PREV, index, -1);

    (list->size_list)--;
    return index_next_element;
}

bool FastUpdateList (List* list, size_t index, int value)
{
    return SetElement (list, ELEMENTS, index, value);
}

bool FastGetElementFromList (List* list, size_t index, int* value)
{
    return GetElement (list, ELEMENTS, index, value);
}
