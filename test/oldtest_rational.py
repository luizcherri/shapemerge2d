from pyshapemerge2d import Rational

def test_floor_ceil():
    Rational(5,2).ceil()==3
    Rational(5,2).floor()==2
    Rational(5,1).floor()==5
    Rational(5,1).ceil()==5
    Rational(50,10).ceil()==5
    Rational(50,10).floor()==5
    Rational(41,10).ceil()==5
    Rational(40,10).ceil()==4
    Rational(41,10).floor()==5
    Rational(49,10).floor()==4
    Rational(50,10).floor()==5
    Rational(-1,10).floor()==-1
    Rational(-10,10).floor()==-1
    Rational(-0,10).floor()==0
    Rational(-11,10).floor()==-2
    Rational(-1,10).ceil()==0
    Rational(-10,10).ceil()==-1
    Rational(-0,10).ceil()==0
    Rational(-11,10).ceil()==-1
    
def test_plus():
    assert Rational(3,2) + Rational(1,2) == Rational(4,2)    
    Rational(int a,int b);
    int floor() const;
    int ceil() const;
    Rational operator+(const Rational& o) const;
    Rational operator-(const Rational& o) const;
    Rational operator*(const Rational& o) const;
    Rational operator/(const Rational& o) const;
};
Rational operator+(const Rational& a,int b);
Rational operator-(const Rational& a,int b);
Rational operator*(const Rational& a,int b);
Rational operator/(const Rational& a,int b);
Rational operator+(int a,const Rational& b);
Rational operator-(int a,const Rational& b);
Rational operator*(int a,const Rational& b);
Rational operator/(int a,const Rational& b);
