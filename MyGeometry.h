#include "Headers.h"
#include "functions.h"
#include "MyMath.h"

#ifndef _GEOMETRY_LIB  // флаг подключения
#define _GEOMETRY_LIB

class tochka{
public:
	long float x,y;
	tochka(long float a = 0, long float b = 0){x = a; y = b;}
	double len(){
		return lenFrom1to2(0, 0, x, y);
	}
	tochka norm(){
		return tochka(x,y)/len();
	}
	tochka operator+(tochka t)
	{
		tochka ret;
		ret.x = t.x + x;
		ret.y = t.y + y;
		return ret;
	}
	tochka operator+=(tochka t)
	{
		x += t.x;
		y += t.y;
		return *this;
	}
	tochka operator-(tochka t)
	{
		tochka ret;
		ret.x = x - t.x;
		ret.y = y - t.y;
		return ret;
	}
	tochka operator-=(tochka t)
	{
		x -= t.x;
		y -= t.y;
		return *this;
	}
	bool operator==(tochka t)
	{
		if(x == t.x && y == t.y) return true;
		else return false;
	}
	bool operator!=(tochka t)
	{
		if(x == t.x && y == t.y) return false;
		else return true;
	}
	tochka operator/(float t)
	{
		return tochka(x/t, y/t);
	}
	tochka operator*(float t)
	{
		return tochka(x*t, y*t);
	}
	tochka operator/=(float t)
	{
		x /= t;
		y /= t;
		return *this;
	}
	tochka operator*=(float t)
	{
		x *= t;
		y *= t;
		return *this;
	}
};

tochka tNull;

class tochka3D{
public:
	long float x,y,z;
	tochka3D(long float a = 0, long float b = 0, long float c = 0){x = a; y = b; z = c;}
	tochka3D(tochka a){x = a.x; y = a.y; z = 0;}
	tochka3D operator+(tochka3D t)
	{
		tochka3D ret;
		ret.x = t.x + x;
		ret.y = t.y + y;
		ret.z = t.y + z;
		return ret;
	}
	tochka3D operator+=(tochka3D t)
	{
		x += t.x;
		y += t.y;
		z += t.z;
		return *this;
	}
	tochka3D operator-(tochka3D t)
	{
		tochka3D ret;
		ret.x = x - t.x;
		ret.y = y - t.y;
		ret.z = z - t.z;
		return ret;
	}
	tochka3D operator-=(tochka3D t)
	{
		x -= t.x;
		y -= t.y;
		z -= t.z;
		return *this;
	}
	bool operator==(tochka3D t)
	{
		if(x == t.x && y == t.y && z == t.z) return true;
		else return false;
	}
	bool operator!=(tochka3D t)
	{
		if(x == t.x && y == t.y && z == t.z) return false;
		else return true;
	}
	tochka3D operator/(float t)
	{
		return tochka3D(x/t, y/t, z/t);
	}
	tochka3D operator*(float t)
	{
		return tochka3D(x*t, y*t, z*t);
	}
	tochka3D operator/=(float t)
	{
		x /= t;
		y /= t;
		z /= t;
		return *this;
	}
	tochka3D operator*=(float t)
	{
		x *= t;
		y *= t;
		z *= t;
		return *this;
	}
};

tochka3D tNull3D, tI(1,0,0), tJ(0,1,0), tK(0,0,1);

class line
{
public:
	tochka t1, t2;
	long float a,b;
	long float angle;
	bool inited,yparal;
	double len;
	line(){ inited = false;}
	void init(tochka tochka1, tochka tochka2)
	{
		inited = true;
		t1 = tochka1;
		t2 = tochka2;
		if(t1.x == t2.x)
		{
			yparal = true;
			a = 0;
			b = t1.x;
		}
		else
		{
			yparal = false;
			a = (t1.y-t2.y) / (t1.x-t2.x);
			b = t1.y - (t1.x * a);
		}
	}
	void update_angle(){
		angle = AngleToTarget(t1.x, t1.y, t2.x, t2.y);
	}
	void update_len(){
		len = lenFrom1to2(t1.x, t1.y, t2.x, t2.y);
	}
	int vhojd_tochki(tochka toch)
	{
		if(MAX(t1.x, t2.x) >= toch.x && toch.x >= MIN(t1.x, t2.x) && MAX(t1.y, t2.y) >= toch.y && toch.y >= MIN(t1.y, t2.y))
		{
			return 1;
		}
		return 0;
	}
	int check_collide(line line2, tochka &otv)
	{
		tochka toch;
		toch.x = (line2.b - b) / (a - line2.a);
		toch.y = a*toch.x+b;
		otv = toch;
		if(a == line2.a && (yparal+line2.yparal != 1)){
			return 0;
		}
		if(line2.yparal)
		{
			toch.x = line2.t1.x;
			toch.y = a*toch.x+b;
			otv.x = toch.x;
			otv.y = toch.y;
		}
		if(yparal)
		{
			toch.x = t1.x;
			toch.y = line2.a*toch.x+line2.b;
			otv.x = toch.x;
			otv.y = toch.y;
		}
		if(vhojd_tochki(toch) && line2.vhojd_tochki(toch)){
			return 1;
		}
		return 0;
	}
	int check_collide(tochka c, double r, tochka &otv1, tochka &otv2)
	{
		bool first = Collide_detect(c.x, c.y, r, t1.x, t1.y, 0), sec = Collide_detect(c.x, c.y, r, t2.x, t2.y, 0);
		if(first && sec){
			otv1 = t1;
			otv2 = t2;
			return 1;
		}
		if(!yparal){
			double ay, by, cy, disc;
			ay = a*a+1;
			by = 2*(a*(b-c.y)-c.x);
			cy = (b-c.y)*(b-c.y) + c.x*c.x - r*r;
		
			disc = by*by - 4*ay*cy;

			if(disc < 0) return 0;
			cy = -by/(2*ay);
			disc = sqrt(disc);
			disc /= 2*ay;

			otv1.x = cy - disc; otv1.y = otv1.x*a + b;
			otv2.x = cy + disc; otv2.y = otv2.x*a + b;

			if(first){
				ay = lenFrom1to2(t2.x, t2.y, otv1.x, otv1.y);
				by = lenFrom1to2(t2.x, t2.y, otv2.x, otv2.y);
				if(ay > by){
					otv1 = t1;
				}else{
					otv2 = t1;
				}
			}else if(sec){
				ay = lenFrom1to2(t1.x, t1.y, otv1.x, otv1.y);
				by = lenFrom1to2(t1.x, t1.y, otv2.x, otv2.y);
				if(ay > by){
					otv1 = t2;
				}else{
					otv2 = t2;
				}
			}else if((t1.x < otv1.x && t2.x < otv1.x) || (t1.x > otv2.x && t2.x > otv2.x)){
				return 0;
			}
		}else{
			if(abs(b-c.x) > r) return 0;
			double dh = sqrt(r*r - (b-c.x)*(b-c.x));
			otv2.x = otv1.x = b;
			otv1.y = c.y + dh;
			otv2.y = c.y - dh;
			if(first){
				if(t2.y < otv2.y){
					otv1.y = t1.y;
				}else{
					otv2.y = t1.y;
				}
			}else if(sec){
				if(t1.y < otv2.y){
					otv1.y = t2.y;
				}else{
					otv2.y = t2.y;
				}
			}else if((t1.y < otv2.y && t2.y < otv2.y) || (t1.y > otv1.y && t2.y > otv1.y)){
				return 0;
			}
		}
		return 1;
	}
	line get_perp(tochka t)
	{
		line ret;
		if(!yparal)
		{
			if(!a == 0)
			{
				int Na = -1/a;
				int Nb = t.y - Na*t.x;
				int Nx = (b - Nb) / (Na - a);
				int Ny = a*Nx + b;
				ret.init(t, tochka(Nx, Ny));
			}
			else
			{
				ret.init(t, tochka(t.x, b));
			}
		}
		else
		{
			ret.init(t, tochka(b, t.y));
		}
		return ret;
	}
	void draw(ALLEGRO_COLOR color, float thicknes)
	{
		al_draw_line(t1.x, t1.y, t2.x, t2.y, color, thicknes);
	}
};

class line3D{
public:
	tochka3D t1, t2;
	line3D(tochka3D a = tNull3D, tochka3D b = tNull3D){
		t1 = a; t2 = b;
	}
	void draw(ALLEGRO_COLOR color, float thickness){
		float k = -0.5;
		al_draw_line(t1.x+(t1.z*k),t1.y+(t1.z*k),t2.x+(t2.z*k),t2.y+(t2.z*k), color, thickness);
	}
};

class maket3D{
public:
	tochka3D *tMass;
	bool **bMatr;
	int len, kol_toch;
	void destroy(){
		if(tMass) delete tMass;
		if(bMatr){
			for(int u = 0; u < len-1; u++){
				if(bMatr[u]) delete bMatr[u];
			}
			delete bMatr;
		}
	}
	maket3D(int n = 0){
		len = n; kol_toch = 0;
		if(n){
			tMass = new tochka3D[n];
			bMatr = new bool*[n-1];
			for(int u = 0; u < n-1; u++){
				bMatr[u] = new bool[n-1];
				for(int t = 0; t < n-1; t++){
					bMatr[u][t] = false;
				}
			}
		}
	}
	maket3D(const maket3D &obj){
		len = obj.len; kol_toch = obj.kol_toch;
		if(len){
			tMass = new tochka3D[len];
			bMatr = new bool*[len-1];
			for(int u = 0; u < kol_toch; u++){
				tMass[u] = obj.tMass[u];
			}
			for(int u = 0; u < len-1; u++){
				bMatr[u] = new bool[len-1];
				for(int t = 0; t < len-1; t++){
					bMatr[u][t] = obj.bMatr[u][t];
				}
			}
		}
	}
	~maket3D(){
		destroy();
	}
	maket3D operator=(maket3D obj)
	{
		destroy();
		len = obj.len; kol_toch = obj.kol_toch;
		if(len){
			tMass = new tochka3D[len];
			bMatr = new bool*[len-1];
			for(int u = 0; u < kol_toch; u++){
				tMass[u] = obj.tMass[u];
			}
			for(int u = 0; u < len-1; u++){
				bMatr[u] = new bool[len-1];
				for(int t = 0; t < len-1; t++){
					bMatr[u][t] = obj.bMatr[u][t];
				}
			}
		}
		return *this;
	}
	void add_tochka(tochka3D t){
		if(kol_toch < len){
			tMass[kol_toch++] = t;
		}else{
			message("Error add tochka");
		}
	}
	void add_line(int n1, int n2){
		if(MAX(n1,n2) >= len || MIN(n1,n2) < 0){
			message("Error add line");
		}else{
			if(n1 != n2) bMatr[MIN(n1, n2)][MAX(n1,n2)-1] = true;
		}
	}
	void draw(ALLEGRO_COLOR color, float thicknes){
		line3D li;
		for(int u = 0; u < len-1; u++){
			for(int t = u; t < len-1; t++){
				if(bMatr[u][t]){
					li = line3D(tMass[u], tMass[t+1]);
					li.draw(color, thicknes);
				}
			}
		}
	}
	maket3D use_matr(Matr &m){
		maket3D ret(len);
		if(m.w != 4 || m.h != 4){
			message("Error use transformation");
		}else{
			Matr m2;
			m2.init(4, kol_toch);
			for(int u = 0; u < kol_toch; u++){
				m2.matr[0][u] = tMass[u].x;
				m2.matr[1][u] = tMass[u].y;
				m2.matr[2][u] = tMass[u].z;
				m2.matr[3][u] = 1;
			}
			m2 = m2*m;
			for(int u = 0; u < kol_toch; u++){
				ret.add_tochka(tochka3D(m2.matr[0][u]/m2.matr[3][u], 
									m2.matr[1][u]/m2.matr[3][u], 
									m2.matr[2][u]/m2.matr[3][u]));
			}
			for(int u = 0; u < len-1; u++){
				for(int t = 0; t < len-1; t++){
					ret.bMatr[u][t] = bMatr[u][t];
				}
			}
		}
		return ret;
	}
};


Matr translate3D(tochka3D t);
Matr scale3D(tochka3D t);
Matr rotate3D(tochka3D t1, tochka3D t2, float angle);
Matr rotate3D(tochka3D t1, float angle_x, float angle_y, float angle_z);
Matr rotate3D(float angle_x, float angle_y, float angle_z);

Matr otraj3D(float a, float b, float c, float d);
Matr otraj3D(tochka3D t1, tochka3D t2, tochka3D t3);
Matr otraj3D(bool x, bool y, bool z);

Matr translate3D(tochka3D t){
	Matr m; m.init(4,4);
	m.matr[0][3] = t.x; m.matr[1][3] = t.y; m.matr[2][3] = t.z;
	return m;
}
Matr scale3D(tochka3D t){
	Matr m; m.init(4,4);
	m.matr[0][0] = t.x;
	m.matr[1][1] = t.y;
	m.matr[2][2] = t.z;
	return m;
}
Matr rotate3D(tochka3D t1, tochka3D t2, float angle){
	tochka3D vec = t2-t1;

	vec /= sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);

	float d = sqrt(vec.y*vec.y + vec.z*vec.z);
	float cosA = (d == 0) ? (1) : (vec.z / d), sinA = (d == 0) ? (0) : (vec.y / d);
	float cosB = d, sinB = vec.x;
	Matr m1, m2;
	m1.init(4,4);
	m1.matr[0][3] = -t1.x; m1.matr[1][3] = -t1.y; m1.matr[2][3] = -t1.z; 

	m2.init(4,4);
	m2.matr[1][1] = m2.matr[2][2] = cosA;
	m2.matr[1][2] = -(m2.matr[2][1] = sinA);
	m1 = m1*m2;

	m2.init(4,4);
	m2.matr[0][0] = m2.matr[2][2] = cosB;
	m2.matr[0][2] = -(m2.matr[2][0] = sinB);
	m1 = m1*m2;

	m2.init(4,4);
	m2.matr[0][0] = m2.matr[1][1] = cos(angle);
	m2.matr[0][1] = -(m2.matr[1][0] = sin(angle));
	m1 = m1*m2;//*m1.Obr();

	m2.init(4,4);
	m2.matr[0][0] = m2.matr[2][2] = cosB;
	m2.matr[0][2] = -(m2.matr[2][0] = -sinB);
	m1 = m1*m2;

	m2.init(4,4);
	m2.matr[1][1] = m2.matr[2][2] = cosA;
	m2.matr[1][2] = -(m2.matr[2][1] = -sinA);
	m1 = m1*m2;

	m2.init(4,4);
	m2.matr[0][3] = t1.x; m2.matr[1][3] = t1.y; m2.matr[2][3] = t1.z; 
	m1 = m1*m2;

	return m1;
}
Matr rotate3D(tochka3D t1, float angle_x, float angle_y, float angle_z){
	Matr m1, m2;
	m1.init(4,4);
	m1.matr[0][3] = -t1.x; m1.matr[1][3] = -t1.y; m1.matr[2][3] = -t1.z; 

	if(angle_x != 0){
		m2.init(4,4);
		m2.matr[1][1] = m2.matr[2][2] = cos(angle_x);
		m2.matr[1][2] = -(m2.matr[2][1] = sin(angle_x));
		m1 = m1*m2;
	}if(angle_y != 0){
		m2.init(4,4);
		m2.matr[0][0] = m2.matr[2][2] = cos(angle_y);
		m2.matr[2][0] = -(m2.matr[0][2] = sin(angle_y));
		m1 = m1*m2;
	}if(angle_z != 0){
		m2.init(4,4);
		m2.matr[0][0] = m2.matr[1][1] = cos(angle_z);
		m2.matr[0][1] = -(m2.matr[1][0] = sin(angle_z));
		m1 = m1*m2;
	}

	m2.init(4,4);
	m2.matr[0][3] = t1.x; m2.matr[1][3] = t1.y; m2.matr[2][3] = t1.z; 
	m1 = m1*m2;

	return m1;
}
Matr rotate3D(float angle_x, float angle_y, float angle_z){
	Matr m1, m2;
	m1.init(4,4);
	if(angle_x != 0){
		m2.init(4,4);
		m2.matr[1][1] = m2.matr[2][2] = cos(angle_x);
		m2.matr[1][2] = -(m2.matr[2][1] = sin(angle_x));
		m1 = m1*m2;
	}if(angle_y != 0){
		m2.init(4,4);
		m2.matr[0][0] = m2.matr[2][2] = cos(angle_y);
		m2.matr[2][0] = -(m2.matr[0][2] = sin(angle_y));
		m1 = m1*m2;
	}if(angle_z != 0){
		m2.init(4,4);
		m2.matr[0][0] = m2.matr[1][1] = cos(angle_z);
		m2.matr[0][1] = -(m2.matr[1][0] = sin(angle_z));
		m1 = m1*m2;
	}

	return m1;
}

Matr otraj3D(float a, float b, float c, float dv){
	tochka3D vec(a, b, c), t1;

	vec /= sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);

	float d = sqrt(vec.y*vec.y + vec.z*vec.z);
	float cosA = vec.z / d, sinA = vec.y / d;
	float cosB = d, sinB = vec.x;
	if(a != 0){
		t1 = tochka3D(-dv/a, 0, 0);
	}else if(b != 0){
		t1 = tochka3D(0, -dv/b, 0);
	}else if(c != 0){
		t1 = tochka3D(0, 0, -dv/c);
	}else{
		message("Error otraj3D");
		Matr m1;
		m1.init(4, 4);
		return m1;
	}
	Matr m1, m2;
	m1.init(4,4);
	m1.matr[0][3] = -t1.x; m1.matr[1][3] = -t1.y; m1.matr[2][3] = -t1.z; 

	m2.init(4,4);
	m2.matr[1][1] = m2.matr[2][2] = cosA;
	m2.matr[1][2] = -(m2.matr[2][1] = sinA);
	m1 = m1*m2;

	m2.init(4,4);
	m2.matr[0][0] = m2.matr[2][2] = cosB;
	m2.matr[0][2] = -(m2.matr[2][0] = sinB);
	m1 = m1*m2;

	m2.init(4,4);
	m2.matr[2][2] = -1;
	m1 = m1*m2;//*m1.Obr();

	m2.init(4,4);
	m2.matr[0][0] = m2.matr[2][2] = cosB;
	m2.matr[0][2] = -(m2.matr[2][0] = -sinB);
	m1 = m1*m2;

	m2.init(4,4);
	m2.matr[1][1] = m2.matr[2][2] = cosA;
	m2.matr[1][2] = -(m2.matr[2][1] = -sinA);
	m1 = m1*m2;

	m2.init(4,4);
	m2.matr[0][3] = t1.x; m2.matr[1][3] = t1.y; m2.matr[2][3] = t1.z; 
	m1 = m1*m2;

	return m1;
}
Matr otraj3D(tochka3D t1, tochka3D t2, tochka3D t3){
	tochka3D per1 = t2-t1, per2 = t3-t1;
	tochka3D vec(per2.y*per1.z - per2.z*per1.y, per2.z*per1.x - per2.x*per1.z, per2.x*per1.y - per2.y*per1.x);

	vec /= sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);

	float d = sqrt(vec.y*vec.y + vec.z*vec.z);
	float cosA = vec.z / d, sinA = vec.y / d;
	float cosB = d, sinB = vec.x;
	Matr m1, m2;
	m1.init(4,4);
	m1.matr[0][3] = -t1.x; m1.matr[1][3] = -t1.y; m1.matr[2][3] = -t1.z; 

	m2.init(4,4);
	m2.matr[1][1] = m2.matr[2][2] = cosA;
	m2.matr[1][2] = -(m2.matr[2][1] = sinA);
	m1 = m1*m2;

	m2.init(4,4);
	m2.matr[0][0] = m2.matr[2][2] = cosB;
	m2.matr[0][2] = -(m2.matr[2][0] = sinB);
	m1 = m1*m2;

	m2.init(4,4);
	m2.matr[2][2] = -1;
	m1 = m1*m2;//*m1.Obr();

	m2.init(4,4);
	m2.matr[0][0] = m2.matr[2][2] = cosB;
	m2.matr[0][2] = -(m2.matr[2][0] = -sinB);
	m1 = m1*m2;

	m2.init(4,4);
	m2.matr[1][1] = m2.matr[2][2] = cosA;
	m2.matr[1][2] = -(m2.matr[2][1] = -sinA);
	m1 = m1*m2;

	m2.init(4,4);
	m2.matr[0][3] = t1.x; m2.matr[1][3] = t1.y; m2.matr[2][3] = t1.z; 
	m1 = m1*m2;

	return m1;
}
Matr otraj3D(bool x, bool y, bool z){
	Matr m1; m1.init(4,4);
	m1.matr[0][0] *= 1-2*x;
	m1.matr[1][1] *= 1-2*y;
	m1.matr[2][2] *= 1-2*z;
	return m1;
}
#endif