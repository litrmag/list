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

bool ListInit (List* list);
bool AddToList (List* list, size_t index, int value);
bool DeleteFromList (List* list, size_t index);
bool UpdateList (List* list, size_t index, int value);
bool GetElementFromList (List* list, size_t index, int* value);

bool ArrayInit (Array* arr);
bool PushBack (Array* arr, int value);
bool ExpandSize (Array* arr);
bool SetElement (Array* array, size_t index, int value);
bool GetElement (Array* array, size_t index, int *value);
size_t GetSize (Array* array);

int main ()
{
    List* list = (List*)calloc(1, sizeof(List));
    ListInit (list);

    AddToList (list, 1, 10);

    AddToList (list, 2, 20);

    AddToList (list, 3, 30);

    AddToList (list, 4, 40);

    AddToList (list, 4, 35);

    DeleteFromList (list, 3);

    for (int i = 0; i <= 4; i++)
    {
        int value = 0;
        GetElementFromList (list, i, &value);
        printf ("[%d] - %d\n", i, value);
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

    PushBack (list->elements, 0);
    PushBack (list->next, 0);
    PushBack (list->prev, 0);

    list->next_free = -1;
    return true;
}

bool AddToList (List* list, size_t index, int value) //TODO вот тут ломается, ищи блять
{
    if (list->next_free == -1)
    {
        PushBack (list->elements, value);
        PushBack (list->next, -1);
        PushBack (list->prev, -1);


        size_t size_list = GetSize (list->elements);
        list->next_free = size_list - 1;

    } else {
        SetElement (list->elements, list->next_free, value);
    }

    int ind = 0;
    for (int i = 0; i < index - 1; i++)
    {
        GetElement (list->next, ind, &ind);
    }


    int index_prev_element = ind;

    SetElement (list->prev, list->next_free, ind);

    int new_tail = list->next_free;
    int new_next_free = 0;
    GetElement (list->next, list->next_free, &new_next_free);

    int index_next_element = 0;
    GetElement (list->next, ind, &index_next_element);
    SetElement (list->next, list->next_free, index_next_element);
    SetElement (list->next, ind, list->next_free);

    list->next_free = new_next_free;

    if (index_next_element != -1)
    {
        SetElement (list->prev, index_next_element, list->next_free);
    } else {
        SetElement (list->prev, 0, new_tail);
    }

    (list->size_list)++;
    return true;
}

bool DeleteFromList (List* list, size_t index)
{
    int ind = 0;
    for (int i = 0; i < index - 1; i++)
    {
        GetElement (list->next, ind, &ind);
    }
    int index_prev_element = ind;
    GetElement (list->next, ind, &ind);
    int index_next_element = 0;
    GetElement (list->next, ind, &index_next_element);

    SetElement (list->next, index_prev_element, index_next_element);
    SetElement (list->prev, index_next_element, index_prev_element);

    SetElement (list->next, ind, list->next_free);
    list->next_free = ind;

    (list->size_list)--;
    return true;
}

bool UpdateList (List* list, size_t index, int value)
{
    int ind = 0;
    for (int i = 0; i <= index; i++)
    {
        GetElement (list->next, ind, &ind);
    }
    SetElement (list->elements, ind, value);
    return true;
}

bool GetElementFromList (List* list, size_t index, int* value)
{
    if (index >= list->size_list) {
        printf ("Index out of range\n");
        return false;
    }

    int ind = 0;
    for (int i = 0; i <= index; i++)
    {
        GetElement (list->next, ind, &ind);
    }
    GetElement (list->elements, ind, value);
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

bool PushBack (Array* arr, int value)
{
    ExpandSize (arr);
    arr->data[arr->size] = value;
    (arr->size)++;
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

bool SetElement (Array* array, size_t index, int value)
{
    if (index >= array->size)
    {
        printf ("Access beyond the memory limit\n");
        exit (1);
        return false;
    }
    (array->data)[index] = value;
    return true;
}

bool GetElement (Array* array, size_t index, int *value)
{
    if (index >= array->size)
    {
        printf ("Access beyond the memory limit\n");
        exit (1);
        return false;
    }
    *value = (array->data)[index];
    return true;
}

size_t GetSize (Array* array)
{
    return array->size;
}

