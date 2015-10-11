#pragma one

#include "prtypes.h"
#include "prmachine.h"

void ini_alloc();
static void_ptr_t my_Heap_alloc( my_alloc_size_t how_much );
static void_ptr_t my_Str_alloc( my_alloc_size_t how_much);
dyn_ptr_t my_Dyn_alloc( my_alloc_size_t how_much); /* in bytes */
node_ptr_t ** my_Trail_alloc();
subst_ptr_t my_Subst_alloc( my_alloc_size_t how_much); /* in bytes */
temp_ptr_t my_Temp_alloc( my_alloc_size_t how_much); /* in bytes */
char *my_alloc( my_alloc_size_t how_much, int status);
long offset_subst( subst_ptr_t substptr);
string_ptr_t get_string(my_alloc_size_t how_much,int status);
real_ptr_t get_real(int status);
atom_ptr_t get_atom(int status);
pair_ptr_t get_pair(int status)
node_ptr_t get_node(int status)

clause_ptr_t get_clause(int status);

pred_rec_ptr_t get_pred();
void alloc_err( char *s);
void fatal_alloc_err( char *s);
int check_object( char *objptr);
void clean_temp();
void clean_pred( atom_ptr_t atomptr);
void space_left(zone_size_t *ph,zone_size_t *pstr,zone_size_t *pd,zone_size_t *ps,zone_size_t *ptr, *pte);
{
	*ph = ADDRESS_DIFF(HighHeap_ptr, Heap_ptr);
	*pstr = ADDRESS_DIFF(HighStr_ptr, Str_ptr);
	*pd = ADDRESS_DIFF(HighDyn_ptr, Dyn_ptr);
	*ps = ADDRESS_DIFF(HighTrail_ptr, Trail_ptr);
	*ptr = ADDRESS_DIFF(HighSubst_ptr, Subst_ptr);
	*pte = ADDRESS_DIFF(HighTemp_ptr, Temp_ptr);
}

void ini_globals();
