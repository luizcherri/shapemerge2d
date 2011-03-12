#include "boolean_op.h"
#include <math.h>

namespace shapemerge2d
{
BooleanOp::BooleanOp()
{
	num_merged_polys=0;
	shape_a=shape_b=0;
}
/**
 * Determine if leftmost edge outer side (facing away from
 * the rest), is to left or right.
 *
 * false: left
 * true: right
 */
static bool leftmost_edge_outer_side(Line line)
{
	if (line.get_v1().x<line.get_v2().x)
	{
		if (line.get_v1().y<line.get_v2().y)
		{
			//   /2 |
			//  /   |
			//1/    |
			return false;
		}
		else
		{
			//1\    |
			//  \   |
			//   \2 |
			return true;
		}
	}
	else
	{
		if (line.get_v1().y<line.get_v2().y)
		{
			//2\    |
			//  \   |
			//   \1 |
			return false;
		}
		else
		{
			//   /1 |
			//  /   |
			//2/    |
			return true;
		}
	}
}
bool Cell::is_enveloping() const
{
	if (polygon->is_ccw())
	{
		//If the assertion below fires, the polygon may
		//contain loops (this is indicative of a bug).
		assert(polygon->naive_double_area()>=0);
		return startside==1;
	}
	else
	{
		//If the assertion below fires, the polygon may
		//contain loops (this is indicative of a bug).
		assert(polygon->naive_double_area()<=0);
		return startside==0;
	}
}

void BooleanOp::mark_side(int side_,Edge* edge_)
{
	if (edge_->side[side_]!=NULL)
		return; //already marked
	Edge* curedge=edge_;
	Cell* cell=new Cell();
	cell->startside=side_;
	cells.push_back(cell);
	Vertex last_edge_end=edge_->v1;
	//printf("Marking cell:%p last_edge_end: %s\n",cell,last_edge_end.__repr__().c_str());
	for(;;)
	{
		int markside=side_; //0 is left side,1 is right side
		cell->vertices.push_back(last_edge_end);
		if (last_edge_end==curedge->v2)
		{
			markside+=1;
			last_edge_end=curedge->v1;
		}
		else
		{
			if (last_edge_end!=curedge->v1)
				throw std::runtime_error("Unexpected error!");
			assert(last_edge_end==curedge->v1);
			last_edge_end=curedge->v2;
		}
		markside&=1;
		/*if (curedge->side[markside]!=NULL)
		{ //DBG



		}*/
		/*if (curedge->side[markside]!=NULL)
			printf("Already marked: %s[%d]\n",curedge->__repr__().c_str(),markside);*/
		assert(curedge->side[markside]==NULL);
		//printf("Marking %s[%d]=%p\n",curedge->__repr__().c_str(),markside,cell);
		curedge->side[markside]=cell;
		cell->edges.insert(curedge);
		if (curedge->side[0] && curedge->side[1]) //Cells on both sides of edge have been found.
		{
			curedge->side[0]->neighbors[curedge->side[1]].insert(curedge);
			curedge->side[1]->neighbors[curedge->side[0]].insert(curedge);
		}
		Edge* next=get_out_edge_from_in_edge(last_edge_end,curedge,side_);
		/*printf("Resolution, out-edge from %s\n   with in-edge: %s (%p)\n   is : %s (%p)\n",
				last_edge_end.__repr__().c_str(),
				curedge->__repr__().c_str(),curedge,
				next->__repr__().c_str(),next);*/
		if (next==edge_)
			return;//finished
		curedge=next;
	}
}
std::string Edge::__repr__() const
{
	std::ostringstream s;
	s<<"Edge("<<v1.__repr__()<<","<<v2.__repr__()<<")";
	return s.str();
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
		/*printf("Checking if %s-%s < %s-%s (turning %s). Answer: %s\n",
				a->v1.__repr__().c_str(),
				a->v2.__repr__().c_str(),
				b->v1.__repr__().c_str(),
				b->v2.__repr__().c_str(),
				side==0 ? "left" : "right",
				ret ? "yes" : "no");*/
		return ret;

	}
};

std::vector<Line> BooleanOp::dbg_step5_sort_edges(Vertex v,Line incoming,std::vector<Line> tosort,int side)
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

	std::vector<Line> ret;
	BOOST_FOREACH(auto& edg,ep)
		ret.push_back(Line(edg->v1,edg->v2));
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
	for(;;)
	{
		bool no_progress=true;
		std::vector<const Edge*> edge_remove;
		BOOST_FOREACH(const auto& pair_,edgemap)
		{
			const std::set<Edge*>& edges=pair_.second;
			if (edges.size()==1)
			{
				const Edge* edge=*edges.begin();
				edge_remove.push_back(edge);
			}
		}
		BOOST_FOREACH(const Edge* e,edge_remove)
		{
			remove_edge(e);
			no_progress=false;
		}
		if (no_progress) break;
	}

}
void BooleanOp::remove_edge(const Edge* e)
{
	Vertex v1=e->v1;
	Vertex v2=e->v2;
	Vertex vs[]={v1,v2};
	BOOST_FOREACH(Vertex v,vs)
	{
		auto edgemap_it=edgemap.find(v);
		assert(edgemap_it!=edgemap.end());
		std::set<Edge*>& edges=edgemap_it->second;
		edges.erase(const_cast<Edge*>(e));
		if (edges.empty())
		{
			vertices.erase(v);
			edgemap.erase(edgemap_it);
		}
	}

	int cnt=pair2edge.erase(VertexPair(e->v1,e->v2));
	assert(cnt==1);
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
Vertex Edge::get_leftmost() const
{
	if (v1.x<v2.x || (v1.x==v2.x && v1.y<v2.y))
		return v1;
	return v2;
}

class AreaSorter
{
    public:
    bool operator()(const Polygon& a,const Polygon& b)const
    {
        return std::abs(a.naive_area())<std::abs(b.naive_area());
    }
};

Vertex Cell::get_leftmost()
{
	Vertex leftmost;
	bool have_leftmost=false;
	BOOST_FOREACH(Edge* edge,edges)
	{
		if (!have_leftmost)
		{
			leftmost=edge->get_leftmost();
			have_leftmost=true;
		}
		else
		{
			Vertex l=edge->get_leftmost();
			if (l<leftmost)
				leftmost=l;
		}
	}
	if (!have_leftmost)
		throw std::runtime_error("The cell didn't have a leftmost vertex!");
	return leftmost;
}

Shape BooleanOp::step11_get_result()
{
	return result;
}
void BooleanOp::step6_determine_cell_cover()
{
	if (vertices.size()==0) throw std::runtime_error("No vertices, can't determine cell cover");
	for(;;)
	{
		Vertex leftmost;
		bool have_leftmost=false;
		BOOST_FOREACH(Cell* cell,cells)
		{
			if (cell->cover_determined) continue;
			Vertex l=cell->get_leftmost();
			if (!have_leftmost)
			{
				leftmost=l;
				have_leftmost=true;
			}
			else
			{
				if (l<leftmost)
					leftmost=l;
			}
		}
		if (!have_leftmost) break; //we're done, we've marked the cover of all cells; no more unprocessed leftmosts
		const std::set<Edge*>& edges=edgemap[leftmost];
		assert(edges.size()>0);
		const Edge* startedge=NULL;
		//Find the most vertical edge at the leftmost vertex.
		//This edge must be an outer edge.
		BOOST_FOREACH(const Edge* edge,edges)
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
		if (leftmost_edge_outer_side(Line(startedge->v1,startedge->v2)))
			sid=1; //0 = left side, 1 = right side

		Cell* curcell=startedge->side[sid];
		assert(curcell);
		std::set<const Polygon*> curpolys;
		std::set<Cell*> visited_cells;
		//printf("  ** startedge: %s\n",startedge->__repr__().c_str());


		BOOST_FOREACH(const Polygon* basepoly,tagmap)
		{
			if (basepoly->is_inside(leftmost))
			{
				if (startedge->polys.find(basepoly)==startedge->polys.end())
				{
					//printf("  ** added poly: %s\n",basepoly->__repr__().c_str());
					curpolys.insert(basepoly);
				}
			}
		}


		recurse_determine_cover(curcell,curpolys,visited_cells);
	}
}
void BooleanOp::recurse_determine_cover(Cell* curcell,std::set<const Polygon*> curpolys,std::set<Cell*>& visited)
{
	if (visited.find(curcell)!=visited.end())
		return; //already visited
	visited.insert(curcell);
	curcell->cover_determined=true;
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
	this->shape_a=shape_a;
	this->shape_b=shape_b;

	if (shape_a==NULL)
		throw std::runtime_error("Only shape_b may be NULL");
	BOOST_FOREACH(const Polygon& poly_a,shape_a->get_polys())
	{
		tagmap.push_back(&poly_a);
		BOOST_FOREACH(Line line_a,poly_a.get_lines())
		{
			line_a.add_tag(tagmap.size()-1);
			all_lines.push_back(line_a);
		}
	}
	if (shape_b!=NULL)
	{
	    BOOST_FOREACH(const Polygon& poly_b,shape_b->get_polys())
	    {
		    tagmap.push_back(&poly_b);
		    BOOST_FOREACH(Line line_b,poly_b.get_lines())
		    {
			    line_b.add_tag(tagmap.size()-1);
			    all_lines.push_back(line_b);
		    }
	    }
    }
}
void BooleanOp::step1_add_lines(Shape* shape_a)
{
    step1_add_lines(shape_a,NULL);
}

void BooleanOp::step2_intersect_lines()
{

	std::vector<Line>::iterator ita=all_lines.begin();
	std::set<Vertex> isect_vert;
	for(;ita!=all_lines.end();++ita)
	{
		std::vector<Line>::iterator itb=all_lines.begin();
		for(;itb!=all_lines.end();++itb)
		{
			if (ita==itb) continue;
			std::vector<Vertex> ret=ita->intersection_points(*itb);
			//std::cout<<"Intersected "<<ita->__repr__()<<" and "<<itb->__repr__()<<"\n";

			BOOST_FOREACH(Vertex v,ret)
			{
				//std::cout<<"Resulting in "<<v.__repr__()<<"\n";
				//std::set<Line*>& lines_of_vertex=vmap[v];
				//lines_of_vertex.insert(&*ita);
				//lines_of_vertex.insert(&*itb);
				lmap[&*ita].insert(v);
				lmap[&*itb].insert(v);
				vertices.insert(v);
				isect_vert.insert(v);
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
	BOOST_FOREACH(Vertex v,isect_vert)
	{
		//printf("Intersect vertex: %s\n",v.__repr__().c_str());
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
std::vector<Line> BooleanOp::dbg_step2_get_split_lines()
{
	std::vector<Line> out;
	BOOST_FOREACH(Line& l,all_lines)
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
			out.push_back(Line(v1,v2));
		}
	}
	return out;
}
void BooleanOp::step3_create_edges()
{
	BOOST_FOREACH(Line& l,all_lines)
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
	strat->init(shape_a,shape_b);
	BOOST_FOREACH(Cell* cell,cells)
	{
		if (cell==NULL)
			throw std::runtime_error("Unexpected error - cell was NULL in classifier");
		assert(cell->classification==UNCLASSIFIED);
		cell->classification=strat->evaluate(*cell);
	}
}
void TidyStrategy::init(Shape* a,Shape* b)
{
	shape_a=a;
}
BooleanUpResult TidyStrategy::evaluate(const Cell& cell)
{
	//printf("Evaluating cell: %s\n",cell.__repr__().c_str());
	if (cell.is_leader)
		return VOID;
	return SOLID;
}
void BooleanOrStrategy::init(Shape* a,Shape* b)
{
	shape_a=a;
	shape_b=b;
}
BooleanUpResult BooleanOrStrategy::evaluate(const Cell& cell)
{
	if (cell.cover.empty())
		return VOID;
	const Polygon* smallest_a=NULL;
	const Polygon* smallest_b=NULL;
	uint64_t smallest_a_area=(uint64_t)-1;
	uint64_t smallest_b_area=(uint64_t)-1;


	BOOST_FOREACH(const Polygon* cov,cell.cover)
	{
		uint64_t area=cov->naive_double_area();
		if (cov->get_shape()==shape_a)
		{
			if (smallest_a==NULL || area<smallest_a_area)
			{
				smallest_a=cov;
				smallest_a_area=area;
			}
		}
		if (cov->get_shape()==shape_b)
		{
			if (smallest_b==NULL || area<smallest_b_area)
			{
				smallest_b=cov;
				smallest_b_area=area;
			}
		}
	}
	bool asolid=smallest_a ? smallest_a->get_kind()==Polygon::SOLID : false;
	bool bsolid=smallest_b ? smallest_b->get_kind()==Polygon::SOLID : false;
	if (asolid || bsolid) //OR Operation
		return SOLID;
	else
		return HOLE;
}
std::string Cell::get_classification()
{
	switch(classification)
	{
	case HOLE: return "HOLE";
	case SOLID: return "SOLID";
	case VOID: return "VOID";
	case UNCLASSIFIED: return "UNCLASSIFIED";
	default:
		throw std::runtime_error("unexpected error in get_classification");
	}
	return "";
}
std::string Cell::__repr__() const
{
	std::ostringstream str;
	str<<"Cell("<<bur_tostr(classification)<<", "<<edges.size();
	if (is_leader)
		str<<" <leader>";
	if (is_enveloping())
		str<<" <envelop>";
	str<<" merged: "<<merged_poly<<", component: "<<component;
	str<<" edges, covered by: [";
	BOOST_FOREACH(const Polygon* cov,cover)
	{
		str<<" "<<cov->get_shape()->get_name();
	}
	str<<"] edges are:";
	BOOST_FOREACH(Edge* edge,edges)
	{
		str<<" "<<edge->v1.__repr__();
		str<<"-"<<edge->v2.__repr__();
	}
	str<<")";
	return str.str();
}

void BooleanOp::step5_create_cells()
{
	//Now we have a net of vertexes and edges,
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
	BOOST_FOREACH(Cell* cell,cells)
	{
		cell->polygon=new Polygon(cell->vertices);
	}
}
static void recursively_mark_component(Cell* cell,ComponentInfo& comp,int curcomp)
{
	if (cell->component==curcomp) return;
	if (cell->component!=-1)
		throw std::runtime_error("Unexpected error - encountered cell from incomplete recursive-marking op");
	cell->component=curcomp;
	if (comp.leader==NULL || cell->polygon->naive_double_area()>comp.leader->polygon->naive_double_area())
	{
		if (cell->is_enveloping())
			comp.leader=cell;
	}
	//printf("Marking component #%d: %s\n",curcomp,cell->polygon->__repr__().c_str());
	BOOST_FOREACH(Edge* edge,cell->edges)
	{
		if (edge->side[0])
			recursively_mark_component(edge->side[0],comp,curcomp);
		if (edge->side[1])
			recursively_mark_component(edge->side[1],comp,curcomp);
	}
}
static void mark_components(std::vector<ComponentInfo>& components,std::vector<Cell*> cells)
{
	BOOST_FOREACH(Cell* cell,cells)
	{
		if (cell->component!=-1) continue;
		components.push_back(ComponentInfo());
		recursively_mark_component(cell,components.back(),components.size()-1);
		//printf("Finished component\n");
		if (components.back().leader==NULL)
			throw std::runtime_error("No cells in component?");
	}
	BOOST_FOREACH(Cell* cell,cells)
	{
		if (cell->component<0 || cell->component>=(int)components.size())
			throw std::runtime_error("Unexpected component number in cell");
		if (components[cell->component].leader==cell)
		{
			//printf("Marking cell as leader for component #%d\n",cell->component);
			cell->is_leader=true;
		}
	}
}
void BooleanOp::step5b_determine_cell_hierarchy()
{

	mark_components(components,cells);

	std::vector<Cell*> sortcells;
	sortcells=cells;
	//printf("Determine hierarchy: %d\n",(int)sortcells.size());
	std::sort(sortcells.begin(),sortcells.end(),SortCellsOnArea());
	for(int idx=0;idx<(int)sortcells.size();++idx)
	{
		Cell* curcell=sortcells[idx];
		if (curcell->vertices.size()<1) throw std::runtime_error("cell has no vertices");
		if (curcell->is_leader!=true)
			continue; //only the component leader can have a parent...

		//printf("Cell-Polygon: %s:  (area:%ld,component:#%d)\n",
		//		curcell->polygon->__repr__().c_str(),curcell->polygon->naive_area(),
		//		curcell->component);
		//this is the exterior of a polygon, or a hole.
		//these need to be connected to any enveloping polygon

		Vertex curpoint=curcell->vertices[0];
		for(int i=idx+1;i<(int)sortcells.size();++i)
		{
			Cell* outer=sortcells[i];
			assert(outer->component!=-1);
			if (outer->component!=curcell->component && outer->polygon->is_inside(curpoint))
			{//curcell is inside "outer"
				curcell->parent=outer;
				outer->children.push_back(curcell);
				//printf("Polygon of area %ld is inside that with %ld\n",
				//		curcell->polygon->naive_area(),
				//		outer->polygon->naive_area());
				break;
			}
		}
	}

}

const char* bur_tostr(BooleanUpResult r)
{
	switch(r)
	{
		case HOLE: return "HOLE";
		case SOLID: return "SOLID";
		case VOID: return "VOID";
		case UNCLASSIFIED: return "UNCLASSIFIED";
		default:
			return "UNKNOWN";
	}
}


void BooleanOp::step8_merge_cells()
{
	num_merged_polys=0;
	std::set<int> is_merged_poly_void;
	BOOST_FOREACH(Cell* cell,cells)
	{
		if (cell->merged_poly!=-1) continue; //Already merged
		//printf("Restart with cell %s\n",cell->__repr__().c_str());
		int cur_poly=num_merged_polys;
		++num_merged_polys;
		std::set<Cell*> merge_front;
		merge_front.insert(cell);
		while(!merge_front.empty())
		{
			std::set<Cell*> next_merge_front;
			//printf("Rerunning through merge front, %d items\n",(int)merge_front.size());
			BOOST_FOREACH(Cell* mergecell,merge_front)
			{
				//printf("Merging %s\n======================\n",
				//		mergecell->__repr__().c_str());
				if (mergecell->merged_poly!=-1) continue;
				mergecell->merged_poly=cur_poly;
				std::vector<Cell*> neighs;
				BOOST_FOREACH(auto neighitem,mergecell->neighbors)
				{
					Cell* neighcell=neighitem.first;
					neighs.push_back(neighcell);
				}
				if (mergecell->parent!=NULL)
					neighs.push_back(mergecell->parent);
				BOOST_FOREACH(Cell* child,mergecell->children)
					neighs.push_back(child);
				BOOST_FOREACH(Cell* neighcell,neighs)
				{
					//printf("Considering %s\n",neighcell->__repr__().c_str());
					if (neighcell->merged_poly==-1 && (
							(neighcell->classification==cell->classification) ||
							(neighcell->classification==HOLE && cell->classification==VOID) ||
							(neighcell->classification==VOID && cell->classification==HOLE)
							))
					{
						if (neighcell->classification==VOID || cell->classification==VOID)
						{
							assert(neighcell->classification!=SOLID);
							assert(cell->classification!=SOLID);
							is_merged_poly_void.insert(cur_poly);
						}

						//printf("Merging:\n  ->%s\n  ->%s\n",
						//		cell->__repr__().c_str(),
						//		neighcell->__repr__().c_str());
						next_merge_front.insert(neighcell);
					}
					else
					{
						//printf("Did not add it\n");
					}
				}
			}
			merge_front.swap(next_merge_front);
		}
	}
	//printf("=== All Cells after step 8 ===\n");
	BOOST_FOREACH(Cell* cell,cells)
	{
		if (is_merged_poly_void.find(cell->merged_poly)!=is_merged_poly_void.end())
		{
			cell->classification=VOID;
		}
		//printf("%s\n",cell->__repr__().c_str());
	}
	//printf("==============================\n");

}

void BooleanOp::step9_calc_result()
{
	std::vector<Polygon> res_polys;
	for(int cur_merged_poly=0;cur_merged_poly<num_merged_polys;++cur_merged_poly)
	{
		if (pair2edge.size()==0) throw std::runtime_error("No edges, can't calculate result");
		Edge* leftmost=NULL;
		//find "leftmost" edge of poly marked with 'merged_poly' == cur_merged_poly
		//printf("---------------------------------------\n");
		//printf("Merged poly: #%d: Iterate through pair2edge\n-----------------------------------------\n",cur_merged_poly);
		BooleanUpResult cur_classification=UNCLASSIFIED;
		BOOST_FOREACH(auto& eitem,pair2edge)
		{
			Edge* e=&(eitem.second);
			assert(e->side[0] && e->side[1]);
			if ((e->side[0]->merged_poly==cur_merged_poly)==
				  (e->side[1]->merged_poly==cur_merged_poly))
				continue;
			Cell* cell=NULL;
			if (e->side[0]->merged_poly==cur_merged_poly)
				cell=e->side[0];
			if (e->side[1]->merged_poly==cur_merged_poly)
				cell=e->side[1];
			if (cell->is_leader)
			{
				//printf("Merged poly #%d is leader of component #%d\n",
				//		cur_merged_poly,cell->component);
				continue; //component leaders shouldn't give polygons
			}
			assert(cell);
			if (cur_classification==UNCLASSIFIED)
				cur_classification=cell->classification;
			else
				assert(cur_classification==cell->classification);

			Vertex v=e->get_leftmost();
			//printf("Leftmost Vertex of edge: %s\n",v.__repr__().c_str());
			//printf("Value of 'leftmost' %p: %s\n",leftmost,leftmost ? leftmost->get_leftmost().__repr__().c_str() : "null");
			if (leftmost==NULL)
				leftmost=e;
			else if (v.x<leftmost->get_leftmost().x)
				leftmost=e;
			else if (v.x==leftmost->get_leftmost().x && e->line_k<leftmost->line_k)
				leftmost=e;
			//printf("Updated 'leftmost' %p: %s\n",leftmost,leftmost->get_leftmost().__repr__().c_str());
		}
		if (cur_classification==VOID)
			continue; //Make no polygon from the void
		if (leftmost==NULL) //happens if only the component leader is in the merged-poly.
			continue;
		//printf("Finished iterating through pair2edge\n-----------------------------------------\n");
		assert(leftmost!=NULL);

		//The 'leftmost' edge is here known to be
		//at the edge of a coherent body,
		//and also to be headed away on a counter clockwise
		//trip around that body.
		//printf("Left most edge selected: %s\n",leftmost->__repr__().c_str());
		//Vertex curvertex=leftmost->get_leftmost();
		//Vertex startvertex=curvertex;
		Vertex curvertex,startvertex;
		Edge* curedge=leftmost;
		if (leftmost_edge_outer_side(Line(leftmost->v1,leftmost->v2)))
		{
			startvertex=curedge->v1;
			curvertex=curedge->v1;
		}
		else
		{
			startvertex=curedge->v2;
			curvertex=curedge->v2;
		}
		std::vector<Vertex> output;
		output.push_back(curvertex);
		for(;;)
		{
			bool reversed=(curvertex==curedge->v2);
			Vertex nextvertex=(reversed) ? curedge->v1 : curedge->v2;
			if (nextvertex==startvertex)
				break;
			auto edgemap_it=edgemap.find(nextvertex);
			assert(edgemap_it!=edgemap.end());
			std::vector<Edge*> edge_candidates;
			//std::cout<<"Looking for edge candidates, next vertex: "<<nextvertex.__repr__()<<", incoming="<<curedge->v1.__repr__()<<"->"<<curedge->v2.__repr__()<<"\n";
			//std::cout<<"num cands: "<<edgemap_it->second.size()<<"\n";
			BOOST_FOREACH(Edge* candidate,edgemap_it->second)
			{

				if (candidate==curedge)
					continue;
				//std::cout<<"  Candidate="<<candidate->v1.__repr__()<<"->"<<candidate->v2.__repr__()<<"\n";
				/*
				find the most counter clockwise edge with boundary
				to cell with merged_poly_nr
				*/
				/*
				printf("    Candidate sides: %d/%d - looking for %d\n",
						candidate->side[0]->merged_poly,
						candidate->side[1]->merged_poly,
						cur_merged_poly);*/
				bool side0_is=(candidate->side[0]->merged_poly==cur_merged_poly);
				bool side1_is=(candidate->side[1]->merged_poly==cur_merged_poly);
				if (side0_is!=side1_is) //A boundary
				{
					//xprintf("")
					edge_candidates.push_back(candidate);
				}
			}
			//std::cout<<" ==========================================\n";
			if (edge_candidates.empty())
			{
				std::ostringstream s;
				s<<"  Couldn't find any matching outgoing edge from: "<<nextvertex.__repr__();
				throw std::runtime_error(s.str());
			}
			//BOOST_FOREACH(Edge* edge,edge_candidates)
			//	std::cout<<"  chosen="<<edge->v1.__repr__()<<"->"<<edge->v2.__repr__()<<"\n";
			//std::cout<<"  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n";
			EdgeSorter es(nextvertex,curedge,reversed ? 1 : 0);
			std::sort(edge_candidates.begin(),edge_candidates.end(),es); //FIXME: Don't sort, just find smallest.
			assert(edge_candidates.size()>0);
			Edge* nextedge=edge_candidates.front();
			//printf("Selected edge:%s\n",nextedge->__repr__().c_str());
			curedge=nextedge;
			curvertex=nextvertex;
			output.push_back(curvertex);
		}
		Polygon::Kind polykind=Polygon::HOLE;
		if (cur_classification==HOLE)
			polykind=Polygon::HOLE;
		else if (cur_classification==SOLID)
			polykind=Polygon::SOLID;
		else
		{
			assert(cur_classification!=VOID);
			std::ostringstream s;
			s<<"Found merged cell with invalid classification: "<<cur_classification;
			throw std::runtime_error(s.str());
		}
		//printf("Finished generating poly #%ld, outputting it\n",res_polys.size());
		Polygon poly(output,polykind);
		if (!poly.is_ccw())
		{
			throw std::runtime_error("Polygon is not ccw");
		}
		poly.merge_straight_sections();
		res_polys.push_back(poly);
	}
	result=Shape("mergeresult",res_polys);

}

}
