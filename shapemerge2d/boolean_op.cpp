#include "boolean_op.h"
#include <math.h>

namespace shapemerge2d
{
BooleanOp::BooleanOp()
{
	result=NULL;
	num_merged_polys=0;
}
void BooleanOp::mark_side(int side_,Edge* edge_)
{
	if (edge_->side[side_]!=NULL)
		return; //already marked
	Edge* curedge=edge_;
	Cell* cell=new Cell();
	cells.push_back(cell);
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
Vertex BooleanOp::get_leftmost() const
{
	if (e.v1.x<e.v2.x || (e.v1.x==e.v2.x && e.v1.y<e.v2.y))
		return e.v1;
	return e.v2;
}

void BooleanOp::step9_calc_result()
{
	for(int cur_merged_poly=0;cur_merged_poly<num_merged_polys;++cur_merged_poly)
	{
		if (pair2edge.size()==0) throw std::runtime_error("No edges, can't calculate result");
		Edge* leftmost=NULL;

		BOOST_FOREACH(auto eitem,pair2edge)
		{
			Edge* e=&(eitem.second);
			assert(e->side[0] && e->side[1]);
			if (!(e->side[0].merged_poly==cur_merged_poly ||
				  e->side[1].merged_poly==cur_merged_poly))
				continue;
			Vertex v=e->get_leftmost();
			//printf("Leftmost Vertex of edge: %s\n",v.__repr__().c_str());
			if (leftmost==NULL) leftmost=e
			else if (v.x<leftmost.x) leftmost=e;
			else if (v.x==leftmost->x && e->line_k<leftmost->line_k)
				leftmost=e;
		}
		assert(leftmost!=NULL);

		//The 'leftmost' edge is here known to be
		//at the edge of a coherent SOLID body,
		//and also to be headed away on a counter clockwise
		//trip around that solid object

		Vertex curvertex=leftmost.get_leftmost();
		Edge* curedge=leftmost;
		for(;;)
		{
			bool reversed=(curvertex==curedge->v2);
			Vertex nextvertex=(reversed) ? curedge->v1 : curedge->v2;
			auto edgemap_it=edgemap.find(nextvertex);
			assert(edgemap_it!=edgemap.end());
			BOOST_FOREACH(Edge* candidate,edgemap_it->second)
			{
				find the one and only edge with boundary
				to cell with merged_poly_nr
			}
		}
	}

}
Shape* BooleanOp::step9_get_result()
{
	return result;
}

void BooleanOp::step6_determine_cell_cover()
{
	if (vertices.size()==0) throw std::runtime_error("No vertices, can't determine cell cover");
	Vertex leftmost=*vertices.begin();
	BOOST_FOREACH(Vertex v,vertices)
	{
		printf("Vertex: %s\n",v.__repr__().c_str());
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
	if (!(startedge->line_is_vertical) && abs(startedge->line_k)==0)
		throw std::runtime_error("Unexpected error - leftmost vertex has only completely horizontal edges. These should have been filtered out by dead-end filter.");

	assert(startedge!=NULL);
	assert(startedge->side[0]!=NULL && startedge->side[1]!=NULL);

	int sid=0;
	if (startedge->v1.x<startedge->v2.x)
	{
		if (startedge->v1.y<startedge->v2.y)
		{
			//   /2 |
			//  /   |
			//1/    |
			sid=0;
		}
		else
		{
			//1\    |
			//  \   |
			//   \2 |
			sid=1;
		}
	}
	else
	{
		if (startedge->v1.y<startedge->v2.y)
		{
			//2\    |
			//  \   |
			//   \1 |
			sid=0;
		}
		else
		{
			//   /1 |
			//  /   |
			//2/    |
			sid=1;
		}
	}

	Cell* curcell=startedge->side[sid];
	assert(curcell);
	std::set<const Polygon*> curpolys; //We're starting out in the void
	std::set<Cell*> visited_cells;
	recurse_determine_cover(curcell,curpolys,visited_cells);
}
void BooleanOp::recurse_determine_cover(Cell* curcell,std::set<const Polygon*> curpolys,std::set<Cell*>& visited)
{
	if (visited.find(curcell)!=visited.end())
		return; //already visited
	visited.insert(curcell);
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
			continue; //short circuit optimization, primary check is at start of function.
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
		}
		recurse_determine_cover(othercell,adjusted,visited);
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
		vertices.insert(start);
		vertices.insert(end);
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
std::vector<std::string> Cell::get_shapes()
{
	std::set<std::string> shapenames;
	BOOST_FOREACH(auto poly,cover)
	{
		const Shape* shp=poly->get_shape();
		assert(shp!=NULL);
		shapenames.insert(shp->get_name());
	}
	std::vector<std::string> shapev;
	BOOST_FOREACH(auto s,shapenames)
		shapev.push_back(s);
	return shapev;
}

std::vector<Cell> Cell::get_neighbors()
{
	std::vector<Cell> cells;
	BOOST_FOREACH(auto n,neighbors)
	{
		assert(n.first);
		cells.push_back(*n.first);
	}
	return cells;
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
void BooleanOp::step7_classify_cells(BooleanOpStrategy* strat)
{
	BOOST_FOREACH(Cell* cell,cells)
	{
		cell->classification=strat->evaluate(*cell);
	}
}
BooleanUpResult BooleanOrStrategy::evaluate(const Cell& cell)
{
	if (cell.cover.size()>0)
		return SOLID;
	return HOLE;
}
std::string Cell::get_classification()
{
	switch(classification)
	{
	case NONE: return "NONE";
	case HOLE: return "HOLE";
	case SOLID: return "SOLID";
	default:
		throw std::runtime_error("unexpected error in get_classification");
	}
	return "";
}
std::string Cell::__repr__() const
{
	std::ostringstream str;
	str<<"Cell("<<edges.size()<<" edges ";
	BOOST_FOREACH(const Polygon* cov,cover)
	{
		str<<" "<<cov->get_shape()->get_name();
	}
	str<<")";
	return str.str();
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
void BooleanOp::step8_merge_cells()
{
	num_merged_polys=0;
	BOOST_FOREACH(Cell* cell,cells)
	{
		if (cell->merged_poly!=-1) continue; //Already merged
		printf("Restart with cell %s\n",cell->__repr__().c_str());
		int cur_poly=num_merged_polys;
		++num_merged_polys;
		std::set<Cell*> merge_front;
		merge_front.insert(cell);
		while(!merge_front.empty())
		{
			std::set<Cell*> next_merge_front;
			printf("Rerunning through merge front, %d items\n",(int)merge_front.size());
			BOOST_FOREACH(auto mergecell,merge_front)
			{
				printf("Merging %s\n======================\n",
						mergecell->__repr__().c_str());
				if (mergecell->merged_poly!=-1) continue;
				mergecell->merged_poly=cur_poly;
				BOOST_FOREACH(auto neighitem,mergecell->neighbors)
				{
					Cell* neighcell=neighitem.first;
					printf("Considering %s\n",
						neighcell->__repr__().c_str());
					if (neighcell->merged_poly==-1 && neighcell->classification==cell->classification)
					{
						printf("Added it\n");
						next_merge_front.insert(neighcell);
					}
					else
					{
						printf("Did not add it\n");
					}
				}
			}
			merge_front.swap(next_merge_front);
		}
	}

}

}
