#include "boolean_op.h"
#include <math.h>

BooleanOp::BooleanOp()
{
}
#if 0
void BooleanOp::mark_side(int side_,Edge* edge_,std::set<Cell*>& cells)
{
	assert(side_==0 || side_==1);
	if (edge->side[side_]!=NULL)
		return; //already marked
	Edge* curedge=edge_;
	Cell* cell=new Cell();
	cells.insert(cell);
	Vertex last_cell_end=edge->v2;
	for(;;)
	{
		int markside=side_;
		if (last_cell_end==next->v2)
			markside+=1;
		else
		if (last_cell_end!=next->v1)
			throw std::runtime_error("Unexpected error!");
		markside&=1;
		assert(curedge->side[markside]==NULL);
		curedge->side[markside]=cell;
		cell->edges.insert(curedge);
		if (curedge->side[0] && curedge->side[1])
		{
			curedge->side[0]->neighbors[curedge->side[1]].insert(curedge);
			curedge->side[1]->neighbors[curedge->side[0]].insert(curedge);
		}
		Edge* next=get_out_edge_from_in_edge(curedge);
		if (next==edge)
			return;//finished
	}
}
struct LeftmostEdge
{
	bool operator()(const Cell* a,const Edge* b)
		{
			assert(a!=NULL && b!=NULL);
			Vertex ac=a->approx_center();
			Vertex bc=b->approx_center();
			if (ac.x<bc.x) return true;
			if (ac.x>bc.x) return false;
			return ac.y<bc.y;
		}
};
Vertex BooleanOp::leftmost_vertex() const
{
	if (vertices.size()==0)
		throw std::runtime_error("can't fetch leftmost vertex - there are no vertices at all!");
	Vertex leftmost=*(vertices.begin());
	BOOST_FOREACH(const Vertex& v,vertices)
	{
		if (v.x<leftmost.x || (v.x==leftmost.x && v.y<leftmost.y))
			leftmost=v;
	}
	return leftmost;
}

bool BooleanOp::walk_to_edge_impl(Cell* cell,std::set<Cell*>& visited)
{
	std::map<Cell*,std::set<Edge*> >::iterator it=cell->neighbors.begin();
	std::map<Cell*,std::set<Edge*> >::iterator it2=cell->neighbors.end();
	std::vector<Cell*> sorted;
	sorted.reserve(cell->neighbors.size());
	for(;it!=it2;)
	{
		sorted.push_back(it->first);
	}
	Vertex leftmost=leftmost_vertex();
	std::sort(sorted.begin(),sorted.end(),LeftmostEdge());
	BOOST_FOREACH(Cell* neighborcell,sorted)
	{
		if (visited.find(neighborcell)!=visited.end())
			continue;//already visited
		Vertex cell_leftmost=neighborcell->leftmost_vertex();
		if (cell_leftmost==leftmost)
			return true, //found edge
	}
	return false; //all were visted.
}
bool BooleanOp::walk_to_edge(Cell* cell)
{
	std::set<Cell*> visited;
	return walk_to_edge_impl(cell,visited);
}
Vertex Cell::approx_center() const
{
	if (edges.size()==0)
		throw std::runtime_error("Cell without edges has no center!");
	double sx=0,sy=0;
	BOOST_FOREACH(Edge* edge,edges)
	{
		sx+=edge->v1.x;
		sy+=edge->v1.y;
		sx+=edge->v2.x;
		sy+=edge->v2.y;
	}
	sx/=edges.size()*2;
	sy/=edges.size()*2;
	return Vertex(round(sx),round(sy));
}
static Vertex leftmost_vertex_of(const Edge& e)
{
	if (e.v1.x<e.v2.x || (e.v1.x==e.v2.x && e.v1.y<e.v2.y))
		return e.v1;
	return e.v2;
}
Vertex Cell::leftmost_vertex() const
{
	if (edges.size()==0)
		throw std::runtime_error("Cell without edges has no leftmost vertex!");
	Vertex leftmost=leftmost_vertex_of(**edges.begin());
	BOOST_FOREACH(Edge* edge,edges)
	{
		Vertex edgev=leftmost_vertex_of(*edge);
		if (edgev.x<leftmost.x || (edgev.x==leftmost.x && edgev.y<leftmost.y))
			leftmost=edgev;
	}
	return leftmost;
}
#endif

void BooleanOp::step5_eliminate_deadends()
{
	std::vector<Vertex> v_remove;
	std::vector<VertexPair> vp_remove;
	BOOST_FOREACH(std::pair<Vertex,std::set<Edge*> >& pair_,edgemap)
	{
		Vertex& v=pair_.first;
		std::set<Edge*>& edges=pair_.second;
		if (edges.size()==1)
		{
			Edge* edge=edges[0];
			v_remove.push_back(v);
			vp_remove.push_back(VertexPair(edge->v1,edge->v2));
		}
	}
	BOOST_FOREACH(Vertex v,v_remove)
	{
		vertices.remove(v);
		edgemap.remove(v);
	}
	BOOST_FOREACH(VertexPair vp,vp_remove)
		pair2edge.remove(vp);
}
void BooleanOp::step6_determine_cell_cover();
{
	BOOST_FOREACH(std::pair<VertexPair,Edge>)
}
void BooleanOp::step1_add_lines(Shape* shape_a,Shape* shape_b)
{
	BOOST_FOREACH(const Polygon& poly_a,shape_a->get_polys())
	{
		tagmap.push_back(poly_a);
		BOOST_FOREACH(Line line_a,poly_a.get_lines())
		{
			line_a.add_tag(tagmap.size()-1);
			all_lines.push_back(line_a);
		}
	}
	BOOST_FOREACH(const Polygon& poly_b,shape_b->get_polys())
	{
		tagmap.push_back(poly_b);
		BOOST_FOREACH(Line line_b,poly_b.get_lines())
		{
			line_b.add_tag(tagmap.size()-1);
			all_lines.push_back(line_b);
		}
	}

}
void BooleanOp::step2_intersect_lines()
{

	std::vector<Line2>::iterator ita=all_lines.begin();
	while(ita!=all_lines.end())
	{
		std::vector<Line2>::iterator itb=all_lines.begin();
		while(itb!=all_lines.end())
		{
			if (ita==itb) continue;
			std::vector<Vertex> ret=ita->intersection_points(*itb);
			BOOST_FOREACH(Vertex v,ret)
			{
				std::set<Line2*>& lines_of_vertex=vmap[v];
				lines_of_vertex.insert(&*ita);
				lines_of_vertex.insert(&*itb);
				lmap[&*ita].insert(v);
				lmap[&*itb].insert(v);
				vertices.insert(v);
			}
		}
		//start and end vertex of line
		Vertex start=ita->get_v1();
		Vertex end=ita->get_v2();
		lmap[&*ita].insert(start);
		lmap[&*ita].insert(end);
		vmap[start].insert(&*ita);
		vmap[end].insert(&*ita);
	}
}
void BooleanOp::step3_create_edges()
{
	BOOST_FOREACH(Line2& l,all_lines)
	{
		std::set<Vertex>& tver=lmap[&l];
		std::vector<Vertex> ver;
		ver.assign(tver.begin(),tver.end());
		std::sort(ver.begin(),ver.end(),SortAlongLine(l));
		assert(ver.size()>=2); //always at least start and end of line
		for(int i=1;i<(int)ver.size();++i)
		{
			Vertex v1=ver[i-1];
			Vertex v2=ver[i];
			VertexPair pair(v1,v2);
			Edge& edge=pair2edge[pair];
			edgemap[v1].insert(&edge);
			edgemap[v2].insert(&edge);
			BOOST_FOREACH(int tag,l.get_tags())
				edge.polys.insert(tag);
		}
	}
}
void BooleanOp::step4_create_cells()
{
	//Now we have net of vertexes and edges,
	//with polygon ownership of the edges.
	//We just need to find out which polygons cover
	//which cells.
	std::map<VertexPair,Edge>::iterator it=pair2edge.begin();
	std::map<VertexPair,Edge>::iterator it2=pair2edge.end();
	for(;it!=it2;++it)
	{
		mark_left(&(it->second));
	}



}


Shape boolean_add(const Shape& a,const Shape& b)
{
	BooleanOp bop(&a,&b,BooleanOp::addition);
	return bop.addition();
}


/*
struct Node
{
	Vertex v;
	std::set<Edge*> edges;
	void sort();
};
struct EdgeSorter
{
	Node* home;
	EdgeSorter(Node* home_) : home(home_)
	{
	}
	bool operator()(const Edge* e1,const Edge* e2)
	{
		Node* other1=e1->get_other_node(home);
		Node* other2=e2->get_other_node(home);
		double e1a=atan2(other1->v.y,other1->v.x);
		double e2a=atan2(other2->v.y,other2->v.x);
		return e1a<e2a;
	}
};
void Node::sort()
{
	EdgeSorter es(this);
	std::sort(edges.begin(),edges.end(),es);
}*/
