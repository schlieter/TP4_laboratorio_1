#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/ArrayList.h"
#define AL_INCREMENT      10
#define AL_INITIAL_VALUE  10

// funciones privadas
int resizeUp(ArrayList* pList);
int resizeDown(ArrayList* pList);
int expand(ArrayList* pList,int index);
int contract(ArrayList* pList,int index);


//___________________

/** \brief Allocate a new arrayList with AL_INITIAL_VALUE elements.
 * \param void
 * \return ArrayList* Return (NULL) if Error [if can't allocate memory]
 *                  - (pointer to new arrayList) if ok
 */
ArrayList* al_newArrayList(void)
{
    ArrayList* pList;
    ArrayList* returnAux = NULL;
    void* pElements;
    pList=(ArrayList *)malloc(sizeof(ArrayList));

    if(pList != NULL)
    {
        pElements = malloc(sizeof(void *)*AL_INITIAL_VALUE );
        if(pElements != NULL)
        {
            pList->size=0;
            pList->pElements=pElements;
            pList->reservedSize=AL_INITIAL_VALUE;
            pList->add=al_add;
            pList->len=al_len;
            pList->set=al_set;
            pList->remove=al_remove;
            pList->clear=al_clear;
            pList->clone=al_clone;
            pList->get=al_get;
            pList->contains=al_contains;
            pList->push=al_push;
            pList->indexOf=al_indexOf;
            pList->isEmpty=al_isEmpty;
            pList->pop=al_pop;
            pList->subList=al_subList;
            pList->containsAll=al_containsAll;
            pList->deleteArrayList = al_deleteArrayList;
            pList->sort = al_sort;
            returnAux = pList;
        }
        else
        {
            free(pList);
        }
    }

    return returnAux;
}


/** \brief  Add an element to arrayList and if is
 *          nessesary resize the array
 * \param pList ArrayList* Pointer to arrayList
 * \param pElement void* Pointer to element
 * \return int Return (-1) if Error [pList or pElement are NULL pointer] - (0) if Ok
 *
 */
int al_add(ArrayList* pList,void* pElement)
{
    int returnAux = -1, flag = 1;
    int aux;
    if(pList != NULL && pElement != NULL)
    {
        aux = resizeUp(pList);
        if(aux == 0)
        {
            flag = 1;
        }
        else
        {
            flag = 0;
        }
        if(flag)
        {
            pList->pElements[pList->size] = pElement;
            pList->size++;
            returnAux = 0;
        }
    }
    return returnAux;
}

/** \brief  Delete arrayList
 * \param pList ArrayList* Pointer to arrayList
 * \return int Return (-1) if Error [pList is NULL pointer] - (0) if Ok
 *
 */
int al_deleteArrayList(ArrayList* pList)
{
    int returnAux = -1;
    if(pList != NULL)
    {
        free(pList->pElements);
        free(pList);
        returnAux = 0;
    }
    return returnAux;
}

/** \brief
 * \param p
 * \return int Return length of array or (-1) if Error [pList is NULL pointer]
 *
 */
int al_len(ArrayList* pList)
{
    int returnAux = -1;
    if(pList != NULL)
    {
        returnAux = pList->size;
    }
    return returnAux;
}


/** \brief  Get an element by index
 * \param pList ArrayList* Pointer to arrayList
 * \param index int Index of the element
 * \return void* Return (NULL) if Error [pList is NULL pointer or invalid index] - (Pointer to element) if Ok
 *
 */
void* al_get(ArrayList* pList , int index)
{
    void* returnAux = NULL;
    if(pList != NULL && (index >= 0 && index < pList->size))
    {
        returnAux = pList->pElements[index];
    }
    return returnAux;
}


/** \brief  Find if pList contains at least one element pElement
 * \param pList ArrayList* Pointer to arrayList
 * \param pElement void* Pointer to element
 * \return int Return (-1) if Error [pList or pElement are NULL pointer]
 *                  - ( 0) if Ok but not found a element
 *                  - ( 1) if this list contains at least one element pElement
 *
 */
int al_contains(ArrayList* pList, void* pElement)
{
    int returnAux = -1, i;
    if(pList != NULL && pElement != NULL)
    {
        for(i=0;i<al_len(pList);i++)
        {
            if(pList->pElements[i]==pElement)
            {
                returnAux = 1;
                break;
            }
            returnAux=0;
        }
    }
    return returnAux;
}


/** \brief  Set a element in pList at index position
 * \param pList ArrayList* Pointer to arrayList
 * \param index int Index of the element
 * \param pElement void* Pointer to element
 * \return int Return (-1) if Error [pList or pElement are NULL pointer or invalid index]
 *                  - ( 0) if Ok
 *
 */
int al_set(ArrayList* pList, int index,void* pElement)
{
    int returnAux = -1;
    if(pList != NULL && pElement != NULL && (index >= 0 && index < pList->size))
    {
        pList->pElements[index] = pElement;
        returnAux = 0;
    }
    return returnAux;
}


/** \brief  Remove an element by index
 * \param pList ArrayList* Pointer to arrayList
 * \param index int Index of the element
 * \return int Return (-1) if Error [pList is NULL pointer or invalid index]
 *                  - ( 0) if Ok
 */
int al_remove(ArrayList* pList,int index)
{
    int returnAux = -1,aux;
    if(pList != NULL && (index >= 0 && index < pList->size))
    {
        aux = contract(pList,index);
        if(aux == 0)
        {
            returnAux = 0;
        }
    }
    return returnAux;
}



/** \brief Removes all of the elements from this list
 * \param pList ArrayList* Pointer to arrayList
 * \return int Return (-1) if Error [pList is NULL pointer]
 *                  - ( 0) if Ok
 */
int al_clear(ArrayList* pList)
{
    int returnAux = -1;
    if(pList != NULL)
    {
        free(pList->pElements);
        pList->size = 0;
        resizeDown(pList);
        returnAux = 0;
    }
    return returnAux;
}



/** \brief Returns an array containing all of the elements in this list in proper sequence
 * \param pList ArrayList* Pointer to arrayList
 * \return ArrayList* Return  (NULL) if Error [pList is NULL pointer]
 *                          - (New array) if Ok
 */
ArrayList* al_clone(ArrayList* pList)
{
    ArrayList* returnAux = NULL;
    ArrayList* pList2 = NULL;
    void* pElement2;
    int i;
    if(pList != NULL)
    {
        pList2 = al_newArrayList();
        if(pList2 != NULL)
        {
            for(i=0;i<al_len(pList);i++)
            {
                pElement2 = al_get(pList,i);
                al_add(pList2,pElement2);
            }
            if(pList->size == pList2->size)
            {
                return pList2;
            }
        }
    }
    return returnAux;
}




/** \brief Inserts the element at the specified position
 * \param pList ArrayList* Pointer to arrayList
 * \param index int Index of the element
 * \param pElement void* Pointer to element
 * \return int Return (-1) if Error [pList or pElement are NULL pointer or invalid index]
 *                  - ( 0) if Ok
 */
int al_push(ArrayList* pList, int index, void* pElement)
{
    int returnAux = -1, auxExp, aux;
    if(pList != NULL && pElement != NULL && (index >= 0 && index < pList->size))
    {
        aux = al_isEmpty(pList);
        if(aux == 0)
        {
            auxExp = expand(pList,index);
            if(auxExp == 0)
            {
                pList->pElements[index] = pElement;
                pList->size++;
                returnAux = 0;
            }
        }
    }
    else if(index == pList->size)
    {
        aux=al_add(pList,pElement);
        if(aux == 0)
            returnAux = 0;
    }
    return returnAux;
}



/** \brief Returns the index of the first occurrence of the specified element
 * \param pList ArrayList* Pointer to arrayList
 * \param pElement void* Pointer to element
 * \return int Return (-1) if Error [pList or pElement are NULL pointer] - (index to element) if Ok
 */
int al_indexOf(ArrayList* pList, void* pElement)
{
    int returnAux = -1,aux,i;
    if(pList!= NULL && pElement != NULL)
    {
        aux = al_isEmpty(pList);
        if(aux == 0)
        {
            for(i=0;i<al_len(pList);i++)
            {
                if(pList->pElements[i] == pElement)
                {
                    returnAux = i;
                }
            }
        }
    }
    return returnAux;
}



/** \brief Returns true if this list contains no elements.
 * \param pList ArrayList* Pointer to arrayList
 * \return int Return (-1) if Error [pList is NULL pointer] - (0) if Not Empty - (1) if is Empty
 */
int al_isEmpty(ArrayList* pList)
{
    int returnAux = -1;
    if(pList != NULL)
    {
        if(pList->size == 0)
        {
            returnAux = 1;
        }
        else
        {
            returnAux = 0;
        }
    }
    return returnAux;
}




/** \brief Remove the item at the given position in the list, and return it.
 * \param pList ArrayList* Pointer to arrayList
 * \param index int Index of the element
 * \return int Return (NULL) if Error [pList is NULL pointer or invalid index]
 *                  - ( element pointer) if Ok
 */
void* al_pop(ArrayList* pList,int index)
{
    void* returnAux = NULL;
    int aux,i;
    if(pList != NULL && (index >= 0 && index < pList->size))
    {
        aux = al_isEmpty(pList);
        if(aux == 0)
        {
            returnAux = al_get(pList, index);
            if(returnAux != NULL)
            {
                i = al_remove(pList, index);
                if(i == -1)
                    returnAux=NULL;
            }
        }
    }
    else if(index == pList->size)
    {
        returnAux = NULL;
    }
    return returnAux;
}


/** \brief Returns a new arrayList with a portion of pList between the specified
 *         fromIndex, inclusive, and toIndex, exclusive.
 * \param pList ArrayList* Pointer to arrayList
 * \param from int Initial index of the element (inclusive)
 * \param to int Final index of the element (exclusive)
 * \return int Return (NULL) if Error [pList is NULL pointer or invalid 'from' or invalid 'to']
 *                  - ( pointer to new array) if Ok
 */
ArrayList* al_subList(ArrayList* pList,int from,int to)
{
    void* returnAux = NULL;
    void* pElement2;
    ArrayList* pList2 = NULL;
    int aux,i;
    if(pList != NULL && from >= 0 && from <= to && to <= pList->size)
    {
        pList2 = al_newArrayList();
        if(pList2 != NULL)
        {
            for(i=from;i<to;i++)
            {
               pElement2 = al_get(pList,i);
               aux = al_add(pList2,pElement2);
            }
            if(al_isEmpty(pList2) == 0)
            {
                return pList2;
            }
        }
    }
    return returnAux ;
}





/** \brief Returns true if pList list contains all of the elements of pList2
 * \param pList ArrayList* Pointer to arrayList
 * \param pList2 ArrayList* Pointer to arrayList
 * \return int Return (-1) if Error [pList or pList2 are NULL pointer]
 *                  - (0) if Not contains All - (1) if is contains All
 */
int al_containsAll(ArrayList* pList,ArrayList* pList2)
{
    int returnAux = -1,i,j,cont=0;
    if(pList != NULL && pList2 != NULL)
    {
        for(j=0;j<pList2->size;j++)
        {
            for(i=0; i<pList->size; i++)
            {
                if(pList2->pElements[j-1] != pList->pElements[i-1])
                {
                    continue;
                }
                else
                {

                    cont++;
                    break;
                }
            }
        }
        if(cont == pList2->size)
        {
            returnAux = 1;
        }
        else
        {
            returnAux = 0;
        }
    }
    return returnAux;
}

/** \brief Sorts objects of list, use compare pFunc
 * \param pList ArrayList* Pointer to arrayList
 * \param pFunc (*pFunc) Pointer to fuction to compare elements of arrayList
 * \param order int  [1] indicate UP - [0] indicate DOWN
 * \return int Return (-1) if Error [pList or pFunc are NULL pointer]
 *                  - (0) if ok
 */
int al_sort(ArrayList* pList, int (*pFunc)(void* ,void*), int order)
{
    int returnAux = -1, i, j, aux;
    void* AuxpList;
    if(pList != NULL && pFunc != NULL && order > -1 && order < 2)
    {
        for(i=0;i<al_len(pList)-1;i++)
        {
            for(j=i+1;j<al_len(pList);j++)
            {
                aux = pFunc(pList->pElements[i],pList->pElements[j]);
                switch(aux)
                {
                    case -1:
                        if(order == 0)
                        {
                            AuxpList = pList->pElements[j];
                            pList->pElements[j] = pList->pElements[i];
                            pList->pElements[i] = AuxpList;
                        }
                        else
                        {
                            continue;
                        }
                        break;
                    case 0:
                        continue;
                        break;
                    case 1:
                        if(order == 0)
                        {
                            continue;
                        }
                        else
                        {
                            AuxpList = pList->pElements[j];
                            pList->pElements[j] = pList->pElements[i];
                            pList->pElements[i] = AuxpList;
                        }
                        break;
                }
            }
        }
        returnAux = 0;
    }

    return returnAux;
}


/** \brief Increment the number of elements in pList in AL_INCREMENT elements.
 * \param pList ArrayList* Pointer to arrayList
 * \return int Return (-1) if Error [pList is NULL pointer or if can't allocate memory]
 *                  - (0) if ok
 */
int resizeUp(ArrayList* pList)
{
    int retorno = -1;
    void* aux;
    if(pList != NULL)
    {
        if(pList->size == pList->reservedSize)
        {
            aux = realloc(pList->pElements,sizeof(void*)* (pList->reservedSize + AL_INCREMENT));
            if(aux != NULL)
            {
                pList->reservedSize = (pList->reservedSize) + AL_INCREMENT;
                pList->pElements = aux;
                retorno = 0;
            }
        }
        else
        {
            retorno = 0;
        }
    }
    return retorno;
}

/** \brief
 *
 * \param
 * \param
 * \return
 *
 */
int resizeDown(ArrayList* pList)
{
    int retorno = -1;
    void* aux;
    if(pList != NULL)
    {
        if(pList->reservedSize >= pList->size * 5)
        {
            aux = realloc(pList->pElements,sizeof(void*)* (pList->size + AL_INCREMENT));
            if(aux != NULL)
            {
                pList->reservedSize = pList->size + AL_INCREMENT;
                pList->pElements = aux;
                retorno = 0;
            }
        }
    }
    return retorno;
}

/** \brief  Expand an array list
 * \param pList ArrayList* Pointer to arrayList
 * \param index int Index of the element
 * \return int Return (-1) if Error [pList is NULL pointer or invalid index]
 *                  - ( 0) if Ok
 */
int expand(ArrayList* pList,int index)
{
    int returnAux = -1,auxRup,i;
    if(pList != NULL && (index >= 0 && index < pList->size))
    {
        auxRup = resizeUp(pList);
        if(auxRup == 0)
        {
            for(i=(al_len(pList)-1);i>=index;i--)
            {
                pList->pElements[i+1] = pList->pElements[i];
            }
            returnAux = 0;
        }
    }
    return returnAux;
}

/** \brief  Contract an array list
 * \param pList ArrayList* Pointer to arrayList
 * \param index int Index of the element
 * \return int Return (-1) if Error [pList is NULL pointer or invalid index]
 *                  - ( 0) if Ok
 */
int contract(ArrayList* pList,int index)
{
    int returnAux = -1,i;
    if(pList != NULL && (index >= 0 && index < pList->size))
    {
        for(i=index;i<al_len(pList);i++)
        {
            pList->pElements[i] = pList->pElements[i+1];
        }
        pList->size--;
        returnAux = 0;
    }
    return returnAux;
}
