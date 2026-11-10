#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

typedef struct
{
    int element;
    int prev;
    int next;
} ListElement;

typedef struct
{
    ListElement* list_elements;
    size_t capacity;
    size_t size;
} Array;

typedef struct
{
    Array* data;
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
void ImageListDump (List* list);
void HtmlListDump (List* list);

bool PushBack (List* list, ListElement element);
bool SetElement (List* list, DataType dt, size_t index, int value);
bool GetElement (List* list, DataType dt, size_t index, int* value);

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
    if (!list)
    {
        printf ("Memory allocation error\n");
        return 1;
    }

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
    HtmlListDump (list);

    for (int i = 0; i <= 6; i++)
    {
        int value = 0;
        GetElement (list, ELEMENTS, i, &value);
        printf ("[%d] = %d\n", i, value);
        int next = 0;
        GetElement (list, NEXT, i, &next);
        printf ("next[%d] = %d\n", i, next);
        int prev = 0;
        GetElement (list, PREV, i, &prev);
        printf ("prev[%d] = %d\n\n", i, prev);
    }
    return 0;
}

bool ListInit (List* list)
{
    assert (list);
    list->data = (Array*)calloc(1, sizeof(Array));
    if (list->data == NULL)
    {
        printf ("Allocation memory error\n");
        exit(1);
        return false;
    }

    ArrayInit (list->data);

    PushBack (list, (ListElement){0, 0, 0});

    list->next_free = -1;
    list->size_list = 0;
    return true;
}

bool ArrayInit (Array* arr)
{
    assert (arr);
    arr->list_elements = (ListElement*)calloc(2, sizeof(ListElement));
    if (arr->list_elements == NULL)
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
        size_t size_list_elements = 0;
        GetSize (list->data, &size_list_elements);
        list->next_free = size_list_elements;

        PushBack (list, (ListElement){-1, -1, -1});
    }

    int index_prev_element = 0;
    for (int i = 0; i < index; i++)
    {
        GetElement (list, NEXT, index_prev_element, &index_prev_element);
    }

    int index_next_element = 0;
    GetElement (list, NEXT, index_prev_element, &index_next_element);
    int index_current_element = list->next_free;

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
    assert (list);
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

void ImageListDump (List* list)
{
    assert (list);
    FILE* image = fopen ("list_dump.txt", "w");
    fprintf (image, "digraph G{\nrankdir = LR;\n");
    fprintf (image, "node[color = \"#DB7093\",fontsize = 14];\n");
    size_t size = 0;
    GetSize (list->data, &size);

    fprintf (image, "%d[shape = record, label = \" index = %d | elem = %d | { tail = %d | head = %d } \", style = \"rounded, filled\", fillcolor = \"#FFC0CB\"];\n",
                       0,
                       0,
                       GetElementUnsecure (list, ELEMENTS, 0),
                       GetElementUnsecure (list, PREV, 0),
                       GetElementUnsecure (list, NEXT, 0));
    for (int i = 1; i < size; i++)
    {
        if (GetElementUnsecure (list, PREV, i) != -1)
            fprintf (image, "%d[shape = record, label = \" index = %d | elem = %d | { prev = %d | next = %d } \", style = \"rounded, filled\", fillcolor = \"#B0E0E6\", color = \"#608fa2b9\"];",
                            i,
                            i,
                            GetElementUnsecure (list, ELEMENTS, i),
                            GetElementUnsecure (list, PREV, i),
                            GetElementUnsecure (list, NEXT, i));
        else {
            char err[] = "EMPTY";
            fprintf (image, "%d[shape = record, label = \" index = %d | %s | next = %d\", style = \"rounded, filled\", fillcolor = \"#00FF7F\", color = \"#028a46\"];\n",
                            i,
                            i,
                            "EMPTY",
                            GetElementUnsecure (list, NEXT, i));
        }
    }
    for (int i = 0; i < size - 1; i++)
    {
        fprintf (image, "%d->%d[color = \"#FFFFFF\", weight = 100];\n", i, i + 1);
    }
    for (int i = 0; i < size; i++)
    {
        if (i == 0 || GetElementUnsecure (list, NEXT, i) == 0)
            fprintf (image, "%d->%d[color =\"#FF1493\", weight = 0];\n", i, GetElementUnsecure (list, NEXT, i));
        else if (GetElementUnsecure (list, PREV, i) != -1)
            fprintf (image, "%d->%d[color =\"#14c0ff\", weight = 0];\n", i, GetElementUnsecure (list, NEXT, i));
        else if (GetElementUnsecure (list, NEXT, i) != -1)
            fprintf (image, "%d->%d[color =\"#358c4b\", weight = 0];\n", i, GetElementUnsecure (list, NEXT, i));
        /*if (i == 0 || GetElementUnsecure (list, PREV, i) == 0)
            fprintf (image, "%d->%d[color =\"#87CEFA\", weight = 0, constraint=false];\n", i, GetElementUnsecure (list, PREV, i));
        else if (GetElementUnsecure (list, PREV, i) != -1)
            fprintf (image, "%d->%d[color =\"#0000FF\", weight = 0, constraint=false];\n", i, GetElementUnsecure (list, PREV, i));*/
    }
    if (list->next_free != -1)
        fprintf (image, "free[style = \"filled\", fillcolor = \"#7B68EE\", color = \"#4f4299\"];"
                 "\nfree->%d[color = \"#6759b9\", weight = 0];\n", list->next_free);
    fprintf (image, "}\n");
    fclose (image);

    system ("dot -Tpng list_dump.txt -o list.png");
}

void HtmlListDump (List* list)
{
    assert (list);
    ImageListDump (list);

    size_t size = 0;
    GetSize (list->data, &size);

    FILE* html = fopen ("list.html", "w");
    fprintf(html, "<html><body>\n");

    fprintf(html, "<h2>List dump</h2>\n");

    fprintf(html, "<table border=\"1\" style =\"background-color: #B4B4F6;\">\n");
    fprintf(html, "<tr><th>index</th><th>element</th><th>next</th><th>prev</th></tr>\n");

    for (size_t i = 0; i < size; i++)
    {
        fprintf(html,
                "<tr><td>%zu</td><td>%d</td><td>%d</td><td>%d</td></tr>\n",
                i,
                GetElementUnsecure(list, ELEMENTS, i),
                GetElementUnsecure(list, NEXT, i),
                GetElementUnsecure(list, PREV, i));
    }
    fprintf(html, "</table>\n");
    fprintf(html, "<img src=\"list.png\">\n");
    fprintf(html, "</body></html>\n");

    fclose(html);
}

bool PushBack (List* list, ListElement element)
{
    assert (list);
    ExpandSize (list->data);
    list->data->list_elements[list->data->size] = element;
    (list->data->size)++;
    return true;
}

bool SetElement (List* list, DataType dt, size_t index, int value)
{
    assert (list);
    if (index >= list->data->size)
    {
        printf ("Access beyond the memory limit\n");
        return false;
    }

    switch (dt)
    {
        case ELEMENTS: list->data->list_elements[index].element = value;
                       break;
        case NEXT:     list->data->list_elements[index].next = value;
                       break;
        case PREV:     list->data->list_elements[index].prev = value;
                       break;
        default:       printf("Unknown DataType\n");
                       exit(-1);
    }
    return true;
}

bool GetElement (List* list, DataType dt, size_t index, int *value)
{
    assert (list);
    if (index >= list->data->size)
    {
        printf ("Access beyond the memory limit\n");
        return false;
    }
    switch (dt)
    {
        case ELEMENTS: *value = (list->data->list_elements)[index].element;
                       break;
        case NEXT:     *value = (list->data->list_elements)[index].next;
                       break;
        case PREV:     *value = (list->data->list_elements)[index].prev;
                       break;
        default:       printf("Unknown DataType\n");
                       exit(-1);
    }
    return true;
}

bool ExpandSize (Array* array)
{
    if (array->size >= array->capacity)
    {
        size_t new_capacity = 2 * array->capacity;
        ListElement* new_data = (ListElement*)realloc(array->list_elements, new_capacity * sizeof(ListElement));
        if (new_data != NULL)
        {
            array->list_elements = new_data;
            array->capacity = new_capacity;
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

size_t FastAddToList (List* list, size_t index, int value) //TODO доделать эти быстрые функции
{
    assert (list);
    if (list->next_free == -1)
    {
        size_t size_list_elements = 0;
        GetSize (list->data, &size_list_elements);

        PushBack (list, (ListElement){-1, -1, -1});
    }
    int index_prev_element = index;

    int index_next_element = 0;
    GetElement (list, NEXT, index_prev_element, &index_next_element);
    int index_current_element = list->next_free;

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
    assert (list);
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
