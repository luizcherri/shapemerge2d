import wx
class MainFrame(wx.Frame):
    def __init__(self,polys,drawer):
        wx.Frame.__init__(self,None,-1,"Lerad")
        self.Show(True)
        self.polys=polys
        self.drawer=drawer
        self.Bind(wx.EVT_PAINT,self.OnPaint)
    def OnPaint(self,dummy=None):
        self.drawer(self)

class Poly(object):
    def __init__(self,coords):
        self.coords=coords
        self.min_x=min(c[0] for c in self.coords)
        self.min_y=min(c[1] for c in self.coords)
        self.max_x=max(c[0] for c in self.coords)
        self.max_y=max(c[1] for c in self.coords)        
    def draw(self,dc,scaler):
        coords=[]
        for coord in self.coords:
            x=scaler.scale_x(coord[0])
            y=scaler.scale_y(coord[1])
            coords.append(wx.Point(x,y))        
        dc.DrawPolygon(coords)
def getcol(color):
    if color=='black':
        col=wx.Color(0,0,0)
    elif color=='white':
        col=wx.Color(255,255,255)
    elif color=='red':
        col=wx.Color(255,0,0)
    elif color=='green':
        col=wx.Color(0,255,0)
    elif color=='blue':
        col=wx.Color(0,0,255)
    elif color=='yellow':
        col=wx.Color(255,255,0)
    elif isinstance(color,tuple):
        col=wx.Color(*color)        
    else:
        raise Exception("Unknown color: "+color)            
    return col  
class Line(object):
    def __init__(self,x1,y1,x2,y2,color='black'):
        self.min_x=x1
        self.min_y=y1
        self.max_x=x2
        self.max_y=y2
        self.color=color
    def draw(self,dc,scaler):
        pen=wx.Pen(getcol(self.color))
        dc.SetPen(pen)
        dc.DrawLine(
            scaler.scale_x(self.min_x),
            scaler.scale_y(self.min_y),
            scaler.scale_x(self.max_x),
            scaler.scale_y(self.max_y))
    
class Square(object):
    def __init__(self,x1,y1,x2,y2,color='white'):
        self.min_x=x1
        self.min_y=y1
        self.max_x=x2
        self.max_y=y2
        self.color=color
    def draw(self,dc,scaler):
        brush=wx.Brush(getcol(self.color))
        dc.SetBrush(brush)
        dc.DrawRectangle(
            scaler.scale_x(self.min_x),
            scaler.scale_y(self.min_y),
            scaler.scale_x(self.max_x)-scaler.scale_x(self.min_x),
            scaler.scale_y(self.max_y)-scaler.scale_y(self.min_y))
        
    
                        
def draw_things(ps):            
    class Scaler(object):            
        def __init__(self,frame):
            self.frame=frame
            self.f=min(frame.ClientSize[0]/float(frame.max_x-frame.min_x),
                frame.ClientSize[1]/float(frame.max_y-frame.min_y))       
        def scale_x(self,v):
                return int(float(v-self.frame.min_x)*self.f)
        def scale_y(self,v):
                return self.frame.ClientSize[1]-int(float(v-self.frame.min_y)*self.f)
    myapp=wx.PySimpleApp()
    def polydraw(self):
        dc=wx.PaintDC(self)
        scaler=Scaler(self)
        gridsize=15
        g=gridsize
        for l in xrange(-g,g):
            #vert line
            dc.DrawLine(
                scaler.scale_x(l),
                scaler.scale_y(-g),
                scaler.scale_x(l),
                scaler.scale_y(g))
            #horiz line
            dc.DrawLine(
                scaler.scale_x(-g),
                scaler.scale_y(l),
                scaler.scale_x(g),
                scaler.scale_y(l))

        for fat in xrange(-4,4):
            dc.DrawLine(
                scaler.scale_x(-g),
                scaler.scale_y(0)+fat,
                scaler.scale_x(g),
                scaler.scale_y(0)+fat)
            dc.DrawLine(
                scaler.scale_x(0)+fat,
                scaler.scale_y(-g),
                scaler.scale_x(0)+fat,
                scaler.scale_y(g))
            
        for p in self.polys:
            p.draw(dc,scaler)
            
    m=MainFrame(ps,polydraw)
    m.min_x=1e30
    m.min_y=1e30
    m.max_x=-1e30
    m.max_y=-1e30
    for p in ps:
        #print "A poly:",p
        m.min_x=min(m.min_x,p.min_x)
        m.min_y=min(m.min_y,p.min_y)
        m.max_x=max(m.max_x,p.max_x)
        m.max_y=max(m.max_y,p.max_y)
    m.min_x-=abs(m.max_x-m.min_x)*0.1
    m.min_y-=abs(m.max_y-m.min_y)*0.1
    m.max_x+=abs(m.max_x-m.min_x)*0.1
    m.max_y+=abs(m.max_y-m.min_y)*0.1
        
    myapp.MainLoop()
                
        
if __name__=="__main__":
    draw_things([
            Poly([(100,100),
            (200,100),
            (200,200)]),
            Poly([(200,200),
            (300,200),
            (300,300)]),
            Square(50,200,100,250),
            Line(0,200,200,0)
            ]
        )


