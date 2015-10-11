void ini_copy();
clause_ptr_t copy_clause(int status,node_ptr_t nodeptr,subst_ptr_t substptr,atom_ptr_t* predptr);
void copy_node(int status,node_ptr_t target,node_ptr_t source, subst_ptr_t substptr);
int do_assertz(int status, node_ptr_t nodeptr, subst_ptr_t substptr);
int do_asserta(int status, node_ptr_t nodeptr, subst_ptr_t substptr);
int do_assertn(int status, node_ptr_t nodeptr, subst_ptr_t substptr, integer n);
int remove_clause(atom_ptr_t atomptr,/* the predicate */ clause_ptr_t clauseptr/* remove this */ );
int add_as_nth(clause_ptr_t clauseptr,atom_ptr_t pred, integer n);
