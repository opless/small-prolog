clause_ptr_t make_clause(node_ptr_t clhead, node_ptr_t clgoals, int status,atom_ptr_t* predptr);
int do_consult(char *infilename,int status);
int load(char *s);
void add_to_end(clause_ptr_t clauseptr,atom_ptr_t pred);
void record_pred(atom_ptr_t atomptr);
void do_listing();
