#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define H if(1)

typedef struct huffman_tree huffman_tree;
typedef struct priority_queue priority_queue;

struct huffman_tree
{
	int frequency;
	unsigned char byte;
	huffman_tree *next;
	huffman_tree *left;
	huffman_tree *right;
};

struct priority_queue
{
	huffman_tree *head;
};

priority_queue* create_priority_queue()
{
	priority_queue *a = (priority_queue*) malloc(sizeof(priority_queue));
	a->head=NULL;
	return a;
}

int queue_is_empty (priority_queue *pq)
{
	if(pq->head != NULL)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int is_bit_i_set(unsigned char c, int i)
{
	unsigned char mask = 1 << i;

	return mask & c;
}

void enqueue(priority_queue *pq, unsigned char byte, int f)
{
	huffman_tree *ht = (huffman_tree*) malloc(sizeof(huffman_tree));
	ht->byte = byte;
	ht->frequency = f;

	if((queue_is_empty(pq)) || (f < pq->head->frequency))
	{
		ht->next = pq->head;
		pq->head =  ht;
	}
	else
	{
		huffman_tree *current = pq->head;
		while((current->next != NULL) && (current->next->frequency < f))
		{
			current = current->next;
		}

		ht->next = current->next;
		current->next = ht;
	}
}

huffman_tree* dequeue(priority_queue *pq)
{
	if(queue_is_empty(pq))
	{
		//printf("Priority Queue underflow\n");
		return NULL;
	}
	else
	{
		huffman_tree *ht = pq->head;
		pq->head = pq->head->next;
		ht->next = NULL;
		return ht;
	}
}

void print_priority_queue(priority_queue *pq)
{
	priority_queue *aux;
	aux=pq;

	while(aux->head != NULL)
	{
		printf("%c ", aux->head->byte);
		aux->head=aux->head->next;
	}
	printf("\n");
}

priority_queue* create_huffman_queue(priority_queue *pq, int *table)
{
	for(int i=0;i<255;i++)
	{
		if(table[i] != 0 && table[i] != EOF)
		{
			enqueue(pq,i,table[i]);
		}
	}
	free(table);

	return pq;
}

huffman_tree* create_huffman_struct(unsigned char byte, int f, huffman_tree *left, huffman_tree *right)
{
	huffman_tree *new_ht = (huffman_tree*) malloc(sizeof(huffman_tree));
	new_ht->byte = byte;
	new_ht->frequency = f;
	new_ht->next = NULL;
	new_ht->left = left;
	new_ht->right = right;
	return new_ht;
}

huffman_tree* create_huffman_mix(huffman_tree *left,huffman_tree *right)
{
	unsigned char c = '*';
	huffman_tree *ht =  create_huffman_struct(c,left->frequency + right->frequency, left,right);
	return ht;
}

void enqueue_huffman(priority_queue *pq, huffman_tree *ht)
{
	if((queue_is_empty(pq)) || (ht->frequency < pq->head->frequency))
	{
		ht->next = pq->head;
		pq->head = ht;
	}
	else
	{
		huffman_tree *current = pq->head;
		while((current->next != NULL) && (current->next->frequency < ht->frequency))
		{
			current = current->next;
		}

		ht->next = current->next;
		current->next = ht;
	}
}

int huffman_is_empty(huffman_tree *ht)
{
	if(ht != NULL)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void print_pre_order(huffman_tree *bt)
{

	if(!huffman_is_empty(bt))
	{
		printf("%c ", bt->byte);
		print_pre_order(bt->left);
		print_pre_order(bt->right);
	}
}

void* read_bits(char *file_name)
{
	int* table = (int*) malloc(sizeof(int)*255);

	FILE* file = fopen(file_name, "r");

	unsigned char c,eof = EOF;

	int i;

	memset(table,0,sizeof(int)*255);

	do{
		c = getc(file);

		table[c]++;
	}while(c != eof);

	return (void*) table;
}

void flush_in() {
    int ch;
    do {
        ch = fgetc(stdin);
    } while (ch != EOF && ch != '\n');
}

void compress()
{
	int i;
	unsigned char ast = '*';
	char file_name[270];
	int *table;
	priority_queue *save_head;
	huffman_tree *dequeued1,*dequeued2,*to_enqueue;

	scanf("%s", &file_name);
	flush_in();

	table = read_bits(file_name);

	priority_queue *pq = create_priority_queue();

	pq = create_huffman_queue(pq,table);

	//COMEÇO DA CRIAÇÃO DA ARVORE
	while(pq->head->next != NULL)
	{
		//print_priority_queue(pq);
		dequeued1 = dequeue(pq);
		dequeued2 = dequeue(pq);
		to_enqueue = create_huffman_mix(dequeued1,dequeued2);

		enqueue_huffman(pq,to_enqueue);
	}

	printf("PRINTING TREE\n");
	print_pre_order(pq->head);
	printf("PRINTING QUEUE\n");
	print_priority_queue(pq);

}

int main()
{
	compress();
}