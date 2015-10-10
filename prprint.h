#pragma once

int prompt_user();
void pr_solution( varindx nvar, subst_ptr_t substptr);
int print_character( uchar_t c );
int out_node(node_ptr_t nodeptr, subst_ptr_t  substptr);
int out_pair( node_ptr_t listnode, subst_ptr_t substptr);
int out1_pair(node_ptr_t nodeptr,subst_ptr_t substptr);
int tty_out_node(node_ptr_t nodeptr,subst_ptr_t substptr);
int pr_node(node_ptr_t nodeptr);
int pr_pair(pair_ptr_t pairptr);
int pr1_pair(pair_ptr_t pairptr);
int pr_clause(clause_ptr_t clauseptr);
int pr_packet(clause_ptr_t clauseptr);
int maplist(node_ptr_t listnode,subst_ptr_t substptr ,intfun fn);
