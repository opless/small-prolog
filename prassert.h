/* prassert.c */
/* implementation of assertz, asserta and clause handling
 */
#include <stdio.h>
#include <string.h>

#include "prtypes.h"
#include "prlex.h"
#include "prunify.h"
#include "prmachine.h"
#include "prcnsult.h"
#include "prassert.h"

#define BADCOPYTYPE "Illegal data type in assert"
#define NOTALIST "You did not give me a list"
#define TAILNOTLIST "The tail is not a list"
#define HEADNOTLIST "The head is not a list"

#ifndef SEGMENTED_ACHITECTURE
#define IS_DYNAMIC(X) ((Dyn_mem <= (dyn_ptr_t)(X)) && ((dyn_ptr_t)(X)<HighDyn_ptr))
#else
#define IS_DYNAMIC(X) 1 /* implies you must make a fresh copy of each object */
#endif

extern node_ptr_t DerefNode, NilNodeptr;
extern subst_ptr_t DerefSubst;
extern atom_ptr_t Nil;
extern dyn_ptr_t Dyn_mem, HighDyn_ptr;

static char * VarSubsts[MAX_VAR];
static varindx NVar_copied;

void ini_copy()
{
	NVar_copied = 0;
}

/**********************************************************************
			copy_clause() 
 *********************************************************************/
clause_ptr_t copy_clause(status, nodeptr, substptr, predptr)
node_ptr_t nodeptr;/* represents the body of the clause */
subst_ptr_t substptr;
atom_ptr_t *predptr;
{
	void copy_node();
	clause_ptr_t clauseptr, make_clause();
	node_ptr_t clause_head, clause_tail, get_node();
	objtype_t type;

	ini_copy();
	dereference(nodeptr, substptr);
	nodeptr = DerefNode;
	substptr = DerefSubst;

	if(NODEPTR_TYPE(nodeptr) != PAIR)
	{
		errmsg(NOTALIST);
		return(NULL);
	}

	dereference(NODEPTR_HEAD(nodeptr), substptr);
	type = NODEPTR_TYPE(DerefNode);
	if(type != PAIR)
	{
		if(type == ATOM)
		{
			clause_head = get_node(status);
			copy_node(status, clause_head, nodeptr, substptr);
			clause_tail = NilNodeptr;
			clauseptr = make_clause(clause_head, clause_tail, status, predptr);
			return(clauseptr);
		}
		else
			return(NULL);
	}
	else
		clause_head = get_node(status);
	copy_node(status, clause_head, DerefNode, DerefSubst);
	dereference(NODEPTR_TAIL(nodeptr), substptr);

	if(IS_NIL(DerefNode))
	{
		clause_tail = NilNodeptr;
	}
	else
		if(NODEPTR_TYPE(DerefNode) != PAIR)
		{
			errmsg(TAILNOTLIST);
			return(NULL);
		}
		else
		{
			clause_tail = get_node(status);
			copy_node(status, clause_tail, DerefNode, DerefSubst);
		}

	clauseptr = make_clause(clause_head, clause_tail, status, predptr);
	return(clauseptr);
}

/*********************************************************************
			copy_node()
 **********************************************************************/
void copy_node(status, target, source, substptr)
node_ptr_t source, target;
subst_ptr_t substptr;
{
	objtype_t type;
	string_ptr_t stringptr, s, get_string();
#ifdef REAL
	real_ptr_t realptr, get_real();
#endif
	pair_ptr_t pairptr, get_pair();
	char *molec;
	int i;

	type = NODEPTR_TYPE(source);
	NODEPTR_TYPE(target) = type;

	switch(type)
	{
	case ATOM:
		NODEPTR_ATOM(target) = NODEPTR_ATOM(source);
		break;

	case VAR:
		molec = NODEPTR_OFFSET(source) + (char *)substptr;
		for(i = 0; i < NVar_copied; i++)/*search molec in Varsubsts */
		{
			if(molec == VarSubsts[i])break;
		}

		if(i == NVar_copied)/* it's new */
		{
			VarSubsts[i] = molec;
			NVar_copied++;
		}

		NODEPTR_OFFSET(target) = i * sizeof(struct subst);
		break;

	case INT:
		NODEPTR_INT(target) = NODEPTR_INT(source);
		break;
#ifdef CHARACTER
	case CHARACTER:
		NODEPTR_CHARACTER(target) = NODEPTR_CHARACTER(source);
		break;
#endif
#ifdef REAL
	case REAL:
		if(IS_DYNAMIC(NODEPTR_REALP(source)))
		{
			realptr = get_real(status);
			*realptr = NODEPTR_REAL(source);
			NODEPTR_REALP(target) = realptr;
		}
		else
		{
			NODEPTR_REALP(target) = NODEPTR_REALP(source);
		}
		break;
#endif
	case PAIR:
		pairptr = get_pair(status);
		NODEPTR_PAIR(target) = pairptr;
		dereference(NODEPTR_HEAD(source), substptr);
		copy_node(status, NODEPTR_HEAD(target), DerefNode, DerefSubst);
		dereference(NODEPTR_TAIL(source), substptr);
		copy_node(status, NODEPTR_TAIL(target), DerefNode, DerefSubst);
		break;

	case STRING:
		s = NODEPTR_STRING(source);
		if(IS_DYNAMIC(s))
		{
			if(status == PERMANENT)status = PERM_STRING;
			stringptr = get_string((my_alloc_size_t)(strlen(s) + 1), status);
			strcpy(stringptr, s);
			NODEPTR_STRING(target) = stringptr;
		}
		else
			NODEPTR_STRING(target) = s;
		break;
	default:
		errmsg(BADCOPYTYPE);
	}
}

/*********************************************************************
 			do_assertz()
 For the assertz builtin.
 *********************************************************************/
int do_assertz(int status, node_ptr_t nodeptr, subst_ptr_t substptr)
{
	clause_ptr_t clauseptr;
	atom_ptr_t pred;

	clauseptr = copy_clause(status, nodeptr, substptr, &pred);

	if(clauseptr == NULL)
		return(0);

	add_to_end(clauseptr, pred);
	return(1);
}

/*********************************************************************
 			do_asserta()
 For the asserta builtin.
 *********************************************************************/
int do_asserta(int status, node_ptr_t nodeptr, subst_ptr_t substptr)
{
	clause_ptr_t clauseptr;
	atom_ptr_t pred;

	clauseptr = copy_clause(status, nodeptr, substptr, &pred);

	if(clauseptr == NULL)
		return(0);

	record_pred(pred);
	CLAUSEPTR_NEXT(clauseptr) = ATOMPTR_CLAUSE(pred);
	ATOMPTR_CLAUSE(pred) = clauseptr;
	return(1);
}

/******************************************************************************
 			do_assertn()
 asserts a clause at the nth position if it can
 n begins at 1.
 ******************************************************************************/
int do_assertn(int status, node_ptr_t nodeptr, subst_ptr_t substptr, integer n)

{
	clause_ptr_t clauseptr;
	atom_ptr_t pred;

	if (n < 1)
		return 0;
	if (n == 1)
		return(do_asserta(status, nodeptr, substptr));

	clauseptr = copy_clause(status, nodeptr, substptr, &pred);
	if (clauseptr == NULL)
		return 0;

	return (add_as_nth(clauseptr, pred, n));
}
/******************************************************************************
			remove_clause()
 ******************************************************************************/
int remove_clause(atom_ptr_t atomptr,/* the predicate */ clause_ptr_t clauseptr/* remove this */ )
{
	if (clauseptr == ATOMPTR_CLAUSE(atomptr))
	{
		ATOMPTR_CLAUSE(atomptr) = CLAUSEPTR_NEXT(clauseptr);
		return 1;
	}
	else
	{
		clause_ptr_t clp, previous;

		clp = ATOMPTR_CLAUSE(atomptr);
		for(;;)
		{
			previous = clp;
			clp = CLAUSEPTR_NEXT(clp);
			if(clp == NULL)
				return 0;
			if (clp == clauseptr)
			{
				CLAUSEPTR_NEXT(previous) = CLAUSEPTR_NEXT(clp);
				return 1;
			}
		}
	}
}

/******************************************************************************
 			add_as_nth()
 Tries to add a clause in the nth position of its packet.
 Returns 0 iff unsuccessful.
 ******************************************************************************/
int add_as_nth(clause_ptr_t clauseptr,atom_ptr_t pred, integer n)
{
	clause_ptr_t clp;

	clp = ATOMPTR_CLAUSE(pred);
	--n;

	while (--n >= 0)
	{
		clp = CLAUSEPTR_NEXT( clp);
		if( clp == NULL)
			return 0;
	}
	CLAUSEPTR_NEXT( clauseptr) = CLAUSEPTR_NEXT( clp );
	CLAUSEPTR_NEXT( clp) = clauseptr;
	return 1;
}
/* end of file */
