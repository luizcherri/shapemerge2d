from pyshapemerge2d import Line2,Vertex,Vector,BooleanOp,Polygon,vvector,Shape,lvector,BooleanOrStrategy

from visualize import draw_things
import visualize
from math import sin,cos
from itertools import izip,count

vorig=Vertex
def test_edge_sort():
    b=BooleanOp()
    v=vorig(0,0)
    for x in xrange(100):
        if x==0:
            ang=0
        else:
            ang=x/6.0
        def Vertex(a,b):
            x=a*cos(ang)-b*sin(ang)
            y=b*cos(ang)+a*sin(ang)
            return vorig(int(x),int(y))
        for side in [0,1]: 
            l=Line2(Vertex(-10,0),Vertex(0,0))
            tosort=lvector([
                    Line2(Vertex(0,0),Vertex(-10,10)),
                    Line2(Vertex(0,0),Vertex(0,10)),
                    Line2(Vertex(0,0),Vertex(10,10))])
            out=list(b.dbg_step5_sort_edges(v,l,tosort,side))
            if side==0:
                assert out[0]==Line2(Vertex(0,0),Vertex(-10,10))
                assert out[1]==Line2(Vertex(0,0),Vertex(0,10))
                assert out[2]==Line2(Vertex(0,0),Vertex(10,10))
            else:
                assert out[2]==Line2(Vertex(0,0),Vertex(-10,10))
                assert out[1]==Line2(Vertex(0,0),Vertex(0,10))
                assert out[0]==Line2(Vertex(0,0),Vertex(10,10))
                
            tosort=lvector([
                    Line2(Vertex(0,0),Vertex(-10,10)),
                    Line2(Vertex(0,10),Vertex(0,0)),
                    Line2(Vertex(0,0),Vertex(10,10))])
            out=list(b.dbg_step5_sort_edges(v,l,tosort,side))
            if side==0:
                assert out[0]==Line2(Vertex(0,0),Vertex(-10,10))
                assert out[1]==Line2(Vertex(0,10),Vertex(0,0))
                assert out[2]==Line2(Vertex(0,0),Vertex(10,10))
            else:
                assert out[2]==Line2(Vertex(0,0),Vertex(-10,10))
                assert out[1]==Line2(Vertex(0,10),Vertex(0,0))
                assert out[0]==Line2(Vertex(0,0),Vertex(10,10))
                
            tosort=lvector([
                    Line2(Vertex(-10,10),Vertex(0,0)),
                    Line2(Vertex(0,10),Vertex(0,0)),
                    Line2(Vertex(0,0),Vertex(10,10))])
            out=list(b.dbg_step5_sort_edges(v,l,tosort,side))
            if side==0:
                assert out[0]==Line2(Vertex(-10,10),Vertex(0,0))
                assert out[1]==Line2(Vertex(0,10),Vertex(0,0))
                assert out[2]==Line2(Vertex(0,0),Vertex(10,10))
            else:
                assert out[2]==Line2(Vertex(-10,10),Vertex(0,0))
                assert out[1]==Line2(Vertex(0,10),Vertex(0,0))
                assert out[0]==Line2(Vertex(0,0),Vertex(10,10))
                

class NondirLine(object):
    def __init__(self,v1,v2):
        self.v1,self.v2=v1,v2
    def __hash__(self):
        return self.v1.get_x()+self.v2.get_x()+\
                self.v1.get_y()+self.v2.get_y()
    def __eq__(self,o):
        if self.v1==o.v1 and self.v2==o.v2:
            return True
        if self.v1==o.v2 and self.v2==o.v1:
            return True
        return False
    def get_v1(self):
        return self.v1
    def get_v2(self):
        return self.v2
    def __repr__(self):
        return "NondirLine(%s,%s)"%(self.v1,self.v2)
    
def vistest_add_lines():
    #print "Press any key when debugger ready"
    #raw_input()
    bo=BooleanOp()
    poly_a=Polygon(vvector([
        Vertex(0,0),Vertex(2,0),
        Vertex(2,2),Vertex(0,2)]))
    poly_b=Polygon(vvector([
        Vertex(1,1),Vertex(3,1),
        Vertex(3,3),Vertex(1,3)]))
    shape_a=Shape("shape_a",poly_a)
    shape_b=Shape("shape_b",poly_b)
    #print "Shape_a,shape_b: ",shape_a,shape_b
    bo.step1_add_lines(shape_a,shape_b)
    bo.step2_intersect_lines()
    lines=list(bo.dbg_step2_get_split_lines())
    #print "The found, split lines:"
    #print "\n".join(str(x) for x in lines)
    assert len(lines)==12
    assert Line2(Vertex(0,0),Vertex(2,0)) in lines
    assert Line2(Vertex(2,0),Vertex(2,1)) in lines
    assert Line2(Vertex(2,1),Vertex(2,2)) in lines
    assert Line2(Vertex(2,2),Vertex(1,2)) in lines
    assert Line2(Vertex(1,2),Vertex(0,2)) in lines
    assert Line2(Vertex(0,2),Vertex(0,0)) in lines
    assert Line2(Vertex(1,1),Vertex(2,1)) in lines
    assert Line2(Vertex(2,1),Vertex(3,1)) in lines    
    assert Line2(Vertex(3,1),Vertex(3,3)) in lines
    assert Line2(Vertex(3,3),Vertex(1,3)) in lines
    assert Line2(Vertex(1,3),Vertex(1,2)) in lines
    assert Line2(Vertex(1,2),Vertex(1,1)) in lines
    
    bo.step3_create_edges()
    edges=bo.dbg_step3_and_4_get_edges()    
    edgelines=dict((Line2(edge.get_v1(),edge.get_v2()),edge) for edge in edges)
    
    assert set(edgelines.keys())==set(lines) 
    for key,edge in edgelines.items():
        if key.get_v1().get_y()==key.get_v2().get_y():
            assert edge.get_k_approx()==0.0
            assert edge.get_is_vertical()==False
        else:
            #vertical
            assert edge.get_k_approx()==0.0 #also for vertical but:
            assert edge.get_is_vertical()==True
    
    bo.step4_eliminate_deadends()
    edges=bo.dbg_step3_and_4_get_edges()    
    edgelines=dict((Line2(edge.get_v1(),edge.get_v2()),edge) for edge in edges)
    
    assert set(edgelines.keys())==set(lines) 
    for key,edge in edgelines.items():
        if key.get_v1().get_y()==key.get_v2().get_y():
            assert edge.get_k_approx()==0.0
            assert edge.get_is_vertical()==False
        else:
            #vertical
            assert edge.get_k_approx()==0.0 #also for vertical but:
            assert edge.get_is_vertical()==True

            
    bo.step5_create_cells()
    cells=list(bo.dbg_step5_get_cells())
    #print "Cells:\n","\n".join(str(x) for x in cells)
    def lineset(*vertices):
        s=set()
        for v1,v2 in zip(vertices,vertices[1:]+vertices[:1]):
            s.add(NondirLine(v1,v2))
        return frozenset(s)
    def lineset2(*vertices):
        s=set()
        for v1,v2 in zip(vertices,vertices[1:]+vertices[:1]):
            s.add(Line2(v1,v2))
        return frozenset(s)
            
    facit_cells={
                 lineset(Vertex(0,0),Vertex(2,0),Vertex(2,1),Vertex(3,1),
                         Vertex(3,3),Vertex(1,3),Vertex(1,2),Vertex(0,2)):"outline",
                 lineset(Vertex(1,1),Vertex(2,1),Vertex(2,2),Vertex(1,2)):"center",
                 lineset(Vertex(2,1),Vertex(3,1),Vertex(3,3),Vertex(1,3),
                         Vertex(1,2),Vertex(2,2)):"upper_right",
                 lineset(Vertex(0,0),Vertex(2,0),Vertex(2,1),Vertex(1,1),
                         Vertex(1,2),Vertex(0,2)):"lower_left"
                 }
    #print "Facit cells","\n".join(str(x) for x in facit_cells.keys())
    assert len(cells)==4
    
    for cell in cells:
        edges=cell.dbg_get_edges()
        edgelines=set(NondirLine(edge.get_v1(),edge.get_v2()) for edge in edges)
        
        if not (edgelines in facit_cells.keys()):
            
            
            sqs=[]
            for line in edgelines:
                sqs.append(visualize.Line(
                    line.get_v1().get_x(),
                    line.get_v1().get_y(),
                    line.get_v2().get_x(),
                    line.get_v2().get_y(),                       
                    (255,0,0)))
            draw_things(sqs)
    bo.step6_determine_cell_cover()

    cells=list(bo.dbg_step5_get_cells())
    for cell in cells:
        edges=cell.dbg_get_edges()
        edgelines=frozenset(NondirLine(edge.get_v1(),edge.get_v2()) for edge in edges)
        shapes=list(cell.get_shapes())
        realcellname=facit_cells[edgelines]
        facit_cell_shapes={
            'lower_left':('shape_a',),
            'center':('shape_a','shape_b'),
            'upper_right':('shape_b',),
            'outline':()
            }
        #print "Facit:",facit_cell_shapes[realcellname]
        #print "Actual:",tuple(shapes)
        assert facit_cell_shapes[realcellname]==tuple(shapes)
            
        #print "Cell: %s, shapes: %s"%(realcellname,", ".join(shapes))
    
    bas=BooleanOrStrategy()
    bo.step7_classify_cells(bas)
    cells=list(bo.dbg_step5_get_cells())
    for cell in cells:
        edges=cell.dbg_get_edges()
        edgelines=frozenset(NondirLine(edge.get_v1(),edge.get_v2()) for edge in edges)
        shapes=list(cell.get_shapes())
        realcellname=facit_cells[edgelines]
        facit_cell_kind={
            'lower_left':'SOLID',
            'center':'SOLID',
            'upper_right':'SOLID',
            'outline':'VOID'
            }
        #print "Cell %s classification: %s"%(realcellname,cell.get_classification())
        assert cell.get_classification()==facit_cell_kind[realcellname]
        
    bo.step8_merge_cells()
    cells=list(bo.dbg_step5_get_cells())
    def lookup_cellname(cell):
        edges=cell.dbg_get_edges()
        edgelines=frozenset(NondirLine(edge.get_v1(),edge.get_v2()) for edge in edges)
        shapes=list(cell.get_shapes())
        realcellname=facit_cells[edgelines]
        return realcellname
        
    for cell in cells:
        facit_cell_merged={
            'lower_left':0,
            'center':0,
            'upper_right':0,
            'outline':1
            }
        realcellname=lookup_cellname(cell)
        #print "Cell %s merged_poly: %s, class: %s"%(realcellname,cell.get_merged_poly(),cell.get_classification())
        #print "%s: Neighbors:"%realcellname," ".join(lookup_cellname(ce) for ce in cell.get_neighbors())
        assert cell.get_merged_poly()==facit_cell_merged[realcellname]
    
    bo.step9_calc_result()
    shape=bo.step9_get_result()
    polys=list(shape.get_polys())
    assert len(polys)==1
    poly,=polys
    
    "Poly: %s %s"%(poly.get_kind_str(),list(poly.get_lines()))
    assert poly.get_kind_str()=="SOLID"
    plines=frozenset(poly.get_lines())
    should=lineset2(Vertex(0,0),Vertex(2,0),Vertex(2,1),Vertex(3,1),
                         Vertex(3,3),Vertex(1,3),Vertex(1,2),Vertex(0,2))
    #print "Is: ",plines
    #print "Should: ",should
    assert plines==should
                            
    sqs=[]
    for line in plines:
        sqs.append(visualize.Line(
            line.get_v1().get_x(),
            line.get_v1().get_y(),
            line.get_v2().get_x(),
            line.get_v2().get_y(),                       
            (255,0,0)))
    draw_things(sqs)
                


def test_edge_sort():
    b=BooleanOp()
    v=vorig(0,0)
    for x in xrange(100):
        if x==0:
            ang=0
        else:
            ang=x/6.0
        def Vertex(a,b):
            x=a*cos(ang)-b*sin(ang)
            y=b*cos(ang)+a*sin(ang)
            return vorig(int(x),int(y))
        for side in [0,1]: 
            l=Line2(Vertex(-10,0),Vertex(0,0))
            tosort=lvector([
                    Line2(Vertex(0,0),Vertex(-10,10)),
                    Line2(Vertex(0,0),Vertex(0,10)),
                    Line2(Vertex(0,0),Vertex(10,10))])
            out=list(b.dbg_step5_sort_edges(v,l,tosort,side))
            if side==0:
                assert out[0]==Line2(Vertex(0,0),Vertex(-10,10))
                assert out[1]==Line2(Vertex(0,0),Vertex(0,10))
                assert out[2]==Line2(Vertex(0,0),Vertex(10,10))
            else:
                assert out[2]==Line2(Vertex(0,0),Vertex(-10,10))
                assert out[1]==Line2(Vertex(0,0),Vertex(0,10))
                assert out[0]==Line2(Vertex(0,0),Vertex(10,10))
                
            tosort=lvector([
                    Line2(Vertex(0,0),Vertex(-10,10)),
                    Line2(Vertex(0,10),Vertex(0,0)),
                    Line2(Vertex(0,0),Vertex(10,10))])
            out=list(b.dbg_step5_sort_edges(v,l,tosort,side))
            if side==0:
                assert out[0]==Line2(Vertex(0,0),Vertex(-10,10))
                assert out[1]==Line2(Vertex(0,10),Vertex(0,0))
                assert out[2]==Line2(Vertex(0,0),Vertex(10,10))
            else:
                assert out[2]==Line2(Vertex(0,0),Vertex(-10,10))
                assert out[1]==Line2(Vertex(0,10),Vertex(0,0))
                assert out[0]==Line2(Vertex(0,0),Vertex(10,10))
                
            tosort=lvector([
                    Line2(Vertex(-10,10),Vertex(0,0)),
                    Line2(Vertex(0,10),Vertex(0,0)),
                    Line2(Vertex(0,0),Vertex(10,10))])
            out=list(b.dbg_step5_sort_edges(v,l,tosort,side))
            if side==0:
                assert out[0]==Line2(Vertex(-10,10),Vertex(0,0))
                assert out[1]==Line2(Vertex(0,10),Vertex(0,0))
                assert out[2]==Line2(Vertex(0,0),Vertex(10,10))
            else:
                assert out[2]==Line2(Vertex(-10,10),Vertex(0,0))
                assert out[1]==Line2(Vertex(0,10),Vertex(0,0))
                assert out[0]==Line2(Vertex(0,0),Vertex(10,10))
                

class NondirLine(object):
    def __init__(self,v1,v2):
        self.v1,self.v2=v1,v2
    def __hash__(self):
        return self.v1.get_x()+self.v2.get_x()+\
                self.v1.get_y()+self.v2.get_y()
    def __eq__(self,o):
        if self.v1==o.v1 and self.v2==o.v2:
            return True
        if self.v1==o.v2 and self.v2==o.v1:
            return True
        return False
    def get_v1(self):
        return self.v1
    def get_v2(self):
        return self.v2
    def __repr__(self):
        return "NondirLine(%s,%s)"%(self.v1,self.v2)
def dump_cells(bo):
    for cell in list(bo.dbg_step5_get_cells()):
        sqs=[]
        for edge in list(cell.dbg_get_edges()):
            line=Line2(edge.get_v1(),edge.get_v2())
            sqs.append(visualize.Line(
                line.get_v1().get_x(),
                line.get_v1().get_y(),
                line.get_v2().get_x(),
                line.get_v2().get_y(),                       
                (255,0,0)))
        #print "Cell cover:",list(cell.get_shapes())
        #print "Cell type:",cell.get_classification()
        draw_things(sqs)
       
def merge_shapes(shape_a,shape_b,vis=False):
    bo=BooleanOp()
    print "Shape_a,shape_b: ",shape_a,shape_b
    bo.step1_add_lines(shape_a,shape_b)
    bo.step2_intersect_lines()
    
    bo.step3_create_edges()
    bo.step4_eliminate_deadends()    
    bo.step5_create_cells()
    #dump_cells(bo)
    bo.step6_determine_cell_cover()
        
    bas=BooleanOrStrategy()
    bo.step7_classify_cells(bas)
    
    bo.step8_merge_cells();
    bo.step9_calc_result();
    bo.step10_eliminate_enclosed_cells();
	
    #bo.step8_merge_cells()
    #bo.step9_calc_result()
    shape=bo.step11_get_result()
    polys=list(shape.get_polys())
    if vis:
        plines=set()
        for poly in polys:
            #print "Poly: %s %s"%(poly.get_kind_str(),list(poly.get_lines()))
            plines=plines.union(frozenset(poly.get_lines()))
        sqs=[]
        for line in plines:
            sqs.append(visualize.Line(
                line.get_v1().get_x(),
                line.get_v1().get_y(),
                line.get_v2().get_x(),
                line.get_v2().get_y(),                       
                (255,0,0)))
        draw_things(sqs)    
    return shape

def position_line(s,match):
    
    for line,ypos in izip(s.split("\n"),count()):
        for c,xpos in izip(line,count()):
            if match==c:
                if xpos<min[0]: min[0]=xpos
                if xpos>max[0]: max[0]=xpos
                if ypos<min[1]: min[1]=ypos
                if ypos>max[1]: max[1]=ypos
    return Line2(Vertex(min[0],min[1]),Vertex(max[0],max[1]))

def pyshape(name,data,kind="solid"):
    assert data.count("\t")==0
    xpos=1
    ypos=data.count("\n")
    chs=dict()
    for c in data:
        if c=='\t': raise Exception("tab not allowed")
        if c=='\n': 
            xpos=1
            ypos-=1
        else:
            if c!=' ':
                chs.setdefault(c,[]).append((xpos,ypos))
            xpos+=1
    lines=[]
    for c,poss in chs.items():
        endpoints=[]
        for pos in poss:
            cnt=0
            for aroundx in [-1,0,1]:
                for aroundy in [-1,0,1]:
                    if aroundx==0 and aroundy==0: continue
                    if (pos[0]+aroundx,pos[1]+aroundy) in poss:
                        cnt+=1
            if cnt==1:
                endpoints.append(Vertex(*pos))
        assert(len(endpoints)==2)
        lines.append((c,Line2(endpoints[0],endpoints[1])))
    lines.sort()
    #print "C,poss: ",lines
    outv=[]
    last=None
    for (n1,line1),(n2,line2) in izip(lines,lines[1:]+lines[:1]):
        if not ((line1.get_v2()-line2.get_v1()).taxilength()==1 or (line1.get_v2()-line2.get_v2()).taxilength()==1):
            line1=Line2(line1.get_v2(),line1.get_v1())            
        if (line1.get_v2()-line2.get_v1()).taxilength()==1:
            outline=Line2(line1.get_v1(),line2.get_v1())
        elif (line1.get_v2()-line2.get_v2()).taxilength()==1:
            outline=Line2(line1.get_v1(),line2.get_v2())
        else:
            raise Exception("Unexpected error - bad input-data?")
        #print "Line: %s:%s"%(n1,outline)
        outv.append(outline.get_v1())
    poly=Polygon(vvector(outv))
    if not poly.is_ccw():
        raise Exception("shouldn't be cw")
    if kind=="hole":
        poly.set_kind(Polygon.HOLE)
    shape=Shape(name,poly)
    assert shape!=None
    return shape

def asciart_put(art,x,y,ch):
    if x<0 or y<0 or x>100 or y>100: raise Exception("Too big shape for asciart")
    while len(art)<=y:
        art.append([])
    line=art[y]
    while len(line)<=x:
        line.append(" ")
    line[x]=ch
def asciart_line(art,v1,v2,ch):
    x1,y1=v1
    x2,y2=v2
    x1=int(x1)
    y1=int(y1)
    x2=int(x2)
    y2=int(y2)
    #print x1,y1,x2,y2
    if abs(x1-x2)<=1 and abs(y1-y2)<=1:
        asciart_put(art,x1,y1,ch)
        if x1!=x2 or y1!=y2:
            asciart_put(art,x2,y2,ch)
        return
    xh=(x1+x2)//2
    yh=(y1+y2)//2
    asciart_line(art,(x1,y1),(xh,yh),ch)
    asciart_line(art,(xh,yh),(x2,y2),ch)
    
    
    
    
def asciart(shape):
    c=65
    asciart=[]
    for poly in shape.get_polys():
        for line in poly.get_lines():
            v1=line.get_v1()
            v2=line.get_v2()
            asciart_line(asciart,(v1.get_x(),v1.get_y()),(v2.get_x(),v2.get_y()),chr(c))
            c+=1
            if c>ord('Z'):
                c=65
    return "\n".join(reversed(["".join(line) for line in asciart]))
    
    
    
def test_verify_shape1():
    input1=pyshape("leftbox",\
"""
DCCCCCCCCCC
D         B
D         B
D         B
D         B
D         B
AAAAAAAAAAB
""")
    input2=pyshape("rightbox",\
"""
   DCCCCCCCCCC
   D         B
   D         B
   D         B
   D         B
   D         B
   AAAAAAAAAAB
""")
    
    should_output=pyshape("resultbox",\
"""
DCCCCCCCCCCCCC
D            B
D            B
D            B
D            B
D            B
AAAAAAAAAAAAAB
""")
    real_output=merge_shapes(input1,input2,False)    
    assert should_output==real_output
    
def test_verify_shape2():
    input1=pyshape("leftbox",\
"""
DCCCCCCCCCCCC
D           B
D           B
D           B
D           B
D           B
AAAAAAAAAAAAB
""")
    input2=pyshape("rightbox",\
"""
   DCCCCCCCCCC
   D         B
   D         B
   D         B
   D         B
   D         B
   AAAAAAAAAAB
""")
    
    should_output=pyshape("resultbox",\
"""
DCCCCCCCCCCCCC
D            B
D            B
D            B
D            B
D            B
AAAAAAAAAAAAAB
""")
    real_output=merge_shapes(input1,input2,False)    
    assert should_output==real_output


def test_verify_shape3():
    input1=pyshape("leftbox",\
"""
DCCCCCCCCCCCCC
D            B
D            B
D            B
D            B
D            B
AAAAAAAAAAAAAB
""")
    input2=pyshape("rightbox",\
"""
   DCCCCCCCCCC
   D         B
   D         B
   D         B
   D         B
   D         B
   AAAAAAAAAAB
""")
    
    should_output=pyshape("resultbox",\
"""
DCCCCCCCCCCCCC
D            B
D            B
D            B
D            B
D            B
AAAAAAAAAAAAAB
""")
    real_output=merge_shapes(input1,input2,False)    
    assert should_output==real_output


def test_verify_shape4():
    input1=pyshape("leftbox",\
"""
FEEEEEEEEEEEEEEEEEEE
F                  D
F                  D
F            CCCCCCD
F            B
F            B
AAAAAAAAAAAAAB
""")
    input2=pyshape("rightbox",\
"""
   DCCCCCCCCCC
   D         B
   D         B
   D         B
   D         B
   D         B
   AAAAAAAAAAB
""")
    
    should_output=pyshape("resultbox",\
"""
FEEEEEEEEEEEEEEEEEEE
F                  D
F                  D
F            CCCCCCD
F            B
F            B
AAAAAAAAAAAAAB
""")
    real_output=merge_shapes(input1,input2,False)    
    assert should_output==real_output


def test_verify_shape5():
    input1=pyshape("leftbox",\
"""
FEEEEEEEEEEEEEEEEEEE
F                  D
F                  D
F            CCCCCCD
F            B
F            B
AAAAAAAAAAAAAB
""")
    input2=pyshape("rightbox",\
"""
   DCCCCCCCCCC
   D         B
   D         B
   D         B
   D         B
   D         B
   D         B
   D         B
   AAAAAAAAAAB
""")
    
    should_output=pyshape("resultbox",\
"""
   HGGGGGGGGGG
   H         F
JIII         FEEEEEE
J                  D
J                  D
J            CCCCCCD
J            B
J            B
AAAAAAAAAAAAAB
""")
    real_output=merge_shapes(input1,input2,False)    
    assert should_output==real_output


def test_verify_shape6():
    input1=pyshape("leftbox",\
"""
FEEEEEEEEEEEEEEEEEEE
F                  D
F                  D
F            CCCCCCD
F            B
F            B
AAAAAAAAAAAAAB
""")
    input2=pyshape("rightbox",\
"""
   DCCCCCC
   D     B
   D     B
   AAAAAAB
""")
    
    should_output=pyshape("resultbox",\
"""
FEEEEEEEEEEEEEEEEEEE
F                  D
F                  D
F            CCCCCCD
F            B
F            B
AAAAAAAAAAAAAB
""")
    real_output=merge_shapes(input1,input2,False)    
    assert should_output==real_output


def test_verify_shape7():
    input1=pyshape("leftbox",\
"""
FEEEEEEEEEEEEEEEEEEE
F                  D
F                  D
F            CCCCCCD
F            B
F            B
AAAAAAAAAAAAAB
""")
    input2=pyshape("rightbox",\
"""
   DCCCCCC
   D     B
   D     B
   AAAAAAB
""")
    
    should_output=pyshape("resultbox",\
"""
FEEEEEEEEEEEEEEEEEEE
F                  D
F                  D
F            CCCCCCD
F            B
F            B
AAAAAAAAAAAAAB
""")
    real_output=merge_shapes(input1,input2,False)    
    print asciart(real_output)
    assert should_output==real_output


def test_verify_shape8():
    input1=pyshape("leftbox",\
"""
FEEEEEEEEEEEEEEEEEEE
F                  D
F                  D
F            CCCCCCD
F            B
F            B
AAAAAAAAAAAAAB
""")
    input2=pyshape("rightbox",\
"""

   DCCCCCC
   D     B
   D     B
   AAAAAAB

""")
    
    should_output=pyshape("resultbox",\
"""
FEEEEEEEEEEEEEEEEEEE
F                  D
F                  D
F            CCCCCCD
F            B
F            B
AAAAAAAAAAAAAB
""")
    real_output=merge_shapes(input1,input2,False)    
    print asciart(real_output)
    assert should_output==real_output
        
        

def test_verify_shape9():
    input1=pyshape("leftbox",\
"""

 DCCCC
 D   B
 AAAAB
""")
    input2=pyshape("rightbox",\
"""
DCCCCCC
D     B
D     B
D     B
AAAAAAB
""")        
    should_output=pyshape("resultbox",\
"""
DCCCCCC
D     B
D     B
D     B
AAAAAAB
""")
    real_output=merge_shapes(input1,input2,False)    
    print asciart(real_output)
    assert should_output==real_output
    
    
def test_verify_shape10():
    input1=pyshape("leftbox",\
"""


  DCCCC
  D   B
  AAAAB

""","hole")
    input2=pyshape("rightbox",\
"""
DCCCCCCCC
D       B
D       B
D       B
D       B
D       B
AAAAAAAAB
""")        
    should_art=\
"""
DCCCCCC
D     B
D     B
D     B
AAAAAAB
"""
    print "Merging shapes: ",input1,input2
    real_output=merge_shapes(input1,input2,False)    

    asci=asciart(real_output)
    print "============================"
    print asci
    print "============================"
    assert 0
    

def test_verify_shape11():
    input1=pyshape("leftbox",\
"""
HGGGGGGGG
H       F
H EEEEEEF
H D
H DCCCCCC
H       B
AAAAAAAAB
""","hole")
    
    input2=pyshape("rightbox",\
"""


      DCCC
      D  B
      AAAB                      


""")        
troubleshoot this - why is it wrong?
    print "Merging shapes: ",input1,input2
    real_output=merge_shapes(input1,input2,False)    
    asci=asciart(real_output)
    print "============================"
    print asci
    print "============================"
    assert 0

    
def vistest_merge_shapes():    
    for x in xrange(10):
        x=5
        poly_a=Polygon(vvector([
            Vertex(0+x,0),Vertex(2+x,0),
            Vertex(2+x,2),Vertex(0+x,2)]))
        poly_b=Polygon(vvector([
            Vertex(1,1),Vertex(3,1),
            Vertex(3,4),Vertex(1,4)]))
        shape_a=Shape("shape_a",poly_a)
        shape_b=Shape("shape_b",poly_b)
        #   OOOO
        #   OOOO
        # OOOOOO
        # OOOO
        # OOOO
        bo=BooleanOp()
        #print "Shape_a,shape_b: ",shape_a,shape_b
        bo.step1_add_lines(shape_a,shape_b)
        bo.step2_intersect_lines()
        splitset=set([NondirLine(x.get_v1(),x.get_v2()) for x in bo.dbg_step2_get_split_lines()])
        #print "Split result: %s"%("\n".join(str(l) for l in splitset),)
        assert not NondirLine(Vertex(0,2),Vertex(2,2)) in splitset
                
            
        bo.step3_create_edges()
        edges=list(bo.dbg_step3_and_4_get_edges())
        nondiredges=list(NondirLine(x.get_v1(),x.get_v2()) for x in bo.dbg_step3_and_4_get_edges())
         
        sqs=[]
        cnt=0
        #print "Edges: %s"%(edges,)
        for edge in edges:
            line=Line2(edge.get_v1(),edge.get_v2())
            r=(25*cnt)%128+128
            g=(128*cnt)%128+128
            b=(64*cnt)%128+128
            sqs.append(visualize.Line(
                line.get_v1().get_x(),
                line.get_v1().get_y(),
                line.get_v2().get_x(),
                line.get_v2().get_y(),                       
                (r,g,b)))
            sqs.append(visualize.Square(
                line.get_v1().get_x()-0.1,
                line.get_v1().get_y()-0.1,
                line.get_v1().get_x()+0.1,
                line.get_v1().get_y()+0.1,
                (r,g,b)))
            cnt+=1
        draw_things(sqs)
        assert not (NondirLine(Vertex(0,2),Vertex(2,2)) in nondiredges)
        
        bo.step4_eliminate_deadends()    
    
        bo.step5_create_cells()
        
        bo.step6_determine_cell_cover()    
        bas=BooleanOrStrategy()
        bo.step7_classify_cells(bas)
        for cell in list(bo.dbg_step5_get_cells()):
            sqs=[]
            for edge in list(cell.dbg_get_edges()):
                line=Line2(edge.get_v1(),edge.get_v2())
                sqs.append(visualize.Line(
                    line.get_v1().get_x(),
                    line.get_v1().get_y(),
                    line.get_v2().get_x(),
                    line.get_v2().get_y(),                       
                    (255,0,0)))
            #print "Cell cover:",list(cell.get_shapes())
            #print "Cell type:",cell.get_classification()
            draw_things(sqs)
        
        bo.step8_merge_cells()
        bo.step9_calc_result()
        shape=bo.step9_get_result()
        polys=list(shape.get_polys())
        plines=set()
        for poly in polys:
            #print "Poly: %s %s"%(poly.get_kind_str(),list(poly.get_lines()))
            plines=plines.union(frozenset(poly.get_lines()))
        sqs=[]
        edges=set(bo.dbg_step3_and_4_get_edges())
        for line in plines:
            sqs.append(visualize.Line(
                line.get_v1().get_x(),
                line.get_v1().get_y(),
                line.get_v2().get_x(),
                line.get_v2().get_y(),                       
                (0,200,0)))
        draw_things(sqs)
        
        
    
