#ifndef buffer_h
#define buffer_h

typedef int buffer_item;
#define BUFFER_SIZE 5

/* the buffer */
buffer_item buffer[BUFFER_SIZE];
int waiting[BUFFER_SIZE];

/* insert item into buffer
return 0 if successful, otherwise
return -1 indicating an error condition */
int insert_item(buffer_item item) {
    
    int insert_success=-1;
    for(int i=0; i<BUFFER_SIZE; i++)
    {
        if(waiting[i])
        {
            insert_success = 0;
            buffer[i] = item;
            waiting[i] = 0;
            break;
        }
    }
   
    return insert_success;
}

/* remove an object from buffer
placing it in item
return 0 if successful, otherwise
return -1 indicating an error condition */
int remove_item(buffer_item *item) {
  
    int remove_success=-1;
    for(int i=0; i<BUFFER_SIZE; i++)
    {
        if(!waiting[i])
        {
            remove_success = 0;
            (*item) = buffer[i];
            waiting[i] = 1;
            break;
        }
    }
    
    return remove_success;
}

#endif
