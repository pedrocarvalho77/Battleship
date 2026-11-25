#include "quadtree.h"

// A node covers the square [corner, corner+side) in both axes; side is a power of 2.
// Leaves live at side == 1 and represent one map cell.

QD_NODE* new_qd_node(QD_TNODE type){

	QD_NODE* qd_node = (QD_NODE*) calloc(1, sizeof(QD_NODE));

	if(qd_node == NULL){
		printf("Error\n");
		exit(0);
	}
	qd_node->type = type;

	return qd_node;
}

static int inside(POINT* p, POINT* corner, int side){
	return get_x(p) >= get_x(corner) && get_x(p) < get_x(corner) + side &&
	       get_y(p) >= get_y(corner) && get_y(p) < get_y(corner) + side;
}

// returns the leaf at p, or NULL if there is none
QD_NODE* search(QD_NODE* qtree, POINT* p, POINT* corner, int side){

	int cx = get_x(corner);
	int cy = get_y(corner);

	while(qtree != NULL){
		POINT c = {cx, cy};
		if(!inside(p, &c, side)) return NULL;
		if(side == 1) return qtree->type == QDLEAF ? qtree : NULL;
		int half = side/2;
		int q = (get_x(p) >= cx + half) + 2*(get_y(p) >= cy + half);
		if(q & 1) cx += half;
		if(q & 2) cy += half;
		side = half;
		qtree = qtree->node.quadrants[q];
	}
	return NULL;
}

// inserts a leaf at p; returns 1 if inserted, 0 if outside the tree or already present
int insert(QD_NODE* qtree, POINT* p, POINT* corner, int side){

	int cx = get_x(corner);
	int cy = get_y(corner);
	POINT c = {cx, cy};

	if(qtree == NULL || !inside(p, &c, side)) return 0;

	while(side > 1){
		int half = side/2;
		int q = (get_x(p) >= cx + half) + 2*(get_y(p) >= cy + half);
		if(q & 1) cx += half;
		if(q & 2) cy += half;
		side = half;
		if(qtree->node.quadrants[q] == NULL){
			qtree->node.quadrants[q] = new_qd_node(QDNODE);
		}
		qtree = qtree->node.quadrants[q];
	}
	if(qtree->type == QDLEAF) return 0;
	qtree->type = QDLEAF;
	qtree->node.leaf.p = make_point(get_x(p), get_y(p));
	qtree->node.leaf.field_pointer = new_cell();
	set_state(qtree->node.leaf.field_pointer, '1');
	return 1;
}

// marks the piece in the leaf as hit
int change_field_pointer(QD_NODE* qtree){

	if(qtree == NULL || qtree->type != QDLEAF) return 0;
	qtree->node.leaf.field_pointer->state = '2';
	return 1;
}
