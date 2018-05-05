
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"
#include "stdlib.h"
#include <vector>
/* given */
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)
	{}

/* given */
twoDtree::~twoDtree(){
	clear();
}

/* given */
twoDtree::twoDtree(const twoDtree & other) {
	copy(other);
}

/* given */
twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}

twoDtree::twoDtree(PNG & imIn){
	this->height = imIn.height();
	this->width = imIn.width();
	stats st(imIn);
	this->root = buildTree(st,pair<int,int>(0,0), pair<int,int>(imIn.width()-1,imIn.height()-1));

}

twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr) {
	Node *n =new Node(ul,lr,s.getAvg(ul,lr));
//	printf("in buildTree ul: %d %d | lr: %d %d\n",ul.first, ul.second, lr.first, lr.second);
	if(ul == lr) {
	//	printf("case 0\n");
		n->left = NULL;
		n->right = NULL;
		return n;
	} else if (ul.first+1 == lr.first && ul.second+1 == lr.second){
//	printf("case 3\n");
		pair<int, int> p(ul.first,lr.second);
		pair<int, int> q(lr.first,ul.second);
		n->left = buildTree(s, ul, q);
		n->right = buildTree(s,p,lr);
	} else if (ul.first+1 == lr.first && ul.second == lr.second){
	//	printf("in case 1\n");
		n->left = buildTree(s,ul,ul);
		n->right = buildTree(s, lr,lr);
	} else if (ul.first == lr.first && ul.second+1 == lr.second){
//	printf("case 4\n");
		n->left = buildTree(s,ul,ul);
		n->right = buildTree(s, lr,lr);
	}
	else {
	//	printf("case 2\n");
		//find split line
		unsigned long long min = s.getScore(ul,lr) * 2;
		pair<int,int> minp;
		pair<int,int> minq;

	//	printf("haha after get first score, min = %llu\n", min);

		for(int i = get<1>(ul); i< get<1>(lr); i++){
			pair<int,int> p(get<0>(lr),i);
			pair<int,int> q(get<0>(ul),i+1);
	//		printf("in first for\n");
		unsigned long	long st = s.getScore(ul,p)+s.getScore(q,lr);
			if(st<=min){
				minp = p;
				minq = q;
				min = st;
			}
		}
		for (int i = get<0>(ul); i < get<0>(lr); i++) {
			pair<int,int> p(i,get<1>(lr));
			pair<int,int> q(i+1,get<1>(ul));
//			printf("in second for\n");
		unsigned long	long st = s.getScore(ul,p)+s.getScore(q,lr);
//			printf("st = %llu\n", st);
			if(st<=min){
				minp = p;
				minq = q;
				min = st;
			}
		}
	//	printf("ul %d %d | minp %d %d | minq %d %d | lr %d %d \n", ul.first, ul.second, minp.first, minp.second, minq.first, minq.second, lr.first, lr.second);
		n->left = buildTree(s, ul, minp);
		n->right = buildTree(s, minq, lr);

	}
	return n;

}

PNG twoDtree::render(){
	/* your code here */
//    cout<<(int)root->avg.r<<endl;
//    cout<<(int)root->avg.g<<endl;
//    cout<<(int)root->avg.b<<endl;
    PNG image = PNG(width,height);
//		printf("%lf\n",countleaves(root));
		getleaves(root);
//		printf("after get leaves\n");
    for(int i=0;i<ns.size();i++) {
	//		printf("ul %d %d | lr %d %d\n",ns[i]->upLeft.first, ns[i]->upLeft.second, ns[i]->lowRight.first, ns[i]->lowRight.second);
			if(ns[i]->upLeft.first == ns[i]->lowRight.first && ns[i]->upLeft.second == ns[i]->lowRight.second)
				  *image.getPixel(ns[i]->upLeft.first,ns[i]->upLeft.second)=ns[i]->avg;
			else {
				for(int y = get<1>(ns[i]->upLeft); y <= get<1>(ns[i]->lowRight); y++) {
						for(int x = get<0>(ns[i]->upLeft); x <= get<0>(ns[i]->lowRight); x++) {
								*image.getPixel(x,y)=ns[i]->avg;
						}
				}
			}
//        cout<<(int)ns[i]->avg.r<<endl;
//        cout<<(int)ns[i]->avg.g<<endl;
//        cout<<(int)ns[i]->avg.b<<endl;
    }
		ns.clear();
    return image;
}

void twoDtree::getleaves(Node* n){
	if(n == NULL) return;
	if(n->left == NULL && n->right == NULL){
		ns.push_back(n);
	}
	if (n->left != NULL) getleaves(n->left);
	if(n->right != NULL) getleaves(n->right);
}



void twoDtree::prune(double pct, int tol){
	/* your code here */
    prune2(root->left,pct,tol);
		prune2(root->right,pct,tol);
}

void twoDtree::prune2(Node* node, double pct, int tol) {
	//	printf("count  %lf\n", countleaves(node));
    if (node != NULL) {
        if (pruneHelper(node,pct,tol)) {
          if(node->left != NULL) clearHelper(node->left);
					if(node->right != NULL) clearHelper(node->right);
        }
        else {
            prune2(node->left,pct,tol);
            prune2(node->right,pct,tol);
        }
    }
}


bool twoDtree::pruneHelper(Node* node, double pct, int tol){
    bool p;
  //  ns.clear();
		double countleaf = countleaves(node);
    double accept = 0;
  //  helper(node);
		accept = helper(node, node->avg,tol);
	//	printf("accept size %lu ns size %lu\n",accept.size(),ns.size());
    double r = accept/countleaf;
    if (r >= pct) {
        p = true;

    }
    else{
        p = false;
    }
    return p;
}

double twoDtree::countleaves(Node* node){
	if (node == NULL) return 0;
	if(node->left == NULL && node->right == NULL){
		return 1;
	}
	return countleaves(node->left)+countleaves(node->right);

}
double twoDtree::helper(Node* node, RGBAPixel pixel, int tol){
    if (node!=NULL){
        if( node->left == NULL && node->right == NULL ) {
					if ((node->avg.r - pixel.r)*(node->avg.r - pixel.r)+(node->avg.g - pixel.g)*(node->avg.g - pixel.g)+(node->avg.b - pixel.b)*(node->avg.b - pixel.b) <= tol) {
							return 1;
						}
					else return 0;
        }
        else {
        	return helper(node->left,pixel,tol)+helper(node->right,pixel,tol);
        }
    }
		return 0;
}

void twoDtree::clear() {
	/* your code here */
    height = 0;
    width = 0;
    clearHelper(root);
}


void twoDtree::clearHelper(Node*& subtree) {
		if(subtree->left != NULL) clearHelper(subtree->left);
		if(subtree->right != NULL) clearHelper(subtree->right);
    delete subtree;
    subtree = NULL;
}

void twoDtree::copy(const twoDtree & orig){
	/* your code here */
    height = orig.height;
    width = orig.width;
    if (orig.root !=NULL) {
    pair<int,int> newUpLeft(get<0>(orig.root->upLeft),get<1>(orig.root->upLeft));
    pair<int,int> newLowRight(get<0>(orig.root->lowRight),get<1>(orig.root->lowRight));
    RGBAPixel newAvg = orig.root->avg;
    Node* n = new Node(newUpLeft,newLowRight,newAvg);
    copyHelper(orig.root,n);
    root = n;
  }
}

void twoDtree::copyHelper(Node* r, Node* treeNewRoot) {
    if (r->left != NULL)
    {
        pair<int,int> newUpLeft(get<0>(r->left->upLeft),get<1>(r->left->upLeft));
        pair<int,int> newLowRight(get<0>(r->left->lowRight),get<1>(r->left->lowRight));
        RGBAPixel newAvg = r->left->avg;
        Node* treeNewLeft = new Node(newUpLeft,newLowRight,newAvg);
        treeNewRoot->left=treeNewLeft;
        copyHelper(r->left,treeNewRoot->left);
}

    if (r->right != NULL)
    {
        pair<int,int> newUpLeft(get<0>(r->right->upLeft),get<1>(r->right->upLeft));
        pair<int,int> newLowRight(get<0>(r->right->lowRight),get<1>(r->right->lowRight));
        RGBAPixel newAvg = r->right->avg;
        Node* treeNewRight = new Node(newUpLeft,newLowRight,newAvg);
        treeNewRoot->right=treeNewRight;
        copyHelper(r->right,treeNewRoot->right);
    }
}
