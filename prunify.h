#pragma once

int unify(node_ptr_t node1ptr, subst_ptr_t subst1ptr,node_ptr_t  node2ptr, subst_ptr_t subst2ptr);

int bind_var(node_ptr_t node1ptr,subst_ptr_t subst1ptr,node_ptr_t node2ptr,subst_ptr_t subst2ptr);
void reset_trail(node_ptr_t **from);
int dereference(node_ptr_t nodeptr,subst_ptr_t substptr);
int occur_check(node_ptr_t node1ptr,subst_ptr_t subst1ptr,node_ptr_t node2ptr,subst_ptr_t subst2ptr);

