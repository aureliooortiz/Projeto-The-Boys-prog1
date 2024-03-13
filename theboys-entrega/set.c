#include <stdio.h>
#include <stdlib.h>
#include "set.h"

// Creates an empty set capable of holding "size" values (from 0 to size -1)
// Return: pointer to the new set or NULL if failure
struct set_t *set_create (int max) {
	struct set_t *s;
	
	s = malloc(sizeof(struct set_t));
	if(!s) return NULL;
	s->flag = malloc(max * sizeof(bool));
	if(!s->flag) return NULL;
	
	//numero atual de elementos 
	s->num = 0;						
	for(int i = 0; i < max;i++){	
		s->flag[i] = false;	
	}
	//numero maximo de elementos
	s->size = max;					
	return s;
}

// Destroys the set, freeing its memory
// Return: NULL
struct set_t *set_destroy (struct set_t *s){
	free(s->flag);
	free(s);
	return NULL;
}

// Add an item to the set.
// Return: 1 if success or 0 if failure
int set_add (struct set_t *s, int item){
	if(!s) return 0;
	if(item >= s->size) return 0; 		//se a posicao que deseja adicionar for 	
										//maior ou igual a capacidade retorna
	if(!s->flag[item]){
		s->flag[item] = true;
		s->num++;
		return 1;
	}
	return 0;
}

// Removes an item from the set.
// Return: 1 if success or 0 if failure
int set_del (struct set_t *s, int item){
	if(!s) return 0;
	if(item >= s->size) return 0; 		//se a posicao que deseja remover for 	
										//maior ou igual a capacidade retorna
	if(s->flag[item]){
		s->flag[item] = false;
		s->num--;
		return 1;
	}
	return 0;
}

// Verifies wether the item belongs to (is in) the set.
// Return: 1 if success or 0 if failure
int set_in (struct set_t *s, int item){
	if(!s || !s->flag) return 0;
	//se o item for maior ou igual ao tamanho
	//ou menor que 0 
	//não pertence ao conjunto
	if(item >= s->size || item < 0) return 0;
	
	if(s->flag[item]){
		return 1;
	}
	return 0;
}

// Verifies wether the set is empty
// Return: 1 if success or 0 if failure
int set_empty (struct set_t *s){
	if(!s) return 0;	
	
	if(s->num > 0){
		return 1;
	}
	return 0;
}

// Informs the set's cardinality (number of items or |S|)
// Return: n >=0 if success or -1 if failure
int set_card (struct set_t *s){
	if(!s) return 0;
	
	if(s->num >= 0){
		return s->num;
	}
	return -1;	
}

// Verifies wether s1 contains s2
// Return: 1 if success or 0 if failure
int set_contains (struct set_t *s1, struct set_t *s2){
	if(!s1 || !s2 ) return 0;
	if(!s1->flag || !s2->flag) return 0;

	for(int i = 0;i < s1->size;i++){
		if(s1->flag[i] && !s2->flag[i]){
			return 0;
		}
	}
	return 1;
}

// Verifies wether s1 == s2
// Return: 1 if success or 0 if failure
int set_equal (struct set_t *s1, struct set_t *s2){
	if(!s1 || !s2 ) return 0;
	if(!s1->flag || !s2->flag) return 0;	
			
	for(int i = 0; i < s1->size;i++){					//verifica se os elementos de cada conjunto
		if(s1->flag[i] != s2->flag[i]){					//são iguais,e para se achar um diferente,	
			return 0;								
		}										
	}											
	return 1;
}

// Puts in s3 the union of sets s1 and s2; s3 previous content
// is cleared, but its range (0, max) is not modified
// Return: 1 if success or 0 if failure
int set_union (struct set_t *s1, struct set_t *s2, struct set_t *s3){
	if(!s1 || !s2 || !s3) return 0;
	if(!s1->flag || !s2->flag || !s3->flag) return 0;
	
	for(int i = 0;i < s1->size;i++){				//faz a união de s1 e s2
		if(s1->flag[i] || s2->flag[i]){				//e coloca em s3
			s3->flag[i] = true;							
			s3->num++;	
		}
	}
	return 1;	
	
}	

// Puts in s3 the intersection of sets s1 and s2; s3 previous content
// is cleared, but its range (0, max) is not modified
// Return: 1 if success or 0 if failure
int set_intersect (struct set_t *s1, struct set_t *s2, struct set_t *s3){
	if(!s1 || !s2 || !s3) return 0;
	if(!s1->flag || !s2->flag || !s3->flag) return 0;
	
	for(int i = 0;i < s1->size;i++){					//faz a intersecção de s1 e s2					
		if(s1->flag[i] && s2->flag[i]){					//e coloca em s3
			s3->flag[i] = true;							
			s3->num++;	
		}
	}
	return 1;
}	

// Puts in s3 the difference s1 - s2; s3 previous content is cleared,
// but its range (0, max) is not modified
// Return: 1 if success or 0 if failure
int set_diff (struct set_t *s1, struct set_t *s2, struct set_t *s3){
	if(!s1 || !s2 || !s3) return 0;
	if(!s1->flag || !s2->flag || !s3->flag) return 0;
		
	for(int i = 0;i < s1->size;i++){					//faz a diferença de s1 e s2					
		if(s1->flag[i] && !s2->flag[i]){				//e coloca em s3
			s3->flag[i] = true;							
			s3->num++;	
		}
	}
	return 1;	
}

// Puts in s2 a copy of set s1; s2 previous content is cleared,
// but its range (0, max) is not modified
// Return: 1 if success or 0 if failure
int set_copy (struct set_t *s1, struct set_t *s2){
	if(!s1 || !s2) return 0;
	if(!s1->flag || !s2->flag) return 0;
	
	s2->num = 0;			//reseta a cardinalidade		
	for(int i = 0; i < s1->size;i++){
		s2->flag[i] = s1->flag[i];
		if(s2->flag[i]){
			s2->num++;
		}
	}
	return 1;
}

// Prints the content of a set using this template:
// [ %d %d %d ]\n
// (Does not belong to the TAD definition, but is useful for testing)
void set_print (struct set_t *s){
	int i;
	int k;
	
	if(!s || !s->flag) return;
	
	i = 0;
	k = 0;
	printf("[ ");
	while(k < s->num && i < s->size){
		if(s->flag[i]){
			printf("%d ",i);
			k++;
		}
		i++;
	}
	
	printf("]");
	printf("\n");
}
