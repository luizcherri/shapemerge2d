#include "boolean_op.h"
#include <math.h>

namespace shapemerge2d
{
BooleanOp::BooleanOp()
{
}
void BooleanOp::mark_side(int side_,Edge* edge_)
{
	if (edge_->side[side_]!=NULL)
		return; //already marked
	Edge* curedge=edge_;
	Cell* cell=new Cell();
	Vertex last_cell_end=edge_->v1;

	for(;;)
	{
		int markside=side_; //0 is left side,1 is right side
		if (last_cell_end==curedge->v2)
		{
			markside+=1;
			last_cell_end=curedge->v1;
		}
		else
		{
			if (last_cell_end!=curedge->v1)
				throw std::runtime_error("Unexpected error!");
			assert(last_cell_end==curedge->v1);
			last_cell_end=curedge->v2;
		}
		markside&=1;
		assert(curedge->side[markside]==NULL);
		curedge->side[markside]=cell;
		cell->edges.insert(curedge);
		if (curedge->side[0] && curedge->side[1]) //Cells on both sides of edge have been found.
		{
			curedge->side[0]->neighbors[curedge->side[1]].insert(curedge);
			curedge->side[1]->neighbors[curedge->side[0]].insert(curedge);
		}
		Edge* next=get_out_edge_from_in_edge(last_cell_end,curedge,side_);
		if (next==edge_)
			return;//finished
		curedge=next;
	}
}
struct EdgeSorter
{
	Vertex v;
	Edge* curedge;
	int side;
	Vector vec;
	double avec;
	EdgeSorter(Vertex pv,Edge* pcuredge,int side) : v(pv), curedge(pcuredge),side(side)
	{
		if (curedge->v1==v)
		{
			vec=v-curedge->v2;
		}
		else
		{
			assert(curedge->v2==v);
			vec=v-curedge->v1;
		}
		avec=atan2(vec.y,vec.x);
	}
	bool operator()(Edge* a,Edge* b) const
	{
		Vector va;
		if (a->v1==v)
		{
			va=v-a->v2;
		}
		else
		{
			assert(a->v2==v);
			va=v-a->v1;
		}
		Vector vb;
		if (b->v1==v)
		{
			vb=v-b->v2;
		}
		else
		{
			assert(b->v2==v);
			vb=v-b->v1;
		}
		double aa=atan2(va.y,va.x);
		double ab=atan2(vb.y,vb.x);
		double daa=avec-aa;
		double dab=avec-ab;
		while (daa<0) daa+=2*M_PI;
		while (daa>=2*M_PI) daa-=2*M_PI;
		while (dab<0) dab+=2*M_PI;
		while (dab>=2*M_PI) dab-=2*M_PI;
		bool ret=false;
		if (side==0)
			ret=daa<dab;
		else
			ret=daa>dab;
		printf("Checking if %s-%s < %s-%s (turning %s). Answer: %s\n",
				a->v1.__repr__().c_str(),
				a->v2.__repr__().c_str(),
				b->v1.__repr__().c_str(),
				b->v2.__repr__().c_str(),
				side==0 ? "left" : "right",
				ret ? "yes" : "no");
		return ret;

	}
};
static void selftest_edge_sorter()
{
	{
		Edge incoming(Vertex(0,0),Vertex(10,0));
		Edge out1(Vertex(10,0),Vertex(10,10));
		Edge out2(Vertex(10,0),Vertex(8,10));
		std::vector<Edge*> vec;
		vec.push_back(&out1);
		vec.push_back(&out2);
		EdgeSorter es(Vertex(10,0),&incoming,0);
		std::sort(vec.begin(),vec.end(),es);
		//Verify that out2<out1
		assert(vec[0]->v1==out2.v1);
		assert(vec[1]->v1==out1.v1);
		assert(vec[0]->v2==out2.v2);
		assert(vec[1]->v2==out1.v2);
	}
}
std::vector<Line2> BooleanOp::dbg_step5_sort_edges(Vertex v,Line2 incoming,std::vector<Line2> tosort,int side)
{
	std::vector<Edge> e;
	BOOST_FOREACH(auto l,tosort)
		e.push_back(Edge(l.get_v1(),l.get_v2()));
	Edge inc(incoming.get_v1(),incoming.get_v2());
	EdgeSorter es(v,&inc,side);
	std::vector<Edge*> ep;
	BOOST_FOREACH(auto& l,e)
		ep.push_back(&l);
	std::sort(ep.begin(),ep.end(),es);

	std::vector<Line2> ret;
	BOOST_FOREACH(auto& edg,ep)
		ret.push_back(Line2(edg->v1,edg->v2));
	return ret;
}

Edge* BooleanOp::get_out_edge_from_in_edge(Vertex v,Edge* curedge,int side_)
{
	std::set<Edge*>& candidates_set=edgemap[v];
	std::vector<Edge*> candidates;
	candidates.assign(candidates_set.begin(),candidates_set.end());
	EdgeSorter es(v,curedge,side_);
	std::sort(candidates.begin(),candidates.end(),es);
	BOOST_FOREACH(Edge* cand,candidates)
	{
		if (cand==curedge) continue;
		return cand;
	}
	throw std::runtime_error("Find no outgoing edge from vertex");
	return NULL;
}

#if 0
struct LeftmostEdge
{
	bool operator()(const Cell* a,const Edge* b) const
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

typedef std::pair<Vertex,std::set<Edge*> > p_t;
void BooleanOp::step4_eliminate_deadends()
{
	std::vector<Vertex> v_remove;
	std::vector<VertexPair> vp_remove;
	BOOST_FOREACH(const p_t& pair_,edgemap)
	{
		const Vertex& v=pair_.first;
		const std::set<Edge*>& edges=pair_.second;
		if (edges.size()==1)
		{
			const Edge* edge=*edges.begin();
			v_remove.push_back(v);
			vp_remove.push_back(VertexPair(edge->v1,edge->v2));
		}
	}
	BOOST_FOREACH(Vertex v,v_remove)
	{
		vertices.erase(v);
		edgemap.erase(v);
	}
	BOOST_FOREACH(VertexPair vp,vp_remove)
		pair2edge.erase(vp);
}

/*static Vertex leftmost_vertex_of(const Edge& e)
{
	if (e.v1.x<e.v2.x || (e.v1.x==e.v2.x && e.v1.y<e.v2.y))
		return e.v1;
	return e.v2;
}*/
std::vector<Cell> BooleanOp::dbg_step5_get_cells()
{
	selftest_edge_sorter();
	std::set<Cell*> cells;
	auto it=pair2edge.begin();
	auto it2=pair2edge.end();
	for(;it!=it2;++it)
	{
		BOOST_FOREACH(auto cell,it->second.side)
			cells.insert(cell);
	}
	std::vector<Cell> cellv;
	BOOST_FOREACH(auto cell,cells)
		cellv.push_back(*cell);
	return cellv;
}

static Rational abs(Rational x)
{
	if (x<0)
		return -x;
	return x;
}
void BooleanOp::step6_determine_cell_cover()
{
	if (vertices.size()==0) throw std::runtime_error("No vertices, can't determine cell cover");
	Vertex leftmost=*vertices.begin();
	BOOST_FOREACH(Vertex v,vertices)
	{
		if (v.x<leftmost.x) leftmost=v;
		else if (v.x==leftmost.x && v.y<leftmost.y)
			leftmost=v;
	}
	std::set<Edge*>& edges=edgemap[leftmost];
	assert(edges.size()>0);
	Edge* startedge=NULL;
	BOOST_FOREACH(Edge* edge,edges)
	{
		if (startedge==NULL) startedge=edge;
		if (edge->line_is_vertical)
		{
			//Any vertical line at the leftmost vertex, *MUST* be an outer edge
			startedge=edge;
			break;
		}
		if (abs(edge->line_k)>abs(startedge->line_k))
			startedge=edge;
	}
	assert(startedge!=NULL);
	int sid=0;
	if (startedge->side[0]!=NULL) sid=0;
	if (startedge->side[1]!=NULL) sid=1;
	if (startedge->side[sid]==NULL) throw std::runtime_error("Starting edge is not adjacent to a cell! Strange!");
	Cell* curcell=startedge->side[sid];
	std::set<const Polygon*> curpolys=startedge->polys;
	std::set<Cell*> visited_cells;
	recurse_determine_cover(curcell,curpolys,visited_cells);
}
void BooleanOp::recurse_determine_cover(Cell* curcell,std::set<const Polygon*> curpolys,std::set<Cell*>& visited)
{
	curcell->cover=curpolys;
	BOOST_FOREACH(Edge* border,curcell->edges)
	{
		Cell* othercell=NULL;
		if (border->side[0]==curcell)
		{
			othercell=border->side[1];
		}
		else
		{
			assert(border->side[1]==curcell);
			othercell=border->side[0];
		}
		if (visited.find(othercell)!=visited.end())
			continue; //already visited
		std::set<const Polygon*> adjusted=curpolys;
		BOOST_FOREACH(const Polygon* poly,border->polys)
		{
			std::set<const Polygon*>::iterator it;
			if ((it=adjusted.find(poly))==adjusted.end())
			{
				adjusted.insert(poly);
			}
			else
			{
				adjusted.erase(it);
			}
			recurse_determine_cover(othercell,adjusted,visited);
		}
	}
}
void BooleanOp::step1_add_lines(Shape* shape_a,Shape* shape_b)
{
	BOOST_FOREACH(const Polygon& poly_a,shape_a->get_polys())
	{
		tagmap.push_back(&poly_a);
		BOOST_FOREACH(Line2 line_a,poly_a.get_lines())
		{
			line_a.add_tag(tagmap.size()-1);
			all_lines.push_back(line_a);
		}
	}
	BOOST_FOREACH(const Polygon& poly_b,shape_b->get_polys())
	{
		tagmap.push_back(&poly_b);
		BOOST_FOREACH(Line2 line_b,poly_b.get_lines())
		{
			line_b.add_tag(tagmap.size()-1);
			all_lines.push_back(line_b);
		}
	}

}
void BooleanOp::step2_intersect_lines()
{

	std::vector<Line2>::iterator ita=all_lines.begin();
	for(;ita!=all_lines.end();++ita)
	{
		std::vector<Line2>::iterator itb=all_lines.begin();
		for(;itb!=all_lines.end();++itb)
		{
			if (ita==itb) continue;
			std::vector<Vertex> ret=ita->intersection_points(*itb);
			BOOST_FOREACH(Vertex v,ret)
			{
				//std::set<Line2*>& lines_of_vertex=vmap[v];
				//lines_of_vertex.insert(&*ita);
				//lines_of_vertex.insert(&*itb);
				lmap[&*ita].insert(v);
				lmap[&*itb].insert(v);
				vertices.insert(v);
			}
		}
		//start and end vertex of line
		Vertex start=ita->get_v1();
		Vertex end=ita->get_v2();
		auto& lmapcont=lmap[&*ita];
		lmapcont.insert(start);
		lmapcont.insert(end);
		assert(lmapcont.size()>=2);
		//vmap[start].insert(&*ita);
		//vmap[end].insert(&*ita);
	}
	BOOST_FOREACH(auto c,lmap)
	{
		assert(c.second.size()>=2);
	}
}

std::vector<Edge> Cell::dbg_get_edges()
{
	std::vector<Edge> out;
	BOOST_FOREACH(auto e,edges)
		out.push_back(*e);
	return out;
}

std::vector<Edge> BooleanOp::dbg_step3_and_4_get_edges()
{
	std::vector<Edge> edges;
	BOOST_FOREACH(auto x,pair2edge)
	{
		edges.push_back(x.second);
	}
	return edges;
}
std::vector<Line2> BooleanOp::dbg_step2_get_split_lines()
{
	std::vector<Line2> out;
	BOOST_FOREACH(Line2& l,all_lines)
	{
		assert(lmap.find(&l)!=lmap.end());
		std::set<Vertex>& tver=lmap[&l];
		std::vector<Vertex> ver;
		ver.assign(tver.begin(),tver.end());
		std::sort(ver.begin(),ver.end(),SortAlongLine(l));
		assert(ver.size()>=2); //always at least start and end of line
		for(int i=1;i<(int)ver.size();++i)
		{
			Vertex v1=ver[i-1];
			Vertex v2=ver[i];
			out.push_back(Line2(v1,v2));
		}
	}
	return out;
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
			edge.v1=v1;
			edge.v2=v2;
			edge.line_k=l.get_k();
			edge.line_is_vertical=l.is_vertical();
			edgemap[v1].insert(&edge);
			edgemap[v2].insert(&edge);
			BOOST_FOREACH(int tag,l.get_tags())
			{
				assert(tag>=0 && tag<(int)tagmap.size());
				edge.polys.insert(tagmap[tag]);
			}
		}
	}
}
void BooleanOp::step5_create_cells()
{
	//Now we have net of vertexes and edges,
	//with polygon ownership of the edges.
	//We just need to find out which polygons cover
	//which cells.
	auto it=pair2edge.begin();
	auto it2=pair2edge.end();
	for(;it!=it2;++it)
	{
		mark_side(0,&(it->second));
		mark_side(1,&(it->second));
	}
}

/*
Shape boolean_add(const Shape& a,const Shape& b)
{
	BooleanOp bop(&a,&b,BooleanOp::addition);
	return bop.addition();
}
*/

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
	bool operator()(const Edge* e1,const Edge* e2) const
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
}
